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

#include "Arduino_SPIFFS_Directory.h"

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_SPIFFS_Directory::Arduino_SPIFFS_Directory(spiffs_DIR const * dir)
{
  memcpy(&_dir, dir, sizeof(_dir));
}

Arduino_SPIFFS_Directory::~Arduino_SPIFFS_Directory()
{
  closedir();
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

bool Arduino_SPIFFS_Directory::closedir()
{ 
  return (SPIFFS_OK == SPIFFS_closedir(&_dir));
}

bool Arduino_SPIFFS_Directory::readdir(DirEntry & dir_entry)
{
  spiffs_dirent de;
  
  if(SPIFFS_readdir(&_dir, &de)) {
    dir_entry = DirEntry::create(&de);
    return true;
  } else {
    return false;
  }
}

Arduino_SPIFFS_Directory Arduino_SPIFFS_Directory::operator = (Arduino_SPIFFS_Directory const & other)
{
  memcpy(&_dir, other.getDir(), sizeof(_dir));
}

Arduino_SPIFFS_Directory Arduino_SPIFFS_Directory::create(spiffs_DIR const * dir)
{
  return Arduino_SPIFFS_Directory(dir);
}
