/*
 * panel-applet-info.h
 *
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#ifndef __PANEL_APPLET_INFO_H__
#define __PANEL_APPLET_INFO_H__

#include <glib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _Gde2PanelAppletInfo Gde2PanelAppletInfo;

Gde2PanelAppletInfo *gde2_panel_applet_info_new                             (const gchar  *iid,
								    const gchar  *name,
								    const gchar  *comment,
								    const gchar  *icon,
								    const gchar **old_ids);
void             gde2_panel_applet_info_free                            (Gde2PanelAppletInfo *info);

const gchar     *gde2_panel_applet_info_get_iid                         (Gde2PanelAppletInfo *info);
const gchar     *gde2_panel_applet_info_get_name                        (Gde2PanelAppletInfo *info);
const gchar     *gde2_panel_applet_info_get_description                 (Gde2PanelAppletInfo *info);
const gchar     *gde2_panel_applet_info_get_icon                        (Gde2PanelAppletInfo *info);
const gchar * const *gde2_panel_applet_info_get_old_ids                 (Gde2PanelAppletInfo *info);

#ifdef __cplusplus
}
#endif

#endif /* __PANEL_APPLET_INFO_H__ */
