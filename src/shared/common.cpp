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

std::string TimeStamp()
{
    std::array<char, 64> buffer;
    buffer.fill(0);
    time_t rawtime = time(nullptr);
    auto timeinfo = localtime(&rawtime);
    strftime(buffer.data(), sizeof(buffer), "%Y-%m-%d %H-%M-%S", timeinfo);
    return std::string(buffer.data());
}