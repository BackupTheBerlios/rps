#include "getopt.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <dirent.h>


const static struct option options[]=
{{ "directory",  required_argument, NULL, 'd' },
 { "txtfile", required_argument,    NULL, 't' },  
 { NULL,      0,       NULL, 0 }
};       
         
typedef std::map<std::string,std::string> MS_t;

MS_t read_txt(const std::string &d)
{
   std::ifstream fi(d.c_str());
   std::string line;
   MS_t MS;
//std::cout << "Open "<<d<<'\n';
   while(fi.good())
    {
      std::getline(fi,line);
      if(!fi.good()) break;
//std::cout << "Read "<<line<<'\n';
      std::string::size_type st=line.find(" ");
      if(st==std::string::npos) continue;
      MS[line.substr(0,st)] = line.substr(st+1,std::string::npos);
    }
 return MS;
}

void remove_spaces(MS_t &MS)
{
   std::map<std::string,std::string> MR;
   MR[" "]="_";
   MR["Ä"]="Ae";
   MR["Ü"]="Ue";
   MR["Ö"]="Oe";
   MR["ä"]="ae";
   MR["ü"]="ue";
   MR["ö"]="oe";
   MR["ß"]="ss";
   MR[","]="_";
   MR["-"]="_";
   MR["."]="_";
   MR["&"]="and";
   MR["'"]="";
   MR["\\"]="_";
   MR["/"]="_";
   MR["("]="_";
   MR[")"]="_";
   for(MS_t::iterator i=MS.begin();i!=MS.end();++i)
    {
      for(std::map<std::string,std::string>::const_iterator j=MR.begin();j!=MR.end();++j)
       {
         while(i->second.find(j->first)!=std::string::npos)
           i->second.replace(i->second.find(j->first),j->first.size(),j->second);
       }
//std::cout << i->first<<"###"<<i->second<<"###\n";
    }
}
 

std::string rename_dir(std::string dir,MS_t &MS)
{
   std::string new_dir=MS["Artist:"]+"_"+MS["Title:"];

   if(dir.find_last_of("/")==dir.size()-1)
      dir=dir.substr(0,dir.size()-1);      
//std::cout << dir<<' '<<dir.size()<<' '<<dir.find_last_of("/")<<'\n';
   std::string d=dir.substr(0,dir.find_last_of("/"));
   new_dir=d+"/"+new_dir;
//std::cout << dir<<'\n'<<d<<'\n'<<new_dir<<'\n';
//exit(0);
   if(dir==new_dir) return new_dir;
   std::string com="mv "+dir+" "+new_dir;
//   setenv("LANG","C",1);
std::cout << com<<'\n';
//exit(1);
   system(com.c_str());

   
//   int r=rename(dir.c_str(),new_dir.c_str());
//   std::cout << "rename_dir:"<<r<<'\n';
   return new_dir;
}

void rename_files(const std::string &newdir,MS_t &MS)
{
   DIR *dir=opendir(newdir.c_str());
   if(!dir) {std::cerr << "Can't open "<<newdir<<'\n'; exit(1);}
   while(true)
    {
      dirent *ent=readdir(dir);
      if(ent==NULL) break;
      if(std::string(ent->d_name).find(".")==0) continue;
      std::string file=ent->d_name;
      std::string front = "track";
//      std::string back  = ".cdda.mp3";
//std::cout << file<<'\n';
      std::string num = file.substr(front.size(),2);
std::cout <<  ent->d_name<<' '<<num<<'\t'<<MS[num]<<'\n';
      rename((newdir+"/"+ent->d_name).c_str(),
             (newdir+"/"+num+"_"+MS[num]+".mp3").c_str());
    }
}


int main (int argc, char *argv[])
{
 int opt;
 std::string dir,txt_file;
 while ((opt=getopt_long(argc,argv,"d:t:",options,NULL))!=EOF)
  { switch (opt)
    {  case 'd' : dir=optarg; break;
       case 't' : txt_file=optarg; break;
    }
  }
 if(dir.empty() ) {std::cerr << "no directory given\n";exit(1);}
 if(txt_file.empty()) {std::cerr << "no textfile given\n";exit(1);}
 MS_t MS=read_txt(txt_file);
 remove_spaces(MS);
 std::string new_dir=rename_dir(dir,MS);
 rename_files(new_dir,MS);
 std::cout << "  ... finished "<<new_dir<<"\n\n";
}
