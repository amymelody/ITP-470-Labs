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
					clientProxies.push_back(new ClientProxy(clientSocket));
				}
			}

			for each (ClientProxy* clientProxy in clientProxies) {
				if (FD_ISSET(clientProxy->GetTCPSocket()->GetSocket(), &readSet)) {
					char data[64];
					int size = clientProxy->GetTCPSocket()->Receive(data, sizeof(data), 0);
					if (size != SOCKET_ERROR) {
						if (clientProxy->name.empty()) {
							string nameStr(data);
							nameStr.resize(size);
							clientProxy->name = nameStr;
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

								if (dataStr[0] == '`') {
									dataStr.erase(0, 1);
									string nameStr(dataStr);
									for (int i = 0; i < dataStr.length(); i++) {
										if (dataStr[i] == ' ') {
											dataStr.erase(0, i + 1);
											nameStr.erase(i, dataStr.length()+1);
											break;
										}
									}

									for each (ClientProxy* cp in clientProxies) {
										if (cp->name == nameStr) {
											dataStr = clientProxy->name + " (private): " + dataStr;
											if (FD_ISSET(cp->GetTCPSocket()->GetSocket(), &writeSet)) {
												if (cp->GetTCPSocket()->Send(dataStr.c_str(), dataStr.length(), 0) == SOCKET_ERROR) {
													LOG(L"Error Sending: %d", GetLastError());
												}
											}
										}
									}
								}
								else {
									dataStr = clientProxy->name + ": " + dataStr;
									for each (ClientProxy* clientProxy in clientProxies) {
										if (FD_ISSET(clientProxy->GetTCPSocket()->GetSocket(), &writeSet)) {
											if (clientProxy->GetTCPSocket()->Send(dataStr.c_str(), dataStr.length(), 0) == SOCKET_ERROR) {
												LOG(L"Error Sending: %d", GetLastError());
											}
										}
									}
								}
							}
						}

					/*	string dataStr(data);
						dataStr.resize(size);
						std::wstringstream wData;
						wData << dataStr.c_str();

						wc->Write(wData.str().c_str());*/
					}
				}
			}
		}
	}
}