#include <FileList.hh>
#include <dirent.h>
#include<iostream>
#include <sys/stat.h>


FileList::FileList(const std::string &s)
: mainpath(s) 
{
   FLC.load_cache();
   looking_for_subpaths();
   read_subdirs();
   get_file_info();
   for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
      std::sort(i->second.begin(),i->second.end());
}


void FileList::looking_for_subpaths()
{
     DIR *dir=opendir(mainpath.c_str());
     if(!dir) { std::cerr << "\n directory "<<mainpath<<" not found\n";
                exit(1);}
     struct stat sa;
     while(true)
      {
        dirent *ent=readdir(dir);
        if(ent==NULL) break;
        stat((mainpath+"/"+ent->d_name).c_str(),&sa);
        if( sa.st_mode & S_IFREG ) continue;
        if( std::string(ent->d_name).find(".")==0) continue;
           filemap[ent->d_name];
      }
}

void FileList::read_subdirs()
{
   std::vector<t_filemap::iterator> nodirs;
   for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
    {
      DIR *dir=opendir((mainpath+"/"+i->first).c_str());
      if(!dir) continue;
      while(true)
       {
        dirent *ent=readdir(dir);
        if(ent==NULL) break;
        if( std::string(ent->d_name).find(".")!=0)
           i->second.push_back(Soundfile(i->first,ent->d_name));
       }    
    }
}

void FileList::get_file_info() 
{
   for(t_filemap::iterator i=filemap.begin();i!=filemap.end();++i)
    {
      std::cout << i->first<<'\n';
      for(std::vector<Soundfile>::iterator j=i->second.begin();j!=i->second.end();++j)
       {
         std::string cmd="qmp3info -s "+mainpath+j->Filename();

         bool found_in_cache ; 
         const std::map<std::string,FileListCache::st_cache>::const_iterator ci
                                 = FLC.getCache().find(j->Filename());
         if(FLC.getCache().end()==ci)
               found_in_cache=false;
         else  found_in_cache=true;

         if (found_in_cache)
          {
std::cout << "cached time ="<<ci->second.time<<'\n';
             j->setTime(ci->second.time);
          }
         else
          {
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
         std::cout << "   "<<j->Filename()<<' '<<j->Time()<<'\n';
       }
    }
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
      for(std::vector<Soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
       {
         fo << "Name:(("  <<j->Filename()<<"))\t"
            << "Length:(("<<j->Time()<<"))\t"
            << "Volume:(("<<j->DefaultVolume()<<"))\n";
       }   
    }
}

//////////////////////////////////////////////////////////////////////////

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

