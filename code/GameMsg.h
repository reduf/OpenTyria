#pragma once

#pragma pack(push, 1)
typedef struct GameSrv_InstanceHead {
    uint16_t header;
    uint8_t  b1;
    uint8_t  b2;
    uint8_t  b3;
    uint8_t  b4;
} GameSrv_InstanceHead;

typedef struct GameSrv_InstancePlayerName {
    uint16_t header;
    uint32_t n_name;
    uint16_t name[20];
} GameSrv_InstancePlayerName;

typedef struct GameSrv_InstanceInfo {
    uint16_t header;
    uint32_t agent;
    uint16_t map_id;
    uint8_t  is_explorable;
    uint32_t district;
    uint8_t  language;
    uint8_t  is_observer;
} GameSrv_InstanceInfo;

typedef struct GameSrv_InstanceLoaded {
    uint16_t header; // = GAME_SMSG_INSTANCE_LOADED
    uint32_t unk1;
} GameSrv_InstanceLoaded;

typedef struct GameSrv_ReadyForMapSpawn {
    uint16_t header;
    uint32_t unk;
} GameSrv_ReadyForMapSpawn;

typedef struct GameSrv_ItemStreamCreate {
    uint16_t header;
    uint16_t stream_id;
    uint8_t  is_hero;
} GameSrv_ItemStreamCreate;

typedef struct GameSrv_UpdateGold {
    uint16_t header;
    uint16_t stream;
    uint32_t gold;
} GameSrv_UpdateGold;

typedef struct GameSrv_LoadSpawnPoint {
    uint16_t header;
    uint32_t map_file;
    Vec2f    pos;
    uint16_t plane;
    uint8_t  unk0;
    uint8_t  is_cinematic;
    uint8_t  unknow[8];
} GameSrv_LoadSpawnPoint;

typedef struct GameSrv_RequestPlayers {
    uint16_t header;
    uint8_t  data[16];
} GameSrv_RequestPlayers;

typedef struct GameSrv_RequestItems {
    uint16_t header;
    uint8_t  unk0;
    uint8_t  unk1;
} GameSrv_RequestItems;

typedef struct GameSrv_UpdateProfession {
    uint16_t header;
    uint32_t agent_id;
    uint8_t  primary_profession;
    uint8_t  secondary_profession;
    uint8_t  is_pvp;
} GameSrv_UpdateProfession;

typedef struct GameSrv_UnlockedProf {
    uint16_t header;
    uint32_t agent_id;
    uint32_t unlocked;
} GameSrv_UnlockedProfession;

typedef struct GameSrv_UpdateActiveWeapon {
    uint16_t header;
    uint16_t stream;
    uint8_t  slot;
} GameSrv_UpdateActiveWeapon;

typedef struct GameSrv_InventoryCreateBag {
    uint16_t header;
    uint16_t stream_id;
    uint8_t  bag_type;
    uint8_t  bag_model_id;
    uint16_t bag_id;
    uint8_t  slot_count;
    uint32_t assoc_item_id;
} GameSrv_InventoryCreateBag;

typedef struct GameSrv_UnlockedSkills {
    uint16_t header;
    uint32_t n_bit_map;
    uint32_t bit_map[128];
} GameSrv_UnlockedSkills;

typedef struct GameSrv_UnlockedPvpHeroes {
    uint16_t header;
    uint32_t n_bit_map;
    uint32_t bit_map[8];
} GameSrv_UnlockedPvpHeroes;

typedef struct GameSrv_PvpItemAddUnlock {
    uint16_t header;
    uint16_t item_id;
    uint32_t n_params;
    uint32_t params[64];
} GameSrv_PvpItemAddUnlock;

typedef union GameCliMsg {
    uint16_t                     header;
    uint8_t                      buffer[MSG_MAX_BUFFER_SIZE];
    GameSrv_LoadSpawnPoint       load_spawn_point;
    GameSrv_RequestPlayers       request_players;
    GameSrv_RequestItems         request_items;
} GameCliMsg;

typedef union GameSrvMsg {
    uint16_t                     header;
    uint8_t                      buffer[MSG_MAX_BUFFER_SIZE];
    GameSrv_InstanceHead         instance_head;
    GameSrv_InstancePlayerName   instance_player_name;
    GameSrv_InstanceInfo         instance_info;
    GameSrv_ReadyForMapSpawn     ready_for_map_spawn;
    GameSrv_ItemStreamCreate     item_stream_create;
    GameSrv_UpdateGold           update_gold;
    GameSrv_LoadSpawnPoint       load_spawn_point;
    GameSrv_UpdateProfession     update_profession;
    GameSrv_UnlockedProfession   unlocked_profession;
    GameSrv_UpdateActiveWeapon   update_active_weapon_set;
    GameSrv_InventoryCreateBag   inventory_create_bag;
    GameSrv_UnlockedSkills       unlocked_skills;
    GameSrv_UnlockedPvpHeroes    unlocked_pvp_heroes;
    GameSrv_PvpItemAddUnlock     pvp_item_add_unlock;
} GameSrvMsg;
#pragma pack(pop)
