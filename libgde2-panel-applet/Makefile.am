lib_LTLIBRARIES = libgde2-panel-applet-4.la
noinst_PROGRAMS = test-dbus-applet

AM_CPPFLAGS =							\
	$(LIBGDE2_PANEL_APPLET_CFLAGS)				\
	-I$(top_builddir)/libgde2-panel-applet			\
	-DGDE2LOCALEDIR=\""$(prefix)/$(DATADIRNAME)/locale"\"	\
	$(DISABLE_DEPRECATED_CFLAGS)

AM_CFLAGS = $(WARN_CFLAGS)

libgde2_panel_appletincludedir = $(includedir)/gde2-panel-4.0/libgde2-panel-applet
libgde2_panel_appletinclude_HEADERS =	\
	gde2-panel-applet.h			\
	gde2-panel-applet-gsettings.h		\
	gde2-panel-applet-enums.h

libgde2_panel_applet_4_la_SOURCES =		\
	$(BUILT_SOURCES)		\
	gde2-panel-applet.h			\
	gde2-panel-applet.c			\
	gde2-panel-applet-factory.h		\
	gde2-panel-applet-factory.c		\
	gde2-panel-applet-gsettings.c		\
	gde2-panel-applet-gsettings.h       \
	panel-plug.c			\
	panel-plug-private.h

libgde2_panel_applet_4_la_LIBADD  = \
	$(LIBGDE2_PANEL_APPLET_LIBS) \
	$(X_LIBS)

libgde2_panel_applet_4_la_LDFLAGS = \
	-version-info $(LIB_GDE2_PANEL_APPLET_LT_VERSION)	\
	-no-undefined

test_dbus_applet_LDADD =	\
	$(LIBGDE2_PANEL_APPLET_LIBS)	\
	libgde2-panel-applet-4.la

$(libgde2_panel_applet_4_la_OBJECTS) $(test_dbus_applet_OBJECTS): $(BUILT_SOURCES)

gde2-panel-applet-marshal.h: gde2-panel-applet-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN)$(GLIB_GENMARSHAL) $< --header --prefix=gde2_panel_applet_marshal > $@

gde2-panel-applet-marshal.c: gde2-panel-applet-marshal.list $(GLIB_GENMARSHAL)
	$(AM_V_GEN)echo "#include \"gde2-panel-applet-marshal.h\"" > $@ && \
	$(GLIB_GENMARSHAL) $< --body --prefix=gde2_panel_applet_marshal >> $@

gde2_panel_applet_enum_headers = $(top_srcdir)/libgde2-panel-applet/gde2-panel-applet.h

gde2-panel-applet-enums.c: @REBUILD@ $(gde2_panel_applet_enum_headers)
	$(AM_V_GEN)glib-mkenums \
			--fhead "#include <glib-object.h>\n" \
			--fhead "#include \"gde2-panel-applet-enums.h\"\n\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--fprod "\n#include \"@filename@\"\n" \
			--vhead "static const G@Type@Value _@enum_name@_values[] = {" \
			--vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "  { 0, NULL, NULL }\n};\n\n" \
			--vtail "GType\n@enum_name@_get_type (void)\n{\n" \
			--vtail "  static GType type = 0;\n\n" \
			--vtail "  if (!type)\n" \
			--vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
			--vtail "  return type;\n}\n\n" \
		$(gde2_panel_applet_enum_headers) > $@

gde2-panel-applet-enums.h: @REBUILD@ $(gde2_panel_applet_enum_headers)
	$(AM_V_GEN)glib-mkenums \
			--fhead "#ifndef __GDE2_PANEL_APPLET_ENUMS_H__\n" \
			--fhead "#define __GDE2_PANEL_APPLET_ENUMS_H__\n\n" \
			--fhead "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n" \
			--ftail "#ifdef __cplusplus\n}\n#endif\n\n" \
			--ftail "#endif /* __GDE2_PANEL_APPLET_ENUMS_H__ */\n" \
			--fprod "\n/* --- @filename@ --- */" \
			--eprod "#define PANEL_TYPE_@ENUMNAME@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(gde2_panel_applet_enum_headers) > $@

BUILT_SOURCES = \
	gde2-panel-applet-enums.c			\
	gde2-panel-applet-enums.h			\
	gde2-panel-applet-marshal.c			\
	gde2-panel-applet-marshal.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libgde2panelapplet-4.0.pc

appletdir       = $(datadir)/gde2-panel/applets
applet_in_files = org.gde2.panel.TestApplet.gde2-panel-applet.in
noinst_DATA     = $(applet_in_files:.gde2-panel-applet.in=.gde2-panel-applet)

@PANEL_INTLTOOL_GDE2_PANEL_APPLET_RULE@

EXTRA_DIST =						\
	org.gde2.panel.TestApplet.gde2-panel-applet.in	\
	gde2-panel-applet-marshal.list			\
	libgde2panelapplet-4.0.pc.in			\
	libgde2panelapplet-4.0-uninstalled.pc.in

CLEANFILES = $(BUILT_SOURCES) $(noinst_DATA)

###############################
# Introspection generation

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --warn-all --add-include-path=$(srcdir) --add-include-path=${prefix}/share/gir-1.0
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_sources = 	\
	gde2-panel-applet.c		\
	gde2-panel-applet.h		\
	gde2-panel-applet-gsettings.c	\
	gde2-panel-applet-gsettings.h
built_introspection_sources =	\
	gde2-panel-applet-enums.c	\
	gde2-panel-applet-enums.h

Gde2PanelApplet-4.0.gir: libgde2-panel-applet-4.la
Gde2PanelApplet_4_0_gir_INCLUDES = GObject-2.0 Gtk-$(GTK_API_VERSION) Gio-2.0
Gde2PanelApplet_4_0_gir_CFLAGS = $(AM_CPPFLAGS) -I$(srcdir)
Gde2PanelApplet_4_0_gir_LIBS = libgde2-panel-applet-4.la
Gde2PanelApplet_4_0_gir_FILES = $(addprefix $(srcdir)/,$(introspection_sources)) $(addprefix $(builddir)/,$(built_introspection_sources))
# When requiring gobject-introspection 0.9.5, use this:
#Gde2PanelApplet_4_0_gir_SCANNERFLAGS = --identifier-prefix=Panel --symbol-prefix=panel --pkg-export=libgde2panelapplet-4.0
Gde2PanelApplet_4_0_gir_SCANNERFLAGS = --strip-prefix=Gde2Panel --pkg-export=libgde2panelapplet-4.0
INTROSPECTION_GIRS += Gde2PanelApplet-4.0.gir

girdir = $(INTROSPECTION_GIRDIR)
gir_DATA = $(INTROSPECTION_GIRS)

typelibdir = $(INTROSPECTION_TYPELIBDIR)
typelib_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelib_DATA)
endif

################################

dist-hook:
	cd $(distdir) ; rm -f $(CLEANFILES)

-include $(top_srcdir)/git.mk
