
#include "mainwindow.hh"




int main(int argc, char** argv)
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.gtkmm.bmdc");

  HelloWorld helloworld;
  return app->run(helloworld);
}
