NAME=miniz

ROOMS= start town 

# special "room" : window
ROOMS+= window

GAME_C_FILES = main.c lz4.c lib/blitter/blitter.c lib/blitter/blitter_tmap.c lib/blitter/blitter_sprites.c resources.c
# lz4.s : only used bitbox / non emulator

DEFINES = VGA_MODE=320 VGA_BPP=8

LZ4= lz4 -9 --no-frame-crc --content-size
TMX = $(BITBOX)/lib/blitter/scripts/tmx2.py

include $(BITBOX)/kernel/bitbox.mk 

main.c: $(ROOMS:%=room_%.h) data.h 

all: data.h rooms.inc

rooms.inc: 
	echo "// X macros for rooms\n#define ROOM_LIST=\\" > $@	
	for i in $(ROOMS); do echo "	 X($$i)\\" >> $@ ; done
	echo >> $@


COMPRESSEDFILES = start.tmap start.tset town.tset town.tmap start_lit.spr


data.h: $(ROOMS:%=data/%.tmap) data/window.tmap mkdata.py
	python mkdata.py $$(python -c "import os;print ' '.join('+data/'+c if c in ','.join('$(COMPRESSEDFILES)'.split()) else 'data/'+c for c in os.listdir('data'))") > data.h
	rm data/*.lz4

room_%.h data/%.tmap: rooms/%.tmx $(TMX)
	@mkdir -p data
	$(info -------------------------------------------------)
	$(info $<)
	$(info -------------------------------------------------)
	python2 $(TMX) -o data -mas $< > room_$*.h

window.h data/window.tmap: window.tmx
	python2 $(TMX) -o data -ma $< > $*.h

clean::
	rm -f $(ROOMS:%=room_%.h) data/* data.bin data.h 