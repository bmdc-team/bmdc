#pragma once
#include "book.hh"
class Notepad: public Book
{
	public:
	Notepad():
	m_Box(Gtk::ORIENTATION_VERTICAL),
	label("Notepad")
	{
		label.show();
		
		//Add the TreeView, inside a ScrolledWindow, with the button underneath:
		m_ScrolledWindow.add(m_TextView);

		//Only show the scrollbars when they are necessary:
		m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

		m_Box.pack_start(m_ScrolledWindow);	
		m_Box.show();
	}
	
	~Notepad() {}
	Gtk::Widget& getWidget() { return m_Box;}
	Gtk::Widget& getTabWidget() { return label;}
	
	Gtk::Box m_Box;
	Gtk::Label label;
	
	Gtk::ScrolledWindow m_ScrolledWindow;
	Gtk::TextView m_TextView;
	
	Glib::RefPtr<Gtk::TextBuffer> m_refTextBuffer1;
	
};
