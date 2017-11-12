/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2017 (git@dga.me.uk)
 */
#pragma once

#include "scene/Position.h"

namespace dw {

class DW_API OutputStream {
public:
    virtual ~OutputStream();

    /// Write an arbitrary amount of bytes to the stream.
    /// @return Number of bytes written
    virtual u32 write(const void* src, u32 size) = 0;

// Write for primitive types.
#define IMPL_PRIMITIVE_WRITE(T)          \
    virtual void write(const T& value) { \
        write(&value, sizeof(T));        \
    }

    IMPL_PRIMITIVE_WRITE(i8)
    IMPL_PRIMITIVE_WRITE(u8)
    IMPL_PRIMITIVE_WRITE(i16)
    IMPL_PRIMITIVE_WRITE(u16)
    IMPL_PRIMITIVE_WRITE(i32)
    IMPL_PRIMITIVE_WRITE(u32)
    IMPL_PRIMITIVE_WRITE(i64)
    IMPL_PRIMITIVE_WRITE(u64)
    IMPL_PRIMITIVE_WRITE(char)
    IMPL_PRIMITIVE_WRITE(bool)
    IMPL_PRIMITIVE_WRITE(float)
    IMPL_PRIMITIVE_WRITE(double)

    // Write for common types.
    virtual void write(const Vec3& v) {
        write(v.x);
        write(v.y);
        write(v.z);
    }

    virtual void write(const Position& p) {
        write(p.x);
        write(p.y);
        write(p.z);
    }

    virtual void write(const Quat& q) {
        write(q.x);
        write(q.y);
        write(q.z);
        write(q.w);
    }

    virtual void write(const String& s) {
        for (char c : s) {
            write(&c, sizeof(char));
        }
        char null_byte = '\0';
        write(&null_byte, sizeof(char));
    }
};

namespace stream {
template <typename T> void write(OutputStream& s, const T& value) {
    s.write(value);
}
}  // namespace stream
}  // namespace dw
