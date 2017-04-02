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

from __future__ import unicode_literals

import os
import io
from numpy import *
from plot_io import plot_load_info
from plot_export import plot_export


#matplotlib
import matplotlib
from matplotlib import cm
matplotlib.use('Qt5Agg')
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT as NavigationToolbar
import matplotlib.pyplot as plt
from matplotlib.pyplot import colorbar

from util import numbers_to_latex
from util import pygtk_to_latex_subscript
from util import fx_with_units
from plot_io import plot_save_oplot_file
from util import time_with_units

#qt
from PyQt5.QtCore import QSize, Qt 
from PyQt5.QtWidgets import QWidget,QVBoxLayout,QToolBar,QSizePolicy,QAction,QTabWidget,QTableWidget,QAbstractItemView, QMenuBar,QApplication
from PyQt5.QtGui import QPainter,QIcon,QImage

#calpath
from icon_lib import QIcon_load
from open_save_dlg import save_as_image

from dat_file import dat_file_read
from dat_file_math import dat_file_max_min
from dat_file_math import dat_file_sub
from dat_file_math import dat_file_mul
from dat_file_math import dat_file_sub_float
from dat_file import dat_file
from dat_file import read_data_2d


from dlg_get_multi_text import dlg_get_multi_text

from mpl_toolkits.mplot3d import Axes3D


class plot_widget(QWidget):

	def keyPressEvent(self, event):
		keyname=event.key()
		if (keyname>64 and keyname<91 ) or (keyname>96 and keyname<123):
			modifiers = event.modifiers()
			keyname=chr(keyname)
			if keyname.isalpha()==True:
				if Qt.ShiftModifier == modifiers:
					keyname=keyname.upper()
				else:
					keyname=keyname.lower()
		else:
			return

		if keyname=="a":
			self.do_plot()

		elif keyname=='g':
			if self.data[0].grid==False:
				for i in range(0,len(self.ax)):
					self.ax[i].grid(True)
				self.data[0].grid=True
			else:
				for i in range(0,len(self.ax)):
					self.ax[i].grid(False)
				self.data[0].grid=False
		elif keyname=="r":
			if self.lx==None:
				for i in range(0,len(self.ax)):
					self.lx = self.ax[i].axhline(color='k')
					self.ly = self.ax[i].axvline(color='k')
			self.lx.set_ydata(self.ydata)
			self.ly.set_xdata(self.xdata)

		elif keyname=="l":
			if self.data[0].logy==True:
				self.data[0].logy=False
				for i in range(0,len(self.ax)):
					self.ax[i].set_yscale("linear")
			else:
				self.data[0].logy=True
				for i in range(0,len(self.ax)):
					self.ax[i].set_yscale("log")

		elif keyname=="L":
			if self.data[0].logx==True:
				self.data[0].logx=False
				for i in range(0,len(self.ax)):
					self.ax[i].set_xscale("linear")
			else:
				self.data[0].logx=True
				for i in range(0,len(self.ax)):
					self.ax[i].set_xscale("log")

		elif keyname=="q":
			self.destroy()

		elif  modifiers == Qt.ControlModifier and keyname=='c':
			self.do_clip()

		self.fig.canvas.draw()
		plot_save_oplot_file(self.config_file,self.data[0])

	def do_clip(self):
		buf = io.BytesIO()
		self.fig.savefig(buf)
		QApplication.clipboard().setImage(QImage.fromData(buf.getvalue()))
		buf.close()


	def mouse_move(self,event):
		self.xdata=event.xdata
		self.ydata=event.ydata



	def do_plot(self):
		if len(self.data)==0:
			return
		
		if self.data[0].valid_data==False:
			return

		self.fig.clf()
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(bottom=0.2)
		self.fig.subplots_adjust(left=0.1)
		self.fig.subplots_adjust(hspace = .001)
		dim=""
		if self.data[0].x_len==1 and self.data[0].z_len==1:
			dim="linegraph"
		elif self.data[0].x_len>1 and self.data[0].y_len>1 and self.data[0].z_len==1:
			if self.data[0].type=="3d":
				dim="wireframe"
			if self.data[0].type=="heat":
				dim="heat"
		elif self.data[0].x_len>1 and self.data[0].y_len>1 and self.data[0].z_len>1:
			print("ohhh full 3D")
			dim="3d"
		else:
			print(_("I don't know how to process this type of file!"),self.data[0].x_len, self.data[0].y_len,self.data[0].z_len)
			return

		title=self.data[0].title
		if self.data[0].time!=-1.0 and self.data[0].Vexternal!=-1.0:
			mul,unit=time_with_units(self.data[0].time)
			title=self.data[0].title+" V="+str(self.data[0].Vexternal)+" "+_("time")+"="+str(self.data[0].time*mul)+" "+unit

		self.fig.suptitle(title)

		self.setWindowTitle(title+" - www.gpvdm.com")

		self.ax=[]


		for i in range(0,len(self.input_files)):
			if dim=="linegraph":
				self.ax.append(self.fig.add_subplot(111,axisbg='white'))
			elif dim=="wireframe":
				self.ax.append(self.fig.add_subplot(111,axisbg='white' ,projection='3d'))
			elif dim=="heat":
				self.ax.append(self.fig.add_subplot(111,axisbg='white'))
			elif dim=="3d":
				self.ax.append(self.fig.add_subplot(111,axisbg='white' ,projection='3d'))
			#Only place label on bottom plot
			#	if self.data[i].type=="3d":
			#else:
			#	self.ax[i].tick_params(axis='x', which='both', bottom='off', top='off',labelbottom='off') # labels along the bottom edge are off

			#Only place y label on center plot
			if self.data[0].normalize==True or self.data[0].norm_to_peak_of_all_data==True:
				y_text="Normalized "+self.data[0].data_label
				data_units="au"
			else:
				data_text=self.data[i].data_label
				data_units=self.data[i].data_units

			if self.data[0].logx==True:
				self.ax[i].set_xscale("log")

			if self.data[0].logy==True:
				self.ax[i].set_yscale("log")


		all_plots=[]
		files=[]
		my_max=1.0

			
		if dim=="linegraph":		#This is for the 1D graph case
			self.ax[0].set_xlabel(self.data[0].x_label+" ("+str(self.data[0].x_units)+")")
			self.ax[0].set_ylabel(self.data[0].data_label+" ("+self.data[0].data_units+")")

			for i in range(0,len(self.input_files)):
				cur_plot, = self.ax[i].plot(self.data[i].y_scale,self.data[i].data[0][0], linewidth=3 ,alpha=1.0,color=self.color[i],marker=self.marker[i])

				if self.labels[i]!="":
					files.append("$"+numbers_to_latex(str(self.labels[i]))+" "+pygtk_to_latex_subscript(self.data[0].key_units)+"$")

				all_plots.append(cur_plot)

		elif dim=="wireframe":
			self.ax[0].set_xlabel(self.data[0].x_label+" ("+self.data[0].x_units+")")
			self.ax[0].set_ylabel(self.data[0].y_label+" ("+self.data[0].y_units+")")

			for i in range(0,len(self.input_files)):

				#new_data=[[float for y in range(self.data[0].y_len)] for x in range(self.data[0].x_len)]
				#for x in range(0,self.data[i].x_len):
				#	for y in range(0,self.data[i].y_len):
				#		print(x,y,len(self.data[i].data[0]),len(self.data[i].data[0][0]))
				#		new_data[x][y]=self.data[i].data[0][x][y]
				#z = 10 * outer(ones(size(data.x_scale)), cos(data.y_scale))
				#im=self.ax[0].plot_surface(data.x_scale,data.y_scale,z)
				#print(new_data)
				#print(self.data[i].x_scale)
				#print(self.data[i].y_scale)
				X, Y = meshgrid( self.data[i].y_scale,self.data[i].x_scale)
				Z = self.data[i].data[0]

				# Plot the surface
				im=self.ax[i].plot_wireframe( Y,X, Z)

				#pcolor
		elif dim=="heat":
			self.ax[0].set_xlabel(self.data[0].x_label+" ("+self.data[0].x_units+")")
			self.ax[0].set_ylabel(self.data[0].y_label+" ("+self.data[0].y_units+")")

			for i in range(0,len(self.input_files)):

				im=self.ax[0].pcolor(self.data[i].y_scale,self.data[i].x_scale,self.data[i].data[0])
				self.fig.colorbar(im)

				#pcolor

				#self.fig.colorbar(im, shrink=0.5, aspect=5)
				#self.ax[0].plot_surface(x, y, z, rstride=1, cstride=1, cmap=cm.coolwarm,linewidth=0, antialiased=False)
				#self.ax[0].invert_yaxis()
				#self.ax[0].xaxis.tick_top()
		elif dim=="3d":
			self.ax[0].set_xlabel(self.data[0].x_label+" ("+self.data[0].x_units+")")
			self.ax[0].set_ylabel(self.data[0].y_label+" ("+self.data[0].y_units+")")
			self.ax[0].set_zlabel(self.data[0].z_label+" ("+self.data[0].z_units+")")

			for ii in range(0,len(self.data[i].z_scale)):
				my_max,my_min=dat_file_max_min(self.data[i])
				X, Y = meshgrid( self.data[i].x_scale,self.data[i].y_scale)
				new_data=[[float for y in range(self.data[0].y_len)] for x in range(self.data[0].x_len)]
				for x in range(0,self.data[i].x_len):
					for y in range(0,self.data[i].y_len):
						new_data[x][y]=self.data[i].z_scale[ii]+self.data[i].data[ii][x][y]
				self.ax[i].contourf(X, Y, new_data, zdir='z')#

				self.ax[i].set_xlim3d(0, self.data[i].x_scale[-1])
				self.ax[i].set_ylim3d(0, self.data[i].y_scale[-1])
				self.ax[i].set_zlim3d(0, self.data[i].z_scale[-1])

		#setup the key
		if self.data[0].legend_pos=="No key":
			self.ax[i].legend_ = None
		else:
			self.fig.legend(all_plots, files, self.data[0].legend_pos)
			
		#self.fig.tight_layout(pad=0.0, w_pad=0.0, h_pad=0.0)
		self.fig.canvas.draw()
		#print("exit do plot")

	def callback_plot_save(self):
		response=save_as_image(self)
		if response != None:
			plot_export(response,self.input_files,self.data[0],self.fig)

	def set_labels(self,labels):
		self.labels=labels

	def load_data(self,input_files,config_file):
		self.lx=None
		self.ly=None
		self.input_files=input_files
		self.config_file=config_file

		if self.config_file=="":
			self.config_file=os.path.splitext(input_files[0])[0]+".oplot"

		self.data=[]

		
		for i in range(0,len(self.input_files)):
			dat=dat_file()
			ret=dat_file_read(dat,self.input_files[i])
			self.data.append(dat)

		#Try and get the data from the config file
		self.norm_data()
#		if plot_load_info(self.data[0],self.config_file)==True:
#			print("I have updated the plot info",self.data[0].type)


#		if self.data[0].tag0=="":
#			self.data[0].file0=os.path.basename(input_files[0])

#		plot_save_oplot_file(self.config_file,self.data[0])



	def norm_data(self):
		if len(self.data)>0:
			if self.zero_frame_enable==True:
				if len(self.input_files)>1:
					for i in range(1,len(self.input_files)):
						dat_file_sub(self.data[i],self.data[0])
					
					dat_file_sub(self.data[0],self.data[0])

			for i in range(0,len(self.input_files)):
				for x in range(0,self.data[i].x_len):
					self.data[i].x_scale[x]=self.data[i].x_scale[x]*self.data[i].x_mul

				for y in range(0,self.data[i].y_len):
					self.data[i].y_scale[y]=self.data[i].y_scale[y]*self.data[i].y_mul

				for z in range(0,self.data[i].z_len):
					self.data[i].z_scale[z]=self.data[i].z_scale[z]*self.data[i].z_mul

			if self.data[0].invert_y==True:
				for i in range(0,len(self.input_files)):
					dat_file_mul(self.data[i],-1)

			if self.data[0].subtract_first_point==True:
				val=self.data[0].data[0][0][0]
				for i in range(0,len(self.input_files)):
					dat_file_sub_float(self.data[i],val)


			if self.data[0].add_min==True:
				my_max,my_min=dat_file_max_min(self.data[0])
				for i in range(0,len(self.input_files)):
					dat_file_sub_float(self.data[i],my_min)


			#if self.plot_token.normalize==True:
				#my_max,my_min=dat_file_max_min(self.data[0])
				#for (i in range(0,len(self.input_files)):
				#for x in range(0,data.x_len):
					#for y in range(0,data.y_len):
						#for z in range(0,data.z_len):
							#if data.data[z][x][y]!=0:
								#data.data[z][x][y]=data.data[z][x][y]/my_max
							#else:
								#data.data[z][x][y]=0.0


			#if self.plot_token.ymax!=-1:
				#self.ax[index].set_ylim((self.plot_token.ymin,self.plot_token.ymax))
			#return True
		#else:
			#return False
	
##norm stuff
			#all_max=1.0
			#if self.plot_token.norm_to_peak_of_all_data==True:
				#my_max=-1e40
				#for i in range(0, len(self.input_files)):
					#local_max,my_min=dat_file_max_min(self.data[i])
					#if local_max>my_max:
						#my_max=local_max
				#all_max=my_max

			#for i in range(0, len(self.input_files)):
				#if all_max!=1.0:
					#for x in range(0,data.x_len):
						#for y in range(0,data.y_len):
							#for z in range(0,data.z_len):
								#data.data[z][x][y]=data.data[z][x][y]/all_max

	def gen_colors_black(self,repeat_lines):
		#make 100 black colors
		marker_base=["","x","o"]
#		c_tot=[]
		base=[[0.0,0.0,0.0]]
		self.marker=[]
		self.color=[]
		for i in range(0,100):
			for n in range(0,repeat_lines):
				self.color.append([base[0][0],base[0][1],base[0][2]])
				self.marker.append(marker_base[n])

	def gen_colors(self,repeat_lines):
		base=[[0.0,0.0,1.0],[0.0,1.0,0.0],[1.0,0.0,0.0],[0.0,1.0,1.0],[1.0,1.0,0.0],[1.0,0.0,1.0]]
		c_tot=[]
		self.marker=[]
		marker_base=["","x","o"]
		mul=1.0
		self.color=[]
		for rounds in range(0,20):
			for i in range(0,len(base)):
				for n in range(0,repeat_lines):
					c_tot.append([base[i][0]*mul,base[i][1]*mul,base[i][2]*mul])
					self.marker.append(marker_base[n])
			mul=mul*0.5

		self.color=c_tot

	def callback_black(self):
		self.gen_colors_black(1)
		plot_save_oplot_file(self.config_file,self.data[0])
		self.do_plot()

	def callback_rainbow(self):
		self.gen_colors(1)
		plot_save_oplot_file(self.config_file,self.data[0])
		self.do_plot()

	def callback_save(self):
		output_file=os.path.splitext(self.config_file)[0]+".png"
		plot_export(output_file,self.input_files,self.data[0],self.fig)

	def callback_key(self):
		self.data[0].legend_pos=widget.get_label()
		#print(self.config_file,self.data[0])
		plot_save_oplot_file(self.config_file,self.data[0])
		self.do_plot()

	def callback_units(self):
		units=dlg_get_text( "Units:", self.data[0].key_units)
		if units!=None:
			self.data[0].key_units=units
		plot_save_oplot_file(self.config_file,self.data[0])
		self.do_plot()


	def callback_autoscale_y(self):
		if self.data[0].ymax==-1:
			xmin, xmax, ymin, ymax = self.ax[0].axis()
			self.data[0].ymax=ymax
			self.data[0].ymin=ymin
		else:
			self.data[0].ymax=-1
			self.data[0].ymin=-1

	def callback_normtoone_y(self):
		self.data[0].normalize= not self.data[0].normalize
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def callback_norm_to_peak_of_all_data(self):
		self.data[0].norm_to_peak_of_all_data=not self.data[0].norm_to_peak_of_all_data
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def callback_toggle_log_scale_y(self):
		self.data[0].logy=not self.data[0].logy
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def callback_toggle_log_scale_x(self):
		self.data[0].logx=not self.data[0].logx
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def callback_toggle_label_data(self):
		self.data[0].label_data=not self.data[0].label_data
		plot_save_oplot_file(self.config_file,self.data[0])
		self.do_plot()

	def callback_set_heat_map(self):
		self.data[0].type="heat"
		plot_save_oplot_file(self.config_file,self.data[0])
		plot_save_oplot_file(self.config_file,self.data[0])
		self.do_plot()

	def callback_heat_map_edit(self):
		ret = dlg_get_multi_text([["x start",str(self.data[0].x_start)],["x stop",str(self.data[0].x_stop)],["x points",str(self.data[0].x_points)],["y start",str(self.data[0].y_start)],["y stop",str(self.data[0].y_stop)],["y points",str(self.data[0].y_points)]],title="2D plot editor")
		ret.run()
		ret=ret.get_values()
		if ret!=False:
			[a,b,c,d,e,f] = ret
			#print("---------",a,b,c,d,e,f)
			self.data[0].x_start=float(a)
			self.data[0].x_stop=float(b)
			self.data[0].x_points=float(c)

			self.data[0].y_start=float(d)
			self.data[0].y_stop=float(e)
			self.data[0].y_points=float(f)

			plot_save_oplot_file(self.config_file,self.data[0])
			self.do_plot()


	def callback_toggle_invert_y(self):
		self.data[0].invert_y=not self.data[0].invert_y
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def callback_toggle_subtract_first_point(self):
		self.data[0].subtract_first_point=not self.data[0].subtract_first_point
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def callback_toggle_add_min(self):
		self.data[0].add_min=not self.data[0].add_min
		plot_save_oplot_file(self.config_file,self.data[0])
		self.norm_data()
		self.do_plot()

	def update(self):
		self.load_data(self.input_files,self.config_file)
		self.do_plot()

	def callback_refresh(self):
		self.update()

	def init(self,menu=True):
		self.main_vbox = QVBoxLayout()
		self.config_file=""
		self.labels=[]
		self.fig = Figure(figsize=(2.5,2), dpi=100)
		self.canvas = FigureCanvas(self.fig)  # a gtk.DrawingArea

		self.zero_frame_enable=False
		self.zero_frame_list=[]
		self.gen_colors(1)

		toolbar=QToolBar()
		toolbar.setIconSize(QSize(48, 48))


		self.tb_save = QAction(QIcon_load("document-save-as"), _("Save graph"), self)
		self.tb_save.triggered.connect(self.callback_plot_save)
		toolbar.addAction(self.tb_save)

		self.tb_refresh = QAction(QIcon_load("view-refresh"), _("Refresh graph"), self)
		self.tb_refresh .triggered.connect(self.callback_refresh)
		toolbar.addAction(self.tb_refresh )

		nav_bar=NavigationToolbar(self.canvas,self)
		toolbar.addWidget(nav_bar)


		

		self.fig.canvas.mpl_connect('motion_notify_event', self.mouse_move)

		if menu==True:
			menubar = QMenuBar()

			file_menu = menubar.addMenu(_("File"))

			self.menu_save=file_menu.addAction("&"+_("Save"))
			self.menu_save.triggered.connect(self.callback_save)

			self.menu_save_as=file_menu.addAction("&"+_("Save as"))
			self.menu_save_as.triggered.connect(self.callback_plot_save)


			key_menu = menubar.addMenu("&"+_("Key"))

			key_menu = menubar.addMenu("&"+_("Color"))
			self.menu_black=key_menu.addAction("&"+_("Black"))
			self.menu_black.triggered.connect(self.callback_black)

			self.menu_rainbow=key_menu.addAction("&"+_("Rainbow"))
			self.menu_rainbow.triggered.connect(self.callback_rainbow)

			axis_menu = menubar.addMenu("&"+_("Color"))
			menu=axis_menu.addAction("&"+_("Autoscale"))
			menu.triggered.connect(self.callback_autoscale_y)

			menu=axis_menu.addAction("&"+_("Set log scale y"))
			menu.triggered.connect(self.callback_toggle_log_scale_y)

			menu=axis_menu.addAction("&"+_("Set log scale x"))
			menu.triggered.connect(self.callback_toggle_log_scale_x)

			menu=axis_menu.addAction("&"+_("Set log scale x"))
			menu.triggered.connect(self.callback_toggle_log_scale_x)

			self.menu_rainbow=key_menu.addAction("&"+_("Label data"))
			self.menu_rainbow.triggered.connect(self.callback_toggle_label_data)

			math_menu = menubar.addMenu("&"+_("Math"))

			menu=math_menu.addAction("&"+_("Subtract first point"))
			menu.triggered.connect(self.callback_toggle_subtract_first_point)

			menu=math_menu.addAction("&"+_("Add min point"))
			menu.triggered.connect(self.callback_toggle_add_min)

			menu=math_menu.addAction("&"+_("Invert y-axis"))
			menu.triggered.connect(self.callback_toggle_invert_y)
			
			menu=math_menu.addAction("&"+_("Norm to 1.0 y"))
			menu.triggered.connect(self.callback_normtoone_y)
			
			menu=math_menu.addAction("&"+_("Norm to peak of all data"))
			menu.triggered.connect(self.callback_norm_to_peak_of_all_data)
			
			menu=math_menu.addAction("&"+_("Heat map"))
			menu.triggered.connect(self.callback_set_heat_map)

			menu=math_menu.addAction("&"+_("Heat map edit"))
			menu.triggered.connect(self.callback_heat_map_edit)

			self.main_vbox.addWidget(menubar)
		
		self.main_vbox.addWidget(toolbar)


		self.canvas.figure.patch.set_facecolor("white")
		self.canvas.setMinimumSize(800, 350)
		self.main_vbox.addWidget(self.canvas)

		self.setLayout(self.main_vbox)



