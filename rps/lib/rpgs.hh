#define ASDSUPPORT
#include <string>
#include <map>
#include <vector>

#include <sigc++/signal.h>
#ifdef ASDSUPPORT
extern "C" {
#include <asdlib.h>
}
#endif

class PlayList
{
   public:
      struct st_playlist{int mpgpid; int asdpid; std::string sound;
                         bool repeat;
             st_playlist() : 
                     mpgpid(0),asdpid(0),repeat(false){}
             st_playlist(int m,int a,const std::string s,bool r):
                     mpgpid(m),asdpid(a),sound(s),repeat(r){}

             std::string RepeatStr() const
               { if(repeat) return "(R)";
                 else return ""; }
            };

      
   private:
      std::vector<st_playlist> VP;
      SigC::Signal0<void> sig_playlist_changed;
   public:
      PlayList() {}
      SigC::Signal0<void> &SigPlaylistChanged() {return sig_playlist_changed;}
      void push_back(const st_playlist &p) {VP.push_back(p);sig_playlist_changed();}
      const std::vector<st_playlist>& getPlayList() const {return VP;}

      void remove(std::vector<st_playlist>::iterator i,bool fire) 
         { VP.erase(i);if(fire) sig_playlist_changed();}
      typedef std::vector<st_playlist>::const_iterator const_iterator;
      typedef std::vector<st_playlist>::iterator iterator;
      const_iterator begin() const{ return VP.begin(); }
      const_iterator end()   const{ return VP.end();   }
      iterator begin() { return VP.begin(); }
      iterator end()   { return VP.end();   }
      int size() const { return VP.size();  }

};

#if 0
#ifdef ASDSUPPORT
class ASD_List
{
   public:
      struct st_asd{int id; std::string name;
             st_asd(int i,const std::string &n): id(i),name(n) {}
             bool operator==(const st_asd &b) const 
                  {return id==b.id && name==b.name;}
               };
   private:
      std::vector<st_asd> VA;
   public:
      ASD_List() {}

      const std::vector<st_asd> &getList() const {return VA;}
      std::vector<st_asd> &getList()  {return VA;}
};
#endif
#endif
class soundfile
{
      enum etype{None,mp3,ogg,wav};

      std::string name,path,text;
//      bool is_played;
      etype type;
      int minutes, seconds;

      void soundfile::set_type(const std::string &n);
   public:
      soundfile() : type(None),minutes(0),seconds(0)/*,is_played(false)*/ {}
      soundfile(const std::string &p,const std::string &n) ;

      const std::string &Name() const {return name;}
      const std::string &Path() const {return path;}
      etype Type() const {return type;}
      std::string TypeStr() const;
#if 0
      std::string IsPlayedStr() const;
      const bool &Played() const {return is_played;}
#endif

      void play(const std::string &mainpath,const bool repeatbool,
               PlayList &pl) ; 
};



class RPGS
{
   public:
      typedef  std::map<std::string,std::vector<soundfile> > t_filemap;
   private:
      std::string mainpath;
      t_filemap filemap;
      bool repeat,kill_on_new;


      PlayList playlist;
      void looking_for_subpaths();
      void show_all_subdirs() const;
      void read_subdirs();

#ifdef ASDSUPPORT
      AsdConnection *asdcon;
      struct st_asd{int id; std::string name;
             st_asd(int i,const std::string &n): id(i),name(n) {}
                    bool operator==(const st_asd &b) const
                     {return id==b.id && name==b.name;}
                  };
      std::vector<st_asd> vec_asd_list;
#endif

   public:
      RPGS(const std::string &s);

      const t_filemap &Filemap()const{return filemap;}
      const std::string &MainPath() const {return mainpath;}
      const PlayList &getPlayList() const {return playlist;}
      PlayList &getPlayList() {return playlist;}
      void remove_from_playlist(const int pid,const bool kill_pids);
      void stop_playing();

      void setRepeat(const bool r) {repeat=r;}
      bool getRepeat() const {return repeat;}
      void setKillOnNew(const bool r) {kill_on_new=r;}
      bool getKillOnNew() const {return kill_on_new;}
      void play(soundfile &s) ;
};
