#define WITHSIGNAL

// generated 2003/9/22 11:37:15 CEST by thoma@Tiger.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to main_window_RPS.hh_new

// you might replace
//    class foo : public foo_glade { ... };
// by
//    typedef foo_glade foo;
// if you didn't make any modifications to the widget

#ifndef _MAIN_WINDOW_RPS_HH
#  include "main_window_RPS_glade.hh"
#  define _MAIN_WINDOW_RPS_HH

#include <gtkmm/treestore.h>
#include <signal.h>
#include "../lib/rpgs.hh"


class main_window_RPS : public main_window_RPS_glade
{  
        
   private:
        bool block_fill_playlist;
        RPGS rpgs;

        static void signalhandler(int signr);
        static main_window_RPS *self;

        //Tree model columns:
        class ModelColumnsSelect : public Gtk::TreeModel::ColumnRecord
         {
           public:

           Gtk::TreeModelColumn<std::string> path;
           Gtk::TreeModelColumn<std::string> file;
#if 0
           Gtk::TreeModelColumn<std::string> is_played;
           Gtk::TreeModelColumn<bool> is_played;
#endif
           Gtk::TreeModelColumn<soundfile> sound;

//           ModelColumnsSelect() { add(path); add(file); add(is_played); add(sound); }
           ModelColumnsSelect() { add(path); add(file); add(sound); }
          };
        class ModelColumnsPlay : public Gtk::TreeModel::ColumnRecord
         {
           public:

           Gtk::TreeModelColumn<std::string> repeat;
           Gtk::TreeModelColumn<std::string> sound;
           Gtk::TreeModelColumn<PlayList::st_playlist> st_pl;

           ModelColumnsPlay() { add(repeat); add(sound); add(st_pl); }
          };

        ModelColumnsSelect m_ColumnsSelect;
        ModelColumnsPlay m_ColumnsPlay;
        Glib::RefPtr<Gtk::TreeStore> m_refTreeModelSelect;
        Glib::RefPtr<Gtk::TreeStore> m_refTreeModelPlayList;

        void fill_columns();
        void fill_playlist();
        void entry_selected() ;
        void playlist_entry_selected();
        void signal_playlist_cachanged() {fill_playlist();}

   public:
        main_window_RPS(const std::string &m);
        ~main_window_RPS();

   private:
#if 1
#if 0
        bool on_eventbox_key_release_event(GdkEventKey *ev);
#endif
        void on_togglebutton_repeat_toggled();
        void on_button_quit_clicked();
        void on_togglebutton_kill_on_new_toggled();
        void on_treeview_main_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void on_treeview_main_cursor_changed();
        bool on_treeview_main_button_release_event(GdkEventButton *ev);

        bool on_treeview_main_move_cursor(Gtk::MovementStep step, int count);
        void on_treeview_main_select_cursor_row(bool start_editing);
        void on_treeview_main_toggle_cursor_row();
        void on_treeview_main_start_interactive_search();
#endif
        void on_treeview_playlist_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        bool on_treeview_playlist_button_release_event(GdkEventButton *ev);


};
#endif
