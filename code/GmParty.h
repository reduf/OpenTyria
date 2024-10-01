#pragma once

typedef enum Difficulty {
    Difficulty_Normal = 0,
    Difficulty_Hard   = 1,
} Difficulty;

typedef struct GmPartyPlayer {
    uint32_t agent_id;
    uint32_t player_id;
    bool     ready;
    bool     connected;
} GmPartyPlayer;

typedef struct GmPartyHench {
    uint32_t   agent_id;
    uint32_t   hench_id;
    uint32_t   level;
    Profession profession;
} GmPartyHench;

typedef struct GmPartyHero {
    uint32_t agent_id;
    uint32_t owner_player_id;
    uint32_t hero_id;
    uint32_t level;
} GmPartyHero;

typedef array(GmPartyHero)   GmPartyHeroArray;
typedef array(GmPartyPlayer) GmPartyPlayerArray;
typedef array(GmPartyHench)  GmPartyHenchArray;

typedef struct GmParty {
    uint32_t           party_id;
    Difficulty         difficulty;
    uint8_t            player_max;
    uint8_t            player_count;
    GmPartyHeroArray   heroes;
    GmPartyPlayerArray players;
    GmPartyHenchArray  henchmans;
} GmParty;
typedef array(GmParty) GmPartyArray;

GmParty* GameSrv_CreateParty(GameSrv *srv);
void GmParty_AddPlayer(GmParty *party, uint32_t agent_id, uint32_t player_id);
