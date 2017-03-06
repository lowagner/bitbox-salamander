NAME=miniz

ROOMS:= start town beach

GAME_C_FILES = main.c \
	lib/blitter/blitter.c \
	lib/blitter/blitter_tmap.c \
	lib/blitter/blitter_sprites.c \
	$(ROOMS:%=room_%.c) \
	window.c 

DEFINES = VGA_MODE=320 VGA_BPP=8

TMX = python2 $(BITBOX)/lib/blitter/scripts/tmx2.py -mo data
TSX = python2 $(BITBOX)/lib/blitter/scripts/tsx.py  -mo data

include $(BITBOX)/kernel/bitbox.mk 

.DELETE_ON_ERROR: build/data.mk
build/data.mk: $(ROOMS:%=rooms/%.tmx) window.tmx
	@mkdir -p build
	$(BITBOX)/lib/blitter/scripts/tmx-deps.py $^ > $@

-include build/data.mk

# should be automatic
main.c: data.h 

TOCOMPRESS := start.tmap start.tset town.tset town.tmap bed.spr boss.spr beach.tset beach.tmap
RAWFILES := $(filter-out $(TOCOMPRESS:%=data/%),$(TMX_DATAFILES))

data.h: $(TOCOMPRESS:%=data/%) $(RAWFILES)
	python $(BITBOX)/lib/resources/embed.py --prefix=res_ $(TOCOMPRESS:%=data/%:lz4) $(RAWFILES:%=%:raw) > data.h

clean::
	rm -f $(ROOMS:%=room_%.h) data/* data.h sprite_*.h map_*.h
