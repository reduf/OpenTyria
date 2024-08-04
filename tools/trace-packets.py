import sys
from process import *
from datetime import datetime

packet_names = {
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
    0x0185: 'GAME_SMSG_INSTANCE_EARLY_PACKET',
    0x0188: 'GAME_SMSG_INSTANCE_CHAR_CREATION_START',
    0x0189: 'GAME_SMSG_INSTANCE_CHAR_CREATION_READY',
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

cmsg_names = {
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
0x0087: 'GAME_SMSG_INSTANCE_CHAR_CREATION_START_RECV',
0x0088: 'GAME_SMSG_INSTANCE_CHAR_CREATION_READY_RECV',
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

item_types = {
    0: 'ItemType_Salvage',
    1: 'ItemType_Leadhand',
    2: 'ItemType_Axe',
    3: 'ItemType_Bag',
    4: 'ItemType_Feet',
    5: 'ItemType_Bow',
    6: 'ItemType_Bundle',
    7: 'ItemType_Chest',
    8: 'ItemType_Rune',
    9: 'ItemType_Consumable',
    10: 'ItemType_Dye',
    11: 'ItemType_Material',
    12: 'ItemType_Focus',
    13: 'ItemType_Arms',
    14: 'ItemType_Sigil',
    15: 'ItemType_Hammer',
    16: 'ItemType_Head',
    17: 'ItemType_SalvageItem',
    18: 'ItemType_Key',
    19: 'ItemType_Legs',
    20: 'ItemType_Coins',
    21: 'ItemType_QuestItem',
    22: 'ItemType_Wand',
    24: 'ItemType_Shield',
    26: 'ItemType_Staff',
    27: 'ItemType_Sword',
    29: 'ItemType_Kit',
    30: 'ItemType_Trophy',
    31: 'ItemType_Scroll',
    32: 'ItemType_Daggers',
    33: 'ItemType_Present',
    34: 'ItemType_Minipet',
    35: 'ItemType_Scythe',
    36: 'ItemType_Spear',
    43: 'ItemType_Handbook',
    44: 'ItemType_CostumeBody',
    45: 'ItemType_CostumeHead',
}

dye_colors = {
    0: 'DyeColor_None',
    2: 'DyeColor_Blue',
    3: 'DyeColor_Green',
    4: 'DyeColor_Purple',
    5: 'DyeColor_Red',
    6: 'DyeColor_Yellow',
    7: 'DyeColor_Brown',
    8: 'DyeColor_Orange',
    9: 'DyeColor_Silver',
    10: 'DyeColor_Black',
    11: 'DyeColor_Gray',
    12: 'DyeColor_White',
    13: 'DyeColor_Pink',
}

professions = {
    0 : 'Profession_None',
    1 : 'Profession_Warrior',
    2 : 'Profession_Ranger',
    3 : 'Profession_Monk',
    4 : 'Profession_Necromancer',
    5 : 'Profession_Mesmer',
    6 : 'Profession_Elementalist',
    7 : 'Profession_Assassin',
    8 : 'Profession_Ritualist',
    9 : 'Profession_Paragon',
    10 : 'Profession_Dervish',
}

campaign_types = {
    0: 'CampaignType_Pvp',
    1: 'CampaignType_Prophecies',
    2: 'CampaignType_Faction',
    3: 'CampaignType_Nightfall',
}

def main(args):
    if (2 ** 32) < sys.maxsize:
        print('Use a 32 bits version of Python')
        sys.exit(1)

    # proc = Process(7208)
    proc, = GetProcesses('Gw.exe')
    scanner = ProcessScanner(proc)
    smsg_addr = scanner.find(b'\x50\x8B\x41\x08\xFF\xD0\x83\xC4\x08', 4)
    cmsg_addr = scanner.find(b'\xC7\x47\x54\x01\x00\x00\x00\x1B\xC9\x81\xE1\x00\x80', -0xC5)

    running = True
    def signal_handler(sig, frame):
        global running
        running = False

    @Hook.stdcall(LPVOID, DWORD, LPVOID)
    def on_send_packet(ctx, size, packet):
        header, = proc.read(packet, 'I')
        if header in cmsg_names:
            name = cmsg_names[header]
        else:
            name = "unknown"
        # now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        print(f'SendPacket: {header}, 0x{header:X}, {name}')
        if header == 94:
            campaign_type, profession = proc.read(packet + 4, 'II')
            if profession in professions:
                profession = professions[profession]
            if campaign_type in campaign_types:
                campaign_type = campaign_types[campaign_type]
            print(f'>> profession = {profession}, campaign_type = {campaign_type}')
        if header == 130:
            param1, param2 = proc.read(packet + 4, 'II')
            print(f'>> param1 = {param1}, param2 = {param2}')

    @Hook.rawcall
    def on_recv_packet(ctx):
        packet, = proc.read(ctx.Esp, 'I')
        header, = proc.read(packet, 'I')
        if header in packet_names:
            name = packet_names[header]
        else:
            name = "unknown"

        # now = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        # print(f'RecvPacket: {header}, 0x{header:X}, {name}')

        # if header == 26:
        #     _, val, count, *values = proc.read(packet, 'III64I')
        #     temp = ', '.join(f'{val:X}' for val in values[:count])
        #     print(f'  {val}, {count}, {temp}')
        # if header == 0x000F:
        #     _, val1, val2, val3 = proc.read(packet, 'IIII')
        #     print(f'  {val1}, {val2}, {val3},')
        # if name in ('GAME_SMSG_PLAYER_ATTR_MAX_KURZICK', 'GAME_SMSG_PLAYER_ATTR_MAX_LUXON', 'GAME_SMSG_PLAYER_ATTR_MAX_BALTHAZAR', 'GAME_SMSG_PLAYER_ATTR_MAX_IMPERIAL'):
        #     value, = proc.read(packet + 4, 'I')
        #     print(f'{name} -> {value}')
        # if name == 'GAME_SMSG_PING_REPLY':
        #     val = proc.read(packet + 4, 'I')
        #     print(f'>> {val}')
        # if name == 'GAME_SMSG_AGENT_ATTR_UPDATE_INT':
        #     attr_id, agent_id, value = proc.read(packet + 4, 'III')
        #     print(f'>>> (int) attr_id: {attr_id}, agent_id: {agent_id}, value: {value}')
        # if name == 'GAME_SMSG_ITEM_SET_PROFESSION':
        #     a, b = proc.read(packet + 4, 'II')
        #     print(f'a = {a}, b = {b}')
        # elif name == 'GAME_SMSG_AGENT_ATTR_UPDATE_FLOAT':
        #     attr_id, agent_id, value = proc.read(packet + 4, 'III')
        #     print(f'>>> (float) attr_id: {attr_id}, agent_id: {agent_id}, value: {value}')
        # if name == 'GAME_SMSG_INVENTORY_CREATE_BAG':
        #     stream_id, bag_type, bag_model_id, bag_id, slot_count, assoc_item_id = proc.read(packet + 4, 'IIIIII')
        #     print(f'>> stream_id = {stream_id}, bag_type = {bag_type}, bag_model_id = {bag_model_id}, bag_id = {bag_id}, slot_count = {slot_count}, assoc_item_id = {assoc_item_id}')
        # if name == 'GAME_SMSG_ITEM_MOVED_TO_LOCATION':
        #     stream_id, item_id, bag_id, slot = proc.read(packet + 4, 'IIII')
        #     print(f'>> stream_id = {stream_id}, item_id = {item_id}, bag_id = {bag_id}, slot = {slot}')
        # if name == 'GAME_SMSG_ITEM_CHANGE_LOCATION':
        #     unk1, item_id, bag_id, slot = proc.read(packet + 4, 'IIII')
        #     print(f'>> unk1 = {unk1}, item_id = {item_id}, bag_id = {bag_id}, slot = {slot}')
        # if name == 'GAME_SMSG_ITEM_REMOVE':
        #     a, b = proc.read(packet + 4, 'II')
        #     print(f'>> {a}, {b}')
        # if name == 'GAME_SMSG_ITEM_STREAM_CREATE':
        #     stream_id = proc.read(packet + 4, 'II')
        #     print(f'stream_id = {stream_id}')
        if name == 'GAME_SMSG_ITEM_GENERAL_INFO':
            item_id, file_id, item_type, unk0, dye_color, materials, unk1, flags, value, model, quantity, name, n_modifier, *modifier = proc.read(packet + 4, 'IIIIIIIIIII128sI64I')
            name = name[:name.index(b'\0\0')]
            name = tuple(name[i] + (name[i+1] << 8) for i in range(0, len(name), 2))
            name_str = ', '.join(f'0x{val:X}' for val in name)
            name = f'{{{len(name)}, {{{name_str}}}}}'
            modifier = modifier[:n_modifier]
            modifier_str = ', '.join(f'0x{val:X}' for val in modifier)
            modifier = f'{{{len(modifier)}, {{{modifier_str}}}}}'
            item_type = item_types[item_type]
            dye_color = dye_colors[dye_color]
            if dye_color in ('DyeColor_None', 'DyeColor_Gray'):
                print(f'{{\n    .file_id = 0x{file_id:X},\n    .item_type = {item_type},\n    .unk0 = {unk0},\n    .dye_color = {dye_color},\n    .materials = {materials},\n    .unk1 = {unk1},\n    .flags = 0x{flags:X},\n    .value = {value},\n    .model = {model},\n    .quantity = {quantity},\n    .name = {name},\n    .modifiers = {modifier},\n}},')

    with ProcessDebugger(proc) as dbg:
        dbg.add_hook(smsg_addr, on_recv_packet)
        dbg.add_hook(cmsg_addr, on_send_packet)
        print(f'Start debugging process {proc.name}, {proc.id}')
        while running:
            dbg.poll(32)


if __name__ == '__main__':
    main(sys.argv[1::])
