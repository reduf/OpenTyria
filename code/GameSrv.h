#pragma once

typedef enum AdminCmd {
    AdminCmd_None,
    AdminCmd_Quit,
    AdminCmd_TransferUser,
} AdminCmd;

typedef struct AdminMsg_TransferUser {
    AdminCmd    cmd;
    SocketAddr  peer_addr;
    IoSource    source;
    uintptr_t   token;
    uint8_t     cipher_init_key[20];
    bool        reconnection;
    struct uuid account_id;
    struct uuid char_id;
} AdminMsg_TransferUser;

typedef union AdminMsg {
    AdminCmd cmd;
    union
    {
        AdminMsg_TransferUser transfer_user;
    };
} AdminMsg;
typedef array(AdminMsg) AdminMsgArray;

typedef struct GameClient {
    uint32_t    player_token;
    struct uuid account_id;
    struct uuid char_id;
} GameClient;
typedef array(GameClient) GameClientArray;

typedef struct GameConnection {
    uintptr_t             token;
    IoSource              source;
    array_uint8_t         incoming;
    array_uint8_t         outgoing;
    bool                  writable;
    arc4_context          cipher_enc;
    arc4_context          cipher_dec;
    GameSrvMsg            srv_msg;
    size_t                player_id;
} GameConnection;

typedef struct GameConnMap {
    uintptr_t      key;
    GameConnection value;
} GameConnMap;

typedef struct GamePlayerMsg {
    size_t     player_id;
    GameCliMsg msg;
} GamePlayerMsg;
typedef array(GamePlayerMsg) GamePlayerMsgArray;

typedef struct GameSrv {
    Thread             thread;
    uint32_t           map_token;
    uint16_t           map_id;
    DistrictRegion     region;
    DistrictLanguage   language;
    MapType            map_type;
    uint32_t           district_number;
    Iocp               iocp;
    Database           database;
    bool               quit_signaled;
    GameConnMap       *connections;
    array_uintptr_t    connections_with_event;
    array_uintptr_t    connections_to_remove;
    ArrayEvent         events;
    Mutex              mtx;
    AdminMsgArray      admin_messages;
    GameClientArray    clients;
    GmPlayerArray      players;
    size_t             player_count;
    GamePlayerMsgArray player_messages;
    uint64_t           last_ping_request;
    uint16_t           next_bag_id;
    GmItemArray        items;
    array_uint32_t     free_items_slots;
} GameSrv;
typedef array(GameSrv *) GameSrvArray;

int  GameSrv_Setup(GameSrv *srv);
void GameSrv_Free(GameSrv *srv);
int  GameSrv_Start(GameSrv *srv);
void GameSrv_SendAdminMsg(GameSrv *srv, AdminMsg *msg);
