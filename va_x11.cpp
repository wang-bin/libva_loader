/*
 * Copyright (c) 2020 WangBin <wbsecg1 at gmail.com>
 */
#include "va_symbols.h"
extern va_x11_t va_x11;
extern void init();

extern "C" {

VADisplay vaGetDisplay(Display *dpy)
{
    return va_x11.GetDisplay(dpy);
}

VAStatus vaPutSurface (
    VADisplay dpy,
    VASurfaceID surface,
    Drawable draw, /* X Drawable */
    short srcx,
    short srcy,
    unsigned short srcw,
    unsigned short srch,
    short destx,
    short desty,
    unsigned short destw,
    unsigned short desth,
    VARectangle *cliprects, /* client supplied destination clip list */
    unsigned int number_cliprects, /* number of clip rects in the clip list */
    unsigned int flags /* PutSurface flags */
)
{
    return va_x11.PutSurface(dpy, surface, draw, srcx, srcy, srcw, srch, destx, desty, destw, desth, cliprects, number_cliprects, flags);
}

} // extern "C"