#include "common.hpp"
#include "MindClient.h"
#include "CapturerWin.h"

#define SERVER_ADDRESS "localhost"
#define SERVER_PORT "1995"

void ClientRun(boost::asio::io_service* io_service, Client* clt)
{
    try
    {
        clt->StartSendOperations();
        io_service->run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

int WINAPI WinMain(HINSTANCE thisinstance, HINSTANCE previnstance, LPSTR cmdline, int ncmdshow)
{
    boost::asio::io_service io_service;
    Client* client = new Client(io_service, SERVER_ADDRESS, SERVER_PORT);

    CapturerWin* capturer = new CapturerWin(client);
    std::thread th(&CapturerWin::run, capturer);

    ClientRun(&io_service, client);

    th.join();

    delete capturer;
    delete client;

    return EXIT_SUCCESS;
}