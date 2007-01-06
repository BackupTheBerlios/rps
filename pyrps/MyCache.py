from popen2 import *
import re
import os
from popen2 import *
import sys

class MyCache:
  def __init__(self):
    self.cachefilename=os.environ['HOME']+"/.pyrps.cache"
    try:
      cachefile = file(self.cachefilename,'r')
      self.cache_content = cachefile.read()
      cachefile.close()
    except:
      f = file(self.cachefilename,'w')
      self.cache_content = f.read()
      f.close()
    
  def find(self,name,fullName):

    last_modified = os.path.getmtime(fullName)
    regexpr=".*"+name+".*"+str(last_modified)
    reg = re.compile(regexpr)
    cache_line = reg.findall(self.cache_content)

    #    print name, cache_line
    if len(cache_line) > 1:
        #print "Warning: more than one entry for "+name+" found in cachefile:",cache_line
        reg = re.compile("^"+name+".*")
        for i in cache_line:
#            print "Q:",reg.findall(i)
            if len(reg.findall(i)) :
               X = reg.findall(i)[0]
               break
    elif len(cache_line) is 1:
        X=cache_line[0]
    else :
        X=""

    if X :
        slen = X.replace(name,'')
        slen = slen.replace(str(last_modified),'')
        slen = slen.strip()
        return slen    
    else: # calculate length and add to cachefile
      #print fullName
      cmd="checkmp3 "+fullName+" | grep SONG_LENGTH | awk '{print $2}'"
      #cmd="qmp3info -s "+fullName+" | awk '{print $4}'" #works not with damaged hea
      o,i,e = popen3(cmd)
      slen = o.readline()
      o.close() ; i.close() ; e.close()
      slen = slen.strip()           # remove newline
      if slen.find("."):            # remove hundredth seconds 
        reg  = re.compile('\.\d\d')
        slen = reg.sub("",slen)
      print "Add ",slen, name,last_modified, " to "+self.cachefilename
      cachefile = file(self.cachefilename,'a')
      cachefile.write(name+"  "+slen+"  "+str(last_modified)+"\n")
      cachefile.close()
      return slen
