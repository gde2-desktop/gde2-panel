/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * panel-applet-frame-dbus.c: panel side container for applets
 *
 * Copyright (C) 2001 - 2003 Sun Microsystems, Inc.
 * Copyright (C) 2010 Carlos Garcia Campos <carlosgc@gnome.org>
 * Copyright (C) 2010 Vincent Untz <vuntz@gnome.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * Authors:
 *	Mark McLoughlin <mark@skynet.ie>
 */

#include <config.h>

#include <string.h>

#include <panel-applet-frame.h>
#include <panel-applets-manager.h>

#include "panel-applet-container.h"
#include "panel-applet-frame-dbus.h"

G_DEFINE_TYPE (Gde2PanelAppletFrameDBus,
	       gde2_panel_applet_frame_dbus,
	       PANEL_TYPE_APPLET_FRAME)

struct _Gde2PanelAppletFrameDBusPrivate
{
	Gde2PanelAppletContainer *container;
	GCancellable         *bg_cancellable;
};

/* Keep in sync with gde2-panel-applet.h. Uggh. */
typedef enum {
	APPLET_FLAGS_NONE   = 0,
	APPLET_EXPAND_MAJOR = 1 << 0,
	APPLET_EXPAND_MINOR = 1 << 1,
	APPLET_HAS_HANDLE   = 1 << 2
} Gde2PanelAppletFlags;


static guint
get_gde2_panel_applet_orient (PanelOrientation orientation)
{
	/* For some reason libgde2-panel-applet and panel use a different logic for
	 * orientation, so we need to convert it. We should fix this. */
	switch (orientation) {
	case PANEL_ORIENTATION_TOP:
		return 1;
	case PANEL_ORIENTATION_BOTTOM:
		return 0;
	case PANEL_ORIENTATION_LEFT:
		return 3;
	case PANEL_ORIENTATION_RIGHT:
		return 2;
	default:
		g_assert_not_reached ();
		break;
	}
}

static void
gde2_panel_applet_frame_dbus_update_flags (Gde2PanelAppletFrame *frame,
				      GVariant         *value)
{
	guint32  flags;
	gboolean major;
	gboolean minor;
	gboolean has_handle;

	flags = g_variant_get_uint32 (value);

	major = (flags & APPLET_EXPAND_MAJOR) != 0;
	minor = (flags & APPLET_EXPAND_MINOR) != 0;
	has_handle = (flags & APPLET_HAS_HANDLE) != 0;

	_gde2_panel_applet_frame_update_flags (frame, major, minor, has_handle);
}


static void
gde2_panel_applet_frame_dbus_get_flags_cb (Gde2PanelAppletContainer *container,
				      GAsyncResult         *res,
				      Gde2PanelAppletFrame     *frame)
{
	GVariant *value;
	GError   *error = NULL;

	value = gde2_panel_applet_container_child_get_finish (container, res, &error);
	if (!value) {
		g_warning ("%s\n", error->message);
		g_error_free (error);
		return;
	}

	gde2_panel_applet_frame_dbus_update_flags (frame, value);
	g_variant_unref (value);
}

static void
gde2_panel_applet_frame_dbus_get_size_hints_cb (Gde2PanelAppletContainer *container,
					   GAsyncResult         *res,
					   Gde2PanelAppletFrame     *frame)
{
	GVariant   *value;
	const gint *sz;
	gint       *size_hints = NULL;
	gsize       n_elements;
	GError     *error = NULL;

	value = gde2_panel_applet_container_child_get_finish (container, res, &error);
	if (!value) {
		g_warning ("%s\n", error->message);
		g_error_free (error);
		return;
	}

	sz = g_variant_get_fixed_array (value, &n_elements, sizeof (gint32));
	if (n_elements > 0) {
		size_hints = g_new (gint32, n_elements);
		memcpy (size_hints, sz, n_elements * sizeof (gint32));
	}

	_gde2_panel_applet_frame_update_size_hints (frame, size_hints, n_elements);
	g_variant_unref (value);
}

static void
gde2_panel_applet_frame_dbus_init_properties (Gde2PanelAppletFrame *frame)
{
	Gde2PanelAppletFrameDBus *dbus_frame = GDE2_PANEL_APPLET_FRAME_DBUS (frame);

	gde2_panel_applet_container_child_get (dbus_frame->priv->container, "flags", NULL,
					  (GAsyncReadyCallback) gde2_panel_applet_frame_dbus_get_flags_cb,
					  frame);
	gde2_panel_applet_container_child_get (dbus_frame->priv->container, "size-hints", NULL,
					  (GAsyncReadyCallback) gde2_panel_applet_frame_dbus_get_size_hints_cb,
					  frame);
}

static void
gde2_panel_applet_frame_dbus_sync_menu_state (Gde2PanelAppletFrame *frame,
					 gboolean          movable,
					 gboolean          removable,
					 gboolean          lockable,
					 gboolean          locked,
					 gboolean          locked_down)
{
	Gde2PanelAppletFrameDBus *dbus_frame = GDE2_PANEL_APPLET_FRAME_DBUS (frame);

	gde2_panel_applet_container_child_set (dbus_frame->priv->container,
					  "locked", g_variant_new_boolean (lockable && locked),
					  NULL, NULL, NULL);
	gde2_panel_applet_container_child_set (dbus_frame->priv->container,
					  "locked-down", g_variant_new_boolean (locked_down),
					  NULL, NULL, NULL);
}

static void
gde2_panel_applet_frame_dbus_popup_menu (Gde2PanelAppletFrame *frame,
				    guint             button,
				    guint32           timestamp)
{
	Gde2PanelAppletFrameDBus *dbus_frame = GDE2_PANEL_APPLET_FRAME_DBUS (frame);

	gde2_panel_applet_container_child_popup_menu (dbus_frame->priv->container,
						 button, timestamp,
						 NULL, NULL, NULL);
}

static void
change_orientation_cb (Gde2PanelAppletContainer *container,
		       GAsyncResult         *res,
		       Gde2PanelAppletFrame     *frame)
{
	GError *error = NULL;

	if (!gde2_panel_applet_container_child_set_finish (container, res, &error)) {
		g_warning ("%s\n", error->message);
		g_error_free (error);

		return;
	}

	gtk_widget_queue_resize (GTK_WIDGET (frame));
}

static void
gde2_panel_applet_frame_dbus_change_orientation (Gde2PanelAppletFrame *frame,
					    PanelOrientation  orientation)
{
	Gde2PanelAppletFrameDBus *dbus_frame = GDE2_PANEL_APPLET_FRAME_DBUS (frame);

	gde2_panel_applet_container_child_set (dbus_frame->priv->container,
					  "orient",
					  g_variant_new_uint32 (get_gde2_panel_applet_orient (orientation)),
					  NULL,
					  (GAsyncReadyCallback)change_orientation_cb,
					  frame);
}

static void
gde2_panel_applet_frame_dbus_change_size (Gde2PanelAppletFrame *frame,
				     guint             size)
{
	Gde2PanelAppletFrameDBus *dbus_frame = GDE2_PANEL_APPLET_FRAME_DBUS (frame);

	gde2_panel_applet_container_child_set (dbus_frame->priv->container,
					  "size", g_variant_new_uint32 (size),
					  NULL, NULL, NULL);
}

static void
container_child_background_set (GObject      *source_object,
				GAsyncResult *res,
				gpointer      user_data)
{
	Gde2PanelAppletContainer *container = GDE2_PANEL_APPLET_CONTAINER (source_object);
	Gde2PanelAppletFrameDBus *frame = GDE2_PANEL_APPLET_FRAME_DBUS (user_data);

	gde2_panel_applet_container_child_set_finish (container, res, NULL);

	if (frame->priv->bg_cancellable)
		g_object_unref (frame->priv->bg_cancellable);

	frame->priv->bg_cancellable = NULL;
}

static void
gde2_panel_applet_frame_dbus_change_background (Gde2PanelAppletFrame    *frame,
					   PanelBackgroundType  type)
{
	Gde2PanelAppletFrameDBus *dbus_frame = GDE2_PANEL_APPLET_FRAME_DBUS (frame);
	char *bg_str;

	bg_str = _gde2_panel_applet_frame_get_background_string (
			frame, PANEL_WIDGET (gtk_widget_get_parent (GTK_WIDGET (frame))), type);

	if (bg_str != NULL) {
		if (dbus_frame->priv->bg_cancellable)
			g_cancellable_cancel (dbus_frame->priv->bg_cancellable);
		dbus_frame->priv->bg_cancellable = g_cancellable_new ();

		gde2_panel_applet_container_child_set (dbus_frame->priv->container,
						  "background",
						  g_variant_new_string (bg_str),
						  dbus_frame->priv->bg_cancellable,
						  container_child_background_set,
						  dbus_frame);
		g_free (bg_str);
	}
}

static void
gde2_panel_applet_frame_dbus_flags_changed (Gde2PanelAppletContainer *container,
				       const gchar          *prop_name,
				       GVariant             *value,
				       Gde2PanelAppletFrame     *frame)
{
	gde2_panel_applet_frame_dbus_update_flags (frame, value);
}

static void
gde2_panel_applet_frame_dbus_size_hints_changed (Gde2PanelAppletContainer *container,
					    const gchar          *prop_name,
					    GVariant             *value,
					    Gde2PanelAppletFrame     *frame)
{
	const gint *sz;
	gint       *size_hints = NULL;
	gsize       n_elements;

	sz = g_variant_get_fixed_array (value, &n_elements, sizeof (gint32));
	if (n_elements > 0) {
		size_hints = g_new (gint32, n_elements);
		memcpy (size_hints, sz, n_elements * sizeof (gint32));
	}

	_gde2_panel_applet_frame_update_size_hints (frame, size_hints, n_elements);
}

static void
gde2_panel_applet_frame_dbus_applet_broken (Gde2PanelAppletContainer *container,
				       Gde2PanelAppletFrame     *frame)
{
	_gde2_panel_applet_frame_applet_broken (frame);
}

static void
gde2_panel_applet_frame_dbus_applet_remove (Gde2PanelAppletContainer *container,
				       Gde2PanelAppletFrame     *frame)
{
	_gde2_panel_applet_frame_applet_remove (frame);
}

static void
gde2_panel_applet_frame_dbus_applet_move (Gde2PanelAppletContainer *container,
				     Gde2PanelAppletFrame     *frame)
{
	_gde2_panel_applet_frame_applet_move (frame);
}

static void
gde2_panel_applet_frame_dbus_applet_lock (Gde2PanelAppletContainer *container,
				     gboolean              locked,
				     Gde2PanelAppletFrame     *frame)
{
	_gde2_panel_applet_frame_applet_lock (frame, locked);
}

static void
gde2_panel_applet_frame_dbus_finalize (GObject *object)
{
	Gde2PanelAppletFrameDBus *frame = GDE2_PANEL_APPLET_FRAME_DBUS (object);

	if (frame->priv->bg_cancellable)
		g_object_unref (frame->priv->bg_cancellable);
	frame->priv->bg_cancellable = NULL;

	G_OBJECT_CLASS (gde2_panel_applet_frame_dbus_parent_class)->finalize (object);
}

static void
gde2_panel_applet_frame_dbus_init (Gde2PanelAppletFrameDBus *frame)
{
	GtkWidget *container;

	frame->priv = G_TYPE_INSTANCE_GET_PRIVATE (frame,
						   PANEL_TYPE_APPLET_FRAME_DBUS,
						   Gde2PanelAppletFrameDBusPrivate);

	container = gde2_panel_applet_container_new ();
	gtk_widget_show (container);
	gtk_container_add (GTK_CONTAINER (frame), container);
	frame->priv->container = GDE2_PANEL_APPLET_CONTAINER (container);
	frame->priv->bg_cancellable = NULL;

	g_signal_connect (container, "child-property-changed::flags",
			  G_CALLBACK (gde2_panel_applet_frame_dbus_flags_changed),
			  frame);
	g_signal_connect (container, "child-property-changed::size-hints",
			  G_CALLBACK (gde2_panel_applet_frame_dbus_size_hints_changed),
			  frame);
	g_signal_connect (container, "applet-broken",
			  G_CALLBACK (gde2_panel_applet_frame_dbus_applet_broken),
			  frame);
	g_signal_connect (container, "applet-remove",
			  G_CALLBACK (gde2_panel_applet_frame_dbus_applet_remove),
			  frame);
	g_signal_connect (container, "applet-move",
			  G_CALLBACK (gde2_panel_applet_frame_dbus_applet_move),
			  frame);
	g_signal_connect (container, "applet-lock",
			  G_CALLBACK (gde2_panel_applet_frame_dbus_applet_lock),
			  frame);
}

static void
gde2_panel_applet_frame_dbus_class_init (Gde2PanelAppletFrameDBusClass *class)
{
	GObjectClass *gobject_class = G_OBJECT_CLASS (class);
	Gde2PanelAppletFrameClass *frame_class = GDE2_PANEL_APPLET_FRAME_CLASS (class);

	gobject_class->finalize = gde2_panel_applet_frame_dbus_finalize;

	frame_class->init_properties = gde2_panel_applet_frame_dbus_init_properties;
	frame_class->sync_menu_state = gde2_panel_applet_frame_dbus_sync_menu_state;
	frame_class->popup_menu = gde2_panel_applet_frame_dbus_popup_menu;
	frame_class->change_orientation = gde2_panel_applet_frame_dbus_change_orientation;
	frame_class->change_size = gde2_panel_applet_frame_dbus_change_size;
	frame_class->change_background = gde2_panel_applet_frame_dbus_change_background;

#if GTK_CHECK_VERSION (3, 19, 0)
	GtkWidgetClass *widget_class  = GTK_WIDGET_CLASS (class);
	gtk_widget_class_set_css_name (widget_class, "Gde2PanelAppletFrameDBus");
#endif

	g_type_class_add_private (class, sizeof (Gde2PanelAppletFrameDBusPrivate));
}

static void
gde2_panel_applet_frame_dbus_activated (Gde2PanelAppletContainer *container,
				   GAsyncResult         *res,
				   Gde2PanelAppletFrame     *frame)
{
	Gde2PanelAppletFrameActivating *frame_act;
	GError *error = NULL;

	if (!gde2_panel_applet_container_add_finish (container, res, &error))
		g_assert (error != NULL);

	frame_act = g_object_get_data (G_OBJECT (frame), "gde2-panel-applet-frame-activating");
	g_object_set_data (G_OBJECT (frame), "gde2-panel-applet-frame-activating", NULL);

	_gde2_panel_applet_frame_activated (frame, frame_act, error);
}

gboolean
gde2_panel_applet_frame_dbus_load (const gchar                 *iid,
			      Gde2PanelAppletFrameActivating  *frame_act)
{
	Gde2PanelAppletFrameDBus *dbus_frame;
	Gde2PanelAppletFrame     *frame;
	GVariantBuilder       builder;
	GdkScreen            *screen;
	gchar                *conf_path;
	gchar                *background;
	guint                 orient;

	g_return_val_if_fail (iid != NULL, FALSE);
	g_return_val_if_fail (frame_act != NULL, FALSE);

	if (!gde2_panel_applets_manager_factory_activate (iid))
		return FALSE;

	dbus_frame = g_object_new (PANEL_TYPE_APPLET_FRAME_DBUS, NULL);
	frame = GDE2_PANEL_APPLET_FRAME (dbus_frame);
	_gde2_panel_applet_frame_set_iid (frame, iid);

	screen = panel_applet_frame_activating_get_screen (frame_act);
	orient = get_gde2_panel_applet_orient (gde2_panel_applet_frame_activating_get_orientation (frame_act));
	conf_path = gde2_panel_applet_frame_activating_get_conf_path (frame_act);
	/* we can't really get a background string at this point since we don't
	 * know the position of the applet */
	background = NULL;

	g_variant_builder_init (&builder, G_VARIANT_TYPE ("a{sv}"));
	g_variant_builder_add (&builder, "{sv}",
			       "prefs-path",
			       g_variant_new_string (conf_path));
	g_variant_builder_add (&builder, "{sv}",
			       "orient",
			       g_variant_new_uint32 (orient));
	g_variant_builder_add (&builder, "{sv}",
			       "size",
			       g_variant_new_uint32 (gde2_panel_applet_frame_activating_get_size (frame_act)));
	g_variant_builder_add (&builder, "{sv}",
			       "locked",
			       g_variant_new_boolean (gde2_panel_applet_frame_activating_get_locked (frame_act)));
	g_variant_builder_add (&builder, "{sv}",
			       "locked-down",
			       g_variant_new_boolean (gde2_panel_applet_frame_activating_get_locked_down (frame_act)));
	if (background) {
		g_variant_builder_add (&builder, "{sv}",
				       "background",
				       g_variant_new_string (background));
	}

	g_object_set_data (G_OBJECT (frame), "gde2-panel-applet-frame-activating", frame_act);

	gde2_panel_applet_container_add (dbus_frame->priv->container,
				    screen, iid, NULL,
				    (GAsyncReadyCallback) gde2_panel_applet_frame_dbus_activated,
				    frame,
				    g_variant_builder_end (&builder));

	g_free (conf_path);
	g_free (background);

	return TRUE;
}
