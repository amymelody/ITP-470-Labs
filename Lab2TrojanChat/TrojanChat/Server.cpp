#include "stdafx.h"

void Server::Run(USHORT inPort)
{
	WindowsConsole* wc = WindowsConsole::Instance();
	wc->FlushInputBuffer();

	wc->Write(L"Starting As Server!\n");

	shared_ptr<TCPSocket> socket = TCPSocketUtil::CreateSocket(inPort);
	if (socket == NULL) {
		return;
	}

	if (socket->Listen() == SOCKET_ERROR) {
		LOG(L"Error Listening: %d", GetLastError());
		return;
	}

	for (;;)
	{
		FD_ZERO(&readSet);
		FD_SET(socket->GetSocket(), &readSet);
		for each (ClientProxy* clientProxy in clientProxies) {
			FD_SET(clientProxy->GetTCPSocket()->GetSocket(), &readSet);
		}

		int selectResult = TCPSocketUtil::Select(&readSet, NULL, NULL);
		if (selectResult < 0) {
			LOG(L"Error Selecting For Read Access: %d", GetLastError());
		}
		if (selectResult > 0) {
			if (FD_ISSET(socket->GetSocket(), &readSet)) {
				shared_ptr<TCPSocket> clientSocket = socket->Accept();
				if (clientSocket) {
					ClientProxy* cp = new ClientProxy(clientSocket);
					clientProxies.push_back(cp);
				}
			}

			for (int i = clientProxies.size() - 1; i >= 0; i--) {
				if (FD_ISSET(clientProxies[i]->GetTCPSocket()->GetSocket(), &readSet)) {
					char data[64];
					int size = clientProxies[i]->GetTCPSocket()->Receive(data, sizeof(data), 0);
					if (size == SOCKET_ERROR) {
						LOG(L"Client disconnected: %d", GetLastError());
						clientProxies.erase(clientProxies.begin() + i);
					}
					if (size != SOCKET_ERROR) {
						if (clientProxies[i]->name.empty()) {
							string nameStr(data);
							nameStr.resize(size);
							clientProxies[i]->name = nameStr;
						}
						else {
							FD_ZERO(&writeSet);
							for each (ClientProxy* cp in clientProxies) {
								FD_SET(cp->GetTCPSocket()->GetSocket(), &writeSet);
							}
							selectResult = TCPSocketUtil::Select(NULL, &writeSet, NULL);
							if (selectResult < 0) {
								LOG(L"Error Selecting For Write Access: %d", GetLastError());
							}
							if (selectResult > 0) {
								string dataStr(data);
								dataStr.resize(size);

								bool privateMessage = false;
								if (dataStr[0] == '`') {
									string msgStr(dataStr);
									msgStr.erase(0, 1);
									string nameStr(msgStr);
									for (int j = 0; j < msgStr.length(); j++) {
										if (msgStr[j] == ' ') {
											msgStr.erase(0, j + 1);
											nameStr.erase(j, msgStr.length() + 1);
											break;
										}
									}

									for each (ClientProxy* cp in clientProxies) {
										if (cp->name == nameStr) {
											privateMessage = true;
											msgStr = clientProxies[i]->name + " (private): " + msgStr;
											if (FD_ISSET(cp->GetTCPSocket()->GetSocket(), &writeSet)) {
												if (cp->GetTCPSocket()->Send(msgStr.c_str(), msgStr.length(), 0) == SOCKET_ERROR) {
													LOG(L"Error Sending: %d", GetLastError());
												}
											}
										}
									}
								}
								if (!privateMessage) {
									dataStr = clientProxies[i]->name + ": " + dataStr;
									for each (ClientProxy* cp in clientProxies) {
										if (FD_ISSET(cp->GetTCPSocket()->GetSocket(), &writeSet)) {
											if (cp->GetTCPSocket()->Send(dataStr.c_str(), dataStr.length(), 0) == SOCKET_ERROR) {
												LOG(L"Error Sending: %d", GetLastError());
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}