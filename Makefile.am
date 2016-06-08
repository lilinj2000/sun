AUTOMAKE_OPTIONS = foreign

SUBDIRS = traderflow

bin_PROGRAMS = sun
sun_SOURCES = \
	SunConfig.cc \
	SunServer.cc \
	Main.cc

sun_CPPFLAGS = -I$(top_srcdir) \
		$(BOOST_CPPFLAGS) \
		$(SOIL_CPPFLAGS)


sun_LDADD = \
	$(BOOST_LIBADD) \
	$(FEMA_LIBADD) \
	$(SOIL_LIBADD) \
	../service/libfema.la

sun_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = SunConfig.hh \
		SunServer.hh \
		SunLog.hh

EXTRA_DIST = env.sh sun.cfg log.cfg

sundir = $(bindir)
sun_DATA = env.sh sun.cfg log.cfg
