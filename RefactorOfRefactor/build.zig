const std = @import("std");
const Entry = std.fs.Dir.Entry;

pub fn isfileWithExt(file: *const Entry, extension: *const []const u8) bool {
    return file.kind == Entry.Kind.file and std.mem.eql(u8, extension.*, std.fs.path.extension(file.name));
}

pub fn collectFilesWithExt(allocator: std.mem.Allocator, list: *std.ArrayList([]const u8), extension: []const u8, path: []const u8) anyerror!void {
    var folder = try std.fs.cwd().openDir(path, .{
        .iterate = true,
        .no_follow = true,
    });
    defer folder.close();
    var iterator = folder.iterate();

    while (try iterator.next()) |file| {
        if (isfileWithExt(&file, &extension)) {
            try list.append(try std.fs.path.join(allocator, &.{ path, file.name }));
        }
    }
}

pub fn build(b: *std.Build) void {
    const allocator = b.allocator;
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const executable = b.addExecutable(.{
        .name = "a.out",
        .target = target,
        .optimize = optimize,
    });

    const cppVersion = "-std=c++23";
    const libDir = "lib";
    const headerDir = "hdr";

    var sources = std.ArrayList([]const u8).init(allocator);
    defer sources.deinit();
    collectFilesWithExt(allocator, &sources, ".cpp", "src") catch @panic("Failed to find source files");

    executable.linkLibCpp();
    executable.addLibraryPath(b.path(libDir));
    executable.addIncludePath(b.path(headerDir));
    executable.addCSourceFiles(.{
        .files = sources.items,
        .flags = &.{ cppVersion, "-g", "-Wall", "-Wextra", "-Wpedantic", "-Werror", "-Wno-missing-field-initializers" },
    });

    const staticLibrariesOnly = std.Build.Module.LinkSystemLibraryOptions{
        .preferred_link_mode = .static,
        .search_strategy = .no_fallback,
    };

    executable.linkSystemLibrary2("SDL3", staticLibrariesOnly);
    executable.linkSystemLibrary2("SDL3_image", staticLibrariesOnly);
    executable.linkSystemLibrary2("SDL3_mixer", staticLibrariesOnly);
    executable.linkSystemLibrary2("mpg123", staticLibrariesOnly);
    executable.linkSystemLibrary2("SDL3_ttf", staticLibrariesOnly);
    executable.linkSystemLibrary2("freetype", staticLibrariesOnly);
    executable.linkSystemLibrary2("harfbuzz", staticLibrariesOnly);

    b.exe_dir = "build/bin";
    b.installArtifact(executable);
}
