AUTOMAKE_OPTIONS = foreign

SUBDIRS = traderflow

bin_PROGRAMS = sun
sun_SOURCES = \
	Options.cc \
	Server.cc \
	Main.cc

sun_CPPFLAGS = -I$(top_srcdir) \
		$(BOOST_CPPFLAGS) \
		$(RAPIDJSON_CPPFLAGS) \
		$(FMT_CPPFLAGS) \
		$(SPDLOG_CPPFLAGS) \
		$(SOIL_CPPFLAGS) \
		$(AIR_CPPFLAGS) \
		$(FOAL_CPPFLAGS)

sun_LDADD = \
	$(BOOST_LIBADD) \
	$(FOAL_LIBADD) \
	$(FMT_LIBADD) \
	$(AIR_LIBADD) \
	$(SOIL_LIBADD)

sun_LDFLAGS = $(LIBADD_DL) -Wl,--no-as-needed -pthread

# header files
noinst_HEADERS = Options.hh

# SunServer.hh

EXTRA_DIST = env.sh sun.json

sundir = $(bindir)
sun_DATA = env.sh sun.json
