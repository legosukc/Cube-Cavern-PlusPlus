#include "Network_Server.hpp"

#include "../define.h"

#include <iostream>

#include <map>
#include <vector>

#include <SDL3/SDL_thread.h>
#include <SDL3_net/SDL_net.h>

#include "../FunctionHeaders/Exceptions.hpp"

static std::vector<char> _Packet;

namespace Game::Network::Classes {

    RemoteEvent::RemoteEvent(const char* _Name, EventPool* _ParentPool) noexcept
        : Name(_Name), NameLength(strlen(_Name)), ParentPool(_ParentPool) {}

    RemoteEvent::RemoteEvent(const char* _Name,
                             size_t _NameLength,
                             EventPool* const _ParentPool) noexcept
        : Name(_Name), NameLength(_NameLength), ParentPool(_ParentPool) {}

    void RemoteEvent::FireClient(const void* PacketData, size_t PacketSize) {
        char* EndPtr = &this->ParentPool->_PacketData.back();
        this->ParentPool->_PacketData.reserve(PacketSize);

        std::memcpy(++EndPtr, PacketData, PacketSize);
    }

    void RemoteEvent::FireAllClients(const void* PacketData,
                                     size_t PacketSize) {}

    void RemoteEvent::FireClientNoCopy(const void* PacketData,
                                       size_t PacketSize) {}

    void RemoteEvent::FireAllClientsNoCopy(const void* PacketData,
                                           size_t PacketSize) {}

    // Mimics a client sending an event to the local server.
    void RemoteEvent::FireLocal(const void* PacketData, size_t PacketSize) {
        for (auto Connection : this->_Connections) {
            Connection(PacketData, PacketSize);
        }
    }
}

static std::map<const char*, Game::Network::Classes::EventPool> _Pools;

namespace Game::Network {
    int AddressCount;
    NET_Address** Address = NULL;

    Classes::EventPool DefaultPool;
}

Game::Network::Classes::EventPool& GetPool(const char* PoolName) {
    likely_branch if (::_Pools.count(PoolName) > 0) {
        return ::_Pools[PoolName];
    }

    throw 1;
}

namespace Game::Network::etc {
    volatile bool runboyrun = true;

    SDL_Thread* _GlobalServerLocaterThread = NULL;
    SDL_Thread* _LANCheckerThread = NULL;
}

void Game::Network::etc::_LANChecker() {
    NET_Datagram* ServerMessage;
    NET_DatagramSocket* LANChecker;

    LANChecker = NET_CreateDatagramSocket(*Game::Network::Address, 2011, 0);

    constexpr static const char CheckForAvaliableServersMsg[] =
        "I AM CUBE CAVERN. SHOW YOURSELVES.";

    struct ServerAnnouncePayload_Struct {
        const char Header[25] = {"I AM CUBE CAVERN SERVER."};

        Uint16 Players, MaxPlayers;
        Uint16 ServerVersion;

        char Name[32];
    } ServerAnnouncePayload;

    while (runboyrun) {
        NET_ReceiveDatagram(LANChecker, &ServerMessage);
        if (ServerMessage == NULL) {
            SDL_Delay(500);
            continue;
        }

        if (std::strncmp((char*)ServerMessage->buf, CheckForAvaliableServersMsg,
                         sizeof(CheckForAvaliableServersMsg)) != 0) {
            continue;
        }

        NET_SendDatagram(LANChecker, *Game::Network::Address, 2011,
                         &ServerAnnouncePayload,
                         sizeof(ServerAnnouncePayload_Struct));
    }
}

void Game::Network::SendPacket() {
    char* CurrentEventPool;
    for (auto Byte : ::_Packet) {
        CurrentEventPool = &Byte;
    }
}