#include "mainwindow.hh"
#include <iostream>

#include "notepad.hh"
#include "favoritehubs.hh"

HelloWorld::HelloWorld()
: m_button("Hello World"),   // creates a new button with label "Hello World".
m_VBox(Gtk::ORIENTATION_VERTICAL),
  m_Label1("Contents of tab 1"),
  m_Label2("Contents of tab 2")
{
  // Sets the border width of the window.
  set_border_width(10);
  
  add(m_VBox);
  
  //Add the Notebook, with the button underneath:
  m_Notebook.set_border_width(10);
  m_VBox.pack_start(m_Notebook);
  m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

 m_Notebook.signal_switch_page().connect(sigc::mem_fun(*this,
              &HelloWorld::on_notebook_switch_page) );

  // When the button receives the "clicked" signal, it will call the
  // on_button_clicked() method defined below.
  //m_button.signal_clicked().connect(sigc::mem_fun(*this,
    //          &HelloWorld::on_button_clicked));
  
  m_Notebook.append_page(m_Label1, "First");
 // m_Notebook.append_page(m_Label2, "Second");
  autoOpen_gui();
  
  show_all_children();

}


void HelloWorld::show()
{
	/*QueueManager::getInstance()->addListener(this);
	TimerManager::getInstance()->addListener(this);
	LogManager::getInstance()->addListener(this);

	typedef Func0<MainWindow> F0;
	F0 *f0 = new F0(this, &MainWindow::startSocket_client);
	WulforManager::get()->dispatchClientFunc(f0);

	f0 = new F0(this, &MainWindow::autoConnect_client);
	WulforManager::get()->dispatchClientFunc(f0);

	autoOpen_gui();*/
}

void HelloWorld::autoOpen_gui()
{
	//did it just leak out?
	//remeber every new need delete? or use (some) smart pointer?
	Notepad* np = new Notepad();
	m_Notebook.append_page(np->getWidget(),np->getTabWidget());
	books.push_back(np);
	FavoriteHubs* fh = new FavoriteHubs();
	m_Notebook.append_page(fh->getWidget(),fh->getTabWidget());
	books.push_back(fh);
}

HelloWorld::~HelloWorld()
{
}

void HelloWorld::on_button_clicked()
{
  std::cout << "Hello World" << std::endl;
}

void HelloWorld::on_notebook_switch_page(Gtk::Widget*  page , guint page_num)
{
  std::cout << "Switched to tab with index " << page_num << std::endl;
   Glib::ustring ss =	m_Notebook.get_tab_label_text(*page);

//  char buf[30];
  //sprintf(buf,"%d",page_num);
  set_title(ss);

  //You can also use m_Notebook.get_current_page() to get this index.
}
