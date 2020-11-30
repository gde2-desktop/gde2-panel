/*
 * gde2-panel-applet.h: panel applet writing API.
 *
 * Copyright (C) 2001 Sun Microsystems, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors:
 *     Mark McLoughlin <mark@skynet.ie>
 */

#ifndef __GDE2_PANEL_APPLET_H__
#define __GDE2_PANEL_APPLET_H__

#include <glib.h>
#include <gtk/gtk.h>
#if GTK_CHECK_VERSION (3, 0, 0)
#include <gdk/gdk.h>
#include <cairo.h>
#include <cairo-gobject.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	GDE2_PANEL_APPLET_ORIENT_UP,
	GDE2_PANEL_APPLET_ORIENT_DOWN,
	GDE2_PANEL_APPLET_ORIENT_LEFT,
	GDE2_PANEL_APPLET_ORIENT_RIGHT
#define GDE2_PANEL_APPLET_ORIENT_FIRST GDE2_PANEL_APPLET_ORIENT_UP
#define GDE2_PANEL_APPLET_ORIENT_LAST  GDE2_PANEL_APPLET_ORIENT_RIGHT
} Gde2PanelAppletOrient;

#define PANEL_TYPE_APPLET              (gde2_panel_applet_get_type ())
#define GDE2_PANEL_APPLET(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), PANEL_TYPE_APPLET, Gde2PanelApplet))
#define GDE2_PANEL_APPLET_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), PANEL_TYPE_APPLET, Gde2PanelAppletClass))
#define PANEL_IS_APPLET(o)             (G_TYPE_CHECK_INSTANCE_TYPE ((o), PANEL_TYPE_APPLET))
#define PANEL_IS_APPLET_CLASS(k)       (G_TYPE_CHECK_CLASS_TYPE ((k), PANEL_TYPE_APPLET))
#define GDE2_PANEL_APPLET_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), PANEL_TYPE_APPLET, Gde2PanelAppletClass))

typedef enum {
	PANEL_NO_BACKGROUND,
	PANEL_COLOR_BACKGROUND,
	PANEL_PIXMAP_BACKGROUND
} Gde2PanelAppletBackgroundType;

typedef enum {
	GDE2_PANEL_APPLET_FLAGS_NONE   = 0,
	GDE2_PANEL_APPLET_EXPAND_MAJOR = 1 << 0,
	GDE2_PANEL_APPLET_EXPAND_MINOR = 1 << 1,
	GDE2_PANEL_APPLET_HAS_HANDLE   = 1 << 2
#define GDE2_PANEL_APPLET_FLAGS_ALL (GDE2_PANEL_APPLET_EXPAND_MAJOR|GDE2_PANEL_APPLET_EXPAND_MINOR|GDE2_PANEL_APPLET_HAS_HANDLE)
} Gde2PanelAppletFlags;

typedef struct _Gde2PanelApplet        Gde2PanelApplet;
typedef struct _Gde2PanelAppletClass   Gde2PanelAppletClass;
typedef struct _Gde2PanelAppletPrivate Gde2PanelAppletPrivate;

typedef gboolean (*Gde2PanelAppletFactoryCallback) (Gde2PanelApplet* applet, const gchar *iid, gpointer user_data);

struct _Gde2PanelApplet {
	GtkEventBox event_box;

	Gde2PanelAppletPrivate* priv;
};

struct _Gde2PanelAppletClass {
	GtkEventBoxClass event_box_class;

	void (*change_orient) (Gde2PanelApplet* applet, Gde2PanelAppletOrient orient);

	void (*change_size) (Gde2PanelApplet* applet, guint size);

#if GTK_CHECK_VERSION (3, 0, 0)
	void (*change_background) (Gde2PanelApplet *applet, Gde2PanelAppletBackgroundType type, GdkRGBA* color, cairo_pattern_t *pattern);
#else
	void (*change_background) (Gde2PanelApplet* applet, Gde2PanelAppletBackgroundType type, GdkColor* color, GdkPixmap* pixmap);
#endif
	void (*move_focus_out_of_applet) (Gde2PanelApplet* frame, GtkDirectionType direction);
};

GType gde2_panel_applet_get_type(void) G_GNUC_CONST;

GtkWidget* gde2_panel_applet_new(void);

Gde2PanelAppletOrient gde2_panel_applet_get_orient(Gde2PanelApplet* applet);
guint gde2_panel_applet_get_size(Gde2PanelApplet* applet);
#if GTK_CHECK_VERSION (3, 0, 0)
Gde2PanelAppletBackgroundType gde2_panel_applet_get_background (Gde2PanelApplet *applet, /* return values */ GdkRGBA* color, cairo_pattern_t** pattern);
#else
Gde2PanelAppletBackgroundType gde2_panel_applet_get_background(Gde2PanelApplet* applet, /* return values */ GdkColor* color, GdkPixmap** pixmap);
#endif
void gde2_panel_applet_set_background_widget(Gde2PanelApplet* applet, GtkWidget* widget);

gchar* gde2_panel_applet_get_preferences_path(Gde2PanelApplet* applet);

Gde2PanelAppletFlags gde2_panel_applet_get_flags(Gde2PanelApplet* applet);
void gde2_panel_applet_set_flags(Gde2PanelApplet* applet, Gde2PanelAppletFlags flags);

void gde2_panel_applet_set_size_hints(Gde2PanelApplet* applet, const int* size_hints, int n_elements, int base_size);

gboolean gde2_panel_applet_get_locked_down(Gde2PanelApplet* applet);

void gde2_panel_applet_request_focus(Gde2PanelApplet* applet, guint32 timestamp);

void gde2_panel_applet_setup_menu(Gde2PanelApplet* applet, const gchar* xml, GtkActionGroup* action_group);
void gde2_panel_applet_setup_menu_from_file(Gde2PanelApplet* applet, const gchar* filename, GtkActionGroup* action_group);

int gde2_panel_applet_factory_main(const gchar* factory_id, gboolean out_process, GType applet_type, Gde2PanelAppletFactoryCallback callback, gpointer data);
gboolean _gde2_panel_applet_shlib_factory(void);


/*
 * These macros are getting a bit unwieldy.
 *
 * Things to define for these:
 *	+ required if Native Language Support is enabled (ENABLE_NLS):
 *                   GETTEXT_PACKAGE and GDE2LOCALEDIR
 */

#if !defined(ENABLE_NLS)
	#define _GDE2_PANEL_APPLET_SETUP_GETTEXT(call_textdomain) \
		do { } while (0)
#else /* defined(ENABLE_NLS) */
	#include <libintl.h>
	#define _GDE2_PANEL_APPLET_SETUP_GETTEXT(call_textdomain) \
	do { \
		bindtextdomain (GETTEXT_PACKAGE, GDE2LOCALEDIR); \
		bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8"); \
		if (call_textdomain) \
			textdomain (GETTEXT_PACKAGE); \
	} while (0)
#endif /* !defined(ENABLE_NLS) */

#define GDE2_PANEL_APPLET_OUT_PROCESS_FACTORY(id, type, name, callback, data) \
int main(int argc, char* argv[]) \
{ \
	GOptionContext* context; \
	GError* error; \
	int retval; \
	 \
	_GDE2_PANEL_APPLET_SETUP_GETTEXT (TRUE); \
	 \
	context = g_option_context_new(""); \
	g_option_context_add_group (context, gtk_get_option_group(TRUE)); \
	 \
	error = NULL; \
	if (!g_option_context_parse (context, &argc, &argv, &error)) \
	{ \
		if (error) \
		{ \
			g_printerr ("Cannot parse arguments: %s.\n", error->message); \
			g_error_free (error); \
		} \
		else \
		{ \
			g_printerr ("Cannot parse arguments.\n"); \
		} \
		g_option_context_free (context); \
		return 1; \
	} \
	 \
	gtk_init (&argc, &argv); \
	 \
	retval = gde2_panel_applet_factory_main (id, TRUE, type, callback, data); \
	g_option_context_free (context); \
	 \
	return retval; \
}

#define GDE2_PANEL_APPLET_IN_PROCESS_FACTORY(id, type, descr, callback, data) \
G_MODULE_EXPORT gint _gde2_panel_applet_shlib_factory(void) \
{ \
	_GDE2_PANEL_APPLET_SETUP_GETTEXT(FALSE); \
	return gde2_panel_applet_factory_main(id, FALSE, type, callback, data); \
}

#ifdef __cplusplus
}
#endif

#endif /* __GDE2_PANEL_APPLET_H__ */
