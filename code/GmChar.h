#pragma once

typedef enum Sex {
    Sex_Male,
    Sex_Female,
} Sex;

typedef enum Profession {
    Profession_None                 = 0,
    Profession_Warrior              = 1,
    Profession_Ranger               = 2,
    Profession_Monk                 = 3,
    Profession_Necromancer          = 4,
    Profession_Mesmer               = 5,
    Profession_Elementalist         = 6,
    Profession_Assassin             = 7,
    Profession_Ritualist            = 8,
    Profession_Paragon              = 9,
    Profession_Dervish              = 10,
    Profession_Count
} Profession;

int Profession_FromInt(int value, Profession *result)
{
    switch (value) {
    case Profession_None:
    case Profession_Warrior:
    case Profession_Ranger:
    case Profession_Monk:
    case Profession_Necromancer:
    case Profession_Mesmer:
    case Profession_Elementalist:
    case Profession_Assassin:
    case Profession_Ritualist:
    case Profession_Paragon:
    case Profession_Dervish:
        *result = (Profession) value;
        return ERR_OK;
    default:
        return ERR_UNSUCCESSFUL;
    }
}

typedef enum Campaign {
    Campaign_None             = 0,
    Campaign_Prophecies       = 1,
    Campaign_Factions         = 2,
    Campaign_Nightfall        = 3,
    Campaign_EyeOfTheNorth    = 4,
    Campaign_BonusMissionPack = 5,
} Campaign;

int Campaign_FromInt(int value, Campaign *result)
{
    switch (value) {
    case Campaign_None:
    case Campaign_Prophecies:
    case Campaign_Factions:
    case Campaign_Nightfall:
    case Campaign_EyeOfTheNorth:
    case Campaign_BonusMissionPack:
        *result = (Campaign) value;
        return ERR_OK;
    default:
        return ERR_UNSUCCESSFUL;
    }
}

typedef enum HelmStatus {
    HelmStatus_None,
    HelmStatus_Show,
    HelmStatus_Hide,
} HelmStatus;

#define HEIGHT_BITS     4
#define SKIN_COLOR_BITS 5
#define HAIR_COLOR_BITS 5
#define FACE_STYLE_BITS 5
#define HAIR_STYLE_BITS 6
#define LEVEL_BITS      5

#define HEIGHT_MAX     (1 << HEIGHT_BITS) - 1
#define SKIN_COLOR_MAX (1 << SKIN_COLOR_BITS) - 1
#define HAIR_COLOR_MAX (1 << HAIR_COLOR_BITS) - 1
#define FACE_STYLE_MAX (1 << FACE_STYLE_BITS) - 1
#define HAIR_STYLE_MAX (1 << HAIR_STYLE_BITS) - 1
#define LEVEL_MAX      (1 << LEVEL_BITS) - 1

typedef enum DyeColor {
    DyeColor_None = 0,
    DyeColor_Blue = 2,
    DyeColor_Green = 3,
    DyeColor_Purple = 4,
    DyeColor_Red = 5,
    DyeColor_Yellow = 6,
    DyeColor_Brown = 7,
    DyeColor_Orange = 8,
    DyeColor_Silver = 9,
    DyeColor_Black = 10,
    DyeColor_Gray = 11,
    DyeColor_White = 12,
    DyeColor_Pink = 13
} DyeColor;

int DyeColor_FromInt(int value, DyeColor *result)
{
    switch (value) {
    case DyeColor_None:
    case DyeColor_Blue:
    case DyeColor_Green:
    case DyeColor_Purple:
    case DyeColor_Red:
    case DyeColor_Yellow:
    case DyeColor_Brown:
    case DyeColor_Orange:
    case DyeColor_Silver:
    case DyeColor_Black:
    case DyeColor_Gray:
    case DyeColor_White:
    case DyeColor_Pink:
        *result = (DyeColor)value;
        return ERR_OK;
    default:
        return ERR_UNSUCCESSFUL;
    }
}

typedef struct Appearance {
    unsigned int sex                : 1;
    unsigned int height             : HEIGHT_BITS;
    unsigned int skin_color         : SKIN_COLOR_BITS;
    unsigned int hair_color         : HAIR_COLOR_BITS;
    unsigned int face_style         : FACE_STYLE_BITS;
    unsigned int primary_profession : 4;
    unsigned int hair_style         : HAIR_STYLE_BITS;
    unsigned int campaign           : 2; // 0=prof, 1=faction, 2=nightfall
} Appearance;
STATIC_ASSERT(sizeof(Appearance) == 4);

#define CHARACTER_SETTINGS_VERSION 6

#pragma pack(push, 1)
typedef struct CharacterSettings {
    uint16_t     version;           // currently it's always 6
    uint16_t     last_outpost;
    uint32_t     last_time_played;  // not sure what is the value, but it changes everytime you play and playing with the same character on the same day almost always result to the same id.
    Appearance   appearance;
    uint8_t      guild_hall_id[16];
    uint16_t     campaign             : 4; // 0=pvp, 1=prod, 2=faction, 3=nightfall
    uint16_t     level                : LEVEL_BITS;
    uint16_t     is_pvp               : 1;
    uint16_t     secondary_profession : 4;
    uint16_t     helm_status          : 2;
    uint16_t     h001E;               // Seems to always be \xDD\xDD or \xAD\xBA aka BAAD (on purpose?)
    uint8_t      number_of_pieces;
    uint8_t      h0021[4];            // Can be \xDD\xDD\xDD\xDD or \xF0\xAD\xBA\x0D
    struct
    {
        uint16_t     file_id;
        uint8_t      col1             : 4;
        uint8_t      col2             : 4;
        uint8_t      zero;            // always 0 for some reason
        uint8_t      col3             : 4;
        uint8_t      col4             : 4;
    } pieces[5]; // The number of pieces is defined by `number_of_pieces`.
} CharacterSettings;
#pragma pack(pop)
