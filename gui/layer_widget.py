#!/usr/bin/env python2.7
#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012 Roderick C. I. MacKenzie <r.c.i.mackenzie@googlemail.com>
#
#	www.gpvdm.com
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
from optics import find_materials
#from inp import inp_write_lines_to_file
from util import str2bool
from inp_util import inp_search_token_value
from scan_item import scan_item_add
from scan_item import scan_remove_file
from cal_path import get_image_file_path
from plot_gen import plot_gen
from gpvdm_open import gpvdm_open
from cal_path import get_materials_path
from global_objects import global_object_get
from help import help_window

#inp
from inp import inp_isfile
from inp import inp_copy_file
from inp import inp_update_token_value
from inp import inp_load_file


#epitaxy
from epitaxy import epitaxy_get_pl_file
from epitaxy import epitay_get_next_pl
from epitaxy import epitaxy_get_name
from epitaxy import epitaxy_get_width
from epitaxy import epitaxy_get_mat_file
from epitaxy import epitaxy_get_electrical_layer
from epitaxy import epitaxy_get_layers
from epitaxy import epitaxy_save
from epitaxy import epitaxy_load_from_arrays
from epitaxy import epitay_get_next_dos

#windows
from contacts import contacts_window
from emesh import tab_electrical_mesh
from doping import doping_window
from gui_util import tab_move_down
from gui_util import tab_add
from gui_util import tab_remove
from gui_util import tab_get_value
from gui_util import tab_set_value
from gui_util import yes_no_dlg

#qt
from PyQt5.QtCore import QSize, Qt
from PyQt5.QtGui import QIcon
from PyQt5.QtWidgets import QWidget, QVBoxLayout,QProgressBar,QLabel,QDesktopWidget,QToolBar,QHBoxLayout,QAction, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QDialog,QAbstractItemView

from PyQt5.QtGui import QPixmap
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QWidget

import i18n
_ = i18n.language.gettext

from i18n import yes_no


class layer_widget(QWidget):

	changed = pyqtSignal()

	def combo_changed(self):
		print("saved")
		self.save_model()
		self.emit_change()

	def tab_changed(self, x,y):
		self.save_model()
		self.emit_change()
		
	def emit_change(self):
		print("emit")
		self.changed.emit()
		

	def sync_to_electrical_mesh(self):
		tot=0
		for i in range(0,len(self.model)):
			if yes_no(self.model[i][COLUMN_DEVICE])==True:
				tot=tot+float(self.model[i][COLUMN_THICKNES])

		lines=[]
		if inp_load_file(lines,os.path.join(os.getcwd(),"mesh_y.inp"))==True:
			mesh_layers=int(inp_search_token_value(lines, "#mesh_layers"))
			if mesh_layers==1:
				inp_update_token_value(os.path.join(os.getcwd(),"mesh_y.inp"), "#mesh_layer_length0", str(tot),1)

	def layer_type_edit(self):
		for i in range(0,self.tab.rowCount()):
			if tab_get_value(self.tab,i,3)=="Active layer" and tab_get_value(self.tab,i,4).startswith("dos")==False:
				print("doing update")
				tab_set_value(self.tab,i,4,epitay_get_next_dos())
				tab_set_value(self.tab,i,5,epitay_get_next_pl())

				new_file=tab_get_value(self.tab,i,4)+".inp"
				if inp_isfile(new_file)==False:
					inp_copy_file(new_file,"dos0.inp")

				new_file=tab_get_value(self.tab,i,5)+".inp"
				if inp_isfile(new_file)==False:
					inp_copy_file(new_file,"pl0.inp")

			if tab_get_value(self.tab,i,3)!="Active layer" and tab_get_value(self.tab,i,4).startswith("dos")==True:
				tab_set_value(self.tab,i,4,tab_get_value(self.tab,i,3))
				tab_set_value(self.tab,i,5,"none")

		self.save_model()
		self.emit_change()


	def rebuild_mat_list(self):
		self.material_files=[]
		mat=find_materials()
		print(mat)
		for i in range(0,len(mat)):
			self.material_files.append(mat[i])
			scan_remove_file(os.path.join(get_materials_path(),mat[i]))			
			scan_item_add(os.path.join("materials",mat[i],"fit.inp"),"#wavelength_shift_alpha","Absorption spectrum wavelength shift",1)
			scan_item_add(os.path.join("materials",mat[i],"fit.inp"),"#n_mul","Refractive index spectrum multiplier",1)
			scan_item_add(os.path.join("materials",mat[i],"fit.inp"),"#alpha_mul","Absorption spectrum multiplier",1)

	def callback_view_materials(self):
		dialog=gpvdm_open(get_materials_path())
		dialog.show_inp_files=False
		ret=dialog.window.exec_()

		if ret==QDialog.Accepted:
			plot_gen([dialog.get_filename()],[],"auto")

	def on_move_down(self):
		tab_move_down(self.tab)
		self.changed.emit()
		
	def callback_edit_mesh(self):
		help_window().help_set_help(["mesh.png",_("<big><b>Mesh editor</b></big>\nUse this window to setup the mesh, the window can also be used to change the dimensionality of the simulation.")])

		if self.electrical_mesh.isVisible()==True:
			self.electrical_mesh.hide()
		else:
			self.electrical_mesh.show()

	def __init__(self):
		QWidget.__init__(self)
		self.rebuild_mat_list()
		self.doping_window=False
		self.contacts_window=False

		self.main_vbox=QVBoxLayout()

		self.toolbar=QToolBar()
		self.toolbar.setIconSize(QSize(32, 32))

		self.tb_add = QAction(QIcon(os.path.join(get_image_file_path(),"add.png")), _("Add device layer"), self)
		self.tb_add.triggered.connect(self.on_add_item_clicked)
		self.toolbar.addAction(self.tb_add)

		self.tb_remove = QAction(QIcon(os.path.join(get_image_file_path(),"minus.png")), _("Delete device layer"), self)
		self.tb_remove.triggered.connect(self.on_remove_item_clicked)
		self.toolbar.addAction(self.tb_remove)


		self.tb_remove= QAction(QIcon(os.path.join(get_image_file_path(),"down.png")), _("Move device layer"), self)
		self.tb_remove.triggered.connect(self.on_move_down)
		self.toolbar.addAction(self.tb_remove)

		self.tb_mesh = QAction(QIcon(os.path.join(get_image_file_path(),"mesh.png")), _("Edit the electrical mesh"), self)
		self.tb_mesh.triggered.connect(self.callback_edit_mesh)
		self.toolbar.addAction(self.tb_mesh)

		self.tb_doping = QAction(QIcon(os.path.join(get_image_file_path(),"doping.png")), _("Doping"), self)
		self.tb_doping.triggered.connect(self.callback_doping)
		self.toolbar.addAction(self.tb_doping)

		self.tb_contact = QAction(QIcon(os.path.join(get_image_file_path(),"contact.png")), _("Contacts"), self)
		self.tb_contact.triggered.connect(self.callback_contacts)
		self.toolbar.addAction(self.tb_contact)

		self.tb_open = QAction(QIcon(os.path.join(get_image_file_path(),"open.png")), _("Look at the materials database"), self)
		self.tb_open.triggered.connect(self.callback_view_materials)
		self.toolbar.addAction(self.tb_open)

		self.main_vbox.addWidget(self.toolbar)
	
		self.tab = QTableWidget()
		self.tab.resizeColumnsToContents()

		self.tab.verticalHeader().setVisible(False)
		self.create_model()

		self.tab.cellChanged.connect(self.tab_changed)

		self.main_vbox.addWidget(self.tab)

		self.electrical_mesh=tab_electrical_mesh()
		self.electrical_mesh.changed.connect(self.emit_change)
		self.setLayout(self.main_vbox)




	def create_model(self):
		self.tab.clear()
		self.tab.setColumnCount(6)
		self.tab.setColumnHidden(5, True)
		self.tab.setColumnHidden(4, True)
		self.tab.setSelectionBehavior(QAbstractItemView.SelectRows)
		self.tab.setHorizontalHeaderLabels([_("Layer name"), _("Thicknes"), _("Optical material"), _("Layer type"), _("DoS Layer"),_("PL Layer")])

		data1 = ['row1','row2','row3','row4']
		data2 = ['1','2.0','3.00000001','3.9999999']
		combo_box_options = ["Option 1","Option 2","Option 3"]

		self.tab.setRowCount(epitaxy_get_layers())

		for i in range(0,epitaxy_get_layers()):
			thick=epitaxy_get_width(i)
			material=epitaxy_get_mat_file(i)
			dos_layer=epitaxy_get_electrical_layer(i)
			pl_file=epitaxy_get_pl_file(i)
			name=epitaxy_get_name(i)

			dos_file=""
			
			if dos_layer.startswith("dos")==True:
				dos_file="Active layer"
			else:
				dos_file=dos_layer

			item1 = QTableWidgetItem(str(name))
			self.tab.setItem(i,0,item1)

			item2 = QTableWidgetItem(str(thick))
			self.tab.setItem(i,1,item2)

			combobox = QComboBox()
			#combobox.setEditable(True)

			for a in self.material_files:
				combobox.addItem(str(a))
			self.tab.setCellWidget(i,2, combobox)
			combobox.setCurrentIndex(combobox.findText(material))

			#item3 = QTableWidgetItem(str(dos_file))
			#self.tab.setItem(i,3,item3)
			combobox_layer_type = QComboBox()
			#combobox.setEditable(True)

			combobox_layer_type.addItem("Contact")
			combobox_layer_type.addItem("Active layer")
			combobox_layer_type.addItem("Other")

			self.tab.setCellWidget(i,3, combobox_layer_type)
			combobox_layer_type.setCurrentIndex(combobox_layer_type.findText(str(dos_file)))


			item3 = QTableWidgetItem(str(dos_layer))
			self.tab.setItem(i,4,item3)

			item3 = QTableWidgetItem(str(pl_file))
			self.tab.setItem(i,5,item3)


			scan_item_add("epitaxy.inp","#layer"+str(i),_("Material for ")+str(material),2)
			scan_item_add("epitaxy.inp","#layer"+str(i),_("Layer width ")+str(material),1)

			combobox.currentIndexChanged.connect(self.combo_changed)
			combobox_layer_type.currentIndexChanged.connect(self.layer_type_edit)
		return



	def on_remove_item_clicked(self):
		tab_remove(self.tab)
		self.save_model()
		self.changed.emit()

	def change_active_layer_thickness(self,obj):
		thickness=obj.get_data("refresh")
		print(thickness)
		count=0
		for item in self.model:
			if str2bool(item[COLUMN_DEVICE])==True:
				count=count+1

		if count==1:
			for item in self.model:
				if str2bool(item[COLUMN_DEVICE])==True:
					item[COLUMN_THICKNES]=str(thickness)
					self.save_model()
					self.refresh(False)
					return

	def on_add_item_clicked(self):
		tab_add(self.tab,[_("layer name"),_("100e-9"),_("pcbm"),_("Other"),_("none"),_("none")])
		self.save_model()
		self.changed.emit()

	def save_model(self):

		thick=[]
		mat_file=[]
		dos_file=[]
		pl_file=[]
		name=[]

		for i in range(0,self.tab.rowCount()):
			name.append(str(tab_get_value(self.tab,i, 0)))
			thick.append(str(tab_get_value(self.tab,i, 1)))
			mat_file.append(str(tab_get_value(self.tab,i, 2)))
			dos_file.append(str(tab_get_value(self.tab,i, 4)))
			pl_file.append(str(tab_get_value(self.tab,i, 5)))

		print(dos_file)
		print(pl_file)
		epitaxy_load_from_arrays(name,thick,mat_file,dos_file,pl_file)

		epitaxy_save()
		#self.sync_to_electrical_mesh()

	def callback_doping(self):
		help_window().help_set_help(["doping.png",_("<big><b>Doping window</b></big>\nUse this window to add doping to the simulation")])

		if self.doping_window==False:
			self.doping_window=doping_window()

		if self.doping_window.isVisible()==True:
			self.doping_window.hide()
		else:
			self.doping_window.show()

	def callback_contacts(self):
		help_window().help_set_help(["contact.png",_("<big><b>Contacts window</b></big>\nUse this window to change the layout of the contacts on the device")])

		if self.contacts_window==False:
			self.contacts_window=contacts_window()

		if self.contacts_window.isVisible()==True:
			self.contacts_window.hide()
		else:
			self.contacts_window.show()


