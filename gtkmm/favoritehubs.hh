#pragma once
#include "book.hh"
class FavoriteHubs: public Book
{
	public:
	FavoriteHubs():
	m_Box(Gtk::ORIENTATION_VERTICAL),
	label("Favorite Hubs")
	{
		label.show();
		
		 m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_TextView.set_model(m_refTreeModel);
		//Add the TreeView, inside a ScrolledWindow, with the button underneath:
		m_ScrolledWindow.add(m_TextView);

		//Only show the scrollbars when they are necessary:
		m_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

		m_Box.pack_start(m_ScrolledWindow);	
		m_Box.show();
	}
	//Tree model columns:
	class ModelColumns : public Gtk::TreeModel::ColumnRecord
	{
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); add(m_col_number); add(m_col_percentage);}

    Gtk::TreeModelColumn<unsigned int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    Gtk::TreeModelColumn<short> m_col_number;
    Gtk::TreeModelColumn<int> m_col_percentage;
  };

  ModelColumns m_Columns;
	
	~FavoriteHubs() {}
	Gtk::Widget& getWidget() { return m_Box;}
	Gtk::Widget& getTabWidget() { return label;}
	
	Gtk::Box m_Box;
	Gtk::Label label;
	
	Gtk::ScrolledWindow m_ScrolledWindow;
	Gtk::TreeView m_TextView;
	Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
	
		
};
