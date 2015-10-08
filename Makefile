NAME=adv

ROOMS= start beach house1 town 
ROOM_BIN= $(ROOMS:%=rooms/%.room)
ROOM_LZ4= $(ROOMS:%=rooms/%.room.lz4)

GAME_C_FILES = main.c
GAME_C_OPTS = -DVGAMODE_320
GAME_BINARY_FILES = $(ROOM_BIN)

USE_ENGINE=1

include $(BITBOX)/lib/bitbox.mk

main.c: build/binaries.h

all: $(ROOM_LZ4) 
	du -hc rooms/*.room.lz4 

%.room: %.tmx
	$(info -------------------------------------------------)
	$(info $<)
	$(info -------------------------------------------------)
	python2 tmx8.py $< 

%.room.lz4: %.room
	lz4 $<

clean::
	rm -f rooms/*.room rooms/*.room.lz4