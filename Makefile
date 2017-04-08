NAME=sal

ROOMS:= start town beach bateau town_nuit

GAME_C_FILES = main.c \
	lib/blitter/blitter.c \
	lib/blitter/blitter_tmap.c \
	lib/blitter/blitter_sprites.c \
	$(ROOMS:%=room_%.c) \
	resources.c\
	window.c 

DEFINES = VGA_MODE=320 VGA_BPP=8 NO_AUDIO

# will be used in data.mk
TMX = python2 $(BITBOX)/lib/blitter/scripts/tmx2.py -mo data
TSX = python2 $(BITBOX)/lib/blitter/scripts/tsx.py  -mo data

include $(BITBOX)/kernel/bitbox.mk 

# generates all dependencies of data
.DELETE_ON_ERROR: build/data.mk
build/data.mk: $(ROOMS:%=rooms/%.tmx) window.tmx
	@mkdir -p build
	@mkdir -p data
	$(BITBOX)/lib/blitter/scripts/tmx-deps.py $^ > $@
-include build/data.mk

main.c: data.h # should be automatic

TOCOMPRESS :=  start.tset town.tset bateau.tset beach.tset  town_nuit.tset \
	bed.spr boss.spr bonh.spr \
	start.tmap beach.tmap town.tmap town_nuit.tmap bateau.tmap
RAWFILES := $(filter-out $(TOCOMPRESS:%=data/%),$(TMX_DATAFILES))

data/%.lz4: data/%
	lz4 -f -9 --content-size --no-frame-crc --no-sparse $^ $@
	

data.h : $(TOCOMPRESS:%=data/%.lz4) $(RAWFILES)
	# uurgh. this just appends standard names but replace cannot replace twice .. 
	python $(BITBOX)/lib/resources/embed.py $(foreach fn,$(TOCOMPRESS),data/$(fn).lz4:$(subst .,_,$(fn))) $(RAWFILES) > data.h

clean::
	rm -rf $(ROOMS:%=room_%.h) data data.h sprite_*.h map_*.h

