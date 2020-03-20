/*
 * Copyright (c) 2020 WangBin <wbsecg1 at gmail.com>
 */
#include "va_symbols.h"
#include <dlfcn.h>
#include <pthread.h>
#include <cstdio>

static void* libva = nullptr;
static void* libva_x11 = nullptr;
static void* libva_drm = nullptr;

_Pragma("weak dlopen") // so no need link to libdl directly, no libva.pc change
_Pragma("weak dlsym") // so no need link to libdl directly, no libva.pc change

static void load_va()
{
    static int v = 2;
    libva = dlopen("libva.so.2", RTLD_LAZY|RTLD_LOCAL);
    if (!libva) {
        v = 1;
        printf("failed to load libva.so.2, try libva.so.1\n");
        libva = dlopen("libva.so.1", RTLD_LAZY|RTLD_LOCAL);
    }
    if (!libva) {
        printf("failed to load libva.so.1\n");
        return;
    }
    char va_name[32]{};
    std::snprintf(va_name, sizeof(va_name), "libva-x11.so.%d", v);
    libva_x11 = dlopen(va_name, RTLD_LAZY|RTLD_LOCAL);
    std::snprintf(va_name, sizeof(va_name), "libva-drm.so.%d", v);
    libva_drm = dlopen(va_name, RTLD_LAZY|RTLD_LOCAL);
}

static va_t va;
va_x11_t va_x11;
va_drm_t va_drm;

static pthread_once_t init_flag = PTHREAD_ONCE_INIT;
_Pragma("weak pthread_once") // so no need link to pthread directly, no libva.pc change

void init()
{
    if (va.Initialize)
        return;
    pthread_once(&init_flag, load_va);
#define DLSYM_VA(X) do {\
        va.X = (decltype(&va##X))dlsym(libva, "va" #X); \
    } while (false)
    DLSYM_VA(ErrorStr);
    DLSYM_VA(SetErrorCallback);
    DLSYM_VA(SetInfoCallback);
    DLSYM_VA(DisplayIsValid);
    DLSYM_VA(SetDriverName);
    DLSYM_VA(Initialize);
    DLSYM_VA(Terminate);
    DLSYM_VA(QueryVendorString);
    DLSYM_VA(GetLibFunc);
    DLSYM_VA(MaxNumProfiles);
    DLSYM_VA(MaxNumEntrypoints);
    DLSYM_VA(MaxNumConfigAttributes);
    DLSYM_VA(QueryConfigProfiles);
    DLSYM_VA(QueryConfigEntrypoints);
    DLSYM_VA(GetConfigAttributes);
    DLSYM_VA(CreateConfig);
    DLSYM_VA(DestroyConfig);
    DLSYM_VA(QueryConfigAttributes);
    DLSYM_VA(QuerySurfaceAttributes);
    DLSYM_VA(CreateSurfaces);
    DLSYM_VA(DestroySurfaces);
    DLSYM_VA(CreateContext);
    DLSYM_VA(DestroyContext);
    DLSYM_VA(CreateMFContext);
    DLSYM_VA(MFAddContext);
    DLSYM_VA(MFReleaseContext);
    DLSYM_VA(QueryProcessingRate);
    DLSYM_VA(CreateBuffer);
    DLSYM_VA(CreateBuffer2);
    DLSYM_VA(BufferSetNumElements);
    DLSYM_VA(MapBuffer);
    DLSYM_VA(UnmapBuffer);
    DLSYM_VA(DestroyBuffer);
    DLSYM_VA(AcquireBufferHandle);
    DLSYM_VA(ReleaseBufferHandle);
    DLSYM_VA(ExportSurfaceHandle);
    DLSYM_VA(BeginPicture);
    DLSYM_VA(RenderPicture);
    DLSYM_VA(EndPicture);
    DLSYM_VA(MFSubmit);
    DLSYM_VA(SyncSurface);
    DLSYM_VA(QuerySurfaceStatus);
    DLSYM_VA(QuerySurfaceError);
    DLSYM_VA(MaxNumImageFormats);
    DLSYM_VA(QueryImageFormats);
    DLSYM_VA(CreateImage);
    DLSYM_VA(DestroyImage);
    DLSYM_VA(SetImagePalette);
    DLSYM_VA(GetImage);
    DLSYM_VA(PutImage);
    DLSYM_VA(DeriveImage);
    DLSYM_VA(MaxNumSubpictureFormats);
    DLSYM_VA(QuerySubpictureFormats);
    DLSYM_VA(CreateSubpicture);
    DLSYM_VA(DestroySubpicture);
    DLSYM_VA(SetSubpictureImage);
    DLSYM_VA(SetSubpictureChromakey);
    DLSYM_VA(SetSubpictureGlobalAlpha);
    DLSYM_VA(AssociateSubpicture);
    DLSYM_VA(DeassociateSubpicture);
    DLSYM_VA(MaxNumDisplayAttributes);
    DLSYM_VA(QueryDisplayAttributes);
    DLSYM_VA(GetDisplayAttributes);
    DLSYM_VA(SetDisplayAttributes);

    DLSYM_VA(ProfileStr);
    DLSYM_VA(EntrypointStr);
    DLSYM_VA(ConfigAttribTypeStr);
    DLSYM_VA(BufferTypeStr);
#undef DLSYM_VA
#define DLSYM_VA(X) va_x11.X = (decltype(&va##X))dlsym(libva_x11, "va" #X)
    DLSYM_VA(GetDisplay);
    DLSYM_VA(PutSurface);
#undef DLSYM_VA
#define DLSYM_VA(X) va_drm.X = (decltype(&va##X))dlsym(libva_drm, "va" #X)
    DLSYM_VA(GetDisplayDRM);
}

extern "C" {
/* visibility hidden */
const char *vaErrorStr(VAStatus error_status)
{
    init();
    return va.ErrorStr(error_status);
}

// MUST init() in most functions if this is a static library
VAMessageCallback vaSetErrorCallback(VADisplay dpy, VAMessageCallback callback, void *user_context)
{
    init();
    if (!va.SetErrorCallback)
        return nullptr;
    return va.SetErrorCallback(dpy, callback, user_context);
}

VAMessageCallback vaSetInfoCallback(VADisplay dpy, VAMessageCallback callback, void *user_context)
{
    init();
    if (!va.SetInfoCallback)
        return nullptr;
    return va.SetInfoCallback(dpy, callback, user_context);
}

int vaDisplayIsValid(VADisplay dpy)
{
    init();
    return va.DisplayIsValid(dpy);
}

VAStatus vaSetDriverName(VADisplay dpy, char *driver_name)
{
    init();
    if (!va.SetDriverName)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.SetDriverName(dpy, driver_name);
}

VAStatus vaInitialize(VADisplay dpy, int *major_version, int *minor_version)
{
    init();
    return va.Initialize(dpy, major_version, minor_version);
}

VAStatus vaTerminate(VADisplay dpy)
{
    init();
    return va.Terminate(dpy);
}

const char *vaQueryVendorString(VADisplay dpy)
{
    init();
    return va.QueryVendorString(dpy);
}

VAPrivFunc vaGetLibFunc(VADisplay dpy, const char *func)
{
    init();
    return va.GetLibFunc(dpy, func);
}

int vaMaxNumProfiles(VADisplay dpy)
{
    init();
    return va.MaxNumProfiles(dpy);
}

int vaMaxNumEntrypoints(VADisplay dpy)
{
    init();
    return va.MaxNumEntrypoints(dpy);
}

int vaMaxNumConfigAttributes(VADisplay dpy)
{
    init();
    return va.MaxNumConfigAttributes(dpy);
}

VAStatus vaQueryConfigProfiles(VADisplay dpy, VAProfile *profile_list, int *num_profiles)
{
    init();
    return va.QueryConfigProfiles(dpy, profile_list, num_profiles);
}

VAStatus vaQueryConfigEntrypoints(VADisplay dpy, VAProfile profile,
                                  VAEntrypoint *entrypoint_list,
                                  int *num_entrypoints)
{
    init();
    return va.QueryConfigEntrypoints(dpy, profile, entrypoint_list, num_entrypoints);
}

VAStatus vaGetConfigAttributes(VADisplay dpy, VAProfile profile,
                               VAEntrypoint entrypoint,
                               VAConfigAttrib *attrib_list, int num_attribs)
{
    init();
    if (!va.GetConfigAttributes)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.GetConfigAttributes(dpy, profile, entrypoint, attrib_list, num_attribs);
}

VAStatus vaCreateConfig(VADisplay dpy, VAProfile profile,
                        VAEntrypoint entrypoint, VAConfigAttrib *attrib_list,
                        int num_attribs, VAConfigID *config_id)
{
    init();
    return va.CreateConfig(dpy, profile, entrypoint, attrib_list, num_attribs, config_id);
}

VAStatus vaDestroyConfig(VADisplay dpy, VAConfigID config_id)
{
    init();
    return va.DestroyConfig(dpy, config_id);
}

VAStatus vaQueryConfigAttributes(VADisplay dpy, VAConfigID config_id,
                                 VAProfile *profile, VAEntrypoint *entrypoint,
                                 VAConfigAttrib *attrib_list, int *num_attribs)
{
    init();
    return va.QueryConfigAttributes(dpy, config_id, profile, entrypoint, attrib_list, num_attribs);
}

VAStatus vaQuerySurfaceAttributes(VADisplay dpy, VAConfigID config,
                                  VASurfaceAttrib *attrib_list,
                                  unsigned int *num_attribs)
{
    init();
    return va.QuerySurfaceAttributes(dpy, config, attrib_list, num_attribs);
}

VAStatus vaCreateSurfaces(VADisplay dpy, unsigned int format,
                          unsigned int width, unsigned int height,
                          VASurfaceID *surfaces, unsigned int num_surfaces,
                          VASurfaceAttrib *attrib_list,
                          unsigned int num_attribs)
{
    init();
    return va.CreateSurfaces(dpy, format, width, height, surfaces, num_surfaces, attrib_list, num_surfaces);
}

VAStatus vaDestroySurfaces(VADisplay dpy, VASurfaceID *surfaces,
                           int num_surfaces)
{
    init();
    return va.DestroySurfaces(dpy, surfaces, num_surfaces);
}

VAStatus vaCreateContext(VADisplay dpy, VAConfigID config_id, int picture_width,
                         int picture_height, int flag,
                         VASurfaceID *render_targets, int num_render_targets,
                         VAContextID *context)
{
    init();
    return va.CreateContext(dpy, config_id, picture_width, picture_height, flag, render_targets, num_render_targets, context);
}

VAStatus vaDestroyContext(VADisplay dpy, VAContextID context)
{
    init();
    return va.DestroyContext(dpy, context);
}

VAStatus vaCreateMFContext(VADisplay dpy, VAMFContextID *mf_context)
{
    init();
    return va.CreateMFContext(dpy, mf_context);
}

VAStatus vaMFAddContext(VADisplay dpy, VAMFContextID mf_context,
                        VAContextID context)
{
    init();
    return va.MFAddContext(dpy, mf_context, context);
}

VAStatus vaMFReleaseContext(VADisplay dpy, VAMFContextID mf_context,
                            VAContextID context)
{
    init();
    return va.MFReleaseContext(dpy, mf_context, context);
}

VAStatus vaQueryProcessingRate(VADisplay dpy, VAConfigID config,
                               VAProcessingRateParameter *proc_buf,
                               unsigned int *processing_rate)
{
    init();
    return va.QueryProcessingRate(dpy, config, proc_buf, processing_rate);
}

VAStatus vaCreateBuffer(VADisplay dpy, VAContextID context, VABufferType type,
                        unsigned int size, unsigned int num_elements,
                        void *data, VABufferID *buf_id)
{
    init();
    return va.CreateBuffer(dpy, context, type, size, num_elements, data, buf_id);
}

VAStatus vaCreateBuffer2(VADisplay dpy, VAContextID context, VABufferType type,
                         unsigned int width, unsigned int height,
                         unsigned int *unit_size, unsigned int *pitch,
                         VABufferID *buf_id)
{
    init();
    return va.CreateBuffer2(dpy, context, type, width, height, unit_size, pitch, buf_id);
}

VAStatus vaBufferSetNumElements(VADisplay dpy, VABufferID buf_id,
                                unsigned int num_elements)
{
    init();
    return va.BufferSetNumElements(dpy, buf_id, num_elements);
}

VAStatus vaMapBuffer(VADisplay dpy, VABufferID buf_id, void **pbuf)
{
    init();
    return va.MapBuffer(dpy, buf_id, pbuf);
}

VAStatus vaUnmapBuffer(VADisplay dpy, VABufferID buf_id)
{
    init();
    return va.UnmapBuffer(dpy, buf_id);
}

VAStatus vaDestroyBuffer(VADisplay dpy, VABufferID buffer_id)
{
    init();
    return va.DestroyBuffer(dpy, buffer_id);
}

VAStatus vaAcquireBufferHandle(VADisplay dpy, VABufferID buf_id,
                               VABufferInfo *buf_info)
{
    init();
    if (!va.AcquireBufferHandle)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.AcquireBufferHandle(dpy, buf_id, buf_info);
}

VAStatus vaReleaseBufferHandle(VADisplay dpy, VABufferID buf_id)
{
    init();
    if (!va.ReleaseBufferHandle)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.ReleaseBufferHandle(dpy, buf_id);
}

VAStatus vaExportSurfaceHandle(VADisplay dpy, VASurfaceID surface_id,
                               uint32_t mem_type, uint32_t flags,
                               void *descriptor)
{
    init();
    if (!va.ExportSurfaceHandle)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.ExportSurfaceHandle(dpy, surface_id, mem_type, flags, descriptor);
}

VAStatus vaBeginPicture(VADisplay dpy, VAContextID context,
                        VASurfaceID render_target)
{
    init();
    return va.BeginPicture(dpy, context, render_target);
}

VAStatus vaRenderPicture(VADisplay dpy, VAContextID context,
                         VABufferID *buffers, int num_buffers)
{
    init();
    return va.RenderPicture(dpy, context, buffers, num_buffers);
}

VAStatus vaEndPicture(VADisplay dpy, VAContextID context)
{
    init();
    return va.EndPicture(dpy, context);
}

VAStatus vaMFSubmit(VADisplay dpy, VAMFContextID mf_context,
                    VAContextID *contexts, int num_contexts)
{
    init();
    return va.MFSubmit(dpy, mf_context, contexts, num_contexts);
}

VAStatus vaSyncSurface(VADisplay dpy, VASurfaceID render_target)
{
    init();
    return va.SyncSurface(dpy, render_target);
}

VAStatus vaQuerySurfaceStatus(VADisplay dpy, VASurfaceID render_target,
                              VASurfaceStatus *status)
{
    init();
    return va.QuerySurfaceStatus(dpy, render_target, status);
}

VAStatus vaQuerySurfaceError(VADisplay dpy, VASurfaceID surface,
                             VAStatus error_status, void **error_info)
{
    init();
    return va.QuerySurfaceError(dpy, surface, error_status, error_info);
}

int vaMaxNumImageFormats(VADisplay dpy)
{
    init();
    return va.MaxNumImageFormats(dpy);
}

VAStatus vaQueryImageFormats(VADisplay dpy, VAImageFormat *format_list,
                             int *num_formats)
{
    init();
    return va.QueryImageFormats(dpy, format_list, num_formats);
}

VAStatus vaCreateImage(VADisplay dpy, VAImageFormat *format, int width,
                       int height, VAImage *image)
{
    init();
    return va.CreateImage(dpy, format, width, height, image);
}

VAStatus vaDestroyImage(VADisplay dpy, VAImageID image)
{
    init();
    return va.DestroyImage(dpy, image);
}

VAStatus vaSetImagePalette(VADisplay dpy, VAImageID image,
                           unsigned char *palette)
{
    init();
    return va.SetImagePalette(dpy, image, palette);
}

VAStatus vaGetImage(VADisplay dpy, VASurfaceID surface, int x, int y,
                    unsigned int width, unsigned int height, VAImageID image)
{
    init();
    return va.GetImage(dpy, surface, x, y, width, height, image);
}

VAStatus vaPutImage(VADisplay dpy, VASurfaceID surface, VAImageID image,
                    int src_x, int src_y, unsigned int src_width,
                    unsigned int src_height, int dest_x, int dest_y,
                    unsigned int dest_width, unsigned int dest_height)
{
    init();
    return va.PutImage(dpy, surface, image, src_x, src_y, src_width, src_height, dest_x, dest_y, dest_width, dest_height);
}

VAStatus vaDeriveImage(VADisplay dpy, VASurfaceID surface, VAImage *image)
{
    init();
    return va.DeriveImage(dpy, surface, image);
}

int vaMaxNumSubpictureFormats(VADisplay dpy)
{
    init();
    return va.MaxNumSubpictureFormats(dpy);
}

VAStatus vaQuerySubpictureFormats(VADisplay dpy, VAImageFormat *format_list,
                                  unsigned int *flags,
                                  unsigned int *num_formats)
{
    init();
    return va.QuerySubpictureFormats(dpy, format_list, flags, num_formats);
}

VAStatus vaCreateSubpicture(VADisplay dpy, VAImageID image,
                            VASubpictureID *subpicture)
{
    init();
    return va.CreateSubpicture(dpy, image, subpicture);
}

VAStatus vaDestroySubpicture(VADisplay dpy, VASubpictureID subpicture)
{
    init();
    return va.DestroySubpicture(dpy, subpicture);
}

VAStatus vaSetSubpictureImage(VADisplay dpy, VASubpictureID subpicture,
                              VAImageID image)
{
    init();
    return va.SetSubpictureImage(dpy, subpicture, image);
}

VAStatus vaSetSubpictureChromakey(VADisplay dpy, VASubpictureID subpicture,
                                  unsigned int chromakey_min,
                                  unsigned int chromakey_max,
                                  unsigned int chromakey_mask)
{
    init();
    return va.SetSubpictureChromakey(dpy, subpicture, chromakey_min, chromakey_max, chromakey_mask);
}

VAStatus vaSetSubpictureGlobalAlpha(VADisplay dpy, VASubpictureID subpicture,
                                    float global_alpha)
{
    init();
    return va.SetSubpictureGlobalAlpha(dpy, subpicture, global_alpha);
}

VAStatus vaAssociateSubpicture(VADisplay dpy, VASubpictureID subpicture,
                               VASurfaceID *target_surfaces, int num_surfaces,
                               int16_t src_x, int16_t src_y, uint16_t src_width,
                               uint16_t src_height, int16_t dest_x,
                               int16_t dest_y, uint16_t dest_width,
                               uint16_t dest_height,
                               uint32_t flags)
{
    init();
    return va.AssociateSubpicture(dpy, subpicture, target_surfaces, num_surfaces, src_x, src_y, src_width, src_height, dest_x, dest_y, dest_width, dest_height, flags);
}

VAStatus vaDeassociateSubpicture(VADisplay dpy, VASubpictureID subpicture,
                                 VASurfaceID *target_surfaces,
                                 int num_surfaces)
{
    init();
    return va.DeassociateSubpicture(dpy, subpicture, target_surfaces, num_surfaces);
}

int vaMaxNumDisplayAttributes(VADisplay dpy)
{
    init();
    return va.MaxNumDisplayAttributes(dpy);
}

VAStatus vaQueryDisplayAttributes(VADisplay dpy, VADisplayAttribute *attr_list,
                                  int *num_attributes)
{
    init();
    return va.QueryDisplayAttributes(dpy, attr_list, num_attributes);
}

VAStatus vaGetDisplayAttributes(VADisplay dpy, VADisplayAttribute *attr_list,
                                int num_attributes)
{
    init();
    return va.GetDisplayAttributes(dpy, attr_list, num_attributes);
}

VAStatus vaSetDisplayAttributes(VADisplay dpy, VADisplayAttribute *attr_list,
                                int num_attributes)
{
    init();
    return va.SetDisplayAttributes(dpy, attr_list, num_attributes);
}


const char *vaProfileStr(VAProfile profile)
{
    init();
    init();
    if (!va.ProfileStr)
        return "";
    return va.ProfileStr(profile);
}

const char *vaEntrypointStr(VAEntrypoint entrypoint)
{
    init();
    if (!va.EntrypointStr)
        return "";
    return va.EntrypointStr(entrypoint);
}

const char *vaConfigAttribTypeStr(VAConfigAttribType configAttribType)
{
    init();
    if (!va.ConfigAttribTypeStr)
        return "";
    return va.ConfigAttribTypeStr(configAttribType);
}

const char *vaBufferTypeStr(VABufferType bufferType)
{
    init();
    if (!va.BufferTypeStr)
        return "";
    return va.BufferTypeStr(bufferType);
}
/*
const char *vaStatusStr(VAStatus status)
{
    init();
    if (!va.StatusStr)
        return "";
    return va.StatusStr(status);
}*/
} // extern "C"