#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012 Roderick C. I. MacKenzie <r.c.i.mackenzie@googlemail.com>
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

from scan_item import scan_item_add
from token_lib import tokens
from undo import undo_list_class
from tab_base import tab_base
from util import str2bool
from scan_item import scan_remove_file
from inp import inp_load_file
from inp import inp_update_token_value
from inp import inp_get_token_value
from util import latex_to_html
from i18n import yes_no
from cal_path import get_image_file_path
from gtkswitch import gtkswitch
from leftright import leftright
from help import help_window

from PyQt5.QtCore import pyqtSignal

from PyQt5.QtWidgets import QWidget, QScrollArea,QVBoxLayout,QProgressBar,QLabel,QDesktopWidget,QToolBar,QHBoxLayout,QAction, QSizePolicy, QTableWidget, QTableWidgetItem,QComboBox,QDialog,QAbstractItemView,QGridLayout,QLineEdit

import i18n
_ = i18n.language.gettext

import functools

class tab_class(QScrollArea,tab_base):

	lines=[]
	edit_list=[]
	changed = pyqtSignal()
		
	def __init__(self):
		QScrollArea.__init__(self)
		self.editable=True


	def callback_edit(self, file_name,token,widget):
		if type(widget)==QLineEdit:
			a=undo_list_class()
			a.add([file_name, token, inp_get_token_value(self.file_name, token),widget])
			inp_update_token_value(file_name, token, widget.text(),1)
		elif type(widget)==gtkswitch:
			inp_update_token_value(file_name, token, widget.get_value(),1)
		elif type(widget)==leftright:
			inp_update_token_value(file_name, token, widget.get_value(),1)
		elif type(widget)==QComboBox:
			inp_update_token_value(file_name, token, widget.itemText(widget.currentIndex()),1)
		
		help_window().help_set_help(["32_save.png","<big><b>Saved to disk</b></big>\n"])
		
		self.changed.emit()

	def help(self):
		help_window().get_help(self.file_name)

	def set_edit(self,editable):
		self.editable=editable
		
	def init(self,filename,tab_name):
		self.main_widget=QWidget()
		self.vbox=QVBoxLayout()
		self.file_name=filename
		self.tab_name=tab_name

		self.tab=QGridLayout()
		widget=QWidget()
		widget.setLayout(self.tab)
		self.vbox.addWidget(widget)

		scan_remove_file(filename)

		self.edit_list=[]
		inp_load_file(self.lines,filename)

		n=0
		pos=0
		my_token_lib=tokens()
		height=27
		widget_number=0
		while (pos<len(self.lines)):
			token=self.lines[pos]
			if token=="#ver":
				break

			if token=="#end":
				break

			if token.startswith("#"):
				show=False
				units="Units"

				pos=pos+1
				value=self.lines[pos]

				result=my_token_lib.find(token)
				if result!=False:
					units=result.units
					text_info=result.info
					show=True
				
				#self.set_size_request(600,-1)
				if show == True :
					description=QLabel()
					description.setText(latex_to_html(text_info))


					if result.opt[0]=="switch":
						edit_box=gtkswitch()
						edit_box.set_value(str2bool(value))
						edit_box.changed.connect(functools.partial(self.callback_edit,filename,token,edit_box))
					elif result.opt[0]=="leftright":
						edit_box=leftright()
						edit_box.set_value(str2bool(value))
						edit_box.changed.connect(functools.partial(self.callback_edit,filename,token,edit_box))
					elif result.opt[0]=="text":
						edit_box=QLineEdit()
						if self.editable==False:
							edit_box.setReadOnly(True)
						edit_box.setText(value)
						#edit_box.set_text(self.lines[pos]);
						edit_box.textChanged.connect(functools.partial(self.callback_edit,filename,token,edit_box))
						#edit_box.show()
					else:
						edit_box=QComboBox()
						for i in range(0,len(result.opt)):
							edit_box.addItem(result.opt[i])
							
						all_items  = [edit_box.itemText(i) for i in range(edit_box.count())]
						for i in range(0,len(all_items)):
							if all_items[i] == token:
								edit_box.setCurrentIndex(i)
								
						edit_box.currentIndexChanged.connect(functools.partial(self.callback_edit,filename,token,edit_box))

					edit_box.setFixedSize(300, 25)
					unit=QLabel()
					unit.setText(latex_to_html(units))


					self.tab.addWidget(description,widget_number,0)
					self.tab.addWidget(edit_box,widget_number,1)
					self.tab.addWidget(unit,widget_number,2)
					
					scan_item_add(filename,token,text_info,1)
					
					widget_number=widget_number+1
			pos=pos+1

		spacer = QWidget()
		spacer.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Expanding)
		self.vbox.addWidget(spacer)
		self.main_widget.setLayout(self.vbox)
		
		self.setWidget(self.main_widget)

