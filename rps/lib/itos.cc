#include <itos.hh>

std::string itos(int i)
{
 char buffer[100];
 snprintf(buffer,sizeof buffer,"%i",i);
 return(buffer);
}

