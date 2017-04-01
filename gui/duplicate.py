#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
#
#	https://www.gpvdm.com
#	Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License v2.0, as published by
#    the Free Software Foundation.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, write to the Free Software Foundation, Inc.,
#    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


import os

from scan_select import select_param
from token_lib import tokens
from scan_item import scan_items_get_list

from scan_item import scan_item_save
from scan_plot import scan_gen_plot_data
from scan_io import scan_clean_dir
from scan_io import scan_clean_unconverged
from scan_io import scan_clean_simulation_output
from scan_io import scan_nested_simulation
from server import server_find_simulations_to_run
from scan_io import scan_plot_fits

from plot_io import plot_save_oplot_file
from scan_io import scan_push_to_hpc
from scan_io import scan_import_from_hpc
from cal_path import get_exe_command
from cal_path import get_image_file_path
from scan_item import scan_items_get_file
from scan_item import scan_items_get_token

from gpvdm_select import gpvdm_select

from window_list import windows

from util import str2bool

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar, QTableWidgetItem
from PyQt5.QtGui import QPainter,QIcon

from gui_util import tab_add
from gui_util import tab_remove
from gui_util import tab_get_value

from inp import inp_save_lines
from inp import inp_load_file

class duplicate(QWidget):

	def insert_row(self,i,src_file,src_token,src_path,dest_file,dest_token,dest_path):
		self.tab.blockSignals(True)
		self.tab.insertRow(i)

		item = QTableWidgetItem(src_file)
		self.tab.setItem(i,0,item)

		item = QTableWidgetItem(src_token)
		self.tab.setItem(i,1,item)

		self.item = gpvdm_select()
		self.item.setText(src_path)
		self.item.button.clicked.connect(self.callback_show_list_src)
		self.tab.setCellWidget(i,2,self.item)

		item = QTableWidgetItem(dest_file)
		self.tab.setItem(i,3,item)

		item = QTableWidgetItem(dest_token)
		self.tab.setItem(i,4,item)

		self.item = gpvdm_select()
		self.item.setText(dest_path)
		self.item.button.clicked.connect(self.callback_show_list_dest)
		self.tab.setCellWidget(i,5,self.item)
		
		self.tab.blockSignals(False)

	def callback_show_list_src(self):
		self.select_param_window_src.update()
		self.select_param_window_src.show()

	def callback_show_list_dest(self):
		self.select_param_window_dest.update()
		self.select_param_window_dest.show()
		
	def callback_add_item(self):
		self.insert_row(self.tab.rowCount(),_("Source file"),_("Source token"),_("Source path"),_("Destination file"),_("Destination token"),_("Destination path"))
		self.save_combo()

	def callback_delete_item(self):
		tab_remove(self.tab)
		self.save_combo()

	def save_combo(self):
		lines=[]
		for i in range(0,self.tab.rowCount()):
			lines.append(str(tab_get_value(self.tab,i, 0)))
			lines.append(str(tab_get_value(self.tab,i, 1)))
			lines.append(str(tab_get_value(self.tab,i, 2)))
			lines.append(str(tab_get_value(self.tab,i, 3)))
			lines.append(str(tab_get_value(self.tab,i, 4)))
			lines.append(str(tab_get_value(self.tab,i, 5)))
			
		lines.append("#end")
		print("save as",self.file_name)
		inp_save_lines(self.file_name,lines)


	def tab_changed(self):
		self.save_combo()
		

	def create_model(self):
		self.tab.clear()
		self.tab.setColumnCount(6)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Source File"), _("Source Token"), _("Source path"),_("Destination File"), _("Destination Token"), _("Destination path")])
		self.tab.setColumnWidth(2, 200)
		self.tab.setColumnWidth(5, 200)
		self.file_name="duplicate.inp"

		lines=[]
		pos=0

		if inp_load_file(lines,self.file_name)==True:
			mylen=len(lines)
			while(1):
				file_src=lines[pos]
				if lines[pos]=="#end":
					break
				pos=pos+1

				token_src=lines[pos]
				if lines[pos]=="#end":
					break
				pos=pos+1

				path_src=lines[pos]
				if lines[pos]=="#end":
					break
				pos=pos+1

				file_dest=lines[pos]
				if lines[pos]=="#end":
					break
				pos=pos+1

				token_dest=lines[pos]
				if lines[pos]=="#end":
					break
				pos=pos+1

				path_dest=lines[pos]
				if lines[pos]=="#end":
					break
				pos=pos+1

				self.insert_row(self.tab.rowCount(),file_src,token_src,path_src,file_dest,token_dest,path_dest)

				if pos>mylen:
					break

	def __init__(self):
		QWidget.__init__(self)
		self.setWindowTitle(_("Fit variables duplicate window")+" - https://www.gpvdm.com")   
		self.setWindowIcon(QIcon(os.path.join(get_image_file_path(),"duplicate.png")))
		self.setFixedSize(900, 700)

		self.win_list=windows()
		self.win_list.load()
		self.win_list.set_window(self,"fit_duplicate_window")
		
		self.vbox=QVBoxLayout()

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))

		self.tb_save = QAction(QIcon(os.path.join(get_image_file_path(),"list-add.png")), _("Add"), self)
		self.tb_save.triggered.connect(self.callback_add_item)
		toolbar.addAction(self.tb_save)

		self.tb_save = QAction(QIcon(os.path.join(get_image_file_path(),"list-remove.png")), _("Minus"), self)
		self.tb_save.triggered.connect(self.callback_delete_item)
		toolbar.addAction(self.tb_save)

		self.vbox.addWidget(toolbar)

		self.tab = QTableWidget()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.select_param_window_src=select_param()
		self.select_param_window_src.init(self.tab)
		self.select_param_window_src.set_save_function(self.save_combo)
		self.select_param_window_src.file_name_tab_pos=0
		self.select_param_window_src.token_tab_pos=1
		self.select_param_window_src.path_tab_pos=2


		self.select_param_window_dest=select_param()
		self.select_param_window_dest.init(self.tab)
		self.select_param_window_dest.set_save_function(self.save_combo)
		self.select_param_window_dest.file_name_tab_pos=3
		self.select_param_window_dest.token_tab_pos=4
		self.select_param_window_dest.path_tab_pos=5
		
		self.vbox.addWidget(self.tab)


		self.setLayout(self.vbox)

	def closeEvent(self, event):
		self.win_list.update(self,"fit_duplicate_window")
		self.hide()
