#pragma once

#include "../define.h"

#include "TypeHelper.hpp"


namespace Utils {

	template<typename CallType>
	class Event;

	template<typename ConnectionReturn_T, typename... ConnectionArgs_T>
	class Event<ConnectionReturn_T(ConnectionArgs_T...)> {
		STATIC_WARNING(!std::is_void_v<ConnectionReturn_T>, "Return type of function type template argument 'CallType' isn't 'void'.");
	public:
		~Event();

		using ConnectionFuncType = void(*)(ConnectionArgs_T...);
		class Connection {
			ConnectionFuncType _ConnectionFunc;
			Connection* _PreviousConnection;
			Connection* _NextConnection;
		};

		Connection* Connect(ConnectionFuncType ConnectionFunc);
		void RemoveConnection(Connection* Connection);
		void Fire(ConnectionArgs_T...) const;

	private:
		Connection* _BackConnection = NULL;
		Connection* _FrontConnection = NULL;
	};
}

template<typename ConnectionReturn_T, typename... ConnectionArgs_T>
Utils::Event<ConnectionReturn_T(ConnectionArgs_T...)>::~Event() {

	Connection* CurrentConnection, * NextConnection;

	CurrentConnection = this->_BackConnection;
	while (CurrentConnection != NULL) {
		NextConnection = CurrentConnection->_NextConnection;
		delete CurrentConnection;
		CurrentConnection = NextConnection;
	}
}

template<typename ConnectionReturn_T, typename... ConnectionArgs_T>
typename Utils::Event<ConnectionReturn_T(ConnectionArgs_T...)>::Connection* Utils::Event<ConnectionReturn_T(ConnectionArgs_T...)>::Connect(ConnectionFuncType ConnectionFunc) {

	Connection* NewConnection = new Connection;
	NewConnection->_ConnectionFunc = ConnectionFunc;
	NewConnection->_NextConnection = NULL;

	unlikely_branch
	if (this->_BackConnection == NULL) {
		NewConnection->_PreviousConnection = NULL;
		this->_BackConnection = NewConnection;
	} else {
		this->_FrontConnection->_NextConnection = NewConnection;
	}
	this->_FrontConnection = NewConnection;

	return NewConnection;
}

template<typename ConnectionReturn_T, typename... ConnectionArgs_T>
void Utils::Event<ConnectionReturn_T(ConnectionArgs_T...)>::RemoveConnection(Connection* _Connection) {

	if (_Connection == this->_BackConnection) {

		this->_BackConnection = _Connection->_NextConnection;
		if (_Connection->_NextConnection != NULL) {
			_Connection->_NextConnection->_PreviousConnection = NULL;
		}
		
	} else if (_Connection == this->_FrontConnection) {
		
		this->_FrontConnection = _Connection->_PreviousConnection;
		if (_Connection->_PreviousConnection != NULL) {
			_Connection->_PreviousConnection->_NextConnection = NULL;
		}

	} else {
		_Connection->_PreviousConnection->_NextConnection = _Connection->_NextConnection;
		_Connection->_NextConnection->_PreviousConnection = _Connection->_PreviousConnection;
	}

	delete _Connection;
}

template<typename ConnectionReturn_T, typename... ConnectionArgs_T>
void Utils::Event<ConnectionReturn_T(ConnectionArgs_T...)>::Fire(ConnectionArgs_T... Args) const {

	const Connection* _Connection = this->_BackConnection;
	while (_Connection != NULL) {
		_Connection->_ConnectionFunc(Args...);
		_Connection = _Connection->_NextConnection;
	}
}