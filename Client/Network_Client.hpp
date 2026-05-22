#pragma once

#include <map>
#include <vector>
#include <thread>

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>

#include "../define.h"


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

		static void _LANChecker() {

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
		static std::thread _LANCheckerThread;
	}
}

void Game::Network::Init() {

	if (!NET_Init()) {
		std::cerr << "::FATAL ERROR:: Failed to initalize SDL3_net. Game requires the ability to set up a connection, even if offline, to set up a local server and connect to it." << std::endl;
		exit(EXIT_FAILURE);
	}

	Game::Network::Address = NET_GetLocalAddresses(&Game::Network::AddressCount);

	::Network::_LANCheckerThread = std::thread(::Network::_LANChecker);
}

void Game::Network::Destroy() {

	::Network::runboyrun = false;
	::Network::_LANCheckerThread.join();

	NET_Quit();
}


void Game::Network::SendPacket() {
	
	char* CurrentEventPool;
	for (auto Byte : ::_Packet) {
		CurrentEventPool = &Byte;
	}
}