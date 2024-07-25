const char *SQL_GET_SESSION    = "SELECT created_at, updated_at, account_id FROM sessions WHERE user_id = ? AND session_id = ?;";
const char *SQL_GET_FRIENDS    = "SELECT * FROM friendships WHERE account_id = ?;";

int sqlite3_column_uuid(sqlite3_stmt *stmt, int iCol, struct uuid *result)
{
    const char *text_value = (const char *)sqlite3_column_text(stmt, iCol);
    if (!uuid_parse(result, text_value, strlen(text_value))) {
        log_error("Couldn't parse uuid from database '%s'", text_value);
        return ERR_SERVER_ERROR;
    } else {
        return ERR_OK;
    }
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

int sqlite3_column_s16(sqlite3_stmt *stmt, int iCol, uint16_t *buffer, size_t size, size_t *length)
{
    const char *text_value = (const char *)sqlite3_column_text(stmt, iCol);
    if (!s16_from_ascii(buffer, size, text_value)) {
        log_error("Couldn't parse uuid from database '%s'", text_value);
        return ERR_SERVER_ERROR;
    } else {
        *length = strlen(text_value);
        return ERR_OK;
    }
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

void append_fields(array_char_t *builder, const char **fields, size_t count)
{
    for (size_t idx = 0; idx < count; ++idx) {
        if (idx == count - 1) {
            appendf(builder, "%s ", fields[idx]);
        } else {
            appendf(builder, "%s, ", fields[idx]);
        }
    }
}

int AuthDb_Open(AuthDb *result, const char *path)
{
    int err;

    memset(result, 0, sizeof(*result));
    if ((err = sqlite3_open(path, &result->conn)) != SQLITE_OK) {
        log_error("Failed to open database '%s', err: %d (%s)", path, err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }

    sqlite3 *conn = result->conn;
    if ((err = sqlite3_prepare_v2(conn, SQL_GET_SESSION, -1, &result->stmt_get_session, 0)) != SQLITE_OK) {
        log_error("Failed to create the 'SQL_GET_SESSION' prepared statement, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }

    array_char_t builder = {0};
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbAccountColsName, ARRAY_SIZE(DbAccountColsName));
    appendf(&builder, " FROM accounts WHERE account_id = ?;");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_account, 0)) != SQLITE_OK) {
        log_error("Failed to create the prepared statement, err: %d (%s)", err, sqlite3_errstr(err));
        array_free(&builder);
        return ERR_UNSUCCESSFUL;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbCharacterColsName, ARRAY_SIZE(DbCharacterColsName));
    appendf(&builder, " FROM characters WHERE account_id = ?;");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_characters, 0)) != SQLITE_OK) {
        log_error("Failed to create the 'SQL_GET_CHARACTERS' prepared statement, err: %d (%s)", err, sqlite3_errstr(err));
        array_free(&builder);
        return ERR_UNSUCCESSFUL;
    }

    if ((err = sqlite3_prepare_v2(conn, SQL_GET_FRIENDS, -1, &result->stmt_get_friendships, 0)) != SQLITE_OK) {
        log_error("Failed to create the 'SQL_GET_FRIENDS' prepared statement, err: %d (%s)", err, sqlite3_errstr(err));
        array_free(&builder);
        return ERR_UNSUCCESSFUL;
    }

    return ERR_OK;
}

void AuthDb_Close(AuthDb *database)
{
    int err;

    if ((err = sqlite3_finalize(database->stmt_get_session)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_session', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_account)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_account', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_characters)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_characters', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_friendships)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_friendships', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_close_v2(database->conn)) != SQLITE_OK) {
        log_error("Failed to close database %d (%s)", err, sqlite3_errstr(err));
    }
}

int AuthDb_GetSession(AuthDb *database, struct uuid user_id, struct uuid session_id, DbSession *result)
{
    int err;
    char user_id_buffer[UUID_STRING_LEN + 1];
    char session_id_buffer[UUID_STRING_LEN + 1];

    uuid_snprint(user_id_buffer, sizeof(user_id_buffer), &user_id);
    uuid_snprint(session_id_buffer, sizeof(session_id_buffer), &session_id);

    sqlite3_stmt *stmt = database->stmt_get_session;
    if ((err = sqlite3_bind_text(stmt, 1, user_id_buffer, UUID_STRING_LEN, NULL)) != SQLITE_OK) {
        log_error("Failed to bind user_id '%s' to a statement, err: %d (%s)", user_id_buffer, err, sqlite3_errstr(err));
        sqlite3_reset(stmt);
        return ERR_SERVER_ERROR;
    }

    if ((err = sqlite3_bind_text(stmt, 2, session_id_buffer, UUID_STRING_LEN, NULL)) != SQLITE_OK) {
        log_error("Failed to bind session_id '%s' to a statement, err: %d (%s)", session_id_buffer, err, sqlite3_errstr(err));
        sqlite3_reset(stmt);
        return ERR_SERVER_ERROR;
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        result->user_id = user_id;
        result->session_id = session_id;
        err = sqlite3_column_uuid(stmt, 2, &result->account_id);
        sqlite3_reset(stmt);
        return err;
    } else if (err == SQLITE_DONE) {
        return ERR_UNSUCCESSFUL;
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
}

int AuthDb_GetAccount(AuthDb *database, struct uuid account_id, DbAccount *result)
{
    int err;
    char account_id_buffer[UUID_STRING_LEN + 1];
    uuid_snprint(account_id_buffer, sizeof(account_id_buffer), &account_id);

    sqlite3_stmt *stmt = database->stmt_get_account;
    if ((err = sqlite3_bind_text(stmt, 1, account_id_buffer, UUID_STRING_LEN, NULL)) != SQLITE_OK) {
        log_error("Failed to bind account_id '%s' to a statement, err: %d (%s)", account_id_buffer, err, sqlite3_errstr(err));
        sqlite3_reset(stmt);
        return ERR_SERVER_ERROR;
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (((err = sqlite3_column_uuid(stmt, DbAccountCols_account_id, &result->account_id)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbAccountCols_created_at, &result->created_at)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbAccountCols_updated_at, &result->updated_at)) != 0) ||
            ((err = sqlite3_column_bool(stmt, DbAccountCols_eula_accepted, &result->eula_accepted)) != 0) ||
            ((err = sqlite3_column_uuid(stmt, DbAccountCols_current_char_id, &result->current_char_id)) != 0) ||
            ((err = sqlite3_column_u16(stmt, DbAccountCols_current_territory, &result->current_territory)) != 0) ||
            ((err = sqlite3_column_u16(stmt, DbAccountCols_storage_gold, &result->storage_gold)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_balthazar_points_max, &result->balthazar_points_max)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_balthazar_points_amount, &result->balthazar_points_amount)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_balthazar_points_total, &result->balthazar_points_total)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_kurzick_points_max, &result->kurzick_points_max)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_kurzick_points_amount, &result->kurzick_points_amount)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_kurzick_points_total, &result->kurzick_points_total)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_luxon_points_max, &result->luxon_points_max)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_luxon_points_amount, &result->luxon_points_amount)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_luxon_points_total, &result->luxon_points_total)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_imperial_points_max, &result->imperial_points_max)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_imperial_points_amount, &result->imperial_points_amount)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbAccountCols_imperial_points_total, &result->imperial_points_total)) != 0)
        ) {
            sqlite3_reset(stmt);
            return ERR_SERVER_ERROR;
        }

        result->eula_accepted = sqlite3_column_int(stmt, 3) != 0;
        result->current_territory = (uint16_t)(sqlite3_column_int(stmt, 5) & 0xFFFF);
        sqlite3_reset(stmt);
        return ERR_OK;
    } else if (err == SQLITE_DONE) {
        return ERR_UNSUCCESSFUL;
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
}

int AuthDb_GetCharacters(AuthDb *database, struct uuid account_id, DbCharacterArray *results)
{
    int err;
    char account_id_buffer[UUID_STRING_LEN + 1];
    uuid_snprint(account_id_buffer, sizeof(account_id_buffer), &account_id);

    sqlite3_stmt *stmt = database->stmt_get_characters;
    if ((err = sqlite3_bind_text(stmt, 1, account_id_buffer, UUID_STRING_LEN, NULL)) != SQLITE_OK) {
        log_error("Failed to bind account_id '%s' to a statement, err: %d (%s)", account_id_buffer, err, sqlite3_errstr(err));
        sqlite3_reset(stmt);
        return ERR_SERVER_ERROR;
    }

    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        DbCharacter *result = (DbCharacter *)array_push(results, 1);
        if (((err = sqlite3_column_uuid(stmt, DbCharacterCols_char_id, &result->char_id)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbCharacterCols_created_at, &result->created_at)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbCharacterCols_updated_at, &result->updated_at)) != 0) ||
            ((err = sqlite3_column_uuid(stmt, DbCharacterCols_account_id, &result->account_id)) != 0) ||
            ((err = sqlite3_column_s16(stmt, DbCharacterCols_char_name, result->char_name.buf, ARRAY_SIZE(result->char_name.buf), &result->char_name.len)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbCharacterCols_skill_points, &result->skill_points)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbCharacterCols_skill_points_total, &result->skill_points_total)) != 0) ||
            ((err = sqlite3_column_u32(stmt, DbCharacterCols_experience, &result->experience)) != 0) ||
            ((err = sqlite3_column_u16(stmt, DbCharacterCols_gold, &result->gold)) != 0) ||
            ((err = sqlite3_column_u16(stmt, DbCharacterCols_last_outpost, &result->last_outpost)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_primary_profession, &result->primary_profession)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_secondary_profession, &result->secondary_profession)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_level, &result->level)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_active_weapon_set, &result->active_weapon_set)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_campaign, &result->campaign)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_face, &result->face)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_hair_color, &result->hair_color)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_hair_style, &result->hair_style)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_height, &result->height)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_sex, &result->sex)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbCharacterCols_skin, &result->skin)) != 0)
        ) {
            array_pop(results);
            sqlite3_reset(stmt);
            return ERR_SERVER_ERROR;
        }
    }

    sqlite3_reset(stmt);
    if (err != SQLITE_DONE) {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
    return ERR_OK;
}
