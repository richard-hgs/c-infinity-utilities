#include "pch.h"

#include <string>
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>
#include <map>
#include <chrono>
#include <thread>
#include <fcntl.h>	//fcntl

#include "TemplateUtils.h"

#include "StringUtils.h"

#include "MemoryTools.h"    // Must be the last
#include "TcpUtils.h"

CRITICAL_SECTION critsec;

int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	// This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
	// until 00:00:00 January 1, 1970 
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}

/*
	Receive data in multiple chunks by checking a non-blocking socket
	Timeout in seconds
*/
int recvTimeout(SOCKET mSocket, double timeout) {
	const int CHUNK_SIZE = 500;
	int size_recv, total_size = 0;
	struct timeval begin, now;
	char chunk[CHUNK_SIZE];
	double timediff;

	//make socket non blocking
	// fcntl(s, F_SETFL, O_NONBLOCK);

	//-------------------------
	// Set the socket I/O mode: In this case FIONBIO
	// enables or disables the blocking mode for the 
	// socket based on the numerical value of iMode.
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	u_long iMode = 1;
	int iResult = 0;
	iResult = ioctlsocket(mSocket, FIONBIO, &iMode);
	if (iResult != NO_ERROR) {
		printf("ioctlsocket failed with error: %ld\n", iResult);
	}

	//beginning time
	gettimeofday(&begin, NULL);

	while (1) {
		gettimeofday(&now, NULL);

		//time elapsed in seconds
		timediff = ((double)now.tv_sec - (double)begin.tv_sec) + 1e-6 * ((double)now.tv_usec - (double)begin.tv_usec);

		//if you got some data, then break after timeout
		if (total_size > 0 && timediff > timeout) {
			break;
		} else if (timediff > timeout * 2) {
			//if you got no data at all, wait a little longer, twice the timeout
			break;
		}

		memset(chunk, 0, CHUNK_SIZE);	//clear the variable
		if ((size_recv = recv(mSocket, chunk, CHUNK_SIZE, 0)) < 0) {
			//if nothing was received then we want to wait a little before trying again, 0.1 seconds
			// usleep(100000);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		} else {
			total_size += size_recv;
			printf("%s", chunk);
			//reset beginning time
			gettimeofday(&begin, NULL);
		}
	}

	return total_size;
}

char* TcpUtils::sendRequest2(std::string location, std::string message, size_t replyBuffSize, u_short port) {
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char* server_reply = (char*) malloc(replyBuffSize * sizeof(char));
	int recv_size;

	//Initilasing the winsock libraray
	// std::cout << "Initialising Winsock..." << std::endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		// std::cout << "Failed. Error Code " << WSAGetLastError() << std::endl;
		std::string errorMsg = "{\"status\":400, \"msg\":\"Failed. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}
	// std::cout << "Initialised." << std::endl;


	/*
	Function socket creates the socket and returns the socket descriptor
	First arguement is address version.AF_INET is the Internet Protocol version 4 (IPv4) address family.
	Second arguemnet is to specify what kind of connection we are establishing. SOCK_STREAM is TCP. SOCK_DGRAM is for UDP
	Third arguemnet is for specifying which protocol to be used.
	*/

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		// std::cout << "Could not create socket beacuse of " << WSAGetLastError() << std::endl;
		std::string errorMsg = "{\"status\":400, \"msg\":\"Could not create socket beacuse of. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		WSACleanup();
		return server_reply;
	}
	// std::cout << "Socket created" << std::endl;

	//Local server
	// server.sin_addr.s_addr = inet_addr("127.0.0.1");// inet_addr(cIP);
	// LPWSTR lpLocation;
	// StringUtils::toLpwstr(location, lpLocation);

	InetPton(AF_INET, L"127.0.0.1", &server.sin_addr.s_addr);
	// server.sin_addr.s_net;
	server.sin_family = AF_INET;
	server.sin_port = htons(50000);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
		// std::cout << "connect error" << std::endl;
		closesocket(s);
		WSACleanup();

		std::string errorMsg = "{\"status\":400, \"msg\":\"Connect error. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}

	// std::cout << "Connected" << std::endl;

	//Send some data
	if (send(s, message.c_str(), message.length(), NULL) < 0) {
		std::cout << "Send failed" << std::endl;
		closesocket(s);
		WSACleanup();

		std::string errorMsg = "{\"status\":400, \"msg\":\"Send failed. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}
	// std::cout << "Data Send" << std::endl;

	// recv_size = recvTimeout(s, 1000);

	//Receive a reply from the server
	if ((recv_size = recv(s, server_reply, replyBuffSize, 0)) == SOCKET_ERROR) {
		// std::cout << "recv failed" << std::endl;
		closesocket(s);
		WSACleanup();

		std::string errorMsg = "{\"status\":400, \"msg\":\"Receive failed. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}

	// std::cout << "Reply received" << std::endl;

	//Add a NULL terminating character to make it a proper string before printing
	server_reply[recv_size] = '\0';


	std::cout << server_reply << std::endl;

	closesocket(s);
	WSACleanup();

	return server_reply;
}

char* TcpUtils::sendRequest(std::string location, std::string message, size_t replyBuffSize) {
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char* server_reply = (char*) malloc(replyBuffSize * sizeof(char));
	int recv_size;

	//Initilasing the winsock libraray
	// std::cout << "Initialising Winsock..." << std::endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		// std::cout << "Failed. Error Code " << WSAGetLastError() << std::endl;
		std::string errorMsg = "{\"status\":400, \"msg\":\"Failed. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";
		
		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());
		
		return server_reply;
	}
	// std::cout << "Initialised." << std::endl;


	/*
	Function socket creates the socket and returns the socket descriptor
	First arguement is address version.AF_INET is the Internet Protocol version 4 (IPv4) address family.
	Second arguemnet is to specify what kind of connection we are establishing. SOCK_STREAM is TCP. SOCK_DGRAM is for UDP
	Third arguemnet is for specifying which protocol to be used.
	*/

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		// std::cout << "Could not create socket beacuse of " << WSAGetLastError() << std::endl;
		std::string errorMsg = "{\"status\":400, \"msg\":\"Could not create socket beacuse of. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		WSACleanup();
		return server_reply;
	}
	// std::cout << "Socket created" << std::endl;

	//Local server
	// server.sin_addr.s_addr = inet_addr("127.0.0.1");// inet_addr(cIP);
	std::string strLocation("127.0.0.1");
	LPWSTR lpLocation;
	StringUtils::toLpwstr(strLocation, lpLocation);

	InetPton(AF_INET, lpLocation, &server.sin_addr.s_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	//Connect to remote server
	if (connect(s, (struct sockaddr*) & server, sizeof(server)) < 0) {
		// std::cout << "connect error" << std::endl;
		closesocket(s);
		WSACleanup();

		std::string errorMsg = "{\"status\":400, \"msg\":\"Connect error. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}

	// std::cout << "Connected" << std::endl;

	//Send some data
	if (send(s, message.c_str(), message.length(), 0) < 0) {
		std::cout << "Send failed" << std::endl;
		closesocket(s);
		WSACleanup();

		std::string errorMsg = "{\"status\":400, \"msg\":\"Send failed. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";
		
		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}
	// std::cout << "Data Send" << std::endl;

	//Receive a reply from the server
	if ((recv_size = recv(s, server_reply, replyBuffSize, 0)) == SOCKET_ERROR) {
		// std::cout << "recv failed" << std::endl;
		closesocket(s);
		WSACleanup();

		std::string errorMsg = "{\"status\":400, \"msg\":\"Receive failed. Error Code: " + std::to_string(WSAGetLastError()) + ", Message: " + StringUtils::getLastErrorAsString() + "\"}";

		strcpy_s(server_reply, replyBuffSize, errorMsg.c_str());

		return server_reply;
	}

	// std::cout << "Reply received" << std::endl;

	//Add a NULL terminating character to make it a proper string before printing
	server_reply[recv_size] = '\0';
	// std::cout << server_reply << std::endl;

	closesocket(s);
	WSACleanup();

	return server_reply;
}

void TcpUtils::startServer(std::string location, bool (*callback)(void*), size_t replyBuffSize) {
	auto serverThread = [](void* pData) -> void {
		std::map<std::string, void*> startServerParams = *(std::map<std::string, void*>*) pData;
		std::string location = *(std::string*) startServerParams["location"];
		bool (*callback)(void*) = (bool (*) (void*)) startServerParams["callback"];
		size_t replyBuffSize = *(size_t*) startServerParams["replyBuffSize"];

		WSADATA wsa;
		SOCKET s, new_socket;
		struct sockaddr_in server, client;
		int c, recv_size;
		// const char* message = 0;
		char* client_message = (char*)malloc(replyBuffSize * sizeof(char));
		bool stopServer = false;

		InitializeCriticalSection(&critsec);

		// std::cout << "Initialising Winsock..." << std::endl;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
			std::cout << "Failed. Error Code " << WSAGetLastError() << std::endl;
			return;
		}

		// std::cout << "Initialised." << std::endl;

		//Create a socket. This is a master socket
		if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
			std::cout << "Could not create socket beacuse of " << WSAGetLastError() << std::endl;
		}
		// std::cout << "Socket created" << std::endl;


		//Prepare the sockaddr_in structure
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons(8888);

		//Bind(bind the socket to a local address and port)
		if (bind(s, (struct sockaddr*) & server, sizeof(server)) == SOCKET_ERROR) {
			std::cout << "Bind failed with error code " << WSAGetLastError() << std::endl;
		}

		// std::cout << "Bind done" << std::endl;

		//Listen to incoming connections
		listen(s, 3);

		//Accept and incoming connection
		// std::cout << "Waiting for incoming connections..." << std::endl;
		c = sizeof(struct sockaddr_in);

		//Live Server
		while (!stopServer) {
			new_socket = accept(s, (struct sockaddr*) &client, &c);

			char inetBuff[16] = { 0 };

			// std::cout << "I got a connection from" << inet_ntop(AF_INET, (const void*) &client.sin_addr, inetBuff, sizeof(inetBuff)) << " :" << ntohs(client.sin_port) << std::endl;

			if ((recv_size = recv(new_socket, client_message, replyBuffSize, 0)) == SOCKET_ERROR) {
				std::cout << "Recieve failed from client" << std::endl;
			}
			// std::cout << "Data recieved from client to server is :" << std::endl;
			client_message[recv_size] = '\0';
			// std::cout << client_message << std::endl;

			auto callbackSendReply = [&](std::string replyMessage) {
				//Sent the message to client
				if (send(new_socket, replyMessage.c_str(), replyMessage.length(), 0) < 0) {
					std::cout << "Send failed from server to client" << std::endl;
				}
				// std::cout << "Data send from server to client" << std::endl;
			};

			// std::cout << "Starting a thread for a connection" << std::endl;

			bool replyReceived = false;

			auto calbackReply = [&](void* data) {
				std::map<std::string, void*> callbackReplyParams = *(std::map<std::string, void*>*) data;
				std::string replyToSend = *(std::string*) callbackReplyParams["replyMessage"];

				// std::cout << "replyToSend: " << replyToSend << std::endl;

				callbackSendReply(replyToSend);

				// Free memory pointers ===========================================================
				std::map<std::string, void*>::iterator itr = callbackReplyParams.find("replyMessage");
				if (itr != callbackReplyParams.end()) {
					// found it - delete it
					delete (std::string*) itr->second;
					callbackReplyParams.erase(itr);
				}
			};

			#pragma warning(push)
			#pragma warning(disable: 4291)
			std::map<std::string, void*> callbackParams;
			callbackParams["messageReceived"] = (void*) new std::string(client_message);
			callbackParams["callbackReply"] = (void*)Lambda::ptr<void>(calbackReply);
			#pragma warning(pop)

			bool waitReplyCallback = callback(&callbackParams);

			// std::cout << "waitReplyCallback: " << std::to_string(waitReplyCallback) << std::endl;

			// Free memory pointers ===========================================================
			std::map<std::string, void*>::iterator itr = callbackParams.find("messageReceived");
			if (itr != callbackParams.end()) {
				// found it - delete it
				delete (std::string*) itr->second;
				callbackParams.erase(itr);
			}
			std::map<std::string, void*>::iterator itr2 = callbackParams.find("callbackReply");
			if (itr2 != callbackParams.end()) {
				// found it - delete it
				//delete (std::string*) itr2->second;
				callbackParams.erase(itr2);
			}

			// free(client_message);

			if (!waitReplyCallback) {
				callbackSendReply("");
			}

			//auto workerThread = [&](void* pData) -> void {
			//	// I'm a thread!
			//	/*std::cout << "Inside InterpretClientMessage" << std::endl;
			//	char* ch = (char*)P;
			//	std::cout << "LLLLLLLLL::::" << ch << std::endl;

			//	EnterCriticalSection(&critsec);
			//	int x;
			//	x = count;
			//	x++;
			//	count = x;
			//	std::cout << "Count: " << count << std::endl;
			//	LeaveCriticalSection(&critsec);*/

			//	
			//};

			//DWORD threadID;
			//HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Lambda::ptr<void>(workerThread), (void*)client_message, 0, &threadID);

			////Wait for the thread which we have spanned to finish
			//WaitForSingleObject(handle, INFINITE);
		}

		InitializeCriticalSection(&critsec);
		closesocket(s);
		WSACleanup();

		free(client_message);
	};
	#pragma warning(push)
	#pragma warning(disable: 4291)
	std::map<std::string, void*>* startServerParams = new std::map<std::string, void*>;
	(*startServerParams)["location"] = (void*) new std::string(location);
	(*startServerParams)["callback"] = (bool (*)(void*)) callback;
	(*startServerParams)["replyBuffSize"] = (void*) new size_t(replyBuffSize);
	#pragma warning(pop)

	DWORD threadID;
	HANDLE handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE) (void (*) (void*)) serverThread, (void*) startServerParams, 0, &threadID);
}