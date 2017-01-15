#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012-2016 Roderick C. I. MacKenzie <r.c.i.mackenzie@googlemail.com>
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

import sys
open_gl_ok=False

try:
	from OpenGL.GL import *
	from OpenGL.GLU import *
	from PyQt5 import QtOpenGL
	from PyQt5.QtOpenGL import QGLWidget
	open_gl_ok=True
except:
	print("opengl error ",sys.exc_info()[0])

from PyQt5 import QtGui
from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import QWidget, QHBoxLayout

import os

#inp
from inp import inp_load_file
from inp_util import inp_search_token_value

#path
from cal_path import get_materials_path

#contacts
from contacts_io import contacts_get_contacts
from contacts_io import contacts_get_array

#mesh
from mesh import mesh_get_xpoints
from mesh import mesh_get_ypoints
from mesh import mesh_get_zpoints
from mesh import mesh_get_xlen
from mesh import mesh_get_ylen
from mesh import mesh_get_zlen

#epitaxy
from epitaxy import epitaxy_get_layers
from epitaxy import epitaxy_get_width
from epitaxy import epitaxy_get_mat_file
from epitaxy import epitaxy_get_electrical_layer
from epitaxy import epitaxy_get_pl_file
from epitaxy import epitaxy_get_name

#qt
from PyQt5.QtGui import QFont
import numpy as np
from inp import inp_get_token_value
from util import str2bool

from PyQt5.QtCore import QTimer

import random
from math import pi,acos,sin,cos

from dat_file import dat_file
from dat_file import dat_file_read
from dat_file import dat_file_max_min

from lines import lines_read
from math import sqrt
from math import fabs
from epitaxy import epitaxy_get_device_start

from util import wavelength_to_rgb
import glob

# Rotations for cube.
cube_rotate_x_rate = 0.2
cube_rotate_y_rate = 0.2
cube_rotate_z_rate = 0.2

# Rotation rates for the tetrahedron.
tet_x_rate = 0.0
tet_y_rate = 1.0
tet_z_rate = 0.5
tet_rotate_step = 10.0

def tab(x,y,z,w,h,d):


	glBegin(GL_QUADS)
	glColor3f(0.0,0.0,1.0)
	glVertex3f(x+w+0.05,y,z)
	glVertex3f(x+w+0.2,y ,z)
	glVertex3f(x+w+0.2,y+h ,z)
	glVertex3f(x+w+0.05,y+h ,z)

	glEnd()

stars=[]

def draw_stars():
	global stars
	if len(stars)==0:
		
		for i in range(0,5000):
			phi = random.uniform(0,2*pi)
			costheta = random.uniform(-1,1)
			theta = acos( costheta )
			r=70+random.uniform(0,300)
			x = r * sin( theta) * cos( phi )
			y = r * sin( theta) * sin( phi )
			z = r * cos( theta )
			color=random.uniform(0,1.0)
			r=color
			g=color
			b=color
			s=random.uniform(1,3)	
			stars.append([x,y,z,r,g,b,s])
	
		stars.append([x,4,z,0.5,0.0,0.0,5])
		
	for i in range(0,len(stars)):
		glPointSize(stars[i][6])
		glBegin(GL_POINTS)
		glColor3f(stars[i][3],stars[i][4],stars[i][5])
		glVertex3f(stars[i][0],stars[i][1],stars[i][2])
		#glVertex3f(-1.0,-1.0,0.0)
		glEnd()


def draw_grid():
	glLineWidth(1)


	glColor3f(0.5, 0.5, 0.5)

	start_x=-18.0
	stop_x=20.0
	n=int(stop_x-start_x)
	dx=1.0#(stop_x-start_x)/n
	pos=start_x
	glBegin(GL_LINES)
	for i in range(0,n+1):
		glVertex3f(start_x, 0.0, pos)
		glVertex3f(stop_x, 0.0, pos)
		pos=pos+dx


	start_z=-18.0
	stop_z=20.0
	dz=1.0#(stop_z-start_z)/n
	pos=start_z
	for i in range(0,n+1):
		glVertex3f(pos, 0, start_z)
		glVertex3f(pos, 0, stop_z)
		pos=pos+dz

	glEnd()



def draw_photon(x,z,up):
	glLineWidth(3)
	length=0.9
	if up==True:
		glColor3f(0.0, 0.0, 1.0)
	else:
		glColor3f(0.0, 1.0, 0.0)

	glBegin(GL_LINES)
	wx=np.arange(0, length , 0.025)
	wy=np.sin(wx*3.14159*8)*0.2
	
	start_x=2.7
	stop_x=2.7-length
	for i in range(1,len(wx)):
		glVertex3f(x, start_x-wx[i-1], z+wy[i-1])
		glVertex3f(x, start_x-wx[i], z+wy[i])

	glEnd()

	if up==False:
		glBegin(GL_TRIANGLES)

		glVertex3f(x-0.1, stop_x,z)
		glVertex3f(x+0.1, stop_x ,z)
		glVertex3f(x,stop_x-0.1 ,z)

		glEnd()
	else:
		glBegin(GL_TRIANGLES)

		glVertex3f(x-0.1, start_x,z)
		glVertex3f(x+0.1, start_x ,z)
		glVertex3f(x,start_x+0.1 ,z)

		glEnd()

class fast_data():
	date=0
	m=0
	std=0
	out=[]

def fast_reset(d):
	d.date=0
	d.out=[]
	
def fast_load(d,file_name):

	if os.path.isfile(file_name)==True:
		age = os.path.getmtime(file_name)

		if d.date!=age:
			d.out=[]
			if lines_read(d.out,file_name)==True:
				if len(d.out)==0:
					return False
				#print(d.out)
				d.date=age

				d.m=0
				s=0
				for i in range(0,len(d.out)):
					d.m=d.m+d.out[i].x
				d.m=d.m/len(d.out)

				for i in range(0,len(d.out)):
					s=s+(d.out[i].x-d.m)*(d.out[i].x-d.m)
				d.std=sqrt(s/len(d.out))
				
				return True
			else:
				return False
		
	return True

def draw_rays(ray_file,d,top,width,y_mul,w):

	if fast_load(d,ray_file)==True:

		if len(d.out)>2:
			head, tail = os.path.split(ray_file)
			out=d.out
			m=d.m
			std=d.std
			
			glLineWidth(2)
			wavelength=float(tail[10:-4])
			r,g,b=wavelength_to_rgb(wavelength)

			glColor4f(r, g, b,0.5)
			glBegin(GL_QUADS)

			sub=epitaxy_get_device_start()
			s=0
			mm=0

			std_mul=0.05
			x_mul=width/(std*std_mul)
			i=0
			#step=((int)(len(out)/6000))*2
			#if step<2:
			step=2
				
			while(i<len(out)-2):
				if fabs(out[i].x-m)<std*std_mul:
					if fabs(out[i+1].x-m)<std*std_mul:
						#print(sub)
						glVertex3f(width/2+(out[i].x-m)*x_mul, top-(out[i].y+sub)*y_mul, 0)
						glVertex3f(width/2+(out[i+1].x-m)*x_mul, top-(out[i+1].y+sub)*y_mul, 0)

						glVertex3f(width/2+(out[i+1].x-m)*x_mul, top-(out[i+1].y+sub)*y_mul, w)
						glVertex3f(width/2+(out[i].x-m)*x_mul, top-(out[i].y+sub)*y_mul, w)



				i=i+step

			glEnd()
	
def draw_mode(z_size,depth):

	glLineWidth(5)
	glColor3f(1.0, 1.0, 1.0)
	glBegin(GL_LINES)
	t=[]
	s=[]
	z=[]
	start=0.0
	data=dat_file()
			
	path=os.path.join(os.getcwd(),"light_dump","light_1d_photons_tot_norm.dat")
	if dat_file_read(data,path)==True:
		array_len=data.y_len
		s=data.data[0][0]
		s.reverse()
		#print(path)
		#print(data.data)
		for i in range(1,array_len):
			glVertex3f(0.0, start+(z_size*(i-1)/array_len), depth+s[i-1]*0.5)
			glVertex3f(0.0, start+(z_size*i/array_len), depth+s[i]*0.5)

	glEnd()

def box_lines(x,y,z,w,h,d):

	glLineWidth(10)
	
	glBegin(GL_LINES)

	glColor3f(1.0,1.0,1.0)

	#btm

	glVertex3f(x+0.0,y+0.0,z+0.0)
	glVertex3f(x+w,y+ 0.0,z+0.0)

	glVertex3f(x+w,y+ 0.0,z+0.0)
	glVertex3f(x+w,y+ 0.0,z+d)

	glVertex3f(x+w,y+ 0.0,z+d)
	glVertex3f(x+ 0.0, y+0.0,z+ d) 


	#
	glVertex3f(x+0.0,y+h,z+0.0)
	glVertex3f(x+w,y+ h,z+0.0)


	glVertex3f(x+w,y+ h,z+0.0)
	glVertex3f(x+w,y+ h,z+d)
	
	glVertex3f(x+w,y+ h,z+d)	
	glVertex3f(x+ 0.0, y+h,z+ d) 

	#right

	glVertex3f(x+w,y,z)
	glVertex3f(x+w,y+ h,z)

	glVertex3f(x+w,y+ h,z)
	glVertex3f(x+w,y+ h,z+d)

	glVertex3f(x+w,y+ h,z+d)	
	glVertex3f(x+w, y,z+d) 

	#left

	glVertex3f(x,y,z)
	glVertex3f(x,y+ h,z)

	glVertex3f(x,y+ h,z)
	glVertex3f(x,y+ h,z+d)
	
	glVertex3f(x,y+ h,z+d)
	glVertex3f(x, y,z+d) 


#
	glVertex3f(x,y,z+d)
	glVertex3f(x+w,y,z+d)

	glVertex3f(x+w,y,z+d)
	glVertex3f(x+w,y+h,z+d)

	glVertex3f(x+w,y+h,z+d)	
	glVertex3f(x, y+h,z+d) 


	#top
	glVertex3f(x,y+h,z)
	glVertex3f(x+w,y+ h,z)

	glVertex3f(x+w,y+ h,z)
	glVertex3f(x+w,y+ h,z+ d)
	
	glVertex3f(x+w,y+ h,z+ d)
	glVertex3f(x, y+h,z+ d) 

	glEnd()

def val_to_rgb(v):

	dx=1.0
	r=0*v/dx
	g=0*v/dx
	b=1*v/dx	
	return r,g,b

	dx=1/6.0

	if v<dx:
		r=0*v/dx
		g=0*v/dx
		b=1*v/dx
	elif v<dx*2:
		r=0*(v-dx)/dx
		g=1*(v-dx)/dx
		b=1*(v-dx)/dx
	elif v<dx*3:
		r=0*(v-2*dx)/dx
		g=1*(v-2*dx)/dx
		b=0*(v-2*dx)/dx
	elif v<dx*4:
		r=1*(v-3*dx)/dx
		g=1*(v-3*dx)/dx
		b=0*(v-3*dx)/dx
	elif v<dx*5:
		r=1*(v-4*dx)/dx
		g=0*(v-4*dx)/dx
		b=0*(v-4*dx)/dx
	else:
		r=1*(v-5*dx)/dx
		g=1*(v-5*dx)/dx
		b=1*(v-5*dx)/dx
		
	return r,g,b

		
		
def graph(xstart,ystart,z,w,h,z_range,graph_data):
	xpoints=graph_data.x_len
	ypoints=graph_data.y_len
	
	if xpoints>0 and ypoints>0:
		
		dx=w/xpoints
		dy=h/ypoints

		glBegin(GL_QUADS)


		if z_range==0.0:
			z_range=1.0

		for x in range(0,xpoints):
			for y in range(0,ypoints):
				r,g,b=val_to_rgb(graph_data.data[0][x][y]/z_range)
				glColor4f(r,g,b, 0.7)
				glVertex3f(xstart+dx*x,ystart+y*dy, z)
				glVertex3f(xstart+dx*(x+1),ystart+y*dy, z)
				glVertex3f(xstart+dx*(x+1),ystart+dy*(y+1), z)
				glVertex3f(xstart+dx*x, ystart+dy*(y+1), z) 


		glEnd()
	
def box(x,y,z,w,h,d,r,g,b,alpha):
	red=r
	green=g
	blue=b

	glBegin(GL_QUADS)

	#btm
	glColor4f(red,green,blue,alpha)

	glVertex3f(x+0.0,y+0.0,z+0.0)
	glVertex3f(x+w,y+ 0.0,z+0.0)
	glVertex3f(x+w,y+ 0.0,z+d)
	glVertex3f(x+ 0.0, y+0.0,z+ d) 

	#back
	red=red*0.95
	green=green*0.95
	blue=blue*0.95

	glColor4f(red,green,blue,alpha)

	glVertex3f(x+0.0,y+h,z+0.0)
	glVertex3f(x+w,y+ h,z+0.0)
	glVertex3f(x+w,y+ h,z+d)
	glVertex3f(x+ 0.0, y+h,z+ d) 

	#right
	red=red*0.95
	green=green*0.95
	blue=blue*0.95
	glColor4f(red,green,blue,alpha)

	glVertex3f(x+w,y,z)
	glVertex3f(x+w,y+ h,z)
	glVertex3f(x+w,y+ h,z+d)
	glVertex3f(x+w, y,z+d) 

	#left
	red=red*0.95
	green=green*0.95
	blue=blue*0.95
	glColor4f(red,green,blue,alpha)

	glVertex3f(x,y,z)
	glVertex3f(x,y+ h,z)
	glVertex3f(x,y+ h,z+d)
	glVertex3f(x, y,z+d) 

	#front
	red=r
	green=g
	blue=b

	glColor4f(red,green,blue,alpha)
	glVertex3f(x,y,z+d)
	glVertex3f(x+w,y,z+d)
	glVertex3f(x+w,y+h,z+d)
	glVertex3f(x, y+h,z+d) 

	red=red*0.8
	green=green*0.8
	blue=blue*0.8

	#top
	glColor4f(red,green,blue,alpha)
	glVertex3f(x,y+h,z)
	glVertex3f(x+w,y+ h,z)
	glVertex3f(x+w,y+ h,z+ d)
	glVertex3f(x, y+h,z+ d) 

	glEnd()


class color():

	def __init__(self,r,g,b,alpha):
		self.r=r
		self.g=g
		self.b=b
		self.alpha=alpha

if open_gl_ok==True:		
	class glWidget(QGLWidget):
		tet_rotate = 0.0
		colors=[]
		def __init__(self, parent):
			self.ray_fast=fast_data()
			self.failed=True
			self.graph_path="./snapshots/159/Jn.dat"
			self.graph_z_max=1.0
			self.graph_z_min=1.0
			self.xRot =25.0
			self.yRot =-20.0
			self.zRot =0.0
			self.x_pos=-0.5
			self.y_pos=-0.5
			self.zoom=-12.0
			self.timer=None
			self.zoom_timer=None
			self.suns=0.0
			self.selected_layer=-1
			self.graph_data=dat_file()
			QGLWidget.__init__(self, parent)
			self.lastPos=None
			self.ray_file=""
			#glClearDepth(1.0)              
			#glDepthFunc(GL_LESS)
			#glEnable(GL_DEPTH_TEST)
			#glShadeModel(GL_SMOOTH)
		
			#self.setMinimumSize(650, 500)

		def my_timer(self):
			#self.xRot =self.xRot + 2
			self.yRot =self.yRot + 2
			#self.zRot =self.zRot + 5
			
			self.update()

		def fzoom_timer(self):
			self.zoom =self.zoom+4.0
			if self.zoom>-12.0:
				self.zoom_timer.stop()
			self.update()

		def start_rotate(self):
			self.timer=QTimer()
			self.timer.timeout.connect(self.my_timer)
			self.timer.start(50)

		def keyPressEvent(self, event):

			if type(event) == QtGui.QKeyEvent:
				if event.text()=="f":
					self.showFullScreen()
				if event.text()=="r":
					if self.timer==None:
						self.start_rotate()
					else:
						self.timer.stop()
						self.timer=None
				if event.text()=="z":
					if self.timer==None:
						self.start_rotate()
						if self.zoom>-40:
							self.zoom =-400
						self.zoom_timer=QTimer()
						self.zoom_timer.timeout.connect(self.fzoom_timer)
						self.zoom_timer.start(50)
					else:
						self.zoom_timer.stop()
						self.zoom_timer=None
						
		def mouseMoveEvent(self,event):
			if 	self.timer!=None:
				self.timer.stop()
				self.timer=None

			if self.lastPos==None:
				self.lastPos=event.pos()
			dx = event.x() - self.lastPos.x();
			dy = event.y() - self.lastPos.y();

			if event.buttons()==Qt.LeftButton:
				
				self.xRot =self.xRot + 1 * dy
				self.yRot =self.yRot + 1 * dx

			if event.buttons()==Qt.RightButton:
				self.x_pos =self.x_pos + 0.1 * dx
				self.y_pos =self.y_pos - 0.1 * dy


			self.lastPos=event.pos()
			self.setFocusPolicy(Qt.StrongFocus)
			self.setFocus()
			self.update()

		def mouseReleaseEvent(self,event):
			self.lastPos=None
			
		def wheelEvent(self,event):
			p=event.angleDelta()
			self.zoom =self.zoom + p.y()/120
			self.update()

		def paintGL(self):
			if self.failed==False:
				dos_start=-1
				dos_stop=-1
				self.x_mul=1e3
				self.z_mul=1e3

				width=mesh_get_xlen()*self.x_mul
				depth=mesh_get_zlen()*self.z_mul

				l=epitaxy_get_layers()-1

				xpoints=int(mesh_get_xpoints())
				ypoints=int(mesh_get_ypoints())
				zpoints=int(mesh_get_zpoints())

				x_len=mesh_get_xlen()

				self.emission=False
				self.ray_model=False
				
				lines=[]
				if inp_load_file(lines,"led.inp")==True:
					self.ray_model=val=str2bool(inp_search_token_value(lines, "#led_on"))
					
				lines=[]

				for i in range(0,epitaxy_get_layers()):
					if epitaxy_get_pl_file(i)!="none":
						if inp_load_file(lines,epitaxy_get_pl_file(i)+".inp")==True:
							if str2bool(lines[1])==True:
								self.emission=True
						
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
				glLoadIdentity()

				glTranslatef(self.x_pos, self.y_pos, self.zoom) # Move Into The Screen
				
				glRotatef(self.xRot, 1.0, 0.0, 0.0)
				glRotatef(self.yRot, 0.0, 1.0, 0.0)
				glRotatef(self.zRot, 0.0, 0.0, 1.0)

				glColor3f( 1.0, 1.5, 0.0 )
				glPolygonMode(GL_FRONT, GL_FILL);


				#glClearColor(0.92, 0.92, 0.92, 0.5) # Clear to black.
				glClearColor(0.0, 0.0, 0.0, 0.5)
				lines=[]


				if self.suns!=0:
					if self.suns<=0.01:
						den=1.4
					elif self.suns<=0.1:
						den=0.8
					elif self.suns<=1.0:
						den=0.6
					elif self.suns<=10.0:
						den=0.3
					else:
						den=0.2
				
					x=np.arange(0, width , den)
					z=np.arange(0, depth , den)
					for i in range(0,len(x)):
						for ii in range(0,len(z)):
							draw_photon(x[i],z[ii],False)

				if self.emission==True and self.ray_model==False:
					den=0.6
					x=np.arange(0, width , den)
					y=np.arange(0, depth , den)
					for i in range(0,len(x)):
						for ii in range(0,len(y)):
								draw_photon(x[i]+0.1,y[ii]+0.1,True)


				tot=0

				for i in range(0,epitaxy_get_layers()):
					tot=tot+epitaxy_get_width(i)

				pos=0.0
				self.y_mul=0
				if tot>0:
					self.y_mul=1.5/tot
					
					for i in range(0,epitaxy_get_layers()):

						thick=epitaxy_get_width(l-i)*self.y_mul

						red=self.colors[l-i].r
						green=self.colors[l-i].g
						blue=self.colors[l-i].b
						alpha=self.colors[l-i].alpha
						if i==l-self.selected_layer:
							box_lines(0.0,pos,0,width,thick,depth)

						if epitaxy_get_electrical_layer(l-i).startswith("dos")==True:
							dy=thick/float(ypoints)
							dx=width/float(xpoints)
							dz=depth/float(zpoints)
							xshrink=0.8
							zshrink=0.8
							
							if dos_start==-1:
								dos_start=pos
							
							dos_stop=pos+thick
					
							if xpoints==1:
								xshrink=1.0

							if zpoints==1:
								zshrink=1.0

							if xpoints==1 and zpoints==1:
								box(0.0,pos,0,width,thick,depth,red,green,blue,alpha)
							else:
								for y in range(0,ypoints):
									for x in range(0,xpoints):
										for z in range(0,zpoints):
											box(dx*x,pos+y*(dy),z*dz,dx*xshrink,dy*0.8,dz*zshrink,red,green,blue,alpha)
							tab(0.0,pos,depth,width,thick,depth)
						
						elif epitaxy_get_electrical_layer(l-i).lower()=="contact" and i==l:
							if xpoints==1 and zpoints==1:
								box(0.0,pos,0,width,thick,depth,red,green,blue)
							else:
								for c in contacts_get_array():
									xstart=width*(c.start/x_len)
									xwidth=width*(c.width/x_len)
									#print("contacts",xstart,xwidth,c.width,x_len)
									if (c.start+c.width)>x_len:
										xwidth=width-xstart
									if c.active==True:
										box(xstart,pos,0,xwidth,thick,depth,0.0,1.0,0.0,alpha)
									else:
										box(xstart,pos,0,xwidth,thick,depth,red,green,blue,alpha)


						else:
							box(0.0,pos,0,width,thick,depth,red,green,blue,alpha)
						

						if epitaxy_get_electrical_layer(l-i).startswith("dos")==True:
							text=epitaxy_get_name(l-i)+" (active)"
						else:
							text=epitaxy_get_name(l-i)

						glColor3f(1.0,1.0,1.0)
						font = QFont("Arial")
						font.setPointSize(18)
						if self.zoom>-20:
							self.renderText (width+0.1,pos+thick/2,depth, text,font)

						pos=pos+thick+0.05


				
						glRotatef(self.tet_rotate, tet_x_rate, tet_y_rate, tet_z_rate)

				draw_mode(pos-0.05,depth)
				draw_rays(self.ray_file,self.ray_fast,pos-0.05,width,self.y_mul,depth*1.05)
				#print(self.graph_path)

				full_data_range=self.graph_z_max-self.graph_z_min
				graph(0.0,dos_start,depth+0.5,width,dos_stop-dos_start,full_data_range,self.graph_data)
				draw_grid()
				if self.zoom<-60:
					draw_stars()
					
		def recalculate(self):
			fast_reset(self.ray_fast)
			self.colors=[]
			lines=[]

			if dat_file_read(self.graph_data,self.graph_path)==True:
				#print(self.graph_path)
				self.graph_z_max,self.graph_z_min=dat_file_max_min(self.graph_data)
				#print(self.graph_z_max,self.graph_z_min)
			val=inp_get_token_value("light.inp", "#Psun")
			self.suns=float(val)
			l=epitaxy_get_layers()-1
			for i in range(0,epitaxy_get_layers()):

				path=os.path.join(get_materials_path(),epitaxy_get_mat_file(l-i),"mat.inp")


				if inp_load_file(lines,path)==True:
					red=float(inp_search_token_value(lines, "#Red"))
					green=float(inp_search_token_value(lines, "#Green"))
					blue=float(inp_search_token_value(lines, "#Blue"))
					alpha=float(inp_search_token_value(lines, "#mat_alpha"))
				else:
					red=0.0
					green=0.0
					blue=0.0
					alpha=1.0
				self.colors.append(color(red,green,blue,alpha))
			self.colors.reverse()
			self.update()
			
		def resizeEvent(self,event):
			if self.failed==False:
				#glClearDepth(1.0)              
				#glDepthFunc(GL_LESS)
				#glEnable(GL_DEPTH_TEST)
				#glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				#glEnable(GL_BLEND);
				#glShadeModel(GL_SMOOTH)
				glViewport(0, 0, self.width(), self.height()+100)
				glMatrixMode(GL_PROJECTION)
				glLoadIdentity()                    
				gluPerspective(45.0,float(self.width()) / float(self.height()+100),0.1, 1000.0) 
				glMatrixMode(GL_MODELVIEW)


		def initializeGL(self):
			self.recalculate()
			try:
				glClearDepth(1.0)              
				glDepthFunc(GL_LESS)
				glEnable(GL_DEPTH_TEST)
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_BLEND);
				#glEnable(GL_PROGRAM_POINT_SIZE_EXT);
				glShadeModel(GL_SMOOTH)
				glViewport(0, 0, self.width(), self.height()+100)
				glMatrixMode(GL_PROJECTION)
				glLoadIdentity()                    
				gluPerspective(45.0,float(self.width()) / float(self.height()+100),0.1, 1000.0) 
				glMatrixMode(GL_MODELVIEW)
				#self.resizeEvent.connect(self.resize)
				
				self.failed=False
			except:
				print("OpenGL failed to load falling back to 2D rendering.",sys.exc_info()[0])

else:
	class glWidget(QWidget):

		def __init__(self, parent):
			QWidget.__init__(self)
			self.failed=True
