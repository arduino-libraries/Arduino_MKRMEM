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
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS_File
{

public:

  String     name  ();
  inline int read  (void * buf, int len)  { return SPIFFS_read(_fs, _fh, buf, len); }
  inline int write (void * buf, int len)  { return SPIFFS_write(_fs, _fh, buf, len); }
  inline int lseek (int offs, int whence) { return SPIFFS_lseek(_fs, _fh, offs, whence); }
  inline int eof   ()                     { return SPIFFS_eof(_fs, _fh); }
  inline int tell  ()                     { return SPIFFS_tell(_fs, _fh); }
  inline int close ()                     { return SPIFFS_close(_fs, _fh); }
  inline int remove()                     { return SPIFFS_fremove(_fs, _fh); }
  inline int flush ()                     { return SPIFFS_fflush(_fs, _fh); }


  static Arduino_SPIFFS_File create(spiffs * fs, spiffs_file const fh);


  Arduino_SPIFFS_File operator = (Arduino_SPIFFS_File const & other);
  inline spiffs *    getFs() const { return _fs; }
  inline spiffs_file getFh() const { return _fh; }


private:

  spiffs * _fs;
  spiffs_file _fh;

  Arduino_SPIFFS_File(spiffs * fs, spiffs_file const fh);


};

/**************************************************************************************
 * TYPEDEF
 **************************************************************************************/

typedef Arduino_SPIFFS_File File; /* Shorten the class name down because 'Arduino_SPIFFS_File' makes for a very long type name */

#endif /* ARDUINO_SPIFFS_FILE_H_ */
