#ifndef PLAYLIST_HH
#define PLAYLIST_HH

#include <vector>
#include <soundfile.hh>
#include <sigc++/signal.h>


class PlayList
{
      std::vector<Soundfile> VP;
      SigC::Signal0<void> sig_playlist_changed;
   public:
      PlayList() {}

      SigC::Signal0<void> &SigPlaylistChanged() {return sig_playlist_changed;}
      void push_back(const Soundfile &p) 
         { VP.push_back(p); sig_playlist_changed(); }
      const std::vector<Soundfile>& getPlayList() const {return VP;}

      bool remove(const Soundfile &s, const bool kill_pids);

      typedef std::vector<Soundfile>::const_iterator const_iterator;
      typedef std::vector<Soundfile>::iterator iterator;
      const_iterator begin() const{ return VP.begin(); }  
      const_iterator end()   const{ return VP.end();   }  
      iterator begin() { return VP.begin(); }
      iterator end()   { return VP.end();   }
      int size() const { return VP.size();  }
      int empty() const { return VP.empty();  }
                     
};
#endif
