#pragma once

#include <sockpp/tcp6_acceptor.h>
#include <sockpp/tcp6_connector.h>

#include <sockpp/udp6_socket.h>

namespace {

	struct PacketStruct {
		PacketStruct(const PacketStruct&& mov) {
			this->Socket = mov.Socket;
			this->Data = std::move(mov.Data);
		}

		sockpp::tcp6_socket* Socket;
		std::vector<char> Data;
	};
	std::map<in6_addr, PacketStruct> _Packets;

	struct _TCPHandlerBase {
		virtual ~_TCPHandlerBase() = default;
		virtual void Update() = 0;
	};

	class _TCP6HandlerServer : public _TCPHandlerBase {

		sockpp::tcp6_acceptor Acceptor;
		std::thread AcceptorThread;
		volatile bool TerminateAcceptorThread;

		static void _connectionThread(sockpp::tcp6_socket sock) {

			ssize_t n;
			char buf[512];

			struct {
				char Name[24];
			} PlayerData;
			n = sock.read(&PlayerData, sizeof(PlayerData));

			if (n <= -1) {
				sock.write_n("fuck you.", sizeof("fuck you."));
				goto CloseSocket;
			}
			PlayerData.Name[n - 1] = '\0';

			{
				PacketStruct& Packet = ::_Packets[sock.peer_address().address()];
				Packet.Socket = &sock;
			}


			while ((n = sock.read(buf, sizeof(buf))) > 0) {
				sock.write_n("YO", sizeof("YO"));
			}

		CloseSocket:
			std::cout << "Connection closed from " << sock.peer_address() << std::endl;

			::_Packets.erase(sock.peer_address().address());
		}

		static void _acceptorThread(sockpp::tcp6_acceptor& Acceptor, const volatile bool* pleasestop) {

			sockpp::inet6_address peer;
			do {
				sockpp::tcp6_socket sock = Acceptor.accept(&peer);
				std::cout << "Received a connection request from " << peer << std::endl;

				if (!sock) {
					std::cerr << "Error accepting incoming connection: " << Acceptor.last_error_str() << std::endl;
					continue;
				}
				// Create a thread and transfer the new stream to it.
				std::thread thr(_connectionThread, std::move(sock));
				thr.detach();
			} while (!*pleasestop);
		}

	public:
		_TCP6HandlerServer() {
			this->Acceptor.open(12345);

			if (!this->Acceptor) {
				std::cerr << "Error creating the acceptor: " << this->Acceptor.last_error_str() << std::endl;
				Exceptions::ThrowExceptionExpression<Exceptions::RuntimeError>("Error while creating the TCP6 Acceptor.\n", this->Acceptor.last_error_str());
			}
			std::cout << "Awaiting connections on port " << 12345 << "..." << std::endl;

			this->TerminateAcceptorThread = false;
			this->AcceptorThread = std::thread(this->_acceptorThread, this->Acceptor, &this->TerminateAcceptorThread);
		}

		~_TCP6HandlerServer() {
			this->TerminateAcceptorThread = true;
			this->AcceptorThread.join();

			this->Acceptor.close();
			this->Acceptor.release();
		}

		void Update() override {


		}
	};
}


namespace Game::Network::Classes {

	class PacketVector {

		struct EventData_Parameters_Struct {
			Uint32 Size;
			char Data[];
		};

		struct EventData_Struct {
			~EventData_Struct() {
				for (Uint16 i = 0; i < this->Invocations; ++i) {
					delete[] reinterpret_cast<char*>(this->Parameters[i]);
				}
				delete[] this->Parameters;
			}
			Uint16 Invocations = 0;
			EventData_Parameters_Struct** Parameters = NULL;
		};
		std::map<const char*, EventData_Struct> Events;

	public:

		template<typename Arg_T>
		constexpr inline void AddEvent(char* const Buffer, Arg_T Item) {
			*reinterpret_cast<Arg_T*>(Buffer) = Item;
		}

		template<typename Arg_T, typename... Args_T>
		constexpr inline void AddEvent(char* const Buffer, Arg_T Item, Args_T... Args) {
			*reinterpret_cast<Arg_T*>(Buffer) = Item;
			this->AddEvent<Args_T...>(Buffer + sizeof(Item), Args...);
		}

		template<typename... Args_T>
		inline void AddEvent(const char* EventName, Args_T... Items) {

			EventData_Struct& const Data = this->Events[EventName];

			if (Data.Invocations > 0) {

				unlikely_branch
				if (Data.Invocations == UINT16_MAX) {
					Exceptions::ThrowException<Exceptions::OverflowError>("Max event invocations reached! Cannot invoke more than 65535 times in one EventPacket.");
				}

				EventData_Parameters_Struct** const NewParameterBuffer = new EventData_Parameters_Struct*[++Data.Invocations];// Data.Parameters;
				std::memcpy(NewParameterBuffer, Data.Parameters, (Data.Invocations - 1) * sizeof(EventData_Parameters_Struct**));

				delete[] Data.Parameters;
				Data.Parameters = NewParameterBuffer;
			} else {
				Data.Parameters = new EventData_Parameters_Struct*[++Data.Invocations];
			}

			EventData_Parameters_Struct* Parameters = reinterpret_cast<EventData_Parameters_Struct*>(new char[sizeof(Uint32) + TypeHelper::sizeofTypes_v<Args_T...>]);
			Data.Parameters[Data.Invocations] = Parameters;
			Parameters->Size = TypeHelper::sizeofTypes_v<Args_T...>;

			this->AddEvent<Args_T...>(Parameters->Data, Items...);
		}

		void* Encode() {

			Uint32 Index;
			char* Buffer, * BufferIndex;
			ssize_t Size;

			Size = 0;
			for (auto& Pair : this->Events) {
				Size += strlen(Pair.first) + 1;

				for (Index = 0; Index < Pair.second.Invocations; ++Index) {
					Size += static_cast<ssize_t>(Pair.second.Parameters[Index]->Size);
				}
			}
			
			Buffer = new char[Size];
			for (const auto& Pair : this->Events) {
				
				Index = 0;
				while (true) {
					BufferIndex[Index] = Pair.first[Index];
					if (BufferIndex[Index] == '\0') {
						break;
					}
					++Index;
				}
				BufferIndex += Index;

				for (Index = 0; Index < Pair.second.Invocations; ++Index) {
					std::memcpy(BufferIndex, Pair.second.Parameters[Index]->Data, Pair.second.Parameters[Index]->Size);
					BufferIndex += Pair.second.Parameters[Index]->Size;
				}
			}

			return Buffer;
		}
		static PacketVector Decode(const void* Data);
	};

	class EventPool;

	class RemoteEvent {
		std::vector<void(*)(const void* PacketData, size_t PacketSize)> _Connections;


		RemoteEvent(const EventPool& _EventPool) {
			
		}
	public:
		inline void Connect(void(*ConnectionFunc)(const void* PacketData, size_t PacketSize)) {
			this->_Connections.push_back(ConnectionFunc);
		}

		template<typename Arg_T, typename... Args_T>
		void FireAllClients() {
			
		}

		void FireClient() {

		}

		void FireLocal() {

		}
	};

	class EventPool {
		std::map<const char*, RemoteEvent> _Pool;
		const char* Name;

	public:

		inline RemoteEvent& GetEvent(const char* EventName) {
			return this->_Pool[EventName];
		}
		inline const RemoteEvent& GetEvent(const char* EventName) const {
			return this->_Pool.at(EventName);
		}
	};
}

namespace {
	static std::map<const char*, Game::Network::Classes::EventPool> _Pools;
}


namespace Game::Network {
	inline void Init();
	inline void Destroy();

	::_TCPHandlerBase* TCPHandler;

	Network::Classes::EventPool DefaultPool;
	Network::Classes::EventPool& GetPool(const char* PoolName);

	inline void Update() {

		for (auto& Pair : ::_Packets) {
			Pair.second.Socket->write_n(Pair.second.Data.data(), Pair.second.Data.size() * sizeof(decltype(::PacketStruct::Data)::value_type));
			Pair.second.Data.clear();
		}
	}
}



Game::Network::Classes::EventPool& GetPool(const char* PoolName) {
	likely_branch
	if (::_Pools.count(PoolName) > 0) {
		return ::_Pools[PoolName];
	}
}




void Game::Network::Init() {

	new ::_TCP6HandlerServer();
}

void Game::Network::Destroy() {
	delete Game::Network::TCPHandler;
}


void Game::Network::Update() {
	TCPHandler->Update();
}