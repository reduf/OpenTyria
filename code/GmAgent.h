#pragma once

typedef struct GmAgent {
    uint32_t agent_id;
    float    x;
    float    y;
    uint32_t player_id;
} GmAgent;
typedef array(GmAgent) GmAgentArray;
