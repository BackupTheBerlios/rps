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
   for(t_filemap::const_iterator i=filemap.begin();i!=filemap.end();++i)
    {
      std::cout << i->first<<'\n';
      for(std::vector<Soundfile>::const_iterator j=i->second.begin();j!=i->second.end();++j)
       {
         std::string cmd="qmp3info -s "+mainpath+"/"+j->Path()+"/"+j->Name()+"."+j->TypeStr();

//         system(cmd.c_str());

           char buf[BUFSIZ];
           FILE *ptr;
           std::string time,minu,sec;
           if ((ptr = popen(cmd.c_str(), "r")) != NULL)
           while (fgets(buf, BUFSIZ, ptr) != NULL)
            {
              time=buf;
              if(time.find_last_of("=>")!=std::string::npos && 
                 time.find_last_of(":")!=std::string::npos  &&
                 time.find_last_of("\n")!=std::string::npos)
               {
                 minu=time.substr(time.find_last_of("=>")+1,time.find(":")-1);
                 sec=time.substr(time.find(":")+1,time.find("\n")-2);
                 j->setTime(atoi(minu.c_str()),atoi(sec.c_str()));
               }
//std::cout<< "##"<<time <<"##\n";
            }
           pclose(ptr);
         std::cout << "   "<<j->Path()<<'\t'<<j->Name()<<' '<<j->TypeStr()
                   <<"\tTime:"<<"=>"<<atoi(minu.c_str())<<"<=>"<<atoi(sec.c_str())<<"<=\n";         
//exit (1);
       }
    }
}


