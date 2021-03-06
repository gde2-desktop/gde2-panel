/* window-menu.c: Window Selector applet
 *
 * Copyright (C) 2003 Sun Microsystems, Inc.
 * Copyright (C) 2001 Free Software Foundation, Inc.
 * Copyright (C) 2000 Helix Code, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors:
 *      Mark McLoughlin <mark@skynet.ie>
 *      George Lebl <jirka@5z.com>
 *      Jacob Berkman <jacob@helixcode.com>
 */

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <string.h>
#include <gde2-panel-applet.h>

#include <glib/gi18n.h>
#include <gdk/gdkkeysyms.h>

#include <libgde2-desktop/gde2-aboutdialog.h>

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

#include "wncklet.h"
#include "window-menu.h"

#define WINDOW_MENU_ICON "gde2-panel-window-menu"

typedef struct {
	GtkWidget* applet;
	GtkWidget* selector;
	int size;
	Gde2PanelAppletOrient orient;
} WindowMenu;

static void window_menu_help(GtkAction* action, WindowMenu* window_menu)
{
	wncklet_display_help(window_menu->applet, "gde2-user-guide", "panel-windowselector", WINDOW_MENU_ICON);
}

static void window_menu_about(GtkAction* action, WindowMenu* window_menu)
{
	static const char* authors[] = {
		"Perberos <perberos@gmail.com>",
		"Steve Zesch <stevezesch2@gmail.com>",
		"Stefano Karapetsas <stefano@karapetsas.com>",
		"Mark McLoughlin <mark@skynet.ie>",
		"George Lebl <jirka@5z.com>",
		"Jacob Berkman <jacob@helixcode.com>",
		NULL
	};

	const char* documenters[] = {
		"Sun GNOME Documentation Team <gdocteam@sun.com>",
		NULL
	};

	char copyright[] = \
		"Copyright \xc2\xa9 2011 Perberos\n"
		"Copyright \xc2\xa9 2003 Sun Microsystems, Inc.\n"
		"Copyright \xc2\xa9 2001 Free Software Foundation, Inc.\n"
		"Copyright \xc2\xa9 2000 Helix Code, Inc.";

	gde2_show_about_dialog(GTK_WINDOW(window_menu->applet),
		"program-name", _("Window Selector"),
		"authors", authors,
		"comments", _("The Window Selector shows a list of all windows in a menu and lets you browse them."),
		"copyright", copyright,
		"documenters", documenters,
		"icon-name", WINDOW_MENU_ICON,
		"logo-icon-name", WINDOW_MENU_ICON,
		"translator-credits", _("translator-credits"),
		"version", VERSION,
		"website", "http://www.gde2-desktop.org/",
		NULL);
}

static const GtkActionEntry window_menu_actions[] = {
	{
		"WindowMenuHelp",
		GTK_STOCK_HELP,
		N_("_Help"),
		NULL,
		NULL,
		G_CALLBACK(window_menu_help)
	},
	{
		"WindowMenuAbout",
		GTK_STOCK_ABOUT,
		N_("_About"),
		NULL,
		NULL,
		G_CALLBACK(window_menu_about)
	}
};

static void window_menu_destroy(GtkWidget* widget, WindowMenu* window_menu)
{
	g_free(window_menu);
}

#if GTK_CHECK_VERSION (3, 0, 0)
static gboolean window_menu_on_draw (GtkWidget* widget,
				     cairo_t*   cr,
				     gpointer   data)
{
	GtkStyleContext *context;
	GtkStateFlags    state;
	WindowMenu      *window_menu = data;

	if (!gtk_widget_has_focus (window_menu->applet))
		return FALSE;

	state = gtk_widget_get_state_flags (widget);
	context = gtk_widget_get_style_context (widget);
	gtk_style_context_save (context);
	gtk_style_context_set_state (context, state);

	cairo_save (cr);
	gtk_render_focus (context, cr,
			  0., 0.,
			  gtk_widget_get_allocated_width (widget),
			  gtk_widget_get_allocated_height (widget));
			  cairo_restore (cr);

	gtk_style_context_restore (context);

	return FALSE;
}
#else
static gboolean window_menu_on_expose (GtkWidget*      widget,
				       GdkEventExpose* event,
				       gpointer        data)
{
	WindowMenu* window_menu = data;

	if (gtk_widget_has_focus(window_menu->applet))
		gtk_paint_focus(gtk_widget_get_style(widget),
						gtk_widget_get_window(widget),
						gtk_widget_get_state(widget),
						NULL,
						widget,
						"menu-applet",
						0, 0,
						-1, -1);

	return FALSE;
}
#endif

#if GTK_CHECK_VERSION (3, 0, 0)
#if !GTK_CHECK_VERSION (3, 19, 0)
static inline void force_no_focus_padding(GtkWidget* widget)
{
	GtkCssProvider *provider;

	provider = gtk_css_provider_new ();
	gtk_css_provider_load_from_data (provider,
					 "#PanelApplet-window-menu-applet-button {\n"
					 " border-width: 0px;\n"
					 " -GtkWidget-focus-line-width: 0px;\n"
					 " -GtkWidget-focus-padding: 0px; }",
					 -1, NULL);
	gtk_style_context_add_provider (gtk_widget_get_style_context (widget),
					GTK_STYLE_PROVIDER (provider),
					GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref (provider);

	gtk_widget_set_name(widget, "PanelApplet-window-menu-applet-button");
}
#endif
#else
static inline void force_no_focus_padding(GtkWidget* widget)
{
	gboolean first_time = TRUE;

	if (first_time)
	{
		gtk_rc_parse_string("\n"
			"   style \"window-menu-applet-button-style\"\n"
			"   {\n"
			"      GtkWidget::focus-line-width=0\n"
			"      GtkWidget::focus-padding=0\n"
			"   }\n"
			"\n"
			"    widget \"*.PanelApplet-window-menu-applet-button\" style \"window-menu-applet-button-style\"\n"
			"\n");
		first_time = FALSE;
	}

	gtk_widget_set_name(widget, "PanelApplet-window-menu-applet-button");
}
#endif

static void window_menu_size_allocate(Gde2PanelApplet* applet, GtkAllocation* allocation, WindowMenu* window_menu)
{
	Gde2PanelAppletOrient orient;
	GList* children;
	GtkWidget* child;

	orient = gde2_panel_applet_get_orient(applet);

	children = gtk_container_get_children(GTK_CONTAINER(window_menu->selector));
	child = GTK_WIDGET(children->data);
	g_list_free(children);

	if (orient == GDE2_PANEL_APPLET_ORIENT_LEFT || orient == GDE2_PANEL_APPLET_ORIENT_RIGHT)
	{
		if (window_menu->size == allocation->width && orient == window_menu->orient)
			return;

		window_menu->size = allocation->width;
		gtk_widget_set_size_request(child, window_menu->size, -1);
	}
	else
	{
		if (window_menu->size == allocation->height && orient == window_menu->orient)
			return;

		window_menu->size = allocation->height;
		gtk_widget_set_size_request(child, -1, window_menu->size);
	}

	window_menu->orient = orient;
}

static gboolean window_menu_key_press_event(GtkWidget* widget, GdkEventKey* event, WindowMenu* window_menu)
{
	GtkMenuShell* menu_shell;
	WnckSelector* selector;

	switch (event->keyval)
	{
		case GDK_KEY_KP_Enter:
		case GDK_KEY_ISO_Enter:
		case GDK_KEY_3270_Enter:
		case GDK_KEY_Return:
		case GDK_KEY_space:
		case GDK_KEY_KP_Space:
			selector = WNCK_SELECTOR(window_menu->selector);
			/*
			 * We need to call _gtk_menu_shell_activate() here as is done in
			 * window_key_press_handler in gtkmenubar.c which pops up menu
			 * when F10 is pressed.
			 *
			 * As that function is private its code is replicated here.
			 */
			menu_shell = GTK_MENU_SHELL(selector);

#if !GTK_CHECK_VERSION (3, 0, 0)
			if (!menu_shell->active)
			{
				gtk_grab_add(GTK_WIDGET(menu_shell));
				menu_shell->have_grab = TRUE;
				menu_shell->active = TRUE;
			}
#endif

			gtk_menu_shell_select_first(menu_shell, FALSE);
			return TRUE;
		default:
			break;
	}

	return FALSE;
}

static gboolean filter_button_press(GtkWidget* widget, GdkEventButton* event, gpointer data)
{
	if (event->button != 1)
		g_signal_stop_emission_by_name(widget, "button_press_event");

	return FALSE;
}

gboolean window_menu_applet_fill(Gde2PanelApplet* applet)
{
	WindowMenu* window_menu;
	GtkActionGroup* action_group;
	gchar* ui_path;

	window_menu = g_new0(WindowMenu, 1);

	window_menu->applet = GTK_WIDGET(applet);
#if GTK_CHECK_VERSION (3, 19, 0)
	gtk_widget_set_name (window_menu->applet, "window-menu-applet-button");
#else
	force_no_focus_padding(window_menu->applet);
#endif
	gtk_widget_set_tooltip_text(window_menu->applet, _("Window Selector"));

	gde2_panel_applet_set_flags(applet, GDE2_PANEL_APPLET_EXPAND_MINOR);
	window_menu->size = gde2_panel_applet_get_size(applet);
	window_menu->orient = gde2_panel_applet_get_orient(applet);

	g_signal_connect(window_menu->applet, "destroy", G_CALLBACK(window_menu_destroy), window_menu);

	action_group = gtk_action_group_new("WindowMenu Applet Actions");
	gtk_action_group_set_translation_domain(action_group, GETTEXT_PACKAGE);
	gtk_action_group_add_actions(action_group, window_menu_actions, G_N_ELEMENTS(window_menu_actions), window_menu);
	ui_path = g_build_filename(WNCK_MENU_UI_DIR, "window-menu-menu.xml", NULL);
	gde2_panel_applet_setup_menu_from_file(applet, ui_path, action_group);
	g_free(ui_path);
	g_object_unref(action_group);

	window_menu->selector = wnck_selector_new();
	gtk_container_add(GTK_CONTAINER(window_menu->applet), window_menu->selector);

	gde2_panel_applet_set_background_widget(GDE2_PANEL_APPLET(window_menu->applet), GTK_WIDGET(window_menu->selector));

	g_signal_connect(window_menu->applet, "key_press_event", G_CALLBACK(window_menu_key_press_event), window_menu);
	g_signal_connect(window_menu->applet, "size-allocate", G_CALLBACK(window_menu_size_allocate), window_menu);

	g_signal_connect_after(G_OBJECT(window_menu->applet), "focus-in-event", G_CALLBACK(gtk_widget_queue_draw), window_menu);
	g_signal_connect_after(G_OBJECT(window_menu->applet), "focus-out-event", G_CALLBACK(gtk_widget_queue_draw), window_menu);
#if GTK_CHECK_VERSION (3, 0, 0)
	g_signal_connect_after(G_OBJECT(window_menu->selector), "draw", G_CALLBACK(window_menu_on_draw), window_menu);
#else
	g_signal_connect_after(G_OBJECT(window_menu->selector), "expose-event", G_CALLBACK(window_menu_on_expose), window_menu);
#endif

	g_signal_connect(G_OBJECT(window_menu->selector), "button_press_event", G_CALLBACK(filter_button_press), window_menu);

	gtk_widget_show_all(GTK_WIDGET(window_menu->applet));

	return TRUE;
}
