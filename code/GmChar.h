#pragma once

typedef enum Sex {
    Sex_Male,
    Sex_Female,
} Sex;

typedef enum Campaign {
    Campaign_Prophecies,
    Campaign_Faction,
    Campaign_Nightfall,
} Campaign;

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
#define LEVEL_BITS      6

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

#pragma pack(push, 1)
typedef struct CharacterSettings {
    uint16_t     version;           // currently it's always 6
    uint16_t     last_outpost;
    uint32_t     last_time_played;  // not sure what is the value, but it changes everytime you play and playing with the same character on the same day almost always result to the same id.
    uint32_t     sex                  : 1;
    uint32_t     height               : HEIGHT_BITS;
    uint32_t     skin_color           : SKIN_COLOR_BITS;
    uint32_t     hair_color           : HAIR_COLOR_BITS;
    uint32_t     face_style           : FACE_STYLE_BITS;
    uint32_t     primary_profession   : 4;
    uint32_t     hair_style           : HAIR_STYLE_BITS;
    uint32_t     campaign1            : 2; // 0=prof, 1=faction, 2=nightfall
    uint8_t      guild_hall_id[16];
    uint16_t     campaign2            : 4;
    uint16_t     level                : LEVEL_BITS;
    uint16_t     secondary_profession : 4;
    uint16_t     helm_status          : 2;
    uint16_t     h001E;               // Seems to always be \xDD\xDD or \xAD\xBA aka BAAD (on purpose?)
    uint8_t      number_of_pieces;
    uint8_t      h0021[4];            // Can be \xDD\xDD\xDD\xDD or \xF0\xAD\xBA\x0D
    struct
    {
        uint16_t     piece_id;
        uint8_t      col1             : 4;
        uint8_t      col2             : 4;
        uint8_t      zero;            // always 0 for some reason
        uint8_t      col3             : 4;
        uint8_t      col4             : 4;
    } pieces[5]; // The number of pieces is defined by `number_of_pieces`.
} CharacterSettings;
#pragma pack(pop)
