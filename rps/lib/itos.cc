#include <itos.hh>

std::string itos(const int i,const bool seconds)
{
 char buffer[100];
 if(seconds) snprintf(buffer,sizeof buffer,"%02i",i);
 else        snprintf(buffer,sizeof buffer,"%i",i);
 return(buffer);
}

