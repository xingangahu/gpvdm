//
//  General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
//  base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// 
//  Copyright (C) 2012-2016 Roderick C. I. MacKenzie r.c.i.mackenzie at googlemail.com
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




#include <string.h>
#include <sys/stat.h>
#include "util.h"
#include "const.h"
#include "dump_ctrl.h"
#include "light.h"
#include "buffer.h"
#include <cal_path.h>
#include <lang.h>

void light_setup_dump_dir(struct simulation *sim,struct light *in)
{
FILE *out;
char out_dir[1024];
int i;

	sprintf(out_dir,"%s/light_dump/",get_output_path(sim));
	struct stat st = {0};

	if (stat(out_dir, &st) == -1)
	{
		mkdir(out_dir, 0700);
	}

	out=fopena(out_dir,"wavelengths.dat","w");

	for (i=0;i<in->lpoints;i++)
	{
		fprintf(out,"%.0Lf\n",in->l[i]*1e9);
	}
	fclose(out);
	
strcpy(in->dump_dir,out_dir);
}

void light_dump(struct simulation *sim,struct light *in)
{
FILE *out;
int i;
int ii;
struct buffer buf;
char line[1024];
char temp[1024];
if (get_dump_status(sim,dump_optics_verbose)==TRUE)
{
	buffer_init(&buf);

	out=fopena(in->dump_dir,"light_2d_Ep.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			fprintf(out,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,gpow(gpow(in->Ep[i][ii],2.0)+gpow(in->Epz[i][ii],2.0),0.5));

		}

	fprintf(out,"\n");
	}
	fclose(out);

	out=fopena(in->dump_dir,"light_2d_En.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			fprintf(out,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,gpow(gpow(in->En[i][ii],2.0)+gpow(in->Enz[i][ii],2.0),0.5));
		}

	fprintf(out,"\n");
	}
	fclose(out);

	out=fopena(in->dump_dir,"light_2d_E_mod.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			fprintf(out,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,gpow(gpow(in->Ep[i][ii]+in->En[i][ii],2.0)+gpow(in->Enz[i][ii]+in->Epz[i][ii],2.0),1.0));
		}

	fprintf(out,"\n");
	}
	fclose(out);


	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Photon density");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=in->lpoints;
	buf.y=in->points;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);
	
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,in->photons[i][ii]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);

	buffer_dump_path(sim,in->dump_dir,"light_2d_photons.dat",&buf);
	buffer_free(&buf);


	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Absorbed Photon density");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=in->lpoints;
	buf.y=in->points;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);
	
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,in->photons_asb[i][ii]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);
	
	buffer_dump_path(sim,in->dump_dir,"light_2d_photons_asb.dat",&buf);
	buffer_free(&buf);


	out=fopena(in->dump_dir,"light_2d_n.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			fprintf(out,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,in->n[i][ii]);
		}

	fprintf(out,"\n");
	}
	fclose(out);

	out=fopena(in->dump_dir,"light_lambda_sun.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		fprintf(out,"%Le %Le\n",in->l[i],in->sun[i]);
	}
	fclose(out);

	out=fopena(in->dump_dir,"light_lambda_sun_norm.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		fprintf(out,"%Le %Le\n",in->l[i],in->sun_norm[i]);
	}
	fclose(out);

	out=fopena(in->dump_dir,"light_lambda_sun_photons.dat","w");
	for (i=0;i<in->lpoints;i++)
	{
		fprintf(out,"%Le %Le\n",in->l[i],in->sun_photons[i]);
	}
	fclose(out);

	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Optical absorption coefficient");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=in->lpoints;
	buf.y=in->points;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);
	
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,in->alpha[i][ii]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);
	
	buffer_dump_path(sim,in->dump_dir,"light_lambda_alpha.dat",&buf);
	buffer_free(&buf);

	buffer_malloc(&buf);
	buf.y_mul=1e9;
	buf.x_mul=1e9;
	strcpy(buf.title,"Optical absorption coefficient");
	strcpy(buf.type,"heat");
	strcpy(buf.x_label,"Position");
	strcpy(buf.y_label,"Wavelength");
	strcpy(buf.data_label,"Density");
	strcpy(buf.x_units,"nm");
	strcpy(buf.y_units,"nm");
	strcpy(buf.data_units,"m^{-3}");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=in->lpoints;
	buf.y=in->points;
	buf.z=1;
	buffer_add_info(sim,&buf);

	sprintf(temp,"#data\n");
	buffer_add_string(&buf,temp);
	
	for (i=0;i<in->lpoints;i++)
	{
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le %Le\n",in->l[i],in->x[ii]-in->device_start,in->n[i][ii]);
			buffer_add_string(&buf,line);
		}

	buffer_add_string(&buf,"\n");
	}

	sprintf(temp,"#end\n");
	buffer_add_string(&buf,temp);
	
	buffer_dump_path(sim,in->dump_dir,"light_lambda_n.dat",&buf);
	buffer_free(&buf);

	out=fopena(in->dump_dir,"light_sun_wavelength_E.dat","w");
	for (ii=0;ii<in->lpoints;ii++)
	{
		fprintf(out,"%Le %Le\n",in->l[ii],in->sun_E[ii]);
	}
	fclose(out);
}



}

void light_dump_sim_info(struct simulation *sim,struct light *in)
{
long double ret=0.0;
ret=light_calculate_photons_absorbed_in_active_layer(in);
FILE *out;
out=fopena(get_output_path(sim),"sim_info.dat","w");
fprintf(out,"#light_photons_in_active_layer\n%Le\n",ret);
fprintf(out,"#end");
fclose(out);

}

void light_dump_summary(struct simulation *sim,struct light *in)
{
struct buffer buf;
if (get_dump_status(sim,dump_optics_summary)==TRUE)
{


	buffer_malloc(&buf);
	buf.y_mul=1.0;
	buf.x_mul=1e9;
	strcpy(buf.title,"Wavelength - Reflected light");
	strcpy(buf.type,"xy");
	strcpy(buf.x_label,"Wavelength");
	strcpy(buf.data_label,"Reflected light");
	strcpy(buf.x_units,"nm");
	strcpy(buf.data_units,"a.u.");
	buf.logscale_x=0;
	buf.logscale_y=0;
	buf.x=1;
	buf.y=in->lpoints;
	buf.z=1;
	buffer_add_info(sim,&buf);
	buffer_add_xy_data(sim,&buf,in->l, in->reflect, in->lpoints);
	buffer_dump_path(sim,in->dump_dir,"reflect.dat",&buf);
	buffer_free(&buf);
}



}
void light_dump_1d(struct simulation *sim,struct light *in, int i,char *ext)
{
char line[1024];
char out_name[200];
char temp_name[400];
char temp[1024];

struct buffer data_photons;
struct buffer data_photons_norm;
struct buffer data_light_1d_Ep;
struct buffer data_light_1d_En;
struct buffer data_pointing;
struct buffer data_E_tot;
struct buffer data_1d_photons_tot;
struct buffer data_1d_photons_tot_abs;
struct buffer data_r;
struct buffer data_t;
struct buffer data_n;
struct buffer data_alpha;
struct buffer buf;

buffer_init(&data_photons);
buffer_init(&data_photons_norm);
buffer_init(&data_light_1d_Ep);
buffer_init(&data_light_1d_En);
buffer_init(&data_pointing);
buffer_init(&data_E_tot);
buffer_init(&data_1d_photons_tot);
buffer_init(&data_1d_photons_tot_abs);
buffer_init(&data_r);
buffer_init(&data_t);
buffer_init(&data_n);
buffer_init(&data_alpha);
buffer_init(&buf);
if (get_dump_status(sim,dump_optics)==TRUE)
{



	int ii;
	char name[400];
	double max=0.0;

	if (i==0)
	{
		max=inter_array_get_max(in->photons_tot,in->points);

		buffer_malloc(&data_1d_photons_tot);
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Normalized photon density");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,"Position");
		strcpy(buf.data_label,"Photon desntiy");
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"a.u.");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;

		buffer_add_info(sim,&buf);

		buffer_add_string(&buf,"#data\n");

		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->photons_tot[ii]/max);
			buffer_add_string(&buf,line);
		}

		buffer_add_string(&buf,"#end");

		sprintf(out_name,"light_1d_photons_tot_norm%s.dat",ext);
		buffer_dump_path(sim,in->dump_dir,out_name,&buf);
		buffer_free(&buf);
		printf("Write.....%s %s\n",in->dump_dir,out_name);

		sprintf(temp_name,"light_1d_photons_tot%s.dat",ext);



		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->photons_tot[ii]);
			buffer_add_string(&data_1d_photons_tot,line);

		}

		buffer_dump_path(sim,in->dump_dir,temp_name,&data_1d_photons_tot);



		max=inter_array_get_max(in->Gn,in->points);
		buffer_malloc(&data_1d_photons_tot_abs);
		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Normalized photons absorbed");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,"Position");
		strcpy(buf.data_label,"Absorbed photons");
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"a.u.");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->Gn[ii]/max);
			buffer_add_string(&buf,line);
		}

		sprintf(out_name,"light_1d_photons_tot_abs_norm%s.dat",ext);
		buffer_dump_path(sim,in->dump_dir,out_name,&buf);
		buffer_free(&buf);


		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Electron generation rate"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Generation rate"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"m^{-3}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->Gn[ii]);
			buffer_add_string(&buf,line);
		}

		buffer_dump_path(sim,in->dump_dir,"light_1d_Gn.dat",&buf);
		buffer_free(&buf);



		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,_("Hole generation rate"));
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Generation rate"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"m^{-3}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->Gp[ii]);
			buffer_add_string(&buf,line);
		}

		buffer_dump_path(sim,in->dump_dir,"light_1d_Gp.dat",&buf);
		buffer_free(&buf);
		
	}

	buffer_malloc(&data_photons);
	buffer_malloc(&data_photons_norm);
	buffer_malloc(&data_light_1d_Ep);
	buffer_malloc(&data_light_1d_En);
	buffer_malloc(&data_pointing);
	buffer_malloc(&data_E_tot);
	buffer_malloc(&data_r);
	buffer_malloc(&data_t);
	buffer_malloc(&data_n);
	buffer_malloc(&data_alpha);

	char name_photons[200];
	char name_photons_norm[200];
	char name_light_1d_Ep[200];
	char name_light_1d_En[200];
	char name_pointing[200];
	char name_E_tot[200];
	char name_r[200];
	char name_t[200];
	char name_n[200];
	char name_alpha[200];

	sprintf(name_photons,"light_1d_%.0Lf_photons%s.dat",in->l[i]*1e9,ext);
	sprintf(name_photons_norm,"light_1d_%.0Lf_photons%s_norm.dat",in->l[i]*1e9,ext);
	sprintf(name_light_1d_Ep,"light_1d_%.0Lf_Ep%s.dat",in->l[i]*1e9,ext);
	sprintf(name_light_1d_En,"light_1d_%.0Lf_En%s.dat",in->l[i]*1e9,ext);
	sprintf(name_pointing,"light_1d_%.0Lf_pointing%s.dat",in->l[i]*1e9,ext);
	sprintf(name_E_tot,"light_1d_%.0Lf_E_tot%s.dat",in->l[i]*1e9,ext);
	sprintf(name_r,"light_1d_%.0Lf_r%s.dat",in->l[i]*1e9,ext);
	sprintf(name_t,"light_1d_%.0Lf_t%s.dat",in->l[i]*1e9,ext);
	sprintf(name_n,"light_1d_%.0Lf_n%s.dat",in->l[i]*1e9,ext);
	sprintf(name_alpha,"light_1d_%.0Lf_alpha%s.dat",in->l[i]*1e9,ext);

	max=inter_array_get_max(in->photons[i],in->points);
	for (ii=0;ii<in->points;ii++)
	{
		sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->photons[i][ii]);
		buffer_add_string(&data_photons,line);

		sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->photons[i][ii]/max);
		buffer_add_string(&data_photons_norm,line);

		sprintf(line,"%Le %Le %Le %Le\n",in->x[ii]-in->device_start,gpow(gpow(in->Ep[i][ii],2.0)+gpow(in->Epz[i][ii],2.0),0.5),in->Ep[i][ii],in->Epz[i][ii]);
		buffer_add_string(&data_light_1d_Ep,line);

		sprintf(line,"%Le %Le %Le %Le\n",in->x[ii]-in->device_start,gpow(gpow(in->En[i][ii],2.0)+gpow(in->Enz[i][ii],2.0),0.5),in->En[i][ii],in->Enz[i][ii]);
		buffer_add_string(&data_light_1d_En,line);

		sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->pointing_vector[i][ii]);
		buffer_add_string(&data_pointing,line);

		sprintf(line,"%Le %Le %Le\n",in->x[ii]-in->device_start,in->E_tot_r[i][ii],in->E_tot_i[i][ii]);
		buffer_add_string(&data_E_tot,line);

		sprintf(line,"%Le %Le %Le %Le\n",in->x[ii]-in->device_start,gcabs(in->r[i][ii]),gcreal(in->r[i][ii]),gcimag(in->r[i][ii]));
		buffer_add_string(&data_r,line);

		sprintf(line,"%Le %Le %Le %Le\n",in->x[ii]-in->device_start,gcabs(in->t[i][ii]),gcreal(in->t[i][ii]),gcimag(in->t[i][ii]));
		buffer_add_string(&data_t,line);

		sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->n[i][ii]);
		buffer_add_string(&data_n,line);

		sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->alpha[i][ii]);
		buffer_add_string(&data_alpha,line);
	}

	buffer_dump_path(sim,in->dump_dir,name_photons,&data_photons);
	buffer_dump_path(sim,in->dump_dir,name_photons_norm,&data_photons_norm);
	buffer_dump_path(sim,in->dump_dir,name_light_1d_Ep,&data_light_1d_Ep);
	buffer_dump_path(sim,in->dump_dir,name_light_1d_En,&data_light_1d_En);
	buffer_dump_path(sim,in->dump_dir,name_pointing,&data_pointing);
	buffer_dump_path(sim,in->dump_dir,name_E_tot,&data_E_tot);
	buffer_dump_path(sim,in->dump_dir,name_r,&data_r);
	buffer_dump_path(sim,in->dump_dir,name_t,&data_t);
	buffer_dump_path(sim,in->dump_dir,name_n,&data_n);
	buffer_dump_path(sim,in->dump_dir,name_alpha,&data_alpha);



	buffer_free(&data_photons);
	buffer_free(&data_photons_norm);
	buffer_free(&data_light_1d_Ep);
	buffer_free(&data_light_1d_En);
	buffer_free(&data_pointing);
	buffer_free(&data_E_tot);
	buffer_free(&data_r);
	buffer_free(&data_t);
	buffer_free(&data_n);
	buffer_free(&data_alpha);

	if (i==0)
	{
		buffer_free(&data_1d_photons_tot);
		buffer_free(&data_1d_photons_tot_abs);
	}


	if (get_dump_status(sim,dump_optics_verbose)==TRUE)
	{

		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Layer number vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Layer number"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"au");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %d\n",in->x[ii]-in->device_start,in->layer[ii]);
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_layer%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);


		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"|Electric field| vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("|Electric field|"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"V/m");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,gpow(gpow(in->Ep[i][ii]+in->En[i][ii],2.0)+gpow(in->Enz[i][ii]+in->Epz[i][ii],2.0),0.5));
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_E%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);


		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Transmittance vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Transmittance"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"au");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,gcabs(in->t[i][ii]));
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_t%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);



		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Reflectance vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Reflectance"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"au");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,gcabs(in->r[i][ii]));
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_r%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);





		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Photons absorbed vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Photons absorbed"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"m^{-3} m^{-1}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->photons_asb[i][ii]);
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_photons_abs%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);






		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Real refractive index (n) vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Real refractive index"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"au");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->n[i][ii]);
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_n%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);




		buffer_malloc(&buf);
		buf.y_mul=1.0;
		buf.x_mul=1e9;
		strcpy(buf.title,"Absorption vs position");
		strcpy(buf.type,"xy");
		strcpy(buf.x_label,_("Position"));
		strcpy(buf.data_label,_("Absorption"));
		strcpy(buf.x_units,"nm");
		strcpy(buf.data_units,"m^{-1}");
		buf.logscale_x=0;
		buf.logscale_y=0;
		buf.x=1;
		buf.y=in->points;
		buf.z=1;
		buffer_add_info(sim,&buf);

		sprintf(temp,"#data\n");
		buffer_add_string(&buf,temp);
	
		for (ii=0;ii<in->points;ii++)
		{
			sprintf(line,"%Le %Le\n",in->x[ii]-in->device_start,in->alpha[i][ii]);
			buffer_add_string(&buf,line);
		}


		sprintf(temp,"#end\n");
		buffer_add_string(&buf,temp);

		sprintf(name,"light_1d_%.0Lf_alpha%s.dat",in->l[i]*1e9,ext);
		buffer_dump_path(sim,in->dump_dir,name,&buf);
		buffer_free(&buf);


		
	}



}

}

