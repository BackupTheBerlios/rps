// generated 2003/9/22 11:37:15 CEST by thoma@Tiger.(none)
// using glademm V2.0.0b
//
// newer (non customized) versions of this file go to rpgs.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gtkmm/main.h>

#include "main_window_RPS.hh"
#include <iostream>

void usage(const std::string &s)
{
   std::cerr <<  s <<" must be called with a top level directory, where"
      " the subdirs with different soundfiles are.\n";
   exit(1);
}



int main(int argc, char **argv)
{  
#if defined(ENABLE_NLS)
   bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
   bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
   textdomain (GETTEXT_PACKAGE);
#endif //ENABLE_NLS

#if 0
   if(argc!=2) usage(argv[0]);
   const std::string top_level_dir=argv[1];
#else
   const std::string top_level_dir="/var/thoma/sound/testsound";
#endif
   Gtk::Main m(&argc, &argv);

   main_window_RPS *main_window_RPS = new class main_window_RPS(top_level_dir);
   m.run(*main_window_RPS);
   delete main_window_RPS;
   return 0;
}
