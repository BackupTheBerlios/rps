// generated 2003/10/1 13:35:03 CEST by thoma@Tiger.(none)
// using glademm V2.0.0c_cvs
//
// newer (non customized) versions of this file go to rps.cc_new

// This file is for your program, I won't touch it again!

#include <config.h>
#include <gtkmm/main.h>

#include "main_window_RPS.hh"

int main(int argc, char **argv)
{  
#if defined(ENABLE_NLS)
   bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
   bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
   textdomain (GETTEXT_PACKAGE);
#endif //ENABLE_NLS
   
   Gtk::Main m(&argc, &argv);

   std::string path;
   if(argc==2) path = argv[1];

   main_window_RPS *main_window_RPS = new class main_window_RPS(path);
   m.run(*main_window_RPS);
   delete main_window_RPS;
   return 0;
}
