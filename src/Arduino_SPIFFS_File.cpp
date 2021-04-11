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

#include <Arduino_SPIFFS.h>

/**************************************************************************************
 * EXTERN DEFINITION
 **************************************************************************************/

extern Arduino_SPIFFS filesystem;

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_SPIFFS_File::Arduino_SPIFFS_File(spiffs_file const fh)
: _fh(fh)
{

}

Arduino_SPIFFS_File::~Arduino_SPIFFS_File()
{
  close();
}

/**************************************************************************************
 * OPERATOR OVERRIDES
 **************************************************************************************/

Arduino_SPIFFS_File::operator bool()
{
  return _fh >= 0;
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

int Arduino_SPIFFS_File::read(void * buf, int len)
{
  return filesystem.read(_fh, buf, len);
}

int Arduino_SPIFFS_File::write(void * buf, int len)
{
  return filesystem.write(_fh, buf, len);
}

int Arduino_SPIFFS_File::lseek(int offs, int whence)
{
  return filesystem.lseek(_fh, offs, whence);
}

int Arduino_SPIFFS_File::eof()
{
  return filesystem.eof(_fh);
}

int Arduino_SPIFFS_File::tell()
{
  return filesystem.tell(_fh);
}

int Arduino_SPIFFS_File::close()
{
  return filesystem.close(_fh);
}

int Arduino_SPIFFS_File::remove()
{
  return filesystem.remove(_fh);
}

int Arduino_SPIFFS_File::flush()
{
  return filesystem.flush(_fh);
}

Arduino_SPIFFS_File Arduino_SPIFFS_File::create(spiffs_file const fh)
{
  return Arduino_SPIFFS_File(fh);
}

Arduino_SPIFFS_File Arduino_SPIFFS_File::operator = (Arduino_SPIFFS_File const & other)
{
  _fh = other.getFh();
}
