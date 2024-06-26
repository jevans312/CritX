AC_PREREQ([2.71])
AC_INIT([critterding],[0.0.312])
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS(config.h)

#AC_LANG_CPP
AC_PROG_CXX
AC_PROG_CPP

#LT_INIT
LT_INIT

AC_MSG_CHECKING([host type])
case "$host" in
	*-apple-darwin*)
		HOST_LIBS="-lz"
		HOST_GL_LIBS="-framework OpenGL"
		HOST_SDL_LIBS="-framework SDL -framework Cocoa"
		SDLMAIN="SDLMain.o"
		OS=OSX
                AC_MSG_RESULT(OS X)
		AC_DEFINE_UNQUOTED(_OSX, 1, [Define this if you're building for native OS X])
     		m4_ifdef(
			[AC_PROG_OBJC],
       			[AC_PROG_OBJC],
			[AC_CHECK_TOOL([OBJC], [gcc])
			AC_SUBST(OBJC)
			AC_SUBST(OBJCFLAGS)]
		)
		;;

        *-*-mingw*)
                HOST_LIBS="-lmingw32 -lopengl32 -lGL -lGLU -lGLEW -lfreetype -lSDLmain -lSDL2 -mwindows"
		SDLMAIN=""
		OS=WIN32
                AC_MSG_RESULT(win32)
                ;;
        *)
                HOST_LIBS="-lGL -lGLU -lGLEW -lSDL2"
		SDLMAIN=""
                AC_MSG_RESULT(generic unix)
		OS=UNIX
                ;;
esac

AM_CONDITIONAL([am__fastdepOBJC], false)
AC_SUBST(OS)
AC_SUBST(HOST_LIBS)
AC_SUBST(SDLMAIN)

# SYSTEM FTGL / COMPILE IN FTGL
	AC_SUBST(FTGL_LOCALDIR)
	AC_SUBST(FTGL_LA)
	FTGL_LOCALDIR=""
	FTGL_LA=""

	# have_ftgl
	PKG_PROG_PKG_CONFIG
	PKG_CHECK_MODULES(FTGL, ftgl >= 2.1.3, [FTGL="yes"], [FTGL="no"])

	AS_IF([test "x$FTGL" == "xyes"], [
		# --enable-system-ftgl
		AC_MSG_CHECKING(--enable-system-ftgl)
		AC_ARG_ENABLE(system_ftgl,
			[  --disable-system-ftgl         Do not build against system ftgl.],
			[enable_system_ftgl=$enableval],
			[enable_system_ftgl="yes"]
		)
		AC_MSG_RESULT($enable_system_ftgl)
	])

# POST CHECKS
	# was system ftgl requested
	if test "${enable_system_ftgl}" = "yes"; then
		if test "${FTGL}" = "yes"; then
			CPPFLAGS="$CPPFLAGS $FTGL_CFLAGS"
			HOST_LIBS="${HOST_LIBS} -lftgl"
		else 
			if test "${FTGL}" = "no"; then
				# have Freetype2
				AC_MSG_CHECKING(for Freetype)
				AC_CHECK_PROG([FREETYPE_CONFIG],[freetype-config],yes,no)
				if test "$FREETYPE_CONFIG" = yes; then
					FT2_CFLAGS=`freetype-config --cflags`
					FT2_LIBS=`freetype-config --libs`
					AC_MSG_RESULT($FT2_CFLAGS)
					CPPFLAGS="$CPPFLAGS $FT2_CFLAGS"
					HOST_LIBS="$HOST_LIBS $FT2_LIBS"
					FTGL_LOCALDIR="ftgl"
					FTGL_LA="\$(top_builddir)/src/utils/ftgl/libftgl.la"
				else
					AC_MSG_ERROR([Could not find freetype-config.  FreeType2 is required by FTGL.])
				fi
			fi
		fi
	else
		# have Freetype2
		AC_MSG_CHECKING(for Freetype)
		AC_CHECK_PROG([FREETYPE_CONFIG],[freetype-config],yes,no)
		if test "$FREETYPE_CONFIG" = yes; then
			FT2_CFLAGS=`freetype-config --cflags`
			FT2_LIBS=`freetype-config --libs`
			AC_MSG_RESULT($FT2_CFLAGS)
			CPPFLAGS="$CPPFLAGS $FT2_CFLAGS"
			HOST_LIBS="$HOST_LIBS $FT2_LIBS"
			FTGL_LOCALDIR="ftgl"
			FTGL_LA="\$(top_builddir)/src/utils/ftgl/libftgl.la"
		else
			AC_MSG_ERROR([Could not find freetype-config.  FreeType2 is required by FTGL.])
		fi
	fi

AC_CONFIG_FILES([Makefile src/Makefile src/brainz/Makefile \
				src/math/Makefile \
				src/gl/Makefile \
				src/scenes/Makefile \
				src/scenes/entities/Makefile \
				src/scenes/modes/Makefile \
				src/utils/Makefile \
				src/utils/ftgl/Makefile \
				src/utils/bullet/Makefile \
				src/gui/Makefile \
				src/scenes/gui/Makefile
])
AC_OUTPUT

