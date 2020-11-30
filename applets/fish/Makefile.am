AM_CPPFLAGS = \
	$(LIBGDE2_PANEL_APPLET_CFLAGS) \
	$(FISH_CFLAGS) \
	-I$(srcdir) \
	-I$(srcdir)/../../libgde2-panel-applet \
	-I$(top_builddir)/libgde2-panel-applet \
	-DGDE2LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DFISH_ICONDIR=\""$(fishbitmapsdir)"\" \
	-DFISH_BUILDERDIR=\""$(uidir)"\" \
	-DFISH_MENU_UI_DIR=\""$(xmluidir)"\" \
	$(DISABLE_DEPRECATED_CFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)

FISH_SOURCES = fish.c

FISH_LDADD = \
	../../libgde2-panel-applet/libgde2-panel-applet-4.la \
	$(FISH_LIBS) \
	$(LIBGDE2_PANEL_APPLET_LIBS)

if FISH_INPROCESS
APPLET_IN_PROCESS = true
APPLET_LOCATION   = $(pkglibdir)/libfish-applet.so

fish_applet_libdir = $(pkglibdir)
fish_applet_lib_LTLIBRARIES = libfish-applet.la
libfish_applet_la_SOURCES = $(FISH_SOURCES)
libfish_applet_la_LIBADD = $(FISH_LDADD)
libfish_applet_la_LDFLAGS = -module -avoid-version
libfish_applet_la_CFLAGS = $(AM_CFLAGS)
else
APPLET_IN_PROCESS = false
APPLET_LOCATION = $(libexecdir)/fish-applet

libexec_PROGRAMS = fish-applet
fish_applet_SOURCES = $(FISH_SOURCES)
fish_applet_LDADD = $(FISH_LDADD)
fish_applet_CFLAGS = $(AM_CFLAGS)
endif

appletdir       = $(datadir)/gde2-panel/applets
applet_in_files = org.gde2.panel.FishApplet.gde2-panel-applet.in
applet_DATA     = $(applet_in_files:.gde2-panel-applet.in=.gde2-panel-applet)

$(applet_in_files): $(applet_in_files).in Makefile
	$(AM_V_GEN)sed \
		-e "s|\@LOCATION\@|$(APPLET_LOCATION)|" \
		-e "s|\@IN_PROCESS\@|$(APPLET_IN_PROCESS)|" \
		-e "s|\@VERSION\@|$(PACKAGE_VERSION)|" \
		$< > $@

@PANEL_INTLTOOL_GDE2_PANEL_APPLET_RULE@

if !FISH_INPROCESS
servicedir       = $(datadir)/dbus-1/services
service_in_files = org.gde2.panel.applet.FishAppletFactory.service.in
service_DATA     = $(service_in_files:.service.in=.service)

org.gde2.panel.applet.FishAppletFactory.service: $(service_in_files)
	$(AM_V_GEN)sed \
		-e "s|\@LOCATION\@|$(APPLET_LOCATION)|" \
		$< > $@
endif

fishbitmapsdir = $(datadir)/gde2-panel/pixmaps
fishbitmaps_DATA = \
	wanda.png \
	fishanim.png \
	oldwanda.png \
	footguy.png \
	monkey.png

uidir = $(datadir)/gde2-panel/ui
ui_DATA = fish.ui

xmluidir = $(datadir)/gde2-panel/ui
xmlui_DATA = fish-menu.xml

fish_gschemas_in = org.gde2.panel.applet.fish.gschema.xml.in
gsettings_SCHEMAS = $(fish_gschemas_in:.xml.in=.xml)
@INTLTOOL_XML_NOMERGE_RULE@
@GSETTINGS_RULES@

EXTRA_DIST = \
	wanda.svg \
	$(fishbitmaps_DATA) \
	org.gde2.panel.FishApplet.gde2-panel-applet.in.in \
	$(service_in_files) \
	$(ui_DATA) \
	$(xmlui_DATA) \
	$(fish_gschemas_in)

CLEANFILES = $(applet_DATA) $(applet_DATA).in $(service_DATA) $(gsettings_SCHEMAS)

-include $(top_srcdir)/git.mk
