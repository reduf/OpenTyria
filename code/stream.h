#pragma once

bool read_u16(const uint8_t *data, size_t size, uint16_t *result, size_t *consumed);
bool read_u32(const uint8_t *data, size_t size, uint32_t *result, size_t *consumed);
bool read_array_u8_exact(const uint8_t *data, size_t size, uint8_t *buffer, size_t buffer_size, size_t *consumed);
