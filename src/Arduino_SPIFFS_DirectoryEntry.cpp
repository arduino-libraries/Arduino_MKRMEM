/*
  Copyright (c) 2019 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Arduino_SPIFFS_DirectoryEntry.h"

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_SPIFFS_DirectoryEntry::Arduino_SPIFFS_DirectoryEntry(spiffs_dirent const * dirent)
{
  if(dirent) {
    memcpy(&_dirent, dirent, sizeof(_dirent));
  }
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

String Arduino_SPIFFS_DirectoryEntry::name()
{
  return String(reinterpret_cast<const char *>(&(_dirent.name)));
}

bool Arduino_SPIFFS_DirectoryEntry::isFile()
{
  return (_dirent.type == SPIFFS_TYPE_FILE);
}

bool Arduino_SPIFFS_DirectoryEntry::isDirectory()
{
  return (_dirent.type == SPIFFS_TYPE_DIR);
}

Arduino_SPIFFS_DirectoryEntry Arduino_SPIFFS_DirectoryEntry::create(spiffs_dirent const * dirent)
{
  return Arduino_SPIFFS_DirectoryEntry(dirent);
}

Arduino_SPIFFS_DirectoryEntry Arduino_SPIFFS_DirectoryEntry::operator = (Arduino_SPIFFS_DirectoryEntry const & other)
{
  memcpy(&_dirent, other.getDirEnt(), sizeof(_dirent));
}
