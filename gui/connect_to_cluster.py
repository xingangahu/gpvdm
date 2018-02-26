#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
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

import i18n
_ = i18n.language.gettext

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QPushButton,QCheckBox,QHBoxLayout,QLabel,QWidget,QDialog,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QSystemTrayIcon,QMenu,QListWidget,QListWidgetItem
from PyQt5.QtGui import QIcon
from PyQt5.uic import loadUi

#calpath
from cal_path import get_device_lib_path
from icon_lib import QIcon_load
from cal_path import get_ui_path
from gui_util import error_dlg
from cal_path import get_exe_path

from help import help_window
from inp import inp_load_file
from inp import inp_get_token_value_from_list
from inp import inp_get_token_value

from util import str2bool
from gui_util import dlg_get_text

from tab import tab_class

class connect_to_cluster(QDialog):

	def callback_close(self, widget, data=None):
		self.reject()

	def callback_connect(self):
		self.accept()

	def __init__(self):
		QDialog.__init__(self)
		self.main_vbox=QVBoxLayout()
		self.setFixedSize(600,450) 
		self.setWindowTitle(_("Connect to cluster")+" (https://www.gpvdm.com)")
		self.setWindowIcon(QIcon_load("si"))
		#self.title=QLabel("<big><b>"+_("Which type of device would you like to simulate?")+"</b></big>")

		self.viewer=tab_class()
		self.viewer.init("cluster.inp","cluster")
				#gpvdm_viewer(get_device_lib_path())

		self.main_vbox.addWidget(self.viewer)

		self.hwidget=QWidget()

		self.deploy= QPushButton(_("Deploy to cluster"))
		self.nextButton = QPushButton(_("Connect"))
		self.cancelButton = QPushButton(_("Cancel"))

		self.files=[]

		hbox = QHBoxLayout()

		hbox.addWidget(self.deploy)
		hbox.addStretch(1)
		hbox.addWidget(self.cancelButton)
		hbox.addWidget(self.nextButton)
		self.hwidget.setLayout(hbox)

		self.main_vbox.addWidget(self.hwidget)

		self.setLayout(self.main_vbox)

		self.nextButton.clicked.connect(self.callback_connect)
		self.cancelButton.clicked.connect(self.callback_close)



