#include <soundfile.hh>
#include <unistd.h>
#include <errno.h>

#include <iostream>
void Soundfile::play(const std::string &mainpath,const bool repeatbool)
{
   std::string file = mainpath+Filename();

   int fd[2];
   if (pipe(fd)) { perror("pipe"); return; }
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
   is_played=true;
   repeat=repeatbool;
//std::cout << " play "<<file<<'\t' << mpgpid<<'\t'<<asdpid<<'\t'<<repeat<<'\n';
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
  if (!asd_volume_get(asdcon, const_cast<gchar*>(asdi.getASDI()) , &volume))
   {
    std::cerr << "Could not get device volume for "<<asdi.getASDI()<<'\n';
    return 99;
   }
  else
  {
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


std::string SoundfileBase::TypeStr() const
{
   if(type==None) return "unkown";
   if(type==mp3)  return "mp3";
   if(type==wav)  return "wav";
   if(type==ogg)  return "ogg";
}
