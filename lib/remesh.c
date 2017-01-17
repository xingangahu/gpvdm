//
//  General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
//  base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// 
//  Copyright (C) 2012 Roderick C. I. MacKenzie <r.c.i.mackenzie@googlemail.com>
//
//	https://www.gpvdm.com
//	Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
//
//
// This program is free software; you can redistribute it and/or modify it
// under the terms and conditions of the GNU General Public License,
// version 2, as published by the Free Software Foundation.
//
// This program is distributed in the hope it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.



#include "sim.h"
#include "dump.h"
#include "remesh.h"
#include "ntricks.h"


struct remesh my_mesh;

void remesh_shrink(struct device *in)
{
}

void remesh_expand_array_band(gdouble **y,int band,struct device *in)
{
}

void remesh_expand_array(gdouble *y,struct device *in)
{
}

void remesh_reset(struct device *in,gdouble voltage)
{
}
