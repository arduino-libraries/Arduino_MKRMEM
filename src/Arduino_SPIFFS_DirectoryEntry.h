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

#ifndef ARDUINO_SPIFFS_DIRECTORYENTRY_H_
#define ARDUINO_SPIFFS_DIRECTORYENTRY_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS_DirectoryEntry
{

public:

  Arduino_SPIFFS_DirectoryEntry() : Arduino_SPIFFS_DirectoryEntry(0) { }
  Arduino_SPIFFS_DirectoryEntry(spiffs_dirent const * dirent);


  String name();
  bool   isFile();
  bool   isDirectory();


  static Arduino_SPIFFS_DirectoryEntry create(spiffs_dirent const * dirent);
  
  
  Arduino_SPIFFS_DirectoryEntry operator = (Arduino_SPIFFS_DirectoryEntry const & other);
  inline spiffs_dirent const * getDirEnt() const { return &_dirent; }


private:

  spiffs_dirent _dirent;

};

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef Arduino_SPIFFS_DirectoryEntry DirEntry; /* Shorten the class name down because 'Arduino_SPIFFS_DirectoryEntry' makes for a very long type name */

#endif /* ARDUINO_SPIFFS_DIRECTORYENTRY_H_ */
