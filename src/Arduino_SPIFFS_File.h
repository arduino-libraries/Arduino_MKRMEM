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

#ifndef ARDUINO_SPIFFS_FILE_H_
#define ARDUINO_SPIFFS_FILE_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static int constexpr START   = SPIFFS_SEEK_SET; /* Start to seek from the beginning of the file                */
static int constexpr CURRENT = SPIFFS_SEEK_CUR; /* Start to seek from the current position of the file pointer */
static int constexpr END     = SPIFFS_SEEK_END; /* Start to seek from the end of the file                      */

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS_File
{

public:

  ~Arduino_SPIFFS_File();


  int read  (void * buf, int len);
  int write (void * buf, int len);
  int lseek (int offs, int whence);
  int eof   ();
  int tell  ();
  int close ();
  int remove();
  int flush ();


  static Arduino_SPIFFS_File create(spiffs_file const fh);


  Arduino_SPIFFS_File operator = (Arduino_SPIFFS_File const & other);
  operator bool ();
  inline spiffs_file getFh() const { return _fh; }


private:

  spiffs_file _fh;

  Arduino_SPIFFS_File(spiffs_file const fh);

};

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef Arduino_SPIFFS_File File; /* Shorten the class name down because 'Arduino_SPIFFS_File' makes for a very long type name */

#endif /* ARDUINO_SPIFFS_FILE_H_ */
