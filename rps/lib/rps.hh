#ifndef RPS_LIB_HH
#define RPS_LIB_HH 

#include <PlayList.hh>
#include <FileList.hh>

#include <../config.h>

#if ASDSUPPORT
#include <asd_sound.hh>
#endif

class RPS
{
   private:
      FileList filelist;
      PlayList playlist;

      bool repeat,kill_on_new;

      void stop_playing();

   public:
      RPS(const std::string &s);
      ~RPS() {stop_playing();}

      const PlayList &getPlayList() const {return playlist;}
      PlayList &getPlayList() {return playlist;}
      const FileList &getFileList() const {return filelist;}
      FileList &getFileList() {return filelist;}
      

      void play(Soundfile &s) ;

      void remove_from_playlist(const int pid) ;
      void remove_from_playlist(const Soundfile &s,const bool kill_pids);


      void setRepeat(const bool r) {repeat=r;}
      bool getRepeat() const {return repeat;}
      void setKillOnNew(const bool r) {kill_on_new=r;}
      bool getKillOnNew() const {return kill_on_new;}

#if ASDSUPPORT
   private:
      std::vector<asd_sound_identifier> vec_asd_list;
      void remove_asd_client(const asd_sound_identifier &ain);
   public:
      static RPS *self;
      void new_asd_client(const std::string &s,Soundfile *s);
      
#endif

};

#endif
