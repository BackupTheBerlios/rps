#include <FileList.hh>
#include <dirent.h>
#include<iostream>
#include <sys/stat.h>


FileList::FileList(const std::string &s)
: mainpath(s) 
{
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

         char buf[100];
         FILE *ptr;
         if ((ptr = popen(cmd.c_str(), "r")) != NULL)
         while (fgets(buf, BUFSIZ, ptr) != NULL)
           {
             std::string time=buf;
             if(time.find_last_of("=>")!=std::string::npos && 
                time.find_last_of(":")!=std::string::npos  &&
                time.find_last_of("\n")!=std::string::npos)
              {
                std::string smin=time.substr(time.find_last_of("=>")+1,time.find(":")-time.find_last_of("=>")-1);
                std::string ssec=time.substr(time.find(":")+1,time.find("\n")-time.find(":")-1);
                int min = atoi(smin.c_str());
                int sec = atoi(ssec.c_str());

                j->setTime(min,sec);
               }
            }
           pclose(ptr);
           std::cout << "   "<<j->Filename()<<' '<<j->Time()<<'\n';
       }
    }
}

#include <fstream>
#include <stdlib.h>

void FileList::save() const
{
   std::string sname=getenv("HOME")+std::string("/.rps.rc");
   std::ofstream fo(sname.c_str());
   std::cout << "Saving "<<sname<<'\n';
   for(t_filemap::const_iterator i=filemap.begin();i!=filemap.end();++i)
    {
      for(std::vector<Soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
       {
std::cout << "Save "<<j->Filename()<<'\n';
         fo << "Name:"  <<j->Filename()<<"\t"
            << "Length:"<<j->Time()<<"\t"
            << "Volume:"<<j->DefaultVolume()<<'\n';
       }   
    }
}

