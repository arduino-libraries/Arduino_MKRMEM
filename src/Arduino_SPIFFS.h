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
#include "Arduino_SPIFFS_Directory.h"

extern "C"
{
#include "spiffs.h"
}

/**************************************************************************************
 * CONFIGURATION
 **************************************************************************************/

#define SPIFFS_USE_W25Q16DV_FLASH

/**************************************************************************************
 * SANITY CHECK
 **************************************************************************************/

#if !defined(SPIFFS_USE_W25Q16DV_FLASH)
  #error "Enable at least one flash chip for usage with Arduino_SPIFFS"
#endif

/**************************************************************************************
 * STATIC ASSERTIONS
 **************************************************************************************/

static_assert(sizeof(u8_t) == sizeof(byte), "Arduino SPIFFS Wrapper - u8_t != byte - possible loss of data");
static_assert(sizeof(s32_t) == sizeof(int), "Arduino SPIFFS Wrapper - s32_t != int32_t - possible loss of data");
static_assert(sizeof(u32_t) == sizeof(unsigned int), "Arduino SPIFFS Wrapper - s32_t != int32_t - possible loss of data");

/**************************************************************************************
 * CONSTANTS
 **************************************************************************************/

static uint16_t constexpr APPEND     = SPIFFS_O_APPEND;
static uint16_t constexpr TRUNCATE   = SPIFFS_O_TRUNC;
static uint16_t constexpr CREATE     = SPIFFS_O_CREAT;
static uint16_t constexpr READ_ONLY  = SPIFFS_O_RDONLY;
static uint16_t constexpr WRITE_ONLY = SPIFFS_O_WRONLY;
static uint16_t constexpr READ_WRITE = SPIFFS_O_RDWR;
#ifdef DIRECT
#undef DIRECT
#endif
static uint16_t constexpr DIRECT     = SPIFFS_O_DIRECT; /* Any writes to the filehandle will never be cached but flushed directly */
static uint16_t constexpr EXCLUSIVE  = SPIFFS_O_EXCL;

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS
{
public:

  Arduino_SPIFFS(spiffs_read read_func, spiffs_write write_func, spiffs_erase erase_func);


         int         mount   ();
  inline byte        mounted ()                                           { return SPIFFS_mounted(&_fs); }
  inline void        unmount ()                                           { SPIFFS_unmount(&_fs); }
  
  inline int         format  ()                                           { return SPIFFS_format(&_fs); }
  inline int         check   ()                                           { return SPIFFS_check(&_fs); }
  inline int         info    (unsigned int & total, unsigned int & used)  { return SPIFFS_info(&_fs, reinterpret_cast<u32_t*>(&total), reinterpret_cast<u32_t*>(&used)); }

  inline int         err     ()                                           { return SPIFFS_errno(&_fs); }
  inline void        clearerr()                                           { SPIFFS_clearerr(&_fs); }
  
  inline int         create  (const char * path)                          { return SPIFFS_creat(&_fs, path, 0); }
         File        open    (const char * path, uint16_t const flags);
  inline int         remove  (const char * path)                          { return SPIFFS_remove(&_fs, path); }
  inline int         rename  (const char * old, const char * newPath)     { return SPIFFS_rename(&_fs, old, newPath); }

  inline int         create  (String const & path)                        { return create(path.c_str()); }
  inline File        open    (String const & path, uint16_t const flags)  { return open(path.c_str(), flags); }
  inline int         remove  (String const & path)                        { return remove(path.c_str()); }
  inline int         rename  (String const & old, String const & newPath) { return rename(old.c_str(), newPath.c_str()); }

         Directory   opendir (const char * name);
  inline Directory   opendir (String const & name)                        { return opendir(name.c_str()); }


  /* In order to allow access to the private functions
   * read, write, lseek, eof, tell, close, remove, flush
   * we allow Arduino_SPIFFS_File to be a 'friend' of
   * 'Arduino_SPIFFS'. Another alternative would be to
   * make those functions public but what would users do
   * without the availability of a file handle?
   */
  friend class Arduino_SPIFFS_File; 


private:

  spiffs _fs;
  
  u8_t   _spiffs_work_buf[SPIFFS_CFG_LOG_PAGE_SZ(0)*2];
  u8_t   _spiffs_fds[32*4];
  u8_t   _spiffs_cache_buf[(SPIFFS_CFG_LOG_PAGE_SZ(0)+32)*4];


  inline int read  (spiffs_file fh, void * buf, int len)  { return SPIFFS_read(&_fs, fh, buf, len); }
  inline int write (spiffs_file fh, void * buf, int len)  { return SPIFFS_write(&_fs, fh, buf, len); }
  inline int lseek (spiffs_file fh, int offs, int whence) { return SPIFFS_lseek(&_fs, fh, offs, whence); }
  inline int eof   (spiffs_file fh)                       { return SPIFFS_eof(&_fs, fh); }
  inline int tell  (spiffs_file fh)                       { return SPIFFS_tell(&_fs, fh); }
  inline int close (spiffs_file fh)                       { return SPIFFS_close(&_fs, fh); }
  inline int remove(spiffs_file fh)                       { return SPIFFS_fremove(&_fs, fh); }
  inline int flush (spiffs_file fh)                       { return SPIFFS_fflush(&_fs, fh); }

};

/**************************************************************************************
 * EXTERN DEFINITION
 **************************************************************************************/

extern Arduino_SPIFFS filesystem;

#endif /* ARDUINO_SPIFFS_H_ */
