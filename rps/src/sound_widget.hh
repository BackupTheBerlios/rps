#include <gtkmm/frame.h>
#include <soundfile.hh>
#include <time.h>

class sound_widget : public Gtk::Frame
{
      Soundfile &sound;

      void value_changed(Gtk::Adjustment *a);
      void button_pressed(Soundfile s);

      bool timeout_handler(Gtk::Label *l);
      std::string get_time_text() const;
   public:
      sound_widget(Soundfile &s);
};
