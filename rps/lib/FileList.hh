#include <map>
#include <string>
#include <soundfile.hh>
#include <list>
#include <vector>
#include <assert.h>
#include <sigc++/signal.h>


class FileListCache
{
      friend class FileList;
   public:
      struct st_key{std::string name; time_t file_time;
             st_key(const std::string &n,const  time_t t)
               : name(n),file_time(t) {}
             bool operator<(const st_key &b) const
               { return name<b.name || name==b.name && file_time<b.file_time;}
             bool operator==(const st_key &b) const
               { return name==b.name && file_time<b.file_time;}
             };
      struct st_cache{ std::string time; int default_volume; 
             st_cache() :default_volume(0) {}
             st_cache(const std::string &t, const int d)
                  : time(t),default_volume(d) {}
             };
   private:
      ~FileListCache() { save(); }
      typedef std::map<st_key,st_cache> t_cachemap;
      t_cachemap CM;

   public:
      void load();
      void save() const;

      const std::map<st_key,st_cache> &getCache() const {return CM;}   
      void push_back(const st_key &k,const st_cache &c) {CM[k]=c;}

      typedef t_cachemap::const_iterator const_iterator;
      typedef t_cachemap::iterator iterator;
      const_iterator begin() const{ return CM.begin(); }  
      const_iterator end()   const{ return CM.end();   }  
      iterator begin() { return CM.begin(); }
      iterator end()   { return CM.end();   }

};


class FileList
{
//      SigC::Signal0<void> sig_playlist_changed;
   private:
      struct st_key{std::string path; std::string subpath; 
                    std::string parentpath; int sub_level;
             st_key(const std::string &p,int s) : path(p), sub_level(s) 
               {
                 std::string::size_type st = path.find_last_of("/");
                 if(st == std::string::npos) assert("impossible path\n");
                 subpath=path.substr(st+1,std::string::npos);           
                 std::string sp = path.substr(0,st);
                 st = sp.find_last_of("/"); 
                 if(st == std::string::npos) assert("impossible parpath\n");
                 parentpath = sp.substr(st+1,std::string::npos);                        
               }
             bool operator<(const st_key &b) const 
               {return path<b.path || path==b.path && sub_level<b.sub_level;}
             std::string ParentPath() const {return parentpath;}
             };
      typedef std::map<st_key,std::list<Soundfile> > t_filemap;
      t_filemap filemap;
      std::vector<std::string> mainpath;

//      void looking_for_subpaths();
      void read_subdirs();
      void read_dir(const st_key &key);
      void get_file_info();
      FileListCache FLC;

 
//      SigC::Signal0<void> sig_file_scanned;
//      SigC::Signal0<std::string> sig_file_scanned;
   public:
      FileList() {}
      FileList(const std::vector<std::string> &path);
      ~FileList() { /*FLC.save();*/ /*save_cache();*/ }

//      SigC::Signal0<std::string> &SigFileScanned() {return sig_file_scanned;}
//      const std::string &ScannedFile(const std::string &s){return s;}

      void save_cache() const;
      const std::vector<std::string> MainPath() const {return mainpath;}
      void set_default_volume(const Soundfile &s,const int dv);
      int get_default_volume(const Soundfile &s) const;
      std::list<Soundfile> get_cd_file_list(const std::string &cd) const;


//      const t_filemap &Filemap()const{return filemap;}

      typedef t_filemap::const_iterator const_iterator;
      typedef t_filemap::iterator iterator;
      const_iterator begin() const{ return filemap.begin(); }  
      const_iterator end()   const{ return filemap.end();   }  
      iterator begin() { return filemap.begin(); }
      iterator end()   { return filemap.end();   }
      int size() const { return filemap.size();  }

};
