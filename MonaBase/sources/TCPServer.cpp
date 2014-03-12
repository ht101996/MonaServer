/*
Copyright 2014 Mona
mathieu.poux[a]gmail.com
jammetthomas[a]gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License received along this program for more
details (or else see http://www.gnu.org/licenses/).

This file is a part of Mona.
*/

#include "Mona/TCPServer.h"


using namespace std;


namespace Mona {

TCPServer::TCPServer(const SocketManager& manager) : _hasToAccept(false), _running(false),SocketHandler(manager) {
}


TCPServer::~TCPServer() {
	stop();
}

bool TCPServer::start(Exception& ex,const SocketAddress& address) {
	lock_guard<mutex> lock(_mutex);
	if (_running)
		stop();
	if (!socket().bindWithListen(ex, address))
		return false;
	SocketHandler::address(address);
	return _running=true;
}

void TCPServer::stop() {
	lock_guard<mutex> lock(_mutex);
	if (!_running)
		return;
	close();
	resetAddresses();
	_running = false;
}

void TCPServer::onReadable(Exception& ex) {
	_hasToAccept = true;
	onConnectionRequest(ex);
	if (_hasToAccept)
		socket().rejectConnection();
	_hasToAccept = false;
}


} // namespace Mona
