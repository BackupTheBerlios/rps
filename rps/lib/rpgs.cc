#include "rpgs.hh"


#include <dirent.h>
#include<iostream>
#include <sys/stat.h>
#include <signal.h>

void RPGS::looking_for_subpaths()
{
     DIR *dir=opendir(mainpath.c_str());
     if(!dir) { std::cerr << "\n directory "<<mainpath<<" not found\n"; 
                exit(1);}
     struct stat sa;
     while(true)
      {
        dirent *ent=readdir(dir);
        if(ent==NULL) break;
        stat((mainpath+"/"+ent->d_name).c_str(),&sa);
        if( sa.st_mode & S_IFREG ) continue;
        if( std::string(ent->d_name).find(".")==0) continue;
           filemap[ent->d_name];
      }
}

void RPGS::show_all_subdirs() const
{
//   for(std::vector<std::string>::const_iterator i=paths.begin();i!=paths.end();++i)
   for(t_filemap::const_iterator i=filemap.begin();i!=filemap.end();++i)
    {
      std::cout << i->first<<'\n';
      for(std::vector<soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
        std::cout << '\t'<<j->Path()<<'\t'<<j->Name()<<'\t'<<j->TypeStr()<<'\n';
    }
}

void RPGS::read_subdirs()
{
   std::vector<t_filemap::iterator> nodirs;
   for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
    {
      DIR *dir=opendir((mainpath+"/"+i->first).c_str());
//      if(!dir) {nodirs.push_back(i); continue;}
      if(!dir) continue;
      while(true)
       {
        dirent *ent=readdir(dir);
        if(ent==NULL) break;
        if( std::string(ent->d_name).find(".")!=0)
           i->second.push_back(soundfile(i->first,ent->d_name));
       }    
    }
}

RPGS::RPGS(const std::string &s) 
: mainpath(s),repeat(false),kill_on_new(false)
{
   looking_for_subpaths();
   read_subdirs();
   show_all_subdirs();
#ifdef ASDSUPPORT
   if (!(asdcon = asd_connection_new(NULL)))
    {
      perror("Could not create connection");
      exit (1);
    }
#endif
}

void RPGS::play(soundfile &s)
{
   if(kill_on_new) stop_playing();
   s.play(mainpath,repeat,playlist);

#ifdef ASDSUPPORT
   AsdListProc* response;
   asd_list_sources(asdcon, *response, NULL);
//   response->ProtocolAsdListResponse;
//std::cout << response->shortname<<'\t'<< response->name<<'\t'<< response->type<<'\n';
   int id;
   std::string name;
   vec_asd_list.push_back(st_asd(id,name));
#endif
}

void RPGS::stop_playing()
{
//std::cout << "Stop playing:\t"<<playlist.size()<<'\t';
//std::cout << '\t'<<(playlist.begin()!=playlist.end())<<'\n';
//return;
 for(PlayList::iterator i=playlist.begin();i!=playlist.end();++i)
   { 
      if(!playlist.size())return;
//std::cout << "kill "<<(*i).asdpid<<"\t"<<(*i).mpgpid<<'\n';
      kill(i->asdpid,9);
      kill(i->mpgpid,9);
      playlist.remove(i,false);
      stop_playing();
//std::cout << "killed success\n";
   }   
}


void RPGS::remove_from_playlist(const int pid,const bool kill_pids)
{
 for(PlayList::iterator i=playlist.begin();i!=playlist.end();++i)
   { if(pid==i->mpgpid || pid==i->asdpid )
      { 
        if(kill_pids)
         {
//std::cout << "kill "<<(*i).asdpid<<"\t"<<(*i).mpgpid<<'\n';
           kill(i->asdpid,9);
           kill(i->mpgpid,9);
         }
        playlist.remove(i,true);
        return;  }}   
}

///////////////////////////////////////////////////////////////////////7
soundfile::soundfile(const std::string &p,const std::string &n) 
: path(p),name(n),type(None),minutes(0),seconds(0)//,is_played(false)
{
   set_type(name);
}

void soundfile::play(const std::string &mainpath,const bool repeatbool,
                     PlayList &pl)
{
   std::string file = mainpath+"/"+path+"/"+name+".mp3";

   int fd[2];
   if (pipe(fd)) { perror("pipe"); return; }
   int asdpid,mpgpid;   
   if (!(mpgpid=fork()))
    {close(fd[0]); 
     dup2(fd[1],1);
     close(fd[1]);
     execl("/usr/bin/mpg123",  "mpg123","-sq","-b10240",
          repeatbool?"-Z":"-q",file.c_str(),0);
     perror("/usr/bin/mpg123");
     _exit(errno);
    }
   if (!(asdpid=fork()))
    {close(fd[1]);
     dup2(fd[0],0);
     close(fd[0]); 
     execl("/usr/bin/asdcat",  "asdcat",0);
     perror("/usr/bin/asdcat");
     _exit(errno);
    }
   close(fd[0]); close(fd[1]);
std::cout << " play "<<file<<'\t' << mpgpid<<'\t'<<asdpid<<'\n';  
//   is_played=true;
   pl.push_back(PlayList::st_playlist(mpgpid,asdpid,name,repeatbool));
}

void soundfile::set_type(const std::string &n)
{
   std::vector<std::pair<etype,std::string> > pt;
   pt.push_back(std::pair<etype,std::string>(mp3,".mp3") );
   pt.push_back(std::pair<etype,std::string>(ogg,".ogg") );
   pt.push_back(std::pair<etype,std::string>(wav,".wav") );

   for(std::vector<std::pair<etype,std::string> >::const_iterator i=pt.begin();i!=pt.end();++i)
    {
      if(n.rfind(i->second)!=std::string::npos)
       {
         name = n.substr(0,n.rfind(i->second));
         type = i->first;
         return;
       }
    }
}


std::string soundfile::TypeStr() const
{
   if(type==None) return "unkown";
   if(type==mp3)  return "mp3";
   if(type==wav)  return "wav";
   if(type==ogg)  return "ogg";
}

#if 0
std::string soundfile::IsPlayedStr() const
{
std::cerr << "play asked\n";
   if(is_played) return "(P)";
   else return "";
}
#endif
