/*
 * Copyright (c) 2020 WangBin <wbsecg1 at gmail.com>
 */
#include "va_symbols.h"
#include <cstdio>
#include <cstdlib>
extern va_drm_t va_drm;
extern void init();

extern "C" {
VADisplay vaGetDisplayDRM(int fd)
{
    init();
    return va_drm.GetDisplayDRM(fd);
}
} // extern "C"