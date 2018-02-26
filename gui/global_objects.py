#    General-purpose Photovoltaic Device Model - a drift diffusion base/Shockley-Read-Hall
#    model for 1st, 2nd and 3rd generation solar cells.
#    Copyright (C) 2012 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
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

global_objects=[]
blocked=False
class objects:
	name=""
	object_pointer=None

def global_object_register(name,pointer):
	global global_objects
	found=False
	for i in range(0,len(global_objects)):
		if global_objects[i].name==name:
			global_objects[i].object_pointer=pointer
			found=True

	if found==False:
		a=objects()
		a.name=name
		a.object_pointer=pointer
		global_objects.append(a)
		

def global_object_get(name):
	global global_objects
	for i in range(0,len(global_objects)):
		if global_objects[i].name==name:
			return global_objects[i].object_pointer

	print("name",name,"not found")
	sys.exit()

def global_object_run(name):
	global blocked
	if blocked==True:
		return
	global global_objects
	for i in range(0,len(global_objects)):
		if global_objects[i].name==name:
			blocked=True
			global_objects[i].object_pointer()
			blocked=False
			return

	
def global_isobject(name):
	global global_objects
	for i in range(0,len(global_objects)):
		if global_objects[i].name==name:
			return True

	return False

def global_object_delete(name):
	global global_objects
	for i in range(0,len(global_objects)):
		if global_objects[i].name==name:
			del global_objects[i]
			return
