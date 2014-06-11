#pragma once

class CapturerWin
{
public:
    CapturerWin(Client* client);

    void run();

    std::string GeyKey(unsigned char c);
    std::string intToString(unsigned short i);

private:
    Client* _client;
    std::string _user;
};
