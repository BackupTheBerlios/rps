#include <map>
#include <string>
#include <soundfile.hh>
#include <vector>

class FileList
{
   private:
      typedef std::map<std::string,std::vector<Soundfile> > t_filemap;
      t_filemap filemap;
      std::string mainpath;

      void looking_for_subpaths();
      void read_subdirs();
      void get_file_info();
   public:
      FileList() {}
      FileList(const std::string &s);

      const std::string MainPath() const {return mainpath;}
//      const t_filemap &Filemap()const{return filemap;}

      typedef t_filemap::const_iterator const_iterator;
      typedef t_filemap::iterator iterator;
      const_iterator begin() const{ return filemap.begin(); }  
      const_iterator end()   const{ return filemap.end();   }  
      iterator begin() { return filemap.begin(); }
      iterator end()   { return filemap.end();   }
      int size() const { return filemap.size();  }


};
