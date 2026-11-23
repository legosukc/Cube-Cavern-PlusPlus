#pragma once

#include "../define.h"

#include <iostream>

#include <map>
#include <vector>

#include <SDL3/SDL_thread.h>
#include <SDL3_net/SDL_net.h>

#include "../FunctionHeaders/Exceptions.hpp"

namespace Game::Network::Classes {

    class EventPool;

    class RemoteEvent {
        std::vector<void (*)(const void* PacketData, size_t PacketSize)>
            _Connections;

        const char* Name;
        size_t NameLength;
        EventPool* const ParentPool;

        friend EventPool;

       public:
        RemoteEvent(const char* _Name, EventPool* _ParentPool) noexcept;
        RemoteEvent(const char* _Name,
                           size_t _NameLength,
                           EventPool* const _ParentPool) noexcept;

        inline void Connect(void (*ConnectionFunc)(const void* PacketData,
                                                   size_t PacketSize)) {
            this->_Connections.push_back(ConnectionFunc);
        }

        // Copies "PacketSize" number of bytes from "PacketData" to a packet
        // buffer, which will be sent to the server.
        void FireClient(const void* PacketData, size_t PacketSize);

        // Copies "PacketSize" number of bytes from "PacketData" to a packet
        // buffer, which will be sent to the server.
        void FireAllClients(const void* PacketData, size_t PacketSize);

        // Copies "PacketSize" number of bytes from "PacketData" to a packet
        // buffer, which will be sent to the server.
        void FireClientNoCopy(const void* PacketData, size_t PacketSize);

        // Copies "PacketSize" number of bytes from "PacketData" to a packet
        // buffer, which will be sent to the server.
        void FireAllClientsNoCopy(const void* PacketData, size_t PacketSize);

        // Mimics a client sending an event to the local server.
        void FireLocal(const void* PacketData, size_t PacketSize);
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

namespace Game::Network {
    inline void Init();
    inline void Destroy();

    extern int AddressCount;
    extern NET_Address** Address;

    extern Classes::EventPool DefaultPool;
    Classes::EventPool& GetPool(const char* PoolName);

    void SendPacket();
}

namespace Game::Network::etc {
    extern volatile bool runboyrun;

    void _LANChecker();

    extern SDL_Thread* _GlobalServerLocaterThread;
    extern SDL_Thread* _LANCheckerThread;
}

void Game::Network::Init() {
    if (!NET_Init()) {
        std::cerr << "Failed to initalize SDL_net." << std::endl;
        Exceptions::ThrowSDLError("Failed to initalize SDL_net.");
    }

    Game::Network::Address =
        NET_GetLocalAddresses(&Game::Network::AddressCount);

    SDL_PropertiesID ConnectionThreadProp = SDL_CreateProperties();
    SDL_SetNumberProperty(ConnectionThreadProp,
                          SDL_PROP_THREAD_CREATE_STACKSIZE_NUMBER, 4096);

    // SDL_SetStringProperty(GlobalServerLocaterThreadPrpties,
    // SDL_PROP_THREAD_CREATE_NAME_STRING, "ccGBLFind");
    // SDL_SetPointerProperty(GlobalServerLocaterThreadPrpties,
    // SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER,
    // (void*)::Network::_GlobalServerLocater);
    //::Network::_GlobalServerLocaterThread =
    //:SDL_CreateThreadWithProperties(GlobalServerLocaterThreadPrpties);

    SDL_SetStringProperty(ConnectionThreadProp,
                          SDL_PROP_THREAD_CREATE_NAME_STRING, "ccLANFind");
    SDL_SetPointerProperty(ConnectionThreadProp,
                           SDL_PROP_THREAD_CREATE_ENTRY_FUNCTION_POINTER,
                           (void*)Game::Network::etc::_LANChecker);
    Game::Network::etc::_LANCheckerThread =
        SDL_CreateThreadWithProperties(ConnectionThreadProp);

    SDL_DestroyProperties(ConnectionThreadProp);
}

void Game::Network::Destroy() {
    Game::Network::etc::runboyrun = false;
    SDL_WaitThread(Game::Network::etc::_LANCheckerThread, NULL);
    // SDL_WaitThread(::Network::_GlobalServerLocaterThread, NULL);

    NET_Quit();
}