#ifndef GTKMM_EXAMPLE_HELLOWORLD_H
#define GTKMM_EXAMPLE_HELLOWORLD_H

//#include <gtkmm/button.h>
//#include <gtkmm/window.h>
#include <gtkmm.h>
#include "book.hh"

class HelloWorld : public Gtk::Window
{

public:
  HelloWorld();
  virtual ~HelloWorld();

protected:
  //Signal handlers:
  void on_button_clicked();
  void on_notebook_switch_page(Gtk::Widget* page, guint page_num);

  //Member widgets:
  Gtk::Button m_button;
  
  Gtk::Box m_VBox;
  Gtk::Notebook m_Notebook;
  Gtk::Label m_Label1, m_Label2;
  Gtk::ButtonBox m_ButtonBox;
  void show();
  void autoOpen_gui();
  std::vector<Book*> books;
  
};

#endif // GTKMM_EXAMPLE_HELLOWORLD_H
