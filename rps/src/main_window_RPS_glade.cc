// generated 2003/10/2 15:31:05 CEST by thoma@Tiger.(none)
// using glademm V2.0.0c_cvs
//
// DO NOT EDIT THIS FILE ! It was created using
// glade-- /home/thoma/rollenspiel/rps/rpgs.glade
// for gtk 2.2.4 and gtkmm 2.2.7
//
// Please modify the corresponding derived classes in ./src/main_window_RPS.cc


#if defined __GNUC__ && __GNUC__ < 3
#error This program will crash if compiled with g++ 2.x
// see the dynamic_cast bug in the gtkmm FAQ
#endif //
#include "config.h"
/*
 * Standard gettext macros.
 */
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (GETTEXT_PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif
#include "main_window_RPS_glade.hh"
#include <gdk/gdkkeysyms.h>
#include <gtkmm/accelgroup.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/stockid.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>

main_window_RPS_glade::main_window_RPS_glade(
) : Gtk::Window(Gtk::WINDOW_TOPLEVEL)
{  
   
   Gtk::Window *main_window_RPS = this;
   gmm_data = new GlademmData(get_accel_group());
   
   Gtk::Toolbar *toolbar_main = Gtk::manage(new class Gtk::Toolbar());
   treeview_main = Gtk::manage(new class Gtk::TreeView());
   
   Gtk::ScrolledWindow *scrolledwindow1 = Gtk::manage(new class Gtk::ScrolledWindow());
   scrolledwindow_playlist = Gtk::manage(new class Gtk::ScrolledWindow());
   
   Gtk::HBox *hbox1 = Gtk::manage(new class Gtk::HBox(false, 0));
   Gtk::VBox *vbox2 = Gtk::manage(new class Gtk::VBox(false, 0));
   Gtk::EventBox *eventbox1 = Gtk::manage(new class Gtk::EventBox());
   
   toolbar_main->tools().push_back(Gtk::Toolbar_Helpers::ButtonElem(_("Quit"), *manage(new Gtk::Image(Gtk::StockID("gtk-quit"), Gtk::ICON_SIZE_LARGE_TOOLBAR)), Gtk::Toolbar_Helpers::Callback0()));
   Gtk::Button *button_quit = static_cast<Gtk::Button *>(toolbar_main->tools().back().get_widget());
   
   toolbar_main->tools().push_back(Gtk::Toolbar_Helpers::ToggleElem(_("Repeat"), *manage(new Gtk::Image(Gtk::StockID("gtk-refresh"), Gtk::ICON_SIZE_LARGE_TOOLBAR)), Gtk::Toolbar_Helpers::Callback0()));
   togglebutton_repeat = static_cast<Gtk::ToggleButton *>(toolbar_main->tools().back().get_widget());
   
   toolbar_main->tools().push_back(Gtk::Toolbar_Helpers::ToggleElem(_("Kill on new"), *manage(new Gtk::Image(Gtk::StockID("gtk-cut"), Gtk::ICON_SIZE_LARGE_TOOLBAR)), Gtk::Toolbar_Helpers::Callback0()));
   togglebutton_kill_on_new = static_cast<Gtk::ToggleButton *>(toolbar_main->tools().back().get_widget());
   button_quit->set_flags(Gtk::CAN_FOCUS);
   button_quit->grab_focus();
   togglebutton_repeat->set_active(false);
   togglebutton_kill_on_new->set_active(false);
   toolbar_main->set_tooltips(true);
   treeview_main->set_flags(Gtk::CAN_FOCUS);
   treeview_main->set_headers_visible(true);
   treeview_main->set_rules_hint(false);
   treeview_main->set_reorderable(false);
   treeview_main->set_enable_search(true);
   scrolledwindow1->set_flags(Gtk::CAN_FOCUS);
   scrolledwindow1->set_shadow_type(Gtk::SHADOW_NONE);
   scrolledwindow1->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   scrolledwindow1->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   scrolledwindow1->add(*treeview_main);
   scrolledwindow_playlist->set_flags(Gtk::CAN_FOCUS);
   scrolledwindow_playlist->set_shadow_type(Gtk::SHADOW_NONE);
   scrolledwindow_playlist->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
   scrolledwindow_playlist->property_window_placement().set_value(Gtk::CORNER_TOP_LEFT);
   hbox1->pack_start(*scrolledwindow1);
   hbox1->pack_start(*scrolledwindow_playlist);
   vbox2->pack_start(*toolbar_main, Gtk::PACK_SHRINK, 0);
   vbox2->pack_start(*hbox1);
   eventbox1->set_events(Gdk::EXPOSURE_MASK);
   eventbox1->add(*vbox2);
   main_window_RPS->set_events(Gdk::KEY_RELEASE_MASK);
   main_window_RPS->set_title(_("RolePlayingSound"));
   main_window_RPS->set_modal(false);
   main_window_RPS->property_window_position().set_value(Gtk::WIN_POS_NONE);
   main_window_RPS->set_resizable(true);
   main_window_RPS->property_destroy_with_parent().set_value(false);
   main_window_RPS->add(*eventbox1);
   toolbar_main->show();
   treeview_main->show();
   scrolledwindow1->show();
   scrolledwindow_playlist->show();
   hbox1->show();
   vbox2->show();
   eventbox1->show();
   main_window_RPS->show();
   button_quit->signal_clicked().connect(SigC::slot(*this, &main_window_RPS_glade::on_button_quit_clicked));
   togglebutton_repeat->signal_toggled().connect(SigC::slot(*this, &main_window_RPS_glade::on_togglebutton_repeat_toggled));
   togglebutton_kill_on_new->signal_toggled().connect(SigC::slot(*this, &main_window_RPS_glade::on_togglebutton_kill_on_new_toggled));
   treeview_main->signal_row_activated().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_row_activated));
   treeview_main->signal_cursor_changed().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_cursor_changed));
   treeview_main->signal_move_cursor().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_move_cursor));
#if 0
   treeview_main->signal_select_cursor_row().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_select_cursor_row));
   treeview_main->signal_toggle_cursor_row().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_toggle_cursor_row));
   treeview_main->signal_start_interactive_search().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_start_interactive_search));
#endif
   treeview_main->signal_button_release_event().connect(SigC::slot(*this, &main_window_RPS_glade::on_treeview_main_button_release_event), true);
}

main_window_RPS_glade::~main_window_RPS_glade()
{  delete gmm_data;
}
