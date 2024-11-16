#define _CRT_SECURE_NO_WARNINGS

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>

#include <Windows.h>

#define UUID_NODE_LEN   6

#define DllExport __declspec(dllexport)

struct uuid {
    uint32_t time_low;
    uint16_t time_mid;
    uint16_t time_hi_and_version;
    uint8_t  clock_seq_hi_and_reserved;
    uint8_t  clock_seq_low;
    uint8_t  node[UUID_NODE_LEN];
};

typedef void (*OnPortalNotify_t)(uint32_t msgid, uint32_t size, void *data, void *param);
OnPortalNotify_t g_NotifyCb;
void* g_Param;

struct uuid g_UserId;
struct uuid g_SessionId;

static bool s_parse_uuid(struct uuid *result, const char *ptr)
{
    size_t len = strlen(ptr);
    // This check means that the `sscanf` is safe.
    if (len != sizeof("AABBCCDD-AABB-AABB-AABB-AABBCCDDEEFF") - 1) {
        return false;
    }

    int ret = sscanf(
        ptr,
        "%08" SCNx32 "-%04" SCNx16 "-%04" SCNx16 "-%02hhx%02hhx-%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx",
        &result->time_low, &result->time_mid, &result->time_hi_and_version,
        &result->clock_seq_hi_and_reserved, &result->clock_seq_low, &result->node[0],
        &result->node[1], &result->node[2], &result->node[3], &result->node[4], &result->node[5]);

    if (ret != 11)
        return false;

    return true;
}

DllExport void PortalInitialize(uint32_t port)
{
    (void)port;
    s_parse_uuid(&g_UserId, "fa520ee2-4419-4eb4-ae49-6e9abe6ef24f");
    s_parse_uuid(&g_SessionId, "d8b9bf5d-90b1-4cbd-9b76-88da7be763b6");
}

DllExport void PortalInitializeWithParams(uint32_t one, uint32_t maybe_version, uint32_t zero, uint32_t port, const wchar_t *client_name)
{
}

DllExport void PortalStartCleanup()
{
}

DllExport void PortalLoginSecondaryAuth(const wchar_t* code)
{
    (void)code;
}

DllExport void PortalDestroy(void)
{
}

DllExport void PortalRegisterNotify(OnPortalNotify_t cb, void *param)
{
    g_NotifyCb = cb;
    g_Param = param;
}

DllExport void PortalLogin(const wchar_t *email, const wchar_t *password, const wchar_t *region)
{
    typedef struct Message {
        uint32_t error;
        uint32_t h0004;
    } Message;

    if (wcscmp(email, L"user1@example.com") == 0) {
        s_parse_uuid(&g_UserId, "fa520ee2-4419-4eb4-ae49-6e9abe6ef24f");
        s_parse_uuid(&g_SessionId, "d8b9bf5d-90b1-4cbd-9b76-88da7be763b6");
    } else if (wcscmp(email, L"user2@example.com") == 0) {
        s_parse_uuid(&g_UserId, "5fb222bc-9d19-4308-ac3e-3c62685bc6ae");
        s_parse_uuid(&g_SessionId, "224aee30-d8a5-4f18-ad8e-b5fcbf121c4e");
    } else if (wcscmp(email, L"user3@example.com") == 0) {
        s_parse_uuid(&g_UserId, "9957c79c-b93c-4b60-9d84-9f38a1f62be3");
        s_parse_uuid(&g_SessionId, "ccdb4b8d-1c88-48c0-bda0-0a5e8d87f4da");
    }

    Message msg = {
        .error = 0,
        .h0004 = 0,
    };

    g_NotifyCb(0, sizeof(msg), &msg, g_Param);
}

DllExport void PortalCancelLogin()
{
    
}

DllExport void PortalGetGameAccountList()
{
}

DllExport void PortalListGameAccounts(const wchar_t *game)
{
    typedef struct Message {
        uint32_t error;
        uint16_t h0004; // 1
        uint16_t h0006; // 1082 (0x043A)
        const wchar_t **game; 
        uint32_t numberOfAccounts; // Should be 1 for it to work.
    } Message;

    static const wchar_t *Gw1Name = L"Gw1";

    Message msg = {
        .error = 0,
        .h0004 = 1,
        .h0006 = 0x043A,
        .game = &Gw1Name,
        .numberOfAccounts = 1,
    };

    g_NotifyCb(4, sizeof(msg), &msg, g_Param);
}

DllExport void PortalRequestGameToken(const wchar_t *game1, const wchar_t *game2)
{
    typedef struct Message {
        uint32_t error;
        uint16_t h0004; // 1
        uint16_t h0006; // 1023 (0x03FF)
        uint8_t  session_id[16];
    } Message;

    Message msg = {
        .error = 0,
        .h0004 = 1,
        .h0006 = 1023,
    };

    memcpy(msg.session_id, &g_SessionId, sizeof(g_SessionId));
    g_NotifyCb(2, sizeof(msg), &msg, g_Param);
}

DllExport uint8_t* PortalGetUserId(void)
{
    return (uint8_t *)&g_UserId;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    return TRUE;
}
