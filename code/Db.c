const char *SQL_GET_FRIENDS    = "SELECT * FROM friendships WHERE account_id = ?;";

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

int sqlite3_bind_uuid(sqlite3_stmt *stmt, int iCol, struct uuid uid)
{
    char buffer[UUID_STRING_LEN + 1];
    uuid_snprint(buffer, sizeof(buffer), &uid);
    return sqlite3_bind_text(stmt, iCol, buffer, UUID_STRING_LEN, SQLITE_TRANSIENT);
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

int Db_Open(Database *result, const char *path)
{
    int err;

    memset(result, 0, sizeof(*result));
    if ((err = sqlite3_open(path, &result->conn)) != SQLITE_OK) {
        log_error("Failed to open database '%s', err: %d (%s)", path, err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }

    sqlite3 *conn = result->conn;
    if ((err = sqlite3_prepare_v2(conn, SQL_GET_FRIENDS, -1, &result->stmt_get_friendships, 0)) != SQLITE_OK) {
        log_error("Failed to create the 'SQL_GET_FRIENDS' prepared statement, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }

    array_char_t builder = {0};
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbSessionColsName, ARRAY_SIZE(DbSessionColsName));
    appendf(&builder, " FROM sessions WHERE user_id = ? AND session_id = ?;");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_session, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbAccountColsName, ARRAY_SIZE(DbAccountColsName));
    appendf(&builder, "FROM accounts WHERE account_id = ?;");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_account, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbCharacterColsName, ARRAY_SIZE(DbCharacterColsName));
    appendf(&builder, "FROM characters WHERE account_id = ?;");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_account_characters, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbCharacterColsName, ARRAY_SIZE(DbCharacterColsName));
    appendf(&builder, "FROM characters WHERE account_id = ? AND char_id = ?;");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_character, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    array_clear(&builder);
    appendf(&builder, "SELECT ");
    append_fields(&builder, DbBagColsName, ARRAY_SIZE(DbBagColsName));
    appendf(&builder, "FROM bags WHERE account_id = ? AND (char_id IS NULL OR char_id = ?);");
    array_add(&builder, '\0');

    if ((err = sqlite3_prepare_v2(conn, builder.data, -1, &result->stmt_get_character_bags, 0)) != SQLITE_OK) {
        goto exit_on_error;
    }

    return ERR_OK;
exit_on_error:
    log_error("Failed to create a prepared statement '%s', err: %d (%s)", builder.data, err, sqlite3_errstr(err));
    array_free(&builder);
    return ERR_UNSUCCESSFUL;
}

void Db_Close(Database *database)
{
    int err;

    if ((err = sqlite3_finalize(database->stmt_get_session)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_session', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_account)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_account', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_account_characters)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_account_characters', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_character)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_account_characters', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_friendships)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_friendships', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_finalize(database->stmt_get_character_bags)) != SQLITE_OK) {
        log_error("Failed to finalize 'stmt_get_character_bags', err: %d (%s)", err, sqlite3_errstr(err));
    }

    if ((err = sqlite3_close_v2(database->conn)) != SQLITE_OK) {
        log_error("Failed to close database %d (%s)", err, sqlite3_errstr(err));
    }
}

int Db_GetSession(Database *database, struct uuid user_id, struct uuid session_id, DbSession *result)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_get_session;
    sqlite3_reset(stmt);

    if ((err = sqlite3_bind_uuid(stmt, 1, user_id)) != SQLITE_OK) {
        log_error("Failed to bind user_id to a statement, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_SERVER_ERROR;
    }

    if ((err = sqlite3_bind_uuid(stmt, 2, session_id)) != SQLITE_OK) {
        log_error("Failed to bind session_id to a statement, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_SERVER_ERROR;
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (((err = sqlite3_column_uuid(stmt, DbSessionCols_user_id, &result->user_id)) != 0) ||
            ((err = sqlite3_column_uuid(stmt, DbSessionCols_session_id, &result->session_id)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbSessionCols_created_at, &result->created_at)) != 0) ||
            ((err = sqlite3_column_i64(stmt, DbSessionCols_updated_at, &result->updated_at)) != 0) ||
            ((err = sqlite3_column_uuid(stmt, DbSessionCols_account_id, &result->account_id)) != 0))
        {
            return ERR_UNSUCCESSFUL;
        }

        return ERR_OK;
    } else if (err == SQLITE_DONE) {
        return ERR_UNSUCCESSFUL;
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
}

int Db_GetAccount(Database *database, struct uuid account_id, DbAccount *result)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_get_account;
    sqlite3_reset(stmt);

    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK) {
        log_error(
            "Failed to bind account_id '%s' to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
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
            return ERR_SERVER_ERROR;
        }

        return ERR_OK;
    } else if (err == SQLITE_DONE) {
        return ERR_UNSUCCESSFUL;
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
}

int DbCharacter_from_stmt(sqlite3_stmt *stmt, DbCharacter *result)
{
    int err;
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
        return ERR_SERVER_ERROR;
    }
    return ERR_OK;
}

int Db_GetCharacter(Database *database, struct uuid account_id, struct uuid char_id, DbCharacter *result)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_get_character;
    sqlite3_reset(stmt);

    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind account_id or/and char_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return ERR_SERVER_ERROR;
    }

    if ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        err = DbCharacter_from_stmt(stmt, result);
        if (err != 0) {
            return ERR_SERVER_ERROR;
        } else {
            return ERR_OK;
        }
    } else if (err == SQLITE_DONE) {
        return ERR_UNSUCCESSFUL;
    } else {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
}

int Db_GetCharacters(Database *database, struct uuid account_id, DbCharacterArray *results)
{
    int err;

    sqlite3_stmt *stmt = database->stmt_get_account_characters;
    sqlite3_reset(stmt);

    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK) {
        log_error("Failed to bind account_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return ERR_SERVER_ERROR;
    }

    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        DbCharacter *result = (DbCharacter *)array_push(results, 1);
        err = DbCharacter_from_stmt(stmt, result);
        if (err != 0) {
            array_pop(results);
            return ERR_SERVER_ERROR;
        }
    }

    if (err != SQLITE_DONE) {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
    return ERR_OK;
}

int Db_CharacterBags(Database *database, struct uuid account_id, struct uuid char_id, DbBag *results, size_t count, size_t *returned)
{
    int err;
    
    sqlite3_stmt *stmt = database->stmt_get_character_bags;
    sqlite3_reset(stmt);

    if ((err = sqlite3_bind_uuid(stmt, 1, account_id)) != SQLITE_OK ||
        (err = sqlite3_bind_uuid(stmt, 2, char_id)) != SQLITE_OK)
    {
        log_error(
            "Failed to bind account_id or/and char_id to a statement, err: %d (%s)",
            err,
            sqlite3_errstr(err)
        );
        return ERR_SERVER_ERROR;
    }

    size_t idx = 0;
    while ((err = sqlite3_step(stmt)) == SQLITE_ROW) {
        if (count <= idx) {
            return ERR_SERVER_ERROR;
        }

        DbBag *bag = &results[idx];
        if (((err = sqlite3_column_uuid(stmt, DbBagCols_account_id, &bag->account_id)) != 0) ||
            ((err = sqlite3_column_uuid_or(stmt, DbBagCols_char_id, null_uuid, &bag->char_id)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbBagCols_bag_model_id, &bag->bag_model_id)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbBagCols_bag_type, &bag->bag_type)) != 0) ||
            ((err = sqlite3_column_u8(stmt, DbBagCols_slot_count, &bag->slot_count)) != 0)
        ) {
            return ERR_SERVER_ERROR;
        }

        ++idx;
    }
    
    if (err != SQLITE_DONE) {
        log_warn("Query failed, err: %d (%s)", err, sqlite3_errstr(err));
        return ERR_UNSUCCESSFUL;
    }
    
    *returned = idx;
    return ERR_OK;
}
