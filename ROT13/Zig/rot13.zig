const std = @import("std");

pub const ROT13 = struct {
    input: []const u8,

    pub fn init(input: []const u8) ROT13 {
        return ROT13{ .input = input };
    }

    fn transform(input: []const u8, allocator: std.mem.Allocator) ![]u8 {
        var output = std.ArrayList(u8).init(allocator);
        defer output.deinit();

        for (input) |c| {
            if (c >= 'a' and c <= 'z') {
                const encoded = ((c - 'a' + 13) % 26) + 'a';
                try output.append(encoded);
            } else if (c >= 'A' and c <= 'Z') {
                const encoded = ((c - 'A' + 13) % 26) + 'A';
                try output.append(encoded);
            } else {
                try output.append(c);
            }
        }

        return output.toOwnedSlice();
    }

    pub fn encode(self: *const ROT13, allocator: std.mem.Allocator) ![]u8 {
        return try ROT13.transform(self.input, allocator);
    }

    pub fn decode(self: *const ROT13, encoded_version: []const u8, allocator: std.mem.Allocator) ![]u8 {
        _ = self; 
        return try ROT13.transform(encoded_version, allocator);
    }
};
