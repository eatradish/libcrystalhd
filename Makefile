#
#  Broadcom "BCM970012/BCM970015" Crystal HD device interface library.
#
#
BCLIB_MINOR=6
BCLIB_MAJOR=3
BCLIB_NAME=libcrystalhd.so
BCLIB_SL=$(BCLIB_NAME).$(BCLIB_MAJOR)
BCLIB=$(BCLIB_NAME).$(BCLIB_MAJOR).$(BCLIB_MINOR)
LIBDIR=/usr/lib

AT   = @
ECHO = ${AT} echo
BCGCC = g++


INCLUDES = -I./ -I/usr/include
INCLUDES += -I/usr/include/link


CPPFLAGS = -D__LINUX_USER__
# -DLDIL_PRINTS_ON
# -D_USE_SHMEM_

CPPFLAGS += ${INCLUDES}
CPPFLAGS += -O2 -Wall -fPIC -shared -fstrict-aliasing -msse2
LDFLAGS = -Wl,-soname,${BCLIB_SL} -pthread

SRCFILES = 	libcrystalhd_if.cpp \
		libcrystalhd_int_if.cpp \
		libcrystalhd_fwcmds.cpp \
		libcrystalhd_priv.cpp \
		libcrystalhd_fwdiag_if.cpp \
		libcrystalhd_fwload_if.cpp \
		libcrystalhd_parser.cpp

OBJFILES = ${SRCFILES:.cpp=.o}

all:help $(OBJFILES)
	$(BCGCC) $(CPPFLAGS) $(LDFLAGS) -o $(BCLIB) ${OBJFILES}
	ln -sf $(BCLIB) $(BCLIB_NAME)
	ln -sf $(BCLIB) $(BCLIB_SL)

help:
	${ECHO} OBJFILES = ${OBJFILES}
	${ECHO} SRCFILES = ${SRCFILES}
	${ECHO} LNM = ${BCLIB} ${BCLIB_SL}

clean:
	rm -f  ${BCLIB} ${BCLIB_SL} ${BCLIB_NAME} *.o
	rm -f  ${OBJFILES}

install:
	mkdir -p $(DESTDIR)$(LIBDIR)
	mkdir -p $(DESTDIR)/lib/firmware
	mkdir -p $(DESTDIR)/usr/include/libcrystalhd
	cp libcrystalhd_if.h $(DESTDIR)/usr/include/libcrystalhd/
	chmod 0644 $(DESTDIR)/usr/include/libcrystalhd/libcrystalhd_if.h
	cp include/bc_dts_defs.h $(DESTDIR)/usr/include/libcrystalhd/
	chmod 0644 $(DESTDIR)/usr/include/libcrystalhd/bc_dts_defs.h
	cp include/bc_dts_types.h $(DESTDIR)/usr/include/libcrystalhd/
	chmod 0644 $(DESTDIR)/usr/include/libcrystalhd/bc_dts_types.h
	cp include/libcrystalhd_version.h $(DESTDIR)/usr/include/libcrystalhd/
	chmod 0644 $(DESTDIR)/usr/include/libcrystalhd/libcrystalhd_version.h
	install -m 755 $(BCLIB) $(DESTDIR)$(LIBDIR)
	(cd $(DESTDIR)$(LIBDIR); ln -sf $(BCLIB) $(BCLIB_NAME))
	(cd $(DESTDIR)$(LIBDIR); ln -sf $(BCLIB) $(BCLIB_SL))

