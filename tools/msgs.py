auth_cmsg_names = {
0x0000: 'AUTH_CMSG_HEARTBEAT',
0x0001: 'AUTH_CMSG_SEND_COMPUTER_INFO',
0x0002: 'AUTH_CMSG_SEND_COMPUTER_HASH',
0x0003: 'AUTH_CMSG_ACCOUNT_CREATE',
0x0004: 'AUTH_CMSG_ACCOUNT_LOGIN',
0x0007: 'AUTH_CMSG_DELETE_CHARACTER',
0x000A: 'AUTH_CMSG_CHANGE_PLAY_CHARACTER',
0x000D: 'AUTH_CMSG_DISCONNECT',
0x000E: 'AUTH_CMSG_SET_PLAYER_STATUS',
0x000F: 'AUTH_CMSG_SEND_HARDWARE_INFO',
0x001A: 'AUTH_CMSG_FRIEND_ADD',
0x001C: 'AUTH_CMSG_ADD_ACCESS_KEY',
0x0020: 'AUTH_CMSG_SETTING_UPDATE_CONTENT',
0x0025: 'AUTH_CMSG_REQUEST_GUILD_HALL',
0x0021: 'AUTH_CMSG_SETTING_UPDATE_SIZE',
0x0026: 'AUTH_CMSG_ACCEPT_EULA',
0x0029: 'AUTH_CMSG_REQUEST_GAME_INSTANCE',
0x0035: 'AUTH_CMSG_ASK_SERVER_RESPONSE',
0x0038: 'AUTH_CMSG_PORTAL_ACCOUNT_LOGIN',
}

auth_smsg_names = {
0x0000: 'AUTH_SMSG_HEARTBEAT',
0x0001: 'AUTH_SMSG_SESSION_INFO',
0x0003: 'AUTH_SMSG_REQUEST_RESPONSE',
0x0007: 'AUTH_SMSG_CHARACTER_INFO',
0x0009: 'AUTH_SMSG_GAME_SERVER_INFO',
0x000A: 'AUTH_SMSG_FRIEND_UPDATE_INFO',
0x000B: 'AUTH_SMSG_FRIEND_UPDATE_STATUS',
0x000C: 'AUTH_SMSG_WHISPER_RECEIVED',
0x0011: 'AUTH_SMSG_ACCOUNT_INFO',
0x0014: 'AUTH_SMSG_FRIEND_STREAM_END',
0x0016: 'AUTH_SMSG_ACCOUNT_SETTINGS',
0x0020: 'AUTH_SMSG_FRIEND_UPDATE_LOCATION',
0x0026: 'AUTH_SMSG_SERVER_RESPONSE',
}

game_cmsg_names = {
0x0000: 'GAME_CMSG_TRADE_ACKNOWLEDGE',
0x0001: 'GAME_CMSG_TRADE_CANCEL',
0x0002: 'GAME_CMSG_TRADE_ADD_ITEM',
0x0003: 'GAME_CMSG_TRADE_SEND_OFFER',
0x0005: 'GAME_CMSG_TRADE_REMOVE_ITEM',
0x0006: 'GAME_CMSG_TRADE_CANCEL_OFFER',
0x0007: 'GAME_CMSG_TRADE_ACCEPT',
0x0008: 'GAME_CMSG_DISCONNECT',
0x0009: 'GAME_CMSG_PING_REPLY',
0x000A: 'GAME_CMSG_HEARTBEAT',
0x000B: 'GAME_CMSG_PING_REQUEST',
0x000C: 'GAME_CMSG_ATTRIBUTE_DECREASE',
0x000D: 'GAME_CMSG_ATTRIBUTE_INCREASE',
0x000E: 'GAME_CMSG_ATTRIBUTE_LOAD',
0x000F: 'GAME_CMSG_QUEST_ABANDON',
0x0010: 'GAME_CMSG_QUEST_REQUEST_INFOS',
0x0013: 'GAME_CMSG_HERO_BEHAVIOR',
0x0014: 'GAME_CMSG_HERO_LOCK_TARGET',
0x0017: 'GAME_CMSG_HERO_SKILL_TOGGLE',
0x0018: 'GAME_CMSG_HERO_FLAG_SINGLE',
0x0019: 'GAME_CMSG_HERO_FLAG_ALL',
0x001A: 'GAME_CMSG_HERO_USE_SKILL',
0x001C: 'GAME_CMSG_HERO_ADD',
0x001D: 'GAME_CMSG_HERO_KICK',
0x0021: 'GAME_CMSG_TARGET_CALL',
0x0023: 'GAME_CMSG_PING_WEAPON_SET',
0x0024: 'GAME_CMSG_ATTACK_AGENT',
0x0026: 'GAME_CMSG_CANCEL_MOVEMENT',
0x0027: 'GAME_CMSG_DROP_BUFF',
0x0029: 'GAME_CMSG_DRAW_MAP',
0x002A: 'GAME_CMSG_DROP_ITEM',
0x002D: 'GAME_CMSG_DROP_GOLD',
0x002E: 'GAME_CMSG_EQUIP_ITEM',
0x0031: 'GAME_CMSG_INTERACT_PLAYER',
0x0033: 'GAME_CMSG_DEPOSIT_FACTION',
0x0037: 'GAME_CMSG_INTERACT_LIVING',
0x0039: 'GAME_CMSG_SEND_DIALOG',
0x003C: 'GAME_CMSG_MOVE_TO_COORD',
0x003D: 'GAME_CMSG_INTERACT_ITEM',
0x003E: 'GAME_CMSG_ROTATE_PLAYER',
0x003F: 'GAME_CMSG_CHANGE_SECOND_PROFESSION',
0x0044: 'GAME_CMSG_USE_SKILL',
0x0047: 'GAME_CMSG_TRADE_INITIATE',
0x0048: 'GAME_CMSG_BUY_MATERIALS',
0x004A: 'GAME_CMSG_REQUEST_QUOTE',
0x004B: 'GAME_CMSG_TRANSACT_ITEMS',
0x004D: 'GAME_CMSG_UNEQUIP_ITEM',
0x004F: 'GAME_CMSG_INTERACT_GADGET',
0x0051: 'GAME_CMSG_OPEN_CHEST',
0x0055: 'GAME_CMSG_EQUIP_VISIBILITY',
0x0056: 'GAME_CMSG_TITLE_DISPLAY',
0x0057: 'GAME_CMSG_TITLE_HIDE',
0x005A: 'GAME_CMSG_SKILLBAR_SKILL_SET',
0x005B: 'GAME_CMSG_SKILLBAR_LOAD',
0x005C: 'GAME_CMSG_SKILLBAR_SKILL_REPLACE',
0x005E: 'GAME_CMSG_CHAR_CREATION_CHANGE_PROF',
0x0061: 'GAME_CMSG_SKIP_CINEMATIC',
0x0062: 'GAME_CMSG_SEND_CHAT_MESSAGE',
0x0067: 'GAME_CMSG_ITEM_DESTROY',
0x006A: 'GAME_CMSG_ITEM_IDENTIFY',
0x006B: 'GAME_CMSG_TOME_UNLOCK_SKILL',
0x0070: 'GAME_CMSG_ITEM_MOVE',
0x0071: 'GAME_CMSG_ITEM_ACCEPT_ALL',
0x0073: 'GAME_CMSG_ITEM_SPLIT_STACK',
0x0075: 'GAME_CMSG_ITEM_SALVAGE_SESSION_OPEN',
0x0076: 'GAME_CMSG_ITEM_SALVAGE_SESSION_CANCEL',
0x0077: 'GAME_CMSG_ITEM_SALVAGE_SESSION_DONE',
0x0078: 'GAME_CMSG_ITEM_SALVAGE_MATERIALS',
0x0079: 'GAME_CMSG_ITEM_SALVAGE_UPGRADE',
0x007A: 'GAME_CMSG_ITEM_CHANGE_GOLD',
0x007C: 'GAME_CMSG_ITEM_USE',
0x0082: 'GAME_CMSG_CHANGE_EQUIPPED_ITEM_COLOR',
0x0087: 'GAME_CMSG_CHAR_CREATION_REQUEST_PLAYER',
0x0088: 'GAME_CMSG_CHAR_CREATION_REQUEST_ARMORS',
0x0089: 'GAME_CMSG_CHAR_CREATION_CONFIRM',
0x0086: 'GAME_CMSG_INSTANCE_LOAD_REQUEST_SPAWN',
0x008E: 'GAME_CMSG_INSTANCE_LOAD_REQUEST_PLAYERS',
0x008F: 'GAME_CMSG_INSTANCE_LOAD_REQUEST_ITEMS',
0x0099: 'GAME_CMSG_PARTY_SET_DIFFICULTY',
0x009A: 'GAME_CMSG_PARTY_ACCEPT_INVITE',
0x009B: 'GAME_CMSG_PARTY_ACCEPT_CANCEL',
0x009C: 'GAME_CMSG_PARTY_ACCEPT_REFUSE',
0x009D: 'GAME_CMSG_PARTY_INVITE_NPC',
0x009E: 'GAME_CMSG_PARTY_INVITE_PLAYER',
0x009F: 'GAME_CMSG_PARTY_INVITE_PLAYER_NAME',
0x00A0: 'GAME_CMSG_PARTY_LEAVE_GROUP',
0x00A1: 'GAME_CMSG_PARTY_CANCEL_ENTER_CHALLENGE',
0x00A3: 'GAME_CMSG_PARTY_ENTER_CHALLENGE',
0x00A5: 'GAME_CMSG_PARTY_RETURN_TO_OUTPOST',
0x00A6: 'GAME_CMSG_PARTY_KICK_NPC',
0x00A7: 'GAME_CMSG_PARTY_KICK_PLAYER',
0x00A8: 'GAME_CMSG_PARTY_SEARCH_SEEK',
0x00A9: 'GAME_CMSG_PARTY_SEARCH_CANCEL',
0x00AA: 'GAME_CMSG_PARTY_SEARCH_REQUEST_JOIN',
0x00AB: 'GAME_CMSG_PARTY_SEARCH_REQUEST_REPLY',
0x00AC: 'GAME_CMSG_PARTY_SEARCH_TYPE',
0x00AD: 'GAME_CMSG_PARTY_READY_STATUS',
0x00AE: 'GAME_CMSG_PARTY_ENTER_GUILD_HALL',
0x00AF: 'GAME_CMSG_PARTY_TRAVEL',
0x00B0: 'GAME_CMSG_PARTY_LEAVE_GUILD_HALL',
}

game_smsg_names = {
0x0000: 'GAME_SMSG_TRADE_REQUEST',
0x0001: 'GAME_SMSG_TRADE_TERMINATE',
0x0002: 'GAME_SMSG_TRADE_CHANGE_OFFER',
0x0003: 'GAME_SMSG_TRADE_RECEIVE_OFFER',
0x0004: 'GAME_SMSG_TRADE_ADD_ITEM',
0x0005: 'GAME_SMSG_TRADE_ACKNOWLEDGE',
0x0006: 'GAME_SMSG_TRADE_ACCEPT',
0x0008: 'GAME_SMSG_TRADE_OFFERED_COUNT',
0x000C: 'GAME_SMSG_PING_REQUEST',
0x000D: 'GAME_SMSG_PING_REPLY',
0x000E: 'GAME_SMSG_FRIENDLIST_MESSAGE',
0x000F: 'GAME_SMSG_ACCOUNT_FEATURE',
0x0018: 'GAME_SMSG_UNLOCKED_PVP_HEROES',
0x001A: 'GAME_SMSG_PVP_ITEM_ADD_UNLOCK',
0x001B: 'GAME_SMSG_PVP_ITEM_END',
0x001D: 'GAME_SMSG_UNLOCKED_SKILLS',
0x001E: 'GAME_SMSG_AGENT_MOVEMENT_TICK',
0x001F: 'GAME_SMSG_AGENT_INSTANCE_TIMER',
0x0020: 'GAME_SMSG_AGENT_SPAWNED',
0x0021: 'GAME_SMSG_AGENT_DESPAWNED',
0x0022: 'GAME_SMSG_AGENT_SET_PLAYER',
0x0025: 'GAME_SMSG_AGENT_UPDATE_DIRECTION',
0x0027: 'GAME_SMSG_AGENT_UPDATE_SPEED_BASE',
0x0028: 'GAME_SMSG_AGENT_STOP_MOVING',
0x0029: 'GAME_SMSG_AGENT_MOVE_TO_POINT',
0x002A: 'GAME_SMSG_AGENT_UPDATE_DESTINATION',
0x002B: 'GAME_SMSG_AGENT_UPDATE_SPEED',
0x002C: 'GAME_SMSG_AGENT_UPDATE_POSITION',
0x002D: 'GAME_SMSG_AGENT_PLAYER_DIE',
0x002E: 'GAME_SMSG_AGENT_UPDATE_ROTATION',
0x002F: 'GAME_SMSG_AGENT_UPDATE_ALLEGIANCE',
0x0031: 'GAME_SMSG_HERO_ACCOUNT_NAME',
0x0033: 'GAME_SMSG_MESSAGE_OF_THE_DAY',
0x0034: 'GAME_SMSG_AGENT_PINGED',
0x0037: 'GAME_SMSG_AGENT_CREATE_ATTRIBUTES',
0x003A: 'GAME_SMSG_AGENT_UPDATE_ATTRIBUTE',
0x003D: 'GAME_SMSG_AGENT_ALLY_DESTROY',
0x003E: 'GAME_SMSG_EFFECT_UPKEEP_ADDED',
0x003F: 'GAME_SMSG_EFFECT_UPKEEP_REMOVED',
0x0040: 'GAME_SMSG_EFFECT_UPKEEP_APPLIED',
0x0041: 'GAME_SMSG_EFFECT_APPLIED',
0x0042: 'GAME_SMSG_EFFECT_RENEWED',
0x0043: 'GAME_SMSG_EFFECT_REMOVED',
0x0045: 'GAME_SMSG_SCREEN_SHAKE',
0x0047: 'GAME_SMSG_AGENT_DISPLAY_CAPE',
0x0048: 'GAME_SMSG_QUEST_ADD',
0x004B: 'GAME_SMSG_QUEST_DESCRIPTION',
0x004F: 'GAME_SMSG_QUEST_GENERAL_INFO',
0x0050: 'GAME_SMSG_QUEST_UPDATE_MARKER',
0x0051: 'GAME_SMSG_QUEST_REMOVE',
0x0052: 'GAME_SMSG_QUEST_ADD_MARKER',
0x0053: 'GAME_SMSG_QUEST_UPDATE_NAME',
0x0055: 'GAME_SMSG_NPC_UPDATE_PROPERTIES',
0x0056: 'GAME_SMSG_NPC_UPDATE_MODEL',
0x0058: 'GAME_SMSG_AGENT_CREATE_PLAYER',
0x0059: 'GAME_SMSG_AGENT_DESTROY_PLAYER',
0x005C: 'GAME_SMSG_CHAT_MESSAGE_CORE',
0x005D: 'GAME_SMSG_CHAT_MESSAGE_SERVER',
0x005E: 'GAME_SMSG_CHAT_MESSAGE_NPC',
0x005F: 'GAME_SMSG_CHAT_MESSAGE_GLOBAL',
0x0060: 'GAME_SMSG_CHAT_MESSAGE_LOCAL',
0x0061: 'GAME_SMSG_HERO_BEHAVIOR',
0x0063: 'GAME_SMSG_HERO_SKILL_STATUS',
0x0064: 'GAME_SMSG_HERO_SKILL_STATUS_BITMAP',
0x006A: 'GAME_SMSG_POST_PROCESS',
0x006B: 'GAME_SMSG_DUNGEON_REWARD',
0x006C: 'GAME_SMSG_NPC_UPDATE_WEAPONS',
0x0073: 'GAME_SMSG_MERCENARY_INFO',
0x007D: 'GAME_SMSG_DIALOG_BUTTON',
0x007F: 'GAME_SMSG_DIALOG_BODY',
0x0080: 'GAME_SMSG_DIALOG_SENDER',
0x0082: 'GAME_SMSG_WINDOW_OPEN',
0x0083: 'GAME_SMSG_WINDOW_ADD_ITEMS',
0x0084: 'GAME_SMSG_WINDOW_ITEMS_END',
0x0085: 'GAME_SMSG_WINDOW_ITEM_STREAM_END',
0x0089: 'GAME_SMSG_CARTOGRAPHY_DATA',
0x0090: 'GAME_SMSG_COMPASS_DRAWING',
0x0093: 'GAME_SMSG_MAPS_UNLOCKED',
0x0099: 'GAME_SMSG_AGENT_UPDATE_SCALE',
0x009A: 'GAME_SMSG_AGENT_UPDATE_NPC_NAME',
0x009D: 'GAME_SMSG_AGENT_DISPLAY_DIALOG',
0x009E: 'GAME_SMSG_AGENT_ATTR_UPDATE_INT',
0x009F: 'GAME_SMSG_AGENT_ATTR_UPDATE_INT_TARGET',
0x00A0: 'GAME_SMSG_AGENT_ATTR_PLAY_EFFECT',
0x00A1: 'GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT',
0x00A2: 'GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT_TARGET',
0x00A3: 'GAME_SMSG_AGENT_PROJECTILE_LAUNCHED',
0x00A4: 'GAME_SMSG_SPEECH_BUBBLE',
0x00A5: 'GAME_SMSG_AGENT_UPDATE_PROFESSION',
0x00A9: 'GAME_SMSG_AGENT_CREATE_NPC',
0x00AD: 'GAME_SMSG_UPDATE_AGENT_MODEL',
0x00AF: 'GAME_SMSG_UPDATE_AGENT_PARTYSIZE',
0x00B5: 'GAME_SMSG_PLAYER_UNLOCKED_PROFESSION',
0x00B6: 'GAME_SMSG_PLAYER_UPDATE_PROFESSION',
0x00B8: 'GAME_SMSG_MISSION_INFOBOX_ADD',
0x00B9: 'GAME_SMSG_MISSION_STREAM_START',
0x00BA: 'GAME_SMSG_MISSION_OBJECTIVE_ADD',
0x00BB: 'GAME_SMSG_MISSION_OBJECTIVE_COMPLETE',
0x00BC: 'GAME_SMSG_MISSION_OBJECTIVE_UPDATE_STRING',
0x00C2: 'GAME_SMSG_WINDOW_MERCHANT',
0x00C3: 'GAME_SMSG_WINDOW_OWNER',
0x00C5: 'GAME_SMSG_TRANSACTION_REJECT',
0x00CB: 'GAME_SMSG_TRANSACTION_DONE',
0x00D8: 'GAME_SMSG_SKILLBAR_UPDATE_SKILL',
0x00D9: 'GAME_SMSG_SKILLBAR_UPDATE',
0x00DA: 'GAME_SMSG_SKILLS_UNLOCKED',
0x00DB: 'GAME_SMSG_SKILL_ADD_TO_WINDOW_COUNT',
0x00DF: 'GAME_SMSG_SKILL_ADD_TO_WINDOWS_DATA',
0x00E0: 'GAME_SMSG_SKILL_ADD_TO_WINDOWS_END',
0x00E1: 'GAME_SMSG_SKILL_INTERUPTED',
0x00E2: 'GAME_SMSG_SKILL_CANCEL',
0x00E2: 'GAME_SMSG_SKILL_ACTIVATED',
0x00E3: 'GAME_SMSG_SKILL_ACTIVATE',
0x00E4: 'GAME_SMSG_SKILL_RECHARGE',
0x00E5: 'GAME_SMSG_SKILL_RECHARGED',
0x00E8: 'GAME_SMSG_PLAYER_ATTR_SET',
0x00E9: 'GAME_SMSG_PLAYER_ATTR_MAX_KURZICK',
0x00EA: 'GAME_SMSG_PLAYER_ATTR_MAX_LUXON',
0x00EB: 'GAME_SMSG_PLAYER_ATTR_MAX_BALTHAZAR',
0x00EC: 'GAME_SMSG_PLAYER_ATTR_MAX_IMPERIAL',
0x00ED: 'GAME_SMSG_PLAYER_ATTR_UPDATE',
0x00EF: 'GAME_SMSG_AGENT_INITIAL_EFFECTS',
0x00F0: 'GAME_SMSG_AGENT_UPDATE_EFFECTS',
0x00F1: 'GAME_SMSG_INSTANCE_LOADED',
0x00F2: 'GAME_SMSG_TITLE_RANK_DATA',
0x00F3: 'GAME_SMSG_TITLE_RANK_DISPLAY',
0x00F4: 'GAME_SMSG_TITLE_UPDATE',
0x00F5: 'GAME_SMSG_TITLE_TRACK_INFO',
0x00F6: 'GAME_SMSG_ITEM_PRICE_QUOTE',
0x00F8: 'GAME_SMSG_ITEM_PRICES',
0x00F9: 'GAME_SMSG_VANQUISH_PROGRESS',
0x00FA: 'GAME_SMSG_VANQUISH_COMPLETE',
0x00FD: 'GAME_SMSG_CINEMATIC_SKIP_EVERYONE',
0x00FE: 'GAME_SMSG_CINEMATIC_SKIP_COUNT',
0x00FF: 'GAME_SMSG_CINEMATIC_START',
0x0101: 'GAME_SMSG_CINEMATIC_TEXT',
0x0102: 'GAME_SMSG_CINEMATIC_DATA_END',
0x0103: 'GAME_SMSG_CINEMATIC_DATA',
0x0104: 'GAME_SMSG_CINEMATIC_END',
0x0109: 'GAME_SMSG_SIGNPOST_BUTTON',
0x010A: 'GAME_SMSG_SIGNPOST_BODY',
0x010B: 'GAME_SMSG_SIGNPOST_SENDER',
0x010D: 'GAME_SMSG_MANIPULATE_MAP_OBJECT',
0x0110: 'GAME_SMSG_MANIPULATE_MAP_OBJECT2',
0x0117: 'GAME_SMSG_GUILD_PLAYER_ROLE',
0x0119: 'GAME_SMSG_TOWN_ALLIANCE_OBJECT',
0x011F: 'GAME_SMSG_GUILD_ALLIANCE_INFO',
0x0120: 'GAME_SMSG_GUILD_GENERAL_INFO',
0x0121: 'GAME_SMSG_GUILD_CHANGE_FACTION',
0x0122: 'GAME_SMSG_GUILD_INVITE_RECEIVED',
0x0126: 'GAME_SMSG_GUILD_PLAYER_INFO',
0x0127: 'GAME_SMSG_GUILD_PLAYER_REMOVE',
0x0129: 'GAME_SMSG_GUILD_PLAYER_CHANGE_COMPLETE',
0x012A: 'GAME_SMSG_GUILD_CHANGE_PLAYER_CONTEXT',
0x012B: 'GAME_SMSG_GUILD_CHANGE_PLAYER_STATUS',
0x012C: 'GAME_SMSG_GUILD_CHANGE_PLAYER_TYPE',
0x0134: 'GAME_SMSG_ITEM_UPDATE_OWNER',
0x0138: 'GAME_SMSG_ITEM_UPDATE_QUANTITY',
0x0139: 'GAME_SMSG_ITEM_UPDATE_NAME',
0x013D: 'GAME_SMSG_ITEM_MOVED_TO_LOCATION',
0x013E: 'GAME_SMSG_INVENTORY_CREATE_BAG',
0x013F: 'GAME_SMSG_GOLD_CHARACTER_ADD',
0x0140: 'GAME_SMSG_GOLD_STORAGE_ADD',
0x0143: 'GAME_SMSG_ITEM_STREAM_CREATE',
0x0144: 'GAME_SMSG_ITEM_STREAM_DESTROY',
0x0146: 'GAME_SMSG_ITEM_WEAPON_SET',
0x0147: 'GAME_SMSG_ITEM_SET_ACTIVE_WEAPON_SET',
0x014A: 'GAME_SMSG_ITEM_CHANGE_LOCATION',
0x014C: 'GAME_SMSG_ITEM_REMOVE',
0x014E: 'GAME_SMSG_GOLD_CHARACTER_REMOVE',
0x014F: 'GAME_SMSG_GOLD_STORAGE_REMOVE',
0x0153: 'GAME_SMSG_TOME_SHOW_SKILLS',
0x0159: 'GAME_SMSG_ITEM_SET_PROFESSION',
0x0160: 'GAME_SMSG_ITEM_GENERAL_INFO',
0x0161: 'GAME_SMSG_ITEM_REUSE_ID',
0x0162: 'GAME_SMSG_ITEM_SALVAGE_SESSION_START',
0x0163: 'GAME_SMSG_ITEM_SALVAGE_SESSION_CANCEL',
0x0164: 'GAME_SMSG_ITEM_SALVAGE_SESSION_DONE',
0x0165: 'GAME_SMSG_ITEM_SALVAGE_SESSION_SUCCESS',
0x0166: 'GAME_SMSG_ITEM_SALVAGE_SESSION_ITEM_KEPT',
0x017A: 'GAME_SMSG_INSTANCE_SHOW_WIN',
0x017B: 'GAME_SMSG_INSTANCE_LOAD_HEAD',
0x017C: 'GAME_SMSG_INSTANCE_LOAD_PLAYER_NAME',
0x017D: 'GAME_SMSG_INSTANCE_COUNTDOWN_STOP',
0x017F: 'GAME_SMSG_INSTANCE_COUNTDOWN',
0x0185: 'GAME_SMSG_INSTANCE_PLAYER_DATA_START',
0x0187: 'GAME_SMSG_CHAR_CREATION_SUCCESS',
0x0188: 'GAME_SMSG_CHAR_CREATION_START',
0x0189: 'GAME_SMSG_INSTANCE_PLAYER_DATA_DONE',
0x018A: 'GAME_SMSG_CHAR_CREATION_ERROR',
0x018D: 'GAME_SMSG_INSTANCE_LOAD_FINISH',
0x018F: 'GAME_SMSG_JUMBO_MESSAGE',
0x0194: 'GAME_SMSG_INSTANCE_LOAD_SPAWN_POINT',
0x0198: 'GAME_SMSG_INSTANCE_LOAD_INFO',
0x019F: 'GAME_SMSG_CREATE_MISSION_PROGRESS',
0x01A1: 'GAME_SMSG_UPDATE_MISSION_PROGRESS',
0x01A4: 'GAME_SMSG_TRANSFER_GAME_SERVER_INFO',
0x01AA: 'GAME_SMSG_READY_FOR_MAP_SPAWN',
0x01AE: 'GAME_SMSG_DOA_COMPLETE_ZONE',
0x01BA: 'GAME_SMSG_INSTANCE_TRAVEL_TIMER',
0x01BB: 'GAME_SMSG_INSTANCE_CANT_ENTER',
0x01BD: 'GAME_SMSG_PARTY_SET_DIFFICULTY',
0x01BE: 'GAME_SMSG_PARTY_HENCHMAN_ADD',
0x01BF: 'GAME_SMSG_PARTY_HENCHMAN_REMOVE',
0x01C1: 'GAME_SMSG_PARTY_HERO_ADD',
0x01C2: 'GAME_SMSG_PARTY_HERO_REMOVE',
0x01C3: 'GAME_SMSG_PARTY_INVITE_ADD',
0x01C4: 'GAME_SMSG_PARTY_JOIN_REQUEST',
0x01C5: 'GAME_SMSG_PARTY_INVITE_CANCEL',
0x01C6: 'GAME_SMSG_PARTY_REQUEST_CANCEL',
0x01C7: 'GAME_SMSG_PARTY_REQUEST_RESPONSE',
0x01C8: 'GAME_SMSG_PARTY_INVITE_RESPONSE',
0x01C9: 'GAME_SMSG_PARTY_YOU_ARE_LEADER',
0x01CA: 'GAME_SMSG_PARTY_PLAYER_ADD',
0x01CF: 'GAME_SMSG_PARTY_PLAYER_REMOVE',
0x01D0: 'GAME_SMSG_PARTY_PLAYER_READY',
0x01D1: 'GAME_SMSG_PARTY_CREATE',
0x01D2: 'GAME_SMSG_PARTY_MEMBER_STREAM_END',
0x01D7: 'GAME_SMSG_PARTY_DEFEATED',
0x01D8: 'GAME_SMSG_PARTY_LOCK',
0x01DA: 'GAME_SMSG_PARTY_SEARCH_REQUEST_JOIN',
0x01DB: 'GAME_SMSG_PARTY_SEARCH_REQUEST_DONE',
0x01DC: 'GAME_SMSG_PARTY_SEARCH_ADVERTISEMENT',
0x01DD: 'GAME_SMSG_PARTY_SEARCH_SEEK',
0x01DE: 'GAME_SMSG_PARTY_SEARCH_REMOVE',
0x01DF: 'GAME_SMSG_PARTY_SEARCH_SIZE',
0x01E0: 'GAME_SMSG_PARTY_SEARCH_TYPE',
}