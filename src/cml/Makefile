
SUBDIRS = \
adc_bridge	d_fdiv		d_ram		gain		pwl          \
aswitch		d_inverter	d_source	hyst		s_xfer       \
capacitor	d_jkff		d_srff		ilimit		sine         \
climit		d_nand		d_srlatch	inductor	slew         \
cmeter		d_nor		d_state		int		square       \
core		d_open_c	d_tff		lcouple		summer       \
d_and		d_open_e	d_tristate	limit		triangle     \
d_buffer	d_or		d_xnor		lmeter		zener        \
d_dff		d_osc		d_xor		mult                         \
d_dlatch	d_pulldown	dac_bridge	oneshot                      \
d_dt		d_pullup	divide		potentiometer


all: $(SUBDIRS)
	
$(SUBDIRS): dummy.make
	cd $@;\
		make
	cp -r $@ ../../lib/cml/$@

	
dummy.make: 
	true
