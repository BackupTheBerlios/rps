#!/usr/bin/python2.4
import gtk_window
import os
from popen2 import *


def check_esd():
    esd_found = False
    cmd="ps -ef | grep esd | grep -v grep"
    o,i,e = popen3(cmd)
    while (1):
       if o.readline():
#            print "O:", o.readline()
            esd_found = True           
#       elif e.readline():
#            print "E:", e.readline()
       else:
            break
    o.close()
    if esd_found: 
        print " esd is running"
    else:
        print " starting esd"
        os.system("esd &")

if __name__ == "__main__":
   path="/home/thoma/media/RoleSound/effect"
   check_esd()
   gtk_window.MainWindow(path)
   gtk_window.gtk.main()

