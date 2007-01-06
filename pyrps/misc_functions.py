import os


def isCD(path): 
    print "XXX",path
    ret = False
#    if   file.find(path+"CDs") is 0 : ret = True  
#    elif file.find(path+"/CDs")is 0 : ret = True  
    if  path.find("CDs") is not -1: ret=True
    return ret

