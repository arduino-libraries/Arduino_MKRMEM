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

#include "W25Q16DV.h"

/**************************************************************************************
 * INTERNAL FUNCTION DECLARATION
 **************************************************************************************/

s32_t w25q16_spi_read (u32_t addr, u32_t size, u8_t * buf);
s32_t w25q16_spi_write(u32_t addr, u32_t size, u8_t * buf);
s32_t w25q16_spi_erase(u32_t addr, u32_t size);

/**************************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 **************************************************************************************/

s32_t Arduino_SPIFFS::format()
{
  return SPIFFS_format(&_fs);
}

s32_t Arduino_SPIFFS::mount()
{
  spiffs_config cfg;
    
  cfg.hal_read_f  = w25q16_spi_read;
  cfg.hal_write_f = w25q16_spi_write;
  cfg.hal_erase_f = w25q16_spi_erase;
    
  return SPIFFS_mount(&_fs,
                      &cfg,
                      _spiffs_work_buf,
                      _spiffs_fds,
                      sizeof(_spiffs_fds),
                      _spiffs_cache_buf,
                      sizeof(_spiffs_cache_buf),
                      0);
}

s32_t Arduino_SPIFFS::check()
{
  return SPIFFS_check(&_fs);
}

s32_t Arduino_SPIFFS::info(u32_t & total, u32_t & used)
{
  return SPIFFS_info(&_fs, &total, &used);
}

void Arduino_SPIFFS::unmount()
{
  SPIFFS_unmount(&_fs);
}

/**************************************************************************************
 * INTERNAL FUNCTION DEFINITION
 **************************************************************************************/

s32_t w25q16_spi_read(u32_t addr, u32_t size, u8_t * buf)
{
  w25q16dv.read(addr, buf, size);
  return SPIFFS_OK;
}

s32_t w25q16_spi_write(u32_t addr, u32_t size, u8_t * buf)
{
  w25q16dv.programPage(addr, buf, size);
  return SPIFFS_OK;
}

s32_t w25q16_spi_erase(u32_t addr, u32_t size)
{
  w25q16dv.eraseSector(addr);
  return SPIFFS_OK;
}