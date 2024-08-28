#pragma once

#define TitleDefs \
    X(Hero, 0) \
    X(TyrianCarto, 1) \
    X(CanthanCarto, 2) \
    X(Gladiator, 3) \
    X(Champion, 4) \
    X(Kurzick, 5) \
    X(Luxon, 6) \
    X(Drunkard, 7) \
    X(Deprecated_SkillHunter, 8) \
    X(Survivor, 9) \
    X(KoaBD, 10) \
    X(Deprecated_TreasureHunter, 11) \
    X(Deprecated_Wisdom, 12) \
    X(ProtectorTyria, 13) \
    X(ProtectorCantha, 14) \
    X(Lucky, 15) \
    X(Unlucky, 16) \
    X(Sunspear, 17) \
    X(ElonianCarto, 18) \
    X(ProtectorElona, 19) \
    X(Lightbringer, 20) \
    X(LDoA, 21) \
    X(Commander, 22) \
    X(Gamer, 23) \
    X(SkillHunterTyria, 24) \
    X(VanquisherTyria, 25) \
    X(SkillHunterCantha, 26) \
    X(VanquisherCantha, 27) \
    X(SkillHunterElona, 28) \
    X(VanquisherElona, 29) \
    X(LegendaryCarto, 30) \
    X(LegendaryGuardian, 31) \
    X(LegendarySkillHunter, 32) \
    X(LegendaryVanquisher, 33) \
    X(Sweets, 34) \
    X(GuardianTyria, 35) \
    X(GuardianCantha, 36) \
    X(GuardianElona, 37) \
    X(Asuran, 38) \
    X(Deldrimor, 39) \
    X(Vanguard, 40) \
    X(Norn, 41) \
    X(MasterOfTheNorth, 42) \
    X(Party, 43) \
    X(Zaishen, 44) \
    X(TreasureHunter, 45) \
    X(Wisdom, 46) \
    X(Codex, 47) \

typedef enum Title {
    #define X(name, val) name = val,
    TitleDefs
    #undef X
    Title_Count,
} Title;

const char* Title_ToString(Title title)
{
    switch (title) {
    #define X(name, val) case name: return #name;
    TitleDefs
    #undef X
    default: abort();
    }
}

int Title_FromInt(int title, Title *result)
{
    switch (title) {
    #define X(name, val) case name: break;
    TitleDefs
    #undef X
    default:
        return ERR_UNSUCCESSFUL;
    }
    *result = (Title) title;
    return ERR_OK;
}

