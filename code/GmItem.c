#pragma once

GmItem* GameSrv_AllocateItem(GameSrv *srv)
{
    if (srv->free_items_slots.len == 0) {
        size_t item_count = srv->items.len;
        size_t new_size = max_size_t(item_count * 2, 64);
        array_resize(&srv->items, new_size);
        if (item_count == 0) {
            ++item_count;
        }
        size_t added = new_size - item_count;
        uint32_t *free_idxs = array_push(&srv->free_items_slots, added);
        for (size_t idx = 0; idx < added; ++idx) {
            free_idxs[idx] = (uint32_t)(item_count + idx);
        }
    }

    uint32_t item_id = array_pop(&srv->free_items_slots);
    srv->items.ptr[item_id].item_id = item_id;
    return &srv->items.ptr[item_id];
}

void GameSrv_FreeItemId(GameSrv *srv, uint32_t item_id)
{
    assert(item_id != 0);

    if (srv->items.len <= item_id) {
        log_warn("Can't free item id %" PRIu32 ", when the maximum is %zu", item_id, srv->items.len);
        return;
    }

    memset(&srv->items.ptr[item_id], 0, sizeof(srv->items.ptr[item_id]));
    array_add(&srv->free_items_slots, item_id);
}

GmItem* GameSrv_GetItemById(GameSrv *srv, uint32_t item_id)
{
    if (srv->items.len <= item_id) {
        return NULL;
    }
    GmItem *result = &srv->items.ptr[item_id];
    if (result->item_id == 0) {
        return NULL;
    }
    return result;
}

void GameSrv_SendItemStreamCreate(GameSrv *srv, GameConnection *conn)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_ITEM_STREAM_CREATE);
    GameSrv_ItemStreamCreate *msg = &buffer->item_stream_create;
    msg->stream_id = 1;
    msg->is_hero = 0;
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendItemGeneralInfo(GameSrv *srv, GameConnection *conn, GmItem *item)
{
    GameSrvMsg *buffer = GameSrv_BuildMsg(srv, GAME_SMSG_ITEM_GENERAL_INFO);
    GameSrv_ItemGeneralInfo *msg = &buffer->item_general_info;
    msg->item_id = item->item_id;
    msg->file_id = item->file_id;
    msg->item_type = item->item_type;
    msg->dye_tint = item->dye_tint;
    msg->dye_colors = item->dye_colors;
    msg->materials = item->materials;
    msg->unk1 = item->unk1;
    msg->flags = item->flags;
    msg->value = item->value;
    msg->model_id = item->model_id;
    msg->quantity = item->quantity;
    msg->n_name = item->name.size;
    STATIC_ASSERT(ARRAY_SIZE(msg->name) <= ARRAY_SIZE(item->name.data));
    memcpy_u16(msg->name, item->name.data, item->name.size);
    msg->n_modifiers = item->modifiers.size;
    STATIC_ASSERT(ARRAY_SIZE(msg->modifiers) <= ARRAY_SIZE(item->modifiers.data));
    memcpy_u32(msg->modifiers, item->modifiers.data, item->modifiers.size);
    GameConnection_SendMessage(conn, buffer, sizeof(*msg));
}

void GameSrv_SendItemById(GameSrv *srv, GameConnection *conn, uint32_t item_id)
{
    GmItem *item;
    if ((item = GameSrv_GetItemById(srv, item_id)) == NULL) {
        log_error("Item %u doesn't exist", item_id);
        return;
    }

    GameSrv_SendItemGeneralInfo(srv, conn, item);
    // @TODO: send customization and other stuff
}

