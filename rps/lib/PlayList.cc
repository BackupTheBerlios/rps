#include <PlayList.hh>
#include <iostream>
#include <signal.h>


bool PlayList::remove(const Soundfile &s, const bool kill_pids)
{
  for(iterator i=VP.begin();i!=VP.end();++i)
   { if(s==*i)                                                  
      {    
        if(kill_pids)
         {
std::cout << "kill "<<(*i).AsdPid()<<"\t"<<(*i).MpgPid()<<'\n';
           kill(i->AsdPid(),9);
           kill(i->MpgPid(),9);
         }
        i->stop_playing();
        VP.erase(i);
        sig_playlist_changed();
        
//#ifdef ASDSUPPORT
//        remove_asd_client();
//#endif
        return true;  
      }
   }
  return false;
}
