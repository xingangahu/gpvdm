#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012 Roderick C. I. MacKenzie
#
#	roderick.mackenzie@nottingham.ac.uk
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

import pygtk
pygtk.require('2.0')
import gtk
import sys
import os
import shutil
import commands
import subprocess
from win_lin import running_on_linux
from cal_path import get_exe_command
import urllib2
import socket 
from threading import Thread
import time
import urlparse
import re
import os
from ver import ver_core
from ver import ver_mat
from ver import ver_gui
import gobject
import platform
import getpass
from help import my_help_class
from http import get_data_from_web 
from cal_path import get_share_path
import hashlib

def sp(value):
	return value.split(os.sep)

def update_now():
	print _("Checking web for updates")
	disk_files=[]
	web_src=[]
	disk_dest=[]

	update_path="http://www.gpvdm.com/update_windows/"
	lines=get_data_from_web(update_path+"list.dat")
	print "Got file list"
	lines=lines.split('\n')
	files=[]
	md5=[]
	web_md5=[]
	for i in range(0,len(lines)):
		if lines[i].count("  ")!=0:
			m,f=lines[i].split("  ")
			f=f[2:].split("/")
			md5.append(m)
			files.append(f)

	for i in range(0,len(files)):

		root=files[i][0]
		if root=="device_lib":
		#if root=="images" or root=="solvers" or root=="gpvdm_core.exe" or root=="device_lib" or root=="sim.gpvdm" or root=="lang" or root=="materials" or root=="light":
			md5_web=md5[i]
			md5_disk="none"
			disk_path=os.path.join(get_share_path(),"/".join(files[i]))
			web_path=update_path+"/".join(files[i])
			if os.path.isfile(disk_path):
				md5_disk=hashlib.md5(open(disk_path,'rb').read()).hexdigest()

			#if md5_web!=md5_disk:
			web_src.append(web_path)
			disk_dest.append(disk_path)
			web_md5.append(md5_web)

	for i in range(0,len(web_src)):
		print web_src[i],disk_dest[i]
		a=get_data_from_web(web_src[i])
		l=len(a)
		if l>100:
			l=100;
		if a[:l].count("403 Forbidden")!=0:
			print "Access to file "+web_src[i]+" forbiden"
		else:
			web_hash=hashlib.md5(a).hexdigest()
			list_hash=web_md5[i]
			if web_hash==list_hash:
				print "updating file",disk_dest[i]
				f=open(disk_dest[i], mode='wb')
				lines = f.write(a)
				f.close()
			else:
				print "Checksum error",disk_dest[i]		

class update_thread(gtk.VBox):
	def __init__(self):
		self.__gobject_init__()
		self.text=""

	def get_from_web(self,url):
			page="http://www.gpvdm.com/download_windows/update.php?ver_core="+ver_core()+"&ver_gui="+ver_gui()+"&ver_mat="+ver_mat()+"&os="+platform.platform()
			message=get_data_from_web(page)

			message=message.split("\n")
			print message
			self.text=""
			if message[0].startswith("update"):
				token,ver=message[0].split("#")
				self.text="Version "+ver+" of opvdm is now avaliable."
			gobject.idle_add(gobject.GObject.emit,self,"got-data")
			#self.emit("got-data")

	def foo(self,n):
		self.get_from_web('http://www.gpvdm.com')

	def start(self):
		p = Thread(target=self.foo, args=(10,))
		#multiprocessing.Process(target=self.foo, name="Foo", args=(10,))
		p.daemon = True
		p.start()


gobject.type_register(update_thread)
gobject.signal_new("got-data", update_thread, gobject.SIGNAL_RUN_FIRST,gobject.TYPE_NONE, ())
