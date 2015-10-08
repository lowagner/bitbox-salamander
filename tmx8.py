"""
Make a room file from a tmx and a txt file
todo : sprites , gen C file (to be compiled to RAM with main room object (named rom) at fixed position)
double layer ?
"""

from PIL import Image # using the PIL library, maybe you'll need to install it. python should come with t.
import xml.etree.ElementTree as ET
import sys, struct, os.path, argparse, StringIO, zlib, glob
from subprocess import Popen, PIPE

from opt_spritesheet2 import optimize
from sprite_encode8 import image_encode

parser = argparse.ArgumentParser(description='Process TMX files to tset/tmap/.h files')
parser.add_argument('file', nargs='?',default="rooms/beach.tmx",help='input .tmx filename')
args = parser.parse_args()

tree = ET.parse(args.file)
root = tree.getroot()
def error(msg) : 
    print msg
    sys.exit(1)

base_name = args.file.rsplit('.',1)[0]

tilesets = root.findall('tileset')

of = StringIO.StringIO() 

ts = tilesets[0]
# checks
firstgid=int(ts.get('firstgid'))

# assert len(tilesets)==1 and firstgid==1 , "Just one tileset starting at 1"

tilesize = int(ts.get("tilewidth"))

assert tilesize == int(ts.get("tileheight")) == 16, "only square tiles of 16x16 supported"

# tilemap. Keeps used tiles mapping
tile_mapping = {} # : tile_id global


mw, mh = root.get('width'), root.get('height')
# output global root ? 


layer=root.find("layer[@name='map']")
lw = int(layer.get("width"))
lh = int(layer.get("height"))

data = layer.find("data")
indices = [int(s) for s in data.text.replace("\n",'').split(',')] # read csv
unique_indices = sorted(set(indices))
print len(unique_indices), 'unique tiles'

# - Tiles
# ---------------------------------------------------------------

# XXX if RGBA -> int, else : uint. Here, assume uint
def reduceRGB(c) :
    r,g,b =[x*7/255 for x in c] # convert to three bits
    return r<<5 | (g&~1)<<4 | (b&~1)<<2 | (g&1)

# output data to binary 
of.write(struct.pack("BBBB",lw,lh,len(unique_indices),0))
of.write(struct.pack('%dB'%len(indices),*[unique_indices.index(i) for i in indices]))
print "tilemap is",of.tell()

assert of.tell()%4==0 , 'tilemap size not a multiple of 4'

# get rare tilemap ids 


# tileset properties : name is a unique property - export for all tilesets allowing item-only names 
tilenames = set() # already seen (validate unique)
for ts in tilesets : 
    # tile properties
    first_gid = int(ts.get('firstgid'))
    for tile in ts.findall('tile') : 
        tid = int(tile.get('id'))+first_gid 

        properties = {prop.get('name'):prop.get('value') for prop in tile.findall('./properties/property')}        
        if 'name' in properties : 
            tilename = properties['name']
            if tilename in tilenames : 
                print "// warning duplicate name %s of tile id %d"%(tilename,tid)
            print "#define %s_%s %d"%(base_name,tilename,tid)       
            tilenames.add(properties['name'])

    # export real tiles
    img = ts.find("image").get("source")
    src = Image.open(os.path.join(os.path.dirname(os.path.abspath(args.file)),img)).convert('RGB')
    pixdata = tuple(reduceRGB(c) for c in src.getdata()) # keep image in RAM as RGB tuples. (to palette ?)
    w,h = src.size

    for tile_y in range(h/16) : 
        for tile_x in range(w/16) : 
            tile_id = tile_x + tile_y * (w/16) + first_gid

            if tile_id not in unique_indices : continue

            for row in range(16) : 
                idx = (tile_y*16+row)*w + tile_x*16
                of.write(struct.pack('16B',*pixdata[idx:idx+16])) # non extraire des carres de 16x16 avec PIL et les ecrire 

print "tileset is",16*16*len(unique_indices)

# output objects 
# ---------------------------------------------------------------
objectgroup = root.find('objectgroup[@name="objects"]') 

unique_gid = []
sprites = []

for nb_objects,o in enumerate(objectgroup.findall('object')) :

    x = int(float(o.get('x')))
    y = int(float(o.get('y')))

    gid= int(o.get('gid'))

    try : 
        t_id=unique_gid.index(gid)
    except ValueError : 
        t_id=len(unique_gid)
        unique_gid.append(gid)

    # sprite of this new id
    the_ts=tilesets[0].find("image").get("source")
    for ts in tilesets :
        if int(ts.get('firstgid'))>gid : break
        the_ts=ts

    # unique sprites
    try : 
        sprite_id = sprites.index(the_ts)
    except ValueError : 
        sprite_id=len(sprites)
        sprites.append(the_ts)

    of.write(struct.pack('BBBB',x//16,y//16,t_id,sprite_id))


# output animation file
# --------------------
ofs = of.tell()

for sprite in sprites : 
    tw = int(sprite.get('tilewidth'))
    th = int(sprite.get('tileheight'))
    img_name = sprite.find("image").get("source")
    img_name = os.path.join(os.path.dirname(os.path.abspath(args.file)),img_name)
    print "sprite", img_name, tw,th

    idx,img = optimize(img_name,framesize=(tw,th))

    img.save(img_name.replace('.png','_crop.png')) 

    palette, data, line16 = image_encode(img,of,len(idx)) # send number of frames rather than frame height 

    # Save Sprite animation
    # header
    of.write(struct.pack("<2BH",img.size[0],len(idx),len(data)))
    for t in idx : 
        of.write(struct.pack("4B",*t))
    palette += [0]*(-len(palette)%16) # force it to 16 
    of.write(struct.pack("16B",*palette))
    of.write(data)

    assert(of.tell()%4==0),'bad sprite %d'%(len(data)%4)
    # line16 record ?
    # of.write(struct.pack("%dH"%len(line16),*line16))


of.write('\0')  # end of file - width=0

sprites_size=of.tell()-ofs
print sprites_size,'sprite (compressed)'

# -- fix number of sprites
of.seek(3)
of.write(chr(nb_objects))


open(base_name+'.room','wb').write(of.getvalue())
print "room is",lw,"by",lh,len(of.getvalue()),"bytes"


if False : 
    # write compressed file
    # ---------------------------------------------------------------
    WBITS =10
    heatshrinked,err = Popen(['heatshrink','-w%d'%WBITS],stdout=PIPE,stdin=PIPE,stderr=PIPE).communicate(input=of.getvalue())
    open(base_name+'.room.hs','wb').write(heatshrinked)

    # blabla
    # ---------------------------------------------------------------

    compr_size = len(heatshrinked)
    print "compressed room is",compr_size,'(main room ratio=%.1f, %.1f in 256k )'%(float(of.tell())/(compr_size), 256.*1024/(compr_size))
