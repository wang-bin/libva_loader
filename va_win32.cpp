/*
 * Copyright (c) 2023 WangBin <wbsecg1 at gmail.com>
 */
#include "va_symbols.h"
extern va_win32_t va_win32;
extern void init();

extern "C" {
VADisplay vaGetDisplayWin32(const LUID* adapter_luid)
{
    return va_win32.GetDisplayWin32(adapter_luid);
}
} // extern "C"