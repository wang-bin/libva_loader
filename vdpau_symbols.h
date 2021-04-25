/*
 * Copyright (c) 2021 WangBin <wbsecg1 at gmail.com>
 */
#pragma once

extern "C" {
#include <vdpau/vdpau_x11.h>
}

struct vdpau_t {
    decltype(&vdp_device_create_x11) device_create_x11 = nullptr;
};
