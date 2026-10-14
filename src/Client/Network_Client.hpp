#pragma once

#include "../define.h"

#include <map>
#include <vector>

#ifndef NETWORKING_DISABLED
#include <SDL3_net/SDL_net.h>
#endif

namespace Game::Network::Classes {

    class EventPool;

    class RemoteEvent {
       public:
        RemoteEvent(const char* _Name, EventPool* _ParentPool);
        RemoteEvent(const char* _Name,
                    size_t _NameLength,
                    EventPool* const _ParentPool) noexcept;

        using ConnectionFuncType = void (*)(const void* PacketData,
                                            size_t PacketSize);

        void Connect(ConnectionFuncType ConnectionFunc);
        // Copies "PacketSize" number of bytes from "PacketData" to a packet
        // buffer, which will be sent to the server.
        void FireServer(const void* PacketData, size_t PacketSize);

        // Doesn't copy param "PacketData", faster than FireServer, but make
        // sure the pointer doesn't go out-of-scope or is freed before packets
        // are sent, or it will lead to a nasty crash, euughgg....
        void FireServerNoCopy(const void* PacketData, size_t PacketSize);

        // Mimics a server sending an event to the local client.
        void FireLocal(const void* PacketData, size_t PacketSize);

       private:
        std::vector<void (*)(const void* PacketData, size_t PacketSize)>
            _Connections;

        const char* Name;
        size_t NameLength;
        EventPool* const ParentPool;

        friend EventPool;
    };

    class EventPool {
        std::map<const char*, RemoteEvent> _Pool;
        std::vector<char> _PacketData;

        friend RemoteEvent;

       public:
        RemoteEvent& GetEvent(const char* EventName);
        const RemoteEvent& GetEvent(const char* EventName) const;
    };
}

namespace Game::Network {
    void Init();
    void Destroy();

    inline int AddressCount = 0;

#ifndef NETWORKING_DISABLED
    inline NET_Address** Address = NULL;
#endif

    inline Classes::EventPool DefaultPool;
    Classes::EventPool& GetPool(const char* PoolName);

    void SendPacket();
}