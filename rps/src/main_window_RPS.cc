// generated 2003/9/22 11:37:15 CEST by thoma@Tiger.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to main_window_RPS.cc_new

// This file is for your program, I won't touch it again!

#include "config.h"
#include "main_window_RPS.hh"

#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/treemodel.h>
#include <sys/wait.h>
#include <gdk/gdkkeysyms.h> 
#include <sound_widget.hh>

main_window_RPS *main_window_RPS::self_main_window_RPS;

main_window_RPS::main_window_RPS(const std::string &m)
: rpgs(m)
{
   rpgs.getPlayList().SigPlaylistChanged().connect(
      SigC::slot(*this,&main_window_RPS::signal_playlist_cachanged));
   self_main_window_RPS=this;
   signal(SIGCHLD,&signalhandler);
   fill_columns();
   togglebutton_repeat->set_active(rpgs.getRepeat());
   togglebutton_kill_on_new->set_active(rpgs.getKillOnNew());
   togglebutton_play_dir->set_active(true);
   
   get_window()->move(50,80);
   set_size_request(800,700);
}


void main_window_RPS::signalhandler(int signr)
{
  if ( signr== SIGCHLD )
    { int result;
      int pid = wait(&result);
#if 0
std::cout << "Signalhandler: remove  "<<pid<<"\t"<<result<<'\n';
std::cout << "  "<<WIFEXITED(result)<<' '<<WEXITSTATUS(result)<<' '
          << WIFSIGNALED(result)<<' '<<WTERMSIG(result)<<' '
          << WIFSTOPPED(result)<< ' '<<WSTOPSIG(result)<<'\n';
#endif
      if(WIFEXITED(result)) self_main_window_RPS->rpgs.remove_from_playlist(pid);
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


void main_window_RPS::fill_columns_recursive(const int level,
                                             Gtk::TreeModel::Row *prow,
                                             const std::string parentpath)
{
  Gtk::TreeModel::Row row;
  for(FileList::const_iterator i=rpgs.getFileList().begin();i!=rpgs.getFileList().end();++i)
   {
     if(i->first.sub_level == level) 
      {
        if( !parentpath.empty() && parentpath!=i->first.subpath ) continue;

        if(prow==NULL) row = *(m_refTreeModelSelect->append()); 
        else row = *(m_refTreeModelSelect->append(prow->children()));

        row[m_ColumnsSound.col_name] = i->first.subpath;

        if(i->first.path.find("/CDs/") == std::string::npos)
             row[m_ColumnsSound.is_cd] = false;
        else row[m_ColumnsSound.is_cd] = true;

        fill_soundfiles(i->second,row);
      }
     else if(i->first.sub_level == level+1)
      {
       fill_columns_recursive(level+1,&row,i->first.subpath);       
      }
   }
}


void main_window_RPS::fill_columns()
{
  //Create the Tree model: 
  treeview_main->remove_all_columns();
  m_refTreeModelSelect = Gtk::TreeStore::create(m_ColumnsSound);
  treeview_main->set_model(m_refTreeModelSelect);
  treeview_main->columns_autosize();

  fill_columns_recursive(1);

  //Add the TreeView's view columns:
  treeview_main->append_column("Sound", m_ColumnsSound.col_name);
#if 0
  treeview_main->set_spacing(0);
#endif
  treeview_main->append_column("Time", m_ColumnsSound.col_time);
  treeview_main->queue_resize();
  treeview_main->set_enable_search(true);
  treeview_main->grab_focus();
}                    


void main_window_RPS::fill_soundfiles(const std::list<Soundfile> &VS,Gtk::TreeModel::Row &row)
{
   for(std::list<Soundfile>::const_iterator j=VS.begin();j!=VS.end();++j)
    {
      Gtk::TreeModel::Row childrow = *(m_refTreeModelSelect->append(row.children()));
      childrow[m_ColumnsSound.col_name] = j->Name();
      childrow[m_ColumnsSound.col_time] = j->Time();
      childrow[m_ColumnsSound.sound] = *j;
    }
}

void main_window_RPS::fill_playlist()
{
  scrolledwindow_playlist->remove();
  Gtk::VBox *box = manage(new class Gtk::VBox());
  for(PlayList::iterator i=rpgs.getPlayList().begin();i!=rpgs.getPlayList().end();++i)
   {
      sound_widget *sw =  manage(new class sound_widget(*i));
      box->pack_start(*sw,false,false);
   }
  box->show_all();
  scrolledwindow_playlist->add(*box);
}                    

void main_window_RPS::on_button_quit_clicked() 
{
   hide();
}

main_window_RPS::~main_window_RPS()
{
//   rpgs.stop_playing();
}

bool main_window_RPS::on_main_window_RPS_delete_event(GdkEventAny *ev)
{  
   std::cout << "delete event\n";
   return false;
}

void main_window_RPS::on_togglebutton_play_dir_toggled()
{
}


bool main_window_RPS::on_treeview_main_button_release_event(GdkEventButton *ev)
{  
   entry_selected();
   return 0;
}

void main_window_RPS::entry_selected()
{
   Glib::RefPtr<Gtk::TreeSelection> sel = treeview_main->get_selection();
   Gtk::TreeModel::iterator iter = sel->get_selected();
   if(iter) //If anything is selected
    {
      Gtk::TreeModel::Row row = *iter;
      Soundfile s = row[m_ColumnsSound.sound];
      if(!s.Name().empty())   rpgs.play(s);
    }
}

void main_window_RPS::start_CD() 
{
   if(!togglebutton_play_dir->get_active()) return;
   Glib::RefPtr<Gtk::TreeSelection> sel = treeview_main->get_selection();
   Gtk::TreeModel::iterator iter = sel->get_selected();
   if(iter) //If anything is selected
    {
      Gtk::TreeModel::Row row = *iter;
      bool is_cd = row[m_ColumnsSound.is_cd];
      if(!is_cd) return;
      std::string col = row[m_ColumnsSound.col_name];
      rpgs.play(col);
    }
}

void main_window_RPS::on_treeview_main_row_expanded(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path)
{
   start_CD();
}



void main_window_RPS::on_treeview_main_row_activated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column)
{  
   if(treeview_main->row_expanded(path)) treeview_main->collapse_row(path);
   else                                  treeview_main->expand_row(path,false);
//std::cout << path.get_depth()<<'\t'<<path.size()<<'\n';
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


void main_window_RPS::on_button_stop_CD_clicked()
{
   rpgs.stop_CD();
}
