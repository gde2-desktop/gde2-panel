/*
 * panel-applet-container.h: a container for applets.
 *
 * Copyright (C) 2010 Carlos Garcia Campos <carlosgc@gnome.org>
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
 */

#ifndef __PANEL_APPLET_CONTAINER_H__
#define __PANEL_APPLET_CONTAINER_H__

#include <glib-object.h>
#include <gtk/gtk.h>
#include "panel.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PANEL_TYPE_APPLET_CONTAINER            (gde2_panel_applet_container_get_type ())
#define GDE2_PANEL_APPLET_CONTAINER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), PANEL_TYPE_APPLET_CONTAINER, Gde2PanelAppletContainer))
#define GDE2_PANEL_APPLET_CONTAINER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), PANEL_TYPE_APPLET_CONTAINER, Gde2PanelAppletContainerClass))
#define PANEL_IS_APPLET_CONTAINER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), PANEL_TYPE_APPLET_CONTAINER))
#define PANEL_IS_APPLET_CONTAINER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), PANEL_TYPE_APPLET_CONTAINER))
#define GDE2_PANEL_APPLET_CONTAINER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), PANEL_TYPE_APPLET_CONTAINER, Gde2PanelAppletContainerClass))

#define GDE2_PANEL_APPLET_CONTAINER_ERROR           (gde2_panel_applet_container_error_quark())

typedef enum {
	GDE2_PANEL_APPLET_CONTAINER_INVALID_APPLET,
	GDE2_PANEL_APPLET_CONTAINER_INVALID_CHILD_PROPERTY
} Gde2PanelAppletContainerError;

typedef struct _Gde2PanelAppletContainer        Gde2PanelAppletContainer;
typedef struct _Gde2PanelAppletContainerClass   Gde2PanelAppletContainerClass;
typedef struct _Gde2PanelAppletContainerPrivate Gde2PanelAppletContainerPrivate;

struct _Gde2PanelAppletContainer {
	GtkEventBox parent;

	Gde2PanelAppletContainerPrivate *priv;
};

struct _Gde2PanelAppletContainerClass {
	GtkEventBoxClass parent_class;

	/* Signals */
	void (*applet_broken)          (Gde2PanelAppletContainer *container);
	void (*applet_move)            (Gde2PanelAppletContainer *container);
	void (*applet_remove)          (Gde2PanelAppletContainer *container);
	void (*applet_lock)            (Gde2PanelAppletContainer *container,
					gboolean              locked);
	void (*child_property_changed) (Gde2PanelAppletContainer *container,
					const gchar          *property_name,
					GVariant             *value);
};

GType      gde2_panel_applet_container_get_type                (void) G_GNUC_CONST;
GQuark     gde2_panel_applet_container_error_quark             (void) G_GNUC_CONST;
GtkWidget *gde2_panel_applet_container_new                     (void);


void       gde2_panel_applet_container_add                     (Gde2PanelAppletContainer *container,
							   GdkScreen            *screen,
							   const gchar          *iid,
							   GCancellable        *cancellable,
							   GAsyncReadyCallback  callback,
							   gpointer             user_data,
							   GVariant            *properties);
gboolean   gde2_panel_applet_container_add_finish              (Gde2PanelAppletContainer *container,
							   GAsyncResult         *result,
							   GError              **error);
void       gde2_panel_applet_container_child_popup_menu        (Gde2PanelAppletContainer *container,
							   guint                 button,
							   guint32               timestamp,
							   GCancellable         *cancellable,
							   GAsyncReadyCallback   callback,
							   gpointer              user_data);
gboolean   gde2_panel_applet_container_child_popup_menu_finish (Gde2PanelAppletContainer *container,
							   GAsyncResult         *result,
							   GError              **error);

void       gde2_panel_applet_container_child_set               (Gde2PanelAppletContainer *container,
							   const gchar          *property_name,
							   const GVariant       *value,
							   GCancellable         *cancellable,
							   GAsyncReadyCallback   callback,
							   gpointer              user_data);
gboolean   gde2_panel_applet_container_child_set_finish        (Gde2PanelAppletContainer *container,
							   GAsyncResult         *result,
							   GError              **error);
void       gde2_panel_applet_container_child_get               (Gde2PanelAppletContainer *container,
							   const gchar          *property_name,
							   GCancellable         *cancellable,
							   GAsyncReadyCallback   callback,
							   gpointer              user_data);
GVariant  *gde2_panel_applet_container_child_get_finish        (Gde2PanelAppletContainer *container,
							   GAsyncResult         *result,
							   GError              **error);

#ifdef __cplusplus
}
#endif

#endif /* __PANEL_APPLET_CONTAINER_H__ */
