#pragma once

void random_init(mbedtls_chacha20_context *ctx, const uint8_t key[32])
{
    mbedtls_chacha20_init(ctx);
    if (mbedtls_chacha20_setkey(ctx, key) != 0 ||
        mbedtls_chacha20_starts(ctx, (const uint8_t *)"OpenTyria-Srv", 0)) {
        // This isn't possible, the signature is only to support a common interface.
        abort();
    }
}

void random_get_bytes(mbedtls_chacha20_context *ctx, void *buffer, size_t size)
{
    static uint8_t ZEROES[512];

    for (size_t generated = 0; generated < size; ) {
        size_t to_generate = size_t_min(size, sizeof(ZEROES));
        if (mbedtls_chacha20_update(ctx, to_generate, ZEROES, buffer) != 0) {
            abort(); // Isn't possible to reach `mbedtls_chacha20_update` can't fail.
        }
        generated += to_generate;
    }
}
