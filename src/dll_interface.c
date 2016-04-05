//
//  General-purpose Photovoltaic Device Model gpvdm.com- a drift diffusion
//  base/Shockley-Read-Hall model for 1st, 2nd and 3rd generation solarcells.
// 
//  Copyright (C) 2012 Roderick C. I. MacKenzie
//
//      roderick.mackenzie@nottingham.ac.uk
//      www.roderickmackenzie.eu
//      Room B86 Coates, University Park, Nottingham, NG7 2RD, UK
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

#include "device.h"
#include "light.h"
#include "dll_interface.h"
#include "log.h"
#include "complex_solver.h"
#include "sim.h"
#include <remesh.h>
#include <ntricks.h>
#include <dump.h>
#include <dos.h>

static struct dll_interface pointers;

void dll_interface_fixup(struct device *in)
{
	pointers.get_dump_status = &get_dump_status;
	pointers.light_dump_1d = &light_dump_1d;
	pointers.light_solve_optical_problem = &light_solve_optical_problem;
	pointers.light_free_memory = &light_free_memory;
	pointers.light_transfer_gen_rate_to_device =
	    &light_transfer_gen_rate_to_device;
	pointers.complex_solver = &complex_solver;
	pointers.get_n_den = &get_n_den;
	pointers.get_dn_den = &get_dn_den;
	pointers.get_n_w = &get_n_w;
	pointers.get_p_den = &get_p_den;
	pointers.get_dp_den = &get_dp_den;
	pointers.get_p_w = &get_p_w;
	pointers.dump_matrix = &dump_matrix;
	pointers.ewe = &ewe;
	pointers.get_dump_status = &get_dump_status;
	pointers.solver = &solver;
	pointers.dump_1d_slice = &dump_1d_slice;
	pointers.update_arrays = &update_arrays;
	pointers.remesh_reset = &remesh_reset;
	pointers.newton_set_min_ittr = &newton_set_min_ittr;
	pointers.solver_realloc = &solver_realloc;
	pointers.solve_all = &solve_all;
	pointers.in = in;

	pointers.ntricks_externv_set_load = &ntricks_externv_set_load;
	pointers.sim_externalv = &sim_externalv;
	pointers.ntricks_externv_newton = &ntricks_externv_newton;
	pointers.device_timestep = &device_timestep;
	pointers.ntricks_externv_newton = &ntricks_externv_newton;
	pointers.device_timestep = &device_timestep;
	pointers.sim_i = &sim_i;
}

struct dll_interface *dll_get_interface()
{
	return &pointers;
}
