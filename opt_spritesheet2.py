from PIL import Image
from itertools import takewhile

# just : take a big chunk (vertical stripe), cut just the frame, recut the right data, 
# produce, by frame :  idx, dx,dy,height u8 
# check if exist already : if not compose (paste(x,(0,y)),x)), increase dy 
# finally crop final result with sum(dy), max(dx) !

# and save the thing

def optimize(f,nb = None,framesize=None) : 
	if nb==None and framesize==None : 
		raise ValueError,"provide either a size or a number of frames"
	img = Image.open(f).convert('RGBA')
	iw,ih = img.size
	if nb : 
		w,h=nb
		fw=iw/w
		fh=ih/h
	else : 
		fw,fh = framesize
		w = iw/fw
		h = ih/fh
	print f,img, fw,fh
	unique_frames = []
	nframes = 0

	stripe = Image.new('RGBA',(fw,ih*w))

	indices = [] # animation index, height,dx,dy
	y = 0 # where to paste
	maxw=0 # what to crop finally

	for ty in range(h) : 
		for tx in range(w) : 
			frame = img.crop((tx*iw/w,ty*ih/h,(tx+1)*iw/w,(ty+1)*ih/h))
			def empty(x1,y1,x2,y2) : 
				return all((x[3]<50) for x in frame.crop((x1,y1,x2,y2)).getdata())

			# get pos of this frame
			top=0 
			while empty(0,0,fw,top+1) and top<=fh : top+=1
			bottom=fh 
			while empty(0,bottom,fw,fh) and bottom>0 : bottom-=1
			left=0
			while empty(0,0,left+1,fh) and left <= fw: left+=1
			right=fw
			while empty(right-1,0,fw,fh) and right >0 : right-=1

			cropped=frame.crop((left,top,right,bottom+1))
			cw,ch = cropped.size

			if not(cw*ch) : continue # skip empty frame

			framedata =tuple(cropped.getdata())
			nframes += 1
			try : 
				index = unique_frames.index(framedata)
			except ValueError : 
				index = len(unique_frames)
				unique_frames.append(framedata)
				stripe.paste(cropped,(0,y),cropped) # only if new
				y += ch
				if cw>maxw : maxw=cw
			indices.append((index,left,top,ch))
		indices.append((0,0,0,0)) # separator

	print len(unique_frames),'unique frames /',nframes,'/',w*h
	print 'anim frames [%d]:'%len(indices),indices
	print

	return indices, stripe.crop((0,0,maxw,y))

if __name__=='__main__' : 
	for infile in 'boss_6x10','crab_8x10','bonh_8x4','spectre_8x9','goblin_4x9' : 
		w,h=infile.split('_')[1].split('x')
		indices,img = optimize('rooms/%s.png'%infile,nb=(int(w),int(h)))
		img.save('rooms/%s_crop.png'%infile ) # taille en deux : x2 nb
