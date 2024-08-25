#pragma once

typedef struct slice_void_t {
    size_t       len;
    const void  *ptr;
} slice_void_t;

#define slice(T)           \
union {                    \
    slice_void_t base;     \
    struct {               \
        size_t   len;      \
        const T *ptr;      \
    };                     \
}

typedef slice(char)      slice_char_t;
typedef slice(uint8_t)   slice_uint8_t;
typedef slice(uint16_t)  slice_uint16_t;
typedef slice(uint32_t)  slice_uint32_t;
typedef slice(size_t)    slice_size_t;
typedef slice(uintptr_t) slice_uintptr_t;

#define slice_u8_from_lit(lit) { .len = sizeof(lit) - 1, .ptr = (const uint8_t *)lit }
