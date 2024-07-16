#pragma once

#pragma pack(push, 1)
typedef struct AccountSettings {
    uint16_t header;
    uint32_t req_id;
    uint32_t n_data;
    uint8_t  data[2048];
} AccountSettings;

typedef struct ComputerHash {
    uint16_t header;
    uint32_t version;
    uint8_t  hash[16];
} ComputerHash;

typedef struct ComputerInfo {
    uint16_t header;
    uint32_t n_username;
    uint16_t username[32];
    uint32_t n_pcname;
    uint16_t pcname[32];
} ComputerInfo;

typedef struct AskServerResponse {
    uint16_t header;
    uint32_t req_id;
} AskServerResponse;

typedef struct ClientHeartbeat {
    uint16_t header;
    uint32_t tick;
} ClientHeartbeat;

typedef struct PortalAccountLogin {
    uint16_t header;
    uint32_t req_id;
    uint8_t  user_id[16];
    uint8_t  session_id[16];
    uint32_t n_charname;
    uint16_t charname[20];
} PortalAccountLogin;

typedef struct HardwareInfo {
    uint16_t header;
    uint32_t n_info;
    uint8_t  info[92];
    uint8_t  hash[16];
} HardwareInfo;

typedef struct SetPlayerStatus {
    uint16_t header;
    uint32_t status;
} SetPlayerStatus;

typedef struct ChangeCharacter {
    uint16_t header;
    uint32_t req_id;
    uint32_t n_name;
    uint16_t name[20];
} ChangeCharacter;

typedef struct RequestGameInstance {
    uint16_t header;
    uint32_t req_id;
    uint32_t map_type;
    uint32_t map_id;
    uint32_t region;
    uint32_t district;
    uint32_t language;
} RequestGameInstance;

typedef struct Disconnect {
    uint16_t header;
    uint32_t h0002;
} Disconnect;

typedef struct AcceptEula {
    uint16_t header;
    uint8_t  value;
} AcceptEula;

typedef struct AddAccessKey {
    uint16_t header;
    uint32_t req_id;
    uint32_t n_key;
    uint16_t key[26];
} AddAccessKey;

typedef union AuthCliMsg {
    uint16_t            header;
    uint8_t             buffer[MSG_MAX_BUFFER_SIZE];
    ClientHeartbeat     heartbeat;
    ComputerHash        computer_hash;
    ComputerInfo        computer_info;
    AskServerResponse   ask_server_response;
    PortalAccountLogin  portal_account_login;
    HardwareInfo        hardware_info;
    SetPlayerStatus     set_player_status;
    ChangeCharacter     change_character;
    RequestGameInstance request_game_instance;
    Disconnect          disconnect;
    AcceptEula          accept_eula;
    AddAccessKey        add_access_key;
} AuthCliMsg;

typedef struct SessionInfo {
    uint16_t header;
    uint32_t server_salt;
    uint32_t unk0;
    uint32_t unk1;
} SessionInfo;

typedef struct RequestResponse {
    uint16_t header;
    uint32_t req_id;
    uint32_t status;
} RequestResponse;

typedef struct AccountInfo {
    uint16_t header;
    uint32_t req_id;
    uint32_t h0006;
    uint32_t h000A;
    uint8_t  h000E[8];
    uint8_t  h0016[8];
    uint8_t  account_uuid[16];
    uint8_t  character_uuid[16];
    uint32_t h003E;
    uint32_t n_unk4;
    uint8_t  unk4[200];
    uint8_t  eula_accepted;
    uint32_t unk5;
} AccountInfo;

typedef struct CharacterInfo {
    uint16_t header;
    uint32_t req_id;
    uint8_t  uuid[16];
    uint32_t unk0;
    uint32_t n_name;
    uint16_t name[20];
    uint32_t n_extended;
    uint8_t  extended[64];
} CharacterInfo;

typedef struct FriendStreamEnd {
    uint16_t header;
    uint32_t unk1;
    uint32_t unk2;
} FriendStreamEnd;

typedef struct ServerHeartbeat {
    uint16_t header;
    uint32_t tick;
} ServerHeartbeat;

typedef struct GameServerInfo {
    uint16_t header;
    uint32_t req_id;
    uint32_t map_token;
    uint32_t map_id;
    uint8_t  host[24];
    uint32_t player_token;
} GameServerInfo;

typedef union AuthSrvMsg {
    uint16_t          header;
    uint8_t           buffer[MSG_MAX_BUFFER_SIZE];
    ServerHeartbeat   heartbeat;
    AccountSettings   account_settings;
    SessionInfo       session_info;
    RequestResponse   request_response;
    AccountInfo       account_info;
    CharacterInfo     character_info;
    FriendStreamEnd   friend_stream_end;
    GameServerInfo    game_server_info;
} AuthSrvMsg;
#pragma pack(pop)

typedef array(AuthCliMsg *) AuthCliMsgArray;
