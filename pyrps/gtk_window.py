import gtk
import my_buttons
import my_filetree
import MyPlay


########################################################
class MainWindow(gtk.Window):
########################################################
########################################################
########################################################
  def __del__(self):
    print "MainWindow__del__"
    self.Play.__del__()

  def __init__(self, path,parent=None):

    self.Play = MyPlay.MyPlay(path)

    # Create the toplevel window
    gtk.Window.__init__(self)   
    try:
       self.set_screen(parent.get_screen())
    except AttributeError:
       self.connect('destroy', lambda *w: gtk.main_quit())

    self.set_default_size(1000, 800)
    self.set_title('pgui for POLAIR')
    self.set_border_width(10)

    self.tooltips = gtk.Tooltips()      # tooltipps
    main_vbox = gtk.VBox()              # main vbox
    self.add(main_vbox)   
                                        # fill main box with stuff
#    main_vbox.pack_end(self.plot_command_holder(),expand=False,padding=0)

    self.Buttons = my_buttons.Buttons(self)
    main_vbox.pack_start(self.Buttons,expand=False,fill=True,padding=0)

    self.show()
    self.get_screen().get_display().flush()    

    paned = gtk.HPaned()
    main_vbox.pack_start(paned)
        
    self.filetree = my_filetree.FileTree(self,path)
    paned.pack1(self.filetree, resize=False, shrink=True)

    self.playlist = gtk.Frame("Playlist")
    paned.pack2(self.playlist, resize=False, shrink=True)

    self.show_all()
########################################################
########################################################
