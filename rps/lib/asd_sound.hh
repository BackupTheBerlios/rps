#ifndef ASD_SOUND_HH
#define ASD_SOUND_HH
#include <../config.h>

#if ASDSUPPORT
#include <asdlib.h>
#include <string>

class asd_sound_identifier
{
   private:
      std::string name;
      
   public:
      asd_sound_identifier(const std::string &n) : name(n) {}
      asd_sound_identifier() {}
      
      bool operator==(const asd_sound_identifier &b) const
               {return name==b.name;}

      const gchar* getASDI() const {return name.c_str();}
      gchar* getASDI() {return const_cast<gchar*>(name.c_str());}
//      const gchar* operator()() const {return name.c_str();}
//       gchar* operator()() {return const_cast<gchar*>(name.c_str());}
                                  
};

#endif
#endif
