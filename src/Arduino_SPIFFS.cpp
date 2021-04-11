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

#include "Arduino_SPIFFS.h"

#include "Arduino_W25Q16DV.h"

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

static spiffs_config cfg;

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_SPIFFS::Arduino_SPIFFS(spiffs_read read_func, spiffs_write write_func, spiffs_erase erase_func)
{
  cfg.hal_read_f  = read_func;
  cfg.hal_write_f = write_func;
  cfg.hal_erase_f = erase_func;
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

int Arduino_SPIFFS::mount()
{
  return SPIFFS_mount(&_fs,
                      &cfg,
                      _spiffs_work_buf,
                      _spiffs_fds,
                      sizeof(_spiffs_fds),
                      _spiffs_cache_buf,
                      sizeof(_spiffs_cache_buf),
                      0);
}

File Arduino_SPIFFS::open(const char *path, uint16_t const flags)
{
  spiffs_file const fh = SPIFFS_open(&_fs, path, flags, 0);
  return File::create(fh);
}

Directory Arduino_SPIFFS::opendir(const char *name)
{
  spiffs_DIR d;
  SPIFFS_opendir(&_fs, name, &d);
  return Directory::create(&d);
}

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

Arduino_SPIFFS filesystem(
#if defined(SPIFFS_USE_W25Q16DV_FLASH)
w25q16_spi_read, w25q16_spi_write, w25q16_spi_erase
#endif
);
