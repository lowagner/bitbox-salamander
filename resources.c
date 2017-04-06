#include "bitbox.h"

#define TINYMALLOC_IMPLEMENTATION
#include "lib/resources/tinymalloc.h"
#undef TINYMALLOC_IMPLEMENTATION

#define TINYLZ4_IMPLEMENTATION
#define LZ4_STREAM
#include "lib/resources/tinylz4.h" 
#undef TINYLZ4_IMPLEMENTATION

#define DATA_IMPLEMENTATION
#include "data.h"
#undef DATA_IMPLEMENTATION

#define MAX_COMPRESSED 16 // max nb of decompressed objects at once
#define MEM_SIZE (80*1024)

// keep track of decompressed files in memory
static struct {
	const void *compressed;
	void *decompressed; 
	// refcnt ?
} comptab[MAX_COMPRESSED];

static int nb_compressed;

char RAM[MEM_SIZE];

void resource_init(void)
{
	t_addchunk(&RAM, sizeof(RAM));
	nb_compressed=0;
}


void *load_resource(const void *data)
{
	if (*(uint32_t*)data == MAGIC_LZ4) {
		// do not decompress twice the same resource : look it in compressed table
		for (int i=0;i<nb_compressed;i++) {
			if (comptab[i].compressed == data)
				return comptab[i].decompressed;
		}
		// didnt find it
		if (nb_compressed==MAX_COMPRESSED) {
			message("too many decompressed objects. see resouces.c\n");
			die (7,2);
		}
		comptab[nb_compressed].compressed = data;
		comptab[nb_compressed].decompressed = lz4_stream_decompress(data);

		return comptab[nb_compressed++].decompressed;
	} else {
		return (void*)data; 
	}
}

// free it : if compressed, remove from table (or even refcnt--)

void resource_unload_all()
{
	for (int i=0;i<nb_compressed;i++) {
		t_free(comptab[i].decompressed);
	}
	nb_compressed=0;
}