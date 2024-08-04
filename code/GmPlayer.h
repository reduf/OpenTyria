#pragma once

typedef struct GmPlayer {
    uint32_t     player_id;
    uintptr_t    conn_token;
    struct uuid  account_id;
    struct uuid  char_id;
    DbCharacter  character;
    GmBagArray   bags;
    CampaignType char_creation_campaign_type;
    Profession   char_creation_selected_prof;
} GmPlayer;
typedef array(GmPlayer) GmPlayerArray;
