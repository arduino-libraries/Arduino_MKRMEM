#ifndef MKRMEM_H_
#define MKRMEM_H_

#include "spiffs.h"
#include "W25Q16DV.h"

#include "spiffs_config.h"

static spiffs fs;

static u8_t spiffs_work_buf[SPIFFS_CFG_LOG_PAGE_SZ(0)*2];
static u8_t spiffs_fds[32*4];
static u8_t spiffs_cache_buf[(SPIFFS_CFG_LOG_PAGE_SZ(0)+32)*4];

#endif /* MKRMEM_H_ */
