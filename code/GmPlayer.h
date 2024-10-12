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
    uint16_t          player_id;
    uint32_t          agent_id;
    uintptr_t         conn_token;
    GmUuid            account_id;
    GmUuid            char_id;
    DbCharacter       character;
    DbAccount         account;
    GmBagArray        bags;
    Campaign          char_creation_campaign;
    Profession        primary_profession;
    uint32_t          player_team_token;
} GmPlayer;
typedef array(GmPlayer) GmPlayerArray;

GmPlayer* GameSrv_CreatePlayer(
    GameSrv *srv,
    uintptr_t token,
    GmUuid account_id,
    GmUuid char_id);

void GameSrv_RemovePlayer(GameSrv *srv, size_t player_id);
GmPlayer* GameSrv_GetPlayer(GameSrv *srv, size_t player_id);
void GameSrv_SendHardModeUnlocked(GameSrv *srv, GameConnection *conn);
void GameSrv_SendPlayerMaxFactions(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerFactions(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendInstanceLoadPlayerName(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendAgentAttributePoints(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerProfession(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendUnlockedProfessions(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendSkillbarUpdate(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendSkillsAndAttributes(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerHealthEnergy(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendUnlockedMaps(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendUnlockedSkills(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_SendPlayerAttributes(GameSrv *srv, GameConnection *conn, GmPlayer *player);
void GameSrv_BroadcastUpdatePlayerInfo(GameSrv *srv, GmPlayer *player);
