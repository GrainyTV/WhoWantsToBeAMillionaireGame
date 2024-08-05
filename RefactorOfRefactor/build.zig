const std = @import("std");

pub fn build(b: *std.Build) anyerror!void {
    const allocator = b.allocator;
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const sdl3Dependency = b.dependency("sdl3", .{
        .target = target,
        .optimize = optimize,
    });

    const executable = b.addExecutable(.{
        .name = "a.out",
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(executable);
}
