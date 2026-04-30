#pragma once

#include <sockpp/tcp6_acceptor.h>
#include <sockpp/tcp6_connector.h>

#include <sockpp/udp6_socket.h>

#include <thread>

namespace {

	class _TCP6HandlerClient {

		sockpp::tcp6_connector Connection;

		std::thread* ConnectionThread;
		volatile bool TerminateConnectionThread;

		static void _connectionThread(sockpp::tcp6_connector* Connection, const volatile bool* pleasestop) {
			
			char Buffer[8192];
			while (!*pleasestop) {
				ssize_t n = Connection->read(&Buffer, sizeof(Buffer));

				//Connection.write_n();

				std::cout << Buffer << std::endl;
			}
		}

	public:
		_TCP6HandlerClient() {
			this->Connection.bind({ "::1", 12345 });
			if (!this->Connection) {
				std::cerr << "Error connecting to server at "
					<< sockpp::inet6_address("::1", 12345)
					<< "\n\t" << this->Connection.last_error_str() << std::endl;
				return;
			}

			std::cout << "Created a connection from " << this->Connection.address() << std::endl;

			// Set a timeout for the responses
			if (!this->Connection.read_timeout(std::chrono::seconds(5))) {
				std::cerr << "Error setting timeout on TCP stream: "
					<< this->Connection.last_error_str() << std::endl;
			}

			this->TerminateConnectionThread = false;
			
			this->ConnectionThread = new std::thread(this->_connectionThread, &this->Connection, &this->TerminateConnectionThread);
		}

		~_TCP6HandlerClient() {
			this->TerminateConnectionThread = true;
			this->ConnectionThread->join();
			delete this->ConnectionThread;

			this->Connection.close();
			this->Connection.release();
		}
	};


	static std::vector<char> _Packet;
}


namespace Game::Network::Classes {

	class RemoteEvent {
		std::vector<void(*)(const void* PacketData, size_t PacketSize)> _Connections;

	public:
		inline void Connect(void(*ConnectionFunc)(const void* PacketData, size_t PacketSize)) {
			this->_Connections.push_back(ConnectionFunc);
		}

		void FireServer(const void* PacketData, size_t PacketSize) {
			
		}

		void FireLocal() {

		}
	};

	class EventPool {
		std::map<const char*, RemoteEvent> _Pool;
		
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


namespace Game::Network {
	inline void Init();
	inline void Destroy();

	::_TCP6HandlerClient* TCPHandler;

	Classes::EventPool DefaultPool;
	Classes::EventPool& GetPool(const char* PoolName);

	inline void Update();
}



Game::Network::Classes::EventPool& GetPool(const char* PoolName) {
	likely_branch
	if (::_Pools.count(PoolName) > 0) {
		return ::_Pools[PoolName];
	}

	throw 1;
}




void Game::Network::Init() {

	sockpp::initialize();
	Network::TCPHandler = new _TCP6HandlerClient();
}

void Game::Network::Destroy() {
	delete Game::Network::TCPHandler;
}


void Game::Network::Update() {
	
}