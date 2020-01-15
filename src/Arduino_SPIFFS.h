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

#ifndef ARDUINO_SPIFFS_H_
#define ARDUINO_SPIFFS_H_

/**************************************************************************************
 * INCLUDE
 **************************************************************************************/

#include <Arduino.h>

#include "Arduino_SPIFFS_File.h"

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * STATIC ASSERTIONS
 **************************************************************************************/

static_assert(sizeof(u8_t) == sizeof(byte), "Arduino SPIFFS Wrapper - u8_t != byte - possible loss of data");
static_assert(sizeof(s32_t) == sizeof(int), "Arduino SPIFFS Wrapper - s32_t != int32_t - possible loss of data");
static_assert(sizeof(u32_t) == sizeof(unsigned int), "Arduino SPIFFS Wrapper - s32_t != int32_t - possible loss of data");

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS
{
public:

         int         mount   ();
  inline byte        mounted ()                                                       { return SPIFFS_mounted(&_fs); }
  inline void        unmount ()                                                       { SPIFFS_unmount(&_fs); }
  
  inline int         format  ()                                                       { return SPIFFS_format(&_fs); }
  inline int         check   ()                                                       { return SPIFFS_check(&_fs); }
  inline int         info    (unsigned int & total, unsigned int & used)              { return SPIFFS_info(&_fs, reinterpret_cast<u32_t*>(&total), reinterpret_cast<u32_t*>(&used)); }

  inline int         err     ()                                                       { return SPIFFS_errno(&_fs); }
  inline void        clearerr()                                                       { SPIFFS_clearerr(&_fs); }
  
  inline int         create  (const char *path)                                       { return SPIFFS_creat(&_fs, path, 0); }
         File        open    (const char *path, spiffs_flags flags);

  inline int         remove   (const char *path)                                      { return SPIFFS_remove(&_fs, path); }
  inline int         stat     (const char *path, spiffs_stat &s)                      { return SPIFFS_stat(&_fs, path, &s); }
  inline int         fstat    (spiffs_file fh, spiffs_stat &s)                        { return SPIFFS_fstat(&_fs, fh, &s); }
  inline int         rename   (const char *old, const char *newPath)                  { return SPIFFS_rename(&_fs, old, newPath); }

  inline spiffs_DIR *           opendir (const char *name, spiffs_DIR *d)             { return SPIFFS_opendir(&_fs, name, d); }
  inline int                    closedir(spiffs_DIR *d)                               { return SPIFFS_closedir(d); }
  inline struct spiffs_dirent * readdir (spiffs_DIR *d, struct spiffs_dirent *e)      { return SPIFFS_readdir(d, e); }


private:

  spiffs _fs;
  
  u8_t   _spiffs_work_buf[SPIFFS_CFG_LOG_PAGE_SZ(0)*2];
  u8_t   _spiffs_fds[32*4];
  u8_t   _spiffs_cache_buf[(SPIFFS_CFG_LOG_PAGE_SZ(0)+32)*4];

};

#endif /* ARDUINO_SPIFFS_H_ */
