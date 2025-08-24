const std = @import("std");
const ROT13 = @import("rot13.zig").ROT13;

pub fn main() !void {
    try std.fs.File.stdout().writeAll("Test of ROT13\n");
}

test "encode simple word" {
    var rot = ROT13.init("hello");
    const buf = try rot.encode(std.testing.allocator);
    defer std.testing.allocator.free(buf);
    try std.testing.expectEqualStrings("uryyb", buf);
}

test "decode simple word" {
    var rot = ROT13.init("uryyb");
    const buf = try rot.decode("uryyb", std.testing.allocator);
    defer std.testing.allocator.free(buf);
    try std.testing.expectEqualStrings("hello", buf);
}

test "encode and decode roundtrip" {
    const text = "ZigLang";
    var rot = ROT13.init(text);
    const encoded = try rot.encode(std.testing.allocator);
    defer std.testing.allocator.free(encoded);

    const decoded = try rot.decode(encoded, std.testing.allocator);
    defer std.testing.allocator.free(decoded);

    try std.testing.expectEqualStrings(text, decoded);
}

test "preserve case" {
    var rot = ROT13.init("AbCdEf");
    const buf = try rot.encode(std.testing.allocator);
    defer std.testing.allocator.free(buf);
    try std.testing.expectEqualStrings("NoPqRs", buf);
}

test "ignore spaces and punctuation" {
    var rot = ROT13.init("Hello, World!");
    const buf = try rot.encode(std.testing.allocator);
    defer std.testing.allocator.free(buf);
    try std.testing.expectEqualStrings("Uryyb, Jbeyq!", buf);
}

test "rot13 is symmetric" {
    const text = "SymmetryTest";
    var rot = ROT13.init(text);
    const encoded = try rot.encode(std.testing.allocator);
    defer std.testing.allocator.free(encoded);

    const decoded = try rot.decode(encoded, std.testing.allocator);
    defer std.testing.allocator.free(decoded);

    try std.testing.expectEqualStrings(text, decoded);
}
