#include <FileList.hh>
#include <dirent.h>
#include<iostream>
#include<vector>
#include <sys/stat.h>

FileList::FileList(const std::vector<std::string> &path)
: mainpath(path)
{
   FLC.load();
   for(std::vector<std::string>::const_iterator i=path.begin();i!=path.end();++i)
    {
      st_key key(*i,0);
      read_dir(key);
    }
   get_file_info();
   for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
      i->second.sort();      
}

void FileList::read_dir(const st_key &key)
{
   DIR *dir=opendir(key.path.c_str());
   if(!dir) { std::cerr << "\n directory "<<key.path<<" not found\n";
              exit(1);}
   while(true)
    {
      dirent *ent=readdir(dir);
      if(ent==NULL) break;
      if(std::string(ent->d_name).find(".")==0) continue;

      struct stat sa;
      stat((key.path+"/"+ent->d_name).c_str(),&sa);
      filemap[key]; // necessary to append dirs without files 
      if(sa.st_mode & S_IFDIR)
       {
         st_key nkey(key.path+"/"+ent->d_name,key.sub_level+1);
         read_dir(nkey);
       }
      if(sa.st_mode & S_IFREG )
       {
         filemap[key].push_back(Soundfile(key.path,ent->d_name,sa.st_mtime));
//std::cout <<'#'<<ent->d_name <<"#  F\t"<<key.path<<'\t'<<sa.st_mtime<<'\n';
       }
    }   
}


void FileList::get_file_info() 
{
   for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
    {
      std::cout << i->first.path<<'\t'<<i->first.sub_level<<'\n';
      for(std::list<Soundfile>::iterator j=i->second.begin();j!=i->second.end();++j)
       {
//         sig_file_scanned();
//         SigFileScanned("").connect(SigC::slot(*this,sig_file_scanned));

         bool found_in_cache ; 
         const std::map<FileListCache::st_key,FileListCache::st_cache>::const_iterator ci
            = FLC.getCache().find(FileListCache::st_key(j->CacheName(),j->FileTime()));

         if(FLC.getCache().end()==ci) found_in_cache=false;
         else                         found_in_cache=true;

         if (found_in_cache)
          {
             j->setTime(ci->second.time);
             if(ci->second.time.empty()) found_in_cache=false;
             else j->setTime(ci->second.time);
             j->setDefaultVolume(ci->second.default_volume);
          }
         if(j->Seconds() == 0)
          {
            std::vector<std::pair<std::string,std::string> > com;
            com.push_back(std::pair<std::string,std::string>
               ("checkmp3 "+j->Filename(),"SONG_LENGTH"));
            com.push_back(std::pair<std::string,std::string>
               ("qmp3info -s "+j->Filename(),"=>"));
            for(std::vector<std::pair<std::string,std::string> >::
               const_iterator i=com.begin();i!=com.end();++i)
             {
               char buf[1024];
               FILE *ptr;
               if ((ptr = popen(i->first.c_str(), "r")) != NULL)
               while (fgets(buf, BUFSIZ, ptr) != NULL)
                 {
                   std::string time=buf;
                   if(time.find(i->second)!=std::string::npos)
                    {
                      j->setTime(time.substr(time.find(i->second)
                                    +i->second.size(),std::string::npos));
                      if(j->Time()!="0:00") goto loop_break;
                    }
                  }
               pclose(ptr);
             }
          }
loop_break:
      FLC.push_back(FileListCache::st_key(j->CacheName(),j->FileTime()),
                    FileListCache::st_cache(j->Time(),j->DefaultVolume()));
#if 1
    std::cout << " "<< found_in_cache<<"  "<<j->CacheName()<<' '
                    <<j->Time()<<' '<<j->DefaultVolume()<<'\n';
#endif
       }
    }
}


std::list<Soundfile> FileList::get_cd_file_list(const std::string &cd) const
{
   for(t_filemap::const_iterator i=filemap.begin();i!=filemap.end();++i)
      if(i->first.subpath == cd) 
         return i->second;
}


#include <fstream>
#include <stdlib.h>

void FileList::set_default_volume(const Soundfile &s,const int dv)
{
  for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
     for(std::list<Soundfile>::iterator j=i->second.begin();j!=i->second.end();++j)
        if (j->Name()==s.Name()) j->setDefaultVolume(dv) ;
}

int FileList::get_default_volume(const Soundfile &s) const
{
  for(t_filemap::const_iterator i=filemap.begin();i!=filemap.end();++i)
     for(std::list<Soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
        if (j->Name()==s.Name()) return j->DefaultVolume() ;
}


//////////////////////////////////////////////////////////////////////////
#include<vector>
void FileListCache::load() 
{
   std::string sname=getenv("HOME")+std::string("/.rps.cache");
   std::ifstream fi(sname.c_str());
   if(!fi.good()) { std::cout << sname <<" not found\n";
             return; }
   std::cout << "Loading "<<sname<<'\n';

   std::vector<std::string> FC;
   FC.push_back("Name:((");
   FC.push_back("FileTime:((");
   FC.push_back("Length:((");
   FC.push_back("Volume:((");
   std::string E = "))";

   while(true)
    {
      std::string line;
      std::getline(fi,line);
      if(!fi.good()) break;
      std::vector<std::string> VR;
      for(std::vector<std::string>::const_iterator i=FC.begin();i!=FC.end();++i)
       {
         std::string result;
         std::string::size_type _a = line.find(*i);
         if(_a==std::string::npos)
           {
            std::cerr << "Strange line in cache-file while looking for "
                      << *i<<"\n\t"<<line<<'\n';
            result="";
           }
         else
           {
             std::string::size_type _e = line.find(E); 
             result=line.substr(_a+i->size(),_e-(_a+i->size()));
             line=line.substr(_e+E.size(),std::string::npos);      
           }
         VR.push_back(result);
      }
//std::cout << VR[0]<<'\t'<<atol(VR[1].c_str())<<'\t'<<VR[2]<<'\t'<<VR[3]<<'\n';
     CM[FileListCache::st_key(VR[0],atol(VR[1].c_str()))] = 
       st_cache(VR[2],atoi(VR[3].c_str()));
    }
}

void FileListCache::save() const
{
   std::string sname=getenv("HOME")+std::string("/.rps.cache");
   std::ofstream fo(sname.c_str());
   std::cout << "Saving "<<sname<<'\n';
   for(const_iterator i=begin();i!=end();++i)
    {
      if (!i->second.time.empty())
         fo << "Name:(("  <<i->first.name<<"))\t"
            << "FileTime:(("<<i->first.file_time<<"))\t"
            << "Length:(("<<i->second.time<<"))\t"
            << "Volume:(("<<i->second.default_volume<<"))\n";
    }   
}
