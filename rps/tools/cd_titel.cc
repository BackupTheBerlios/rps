#include "getopt.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <stdio.h>

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
   while(fi.good())
    {
      std::getline(fi,line);
      if(!fi.good()) break;
      std::string::size_type st=line.find(" ");
      if(st==std::string::npos) continue;
      MS[line.substr(0,st)] = line.substr(st+1,std::string::npos);
    }
 return MS;
}

void remove_spaces(MS_t &MS)
{
   for(MS_t::iterator i=MS.begin();i!=MS.end();++i)
    {
      while(i->second.find(" ")!=std::string::npos)
        i->second.replace(i->second.find(" "),1,"_");
std::cout << i->first<<"###"<<i->second<<"###\n";
    }
}
 

void rename_dir(const std::string &dir,MS_t &MS)
{
   std::string new_dir=MS["Artist:"]+"__"+MS["Title:"];
   int r=rename(dir.c_str(),new_dir.c_str());
   std::cout << "rename_dir:"<<r<<'\n';
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
 MS_t MS=read_txt(dir);
 remove_spaces(MS);
// rename_dir(MS);
}
