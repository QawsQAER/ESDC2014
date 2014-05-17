AC_PREREQ(2.61)

# package version number (not shared library version)
# odd micro numbers indicate in-progress development
# even micro numbers indicate released versions
m4_define(helloworld_by_eclipse_version_major, 0)
m4_define(helloworld_by_eclipse_version_minor, 1)
m4_define(helloworld_by_eclipse_version_micro, 0)

m4_define([helloworld_by_eclipse_version],
          [helloworld_by_eclipse_version_major.helloworld_by_eclipse_version_minor.helloworld_by_eclipse_version_micro])
m4_define([helloworld_by_eclipse_api_version],
          [helloworld_by_eclipse_version_major.helloworld_by_eclipse_version_minor])

AC_INIT(helloworld_by_eclipse, helloworld_by_eclipse_version)
AM_INIT_AUTOMAKE(helloworld_by_eclipse, helloworld_by_eclipse_version)
AC_CONFIG_HEADERS(config.h)

# Checks for programs.
AC_PROG_LIBTOOL
AC_PROG_CXX
AC_PROG_INSTALL

# enable pkg-config
PKG_PROG_PKG_CONFIG

# Checks for library functions.
#AC_CHECK_FUNCS([memset])

AC_SUBST(helloworld_by_eclipse_CFLAGS)
AC_SUBST(helloworld_by_eclipse_LIBS)


#icondir=${datadir}/icons/hicolor/32x32/apps
#AC_SUBST(icondir)

AC_OUTPUT([
Makefile
src/Makefile
])

echo ""
echo "   helloworld_by_eclipse $VERSION"
echo "   ====================="
echo ""
echo "   To build the project, run \"make\""
echo ""
