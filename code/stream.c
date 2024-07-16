#pragma once

bool read_u16(const uint8_t *data, size_t size, uint16_t *result, size_t *consumed)
{
    size_t idx = *consumed;
    if ((idx + size) < sizeof(*result)) {
        return false;
    }

    *result = le16dec(&data[idx]);
    *consumed = idx + sizeof(*result);
    return true;
}

bool read_u32(const uint8_t *data, size_t size, uint32_t *result, size_t *consumed)
{
    size_t idx = *consumed;
    if ((idx + size) < sizeof(*result)) {
        return false;
    }

    *result = le32dec(&data[idx]);
    *consumed = idx + sizeof(*result);
    return true;
}

bool read_array_u8_exact(const uint8_t *data, size_t size, uint8_t *buffer, size_t buffer_size, size_t *consumed)
{
    size_t idx = *consumed;
    if ((idx + size) < buffer_size) {
        return false;
    }

    memcpy(buffer, &data[idx], buffer_size);
    *consumed = idx + buffer_size;
    return true;
}
