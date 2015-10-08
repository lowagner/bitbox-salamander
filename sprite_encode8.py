#! /usr/bin/python
from itertools import groupby 
import sys, struct, os

from PIL import Image # using the PIL library, maybe you'll need to install it. python should come with t.

# 8 bits format :  nb skip:4, nb blit: 3, eol:1

DEBUG = False
TRANSP=None
MAXSKIP = 15
MAXBLIT = 7

def reduce(c) :
    'R8G8B8A8 to R3G2B2L1'
    r,g,b =[x*7/255 for x in c[:3]] # convert to three bits
    a=c[3]
    return r<<5 | (g&~1)<<4 | (b&~1)<<2 | (g&1) if a>128 else TRANSP

def p4_encode(data, palette) : 
    linedata=[]
    for i in range(0,len(data),4) : 
        w=0
        for j in range(4) : 
            w |= (palette.index(data[i+j]) if (len(data)>i+j) else 0)<<(j*4)
        linedata.append(w)
    return struct.pack('<%dH'%len(linedata),*linedata)

def image_encode(src,f,x) : 
    # reduce colors ?

    # Get the alpha band
    alpha = src.split()[-1]
    r,g,b= src.convert('RGB').convert('P', palette=Image.ADAPTIVE, colors=16).convert('RGB').split()
    src=Image.merge("RGBA",(r,g,b,alpha))

    data = [reduce(c) for c in src.getdata()] # keep image in RAM as bytes / None
    w,h=src.size
    palette=sorted([x for x in set(data) if x!=None])

    print '//',len(palette),'colors '

    s_blits = [] # stringified blits for all image
    
    start_file = f.tell()
    line16=[] # offsets from start as u16 index on  words 

    for y in range(h) :
        if y%16==0 : 
            ofs = sum(len(x) for x in s_blits)
            line16.append(ofs/4) # XXX use /4 but need to align 

        skipped=0
        blits=[]

        line=data[y*w:(y+1)*w] # byte/none data
        singles=[] 
        for c,g in groupby(line, lambda x:x!=TRANSP) : 
            t = tuple(g)
            if not c : 
                skipped = len(t)                
                # if skip too big, split ! 
                while (skipped>MAXSKIP) : 
                    blits.append([MAXSKIP,(),False])
                    skipped -= MAXSKIP
            else :
                # idem 
                while t :
                    blits.append([skipped,t[:MAXBLIT],False])
                    skipped=0
                    t=t[MAXBLIT:]

        # enleve derniers blits si vides
        while(blits and blits[-1][1])==() : 
            del blits[-1]

        # set EOL 
        if blits : 
            blits[-1][2]=True
        else : 
            blits.append([0,[],True])


        # now encode line : (header + blit) x n
        for skip, blit, eol in blits :          
            header=(skip<<4) | (len(blit) << 1) | (1 if eol else 0)
            s = struct.pack('B', header)
            s+= p4_encode(blit,palette=palette) 
            # pad it
            #s+= '\000'*((-len(s))%4)
            s_blits.append(s)
    data = ''.join(s_blits)
    data+= '\0'*((-len(data))%4)
    return palette, data, line16 
