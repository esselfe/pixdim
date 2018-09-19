
CFLAGS = -std=c11 -Wall -Werror -O0 -g -D_GNU_SOURCE
LDFLAGS = -lm -lz -lmagic -ljpeg -lpng
OBJDIR = obj

# executable symbols
OBJS = $(OBJDIR)/jpg.o $(OBJDIR)/png.o $(OBJDIR)/stat.o $(OBJDIR)/pixdim.o
PROGOBJS = $(OBJDIR)/main.o

# shared lib symbols
SHAREDOBJS = $(OBJDIR)/jpg.lo $(OBJDIR)/png.lo $(OBJDIR)/stat.lo $(OBJDIR)/pixdim.lo

PROGNAME ?= pixdim
PROGNAME_STATIC ?= pixdim.stx
PIXDIM_VERSION=$(shell grep "pixdim_version.*=" pixdim.c |awk '{ print $$6 }' |sed 's/"//g;s/;//')
SHAREDLIBNAME = libpixdim.so
STATICLIBNAME = libpixdim.a

PREFIX ?= /usr/local
HEADER_INSTALLDIR ?= $(PREFIX)/include
LIB_INSTALLDIR ?= $(PREFIX)/lib
PROG_INSTALLDIR ?= $(PREFIX)/bin

.PHONY: default all prepare install install_header install_lib \
uninstall uninstall_header uninstall_lib uninstall_prog clean

default: all
	@ls -li --color=auto $(PROGNAME) $(PROGNAME_STATIC) 2>/dev/null || true

#all: prepare $(OBJS) $(PROGOBJS) $(SHAREDOBJS) $(SHAREDLIBNAME) $(STATICLIBNAME) $(PROGNAME) $(PROGNAME_STATIC)
all: prepare $(OBJS) $(PROGOBJS) $(SHAREDOBJS) $(SHAREDLIBNAME) $(STATICLIBNAME) $(PROGNAME)

prepare:
	@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)

# program object files
$(OBJDIR)/jpg.o: jpg.c
	gcc -c $(CFLAGS) jpg.c -o $(OBJDIR)/jpg.o

$(OBJDIR)/png.o: png.c
	gcc -c $(CFLAGS) png.c -o $(OBJDIR)/png.o

$(OBJDIR)/stat.o: stat.c
	gcc -c $(CFLAGS) stat.c -o $(OBJDIR)/stat.o

$(OBJDIR)/pixdim.o: pixdim.c pixdim.h
	gcc -c $(CFLAGS) pixdim.c -o $(OBJDIR)/pixdim.o

$(OBJDIR)/main.o: main.c jpg.c png.c pixdim.h
	gcc -c $(CFLAGS) main.c -o $(OBJDIR)/main.o

# Shared lib's object files
$(OBJDIR)/jpg.lo: jpg.c
	gcc -c $(CFLAGS) -fPIC jpg.c -o $(OBJDIR)/jpg.lo

$(OBJDIR)/png.lo: png.c
	gcc -c $(CFLAGS) -fPIC png.c -o $(OBJDIR)/png.lo

$(OBJDIR)/stat.lo: stat.c
	gcc -c $(CFLAGS) -fPIC stat.c -o $(OBJDIR)/stat.lo

$(OBJDIR)/pixdim.lo: pixdim.c pixdim.h
	gcc -c $(CFLAGS) -fPIC pixdim.c -o $(OBJDIR)/pixdim.lo

$(SHAREDLIBNAME): $(SHAREDOBJS)
	gcc -shared -fPIC $(LDFLAGS) $(SHAREDOBJS) -o $(SHAREDLIBNAME)

$(STATICLIBNAME): $(OBJS)
	ar cr $(STATICLIBNAME) $(OBJS)
	ranlib $(STATICLIBNAME)

$(PROGNAME): $(OBJS) $(PROGOBJS)
	gcc -L/usr/lib/x86_64-linux-gnu $(LDFLAGS) $(OBJS) $(PROGOBJS) -o $(PROGNAME)
#	@echo "$(LIB_INSTALLDIR)/$(SHAREDLIBNAME)"
#	if [ -e $(LIB_INSTALLDIR)/$(SHAREDLIBNAME) ]; then \
#		ls -li $(LIB_INSTALLDIR)/$(SHAREDLIBNAME); \
#		gcc $(LDFLAGS) -lpixdim $(PROGOBJS) -o $(PROGNAME); \
#	else \
#		gcc $(LDFLAGS) $(OBJS) $(PROGOBJS) -o $(PROGNAME); \
#	fi;

$(PROGNAME_STATIC): pixdim.h pixdim.c jpg.c png.c stat.c main.c
	gcc -std=c11 -Wall -Werror -g -O2 -static \
	pixdim.c jpg.c png.c stat.c main.c \
	-ljpeg -lpng -lmagic -lz -lm \
	-o $(PROGNAME_STATIC)

install: install_header install_lib install_prog

install_header:
	@cp -v pixdim.h $(HEADER_INSTALLDIR)

install_lib:
	@cp -v $(SHAREDLIBNAME) $(STATICLIBNAME) $(LIB_INSTALLDIR)
	ldconfig

install_prog:
	@cp -v $(PROGNAME) $(PROG_INSTALLDIR)

uninstall: uninstall_header uninstall_lib uninstall_prog

uninstall_header:
	@rm -v $(HEADER_INSTALLDIR)/pixdim.h || true

uninstall_lib:
	@rm -v $(LIB_INSTALLDIR)/{$(SHAREDLIBNAME),$(STATICLIBNAME)} || true

uninstall_prog:
	@rm -v $(PROG_INSTALLDIR)/$(PROGNAME) || true

clean:
	@rm -rv $(OBJDIR) $(SHAREDLIBNAME) $(STATICLIBNAME) $(PROGNAME) $(PROGNAME_STATIC) 2>/dev/null || true

