/*
 * Copyright (C) 2011 - 2017 - BMDC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _BMDC_SPLASH_HH_
#define _BMDC_SPLASH_HH_

#include <iostream>
#include <gtk/gtk.h>
#include "../dcpp/Util.h"
#include "genres.h"

using namespace std;
using namespace dcpp;

class Splash
{
	public:
		Splash() : Text("") , percentage("0")
		,perc(0),win(NULL), label(NULL), box(NULL), image(NULL), progressbar(NULL) { }
		void show() {
			win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
			gtk_window_set_decorated(GTK_WINDOW(win),FALSE);
			gtk_window_set_default_size(GTK_WINDOW(win),350,20);
			gtk_window_set_skip_taskbar_hint(GTK_WINDOW(win),TRUE);//@is this good idea?
			gtk_window_set_keep_above(GTK_WINDOW(win), TRUE);
			gtk_window_set_position(GTK_WINDOW(win),GTK_WIN_POS_CENTER);
			label = gtk_label_new("Loading...");
			progressbar = gtk_progress_bar_new ();
			box = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
			
			GResource* res = ::bmdc_get_resource();
			g_resources_register(res);
			image = gtk_image_new_from_resource("/org/bmdc-team/bmdc/icons/hicolor/96x96/apps/bmdc.png");
			
			gtk_container_add(GTK_CONTAINER(box),image);
			gtk_container_add(GTK_CONTAINER(box),label);
			gtk_container_add(GTK_CONTAINER(box), progressbar);
			gtk_container_add(GTK_CONTAINER(win),box);
			gtk_widget_show_now(image);
			gtk_widget_show_now(label);
			gtk_widget_show_now(progressbar);
			gtk_widget_show_now(box);
			gtk_widget_show_now(win);
			update();
		}
		~Splash() {	win = NULL;label= NULL;box= NULL;image= NULL;progressbar= NULL; }

	void setText(const string &text) {
						if(text.empty()) return;
						Text = text;
						cout << "Loading: " << text << endl;
					}
	void setPercentage(const float& ii)
	{
		percentage = Util::toString(ii*100);
		perc = ii;
	}					

	void update() {
					gtk_label_set_text(GTK_LABEL(label),("Loading ..."+Text+" "+percentage+" %").c_str());
					gtk_progress_bar_set_fraction (GTK_PROGRESS_BAR(progressbar),perc);
					while (g_main_context_iteration(NULL, FALSE));
			 }
	void destroy() { gtk_widget_destroy(win); }

	private:
		string Text;
		string percentage;
		float perc;
		GtkWidget *win;
		GtkWidget *label;
		GtkWidget *box;
		GtkWidget *image;
		GtkWidget *progressbar;

};
#else
class Splash;
#endif
