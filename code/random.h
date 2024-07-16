#pragma once

void random_init(mbedtls_chacha20_context *ctx, const uint8_t key[32]);
void random_get_bytes(mbedtls_chacha20_context *ctx, void *buffer, size_t size);

