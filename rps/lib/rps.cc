#include <rps.hh>

#include <iostream>

#if ASDSUPPORT
   RPS *RPS::self;
#endif


RPS::RPS(const std::string &s) 
: repeat(false),kill_on_new(false)
{
#if ASDSUPPORT
   self=this;  
   if (!(asd_connection_new(NULL)))
    {
      std::cout << "Could not create connection, please start asd\n";
      exit (1);
    }
#endif
   filelist=FileList(s);
}

#if ASDSUPPORT

void RPS::remove_asd_client(const asd_sound_identifier &ain)
{
   if(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain)!=vec_asd_list.end())
     {
       vec_asd_list.erase(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain));
     }
}


void RPS::new_asd_client(const std::string &name,Soundfile *s)
{
   asd_sound_identifier ain(name);
   if(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain)==vec_asd_list.end())
     {
       vec_asd_list.push_back(ain);
       s->setClient(ain);
       s->set_volume(filelist.get_default_volume(*s));
     }
}


void _new_asd_client(ProtocolAsdListResponse* response, gpointer userdata)
{ 
  g_assert(response);
//std::cout << response->shortname<<'\n';
  if(std::string(response->type)=="SOCKET")
   {
//std::cout << "Socket :"<<response->shortname<<'\n';
     Soundfile *s=static_cast<Soundfile*>(userdata);   
     RPS::self->new_asd_client(response->shortname,s);
   }
}
#endif


void RPS::play(Soundfile &s)
{
//std::cout << "start play " <<kill_on_new<<' '<<repeat<<'\t'
//<<s.DefaultVolume()<<'\n';
   if(kill_on_new) stop_playing();

   s.play(getFileList().MainPath(),repeat);
#ifdef ASDSUPPORT
   AsdConnection *asdcon = asd_connection_new(NULL) ;
   if (!asdcon)
     { perror("Could not create connection");
       exit (1);
     }  
   asd_list_sources(asdcon, _new_asd_client, &s);
#endif
   playlist.push_back(s);
}

void RPS::play(const std::string &cd) 
{
   PSL=filelist.get_cd_file_list(cd);
   play(PSL.front());
   PSL.pop_front();
}


void RPS::remove_from_playlist(const int pid) 
{
   for(PlayList::iterator i=playlist.begin();i!=playlist.end();++i)
     { 
       if(pid==i->MpgPid() ) std::cout << "mpg-pid: "<<pid<<'\n';
       if(pid==i->AsdPid() ) std::cout << "mpg-pid: "<<pid<<'\n';
       if(pid==i->MpgPid() || pid==i->AsdPid() )
        {
         remove_from_playlist(*i,false);
         break;
        }
     }
}
       
void RPS::remove_from_playlist(const Soundfile &s, const bool kill_pids)
{
   playlist.remove(s,kill_pids);
   remove_asd_client(s.getASDI());
   if(!PSL.empty())  { play(PSL.front()); PSL.pop_front();}
}

void RPS::stop_playing()
{
reloop:
 for(PlayList::iterator i=playlist.begin();i!=playlist.end();++i)
   {
      remove_from_playlist(*i,true);
      goto reloop;
   }  
}
