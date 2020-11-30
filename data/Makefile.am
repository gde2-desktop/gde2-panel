layoutdir   = $(datadir)/gde2-panel/layouts
layout_DATA = \
	default.layout \
	fedora.layout \
	linuxmint.layout \
	mageia.layout \
	opensuse.layout \
	ubuntu.layout

@INTLTOOL_XML_NOMERGE_RULE@

gsettings_ENUM_NAMESPACE = org.gde2.panel
gsettings_ENUM_FILES = $(top_srcdir)/gde2-panel/panel-enums-gsettings.h

panel_gschemas_in = \
	org.gde2.panel.gschema.xml.in \
	org.gde2.panel.object.gschema.xml.in \
	org.gde2.panel.toplevel.gschema.xml.in \
	org.gde2.panel.menubar.gschema.xml.in

gsettings_SCHEMAS = $(panel_gschemas_in:.xml.in=.xml)

%.gschema.xml: %.gschema.xml.in Makefile
	$(AM_V_GEN) $(SED) -e 's^\@GETTEXT_PACKAGE\@^$(GETTEXT_PACKAGE)^g' < $< > $@

@GSETTINGS_RULES@

EXTRA_DIST = \
	$(panel_gschemas_in) \
	$(layout_DATA)

CLEANFILES = $(gsettings_SCHEMAS)
