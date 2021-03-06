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
#include <rps.hh>

class main_window_RPS : public main_window_RPS_glade
{  
   public:
        static main_window_RPS *self_main_window_RPS;
   private:
        RPS rpgs;

        static void signalhandler(int signr);

        //Tree model columns:
        class ModelColumnsSound : public Gtk::TreeModel::ColumnRecord
         {
           public:

           Gtk::TreeModelColumn<std::string> col_name;
           Gtk::TreeModelColumn<std::string> col_time;
           Gtk::TreeModelColumn<Soundfile> sound;
           Gtk::TreeModelColumn<bool> is_cd;

           ModelColumnsSound() { add(col_name); add(col_time); add(sound); add(is_cd);}

          };
        ModelColumnsSound m_ColumnsSound;
        Glib::RefPtr<Gtk::TreeStore> m_refTreeModelSelect;

        void fill_columns();
        void fill_columns_recursive(const int level,Gtk::TreeModel::Row *prow=NULL,
                                    const std::string path="");
        void fill_soundfiles(const std::list<Soundfile> &VS,Gtk::TreeModel::Row &row);
        void fill_playlist();
        void entry_selected() ;
        void signal_playlist_cachanged() {fill_playlist();}
        void start_CD() ;

        std::vector<Soundfile> cd;
   public:
        main_window_RPS(const std::vector<std::string> &path);
        ~main_window_RPS();
      
        RPS &getRPS() {return rpgs;}

   private:
       bool on_main_window_RPS_delete_event(GdkEventAny *ev);

#if 1
#if 0
        bool on_eventbox_key_release_event(GdkEventKey *ev);
#endif
        void on_togglebutton_repeat_toggled();
        void on_button_quit_clicked();
        void on_button_stop_CD_clicked();
        void on_button_stop_now_clicked();
        void on_togglebutton_kill_on_new_toggled();
        void on_togglebutton_play_dir_toggled();
        void on_treeview_main_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
        void on_treeview_main_cursor_changed();
        bool on_treeview_main_button_release_event(GdkEventButton *ev);
        void on_treeview_main_row_expanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);


        bool on_treeview_main_move_cursor(Gtk::MovementStep step, int count);
        void on_treeview_main_select_cursor_row(bool start_editing);
        void on_treeview_main_toggle_cursor_row();
        void on_treeview_main_start_interactive_search();
#endif

};
#endif
