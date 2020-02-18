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
#include "Arduino_MKRMEM_Config.h"

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/

#if SPIFFS_FLASH_TYPE_W25Q16DV
static Arduino_W25Q16DV_SpiffsHalWrapper w25q16dv_wrapper(flash);
#endif /* SPIFFS_FLASH_TYPE_W25Q16DV */

/**************************************************************************************
 * INTERNAL FUNCTION DECLARATION
 **************************************************************************************/

s32_t spiffs_read_func (spiffs_t * fs, u32_t addr, u32_t size, u8_t * buf);
s32_t spiffs_write_func(spiffs_t * fs, u32_t addr, u32_t size, u8_t * buf);
s32_t spiffs_erase_func(spiffs_t * fs, u32_t addr, u32_t size);

/**************************************************************************************
 * CTOR/DTOR
 **************************************************************************************/

Arduino_SPIFFS::Arduino_SPIFFS(Arduino_SpiffsHalWrapper & flash_hal_wrapper)
{
  _fs.user_data = reinterpret_cast<void *>(&flash_hal_wrapper);

  _cfg.hal_read_f  = spiffs_read_func;
  _cfg.hal_write_f = spiffs_write_func;
  _cfg.hal_erase_f = spiffs_erase_func;
}

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

int Arduino_SPIFFS::mount()
{
  return SPIFFS_mount(&_fs,
                      &_cfg,
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
 * INTERNAL FUNCTION DEFINITION
 **************************************************************************************/

s32_t spiffs_read_func(spiffs_t * fs, u32_t addr, u32_t size, u8_t * buf)
{
  Arduino_SpiffsHalWrapper * flash_hal_wrapper = reinterpret_cast<Arduino_SpiffsHalWrapper *>(fs->user_data);
  return flash_hal_wrapper->read(addr, size, buf);
}

s32_t spiffs_write_func(spiffs_t * fs, u32_t addr, u32_t size, u8_t * buf)
{
  Arduino_SpiffsHalWrapper * flash_hal_wrapper = reinterpret_cast<Arduino_SpiffsHalWrapper *>(fs->user_data);
  return flash_hal_wrapper->write(addr, size, buf);
}

s32_t spiffs_erase_func(spiffs_t * fs, u32_t addr, u32_t size)
{
  Arduino_SpiffsHalWrapper * flash_hal_wrapper = reinterpret_cast<Arduino_SpiffsHalWrapper *>(fs->user_data);
  return flash_hal_wrapper->erase(addr, size);
}

/**************************************************************************************
 * EXTERN DECLARATION
 **************************************************************************************/

Arduino_SPIFFS filesystem(
#if SPIFFS_FLASH_TYPE_W25Q16DV
w25q16dv_wrapper
#endif /* #if SPIFFS_FLASH_TYPE_W25Q16DV */
);
