/*
 * panel-applet-frame.h: panel side container for applets
 *
 * Copyright (C) 2001 - 2003 Sun Microsystems, Inc.
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 * Authors:
 *	Mark McLoughlin <mark@skynet.ie>
 */

#ifndef __PANEL_APPLET_FRAME_H__
#define __PANEL_APPLET_FRAME_H__

#include <gtk/gtk.h>

#include "panel-widget.h"
#include "applet.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PANEL_TYPE_APPLET_FRAME         (gde2_panel_applet_frame_get_type ())
#define GDE2_PANEL_APPLET_FRAME(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), PANEL_TYPE_APPLET_FRAME, Gde2PanelAppletFrame))
#define GDE2_PANEL_APPLET_FRAME_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), PANEL_TYPE_APPLET_FRAME, Gde2PanelAppletFrameClass))
#define PANEL_IS_APPLET_FRAME(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), PANEL_TYPE_APPLET_FRAME))
#define PANEL_IS_APPLET_FRAME_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), PANEL_TYPE_APPLET_FRAME))
#define GDE2_PANEL_APPLET_FRAME_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), PANEL_TYPE_APPLET_FRAME, Gde2PanelAppletFrameClass))

typedef struct _Gde2PanelAppletFrame        Gde2PanelAppletFrame;
typedef struct _Gde2PanelAppletFrameClass   Gde2PanelAppletFrameClass;
typedef struct _Gde2PanelAppletFramePrivate Gde2PanelAppletFramePrivate;

struct _Gde2PanelAppletFrameClass {
        GtkEventBoxClass parent_class;

	void     (*init_properties)       (Gde2PanelAppletFrame    *frame);

	void     (*sync_menu_state)       (Gde2PanelAppletFrame    *frame,
					   gboolean             movable,
					   gboolean             removable,
					   gboolean             lockable,
					   gboolean             locked,
					   gboolean             locked_down);

	void     (*popup_menu)            (Gde2PanelAppletFrame    *frame,
					   guint                button,
					   guint32              timestamp);

	void     (*change_orientation)    (Gde2PanelAppletFrame    *frame,
					   PanelOrientation     orientation);

	void     (*change_size)           (Gde2PanelAppletFrame    *frame,
					   guint                size);

	void     (*change_background)     (Gde2PanelAppletFrame    *frame,
					   PanelBackgroundType  type);
};

struct _Gde2PanelAppletFrame {
	GtkEventBox parent;

        Gde2PanelAppletFramePrivate  *priv;
};

GType gde2_panel_applet_frame_get_type           (void) G_GNUC_CONST;

void  gde2_panel_applet_frame_create             (PanelToplevel       *toplevel,
					     int                  position,
					     const char          *iid);

void  gde2_panel_applet_frame_load_from_gsettings    (PanelWidget         *panel_widget,
					     gboolean             locked,
					     int                  position,
					     const char          *id);

void  gde2_panel_applet_frame_sync_menu_state    (Gde2PanelAppletFrame    *frame);

void  gde2_panel_applet_frame_change_orientation (Gde2PanelAppletFrame    *frame,
					     PanelOrientation     orientation);

void  gde2_panel_applet_frame_change_size        (Gde2PanelAppletFrame    *frame,
					     guint                size);

void  gde2_panel_applet_frame_change_background  (Gde2PanelAppletFrame    *frame,
					     PanelBackgroundType  type);

void  gde2_panel_applet_frame_set_panel          (Gde2PanelAppletFrame    *frame,
					     PanelWidget         *panel);


/* For module implementations only */

typedef struct _Gde2PanelAppletFrameActivating        Gde2PanelAppletFrameActivating;

GdkScreen        *panel_applet_frame_activating_get_screen      (Gde2PanelAppletFrameActivating *frame_act);
PanelOrientation  gde2_panel_applet_frame_activating_get_orientation (Gde2PanelAppletFrameActivating *frame_act);
guint32           gde2_panel_applet_frame_activating_get_size        (Gde2PanelAppletFrameActivating *frame_act);
gboolean          gde2_panel_applet_frame_activating_get_locked      (Gde2PanelAppletFrameActivating *frame_act);
gboolean          gde2_panel_applet_frame_activating_get_locked_down (Gde2PanelAppletFrameActivating *frame_act);
gchar            *gde2_panel_applet_frame_activating_get_conf_path   (Gde2PanelAppletFrameActivating *frame_act);

void  _gde2_panel_applet_frame_set_iid               (Gde2PanelAppletFrame           *frame,
						 const gchar                *iid);

void  _gde2_panel_applet_frame_activated             (Gde2PanelAppletFrame           *frame,
						 Gde2PanelAppletFrameActivating *frame_act,
						 GError                     *error);

void  _gde2_panel_applet_frame_update_flags          (Gde2PanelAppletFrame *frame,
						 gboolean          major,
						 gboolean          minor,
						 gboolean          has_handle);

void  _gde2_panel_applet_frame_update_size_hints     (Gde2PanelAppletFrame *frame,
						 gint             *size_hints,
						 guint             n_elements);

char *_gde2_panel_applet_frame_get_background_string (Gde2PanelAppletFrame    *frame,
						 PanelWidget         *panel,
						 PanelBackgroundType  type);

void  _gde2_panel_applet_frame_applet_broken         (Gde2PanelAppletFrame *frame);

void  _gde2_panel_applet_frame_applet_remove         (Gde2PanelAppletFrame *frame);
void  _gde2_panel_applet_frame_applet_move           (Gde2PanelAppletFrame *frame);
void  _gde2_panel_applet_frame_applet_lock           (Gde2PanelAppletFrame *frame,
						 gboolean          locked);
#ifdef __cplusplus
}
#endif

#endif /* __PANEL_APPLET_FRAME_H__ */

