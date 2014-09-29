#include <TrojanBlastClientPCH.h>
#include "Client.h"
#include "UDPSocketUtil.h"
#include <Ws2tcpip.h>

int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );

	wstring ipStr(lpCmdLine);
	string portStr(ipStr.begin(), ipStr.end());

	for (unsigned int i = 0; i < ipStr.length(); i++) {
		if (ipStr[i] == ':') {
			portStr.erase(0, i + 1);
			ipStr.erase(i, portStr.length()+1);
			break;
		}
	}

	string nameStr(portStr.begin(), portStr.end());

	for (unsigned int i = 0; i < nameStr.length(); i++) {
		if (nameStr[i] == ' ') {
			nameStr.erase(0, i + 1);
			portStr.erase(i, nameStr.length());
			break;
		}
	}

	USHORT inPort = atoi(portStr.c_str());

	ULONG ipAddress;
	InetPton(AF_INET, ipStr.c_str(), &ipAddress);


	if( Client::StaticInit( hInstance, nCmdShow, htons(inPort), ipAddress, nameStr ) )
	{
		UDPSocketUtil::StartUp();
		return Client::sInstance->Run();
		UDPSocketUtil::CleanUp();
	}
	else
	{
		//error
		return 1;
	}

}

