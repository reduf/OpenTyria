#pragma once

typedef enum FactionType {
    FactionType_Experience = 0,
    FactionType_KurzickCurrent = 1,
    FactionType_KurzickTotal = 2,
    FactionType_LuxonCurrent = 3,
    FactionType_LuxonTotal = 4,
    FactionType_ImperialCurrent = 5,
    FactionType_ImperialTotal = 6,
    FactionType_Value7 = 7,
    FactionType_Value8 = 8,
    FactionType_Value9 = 9,
    FactionType_Value10 = 10,
    FactionType_BalthazarCurrent = 11,
    FactionType_BalthazarTotal = 12,
    FactionType_SkillpointCurrent = 11,
    FactionType_SkillpointTotal = 12,
} FactionType;

typedef struct GmPlayer {
    uint32_t     player_id;
    uint32_t     agent_id;
    uintptr_t    conn_token;
    struct uuid  account_id;
    struct uuid  char_id;
    DbCharacter  character;
    DbAccount    account;
    GmBagArray   bags;
    CampaignType char_creation_campaign_type;
    Profession   primary_profession;
} GmPlayer;
typedef array(GmPlayer) GmPlayerArray;

GmPlayer* GameSrv_CreatePlayer(
    GameSrv *srv,
    uintptr_t token,
    struct uuid account_id,
    struct uuid char_id);

void GameSrv_RemovePlayer(GameSrv *srv, size_t player_id);
GmPlayer* GameSrv_GetPlayer(GameSrv *srv, size_t player_id);
void GameSrv_SendHardModeUnlocked(GameConnection *conn);
void GameSrv_SendPlayerMaxFactions(GameConnection *conn, GmPlayer *player);
void GameSrv_SendInstanceLoadPlayerName(GameConnection *conn, GmPlayer *player);
void GameSrv_SendAgentAttributePoints(GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerProfession(GameConnection *conn, GmPlayer *player);
void GameSrv_SendUnlockedProfessions(GameConnection *conn, GmPlayer *player);
void GameSrv_SendSkillbarUpdate(GameConnection *conn, GmPlayer *player);
void GameSrv_SendSkillsAndAttributes(GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerFactions(GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerHealthEnergy(GameConnection *conn, GmPlayer *player);
void GameSrv_SendUnlockedMaps(GameConnection *conn, GmPlayer *player);
void GameSrv_SendUnlockedSkills(GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerAttributes(GameConnection *conn, GmPlayer *player);
