#pragma once

#include "../define.h"

#include <iostream>
#include <fstream>

#include <map>
#include <vector>

#include <SDL3/SDL_thread.h>
#include <SDL3_net/SDL_net.h>

#include "../FunctionHeaders/Exceptions.hpp"


namespace {
	static std::vector<char> _Packet;
}

namespace Game::Network::Classes {

	class EventPool;

	class RemoteEvent {
		std::vector<void(*)(const void* PacketData, size_t PacketSize)> _Connections;

		const char* Name;
		size_t NameLength;
		EventPool* const ParentPool;

		friend EventPool;

	public:
		RemoteEvent(const char* _Name, EventPool* _ParentPool) : Name(_Name), NameLength(strlen(_Name)), ParentPool(_ParentPool) {}
		inline RemoteEvent(const char* _Name, size_t _NameLength, EventPool* const _ParentPool) noexcept : Name(_Name), NameLength(_NameLength), ParentPool(_ParentPool) {}

		inline void Connect(void(*ConnectionFunc)(const void* PacketData, size_t PacketSize)) {
			this->_Connections.push_back(ConnectionFunc);
		}

		// Copies "PacketSize" number of bytes from "PacketData" to a packet buffer, which will be sent to the server.
		void FireServer(const void* PacketData, size_t PacketSize);

		// Doesn't copy param "PacketData", faster than FireServer, but make sure the pointer doesn't go out-of-scope or is freed before packets are sent, or it will lead to a nasty crash, euughgg....
		void FireServerNoCopy(const void* PacketData, size_t PacketSize);

		// Mimics a server sending an event to the local client.
		void FireLocal(const void* PacketData, size_t PacketSize) {
			for (auto Connection : this->_Connections) {
				Connection(PacketData, PacketSize);
			}
		}
	};

	class EventPool {
		std::map<const char*, RemoteEvent> _Pool;
		std::vector<char> _PacketData;

		friend RemoteEvent;
		
	public:

		inline RemoteEvent& GetEvent(const char* EventName) {
			return this->_Pool.at(EventName);
		}
		inline const RemoteEvent& GetEvent(const char* EventName) const {
			return this->_Pool.at(EventName);
		}
	};
}

namespace {
	static std::map<const char*, Game::Network::Classes::EventPool> _Pools;
}


void Game::Network::Classes::RemoteEvent::FireServer(const void* PacketData, size_t PacketSize) {

	char* EndPtr = &this->ParentPool->_PacketData.back();
	this->ParentPool->_PacketData.reserve(PacketSize);

	std::memcpy(++EndPtr, PacketData, PacketSize);
}



namespace Game::Network {
	inline void Init();
	inline void Destroy();

	int AddressCount;
	NET_Address** Address;

	Classes::EventPool DefaultPool;
	Classes::EventPool& GetPool(const char* PoolName);

	inline void SendPacket();
}



Game::Network::Classes::EventPool& GetPool(const char* PoolName) {
	likely_branch
	if (::_Pools.count(PoolName) > 0) {
		return ::_Pools[PoolName];
	}

	throw 1;
}


namespace {

	namespace Network {
		static volatile bool runboyrun = true;

		static void _LANChecker(void*) {

			NET_Datagram* ServerMessage;
			NET_DatagramSocket* LANChecker;

			LANChecker = NET_CreateDatagramSocket(*Game::Network::Address, 2011, 0);

			constexpr static const char CheckForAvaliableServersMsg[] = "I AM CUBE CAVERN. SHOW YOURSELVES.";
			constexpr static const char ServerAnnounceHeader[] = "I AM CUBE CAVERN SERVER.";

			struct ServerAnnouncePayload {
				ServerAnnouncePayload() = delete;
				~ServerAnnouncePayload() = delete;

				const char Header[sizeof(ServerAnnounceHeader)];

				Uint16 Players, MaxPlayers;
				Uint16 ServerVersion;

				const char Name[];
			};

			while (::Network::runboyrun) {
				NET_SendDatagram(LANChecker, *Game::Network::Address, 2011, CheckForAvaliableServersMsg, sizeof(CheckForAvaliableServersMsg));

				while (true) {

					SDL_Delay(500);
					if (NET_ReceiveDatagram(LANChecker, &ServerMessage)) {

						if (ServerMessage == NULL) {
							break;
						}

						const ServerAnnouncePayload* const Payload = reinterpret_cast<ServerAnnouncePayload*>(ServerMessage->buf);
						if (ServerMessage->buflen < sizeof(ServerAnnouncePayload) + 2) {
							goto BreakAndFreeDatagram;
						}
						
						if (std::strncmp(Payload->Header, ServerAnnounceHeader, sizeof(ServerAnnounceHeader)) != 0) {

							BreakAndFreeDatagram:
							NET_DestroyDatagram(ServerMessage);
							break;
						}
						std::cout << "Found LAN server, Name: " << Payload->Name
							<< "\nPlayers/MaxPlayers: " << Payload->Players << '/' << Payload->MaxPlayers
							<< "\nServer Version: " << Payload->ServerVersion << std::endl;
						
						NET_DestroyDatagram(ServerMessage);

					} else {
						NET_DestroyDatagramSocket(LANChecker);
						LANChecker = NET_CreateDatagramSocket(*Game::Network::Address, 2011, 0);
						break;
					}
				}
			}
		}

		static int _GlobalServerLocater(void*) {

			enum ServerTrackerResponses {
				NoServers,
				InvalidPageIndex,
				CannotRenewUnregisteredServer,
				RenewedServer,
				RegisteredServer,
				DeletedServer,
				BadRequest = 400
			};

			NET_Address* ServerTrackerAddress = NET_ResolveHostname("ccpp-server-tracker.superjackass64-e41.workers.dev");
			switch (NET_WaitUntilResolved(ServerTrackerAddress, -1)) {
			case NET_Status::NET_SUCCESS:
				std::cout << "Successfully resolved global server tracker address." << std::endl;
				break;

			case NET_Status::NET_FAILURE:
				std::cerr << "Failed to resolve global server tracker address, SDL_net error: " << SDL_GetError() << std::endl;
				return EXIT_FAILURE;
			}

			NET_StreamSocket* ServerTrackerHTTPStream = NET_CreateClient(ServerTrackerAddress, 80, NULL);
			if (ServerTrackerAddress == NULL) {
				std::cerr << "Failed to establish connection to global server tracker, SDL_net error: " << SDL_GetError() << std::endl;
				return EXIT_FAILURE;
			}

			static const char HTTPGetRequest[] = "GET / HTTP/1.1\r\nHost: ccpp-server-tracker.superjackass64-e41.workers.dev\r\nConnection: close\r\n\r\n";

			std::ofstream output;
			output.open("outputofhttpyay.txt");

			std::vector<char> ResponseBuffer(4096);
			while (::Network::runboyrun) {

				SDL_Delay(500);
				if (NET_WriteToStreamSocket(ServerTrackerHTTPStream, HTTPGetRequest, sizeof(HTTPGetRequest)) == -1) {
					std::cout << "NET_WriteToStreamSocket error: failed to send HTTP Get packet to server tracker, retrying...\n" << SDL_GetError() << std::endl;
					goto RestablishConnection;
				}
				if (NET_WaitUntilStreamSocketDrained(ServerTrackerHTTPStream, -1) == -1) {
					std::cout << "failed to send HTTP Get packet to server tracker, retrying...\n" << SDL_GetError() << std::endl;
					goto RestablishConnection;
				}

				NET_ReadFromStreamSocket(ServerTrackerHTTPStream, ResponseBuffer.data(), ResponseBuffer.size());
				output << ResponseBuffer.data() << std::endl;
			
				std::cout << ResponseBuffer.data() << std::endl;

				continue;

			RestablishConnection:

				NET_DestroyStreamSocket(ServerTrackerHTTPStream);
				ServerTrackerHTTPStream = NET_CreateClient(ServerTrackerAddress, 80, NULL);
				if (ServerTrackerAddress == NULL) {
					std::cerr << "Failed to establish connection to global server tracker, SDL_net error: " << SDL_GetError() << std::endl;
					break;
				}
			}
			
			NET_DestroyStreamSocket(ServerTrackerHTTPStream);
			return EXIT_SUCCESS;
		}

		static SDL_Thread* _GlobalServerLocaterThread;
		static SDL_Thread* _LANCheckerThread;
	}
}

void Game::Network::Init() {

	if (!NET_Init()) {
		std::cerr << "Failed to initalize SDL_net. Even if offline, game requires being able to set up a local server and connect to it." << std::endl;
		Exceptions::ThrowSDLError("Failed to initalize SDL_net. Even if offline, game requires being able to set up a local server and connect to it.");
	}

	Game::Network::Address = NET_GetLocalAddresses(&Game::Network::AddressCount);
	
	SDL_PropertiesID GlobalServerLocaterThreadPrpties = SDL_CreateProperties();
	SDL_SetNumberProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_NAME_STRING, SDL_THREAD_PRIORITY_NORMAL);
	
	//SDL_SetStringProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_NAME_STRING, "ccGBLFind");
	//SDL_SetPointerProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void*)::Network::_GlobalServerLocater);
	//::Network::_GlobalServerLocaterThread = SDL_CreateThreadWithProperties(GlobalServerLocaterThreadPrpties);

	SDL_SetStringProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_NAME_STRING, "ccLANFind");
	SDL_SetPointerProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void*)::Network::_LANChecker);
	::Network::_LANCheckerThread = SDL_CreateThreadWithProperties(GlobalServerLocaterThreadPrpties);

	SDL_DestroyProperties(GlobalServerLocaterThreadPrpties);
}

void Game::Network::Destroy() {

	::Network::runboyrun = false;
	SDL_WaitThread(::Network::_LANCheckerThread, NULL);
	//SDL_WaitThread(::Network::_GlobalServerLocaterThread, NULL);

	NET_Quit();
}


void Game::Network::SendPacket() {
	
	char* CurrentEventPool;
	for (auto Byte : ::_Packet) {
		CurrentEventPool = &Byte;
	}
}