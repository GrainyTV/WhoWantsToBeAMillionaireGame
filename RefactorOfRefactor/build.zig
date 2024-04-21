const std = @import("std");
const Cache = std.Build.Cache;
const ArrayList = std.ArrayList;
const Allocator = std.mem.Allocator;
const OpenError = std.fs.Dir.OpenError;
const ArenaAllocator = std.heap.ArenaAllocator;
const OpenDirOptions = std.fs.Dir.OpenDirOptions;

pub fn createCacheFolder(directory: []const u8) !Cache.Directory
{
    const dirHandle = std.fs.cwd().openDir(directory, .{}) catch |exception| create:
    {
        switch (exception)
        {
            OpenError.FileNotFound =>
            {
                try std.fs.cwd().makeDir(directory);
                break :create try std.fs.cwd().openDir(directory, .{});
            },
            
            else => |e| return e,
        }
    };
    
    const cacheDir = Cache.Directory { 
        .path = directory, 
        .handle = dirHandle,
    };
    
    return cacheDir;
}

pub fn allSourceFilesInFolder(allocator: Allocator, directory: []const u8) ![][]const u8
{
    const openDirOptions = OpenDirOptions { 
        .access_sub_paths = false,
        .iterate = true,
        .no_follow = true, 
    };
    var sourceDir = try std.fs.cwd().openDir(directory, openDirOptions);
    defer sourceDir.close();
    
    var iterator = sourceDir.iterate();
    var listOfFiles = ArrayList([]const u8).init(allocator);

    while (try iterator.next()) |cppFile|
    {
        try listOfFiles.append(try std.fs.path.join(allocator, &[_][]const u8{ directory, cppFile.name }));
    }

    return listOfFiles.toOwnedSlice();
}

pub fn build(b: *std.Build) !void
{
    var arena = ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "Game",
        .target = target,
        .optimize = optimize,
    });

    exe.linkLibCpp();

    const includePath = b.path("hdr");
    exe.addIncludePath(includePath);
    
    const libraryPath = b.path("lib");
    exe.addLibraryPath(libraryPath);
    exe.linkSystemLibrary2("SDL3", .{ .use_pkg_config = .no, .preferred_link_mode = .static, .search_strategy = .no_fallback, });
    
    const sourceFiles = try allSourceFilesInFolder(arena.allocator(), "src");
    exe.addCSourceFiles(.{ .files = sourceFiles, .flags = &.{ "-Wall", "-std=c++23" }, });

    b.verbose = true;
    b.exe_dir = ".";
    b.cache_root = try createCacheFolder("zig-cache");
    b.graph.global_cache_root = try createCacheFolder("zig-cache-global");
    b.installArtifact(exe);
}
