size = 32x32

iconsdir = $(datadir)/icons/hicolor/$(size)/apps

icons_DATA =					\
	gde2-panel-clock.png			\
	gde2-panel-drawer.png			\
	gde2-panel-fish.png			\
	gde2-panel-force-quit.png		\
	gde2-panel-launcher.png		\
	gde2-panel-notification-area.png	\
	gde2-panel-separator.png		\
	gde2-panel.png				\
	gde2-panel-window-list.png		\
	gde2-panel-window-menu.png		\
	gde2-panel-workspace-switcher.png

icons_SOURCE =					\
	gde2-panel-clock.svg			\
	gde2-panel-drawer.svg			\
	gde2-panel-fish.svg			\
	gde2-panel-force-quit.svg		\
	gde2-panel-launcher.svg		\
	gde2-panel-notification-area.svg	\
	gde2-panel-separator.svg		\
	gde2-panel.svg				\
	gde2-panel-window-list.svg		\
	gde2-panel-window-menu.svg		\
	gde2-panel-workspace-switcher.svg

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor

install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated. After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi

EXTRA_DIST =		\
	$(icons_DATA)	\
	$(icons_SOURCE)

-include $(top_srcdir)/git.mk
