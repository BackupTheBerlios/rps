#include <rps.hh>

#include <iostream>

#if ASDSUPPORT
   RPS *RPS::self;
#endif


RPS::RPS(const std::vector<std::string> &newpath) 
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

   std::vector<std::string> path;
   load_conf(path);
   if(path.empty() && newpath.empty()) 
     { std::cerr << " Unknown paths, please append valid paths to the\n"
                    " comand line or edit $HOME/.rps.rc\n";
       exit(1);
     }
   if(!newpath.empty()) path=newpath;
   filelist=FileList(path);
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
   static int count=0;   
//std::cout << ain.getASDI()<<'\t'<<name<<'\t';
   if(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain)==vec_asd_list.end())
     {
       vec_asd_list.push_back(ain);
//std::cout << "FOUND\n";
       s->setClient(ain);
       s->set_volume(filelist.get_default_volume(*s));
//      std::cout << " NEW found at try:\t"<<count<<'\n';
      has_found_new_socket=true;
      count=0;
     }
   if(!has_found_new_socket)
    {
//      std::cout << "NOT found count:\t"<<++count<<'\n';
      if(count>100) {std::cout << "I give up\n";  count=0; }
    }
}


void _new_asd_client(ProtocolAsdListResponse* response, gpointer userdata)
{ 
  g_assert(response);
//std::cout << response->shortname<<'\n';
  if(std::string(response->type)=="SOCKET")
   {
//std::cout << "SOCKET :"<<response->shortname<<'\n';
     Soundfile *s=static_cast<Soundfile*>(userdata);   
     RPS::self->new_asd_client(response->shortname,s);
   }
}

void RPS::try_to_get_new_client(Soundfile &s) 
{
   AsdConnection *asdcon = asd_connection_new(NULL) ;
   if (!asdcon)
     { perror("Could not create connection");
       exit (1);
     }  
   asd_list_sources(asdcon, _new_asd_client, &s);  // ASD_LIB function
}
#endif


void RPS::play(Soundfile &s)
{
//std::cout << "start play " <<kill_on_new<<' '<<repeat<<'\t'
//<<s.DefaultVolume()<<'\n';
   if(kill_on_new) stop_playing();

   s.play(repeat);
#ifdef ASDSUPPORT
   has_found_new_socket=false;
   while(true)
    {
      try_to_get_new_client(s);
      if(has_found_new_socket) break;
    }
#endif
   playlist.push_back(s);
}



void RPS::play(const std::string &cd) 
{
   PSL=filelist.get_cd_file_list(cd);
   if(PSL.empty()) return;
   play(PSL.front());
//   PSL.pop_front();
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
   if(!PSL.empty() && PSL.front()==s)  
      {  PSL.pop_front();  
         if(!PSL.empty())  play(PSL.front());
      }
}

void RPS::stop_playing()
{
 PSL.clear();
reloop:
 for(PlayList::iterator i=playlist.begin();i!=playlist.end();++i)
   {
      remove_from_playlist(*i,true);
      goto reloop;
   }  
}


#include <fstream>
void RPS::save_conf() const
{
   std::string sname=getenv("HOME")+std::string("/.rps.rc");
   std::ofstream fo(sname.c_str());
   const std::vector<std::string> &mp=filelist.MainPath();
   std::cout << "Saving "<<sname<<'\t'<<mp.size()<<'\n';
   for(std::vector<std::string>::const_iterator i=mp.begin();i!=mp.end();++i)
      fo << "Path="<<*i<<'\n';
}

void RPS::load_conf(std::vector<std::string> &path) const
{
   std::string sname=getenv("HOME")+std::string("/.rps.rc");
   std::ifstream fi(sname.c_str());
   if(!fi.good()) { std::cout << sname <<" not found\n";
                    return; }
   std::cout << "Loading "<<sname<<'\n';
   while(true)
     {
       std::string search_string="Path=";
       std::string line;
       std::getline(fi,line);
//std::cout << line<<'\n';
       if(!fi.good()) break; 
//std::cout << "Found path "<<line<<'\n';
       std::string::size_type _a = line.find(search_string);
       if(_a!=std::string::npos) 
         path.push_back(line.substr(search_string.size(),std::string::npos));
     }                            
}
