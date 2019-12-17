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

#include "spiffs.h"

/**************************************************************************************
 * CLASS DECLARATION
 **************************************************************************************/

class Arduino_SPIFFS
{
public:

         s32_t       mount   ();
  inline u8_t        mounted ()                                                       { return SPIFFS_mounted(&_fs); }
  inline void        unmount ()                                                       { SPIFFS_unmount(&_fs); }
  
  inline s32_t       format  ()                                                       { return SPIFFS_format(&_fs); }
  inline s32_t       check   ()                                                       { return SPIFFS_check(&_fs); }
  inline s32_t       info    (u32_t & total, u32_t & used)                            { return SPIFFS_info(&_fs, &total, &used); }

  inline s32_t       err     ()                                                       { return SPIFFS_errno(&_fs); }
  inline void        clearerr()                                                       { SPIFFS_clearerr(&_fs); }
  
  inline s32_t       create  (const char *path, spiffs_mode mode)                     { return SPIFFS_creat(&_fs, path, mode); }
  inline spiffs_file open    (const char *path, spiffs_flags flags, spiffs_mode mode) { return SPIFFS_open(&_fs, path, flags, mode); }
  inline s32_t       read    (spiffs_file fh, void *buf, s32_t len)                   { return SPIFFS_read(&_fs, fh, buf, len); }
  inline s32_t       write   (spiffs_file fh, void *buf, s32_t len)                   { return SPIFFS_write(&_fs, fh, buf, len); }
  inline s32_t       lseek   (spiffs_file fh, s32_t offs, int whence)                 { return SPIFFS_lseek(&_fs, fh, offs, whence); }
  inline s32_t       eof     (spiffs_file fh)                                         { return SPIFFS_eof(&_fs, fh); }
  inline s32_t       tell    (spiffs_file fh)                                         { return SPIFFS_tell(&_fs, fh); }
  inline s32_t       close   (spiffs_file fh)                                         { return SPIFFS_close(&_fs, fh); }

  inline s32_t       remove   (const char *path)                                      { return SPIFFS_remove(&_fs, path); }
  inline s32_t       fremove  (spiffs_file fh)                                        { return SPIFFS_fremove(&_fs, fh); }
  inline s32_t       stat     (const char *path, spiffs_stat *s)                      { return SPIFFS_stat(&_fs, path, s); }
  inline s32_t       fstat    (spiffs_file fh, spiffs_stat *s)                        { return SPIFFS_fstat(&_fs, fh, s); }
  inline s32_t       fflush   (spiffs_file fh)                                        { return SPIFFS_fflush(&_fs, fh); }
  inline s32_t       rename   (const char *old, const char *newPath)                  { return SPIFFS_rename(&_fs, old, newPath); }

  inline spiffs_DIR *           opendir (const char *name, spiffs_DIR *d)             { return SPIFFS_opendir(&_fs, name, d); }
  inline s32_t                  closedir(spiffs_DIR *d)                               { return SPIFFS_closedir(d); }
  inline struct spiffs_dirent * readdir (spiffs_DIR *d, struct spiffs_dirent *e)      { return SPIFFS_readdir(d, e); }


private:

  spiffs _fs;
  
  u8_t   _spiffs_work_buf[SPIFFS_CFG_LOG_PAGE_SZ(0)*2];
  u8_t   _spiffs_fds[32*4];
  u8_t   _spiffs_cache_buf[(SPIFFS_CFG_LOG_PAGE_SZ(0)+32)*4];

};

#endif /* ARDUINO_SPIFFS_H_ */
