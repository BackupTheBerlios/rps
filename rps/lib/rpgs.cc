#include "rpgs.hh"


#include <dirent.h>
#include<iostream>
#include <sys/stat.h>
#include <signal.h>


#if 0

RPGS *RPGS::self;

//AsdConnection *ASD::asdcon;




RPGS::RPGS(const std::string &s) 
: mainpath(s),repeat(false),kill_on_new(false)
{
   self=this;
   looking_for_subpaths();
   read_subdirs();
   show_all_subdirs();
#ifdef ASDSUPPORT
   if (!(ASD::asdcon = asd_connection_new(NULL)))
    {
      perror("Could not create connection");
      exit (1);
    }
#endif
}




#ifdef ASDSUPPORT
void _new_asd_client(ProtocolAsdListResponse* response, gpointer userdata)
{ 
  g_assert(response);
  if(std::string(response->type)=="SOCKET")
   {
     soundfile *s=static_cast<soundfile*>(userdata);
     RPGS::self->new_asd_client(response->shortname,s);
   }
}

void RPGS::new_asd_client(const std::string &name,soundfile *s)
{
   const int id=0;

   ASD::st_asd ain(id,name);
   if(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain)==vec_asd_list.end())
     {
       vec_asd_list.push_back(ain);
       s->setClient(ain);
     }
}

void RPGS::remove_asd_client(const std::string &name)
{
   const int id=0;
   ASD::st_asd ain(id,name);
   if(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain)!=vec_asd_list.end())
     {
       vec_asd_list.erase(std::find(vec_asd_list.begin(),vec_asd_list.end(),ain));
     }
}

#endif      


#ifdef ASDSUPPORT
const std::string &name RPGS::id_to_name(const int id)
{
   for(PlayList::const_iterator i=playlist.begin();i!=playlist.end();++i)
    {
      
    }
  asset(!"never get here");
}
#endif


#ifdef ASDSUPPORT
void soundfile::get_volume()
{
//   asd_volume_get(AsdConnection *c, gchar *shortname, Volume *volume);
   gchar *name = const_cast<gchar*>(asd_client.name.c_str());
   asd_volume_get(ASD::asdcon, name , &volume);
//std::cout <<"Vol:"<< volume<<'\n';
}

void soundfile::set_volume(const double v)
{
// asd_volume_set(AsdConnection *c, gchar *shortname, Volume v);
   Volume vol;
   gchar *name = const_cast<gchar*>(asd_client.name.c_str());
   asd_volume_set(ASD::asdcon, name, vol);
}
#endif

#endif
