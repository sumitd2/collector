/** collector

A full notice with attributions is provided along with this source code.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

* In addition, as a special exception, the copyright holders give
* permission to link the code of portions of this program with the
* OpenSSL library under certain conditions as described in each
* individual source file, and distribute linked combinations
* including the two.
* You must obey the GNU General Public License in all respects
* for all of the code used other than OpenSSL.  If you modify
* file(s) with this exception, you may extend this exception to your
* version of the file(s), but you are not obligated to do so.  If you
* do not wish to do so, delete this exception statement from your
* version.
*/

/*
Copyright (C) 2013-2014 Draios inc.

This file is part of sysdig.

sysdig is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

sysdig is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sysdig.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef CSYSDIG
#ifndef NOCURSESUI

class curses_table : 
	public curses_scrollable_list,
	public sinsp_chart
{
public:
	enum alignment
	{
		ALIGN_LEFT,
		ALIGN_RIGHT,
	};

	curses_table(sinsp_cursesui* parent, sinsp* inspector, sinsp_table::tabletype type);
	~curses_table();

	void configure(sinsp_table* table, 
		vector<int32_t>* colsizes, vector<string>* colnames);
	void update_data(vector<sinsp_sample_row>* data, bool force_selection_change = false);
	void render(bool data_changed);
	sysdig_table_action handle_input(int ch);
	void set_x_start(uint32_t x)
	{
		m_table_x_start = x;
	}
	void recreate_win(int h);
	void update_rowkey(int32_t row);
	void goto_row(int32_t row);
	bool get_position(OUT int32_t* pos, OUT int32_t* totlines, OUT float* percent, OUT bool* truncated);
	void follow_end();
	string get_field_val(string fldname);
	uint32_t get_data_size()
	{
		if(m_table != NULL)
		{
			return m_data->size();
		}
		else
		{
			return 0;
		}
	}

	sinsp_table_field_storage m_last_key;
	bool m_drilled_up;
	bool m_selection_changed;
	MEVENT m_last_mevent;
	
private:
	alignment get_field_alignment(ppm_param_type type);
	void print_error(string wstr);
	void print_wait();
	void print_line_centered(string line, int32_t off = 0);

	sinsp* m_inspector;
	WINDOW* m_tblwin;
	sinsp_cursesui* m_parent;
	sinsp_table* m_table;
	int32_t m_table_x_start;
	uint32_t m_table_y_start;
	uint32_t m_scrolloff_x;
	uint32_t m_colsizes[PT_MAX];
	vector<curses_table_column_info> m_legend;
	vector<sinsp_sample_row>* m_data;
	sinsp_filter_check_reference* m_converter;
	vector<uint32_t> m_column_startx;
	char alignbuf[64];
	sinsp_table::tabletype m_type;

	friend class curses_table_sidemenu;
	friend class sinsp_cursesui;        // for access to m_data in sinsp_cursesui::handle_input
};

#endif // NOCURSESUI
#endif // CSYSDIG
