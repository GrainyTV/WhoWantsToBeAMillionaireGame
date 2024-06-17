const std = @import("std");
const ArrayList = std.ArrayList;
const Allocator = std.mem.Allocator;
const OpenDirOptions = std.fs.Dir.OpenDirOptions;
const Module = std.Build.Module;
const BuildDir = std.fs.cwd();
const MAX_BUFFER_SIZE = std.math.maxInt(usize);

const CompileCommandsBuildStep = struct {
    step: std.Build.Step,
    allocator: Allocator,

    pub fn init(owner: *std.Build, allocator: Allocator) CompileCommandsBuildStep {
        return .{
            .step = std.Build.Step.init(.{
                .id = .custom,
                .name = "generate compile_commands.json for clangd",
                .owner = owner,
                .makeFn = CompileCommandsBuildStep.make,
            }),
            .allocator = allocator,
        };
    }

    pub fn make(step: *std.Build.Step, progress: *std.Progress.Node) anyerror!void {
        const self: *CompileCommandsBuildStep = @fieldParentPtr("step", step);
        _ = progress;

        const jsonFragmentFiles = try filesFromPathWithExtension(self.allocator, ".", ".json");
        var compileCommands = try BuildDir.createFile("compile_commands.json", .{});
        defer compileCommands.close();
        const writer = compileCommands.writer();

        try writer.writeAll("[\n");

        for (jsonFragmentFiles) |fragment| {
            const content = try BuildDir.readFileAlloc(self.allocator, fragment, MAX_BUFFER_SIZE);
            try writer.print("\t{s}", .{content});
            try BuildDir.deleteFile(fragment);
        }

        try writer.writeAll("]\n");
    }
};

pub fn clearBuildDirectory(step: *std.Build.Step, progress: *std.Progress.Node) anyerror!void {
    _ = step;
    _ = progress;

    try BuildDir.deleteTree("zig-cache");
    try BuildDir.deleteTree("zig-cache-global");
    BuildDir.deleteFile("compile_commands.json") catch |exception| {
        switch (exception) {
            error.FileNotFound => {},
            else => return exception,
        }
    };
}

pub fn filesFromPathWithExtension(allocator: Allocator, comptime path: []const u8, comptime extension: []const u8) anyerror![][]const u8 {
    var sourceDir = try BuildDir.openDir(path, .{
        .access_sub_paths = false,
        .iterate = true,
        .no_follow = true,
    });
    defer sourceDir.close();
    var iterator = sourceDir.iterate();
    var listOfFiles = ArrayList([]const u8).init(allocator);

    while (try iterator.next()) |file| {
        const fileExtension = std.fs.path.extension(file.name);

        if (std.mem.eql(u8, fileExtension, extension)) {
            try listOfFiles.append(try std.fs.path.join(allocator, &[_][]const u8{ path, file.name }));
        }
    }

    return listOfFiles.toOwnedSlice();
}

pub fn build(b: *std.Build) anyerror!void {
    const allocator = b.allocator;
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "a.out",
        .target = target,
        .optimize = optimize,
    });

    const libraryOptions = Module.LinkSystemLibraryOptions{
        .use_pkg_config = .no,
        .preferred_link_mode = .static,
        .search_strategy = .no_fallback,
    };

    const includePath = b.path("hdr");
    const libraryPath = b.path("lib");
    const sourceFiles = try filesFromPathWithExtension(allocator, "src", ".cpp");

    var compileCommandsStep = try allocator.create(CompileCommandsBuildStep);
    compileCommandsStep.* = CompileCommandsBuildStep.init(b, allocator);

    for (sourceFiles) |cpp| {
        const name = std.fs.path.stem(cpp);
        const obj = b.addObject(.{
            .name = name,
            .target = target,
            .optimize = optimize,
        });
        const jsonFragment = b.fmt("{s}.json", .{name});

        obj.addCSourceFile(.{ .file = b.path(cpp), .flags = &.{ "-Wall", "-std=c++23", "-MJ", jsonFragment } });
        obj.addIncludePath(includePath);
        obj.linkLibCpp();

        exe.addObject(obj);
        compileCommandsStep.step.dependOn(&obj.step);
    }

    exe.step.dependOn(&compileCommandsStep.step);
    exe.addLibraryPath(libraryPath);
    exe.linkSystemLibrary2("SDL3", libraryOptions);
    exe.linkSystemLibrary2("SDL3_image", libraryOptions);

    b.verbose = true;
    b.exe_dir = ".";
    b.cache_root = .{
        .path = "./zig-cache",
        .handle = BuildDir,
    };
    b.graph.global_cache_root = .{
        .path = "./zig-cache-global",
        .handle = BuildDir,
    };
    b.installArtifact(exe);
}
