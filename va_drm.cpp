/*
 * Copyright (c) 2020-2023 WangBin <wbsecg1 at gmail.com>
 */
#include "va_symbols.h"
extern va_drm_t va_drm;
extern void init();

extern "C" {
VADisplay vaGetDisplayDRM(int fd)
{
    return va_drm.GetDisplayDRM(fd);
}
} // extern "C"