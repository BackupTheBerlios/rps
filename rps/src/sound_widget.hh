#include <gtkmm/frame.h>
#include <soundfile.hh>

class sound_widget : public Gtk::Frame
{
      void value_changed(Gtk::Adjustment *a,Soundfile s);
      void button_pressed(Soundfile s);
      void button_pressed_();
   public:
      sound_widget(Soundfile &s);
};
