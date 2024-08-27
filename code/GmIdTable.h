#pragma once

uint32_t GmIdAllocate(array_uint32_t *free_slots, array_void_t *elements, size_t elem_size)
{
    if (free_slots->len == 0)
    {
        size_t new_size, idx;
        if (elements->len == 0) {
            new_size = 32;
            idx = 1; // we always leave the first index empty such that id 0 is default and not a real agent
        } else {
            new_size = elements->len * 2;
            idx = elements->len;
        }

        _array_resize(elements, new_size, elem_size);
        array_reserve(free_slots, elements->len - new_size);
        for (; idx < new_size; ++idx) {
            array_add(free_slots, (uint32_t) idx);
        }
    }

    return array_pop(free_slots);
}
