## Process this file with automake to produce Makefile.in

SUBDIRS = bitmaps

bin_PROGRAMS = qvwm

qvwm_SOURCES = button.cc button.h callback.h colormap.cc configure.cc \
ctrl_button.cc debug.cc debug.h desktop.cc desktop.h dialog.cc dialog.h \
event.cc event.h exit_dialog.cc exit_dialog.h fbutton.cc fbutton.h focus.cc \
focus_mgr.cc focus_mgr.h frame.cc function.cc hash.h icon.cc icon.h \
indicator.cc indicator.h key.cc key.h yaccsrc.yy yaccsrc.h lexsrc.ll list.h \
main.cc main.h menu.cc menu.h mini.cc mini.h misc.h move.cc pager.cc pager.h \
paging.cc paging.h parse.cc parse.h property.cc qvwm.cc qvwm.h \
qvwmrc.cc qvwmrc.h radio_button.cc radio_button.h radio_set.cc radio_set.h \
rearrange.cc resize.cc resize_sp.cc resource.h sbutton.cc sbutton.h \
startmenu.cc startmenu.h string_button.cc string_button.h \
switcher.cc switcher.h system_dialog.cc system_dialog.h taskbar.cc taskbar.h \
tbutton.cc tbutton.h titlebar.cc timer.cc timer.h util.cc util.h ver.h \
window.cc mwm.cc mwm.h gnome.cc gnome.h audio.cc audio.h audio_wav.cc \
audio_wav.h audio_au.cc audio_au.h exec.cc exec.h info_display.cc \
info_display.h tooltip.cc tooltip.h accessory.cc accessory.h \
image.cc image.h pixmap_image.cc pixmap_image.h anim_image.cc anim_image.h \
extra_image.cc extra_image.h session.cc session.h confirm_dialog.cc \
confirm_dialog.h screen_saver.cc screen_saver.h

DEFS += -DIMGDIR=\"@QVWMDIR@/images\" -DSNDDIR=\"@QVWMDIR@/sounds\" -DQVWMDIR=\"@QVWMDIR@\" -DPATH_RM=\"@PATH_RM@\"

DEFS += -DHAVE_CONFIG_H -I$(top_builddir)

# VPATH is not enough for lexsrc.ll and yaccsrc.yy
DEFS += -I$(srcdir)

qvwm_LDADD = @LEXLIB@

YFLAGS = -d

EXTRA_DIST = Imakefile Makefile.std
