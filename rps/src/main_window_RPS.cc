// generated 2003/9/22 11:37:15 CEST by thoma@Tiger.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to main_window_RPS.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "main_window_RPS.hh"

#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/treemodel.h>
#include <sys/wait.h>
#include <gdk/gdkkeysyms.h> 

main_window_RPS *main_window_RPS::self;

main_window_RPS::main_window_RPS(const std::string &m)
: rpgs(m)
{
   rpgs.getPlayList().SigPlaylistChanged().connect(
      SigC::slot(*this,&main_window_RPS::signal_playlist_cachanged));
   self=this;
   signal(SIGCHLD,&signalhandler);
   togglebutton_repeat->set_active(rpgs.getRepeat());
   togglebutton_kill_on_new->set_active(rpgs.getKillOnNew());
   fill_columns();
   
   get_window()->move(10,80);
   set_size_request(700,600);
}




void main_window_RPS::signalhandler(int signr)
{
  if ( signr== SIGCHLD )
    { int result;
      int pid = wait(&result);
//std::cout << "Signalhandler: remove  "<<pid<<"\t"<<result<<'\n';
      self->rpgs.remove_from_playlist(pid,false);
    }
//  signal(SIGCHLD,&signalhandler);
}

#if 0
bool main_window_RPS::on_eventbox_key_release_event(GdkEventKey *ev)
{
std::cout << "event\t"<<ev->keyval<<'\n';
   if (ev->keyval==GDK_F10) on_button_quit_clicked();
   treeview_main->grab_focus();
   return 1;
}
#endif

void main_window_RPS::fill_columns()
{
  //Create the Tree model: 
  treeview_main->remove_all_columns();
  m_refTreeModelSelect = Gtk::TreeStore::create(m_ColumnsSelect);
  treeview_main->set_model(m_refTreeModelSelect);
  treeview_main->columns_autosize();

  //Fill the TreeView's model
  for(RPGS::t_filemap::const_iterator i=rpgs.Filemap().begin();i!=rpgs.Filemap().end();++i)
   {
     Gtk::TreeModel::Row row = *(m_refTreeModelSelect->append());
     row[m_ColumnsSelect.path] = i->first;
     for(std::vector<soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
      {
        Gtk::TreeModel::Row childrow = *(m_refTreeModelSelect->append(row.children()));
        childrow[m_ColumnsSelect.path] = i->first;
        childrow[m_ColumnsSelect.file] = j->Name();
//        childrow[m_ColumnsSelect.is_played] = j->IsPlayedStr();
//        childrow[m_ColumnsSelect.is_played] = j->Played();
        childrow[m_ColumnsSelect.sound] = *j;
      }
   }  
  //Add the TreeView's view columns:
  treeview_main->append_column("Path", m_ColumnsSelect.path);
  treeview_main->append_column("Soundfile", m_ColumnsSelect.file);
//  treeview_main->append_column("plays", m_ColumnsSelect.is_played);
  treeview_main->queue_resize();
  treeview_main->set_enable_search(true);
  treeview_main->grab_focus();
}                    

void main_window_RPS::fill_playlist()
{
//std::cout << "Fill\n";
  treeview_playlist->remove_all_columns();
  m_refTreeModelPlayList = Gtk::TreeStore::create(m_ColumnsPlay);
  treeview_playlist->set_model(m_refTreeModelPlayList);
  //Fill the TreeView's model
  for(PlayList::const_iterator i=rpgs.getPlayList().begin();i!=rpgs.getPlayList().end();++i)
   {
     Gtk::TreeModel::Row row = *(m_refTreeModelPlayList->append());
     row[m_ColumnsPlay.repeat] = i->RepeatStr();
     row[m_ColumnsPlay.sound] = i->sound;
     row[m_ColumnsPlay.st_pl] = *i;
   }  
  //Add the TreeView's view columns:
  treeview_playlist->append_column("Rep.", m_ColumnsPlay.repeat);
  treeview_playlist->append_column("Sound", m_ColumnsPlay.sound);
}                    

void main_window_RPS::on_button_quit_clicked() 
{
   hide();
}

main_window_RPS::~main_window_RPS()
{
   rpgs.stop_playing();
}

bool main_window_RPS::on_treeview_main_button_release_event(GdkEventButton *ev)
{  
   entry_selected();
   return 0;
}

void main_window_RPS::entry_selected()
{
//std::cout << "entry_selected \n";
   Glib::RefPtr<Gtk::TreeSelection> sel = treeview_main->get_selection();
   Gtk::TreeModel::iterator iter = sel->get_selected();
   if(iter) //If anything is selected
    {
      Gtk::TreeModel::Row row = *iter;
      soundfile s = row[m_ColumnsSelect.sound];
//std::cout <<'\t'<< "\tSound: "<<s.Name()<<' '<<s.TypeStr()<<'\n';
      if(!s.Name().empty()) 
        {
         rpgs.play(s);
        }
    }
}

void main_window_RPS::playlist_entry_selected()
{
//std::cout << "entry_playlist_selected \n";
   Glib::RefPtr<Gtk::TreeSelection> sel = treeview_playlist->get_selection();
   Gtk::TreeModel::iterator iter = sel->get_selected();
   if(iter) //If anything is selected
    {
      Gtk::TreeModel::Row row = *iter;
      PlayList::st_playlist s = row[m_ColumnsPlay.st_pl];
      rpgs.remove_from_playlist(s.mpgpid,true);
    }
}

void main_window_RPS::on_treeview_main_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{  
   if(treeview_main->row_expanded(path)) treeview_main->collapse_row(path);
   else                                  treeview_main->expand_row(path,true);
   entry_selected();   
}

void main_window_RPS::on_treeview_main_cursor_changed()
{  
//   std::cout << "on_treeview_main_cursor_changed\n";
}

bool main_window_RPS::on_treeview_main_move_cursor(Gtk::MovementStep step, int count)
{  
//   std::cout << "on_treeview_main_move_cursor\n";
  return 0;
}

void main_window_RPS::on_treeview_main_select_cursor_row(bool start_editing)
{  
   std::cout << "on_treeview_main_select_cursor_row\n";
}

void main_window_RPS::on_treeview_main_toggle_cursor_row()
{  
   std::cout << "on_treeview_main_toggle_cursor_row\n";
}

void main_window_RPS::on_treeview_main_start_interactive_search()
{  
   std::cout << "on_treeview_main_start_interactive_search\n";
}


void main_window_RPS::on_togglebutton_repeat_toggled()
{
   rpgs.setRepeat(togglebutton_repeat->get_active());
}

void main_window_RPS::on_togglebutton_kill_on_new_toggled()
{
   rpgs.setKillOnNew(togglebutton_kill_on_new->get_active());
}


void main_window_RPS::on_treeview_playlist_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{  
   if(treeview_main->row_expanded(path))
      treeview_main->collapse_row(path);
   else treeview_main->expand_row(path,true);
   playlist_entry_selected();   
}

bool main_window_RPS::on_treeview_playlist_button_release_event(GdkEventButton *ev)
{  
   playlist_entry_selected();
   return 0;
}

