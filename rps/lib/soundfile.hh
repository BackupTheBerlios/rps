#ifndef SOUNDFILE_HH
#define SOUNDFILE_HH
#include <string>
#include <../config.h>
//#include <time.h>

#if ASDSUPPORT
#include <asd_sound.hh>
#endif


#include <iostream>
class SoundfileBase
{
      friend class Soundfile;
      enum etype{None,mp3,ogg,wav};

      std::string name,stype,path,subpath/*,parentpath*/,text,time;
      etype type;
      int minutes, seconds;
      int default_volume;

      void set_type(const std::string &n);
      void set_subpath();
   public:
      SoundfileBase() : type(None),minutes(0),seconds(0),default_volume(100) {}
      SoundfileBase(const std::string &p,const std::string &n)
         : path(p),name(n),type(None),minutes(0),seconds(0),default_volume(100) 
            { set_subpath(); set_type(name); }

      void setTime(const int m, const int s) ;
      void setTime(const std::string &_time) ;
      const std::string &Time() const {return time;}
      long int Seconds() const {return minutes*60+seconds;}
      
      std::string Filename() const { return "/"+Path()+"/"+Name()+"."+TypeStr();}
      const std::string &Name() const {return name;}
      const std::string &Path() const {return path;}
      const std::string &SubPath() const {return subpath;}
//      const std::string &ParentPath() const {return parentpath;}
      etype Type() const {return type;}
      std::string TypeStr() const;
      int DefaultVolume() const {return default_volume;}
      void setDefaultVolume(const int d) {default_volume=d;}

      bool operator==(const SoundfileBase &b) const
          { return  Name()==b.Name() && Type()==b.Type() && Path()==b.Path(); }

      bool operator<(const SoundfileBase &b) const
            {return Name()<b.Name() ;}            
};

class Soundfile : public SoundfileBase
{
      bool is_played, repeat;
      int mpgpid, asdpid;

   public:
      Soundfile() : is_played(false),repeat(false),mpgpid(0),asdpid(0) {}
      Soundfile(const std::string &p,const std::string &n)
         : SoundfileBase(p,n), is_played(false),repeat(false) {}

      std::string IsPlayedStr() const;
      std::string RepeatStr() const;
      bool Played() const {return is_played;}

      void play(const bool repeatbool) ; 
      void stop_playing();
      int MpgPid() const {return mpgpid;}
      int AsdPid() const {return asdpid;}

      bool operator==(const Soundfile &b) const
           {return SoundfileBase::operator==(b) && MpgPid()==b.MpgPid() ;}


#if ASDSUPPORT
   private:
      asd_sound_identifier asdi;
      gchar* prozent_to_asd(const int p);
   public:
      asd_sound_identifier getASDI() const {return asdi;}

      std::string getClient() const {return asdi.getASDI();}
      void setClient(const asd_sound_identifier &a) {asdi=a;}
      void set_volume(const int v);
      int get_volume() const;
#endif
      
};

#endif
