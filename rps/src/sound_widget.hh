#include <gtkmm/box.h>
#include <soundfile.hh>

class sound_widget : public Gtk::VBox
{
      void value_changed(Gtk::Adjustment *a,const Soundfile &s);
   public:
      sound_widget(const Soundfile &s);
};
