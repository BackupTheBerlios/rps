#include <map>
#include <string>
#include <soundfile.hh>
#include <vector>
#include <assert.h>

class FileListCache
{
   public:
      struct st_cache{ std::string time; int default_volume; 
             st_cache() :default_volume(0) {}
             st_cache(const std::string &t, const int d)
                  : time(t),default_volume(d) {}
             };
   private:
      std::map<std::string,st_cache> CM;

   public:
      void load_cache();

      const std::map<std::string,st_cache> &getCache() const {return CM;}   
};


class FileList
{
   private:
      struct st_key{std::string path; std::string subpath; int sub_level;
             st_key(const std::string &p,int s) : path(p), sub_level(s) 
               {
                 std::string::size_type st = path.find_last_of("/");
                 if(st == std::string::npos) assert("impossible path\n");
                 subpath=path.substr(st+1,std::string::npos);           
               }
             bool operator<(const st_key &b) const 
               {return sub_level<b.sub_level || 
                       sub_level==b.sub_level && path<b.path;}
             };
      typedef std::map<st_key,std::vector<Soundfile> > t_filemap;
      t_filemap filemap;
      std::string mainpath;

      void looking_for_subpaths();
      void read_subdirs();
      void read_dir(const st_key &key);
      void get_file_info();
      FileListCache FLC;
   public:
      FileList() {}
      FileList(const std::string &s);
      ~FileList() { save_cache(); }

      void save_cache() const;
      const std::string MainPath() const {return mainpath;}
      void set_default_volume(const Soundfile &s,const int dv);
      int get_default_volume(const Soundfile &s) const;
      std::vector<Soundfile> get_cd_file_list(const std::string &cd) const;


//      const t_filemap &Filemap()const{return filemap;}

      typedef t_filemap::const_iterator const_iterator;
      typedef t_filemap::iterator iterator;
      const_iterator begin() const{ return filemap.begin(); }  
      const_iterator end()   const{ return filemap.end();   }  
      iterator begin() { return filemap.begin(); }
      iterator end()   { return filemap.end();   }
      int size() const { return filemap.size();  }

};
