#include <Play.hh>

Play::Play(const Soundfile &S,const bool repeat)
{
   play(S.Filename(),repeat);
}

void Play::play(const std::string &file,const bool repeat)
{
   
   int fd[2];
   if (pipe(fd)) { perror("pipe"); return; }
   if (!(mpgpid=fork()))
    {close(fd[0]); 
     dup2(fd[1],1);
     close(fd[1]); 
     execl("/usr/bin/mpg123",  "mpg123","-sq","-b10240",
          repeat?"-Z":"-q",file.c_str(),0);
     perror("/usr/bin/mpg123");
     _exit(errno);
    }
   if (!(asdpid=fork()))
    {close(fd[1]);
     dup2(fd[0],0);
     close(fd[0]); 
     execl("/usr/bin/asdcat",  "asdcat",0);
     perror("/usr/bin/asdcat");
     _exit(errno);
    }
   close(fd[0]); close(fd[1]);
}

