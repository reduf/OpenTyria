#pragma once

typedef struct Database {
    sqlite3      *conn;
    sqlite3_stmt *stmt_get_session;
    sqlite3_stmt *stmt_get_account;
    sqlite3_stmt *stmt_get_character_and_account;
    sqlite3_stmt *stmt_get_account_characters;
    sqlite3_stmt *stmt_get_character;
    sqlite3_stmt *stmt_get_friendships;
    sqlite3_stmt *stmt_get_character_bags;
    sqlite3_stmt *stmt_insert_character;
} Database;

int Db_Open(Database *result, const char *path);
void Db_Close(Database *database);

int Db_GetSession(Database *database, struct uuid user_id, struct uuid session_id, DbSession *result);
int Db_GetAccount(Database *database, struct uuid account_id, DbAccount *result);
int Db_GetCharacter(Database *database, struct uuid account_id, struct uuid char_id, DbCharacter *result);
int Db_GetCharacters(Database *database, struct uuid account_id, DbCharacterArray *results);
int Db_CharacterBags(Database *database, struct uuid account_id, struct uuid char_id, DbBag *results, size_t count, size_t *returned);

int Db_CreateCharacter(
    Database *database,
    struct uuid account_id,
    struct uuid char_id,
    size_t n_name, const uint16_t *name,
    CharacterSettings *settings);
