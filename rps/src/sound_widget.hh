#include <gtkmm/frame.h>
#include <soundfile.hh>
#include <time.h>

class sound_widget : public Gtk::Frame
{
      Soundfile &sound;
      time_t start_time;

      void value_changed(Gtk::Adjustment *a);
      void button_pressed(Soundfile s);

      bool timeout_handler(Gtk::Label *l);

   public:
      sound_widget(Soundfile &s);
};
