#
SYSROOT := /
TRIPLE := x86_64-linux-gnu
LLVM_VER := 10
CC := clang-$(LLVM_VER)
AR := llvm-ar-$(LLVM_VER)
RANLIB := llvm-ranlib-$(LLVM_VER)

STATIC_CFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden
CFLAGS=--target=$(TRIPLE) --sysroot=$(SYSROOT) -O2 -fPIC $(STATIC_CFLAGS) -Wformat -Werror=format-security -fstack-protector-strong -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=2 -ffunction-sections -fdata-sections
LFLAGS=-fuse-ld=lld --target=$(TRIPLE) --sysroot=$(SYSROOT) -O2 -fPIC -Wl,--gc-sections

VA_OBJS = va.o
VA_X11_OBJS = va_x11.o
VA_DRM_OBJS = va_drm.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $^

all: libva.a libva-x11.a libva-drm.a #libva.so

libva.a: $(VA_OBJS)
	$(AR) -rc $@ $^
	$(RANLIB) $@

libva-x11.a: $(VA_X11_OBJS)
	$(AR) -rc $@ $^
	$(RANLIB) $@

libva-drm.a: $(VA_DRM_OBJS)
	$(AR) -rc $@ $^
	$(RANLIB) $@

libva.so: $(VA_OBJS) $(VA_X11_OBJS) $(VA_DRM_OBJS)
	$(CC) -shared $(LFLAGS) -o $@ $^

clean:
	rm -f $(VA_OBJS) $(VA_X11_OBJS) $(VA_DRM_OBJS) libva.a libva-x11.a libva-drm.a #libva.so
