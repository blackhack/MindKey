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

#ifndef SERIALIZATION_STRUCT_H
#define SERIALIZATION_STRUCT_H

#include "common.hpp"

#define SERIALIZE_VERSION 1

struct PacketStruct
{
    uint32_t Id;
    std::vector<std::string> Data;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & Id;
        ar & Data;
    }
};

BOOST_CLASS_VERSION(PacketStruct, SERIALIZE_VERSION)

#endif // SERIALIZATION_STRUCT_H
