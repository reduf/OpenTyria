#pragma once

typedef struct GmPos {
    float    x;
    float    y;
    uint16_t plane;
} GmPos;

typedef slice(GmPos) GmPosSlice;

typedef struct GmMapData {
    uint16_t     map_id;
    uint32_t     file_id;
    GmPosSlice   spawnpoints;
} GmMapData;
