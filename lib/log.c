//
//  General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
//  base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// 
//  Copyright (C) 2012-2017 Roderick C. I. MacKenzie r.c.i.mackenzie AT googlemail.com
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




#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include <colors.h>
#include <time.h>
#include <stdlib.h>
#include <util.h>
#include <cal_path.h>
#include <string.h>
#include <const.h>
#include <lang.h>

void log_time_stamp(struct simulation *sim)
{
	time_t t=0;
	t=time(NULL);

	struct tm tm = *localtime(&t);

	printf_log(sim,"time: %d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void log_clear(struct simulation *sim)
{
	FILE* out;
	char temp[500];
	join_path(2,temp,get_output_path(sim),"log.dat");
	out=fopen(temp,"w");
	fprintf(out,"gpvdm log file:\n");
	fclose(out);
	
	join_path(2,temp,sim->root_simulation_path,"log_file_access.dat");
	out=fopen(temp,"w");
	fprintf(out,"gpvdm file access log file:\n");
	fclose(out);
		
}

void log_tell_use_where_file_access_log_is(struct simulation *sim)
{
	if (get_dump_status(sim,dump_file_access_log)==TRUE)
	{
		char temp[500];
		join_path(2,temp,sim->root_simulation_path,"log_file_access.dat");
		printf_log(sim,_("File access log written to %s\n"),temp);
	}
		
}
void log_write_file_access(struct simulation *sim,char * file,char mode)
{
	if (get_dump_status(sim,dump_file_access_log)==TRUE)
	{
		FILE* out;
		char temp[500];
		join_path(2,temp,sim->root_simulation_path,"log_file_access.dat");
		out=fopen(temp,"a");
		if (mode=='w')
		{
			fprintf(out,"write:%s\n",file);
		}else
		{
			fprintf(out,"read:%s\n",file);
		}
		
		fclose(out);	
	}
}

void set_logging_level(struct simulation *sim,int value)
{
	sim->log_level=value;
}

void text_to_html(struct simulation *sim,char *out, char *in)
{
	if (sim->html==TRUE)
	{

		int i=0;
		int out_pos=0;
		int len=0;
		len=strlen(in);
		for (i=0;i<len;i++)
		{
			if (in[i]=='\n')
			{
				out[out_pos]=0;
				strcat(out,"<br>");
				out_pos=strlen(out);
			}else
			{
				out[out_pos]=in[i];
				out_pos++;
			}
			
		}
		
		out[out_pos]=0;
	}else
	{
		out[0]=0;
		strcpy(out,in);
	}
	
}

void printf_log(struct simulation *sim, const char *format, ...)
{
	FILE* out;
	char data[1000];
	char data_html[1000];
	char temp[500];
	va_list args;
	va_start(args, format);
	vsprintf(data,format, args);
	
	if ((sim->log_level==log_level_screen)||(sim->log_level==log_level_screen_and_disk))
	{
		text_to_html(sim,data_html, data);
		printf("%s",data_html);
		fflush(stdout);
	}

	if ((sim->log_level==log_level_disk)||(sim->log_level==log_level_screen_and_disk))
	{
		join_path(2,temp,get_output_path(sim),"log.dat");
		out=fopen(temp,"a");
		if (out==NULL)
		{
			printf("error: opening file %s\n",temp);
		}
		fprintf(out,"%s",data);
		fclose(out);
	}

	va_end(args);
}

void wavelength_to_rgb(int *r,int *g,int *b,double wavelength)
{
	double gamma=0.80;
	double intensity_max=1.0;
	double factor=0.0;

	double red=0.0;
	double green=0.0;
	double blue=0.0;

	if ((wavelength >= 380e-9) && (wavelength<440e-9))
	{
		red = -(wavelength - 440e-9) / (440e-9 - 380e-9);
		green = 0.0;
		blue = 1.0;
	}else
	if ((wavelength >= 440e-9) &&  (wavelength<490e-9))
	{
		red = 0.0;
		green = (wavelength - 440e-9) / (490e-9 - 440e-9);
		blue = 1.0;
	}else
	if ((wavelength >= 490e-9) &&  (wavelength<510e-9))
	{
		red = 0.0;
		green = 1.0;
		blue = -(wavelength - 510e-9) / (510e-9 - 490e-9);
	}else
	if ((wavelength >= 510e-9) &&  (wavelength<580e-9))
	{
		red = (wavelength - 510e-9) / (580e-9 - 510e-9);
		green = 1.0;
		blue = 0.0;
	}else
	if ((wavelength >= 580e-9) &&  (wavelength<645e-9))
	{
		red = 1.0;
		green = -(wavelength - 645e-9) / (645e-9 - 580e-9);
		blue = 0.0;
	}else
	if ((wavelength >= 645e-9) &&  (wavelength<781e-9))
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}else
	{
		red = 0.0;
		green = 0.0;
		blue = 0.0;
	}
	
	if (wavelength<420e-9)
	{
		factor = 0.3 + 0.7*(wavelength - 380e-9) / (420e-9 - 380e-9);
	}else
	if ((wavelength >= 420e-9) && (wavelength<701e-9))
	{
		factor = 1.0;
	}else
	if ((wavelength >= 701e-9) &&  (wavelength<781e-9))
	{
		factor = 0.3 + 0.7*(780e-9 - wavelength) / (780e-9 - 700e-9);
	}else
	{
		factor = 0.0;
	}

	if (red != 0.0)
	{
		red = (intensity_max * pow(red * factor, gamma));
	}

	if (green != 0)
	{
		green = (intensity_max * pow(green * factor, gamma));
	}

	if (blue != 0)
	{
		blue = (intensity_max * pow(blue * factor, gamma));
	}


	if ((red==0.0)&&(green==0.0)&&(blue==0.0))
	{
		red=0.392634;
		green=0.000000;
		blue=0.397315;
	}

	*r=255.0*red;
	*g=255.0*green;
	*b=255.0*blue;
}

void waveprint(struct simulation *sim,char *in,double wavelength)
{
	int r;
	int g;
	int b;

	if ((sim->log_level==log_level_screen)||(sim->log_level==log_level_screen_and_disk))
	{
		if (sim->html==TRUE)
		{
			wavelength_to_rgb(&r,&g,&b,wavelength*1e-9);
			printf("<font color=\"#%.2x%.2x%.2x\">",r,g,b);
		}else
		{
			if (wavelength<400.0)
			{
				textcolor(sim,fg_purple);
			}else
			if (wavelength<500.0)
			{
				textcolor(sim,fg_blue);
			}else
			if (wavelength<575.0)
			{
				textcolor(sim,fg_green);
			}else
			if (wavelength<600.0)
			{
				textcolor(sim,fg_yellow);
			}else
			{
				textcolor(sim,fg_red);

			}
		}
	}

	printf_log(sim,"%s",in);

	if ((sim->log_level==log_level_screen)||(sim->log_level==log_level_screen_and_disk))
	{
		if (sim->html==TRUE)
		{
			printf("</font>");
		}else
		{
			textcolor(sim,fg_reset);
		}
			
	}
}

void textcolor(struct simulation *sim, int color)
{
if (sim->html==TRUE)
{
	if (color==fg_purple)
	{
		printf("<font color=\"purple\">");
	}else
	if (color==fg_blue)
	{
		printf("<font color=\"blue\">");
	}else
	if (color==fg_green)
	{
		printf("<font color=\"green\">");
	}else
	if (color==fg_yellow)
	{
		printf("<font color=\"yellow\">");
	}else
	if (color==fg_red)
	{
		printf("<font color=\"red\">");
	}else
	if (color==fg_wight)
	{
		printf("<font color=\"#ffffff\">");
	}else
	if (color==fg_reset)
	{
		printf("</font>");
	}

}else
{
	char command[13];
	sprintf(command, "\e[%dm", color);
	printf("%s", command);
}
}

int log_search_error(char *path)
{
	int ret=-1;
    FILE * fp;
    char * line = NULL;
    ssize_t read;
	size_t len = 0;
    fp = fopen(path, "r");

    if (fp == NULL)
	{
       return ret;
	}

    while ((read = getline(&line, &len, fp)) != -1)
	{
		if (strcmp_begin(line,"error:")==0)
		{
			ret=0;
			break;
		}

    }

    fclose(fp);

    if (line)
	{
        free(line);
	}
    return ret;
}
