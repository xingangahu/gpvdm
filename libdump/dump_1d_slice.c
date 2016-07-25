//
//  General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
//  base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// 
//  Copyright (C) 2012 Roderick C. I. MacKenzie <r.c.i.mackenzie@googlemail.com>
//
//	www.roderickmackenzie.eu
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


#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sim.h>
#include <dump.h>
#include <buffer.h>
#include <util.h>
#include <lang.h>
#include <i.h>
#include <exp.h>
#include <dos.h>



void dump_1d_slice(struct simulation *sim,struct device *in,char *out_dir)
{
int x;
int y;
int z;

int band;
char name[100];
char temp[200];
gdouble Vexternal=get_equiv_V(sim,in);
struct buffer buf;
buffer_init(&buf);

struct stat st = {0};

if (stat(out_dir, &st) == -1)
{
	mkdir(out_dir, 0700);
}

	cal_J_drift_diffusion(in);

	buffer_malloc(&buf);
	sprintf(name,"%s","Jn_drift.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Drift current density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron current density (drift)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Jn_drift);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Jn_diffusion.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Diffusion current density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron current density (diffusion)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Jn_diffusion);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Jp_drift.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Drift current density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Hole current density (drift)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Jp_drift);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Jp_diffusion.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Diffusion current density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Hole current density (diffusion)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Jp_diffusion);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Ec.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("LUMO-position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Energy"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Ec);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Ev.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("HOMO-position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Energy"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Ev);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Tl.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Lattice temperature - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Temperature (K)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("K"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Tl);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Te.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Electron temperature - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Temperature (K)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("K"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Te);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Th.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Hole temperature - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Temperature (K)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("K"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Te);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Nad.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Doping - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Doping density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Nad);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Eg.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Band gap-position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Energy"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Eg);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Fn.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Electron quasi Fermi-level position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Energy"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Fn);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Fp.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Hole quasi Fermi-level position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Energy"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Fp);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","phi.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Potential"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Potential"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("V"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->phi);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dphi.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Potential"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Potential"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("V"));
	strcpy(buf.section_one,_("Change in 1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	x=0;
	y=0;
	z=0;
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],(in->phi[z][x][y]-in->phi_save[z][x][y]));
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Jn.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Current density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron current density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Jn);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Jp.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Current density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Hole current density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Jp);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Jn_plus_Jp.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Total current density (Jn+Jp) - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Total current density (Jn+Jp)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	sprintf(temp,"%Le %Le\n",in->ymesh[0]-in->ymesh[1]/2,get_J_left(in));

	x=0;
	y=0;
	z=0;

	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],(in->Jp[z][x][y]+in->Jn[z][x][y]));
		buffer_add_string(&buf,temp);
	}

	sprintf(temp,"%Le %Le\n",in->ymesh[in->ymeshpoints-1]-in->ymesh[1]/2,get_J_right(in));
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Jp_drift_plus_diffusion.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Total current density (Jn+Jp) - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Total current density (Jn+Jp)"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("A m^{-2}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Transport"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	x=0;
	y=0;
	z=0;

	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],(in->Jp_drift[z][x][y]+in->Jp_diffusion[z][x][y]));
		buffer_add_string(&buf,temp);
	}

	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Fi.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Equlibrium Fermi-level - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Energy"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Fi);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","epsilon_r.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Relative permittivity - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Relative permittivity"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("au"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->epsilonr);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","mu_n.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Electron mobility - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron mobility"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{2} V^{-1} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->mun);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","mu_p.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Hole mobility - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Hole mobility"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{2} V^{-1} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->mup);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","mu_n_ft.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Average electron mobility free mu0*nf/nall"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Mobility"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{2} V^{-1} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);

	x=0;
	y=0;
	z=0;

	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->mun[z][x][y]*in->n[z][x][y]/(in->nt_all[z][x][y]+in->n[z][x][y]));
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","mu_p_ft.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Average electron mobility free mu0*nf/nall"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Mobility"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{2} V^{-1} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->mup[z][x][y]*in->p[z][x][y]/(in->pt_all[z][x][y]+in->p[z][x][y]));
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","nf.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Free electron carrier density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->n);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","pf.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Free hole carrier density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Material parameters"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->p);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","nt.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Trapped electron carrier density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->nt_all);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","pt.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Trapped hole carrier density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->pt_all);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","p.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Total hole density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	x=0;
	y=0;
	z=0;
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->p[z][x][y]+in->pt_all[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","n.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Total hole density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->n[z][x][y]+in->nt_all[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dn.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Change in electron population - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Chaerge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],(in->n[z][x][y]+in->nt_all[z][x][y])-(in->nf_save[z][x][y]+in->nt_save[z][x][y]));
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","charge.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Total charge - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->p[z][x][y]-in->n[z][x][y]+in->pt_all[z][x][y]-in->nt_all[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dcharge.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Change in total charge - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->p[z][x][y]-in->n[z][x][y]+in->pt_all[z][x][y]-in->nt_all[z][x][y]-(in->pt_save[z][x][y]-in->nf_save[z][x][y]+in->pf_save[z][x][y]-in->nt_save[z][x][y]));
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dp.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Change in hole population - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Carrier density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le\n",in->ymesh[y],in->p[z][x][y]+in->pt_all[z][x][y]-in->pf_save[z][x][y]-in->pt_save[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dnt.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Excess electron density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le",in->ymesh[y], in->nt_all[z][x][y]-in->nt_save[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","dpt.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Excess electron density - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Hole density"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Charge density"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %Le",in->ymesh[y], in->pt_all[z][x][y]-in->pt_save[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Gn.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Free electron generation rate - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Generation rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Gn);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Gp.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Free hole generation rate - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Generation rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Gp);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Rn.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Electron recombination rate - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Recombination rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Rn);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","Rp.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Hole recombination rate - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Recombination rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->Rp);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","fsrhn.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Trap fermi level - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Fermi level"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le ",in->ymesh[y]);
		buffer_add_string(&buf,temp);
		for (band=0;band<in->srh_bands;band++)
		{
			sprintf(temp,"%Le %Le ",in->Fnt[z][x][y][band],-in->phi[z][x][y]-in->Xi[z][x][y]+dos_srh_get_fermi_n(in,in->n[z][x][y], in->p[z][x][y],band,in->imat[z][x][y],in->Te[z][x][y]));
			buffer_add_string(&buf,temp);
		}
		buffer_add_string(&buf,"\n");

	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","fsrhh.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Trap fermi level - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Electron Fermi level"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("eV"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le ",in->ymesh[y]);
		buffer_add_string(&buf,temp);
		for (band=0;band<in->srh_bands;band++)
		{
			sprintf(temp,"%Le %Le ",in->Fpt[z][x][y][band],-in->phi[z][x][y]-in->Xi[z][x][y]-in->Eg[z][x][y]-dos_srh_get_fermi_p(in,in->n[z][x][y], in->p[z][x][y],band,in->imat[z][x][y],in->Th[z][x][y]));
			buffer_add_string(&buf,temp);
		}
		buffer_add_string(&buf,"\n");

	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","imat.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Material number - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Number"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("au"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Model"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	for (y=0;y<in->ymeshpoints;y++)
	{
		sprintf(temp,"%Le %d\n",in->ymesh[y], in->imat[z][x][y]);
		buffer_add_string(&buf,temp);
	}
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","Efield.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Material number - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Number"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("au"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Band structure"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	gdouble deriv=0.0;
	for (y=in->ymeshpoints-1;y>1;y--)
	{

		deriv= -(in->phi[z][x][y]-in->phi[z][x][y-1])/(in->ymesh[y]-in->ymesh[y-1]);
		sprintf(temp,"%Le %Le\n",in->ymesh[y], deriv);
		buffer_add_string(&buf,temp);
	}

	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","pf_to_nt.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Free hole to trapped electron - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->ntrap_to_p);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	sprintf(name,"%s","nf_to_pt.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Free electron to trapped hole - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->ptrap_to_n);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","prelax.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Hole relaxation rate - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in,  in->prelax);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	sprintf(name,"%s","nrelax.dat");
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,_("Electron relaxation rate - position"));
	buffer_set_graph_type(&buf,in);
	strcpy(buf.x_label,_("Position"));
	strcpy(buf.y_label,_("Rate"));
	strcpy(buf.x_units,_("nm"));
	strcpy(buf.y_units,_("m^{-3} s^{-1}"));
	strcpy(buf.section_one,_("1D position space output"));
	strcpy(buf.section_two,_("Recombination"));
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.time=in->time;
	buf.Vexternal=Vexternal;
	buffer_add_info(&buf);
	buffer_add_3d_device_data(&buf,in, in->nrelax);
	buffer_dump_path(out_dir,name,&buf);
	buffer_free(&buf);


}
