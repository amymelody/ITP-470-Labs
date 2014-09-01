// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include <cstdio>
#include <tchar.h>
#include <vector>
#include <memory>
#include <cstdint>
#include <sstream>

using std::shared_ptr;
using std::wstring;
using std::string;
using std::unique_ptr;

#include "WindowsConsole.h"
#include "Server.h"
#include "Client.h"
#include "TCPSocket.h"
#include "TCPSocketUtil.h"
#include "StringUtils.h"
