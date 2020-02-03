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

#include "Arduino_SPIFFS_File.h"

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_SPIFFS_File::Arduino_SPIFFS_File(spiffs * fs, spiffs_file const fh)
: _fs(fs)
, _fh(fh)
{

}

Arduino_SPIFFS_File::~Arduino_SPIFFS_File()
{
  close();
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

String Arduino_SPIFFS_File::name()
{
  spiffs_stat stat;
  if(SPIFFS_OK == SPIFFS_fstat(_fs, _fh, &stat)) {
    return String(reinterpret_cast<const char *>(&(stat.name)));
  } else {
    return String("ERROR");
  }
}

Arduino_SPIFFS_File Arduino_SPIFFS_File::create(spiffs * fs, spiffs_file const fh)
{
  return Arduino_SPIFFS_File(fs, fh);
}

Arduino_SPIFFS_File Arduino_SPIFFS_File::operator = (Arduino_SPIFFS_File const & other)
{
  _fs = other.getFs();
  _fh = other.getFh();
}