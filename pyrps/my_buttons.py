import gtk
import os
import MyPlay
import signal

class Buttons(gtk.Frame):
  
  def __init__(self,MainWindow):

    self.MW = MainWindow

    gtk.Frame.__init__(self,"Main Buttons")

    bbox = gtk.HButtonBox()             # but button in box
    bbox.set_border_width(5)
    bbox.set_layout(gtk.BUTTONBOX_SPREAD)   
    bbox.set_spacing(40)
    self.add(bbox)

    self.button_repeat = gtk.ToggleButton("Repeat")
    self.MW.tooltips.set_tip(self.button_repeat, "Repeat sound sample")
#    button.connect('clicked', lambda *w:self.model_reload())
    bbox.pack_start(self.button_repeat,expand=False,fill=False)
#    button = gtk.Button(stock='gtk-execute')
#    self.MW.tooltips.set_tip(button, "Execute plot command")
#    button.connect('clicked', lambda *w:self.plot_command())
#    bbox.pack_start(button,expand=False,fill=False)

    button = gtk.Button(stock='gtk-quit')
    self.MW.tooltips.set_tip(button, "Quit program")
    button.connect('clicked', lambda *w: self.quit_program() )
    bbox.pack_start(button,expand=False,fill=False)

    self.button_stop_on_new = gtk.ToggleButton('Stop For New')
    self.MW.tooltips.set_tip(self.button_stop_on_new , "Stop all xine when starting new sample")
    bbox.pack_start(self.button_stop_on_new ,expand=False,fill=False)

    button = gtk.Button('Stop Playing')
    self.MW.tooltips.set_tip(button, "Stop all xine")
    button.connect('clicked', lambda *w: self.kill_all_songs() )
    bbox.pack_start(button,expand=False,fill=False)

#    button = gtk.Button(stock='gtk-help')
#    self.MW.tooltips.set_tip(button, "Not implemented yet")
#    bbox.pack_start(button,expand=False,fill=False)

  def quit_program(self):
    self.MW.Play.__del__()
    gtk.main_quit()
    os.system("killall xine 2> /dev/null")
    os.system("killall -9 xine 2> /dev/null")
    
  def kill_all_songs(self):
    print "killing all in list"
    for i in self.MW.Play.actual_playlist:
      try:
        os.kill(i[1], signal.SIGTERM)
      except:
        pass
    os.system("killall xine 2> /dev/null")
    os.system("killall -9 xine 2> /dev/null")
    self.MW.Play.actual_playlist=[]
    self.MW.filetree.update_playlist()
