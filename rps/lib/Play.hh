#include <soundfile.hh>
#include <vector>

class Play
{
      int mpgpid, asdpid;
      void play(const std::string &files,const bool repeat) ;
   public:
      Play(const Soundfile &S,const bool repeat);

      int MpgPid() const {return mpgpid;}
      int AsdPid() const {return asdpid;}
};
