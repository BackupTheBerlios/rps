#ifndef RPGS_LIB_HH
#define RPGS_LIB_HH

//#define ASDSUPPORT
#include <string>
#include <map>
#include <vector>
#include <soundfile.hh>
#include <PlayList.hh>
#include <FileList.hh>

#include <sigc++/signal.h>
#ifdef ASDSUPPORT
extern "C" {
#include <asdlib.h>
}
#endif


#ifdef ASDSUPPORT
namespace ASD
{
      struct st_asd{int id; std::string name;
             st_asd() : id(0),name("NONE"){}
             st_asd(int i,const std::string &n): id(i),name(n) {}
                    bool operator==(const st_asd &b) const
                     {return id==b.id && name==b.name;}
                  };

      static AsdConnection *asdcon;
}
#endif



#if 0
class RPGS
{
   public:

      static RPGS *self;
   private:

#ifdef ASDSUPPORT
   private:
      void remove_asd_client(const std::string &name);
      const std::string &name id_to_name(const int id);
#endif

   public:
      RPGS(const std::string &s);

};
#endif
#endif
