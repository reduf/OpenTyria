#ifndef STRICT
# define STRICT
#endif

#ifndef NOMINMAX
# define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
#endif

#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
# pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
# pragma warning(disable: 4214) // nonstandard extension used: bit field types other than int
# pragma comment(lib, "Ws2_32.lib")
# include <Windows.h>
# include <Winsock2.h>
# include <Ws2tcpip.h>
# include <bcrypt.h>
# include <winternl.h>
#else // _WIN32
#endif

#include <assert.h>
#include <inttypes.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mbedtls/bignum.h>
#include <mbedtls/chacha20.h>

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

#include "sqlite3.h"

// arc4 was ported from mbedtls when they removed the implementation.
#include "arc4.h"
#include "arc4.c"

#include "macros.h"
#include "helper.c"
#include "int.h"

#include "endian.h"
#include "uuid.h"
#include "array.h"
#include "slice.h"
#include "errors.h"

#include "stream.h"
#include "iocp.h"
#include "sys.h"
#include "logs.h"
#include "random.h"
#include "network.h"

#include "DbSchema.h"

#include "GmErrors.h"
#include "GmMaps.h"
#include "GmChar.h"
#include "GmFriend.h"
#include "GmMap.h"
#include "GmInventory.h"
#include "GmPlayer.h"
#include "GmAgent.h"
#include "GmItem.h"
#include "GmDefaultArmors.h"

#include "Db.h"

#include "opcodes.h"
#include "msgdefs.h"
#include "msgpack.h"
#include "proto.h"

#include "GameMsg.h"
#include "GameSrv.h"

#include "AuthMsg.h"
#include "AuthSrv.h"

#if !defined(COMPILE_TESTS)
#include "main.c"
#endif

#include "array.c"
#include "AuthSrv.c"
#include "Db.c"
#include "GameSrv.c"
#include "GmAgent.c"
#include "GmDefaultArmors.c"
#include "GmInventory.c"
#include "int.c"
#include "logs.c"
#include "msgdefs.c"
#include "msgpack.c"
#include "network.c"
#include "random.c"
#include "stream.c"
#include "win32/iocp_win32.c"
#include "win32/sys_win32.c"

#if defined(COMPILE_TESTS)
#include "tests.c"

#endif
