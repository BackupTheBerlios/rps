#include <FileList.hh>
#include <dirent.h>
#include<iostream>
#include<vector>
#include <sys/stat.h>


FileList::FileList(const std::string &s)
: mainpath(s) 
{
   FLC.load_cache();
   st_key key(mainpath,0);
   read_dir(key);

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
      if(sa.st_mode & S_IFREG )
       {
         filemap[key].push_back(Soundfile(key.path,ent->d_name));
//std::cout <<'#'<<ent->d_name <<"#  F\t"<<key.path<<'\n';
       }
      if(sa.st_mode & S_IFDIR)
       {
         st_key nkey(key.path+"/"+ent->d_name,key.sub_level+1);
         read_dir(nkey);
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

         bool found_in_cache ; 
         const std::map<std::string,FileListCache::st_cache>::const_iterator ci
                                 = FLC.getCache().find(j->Filename());
         if(FLC.getCache().end()==ci)
               found_in_cache=false;
         else  found_in_cache=true;

         if (found_in_cache)
          {
//std::cout << ci->first<<'\n';
             j->setTime(ci->second.time);
//std::cout << j->Filename()<<" cached time ="<<ci->second.time<<' '<<j->Time()<<'\n';
             if(ci->second.time.empty()) found_in_cache=false;
             else j->setTime(ci->second.time);
             j->setDefaultVolume(ci->second.default_volume);
          }
         else
          {
            std::string cmd="qmp3info -s "+j->Filename();
            char buf[100];
            FILE *ptr;
            if ((ptr = popen(cmd.c_str(), "r")) != NULL)
            while (fgets(buf, BUFSIZ, ptr) != NULL)
              {
                std::string time=buf;
                j->setTime(time);
               }
            pclose(ptr);
          }
         std::cout << "  "<<j->Filename()<<' '<<j->Time()<<' '
                   <<j->DefaultVolume()<<'\n';
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

void FileList::save_cache() const
{
   std::string sname=getenv("HOME")+std::string("/.rps.cache");
   std::ofstream fo(sname.c_str());
   std::cout << "Saving "<<sname<<'\n';
   for(t_filemap::const_iterator i=filemap.begin();i!=filemap.end();++i)
    {
      for(std::list<Soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
       {
         fo << "Name:(("  <<j->Filename()<<"))\t"
            << "Length:(("<<j->Time()<<"))\t"
            << "Volume:(("<<j->DefaultVolume()<<"))\n";
       }   
    }
}

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
void FileListCache::load_cache() 
{
   std::string sname=getenv("HOME")+std::string("/.rps.cache");
   std::ifstream fi(sname.c_str());
   if(!fi.good()) { std::cout << sname <<" not found\n";
             return; }
   std::cout << "Loading "<<sname<<'\n';

   std::vector<std::string> FC;
   FC.push_back("Name:((");
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
         std::string::size_type _a = line.find(*i);
         if(_a==std::string::npos)
           {
            std::cerr << "Strange line in cache-file while looking for "
                      << *i<<"\n\t"<<line<<'\n';
           }
         std::string::size_type _e = line.find(E); 
         std::string result=line.substr(_a+i->size(),_e-(_a+i->size()));
         line=line.substr(_e+E.size(),std::string::npos);      
//std::cout << _e<<"#"<<line<<"#\t#"<<result<<"#\n";
         VR.push_back(result);
      }
//std::cout << VR[0]<<'\t'<<VR[1]<<'\t'<<VR[2]<<'\n';
     CM[VR[0]] = st_cache(VR[1],atoi(VR[2].c_str()));
    }
}

