/*
 * Copyright (c) 2020 WangBin <wbsecg1 at gmail.com>
 */
#pragma once

extern "C" {
#include <va/va_drm.h>
#if VA_CHECK_VERSION(0, 36, 0) //  vaapi 0.36, libva 1.4
# include <va/va_drmcommon.h> /* VA_SURFACE_ATTRIB_MEM_TYPE_DRM_PRIME*/
#endif
#if VA_CHECK_VERSION(1, 0, 0)
# include <va/va_str.h>
#endif
#if (VAAPI_DYLOAD+0)
#include <X11/Xlib.h>
#else
#include <va/va_x11.h> // why ambiguous vaGetDisplay()?
#endif
}

struct va_t {
    decltype(&vaErrorStr) ErrorStr = nullptr;
    decltype(&vaSetErrorCallback) SetErrorCallback = nullptr;
    decltype(&vaSetInfoCallback) SetInfoCallback = nullptr;
    decltype(&vaDisplayIsValid) DisplayIsValid = nullptr;
    decltype(&vaSetDriverName) SetDriverName = nullptr;
    decltype(&vaInitialize) Initialize = nullptr;
    decltype(&vaTerminate) Terminate = nullptr;
    decltype(&vaQueryVendorString) QueryVendorString = nullptr;
    decltype(&vaGetLibFunc) GetLibFunc = nullptr;
    decltype(&vaMaxNumProfiles) MaxNumProfiles = nullptr;
    decltype(&vaMaxNumEntrypoints) MaxNumEntrypoints = nullptr;
    decltype(&vaMaxNumConfigAttributes) MaxNumConfigAttributes = nullptr;
    decltype(&vaQueryConfigProfiles) QueryConfigProfiles = nullptr;
    decltype(&vaQueryConfigEntrypoints) QueryConfigEntrypoints = nullptr;
    decltype(&vaGetConfigAttributes) GetConfigAttributes = nullptr;
    decltype(&vaCreateConfig) CreateConfig = nullptr;
    decltype(&vaDestroyConfig) DestroyConfig = nullptr;
    decltype(&vaQueryConfigAttributes) QueryConfigAttributes = nullptr;
    decltype(&vaQuerySurfaceAttributes) QuerySurfaceAttributes = nullptr;
    decltype(&vaCreateSurfaces) CreateSurfaces = nullptr;
    decltype(&vaDestroySurfaces) DestroySurfaces = nullptr;
    decltype(&vaCreateContext) CreateContext = nullptr;
    decltype(&vaDestroyContext) DestroyContext = nullptr;
    decltype(&vaCreateMFContext) CreateMFContext = nullptr;
    decltype(&vaMFAddContext) MFAddContext = nullptr;
    decltype(&vaMFReleaseContext) MFReleaseContext = nullptr;
    decltype(&vaQueryProcessingRate) QueryProcessingRate = nullptr;
    decltype(&vaCreateBuffer) CreateBuffer = nullptr;
    decltype(&vaCreateBuffer2) CreateBuffer2 = nullptr;
    decltype(&vaBufferSetNumElements) BufferSetNumElements = nullptr;
    decltype(&vaMapBuffer) MapBuffer = nullptr;
    decltype(&vaUnmapBuffer) UnmapBuffer = nullptr;
    decltype(&vaDestroyBuffer) DestroyBuffer = nullptr;
    decltype(&vaAcquireBufferHandle) AcquireBufferHandle = nullptr;
    decltype(&vaReleaseBufferHandle) ReleaseBufferHandle = nullptr;
    decltype(&vaExportSurfaceHandle) ExportSurfaceHandle = nullptr;
    decltype(&vaBeginPicture) BeginPicture = nullptr;
    decltype(&vaRenderPicture) RenderPicture = nullptr;
    decltype(&vaEndPicture) EndPicture = nullptr;
    decltype(&vaMFSubmit) MFSubmit = nullptr;
    decltype(&vaSyncSurface) SyncSurface = nullptr;
    decltype(&vaQuerySurfaceStatus) QuerySurfaceStatus = nullptr;
    decltype(&vaQuerySurfaceError) QuerySurfaceError = nullptr;
    decltype(&vaMaxNumImageFormats) MaxNumImageFormats = nullptr;
    decltype(&vaQueryImageFormats) QueryImageFormats = nullptr;
    decltype(&vaCreateImage) CreateImage = nullptr;
    decltype(&vaDestroyImage) DestroyImage = nullptr;
    decltype(&vaSetImagePalette) SetImagePalette = nullptr;
    decltype(&vaGetImage) GetImage = nullptr;
    decltype(&vaPutImage) PutImage = nullptr;
    decltype(&vaDeriveImage) DeriveImage = nullptr;
    decltype(&vaMaxNumSubpictureFormats) MaxNumSubpictureFormats = nullptr;
    decltype(&vaQuerySubpictureFormats) QuerySubpictureFormats = nullptr;
    decltype(&vaCreateSubpicture) CreateSubpicture = nullptr;
    decltype(&vaDestroySubpicture) DestroySubpicture = nullptr;
    decltype(&vaSetSubpictureImage) SetSubpictureImage = nullptr;
    decltype(&vaSetSubpictureChromakey) SetSubpictureChromakey = nullptr;
    decltype(&vaSetSubpictureGlobalAlpha) SetSubpictureGlobalAlpha = nullptr;
    decltype(&vaAssociateSubpicture) AssociateSubpicture = nullptr;
    decltype(&vaDeassociateSubpicture) DeassociateSubpicture = nullptr;
    decltype(&vaMaxNumDisplayAttributes) MaxNumDisplayAttributes = nullptr;
    decltype(&vaQueryDisplayAttributes) QueryDisplayAttributes = nullptr;
    decltype(&vaGetDisplayAttributes) GetDisplayAttributes = nullptr;
    decltype(&vaSetDisplayAttributes) SetDisplayAttributes = nullptr;

    decltype(&vaProfileStr) ProfileStr = nullptr;
    decltype(&vaEntrypointStr) EntrypointStr = nullptr;
    decltype(&vaConfigAttribTypeStr) ConfigAttribTypeStr = nullptr;
    decltype(&vaBufferTypeStr) BufferTypeStr = nullptr;
    //decltype(&vaStatusStr) StatusStr = nullptr;
// va_vpp.h
    decltype(&vaQueryVideoProcFilters) QueryVideoProcFilters = nullptr;
    decltype(&vaQueryVideoProcFilterCaps) QueryVideoProcFilterCaps = nullptr;
    decltype(&vaQueryVideoProcPipelineCaps) QueryVideoProcPipelineCaps = nullptr;
};

struct va_x11_t {
    decltype(&vaGetDisplay) GetDisplay = nullptr;
    decltype(&vaPutSurface) PutSurface = nullptr;
};

struct va_drm_t {
    decltype(&vaGetDisplayDRM) GetDisplayDRM = nullptr;
};
