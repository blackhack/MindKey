#pragma once


class MindServer
{
public:
    MindServer(boost::asio::io_service& io_service, unsigned short port);
    ~MindServer();

    void HandleAccept(const boost::system::error_code& e, ConnectionPtr conn);
    void HandleRead(const boost::system::error_code& e, ConnectionPtr conn);
    void SaveData(ConnectionPtr conn);
private:
    boost::asio::ip::tcp::acceptor _acceptor;
    std::vector<KeyDataStruct> _receivedData;
};
