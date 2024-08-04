#pragma once

#pragma pack(push, 1)
typedef struct GameSrv_PingReply {
    uint16_t header;
    uint32_t ping;
} GameSrv_PingReply;

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

typedef struct GameSrv_WeaponSet {
    uint16_t header;
    uint16_t stream_id;
    uint8_t  slot;
    uint32_t leadhand;
    uint32_t offhand;
} GameSrv_WeaponSet;

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

typedef struct GameSrv_CharCreationChangeProf {
    uint16_t header;
    uint8_t  campaign_type;
    uint8_t  profession;
} GameSrv_CharCreationChangeProf;

typedef struct GameSrv_ChangeEquippedItemColor {
    uint16_t header;
    uint8_t  equipped_item_slot;
    uint8_t  dye_color;
} GameSrv_ChangeEquippedItemColor;

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

typedef struct GameSrv_AccountFeature {
    uint16_t header;
    uint16_t feature_id;
    uint16_t param1;
    uint16_t param2;
} GameSrv_AccountFeature;

typedef struct GameSrv_KurzickMax {
    uint16_t header;
    uint32_t max_faction;
} GameSrv_KurzickMax;

typedef struct GameSrv_LuxonMax {
    uint16_t header;
    uint32_t max_faction;
} GameSrv_LuxonMax;

typedef struct GameSrv_BalthazarMax {
    uint16_t header;
    uint32_t max_faction;
} GameSrv_BalthazarMax;

typedef struct GameSrv_ImperialMax {
    uint16_t header;
    uint32_t max_faction;
} GameSrv_ImperialMax;

typedef struct GameSrv_AgentCreateAttribute {
    uint16_t header;
    uint32_t agent_id;
    uint8_t  unk1;
    uint8_t  unk2;
} GameSrv_AgentCreateAttribute;

typedef struct GameSrv_SkillbarUpdate {
    uint16_t header;
    uint32_t agent_id;
    uint32_t n_skills;
    uint32_t skills[8];
    uint32_t n_pvp_masks;
    uint32_t pvp_masks[8];
    uint8_t  unk1;
} GameSrv_SkillbarUpdate;

typedef struct GameSrv_AgentAttrUpdateInt {
    uint16_t header;
    uint32_t attr_id;
    uint32_t agent_id;
    uint32_t value;
} GameSrv_AgentAttrUpdateInt;

typedef struct GameSrv_AgentAttrUpdateFloat {
    uint16_t header;
    uint32_t attr_id;
    uint32_t agent_id;
    float    value;
} GameSrv_AgentAttrUpdateFloat;

typedef struct GameSrv_PlayerAttr {
    uint16_t header;
    uint32_t experience;
    uint32_t current_kurzick;
    uint32_t total_earned_kurzick;
    uint32_t current_luxon;
    uint32_t total_earned_luxon;
    uint32_t current_imperial;
    uint32_t total_earned_imperial;
    uint32_t unk_faction4;
    uint32_t unk_faction5;
    uint32_t level;
    uint32_t morale;
    uint32_t current_balth;
    uint32_t total_earned_balth;
    uint32_t current_skill_points;
    uint32_t total_earned_skill_points;
} GameSrv_PlayerAttr;

typedef struct GameSrv_ItemGeneralInfo {
    uint16_t header;
    uint32_t item_id;
    uint32_t file_id;
    uint8_t  item_type;
    uint8_t  unk0;
    uint16_t dye_color;
    uint16_t materials;
    uint8_t  unk1;
    uint32_t flags; // interaction
    uint32_t value;
    uint32_t model;
    uint32_t quantity;
    uint32_t n_name;
    uint16_t name[64];
    uint32_t n_modifiers;
    uint32_t modifiers[64];
} GameSrv_ItemGeneralInfo;

typedef struct GameSrv_ItemRemove {
    uint16_t header;
    uint16_t stream_id;
    uint32_t item_id;
} GameSrv_ItemRemove;

typedef struct GameSrv_ItemSetProfession {
    uint16_t header;
    uint32_t item_id;
    uint8_t  profession;
} GameSrv_ItemSetProfession;

typedef struct GameSrv_ItemMoveToLocation {
    uint16_t header;
    uint16_t stream_id;
    uint32_t item_id;
    uint16_t bag_id;
    uint8_t  slot;
} GameSrv_ItemMoveToLocation;

typedef union GameCliMsg {
    uint16_t                             header;
    uint8_t                              buffer[MSG_MAX_BUFFER_SIZE];
    GameSrv_LoadSpawnPoint               load_spawn_point;
    GameSrv_RequestPlayers               request_players;
    GameSrv_RequestItems                 request_items;
    GameSrv_CharCreationChangeProf       char_creation_change_prof;
    GameSrv_ChangeEquippedItemColor      change_equipped_item_color;
} GameCliMsg;

typedef union GameSrvMsg {
    uint16_t                             header;
    uint8_t                              buffer[MSG_MAX_BUFFER_SIZE];
    GameSrv_PingReply                    ping_reply;
    GameSrv_InstanceHead                 instance_head;
    GameSrv_InstancePlayerName           instance_player_name;
    GameSrv_InstanceInfo                 instance_info;
    GameSrv_ReadyForMapSpawn             ready_for_map_spawn;
    GameSrv_ItemStreamCreate             item_stream_create;
    GameSrv_UpdateGold                   update_gold;
    GameSrv_WeaponSet                    weapon_set;
    GameSrv_LoadSpawnPoint               load_spawn_point;
    GameSrv_UpdateProfession             update_profession;
    GameSrv_UnlockedProfession           unlocked_profession;
    GameSrv_UpdateActiveWeapon           update_active_weapon_set;
    GameSrv_InventoryCreateBag           inventory_create_bag;
    GameSrv_UnlockedSkills               unlocked_skills;
    GameSrv_UnlockedPvpHeroes            unlocked_pvp_heroes;
    GameSrv_PvpItemAddUnlock             pvp_item_add_unlock;
    GameSrv_AccountFeature               account_feature;
    GameSrv_KurzickMax                   kurzick_max;
    GameSrv_LuxonMax                     luxon_max;
    GameSrv_BalthazarMax                 balthazar_max;
    GameSrv_ImperialMax                  imperial_max;
    GameSrv_AgentCreateAttribute         agent_create_attribute;
    GameSrv_SkillbarUpdate               skillbar_update;
    GameSrv_AgentAttrUpdateInt           agent_attr_update_int;
    GameSrv_AgentAttrUpdateFloat         agent_attr_update_float;
    GameSrv_PlayerAttr                   player_attr;
    GameSrv_ItemGeneralInfo              item_general_info;
    GameSrv_ItemRemove                   item_remove;
    GameSrv_ItemSetProfession            item_set_profession;
    GameSrv_ItemMoveToLocation           item_move_to_location;
} GameSrvMsg;
#pragma pack(pop)
