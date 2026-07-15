#pragma once

#include "../define.h"

#include <iostream>

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
		void FireClient(const void* PacketData, size_t PacketSize);

		// Copies "PacketSize" number of bytes from "PacketData" to a packet buffer, which will be sent to the server.
		void FireAllClients(const void* PacketData, size_t PacketSize);


		// Copies "PacketSize" number of bytes from "PacketData" to a packet buffer, which will be sent to the server.
		void FireClientNoCopy(const void* PacketData, size_t PacketSize);

		// Copies "PacketSize" number of bytes from "PacketData" to a packet buffer, which will be sent to the server.
		void FireAllClientsNoCopu(const void* PacketData, size_t PacketSize);


		// Mimics a client sending an event to the local server.
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


void Game::Network::Classes::RemoteEvent::FireClient(const void* PacketData, size_t PacketSize) {

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

			struct ServerAnnouncePayload_Struct {
				const char Header[25] = { "I AM CUBE CAVERN SERVER." };

				Uint16 Players, MaxPlayers;
				Uint16 ServerVersion;

				char Name[32];
			} ServerAnnouncePayload;

			while (::Network::runboyrun) {

				NET_ReceiveDatagram(LANChecker, &ServerMessage);
				if (ServerMessage == NULL) {
					SDL_Delay(500);
					continue;
				}

				if (std::strncmp((char*)ServerMessage->buf, CheckForAvaliableServersMsg, sizeof(CheckForAvaliableServersMsg)) != 0) {
					continue;
				}

				NET_SendDatagram(LANChecker, *Game::Network::Address, 2011, &ServerAnnouncePayload, sizeof(ServerAnnouncePayload_Struct));
			}
		}

		static SDL_Thread* _GlobalServerLocaterThread;
		static SDL_Thread* _LANCheckerThread;
	}
}

void Game::Network::Init() {

	if (!NET_Init()) {
		std::cerr << "Failed to initalize SDL_net." << std::endl;
		Exceptions::ThrowSDLError("Failed to initalize SDL_net.");
	}

	Game::Network::Address = NET_GetLocalAddresses(&Game::Network::AddressCount);

	SDL_PropertiesID ConnectionThreadProp = SDL_CreateProperties();
	SDL_SetNumberProperty(ConnectionThreadProp, SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, 4096);

	//SDL_SetStringProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_NAME_STRING, "ccGBLFind");
	//SDL_SetPointerProperty(GlobalServerLocaterThreadPrpties, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void*)::Network::_GlobalServerLocater);
	//::Network::_GlobalServerLocaterThread = SDL_CreateThreadWithProperties(GlobalServerLocaterThreadPrpties);

	SDL_SetStringProperty(ConnectionThreadProp, SDL_PROP_THREAD_CREATE_NAME_STRING, "ccLANFind");
	SDL_SetPointerProperty(ConnectionThreadProp, SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER, (void*)::Network::_LANChecker);
	::Network::_LANCheckerThread = SDL_CreateThreadWithProperties(ConnectionThreadProp);

	SDL_DestroyProperties(ConnectionThreadProp);
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