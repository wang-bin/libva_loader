/*
 * Copyright (c) 2021-2023 WangBin <wbsecg1 at gmail.com>
 */
#include "vdpau_symbols.h"
#include <cstdio>
#include <dlfcn.h>
//#include <pthread.h>

static void* libvdpau = nullptr;

_Pragma("weak dlopen") // so no need link to libdl directly, no libvdpau.pc change
_Pragma("weak dlsym") // so no need link to libdl directly, no libvdpau.pc change

static void load_vdpau()
{
    libvdpau = dlopen("libvdpau.so.1", RTLD_LAZY|RTLD_LOCAL);
    if (!libvdpau) {
        printf("failed to load libvdpau.so.1\n");
        return;
    }
}

static vdpau_t vdp;
//static pthread_once_t init_flag = PTHREAD_ONCE_INIT;
//_Pragma("weak pthread_once") // so no need link to pthread directly, no libvdpau.pc change

__attribute__((constructor))
static void init()
{
    if (vdp.device_create_x11)
        return;
    load_vdpau();
    //pthread_once(&init_flag, load_vdpau);
#define DLSYM_VDP(X) do {\
        vdp.X = (decltype(&vdp_##X))dlsym(libvdpau, "vdp_" #X); \
    } while (false)
    DLSYM_VDP(device_create_x11);
}

extern "C" {
VdpStatus vdp_device_create_x11(Display* display, int screen, VdpDevice* device, VdpGetProcAddress** get_proc_address)
{
    return vdp.device_create_x11(display, screen, device, get_proc_address);
}
} // extern "C"