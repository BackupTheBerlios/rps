#include <soundfile.hh>
#include <unistd.h>
#include <errno.h>
#include <iostream>

void Soundfile::play(const bool repeatbool)
{
   repeat=repeatbool;

   int fd[2];
   if (pipe(fd)) { perror("pipe"); return; }
   if (!(mpgpid=fork()))
    {close(fd[0]); 
     dup2(fd[1],1);
     close(fd[1]); 
     execl("/usr/bin/mpg123",  "mpg123","-sq","-b10240",
          repeat?"-Z":"-q",Filename().c_str(),0);
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
   is_played=true;
}

void Soundfile::stop_playing()
{
   is_played=false;
   repeat=false;
   asdi=asd_sound_identifier();
}

std::string Soundfile::RepeatStr() const
{ 
   if(repeat) return "(R)"; 
   else       return "";
}

std::string Soundfile::IsPlayedStr() const
{
   if(is_played) return "(P)";
   else return "";
}


#if ASDSUPPORT
std::string itos(int i)
{
 char buffer[100];
 snprintf(buffer,sizeof buffer,"%i",i);
 return(buffer);
}
   
   

gchar* Soundfile::prozent_to_asd(const int p)
{
   int i = int(65535./100*p);
   std::string s=itos(i)+","+itos(i);
   return const_cast<gchar*>(s.c_str());
}

void Soundfile::set_volume(const int v)
{
   default_volume=v;
   Volume volume;
   volume_parse(&volume, prozent_to_asd(v));
   AsdConnection *asdcon = asd_connection_new(NULL) ;
   if (!asd_volume_set(asdcon,  asdi.getASDI(), volume))
     perror("Could not set device volume");
}

int Soundfile::get_volume() const
{
  Volume volume;
//std::cerr << "get volume for "<<asdi.getASDI()<<'\n';
  AsdConnection *asdcon = asd_connection_new(NULL) ;
//  static int count=0;
  if (!asd_volume_get(asdcon, const_cast<gchar*>(asdi.getASDI()) , &volume))
   {
//    ++count;
    std::cerr << "Could not get device volume for "<<asdi.getASDI()
//              <<" count="<<count
                <<'\n';
//    if(count<=100) get_volume();
    return 100;
   }
  else
  {
//    count=0;
    long int s=0;
    for (int i=0;i<ASD_CHANNELS_MAX;++i) s+=volume.factor[i];
    int v =  int(s/65535./ASD_CHANNELS_MAX*100.);
//std::cout << "summe="<<s<<'\t'<< v<<'\n';
    return v;
  }                                        
 return -1;
}

#endif


/****************************************************************************/
#include <vector>
void SoundfileBase::set_subpath()
{
   std::string::size_type st = path.find_last_of("/");
   if(st == std::string::npos)
     std::cerr << "Strange error while lookling for subpath in "<<path<<'\n';
   else
     {
       subpath=path.substr(st+1,std::string::npos);
#if 0
       std::string sp = path.substr(0,st);
       st = sp.find_last_of("/");
       if(st == std::string::npos)
          std::cerr << "Strange error while lookling for subpath in "<<path<<'\n';
       else
          parentpath = sp.substr(st+1,std::string::npos);
//std::cout << Name()<<'\t'<<subpath<<' '<<parentpath <<' '<<<<'\n';
#endif
     }
}

void SoundfileBase::set_type(const std::string &n)
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
         stype= i->second;
         return;
       }
    }
}

void SoundfileBase::setTime(const int m, const int s) 
{
   minutes=m; 
   seconds=s;
   if(s>=10) time = itos(m)+":"+itos(s);
   else     time = itos(m)+":0"+itos(s);
}


void SoundfileBase::setTime(const std::string &_time) 
{
   std::string smin,ssec;
   if(_time.find_last_of("=>")!=std::string::npos && 
      _time.find_last_of(":")!=std::string::npos  &&
      _time.find_last_of("\n")!=std::string::npos)
     {
       smin=_time.substr(_time.find_last_of("=>")+1,_time.find(":")-_time.find_last_of("=>")-1);
       ssec=_time.substr(_time.find(":")+1,_time.find("\n")-_time.find(":")-1);
     }
   else if(_time.find_last_of(":")!=std::string::npos)
     {
       smin=_time.substr(0,_time.find(":"));
       ssec=_time.substr(_time.find(":")+1,std::string::npos);
#if 0
std::cout<<"TXX"<< _time<<"XX\t"<<smin<<'\t'<<ssec<<'\n';
#endif
     }
   else
     {
       std::cout << "unknown time\n";
       return;
     }
    int min = atoi(smin.c_str());
    int sec = atoi(ssec.c_str());
    setTime(min,sec);
}



std::string SoundfileBase::TypeStr() const
{
   if(type==None) return "unkown";
   if(type==mp3)  return "mp3";
   if(type==wav)  return "wav";
   if(type==ogg)  return "ogg";
}
