typedef struct DbSession {
    struct uuid user_id;
    struct uuid session_id;
    // Timestamp created_at;
    // Timestamp updated_at;
    struct uuid account_id;
} DbSession;

typedef struct DbAccount {
    struct uuid account_id;
    // Timestamp created_at;
    // Timestamp updated_at;
    bool eula_accepted;
    struct uuid last_char_id;
    uint16_t current_territory;
} DbAccount;

typedef struct DbCharacter {
    struct uuid char_id;
    // Timestamp created_at;
    // Timestamp updated_at;
    struct uuid account_id;
    size_t      n_char_name;
    uint16_t    char_name[20];
    uint16_t    last_map_id;
} DbCharacter;

typedef array(DbCharacter) DbCharacterArray;

typedef struct AuthDb {
    sqlite3      *conn;
    sqlite3_stmt *stmt_get_session;
    sqlite3_stmt *stmt_get_account;
    sqlite3_stmt *stmt_get_characters;
    sqlite3_stmt *stmt_get_friendships;
} AuthDb;

int AuthDb_Open(AuthDb *result, const char *path);
void AuthDb_Close(AuthDb *database);

int AuthDb_GetSession(AuthDb *database, struct uuid user_id, struct uuid session_id, DbSession *result);
int AuthDb_GetOnLoginData(AuthDb *database, struct uuid account_id, DbAccount *result);
