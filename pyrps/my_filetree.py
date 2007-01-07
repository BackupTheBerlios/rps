import gtk
import gobject
import os
import signal
import sys
import MyCache
import MyPlay
import misc_functions


(
      COLUMN_FULLPATH,
      COLUMN_DIR,
      COLUMN_FILE,
      COLUMN_LENGTH,
      COLUMN_PARENTPATH,
      COLUMN_HAS_SUBDIRS
) = range(6)

(
      COLUMN_PID,
      COLUMN_PLAYFILE
) = range(2)



#class FileTree(gtk.TreeView):
class FileTree(gtk.ScrolledWindow):
  def __init__(self,MainWindow,path):
        self.MainWindow = MainWindow
        self.Cache = MyCache.MyCache()

        gtk.ScrolledWindow.__init__(self)
        self.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        self.set_shadow_type(gtk.SHADOW_IN)

        model = gtk.TreeStore(gobject.TYPE_PYOBJECT, gobject.TYPE_STRING,
                              gobject.TYPE_STRING,gobject.TYPE_STRING,
                              gobject.TYPE_STRING,gobject.TYPE_BOOLEAN)
        treeview=gtk.TreeView(model)

#        selection = treeview.get_selection()
#        selection.set_mode(gtk.SELECTION_BROWSE)
        treeview.set_size_request(200, -1)

        MainWindow.tooltips.set_tip(self, "Select a File")

#        # get all hosts & domains
#        all_files = MainWindow.VecFiles

#        print all_files
        print "creating nodes from all files ..."
        self.create_node(model,None,path,None)
        print " ... finished"

        column = gtk.TreeViewColumn("Directories", gtk.CellRendererText(), text=COLUMN_DIR)
#        column.set_sort_column_id(COLUMN_DIR)
#        column.set_sort_order(gtk.SORT_ASCENDING)
        treeview.append_column(column)
#        column.clicked()

        column = gtk.TreeViewColumn("File", gtk.CellRendererText(), text=COLUMN_FILE)
#        column.set_sort_column_id(COLUMN_FILE)
#        column.set_sort_order(gtk.SORT_ASCENDING)
        treeview.append_column(column)
#        column.clicked()
        column = gtk.TreeViewColumn("Length", gtk.CellRendererText(), text=COLUMN_LENGTH)
        treeview.append_column(column)

        treeview.connect("row-activated",self.row_activated)

#        selection.connect('changed', self.selection_changed)
#        selection.connect('clicked', self.selection_changed)

        self.add(treeview)

#        treeview.expand_row()
#        treeview.expand_all()

  def create_node(self,model,parent_iter,path,parent_path):
    # Confirm the directory is valid and raise an exception if not.
    try:
        self.names = os.listdir(path)
        self.names.sort()      # Get this in a recognizable order.
    except os.error:
        raise NoSuchDirectory(path)

    
    # Iterate over the names in the directory list.
    has_no_subdirs = False
    for name in self.names:
#       print 'name=',name 
       # Pass over current and parent pseudo-directories.
       if name == "." or name == "..":
         next

       # Get the full name including base path.
       fullName = os.path.join(path, name)
                                  
       hiter = model.insert_before(parent_iter, None)
       model.set_value(hiter, COLUMN_FULLPATH, fullName)
       model.set_value(hiter, COLUMN_PARENTPATH, parent_path)
       if os.path.isdir(fullName):   
          has_no_subdirs = True
          model.set_value(hiter, COLUMN_DIR, name)
          self.create_node(model,hiter,fullName,name)
       if os.path.isfile(fullName):
          model.set_value(hiter, COLUMN_FILE, name)
          slen = self.Cache.find(name,fullName)
          model.set_value(hiter, COLUMN_LENGTH, slen)

#    print has_no_subdirs,path
    if(parent_iter):
      model.set_value(parent_iter, COLUMN_HAS_SUBDIRS, has_no_subdirs)

  def row_activated(self,tree,path,view_column):
#      print 'T=',tree,'\nP=',path,'\n', 'C=',view_column
      if tree.row_expanded(path):   tree.collapse_row(path)
      else                      :   tree.expand_row(path, False)

      if tree.get_model():
        iter=tree.get_model().get_iter(path)
        fullpath       = tree.get_model().get_value(iter,COLUMN_FULLPATH)
        has_sub_dirs   = tree.get_model().get_value(iter,COLUMN_HAS_SUBDIRS)
        file           = tree.get_model().get_value(iter,COLUMN_FILE)
        dir            = tree.get_model().get_value(iter,COLUMN_DIR)

        repeat = self.MainWindow.Buttons.button_repeat.get_active()


#        print has_sub_dirs,dir,fullpath,file
      
        kill_for_new = self.MainWindow.Buttons.button_stop_on_new.get_active()
        if file:
          self.MainWindow.Play.Play(fullpath,repeat,kill_for_new,file,None)
          self.update_playlist()
        if dir and not has_sub_dirs:
          if misc_functions.isCD(fullpath): 
            parent_dir=tree.get_model().get_value(iter,COLUMN_PARENTPATH)
            print "PLAYDIR= ",fullpath,parent_dir
            self.MainWindow.Play.Play(fullpath,repeat,kill_for_new,"*",parent_dir)
            self.update_playlist()
#          else:
#            print "Not playing"
                    
      else: 'No Model found'

#################################################################################
#################################################################################
  def update_playlist(self):
    try : # first time options are created no child exists
      self.MainWindow.playlist.remove(self.MainWindow.playlist.get_child())
    except: pass

#    model = gtk.TreeStore(gobject.TYPE_STRING,gobject.TYPE_OBJECT)
    model = gtk.ListStore(gobject.TYPE_PYOBJECT,gobject.TYPE_STRING)
#    model = gtk.TreeStore(gobject.TYPE_STRING,gtk.gdk.Pixbuf)
    tree = gtk.TreeView(model)
    iter = model.insert_before(None, None)
#    print "SIZE=",len(self.MainWindow.Play.actual_playlist)
    for i in self.MainWindow.Play.actual_playlist:
        file=i[0]
        pid=i[1]
        model.set_value(iter, COLUMN_PID     ,    i)
        model.set_value(iter, COLUMN_PLAYFILE, file)
        iter = model.append()
    model.remove(iter) # remove last (empty) row

    column = gtk.TreeViewColumn("Actual Songs (click to kill)", gtk.CellRendererText(), text=COLUMN_PLAYFILE)
    tree.append_column(column)

#    column = gtk.TreeViewColumn("Kill Song",gtk.CellRendererEditable())#, gtk.CellRendererToggle(), text=1)
#    column = gtk.TreeViewColumn('Icon', gtk.CellRendererPixbuf(), pixbuf=1)
#    tree.append_column(column)

#      selection.connect('changed', self.file_selection_changed)

    tree.expand_all()
    tree.show_all()
    tree.connect("row-activated",self.row_activated_kill)

    self.MainWindow.playlist.add(tree)

  def row_activated_kill(self,tree,path,view_column):
#      print 'T=',tree,'\nP=',path,'\n', 'C=',view_column
      if tree.get_model():
        iter=tree.get_model().get_iter(path)
        val = tree.get_model().get_value(iter,COLUMN_PID)
        pid = val[1]
        #print 'val=',val, 'pid=',pid
#        if pid:
#          os.system("kill "+str(pid))
        try:
#          print "trying to kill pid ",pid
          os.kill(pid, signal.SIGTERM)
        except:
          print "pid ",pid ," des not exist "
        self.MainWindow.Play.actual_playlist.remove(val)
        self.update_playlist()
        
      else: 'No pid found'

###################################################################


#  def selection_changed(self,selection):
#      model, iter = selection.get_selected() 
#      if not iter:
#         return False

#      file = model.get_value(iter, 0)
#      print file
#      if file is None : return
#      self.MainWindow.file_selected(file)
####################################################################
      
