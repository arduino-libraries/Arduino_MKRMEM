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

#ifndef ARDUINO_SPIFFS_DIRECTORY_H_
#define ARDUINO_SPIFFS_DIRECTORY_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include "Arduino_SPIFFS_DirectoryEntry.h"

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS_Directory
{

public:

  ~Arduino_SPIFFS_Directory();


  bool closedir();
  bool readdir (DirEntry & dir_entry);
  

  static Arduino_SPIFFS_Directory create(spiffs_DIR const * dir);


  Arduino_SPIFFS_Directory operator = (Arduino_SPIFFS_Directory const & other);
  inline spiffs_DIR const * getDir() const { return &_dir; }


private:

  spiffs_DIR _dir;

  Arduino_SPIFFS_Directory(spiffs_DIR const * dir);

};

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef Arduino_SPIFFS_Directory Directory; /* Shorten the class name down because 'Arduino_SPIFFS_Directory' makes for a very long type name */

#endif /* ARDUINO_SPIFFS_DIRECTORY_H_ */
