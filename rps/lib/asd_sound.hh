#ifndef ASD_SOUND_HH
#define ASD_SOUND_HH
#include <../config.h>

#if ASDSUPPORT

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
                            
};

#endif
#endif
