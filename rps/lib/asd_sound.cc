#include <asd_sound.hh>
#include <iostream>
asd_sound_identifier::asd_sound_identifier(const std::string &n) 
: name(n) 
{
   if(n.find("client")!=std::string::npos)
     id=atoi(n.substr(n.find_last_of("client")+1,std::string::npos).c_str());
}
