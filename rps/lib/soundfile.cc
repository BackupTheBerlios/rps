#include <soundfile.hh>
#include <unistd.h>
#include <errno.h>
#include <Play.hh>

#include <iostream>
void Soundfile::play(const std::string &mainpath,const bool repeatbool)
{
   Play P(*this,repeatbool);
   mpgpid = P.MpgPid(); 
   asdpid = P.AsdPid();
   is_played=true;
   repeat=repeatbool;
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
void SoundfileBase::set_subpath()
{
   std::string::size_type st = path.find_last_of("/");
   if(st == std::string::npos)
     std::cerr << "Strange error while lookling for subpath in "<<path<<'\n';
   else
     {
       subpath=path.substr(st+1,std::string::npos);
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
