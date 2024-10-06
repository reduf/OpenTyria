#pragma once

int sqlite3_column_uuid(sqlite3_stmt *stmt, int iCol, struct uuid *result)
{
    const char *text_value = (const char *)sqlite3_column_text(stmt, iCol);
    if (text_value == NULL || !uuid_parse(result, text_value, strlen(text_value))) {
        return ERR_SERVER_ERROR;
    } else {
        return ERR_OK;
    }
}

int sqlite3_column_uuid_or(sqlite3_stmt *stmt, int iCol, struct uuid def, struct uuid *result)
{
    if (sqlite3_column_uuid(stmt, iCol, result) != 0) {
        *result = def;
    }
    return ERR_OK;
}

int sqlite3_column_bool(sqlite3_stmt *stmt, int iCol, bool *result)
{
    int value = sqlite3_column_int(stmt, iCol);
    if (value == 0) {
        *result = false;
    } else if (value == 1) {
        *result = true;
    } else {
        return ERR_SERVER_ERROR;
    }
    return ERR_OK;
}

int sqlite3_column_u16_array(sqlite3_stmt *stmt, int iCol, uint16_t *buffer, size_t size, size_t *ret)
{
    int err;

    if ((err = sqlite3_column_bytes(stmt, iCol)) < 0) {
        log_warn("sqlite3_column_bytes returned a value less than 0 (%d)", err);
        return ERR_UNSUCCESSFUL;
    }

    size_t length = (size_t)err / 2;
    if (size < length) {
        return ERR_BUFFER_TOO_SMALL;
    }

    const uint16_t *blob;
    if ((blob = sqlite3_column_blob(stmt, iCol)) == NULL) {
        *ret = 0;
        return ERR_OK;
    }

    *ret = length;
    memcpy_u16(buffer, blob, length);
    return ERR_OK;
}

int sqlite3_column_u32_array(sqlite3_stmt *stmt, int iCol, uint32_t *buffer, size_t size, size_t *ret)
{
    int err;

    if ((err = sqlite3_column_bytes(stmt, iCol)) < 0) {
        log_warn("sqlite3_column_bytes returned a value less than 0 (%d)", err);
        return ERR_UNSUCCESSFUL;
    }

    size_t length = (size_t)err / 4;
    if (size < length) {
        return ERR_BUFFER_TOO_SMALL;
    }

    const uint32_t *blob;
    if ((blob = sqlite3_column_blob(stmt, iCol)) == NULL) {
        *ret = 0;
        return ERR_OK;
    }

    *ret = length;
    memcpy_u32(buffer, blob, length);
    return ERR_OK;
}

int sqlite3_column_u8_array(sqlite3_stmt *stmt, int iCol, uint8_t *buffer, size_t size, size_t *ret)
{
    int err;

    if ((err = sqlite3_column_bytes(stmt, iCol)) < 0) {
        log_warn("sqlite3_column_bytes returned a value less than 0 (%d)", err);
        return ERR_UNSUCCESSFUL;
    }

    size_t length = (size_t)err;
    if (size < length) {
        return ERR_BUFFER_TOO_SMALL;
    }

    const uint8_t *blob;
    if ((blob = sqlite3_column_blob(stmt, iCol)) == NULL) {
        *ret = 0;
        return ERR_OK;
    }

    *ret = length;
    memcpy(buffer, blob, length);
    return ERR_OK;
}

int sqlite3_column_i64(sqlite3_stmt *stmt, int iCol, int64_t *result)
{
    *result = sqlite3_column_int64(stmt, iCol);
    return ERR_OK;
}

int sqlite3_column_u32(sqlite3_stmt *stmt, int iCol, uint32_t *result)
{
    int64_t value = sqlite3_column_int64(stmt, iCol);
    if (value < 0 || (int64_t)UINT32_MAX < value) {
        return ERR_SERVER_ERROR;
    }
    *result = (uint32_t)value;
    return ERR_OK;
}

int sqlite3_column_u16(sqlite3_stmt *stmt, int iCol, uint16_t *result)
{
    int value = sqlite3_column_int(stmt, iCol);
    if (value < 0 || UINT16_MAX < value) {
        return ERR_SERVER_ERROR;
    }
    *result = (uint16_t)value;
    return ERR_OK;
}

int sqlite3_column_u8(sqlite3_stmt *stmt, int iCol, uint8_t *result)
{
    int value = sqlite3_column_int(stmt, iCol);
    if (value < 0 || UINT8_MAX < value) {
        return ERR_SERVER_ERROR;
    }
    *result = (uint8_t)value;
    return ERR_OK;
}

int sqlite3_bind_uuid(sqlite3_stmt *stmt, int iCol, struct uuid uid)
{
    char buffer[UUID_STRING_LEN + 1];
    uuid_snprint(buffer, sizeof(buffer), &uid);
    return sqlite3_bind_text(stmt, iCol, buffer, UUID_STRING_LEN, SQLITE_TRANSIENT);
}

int sqlite3_bind_u8(sqlite3_stmt *stmt, int iCol, uint8_t val)
{
    return sqlite3_bind_int(stmt, iCol, (int)val);
}

int sqlite3_bind_u16(sqlite3_stmt *stmt, int iCol, uint16_t val)
{
    return sqlite3_bind_int(stmt, iCol, (int)val);
}

int sqlite3_bind_u32(sqlite3_stmt *stmt, int iCol, uint32_t val)
{
    return sqlite3_bind_int64(stmt, iCol, (int64_t)val);
}

int sqlite3_bind_u16_array(sqlite3_stmt *stmt, int iCol, size_t len, const uint16_t *ptr)
{
    return sqlite3_bind_blob64(stmt, iCol, ptr, len * 2, NULL);
}

int sqlite3_bind_u8_array(sqlite3_stmt *stmt, int iCol, size_t len, const uint8_t *ptr)
{
    return sqlite3_bind_blob64(stmt, iCol, ptr, len, NULL);
}

void append_fields(array_char_t *builder, const char *table, const char **fields, size_t count)
{
    for (size_t idx = 0; idx < count; ++idx) {
        if (idx == count - 1) {
            appendf(builder, "%s.%s ", table, fields[idx]);
        } else {
            appendf(builder, "%s.%s, ", table, fields[idx]);
        }
    }
}

#define return_close(ret, stmt) return close_stmt_and_ret(ret, stmt)
int close_stmt_and_ret(int ret, sqlite3_stmt *stmt)
{
    sqlite3_reset(stmt);
    return ret;
}

int Db_Open(Database *result, const char *path)
{
    int err;

    const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX;

    memset(result, 0, sizeof(*result));
    if ((err = sqlite3_open_v2(path, &result->conn, flags, NULL)) != SQLITE_OK) {
        log_error("Failed to open database '%s', err: %d (%s)", path, err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }

    sqlite3 *conn = result->conn;

    const char *SQL_GET_FRIENDS = "SELECT * FROM friendships WHERE account_id = ?;";
    if ((err = sqlite3_prepare_v2(conn, SQL_GET_FRIENDS, -1, &result->stmt_select_friendships, 0)) != SQLITE_OK) {
        log_error("Failed to create the 'SQL_GET_FRIENDS' prepared statement, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }

    array_char_t builder = {0};
    appendf(&builder, "SELECT ");
    append_fields(&builder, "sessions", DbSessionColsName, ARRAY_SIZE(DbSessionColsName));
    appendf(&builder, " FROM sessions WHERE user_id = ? AND session_id = ?;");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_session, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, "accounts", DbAccountColsName, ARRAY_SIZE(DbAccountColsName));
    appendf(&builder, "FROM accounts WHERE account_id = ?;");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_account, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, "characters", DbCharacterColsName, ARRAY_SIZE(DbCharacterColsName));
    appendf(&builder, "FROM characters WHERE account_id = ?;");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_account_characters, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, "characters", DbCharacterColsName, ARRAY_SIZE(DbCharacterColsName));
    appendf(&builder, "FROM characters WHERE account_id = ? AND char_id = ?;");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_character, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, "characters", DbCharacterColsName, ARRAY_SIZE(DbCharacterColsName));
    appendf(&builder, ", ");
    append_fields(&builder, "accounts", DbAccountColsName, ARRAY_SIZE(DbAccountColsName));
    appendf(&builder, "FROM characters JOIN accounts ON accounts.account_id = characters.account_id WHERE characters.account_id = ? AND characters.char_id = ?;");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_character_and_account, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, "bags", DbBagColsName, ARRAY_SIZE(DbBagColsName));
    appendf(&builder, "FROM bags WHERE account_id = ? AND (char_id IS NULL OR char_id = ?);");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_character_bags, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    const char *sql = "INSERT INTO characters (char_id, account_id, charname, settings, unlocked_professions) VALUES (?, ?, ?, ?, ?);";
    if ((err = sqlite3_prepare_v2(conn, sql, -1, &result->stmt_insert_character, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    sql = "DELETE FROM characters WHERE account_id = ? AND char_id = ?;";
    if ((err = sqlite3_prepare_v2(conn, sql, -1, &result->stmt_delete_character, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    sql = "INSERT INTO bags (account_id, char_id, bag_model_id, bag_type, slot_count) VALUES (?, ?, ?, ?, ?);";
    if ((err = sqlite3_prepare_v2(conn, sql, -1, &result->stmt_create_bag, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    sql = "INSERT INTO items (account_id, char_id, bag_model_id, slot, quantity, dye_tint, dye_colors, model_id, file_id, flags, item_type, profession) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";
    if ((err = sqlite3_prepare_v2(conn, sql, -1, &result->stmt_create_item, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, "items", DbItemColsName, ARRAY_SIZE(DbItemColsName));
    appendf(&builder, "FROM items WHERE account_id = ? AND (char_id IS NULL OR char_id = ?);");

    if ((err = sqlite3_prepare_v2(conn, builder.ptr, (int)builder.len, &result->stmt_select_character_items, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    return ERR_OK;
exit_on_error:
    log_error(
        "Failed to create a prepared statement '%.*s', err: %d (%s)",
        (int)builder.len,
        builder.ptr,
        err,
        sqlite3_errstr(err)
    );
    array_free(&builder);
    return ERR_UNSUCCESSFUL;
}

void Db_Close(Database *database)
{
    int err;

    if ((err = sqlite3_finalize(database->stmt_select_session)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_session', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_account)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_account', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_account_characters)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_account_characters', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_character)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_account_character', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_character_and_account)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_account_character_and_account', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_friendships)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_friendships', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_character_bags)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_character_bags', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_insert_character)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_insert_character', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_delete_character)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_delete_character', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_create_bag)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_create_bag', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_create_item)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_create_item', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_select_character_items)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_select_character_items', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_close_v2(database->conn)) != SQLITE_OK) {
        log_error("Failed to close database %d (%s)", err, sqlite3_errstr(err));
    }
}

int DbCharacter_from_stmt(sqlite3_stmt *stmt, int idx, DbCharacter *result)
{
    int err;
    if (((err = sqlite3_column_uuid(stmt, idx + DbCharacterCols_char_id, &result->char_id)) != 0) ||
        ((err = sqlite3_column_i64(stmt, idx + DbCharacterCols_created_at, &result->created_at)) != 0) ||
        ((err = sqlite3_column_i64(stmt, idx + DbCharacterCols_updated_at, &result->updated_at)) != 0) ||
        ((err = sqlite3_column_uuid(stmt, idx + DbCharacterCols_account_id, &result->account_id)) != 0) ||
        ((err = sqlite3_column_u16_array(stmt, idx + DbCharacterCols_charname, result->charname.buf, ARRAY_SIZE(result->charname.buf), &result->charname.len)) != 0) ||
        ((err = sqlite3_column_u8_array(stmt, idx + DbCharacterCols_settings, result->settings.buf, ARRAY_SIZE(result->settings.buf), &result->settings.len)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill_points, &result->skill_points)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill_points_total, &result->skill_points_total)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_experience, &result->experience)) != 0) ||
        ((err = sqlite3_column_u16(stmt, idx + DbCharacterCols_gold, &result->gold)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbCharacterCols_active_weapon_set, &result->active_weapon_set)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbCharacterCols_primary_profession, &result->primary_profession)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbCharacterCols_secondary_profession, &result->secondary_profession)) != 0) ||
        ((err = sqlite3_column_u32_array(stmt, idx + DbCharacterCols_unlocked_skills, result->unlocked_skills.buf, ARRAY_SIZE(result->unlocked_skills.buf), &result->unlocked_skills.len)) != 0) ||
        ((err = sqlite3_column_u32_array(stmt, idx + DbCharacterCols_unlocked_maps, result->unlocked_maps.buf, ARRAY_SIZE(result->unlocked_maps.buf), &result->unlocked_maps.len)) != 0) ||
        ((err = sqlite3_column_u32_array(stmt, idx + DbCharacterCols_completed_missions_nm, result->completed_missions_nm.buf, ARRAY_SIZE(result->completed_missions_nm.buf), &result->completed_missions_nm.len)) != 0) ||
        ((err = sqlite3_column_u32_array(stmt, idx + DbCharacterCols_completed_bonuses_nm, result->completed_bonuses_nm.buf, ARRAY_SIZE(result->completed_bonuses_nm.buf), &result->completed_bonuses_nm.len)) != 0) ||
        ((err = sqlite3_column_u32_array(stmt, idx + DbCharacterCols_completed_missions_hm, result->completed_missions_hm.buf, ARRAY_SIZE(result->completed_missions_hm.buf), &result->completed_missions_hm.len)) != 0) ||
        ((err = sqlite3_column_u32_array(stmt, idx + DbCharacterCols_completed_bonuses_hm, result->completed_bonuses_hm.buf, ARRAY_SIZE(result->completed_bonuses_hm.buf), &result->completed_bonuses_hm.len)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_unlocked_professions, &result->unlocked_professions)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill1, &result->skill1)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill2, &result->skill2)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill3, &result->skill3)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill4, &result->skill4)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill5, &result->skill5)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill6, &result->skill6)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill7, &result->skill7)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbCharacterCols_skill8, &result->skill8)) != 0) ||
        ((err != 0))
    ) {
        return ERR_SERVER_ERROR;
    }
    return ERR_OK;
}

int DbAccount_from_stmt(sqlite3_stmt *stmt, int idx, DbAccount *result)
{
    int err;
    if (((err = sqlite3_column_uuid(stmt, idx + DbAccountCols_account_id, &result->account_id)) != 0) ||
        ((err = sqlite3_column_i64(stmt, idx + DbAccountCols_created_at, &result->created_at)) != 0) ||
        ((err = sqlite3_column_i64(stmt, idx + DbAccountCols_updated_at, &result->updated_at)) != 0) ||
        ((err = sqlite3_column_bool(stmt, idx + DbAccountCols_eula_accepted, &result->eula_accepted)) != 0) ||
        ((err = sqlite3_column_uuid(stmt, idx + DbAccountCols_current_char_id, &result->current_char_id)) != 0) ||
        ((err = sqlite3_column_u16(stmt, idx + DbAccountCols_current_territory, &result->current_territory)) != 0) ||
        ((err = sqlite3_column_u16(stmt, idx + DbAccountCols_storage_gold, &result->storage_gold)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_balthazar_points_max, &result->balthazar_points_max)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_balthazar_points_amount, &result->balthazar_points_amount)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_balthazar_points_total, &result->balthazar_points_total)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_kurzick_points_max, &result->kurzick_points_max)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_kurzick_points_amount, &result->kurzick_points_amount)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_kurzick_points_total, &result->kurzick_points_total)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_luxon_points_max, &result->luxon_points_max)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_luxon_points_amount, &result->luxon_points_amount)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_luxon_points_total, &result->luxon_points_total)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_imperial_points_max, &result->imperial_points_max)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_imperial_points_amount, &result->imperial_points_amount)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbAccountCols_imperial_points_total, &result->imperial_points_total)) != 0)
    ) {
        return ERR_SERVER_ERROR;
    }
    return ERR_OK;
}

int Db_GetSession(Database *database, struct uuid user_id, struct uuid session_id, DbSession *result)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_select_session;
    if ((err = sqlite3_bind_uuid(stmt, 1, user_id)) != SQLITE_OK) {
        log_error("Failed to bind user_id to a statement, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_bind_uuid(stmt, 2, session_id)) != SQLITE_OK) {
        log_error("Failed to bind session_id to a statement, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (((err = sqlite3_column_uuid(stmt, DbSessionCols_user_id, &result->user_id)) != 0) ||
            ((err = sqlite3_column_uuid(stmt, DbSessionCols_session_id, &result->session_id)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbSessionCols_created_at, &result->created_at)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbSessionCols_updated_at, &result->updated_at)) != 0) ||
            ((err = sqlite3_column_uuid(stmt, DbSessionCols_account_id, &result->account_id)) != 0))
        {
            return_close(ERR_UNSUCCESSFUL, stmt);
        }

        return_close(ERR_OK, stmt);
    } else if (err == SQLITE_DONE) {
        return_close(ERR_UNSUCCESSFUL, stmt);
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_GetAccount(Database *database, struct uuid account_id, DbAccount *result)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_select_account;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK) {
        log_error(
            "Failed to bind account_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (DbAccount_from_stmt(stmt, 0, result) != 0) {
            return_close(ERR_SERVER_ERROR, stmt);
        } else {
            return_close(ERR_OK, stmt);
        }
    } else if (err == SQLITE_DONE) {
        return_close(ERR_UNSUCCESSFUL, stmt);
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_GetCharacter(Database *database, struct uuid account_id, struct uuid char_id, DbCharacter *result)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_select_character;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind account_id or/and char_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        err = DbCharacter_from_stmt(stmt, 0, result);
        if (err != 0) {
            return_close(ERR_SERVER_ERROR, stmt);
        } else {
            return_close(ERR_OK, stmt);
        }
    } else if (err == SQLITE_DONE) {
        return_close(ERR_UNSUCCESSFUL, stmt);
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_GetCharacterAndAccount(
    Database *database,
    struct uuid account_id,
    struct uuid char_id,
    DbAccount *account,
    DbCharacter *character)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_select_character_and_account;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind account_id or/and char_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if ((err = DbCharacter_from_stmt(stmt, 0, character)) != 0 ||
            (err = DbAccount_from_stmt(stmt, DbCharacterCols_Count, account)) != 0)
        {
            return_close(ERR_SERVER_ERROR, stmt);
        } else {
            return_close(ERR_OK, stmt);
        }
    } else if (err == SQLITE_DONE) {
        return_close(ERR_UNSUCCESSFUL, stmt);
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_GetCharacters(Database *database, struct uuid account_id, DbCharacterArray *results)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_select_account_characters;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK) {
        log_error("Failed to bind account_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        DbCharacter *result = (DbCharacter *)array_push(results, 1);
        if (DbCharacter_from_stmt(stmt, 0, result) != 0) {
            array_pop(results);
            return_close(ERR_SERVER_ERROR, stmt);
        }
    }

    if (err != SQLITE_DONE) {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
    return_close(ERR_OK, stmt);
}

int Db_CharacterBags(Database *database, struct uuid account_id, struct uuid char_id, DbBag *results, size_t count, size_t *returned)
{
    int err;
    
    sqlite3_stmt *stmt = database->stmt_select_character_bags;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind account_id or/and char_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    size_t idx = 0;
    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (count <= idx) {
            return_close(ERR_SERVER_ERROR, stmt);
        }

        DbBag *bag = &results[idx];
        if (((err = sqlite3_column_uuid(stmt, DbBagCols_account_id, &bag->account_id)) != 0) ||
            ((err = sqlite3_column_uuid_or(stmt, DbBagCols_char_id, null_uuid, &bag->char_id)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbBagCols_bag_model_id, &bag->bag_model_id)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbBagCols_bag_type, &bag->bag_type)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbBagCols_slot_count, &bag->slot_count)) != 0)
        ) {
            return_close(ERR_SERVER_ERROR, stmt);
        }

        ++idx;
    }
    
    if (err != SQLITE_DONE) {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
    
    *returned = idx;
    return_close(ERR_OK, stmt);
}

int Db_CreateCharacter(
    Database *database,
    struct uuid account_id,
    struct uuid char_id,
    size_t n_name, const uint16_t *name,
    uint32_t unlocked_professions,
    CharacterSettings *settings)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_insert_character;
    if ((err = sqlite3_bind_uuid(stmt, 1, char_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u16_array(stmt, 3, n_name, name)) != SQLITE_OK ||
        (err = sqlite3_bind_u8_array(stmt, 4, sizeof(*settings), (const uint8_t *)settings)) != SQLITE_OK ||
        (err = sqlite3_bind_u32(stmt, 5, unlocked_professions)) != SQLITE_OK
    ) {
        log_error(
            "Failed to bind values to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_DONE) {
        return_close(ERR_OK, stmt);
    } else {
        log_error("Failed to insert a row, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_DeleteCharacter(Database *database, struct uuid account_id, struct uuid char_id)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_delete_character;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind values to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_DONE) {
        return_close(ERR_OK, stmt);
    } else {
        log_error("Failed to delete a character, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_CreateBag(Database *database, DbBag *bag)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_create_bag;
    if ((err = sqlite3_bind_uuid(stmt, 1, bag->account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, bag->char_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 3, bag->bag_model_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 4, bag->bag_type)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 5, bag->slot_count)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind values to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_DONE) {
        return_close(ERR_OK, stmt);
    } else {
        log_error("Failed to create a bag, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_CreateBags(Database *database, DbBag *bags, size_t count)
{
    int err;
    for (size_t idx = 0; idx < count; ++idx) {
        if ((err = Db_CreateBag(database, &bags[idx])) != 0) {
            return err;
        }
    }

    return ERR_OK;
}

int Db_CreateItem(Database *database, DbItem *item)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_create_item;
    if ((err = sqlite3_bind_uuid(stmt, 1, item->account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, item->char_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 3, item->bag_model_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u16(stmt, 4, item->slot)) != SQLITE_OK ||
        (err = sqlite3_bind_u16(stmt, 5, item->quantity)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 6, item->dye_tint)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 7, item->dye_colors)) != SQLITE_OK ||
        (err = sqlite3_bind_u32(stmt, 8, item->model_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u32(stmt, 9, item->file_id)) != SQLITE_OK ||
        (err = sqlite3_bind_u32(stmt, 10, item->flags)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 11, item->item_type)) != SQLITE_OK ||
        (err = sqlite3_bind_u8(stmt, 12, item->profession)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind values to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_DONE) {
        return_close(ERR_OK, stmt);
    } else {
        log_error("Failed to create a bag, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }
}

int Db_CreateItems(Database *database, DbItem *items, size_t count)
{
    int err;
    for (size_t idx = 0; idx < count; ++idx) {
        if ((err = Db_CreateItem(database, &items[idx])) != 0) {
            return err;
        }
    }

    return ERR_OK;
}

int DbItem_from_stmt(sqlite3_stmt *stmt, int idx, DbItem *result)
{
    int err;
    if (((err = sqlite3_column_uuid(stmt, idx + DbItemCols_account_id, &result->account_id)) != 0) ||
        ((err = sqlite3_column_uuid(stmt, idx + DbItemCols_char_id, &result->char_id)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbItemCols_bag_model_id, &result->bag_model_id)) != 0) ||
        ((err = sqlite3_column_u16(stmt, idx + DbItemCols_slot, &result->slot)) != 0) ||
        ((err = sqlite3_column_i64(stmt, idx + DbItemCols_created_at, &result->created_at)) != 0) ||
        ((err = sqlite3_column_i64(stmt, idx + DbItemCols_updated_at, &result->updated_at)) != 0) ||
        ((err = sqlite3_column_u16(stmt, idx + DbItemCols_quantity, &result->quantity)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbItemCols_dye_tint, &result->dye_tint)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbItemCols_dye_colors, &result->dye_colors)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbItemCols_item_type, &result->item_type)) != 0) ||
        ((err = sqlite3_column_u8(stmt, idx + DbItemCols_profession, &result->profession)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbItemCols_model_id, &result->model_id)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbItemCols_file_id, &result->file_id)) != 0) ||
        ((err = sqlite3_column_u32(stmt, idx + DbItemCols_flags, &result->flags)) != 0)
    ) {
        return ERR_SERVER_ERROR;
    }
    return ERR_OK;
}

int Db_GetItems(Database *database, struct uuid account_id, struct uuid char_id, DbItemArray *results)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_select_character_items;
    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind account_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return_close(ERR_SERVER_ERROR, stmt);
    }

    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        DbItem *result;
        if ((result = array_push(results, 1)) == NULL) {
            return_close(ERR_UNSUCCESSFUL, stmt);
        }

        if (DbItem_from_stmt(stmt, 0, result) != 0) {
            return_close(ERR_SERVER_ERROR, stmt);
        }
    }

    if (err != SQLITE_DONE) {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return_close(ERR_UNSUCCESSFUL, stmt);
    }

    return_close(ERR_OK, stmt);
}
