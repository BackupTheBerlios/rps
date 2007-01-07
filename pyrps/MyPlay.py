#from popen2 import *
import os
from popen2 import *
import subprocess

class MyPlay:

  def __init__(self,path,MainWindow):

    self.MW = MainWindow

    self.path=path
    self.xine_conf = os.environ['HOME']+"/.xine/config"
    os.system("cp -v "+self.xine_conf+" "+self.xine_conf+".rps")

    #self.start_x=870
    self.start_x=600
    os.system("sed 's/\(.*\)gui.panel_x:\(.*\)/gui.panel_x:"+str(self.start_x)+"/g' "+self.xine_conf+" > X.tmp && cp X.tmp "+self.xine_conf)

    self.start_y=0
    self.y_increment=100
    #self.max_y=900
    self.max_y=700
    
    self.actual_playlist = []

  def __del__(self):
    os.system("mv -v "+self.xine_conf+".rps "+self.xine_conf)

  def increment_panel_position(self):
#    print "panel position:",self.start_y
    os.system("sed 's/\(.*\)gui.panel_y:\(.*\)/gui.panel_y:"+str(self.start_y)+"/g' "+self.xine_conf+" > X.tmp && cp X.tmp "+self.xine_conf)
    self.start_y += self.y_increment
    if (self.start_y > self.max_y): self.start_y = 0

#  def isCD(self,file):
#    ret = False
#    if   file.find(self.path+"CDs") is 0 : ret = True
#    elif file.find(self.path+"/CDs")is 0 : ret = True
#    return ret

  def Play(self,fullpath,repeat,kill_for_new,filename,parent_dir):

    if kill_for_new:
      print "   active killer"
      self.MW.Buttons.kill_all_songs()
    else:
      print "   nokill"

    self.increment_panel_position()
    path = fullpath.rstrip(filename) 

    cmd = "cd "+path+"; xine --no-splash --hide-video --hide-gui --auto-play=q "
    if (repeat): cmd += "--loop=repeat "
    cmd += filename

#    pid = Popen3(cmd)
#    sp = subprocess.Popen(cmd,shell=False)
#    pid = sp.pid
#    print sp.pid
#    print "pids:",sp.pid
#    if filename is "*" :
#      self.actual_playlist.append( [parent_dir,pid] )
#    else:
#      self.actual_playlist.append( [filename,pid] )
#
#    print "Playlist :",self.actual_playlist

##    print  cmd
    os.system(cmd+" &")
    if filename is not "*" :
      pid="ps -ef | grep xine | grep -v grep | grep "+filename+" | awk '{print $2}' | sort -n"
    else:
      pid="ps -ef | grep xine | grep -v grep | grep "+fullpath+" | awk '{print $2}' | sort -n"
#    pid="ps -ef | grep xine | grep "+file
    print pid
    o,i,e = popen3(pid)
    list_of_pids= []
    while (1):
       line = o.readline()
       if   line   : list_of_pids.append( line )
       elif e.readline()  : print "ERROR from Play (pid)",e.readline()
       else           : break       
    o.close()
    i.close()
    e.close()
    
    if len(list_of_pids) is not 1:
      print "WARNING: Found not exactly one PID:", list_of_pids
    
    if len(list_of_pids) > 0 : 
#      print "PID: ",list_of_pids[0]
      if filename is "*" :
#        self.actual_playlist.append([parent_dir,int(list_of_pids[0])])
        self.actual_playlist.append([fullpath,int(list_of_pids[0])])
      else:
        self.actual_playlist.append([filename,int(list_of_pids[0])])

    