#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT(sun, 0.1, lilinj2000@gmail.com)
AC_CONFIG_SRCDIR([])
#AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
LT_INIT
AC_PROG_RANLIB
# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
#AC_CHECK_HEADER_STDBOOL
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

# Checks for library functions.
LT_RELEASE=0.1
LT_CURRENT=0
LT_REVISION=1
LT_AGE=0
AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

node=`uname -n`
home_3rd=~/3rd/${node}
home_libs=~/libs/${node}

dnl boost lib
AC_ARG_WITH([boost],
    [AS_HELP_STRING([--with-boost],
        [home path of the boost, defaults to $home_3rd/boost])],
    [BOOST_HOME="$withval"],
    [BOOST_HOME="$home_3rd/boost"])
AC_SUBST([BOOST_HOME])

BOOST_CPPFLAGS="-I$BOOST_HOME/include -DBOOST_ALL_DYN_LINK"
BOOST_LIBADD=" \
              $BOOST_HOME/lib/libboost_filesystem.so \
  	      $BOOST_HOME/lib/libboost_regex.so \
	      $BOOST_HOME/lib/libboost_system.so"
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LIBADD)

dnl rapidjson lib
RAPIDJSON_HOME=$home_3rd/rapidjson
AC_ARG_WITH([rapidjson], 
    [AS_HELP_STRING([--with-rapidjson],
        [home path of the rapidjson, defaults to $RAPIDJSON_HOME])],
    [RAPIDJSON_HOME="$withval"],
    [RAPIDJSON_HOME="$RAPIDJSON_HOME"])
AC_SUBST([RAPIDJSON_HOME])

RAPIDJSON_CPPFLAGS="-I$RAPIDJSON_HOME/include -DRAPIDJSON_HAS_STDSTRING"
AC_SUBST(RAPIDJSON_CPPFLAGS)

dnl fmt lib
FMT_HOME=$home_3rd/fmt
AC_ARG_WITH([fmt], 
    [AS_HELP_STRING([--with-fmt],
        [home path of the fmt, defaults to $FMT_HOME])],
    [FMT_HOME="$withval"],
    [FMT_HOME="$FMT_HOME"])
AC_SUBST([FMT_HOME])

FMT_CPPFLAGS="-I$FMT_HOME/include"
FMT_LIBADD="-L$FMT_HOME/lib -lfmt"
AC_SUBST(FMT_CPPFLAGS)
AC_SUBST(FMT_LIBADD)

dnl spdlog lib
SPDLOG_HOME=$home_3rd/spdlog
AC_ARG_WITH([spdlog], 
    [AS_HELP_STRING([--with-spdlog],
        [home path of the spdlog, defaults to $SPDLOG_HOME])],
    [SPDLOG_HOME="$withval"],
    [SPDLOG_HOME="$SPDLOG_HOME"])
AC_SUBST([SPDLOG_HOME])

SPDLOG_CPPFLAGS="-I$SPDLOG_HOME/include -DSPDLOG_FMT_EXTERNAL"
AC_SUBST(SPDLOG_CPPFLAGS)

dnl readerwriterqueue lib
READERWRITERQUEUE_HOME=$home_3rd/readerwriterqueue
AC_ARG_WITH([readerwriterqueue], 
    [AS_HELP_STRING([--with-readerwriterqueue],
        [home path of the readerwriterqueue, defaults to $READERWRITERQUEUE_HOME])],
    [READERWRITERQUEUE_HOME="$withval"],
    [READERWRITERQUEUE_HOME="$READERWRITERQUEUE_HOME"])
AC_SUBST([READERWRITERQUEUE_HOME])

READERWRITERQUEUE_CPPFLAGS="-I$READERWRITERQUEUE_HOME/include"
AC_SUBST(READERWRITERQUEUE_CPPFLAGS)

dnl args lib
ARGS_HOME=$home_3rd/args
AC_ARG_WITH([args], 
    [AS_HELP_STRING([--with-args],
        [home path of the args, defaults to $ARGS_HOME])],
    [ARGS_HOME="$withval"],
    [ARGS_HOME="$ARGS_HOME"])
AC_SUBST([ARGS_HOME])
ARGS_CPPFLAGS="-I$ARGS_HOME/include"
AC_SUBST(ARGS_CPPFLAGS)

dnl soil lib
AC_ARG_WITH([soil],
    [AS_HELP_STRING([--with-soil],
        [home path of the soil, defaults to $home_libs/soil])],
    [SOIL_HOME="$withval"],
    [SOIL_HOME="$home_libs/soil"])
AC_SUBST([SOIL_HOME])

SOIL_CPPFLAGS="-I$SOIL_HOME/include"
SOIL_LIBADD="$SOIL_HOME/lib/libsoil.la"
AC_SUBST(SOIL_CPPFLAGS)
AC_SUBST(SOIL_LIBADD)

dnl foal lib
FOAL_HOME=$home_libs/foal
AC_ARG_WITH([foal], 
    [AS_HELP_STRING([--with-foal],
        [home path of the foal, defaults to $FOAL_HOME])],
    [FOAL_HOME="$withval"],
    [FOAL_HOME="$FOAL_HOME"])
AC_SUBST([FOAL_HOME])

FOAL_CPPFLAGS="-I$FOAL_HOME/include"
FOAL_LIBADD="$FOAL_HOME/lib/libfoal.la"
AC_SUBST(FOAL_CPPFLAGS)
AC_SUBST(FOAL_LIBADD)

dnl air lib
AC_ARG_WITH([air],
    [AS_HELP_STRING([--with-air],
        [home path of the air, defaults to $home_libs/air])],
    [AIR_HOME="$withval"],
    [AIR_HOME="$home_libs/air"])
AC_SUBST([AIR_HOME])

AIR_CPPFLAGS="-I$AIR_HOME/include"
AIR_LIBADD="$AIR_HOME/lib/libair.la"
AC_SUBST(AIR_CPPFLAGS)
AC_SUBST(AIR_LIBADD)

AC_OUTPUT([
	Makefile
	src/Makefile])





