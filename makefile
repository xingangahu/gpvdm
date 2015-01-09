#<clean=none></clean>
plugins:=$(shell ./get_elec_plugins.sh)

ifndef DEST_DIR
	DEST_DIR=./install/
endif

ifdef OPT_FLAGS
	opt_slow=$(OPT_FLAGS)
	opt_normal=$(OPT_FLAGS)
	opt_fast=$(OPT_FLAGS)
	warn=
	debug_opt=
else
	opt_slow=-O0
	opt_normal=-O2 -D nolock
	opt_fast=-O5
	warn=-Werror -Wall
	debug_opt=-pg  -ggdb -g
endif


ifeq ($(wildcard ~/.opvdm_hpc_flag),)

else 
	debug_opt=
endif

	link=-lz 
	flags=${debug_opt} -D dos_bin
	inc=-I/usr/include/suitesparse/ -I ./plugins/i/

.PHONY: clean

main: main.c solver.o sim_find_n0.o sim_run.o newton_update.o  pos.o inital.o advmath.o config.o plot.o memory.o dos.o gendosfdgaus.o exp.o time.o fast.o anal.o dump.o ntricks.o dos_an.o startstop.o render1d.o complex_solver.o dump_slice.o thermal.o light_interface.o dump_ctrl.o
	./build.sh
	./build_fit_plugins.sh
	./buildplugins.sh "$(opt_normal) $(debug_opt)"
	gcc main.c solver.o sim_find_n0.o sim_run.o newton_update.o pos.o inital.o advmath.o config.o plot.o memory.o dos.o gendosfdgaus.o exp.o time.o $(plugins) fast.o anal.o dump.o ntricks.o dos_an.o startstop.o render1d.o complex_solver.o dump_slice.o thermal.o light_interface.o dump_ctrl.o -o go.o  -lumfpack $(flags) $(link) $(inc) -lamd -Wall -export-dynamic -lm  -lgsl -lgslcblas -lblas -lcrypto -lcurl   -ldl -rdynamic


install:

	mkdir $(DEST_DIR)/usr
	mkdir $(DEST_DIR)/usr/bin
	mkdir $(DEST_DIR)/usr/share
	mkdir $(DEST_DIR)/usr/share/opvdm
	mkdir $(DEST_DIR)/usr/share/applications
	mkdir $(DEST_DIR)/usr/share/opvdm/gui
	cp *.inp $(DEST_DIR)/usr/share/opvdm/
	cp README $(DEST_DIR)/usr/share/opvdm/
	cp ./gui/image.jpg $(DEST_DIR)/usr/share/opvdm/gui/
	cp ./gui/icon.png $(DEST_DIR)/usr/share/opvdm/gui/
	cp ./gui/*.py $(DEST_DIR)/usr/share/opvdm/gui/
	cp ./gui/opvdm.desktop $(DEST_DIR)/usr/share/applications/
	cp plot $(DEST_DIR)/usr/share/opvdm/ -rf

	cp opvdm_clone $(DEST_DIR)/usr/bin/
	cp opvdm_import $(DEST_DIR)/usr/bin/
	cp opvdm_dump_tab $(DEST_DIR)/usr/bin/
	cp opvdm_load $(DEST_DIR)/usr/bin/
	cp opvdm_save $(DEST_DIR)/usr/bin/

	cp opvdm $(DEST_DIR)/usr/bin/opvdm
	cp go.o $(DEST_DIR)/usr/bin/opvdm_core

	mkdir $(DEST_DIR)/usr/share/man
	mkdir $(DEST_DIR)/usr/share/man/man1
	cp ./man_pages/*.gz $(DEST_DIR)/usr/share/man/man1/


	chmod 755 $(DEST_DIR)/usr/bin/opvdm
	chmod 755 $(DEST_DIR)/usr/bin/opvdm_core
	chmod 755 $(DEST_DIR)/usr/bin/opvdm_clone
	chmod 755 $(DEST_DIR)/usr/bin/opvdm_import
	chmod 755 $(DEST_DIR)/usr/bin/opvdm_dump_tab
	chmod 755 $(DEST_DIR)/usr/bin/opvdm_load
	chmod 755 $(DEST_DIR)/usr/bin/opvdm_save
	chmod 0644 $(DEST_DIR)/usr/share/opvdm/*.inp
	chmod 0644 $(DEST_DIR)/usr/share/opvdm/gui/image.jpg
	chmod 0644 $(DEST_DIR)/usr/share/opvdm/gui/icon.png
	chmod 0644 $(DEST_DIR)/usr/share/opvdm/gui/*.py
	chmod 0644 $(DEST_DIR)/usr/share/applications/opvdm.desktop
	chmod 0755 $(DEST_DIR)/usr/share/opvdm/plot
	chmod 0644 $(DEST_DIR)/usr/share/opvdm/plot/*

dump_ctrl.o: dump_ctrl.c
	gcc -c dump_ctrl.c -o dump_ctrl.o  $(inc) $(flags) $(opt_normal) $(warn)

stark_ntricks.o: stark_ntricks.c
	gcc -c stark_ntricks.c -o stark_ntricks.o  $(inc) $(flags)  $(opt_normal) $(warn)



celiv_ntricks.o: celiv_ntricks.c
	gcc -c celiv_ntricks.c -o celiv_ntricks.o  $(inc) $(flags) $(opt_normal) $(warn)

tpc_ntricks.o: tpc_ntricks.c
	gcc -c tpc_ntricks.c -o tpc_ntricks.o  $(inc) $(flags) $(opt_normal) $(warn)

sim_find_n0.o: sim_find_n0.c
	gcc -c sim_find_n0.c -o sim_find_n0.o  $(inc) $(flags) $(opt_fast) $(warn)

sim_run.o: sim_run.c
	gcc -c sim_run.c -o sim_run.o  $(inc) $(flags) $(opt_fast) $(warn)

newton_update.o: newton_update.c
	gcc -c newton_update.c -o newton_update.o  $(inc) $(flags) $(opt_fast) -Wall

dump_slice.o: dump_slice.c
	gcc -c dump_slice.c -o dump_slice.o  $(inc) $(flags) $(opt_normal) $(warn)

thermal.o: thermal.c
	gcc -c thermal.c -o thermal.o  $(inc) $(flags) $(opt_normal) $(warn)

light_interface.o: light_interface.c
	gcc -c light_interface.c -o light_interface.o  $(inc) $(flags) $(opt_normal) $(warn)

render1d.o: render1d.c
	gcc -c render1d.c -o render1d.o  $(inc) $(flags) $(opt_normal) $(warn)

startstop.o: startstop.c
	gcc -c startstop.c -o startstop.o  $(inc) $(flags) $(opt_normal) $(warn)

dos_an.o: dos_an.c
	gcc -c dos_an.c -o dos_an.o  $(inc) $(flags) $(opt_normal) $(warn)


stark.o: stark.c
	gcc -c stark.c -o stark.o  $(inc) $(flags) $(opt_normal) $(warn)

ntricks.o: ntricks.c
	gcc -c ntricks.c -o ntricks.o  $(inc) $(flags) $(opt_normal) $(warn)

celiv.o: celiv.c
	gcc -c celiv.c -o celiv.o  $(inc) $(flags) $(opt_normal) $(warn)


dump.o: dump.c
	gcc -c dump.c -o dump.o  $(inc) $(flags) $(opt_normal) $(warn)

anal.o: anal.c
	gcc -c anal.c -o anal.o  $(inc) $(flags) $(opt_normal) $(warn)

tpc.o: tpc.c
	gcc -c tpc.c -o tpc.o  $(inc) $(flags) $(opt_normal) $(warn)


fast.o: fast.c
	gcc -c fast.c -o fast.o  $(inc) $(opt_fast) $(warn)

time.o: time.c
	gcc -c time.c -o time.o  $(inc) $(flags) $(opt_normal) $(warn)

exp.o: exp.c
	gcc -c exp.c -o exp.o  $(inc) $(flags) $(opt_normal) $(warn)



gendosfdgaus.o: gendosfdgaus.c
	gcc -c gendosfdgaus.c -o gendosfdgaus.o  $(inc) $(flags) $(opt_normal) $(warn)

memory.o: memory.c
	gcc -c memory.c -o memory.o $(inc) $(flags) $(opt_normal) $(warn)

dos.o: dos.c
	gcc -c dos.c -o dos.o $(inc)  $(flags)  $(opt_fast) $(warn)
excite.o: excite.c
	gcc -c excite.c -o excite.o  $(inc) $(flags) $(opt_normal) $(warn)

solver.o: solver.c
	gcc -c solver.c -o solver.o  $(inc) $(flags) $(opt_normal) $(warn)

pos.o: pos.c
	gcc -c pos.c -o pos.o  $(inc) $(flags) $(opt_normal) $(warn)

inital.o: inital.c
	gcc -c inital.c -o inital.o  $(inc) $(flags) $(opt_normal) $(warn)

config.o: config.c
	gcc -c config.c -o config.o  $(inc) $(flags) $(opt_normal) $(warn)

plot.o: plot.c
	gcc -c plot.c -o plot.o  $(flags) $(inc) $(opt_normal) $(warn)


advmath.o: advmath.c
	gcc -c advmath.c -o advmath.o  $(flags) $(inc) $(opt_slow) $(warn)


complex_solver.o: complex_solver.c
	gcc -c complex_solver.c -o complex_solver.o  $(flags) $(inc) $(opt_normal) $(warn)

clean:
	./clean_all.sh
	rm *.o *.dat *.jpg *.avi *.eps
	rm sim -rf

frame:
	gnuplot frame.plot >frame.eps
	convert -rotate 90 -resize 800x600 -background white -layers flatten -density 600x600 frame.eps frame.jpg
	gnome-open frame.jpg
