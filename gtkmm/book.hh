#pragma once
#include <gtkmm.h>
class Book
{
	public:
		Book(){}
		~Book(){}
		virtual Gtk::Widget& getWidget() = 0;
		virtual Gtk::Widget& getTabWidget() = 0;
	
	
};
