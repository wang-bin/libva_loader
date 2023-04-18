/*
 * Copyright (c) 2020-2023 WangBin <wbsecg1 at gmail.com>
 */
#include "va_symbols.h"
#include <cstdio>
#if defined(_WIN32)
# ifndef UNICODE
#   define UNICODE 1
# endif
# include <windows.h>
# ifdef WINAPI_FAMILY
#  include <winapifamily.h>
#  if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#    define VA_WINRT 1
#  endif
# endif
# if (VA_WINRT+0)
#   define dlopen(filename, flags) LoadPackagedLibrary(filename, 0)
# else
#   define dlopen(filename, flags) LoadLibrary(filename)
# endif
# define dlsym(handle, symbol) GetProcAddress((HMODULE)handle, symbol)
# define dlclose(handle) FreeLibrary((HMODULE)handle)
//#elif defined(__APPLE__) // uncomment the following lines if use framework
//# define dlopen(filename, flags) load_bundle(filename)
//# define dlsym(handle, symbol) ((handle) ? CFBundleGetFunctionPointerForName(handle, CFSTR(symbol)) : nullptr)
//# define dlclose(handle) CFRelease(handle)
#else
# include <dlfcn.h>
#endif

static void* libva = nullptr;
#if (_WIN32 + 0)
static void* libva_win32 = nullptr;
#else
static void* libva_x11 = nullptr;
static void* libva_drm = nullptr;

_Pragma("weak dlopen") // so no need link to libdl directly, no libva.pc change
_Pragma("weak dlsym") // so no need link to libdl directly, no libva.pc change
#endif

static void load_va()
{
#if (_WIN32 + 0)
    libva = dlopen(TEXT("va.dll"), RTLD_LAZY|RTLD_LOCAL);
    if (!libva) {
        printf("failed to open va.dll\n");
        return;
    }
    libva_win32 = dlopen(TEXT("va_win32.dll"), RTLD_LAZY|RTLD_LOCAL);
#else
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
#endif
}

static va_t va;
#if (_WIN32 + 0)
va_win32_t va_win32;
#else
va_x11_t va_x11;
va_drm_t va_drm;
#endif
//static pthread_once_t init_flag = PTHREAD_ONCE_INIT;
//_Pragma("weak pthread_once") // so no need link to pthread directly, no libva.pc change

#if defined(_MSC_VER)
# define INIT_FUNC(f) \
    static void f(); \
    static const auto f##_v = []() noexcept { f(); return 0;}(); \
    static void f()
#else
# define INIT_FUNC(f) \
    __attribute__((constructor, used)) static void f()
#endif

INIT_FUNC(init)
{
    if (va.Initialize)
        return;
    load_va();
    //pthread_once(&init_flag, load_va);
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

    DLSYM_VA(SyncBuffer);

    DLSYM_VA(QueryVideoProcFilters);
    DLSYM_VA(QueryVideoProcFilterCaps);
    DLSYM_VA(QueryVideoProcPipelineCaps);

#if (_WIN32 + 0)
#undef DLSYM_VA
#define DLSYM_VA(X) va_win32.X = (decltype(&va##X))dlsym(libva_win32, "va" #X)
    DLSYM_VA(GetDisplayWin32);
#else
#undef DLSYM_VA
#define DLSYM_VA(X) va_x11.X = (decltype(&va##X))dlsym(libva_x11, "va" #X)
    DLSYM_VA(GetDisplay);
    DLSYM_VA(PutSurface);
#undef DLSYM_VA
#define DLSYM_VA(X) va_drm.X = (decltype(&va##X))dlsym(libva_drm, "va" #X)
    DLSYM_VA(GetDisplayDRM);
#endif
}

extern "C" {
/* visibility hidden */
const char *vaErrorStr(VAStatus error_status)
{
    return va.ErrorStr(error_status);
}

// MUST init() in most functions if this is a static library
VAMessageCallback vaSetErrorCallback(VADisplay dpy, VAMessageCallback callback, void *user_context)
{
    if (!va.SetErrorCallback)
        return nullptr;
    return va.SetErrorCallback(dpy, callback, user_context);
}

VAMessageCallback vaSetInfoCallback(VADisplay dpy, VAMessageCallback callback, void *user_context)
{
    if (!va.SetInfoCallback)
        return nullptr;
    return va.SetInfoCallback(dpy, callback, user_context);
}

int vaDisplayIsValid(VADisplay dpy)
{
    return va.DisplayIsValid(dpy);
}

VAStatus vaSetDriverName(VADisplay dpy, char *driver_name)
{
    if (!va.SetDriverName)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.SetDriverName(dpy, driver_name);
}

VAStatus vaInitialize(VADisplay dpy, int *major_version, int *minor_version)
{
    return va.Initialize(dpy, major_version, minor_version);
}

VAStatus vaTerminate(VADisplay dpy)
{
    return va.Terminate(dpy);
}

const char *vaQueryVendorString(VADisplay dpy)
{
    return va.QueryVendorString(dpy);
}

VAPrivFunc vaGetLibFunc(VADisplay dpy, const char *func)
{
    return va.GetLibFunc(dpy, func);
}

int vaMaxNumProfiles(VADisplay dpy)
{
    return va.MaxNumProfiles(dpy);
}

int vaMaxNumEntrypoints(VADisplay dpy)
{
    return va.MaxNumEntrypoints(dpy);
}

int vaMaxNumConfigAttributes(VADisplay dpy)
{
    return va.MaxNumConfigAttributes(dpy);
}

VAStatus vaQueryConfigProfiles(VADisplay dpy, VAProfile *profile_list, int *num_profiles)
{
    return va.QueryConfigProfiles(dpy, profile_list, num_profiles);
}

VAStatus vaQueryConfigEntrypoints(VADisplay dpy, VAProfile profile,
                                  VAEntrypoint *entrypoint_list,
                                  int *num_entrypoints)
{
    return va.QueryConfigEntrypoints(dpy, profile, entrypoint_list, num_entrypoints);
}

VAStatus vaGetConfigAttributes(VADisplay dpy, VAProfile profile,
                               VAEntrypoint entrypoint,
                               VAConfigAttrib *attrib_list, int num_attribs)
{
    if (!va.GetConfigAttributes)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.GetConfigAttributes(dpy, profile, entrypoint, attrib_list, num_attribs);
}

VAStatus vaCreateConfig(VADisplay dpy, VAProfile profile,
                        VAEntrypoint entrypoint, VAConfigAttrib *attrib_list,
                        int num_attribs, VAConfigID *config_id)
{
    return va.CreateConfig(dpy, profile, entrypoint, attrib_list, num_attribs, config_id);
}

VAStatus vaDestroyConfig(VADisplay dpy, VAConfigID config_id)
{
    return va.DestroyConfig(dpy, config_id);
}

VAStatus vaQueryConfigAttributes(VADisplay dpy, VAConfigID config_id,
                                 VAProfile *profile, VAEntrypoint *entrypoint,
                                 VAConfigAttrib *attrib_list, int *num_attribs)
{
    return va.QueryConfigAttributes(dpy, config_id, profile, entrypoint, attrib_list, num_attribs);
}

VAStatus vaQuerySurfaceAttributes(VADisplay dpy, VAConfigID config,
                                  VASurfaceAttrib *attrib_list,
                                  unsigned int *num_attribs)
{
    return va.QuerySurfaceAttributes(dpy, config, attrib_list, num_attribs);
}

VAStatus vaCreateSurfaces(VADisplay dpy, unsigned int format,
                          unsigned int width, unsigned int height,
                          VASurfaceID *surfaces, unsigned int num_surfaces,
                          VASurfaceAttrib *attrib_list,
                          unsigned int num_attribs)
{
    return va.CreateSurfaces(dpy, format, width, height, surfaces, num_surfaces, attrib_list, num_surfaces);
}

VAStatus vaDestroySurfaces(VADisplay dpy, VASurfaceID *surfaces,
                           int num_surfaces)
{
    return va.DestroySurfaces(dpy, surfaces, num_surfaces);
}

VAStatus vaCreateContext(VADisplay dpy, VAConfigID config_id, int picture_width,
                         int picture_height, int flag,
                         VASurfaceID *render_targets, int num_render_targets,
                         VAContextID *context)
{
    return va.CreateContext(dpy, config_id, picture_width, picture_height, flag, render_targets, num_render_targets, context);
}

VAStatus vaDestroyContext(VADisplay dpy, VAContextID context)
{
    return va.DestroyContext(dpy, context);
}

VAStatus vaCreateMFContext(VADisplay dpy, VAMFContextID *mf_context)
{
    return va.CreateMFContext(dpy, mf_context);
}

VAStatus vaMFAddContext(VADisplay dpy, VAMFContextID mf_context,
                        VAContextID context)
{
    return va.MFAddContext(dpy, mf_context, context);
}

VAStatus vaMFReleaseContext(VADisplay dpy, VAMFContextID mf_context,
                            VAContextID context)
{
    return va.MFReleaseContext(dpy, mf_context, context);
}

VAStatus vaQueryProcessingRate(VADisplay dpy, VAConfigID config,
                               VAProcessingRateParameter *proc_buf,
                               unsigned int *processing_rate)
{
    return va.QueryProcessingRate(dpy, config, proc_buf, processing_rate);
}

VAStatus vaCreateBuffer(VADisplay dpy, VAContextID context, VABufferType type,
                        unsigned int size, unsigned int num_elements,
                        void *data, VABufferID *buf_id)
{
    return va.CreateBuffer(dpy, context, type, size, num_elements, data, buf_id);
}

VAStatus vaCreateBuffer2(VADisplay dpy, VAContextID context, VABufferType type,
                         unsigned int width, unsigned int height,
                         unsigned int *unit_size, unsigned int *pitch,
                         VABufferID *buf_id)
{
    return va.CreateBuffer2(dpy, context, type, width, height, unit_size, pitch, buf_id);
}

VAStatus vaBufferSetNumElements(VADisplay dpy, VABufferID buf_id,
                                unsigned int num_elements)
{
    return va.BufferSetNumElements(dpy, buf_id, num_elements);
}

VAStatus vaMapBuffer(VADisplay dpy, VABufferID buf_id, void **pbuf)
{
    return va.MapBuffer(dpy, buf_id, pbuf);
}

VAStatus vaUnmapBuffer(VADisplay dpy, VABufferID buf_id)
{
    return va.UnmapBuffer(dpy, buf_id);
}

VAStatus vaDestroyBuffer(VADisplay dpy, VABufferID buffer_id)
{
    return va.DestroyBuffer(dpy, buffer_id);
}

VAStatus vaAcquireBufferHandle(VADisplay dpy, VABufferID buf_id,
                               VABufferInfo *buf_info)
{
    if (!va.AcquireBufferHandle)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.AcquireBufferHandle(dpy, buf_id, buf_info);
}

VAStatus vaReleaseBufferHandle(VADisplay dpy, VABufferID buf_id)
{
    if (!va.ReleaseBufferHandle)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.ReleaseBufferHandle(dpy, buf_id);
}

VAStatus vaExportSurfaceHandle(VADisplay dpy, VASurfaceID surface_id,
                               uint32_t mem_type, uint32_t flags,
                               void *descriptor)
{
    if (!va.ExportSurfaceHandle)
        return VA_STATUS_ERROR_UNKNOWN;
    return va.ExportSurfaceHandle(dpy, surface_id, mem_type, flags, descriptor);
}

VAStatus vaBeginPicture(VADisplay dpy, VAContextID context,
                        VASurfaceID render_target)
{
    return va.BeginPicture(dpy, context, render_target);
}

VAStatus vaRenderPicture(VADisplay dpy, VAContextID context,
                         VABufferID *buffers, int num_buffers)
{
    return va.RenderPicture(dpy, context, buffers, num_buffers);
}

VAStatus vaEndPicture(VADisplay dpy, VAContextID context)
{
    return va.EndPicture(dpy, context);
}

VAStatus vaMFSubmit(VADisplay dpy, VAMFContextID mf_context,
                    VAContextID *contexts, int num_contexts)
{
    return va.MFSubmit(dpy, mf_context, contexts, num_contexts);
}

VAStatus vaSyncSurface(VADisplay dpy, VASurfaceID render_target)
{
    return va.SyncSurface(dpy, render_target);
}

VAStatus vaQuerySurfaceStatus(VADisplay dpy, VASurfaceID render_target,
                              VASurfaceStatus *status)
{
    return va.QuerySurfaceStatus(dpy, render_target, status);
}

VAStatus vaQuerySurfaceError(VADisplay dpy, VASurfaceID surface,
                             VAStatus error_status, void **error_info)
{
    return va.QuerySurfaceError(dpy, surface, error_status, error_info);
}

int vaMaxNumImageFormats(VADisplay dpy)
{
    return va.MaxNumImageFormats(dpy);
}

VAStatus vaQueryImageFormats(VADisplay dpy, VAImageFormat *format_list,
                             int *num_formats)
{
    return va.QueryImageFormats(dpy, format_list, num_formats);
}

VAStatus vaCreateImage(VADisplay dpy, VAImageFormat *format, int width,
                       int height, VAImage *image)
{
    return va.CreateImage(dpy, format, width, height, image);
}

VAStatus vaDestroyImage(VADisplay dpy, VAImageID image)
{
    return va.DestroyImage(dpy, image);
}

VAStatus vaSetImagePalette(VADisplay dpy, VAImageID image,
                           unsigned char *palette)
{
    return va.SetImagePalette(dpy, image, palette);
}

VAStatus vaGetImage(VADisplay dpy, VASurfaceID surface, int x, int y,
                    unsigned int width, unsigned int height, VAImageID image)
{
    return va.GetImage(dpy, surface, x, y, width, height, image);
}

VAStatus vaPutImage(VADisplay dpy, VASurfaceID surface, VAImageID image,
                    int src_x, int src_y, unsigned int src_width,
                    unsigned int src_height, int dest_x, int dest_y,
                    unsigned int dest_width, unsigned int dest_height)
{
    return va.PutImage(dpy, surface, image, src_x, src_y, src_width, src_height, dest_x, dest_y, dest_width, dest_height);
}

VAStatus vaDeriveImage(VADisplay dpy, VASurfaceID surface, VAImage *image)
{
    return va.DeriveImage(dpy, surface, image);
}

int vaMaxNumSubpictureFormats(VADisplay dpy)
{
    return va.MaxNumSubpictureFormats(dpy);
}

VAStatus vaQuerySubpictureFormats(VADisplay dpy, VAImageFormat *format_list,
                                  unsigned int *flags,
                                  unsigned int *num_formats)
{
    return va.QuerySubpictureFormats(dpy, format_list, flags, num_formats);
}

VAStatus vaCreateSubpicture(VADisplay dpy, VAImageID image,
                            VASubpictureID *subpicture)
{
    return va.CreateSubpicture(dpy, image, subpicture);
}

VAStatus vaDestroySubpicture(VADisplay dpy, VASubpictureID subpicture)
{
    return va.DestroySubpicture(dpy, subpicture);
}

VAStatus vaSetSubpictureImage(VADisplay dpy, VASubpictureID subpicture,
                              VAImageID image)
{
    return va.SetSubpictureImage(dpy, subpicture, image);
}

VAStatus vaSetSubpictureChromakey(VADisplay dpy, VASubpictureID subpicture,
                                  unsigned int chromakey_min,
                                  unsigned int chromakey_max,
                                  unsigned int chromakey_mask)
{
    return va.SetSubpictureChromakey(dpy, subpicture, chromakey_min, chromakey_max, chromakey_mask);
}

VAStatus vaSetSubpictureGlobalAlpha(VADisplay dpy, VASubpictureID subpicture,
                                    float global_alpha)
{
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
    return va.AssociateSubpicture(dpy, subpicture, target_surfaces, num_surfaces, src_x, src_y, src_width, src_height, dest_x, dest_y, dest_width, dest_height, flags);
}

VAStatus vaDeassociateSubpicture(VADisplay dpy, VASubpictureID subpicture,
                                 VASurfaceID *target_surfaces,
                                 int num_surfaces)
{
    return va.DeassociateSubpicture(dpy, subpicture, target_surfaces, num_surfaces);
}

int vaMaxNumDisplayAttributes(VADisplay dpy)
{
    return va.MaxNumDisplayAttributes(dpy);
}

VAStatus vaQueryDisplayAttributes(VADisplay dpy, VADisplayAttribute *attr_list,
                                  int *num_attributes)
{
    return va.QueryDisplayAttributes(dpy, attr_list, num_attributes);
}

VAStatus vaGetDisplayAttributes(VADisplay dpy, VADisplayAttribute *attr_list,
                                int num_attributes)
{
    return va.GetDisplayAttributes(dpy, attr_list, num_attributes);
}

VAStatus vaSetDisplayAttributes(VADisplay dpy, VADisplayAttribute *attr_list,
                                int num_attributes)
{
    return va.SetDisplayAttributes(dpy, attr_list, num_attributes);
}


const char *vaProfileStr(VAProfile profile)
{
    if (!va.ProfileStr)
        return "";
    return va.ProfileStr(profile);
}

const char *vaEntrypointStr(VAEntrypoint entrypoint)
{
    if (!va.EntrypointStr)
        return "";
    return va.EntrypointStr(entrypoint);
}

const char *vaConfigAttribTypeStr(VAConfigAttribType configAttribType)
{
    if (!va.ConfigAttribTypeStr)
        return "";
    return va.ConfigAttribTypeStr(configAttribType);
}

const char *vaBufferTypeStr(VABufferType bufferType)
{
    if (!va.BufferTypeStr)
        return "";
    return va.BufferTypeStr(bufferType);
}
/*
const char *vaStatusStr(VAStatus status)
{
    if (!va.StatusStr)
        return "";
    return va.StatusStr(status);
}*/

#if VA_CHECK_VERSION(1, 9, 0)
VAStatus vaSyncBuffer(VADisplay dpy, VABufferID buf_id, uint64_t timeout_ns)
{
    if (!va.SyncBuffer)
        return VA_STATUS_ERROR_UNIMPLEMENTED;
    return va.SyncBuffer(dpy, buf_id, timeout_ns);
}
#endif

VAStatus vaQueryVideoProcFilters(VADisplay dpy, VAContextID context, VAProcFilterType *filters, unsigned int *num_filters)
{
    return va.QueryVideoProcFilters(dpy, context, filters, num_filters);
}

VAStatus vaQueryVideoProcFilterCaps(VADisplay dpy, VAContextID context, VAProcFilterType type, void *filter_caps, unsigned int *num_filter_caps)
{
    return va.QueryVideoProcFilterCaps(dpy, context, type, filter_caps, num_filter_caps);
}

VAStatus vaQueryVideoProcPipelineCaps(VADisplay dpy, VAContextID context, VABufferID *filters, unsigned int num_filters, VAProcPipelineCaps *pipeline_caps)
{
    return va.QueryVideoProcPipelineCaps(dpy, context, filters, num_filters, pipeline_caps);
}
} // extern "C"