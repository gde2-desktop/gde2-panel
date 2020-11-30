noinst_LTLIBRARIES =			\
	libgde2-panel-applet-private.la	\
	libgde2-panel-applet-private-mini.la

AM_CPPFLAGS =							\
	$(PANEL_CFLAGS)						\
	-I.							\
	-I$(srcdir)						\
	-I$(top_srcdir)/gde2-panel				\
	-I$(top_builddir)/gde2-panel				\
	-I$(top_builddir)/gde2-panel/libgde2-panel-applets-private	\
	-I$(top_builddir)/gde2-panel/libpanel-util		\
	-DDATADIR=\""$(datadir)"\"				\
	-DGDE2_PANEL_APPLETS_DIR=\"$(appletsdir)\"			\
	$(DISABLE_DEPRECATED_CFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)

libgde2_panel_applet_private_la_SOURCES =	\
	panel-applets-manager-dbus.c	\
	panel-applets-manager-dbus.h	\
	panel-applet-container.c	\
	panel-applet-container.h	\
	panel-applet-frame-dbus.c	\
	panel-applet-frame-dbus.h

libgde2_panel_applet_private_mini_la_SOURCES =\
	panel-applet-mini.c		\
	panel-applets-manager-dbus.c	\
	panel-applets-manager-dbus.h	\
	panel-applet-container.c	\
	panel-applet-container.h

-include $(top_srcdir)/git.mk
