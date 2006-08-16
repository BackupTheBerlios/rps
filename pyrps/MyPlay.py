#from popen2 import *
import os
from popen2 import *

class MyPlay:


  def __init__(self):
    self.xine_conf = os.environ['HOME']+"/.xine/config"
    os.system("cp -v "+self.xine_conf+" "+self.xine_conf+".rps")

    self.start_x=870
    os.system("sed 's/\(.*\)gui.panel_x:\(.*\)/gui.panel_x:"+str(self.start_x)+"/g' "+self.xine_conf+" > X.tmp && cp X.tmp "+self.xine_conf)

    self.start_y=0
    self.y_increment=100
    self.max_y=900

    self.actual_playlist = []

  def __del__(self):
    os.system("mv -v "+self.xine_conf+".rps "+self.xine_conf)

  def increment_panel_position(self):
#    print "panel position:",self.start_y
    os.system("sed 's/\(.*\)gui.panel_y:\(.*\)/gui.panel_y:"+str(self.start_y)+"/g' "+self.xine_conf+" > X.tmp && cp X.tmp "+self.xine_conf)
    self.start_y += self.y_increment
    if (self.start_y > self.max_y): self.start_y = 0
    
  def Play(self,file,repeat,filename):
    self.increment_panel_position()
    cmd = "xine --no-splash --hide-video --hide-gui --auto-play=q "
#    cmd="xine "
    if (repeat): cmd += "--loop=repeat "
    cmd += file +" &"

#    print cmd
    os.system(cmd)
    pid="ps -ef | grep xine | grep -v grep | grep "+file+" | awk '{print $2}' | sort -n"
#    pid="ps -ef | grep xine | grep "+file
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
      self.actual_playlist.append([filename,int(list_of_pids[0])])

    