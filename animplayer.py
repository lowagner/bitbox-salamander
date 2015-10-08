#!/usr/bin/python
import os, sys,glob
import pygame
from pygame.locals import *
import argparse

BGCOLOR = (10,10,10)

class Anim :
    framerates = [3,5,6,8,10,12,15,20,30,60]
    zooms = [1,2,3,4,8,16]
    def __init__(self,file,w,h) :
        self.w=w
        self.h=h
        self.file=file


        self.screen = pygame.display.set_mode((32,32))
        pygame.display.set_caption('bitbox anim player')
        pygame.mouse.set_visible(0)
        self.clock = pygame.time.Clock()
        self.font = pygame.font.SysFont('default', 12)
        pygame.time.set_timer(USEREVENT+1, 2000) # check file each X ms

        self.zoom = 1
        self.framerate = 4
        self.step = 1

        self.info = True # display info on screen
        self.stat = None



    def reload(self, colorkey=-1,rezoom=False):
        try:
            t_image = pygame.image.load(self.file)
        except (pygame.error, IOError), message:
            t_image = self.font.render('Cannot load %s'%self.file,True,(255,0,0))


        x=t_image.get_width()
        y=t_image.get_height()


        m=min(x,y)

        pygame.display.set_icon(t_image.subsurface((0,0,m,m)))

        if rezoom :
            self.zoom=0
            while (self.zooms[self.zoom]*x / self.w)<128 :
                self.zoom += 1

        zoom = self.zooms[self.zoom]
        nx = x*zoom
        ny = y*zoom
        self.image = pygame.transform.scale(t_image,(nx,ny)).convert()

        if colorkey is not None:
            if colorkey is -1:
                colorkey = self.image.get_at((0,0))
            self.image.set_colorkey(colorkey, RLEACCEL)

        self.image_w = self.image.get_width()
        self.image_h = self.image.get_height()


        self.screen = pygame.display.set_mode((nx/self.w,ny/self.h))
        self.stat = os.stat(self.file)

        print 'reloaded',self.file, self.zooms[self.zoom]


    def run(self) :
        self.reload(rezoom=True)
        frame = 0
        anim_id=0
        self.nframes = self.w
        while True :
            self.clock.tick(self.framerates[self.framerate])

            frame += self.step
            if frame > self.nframes-1 :
                frame = 0

            # user input
            pygame.event.pump()
            for event in pygame.event.get():
                if event.type == QUIT:
                    return
                if event.type == KEYDOWN  :
                    if event.key == K_ESCAPE:
                        return

                    elif event.key == K_RIGHT and self.framerate<len(self.framerates)-1:
                        self.framerate +=1

                    elif event.key == K_LEFT and self.framerate>0:
                        self.framerate -=1

                    # override nb frames
                    elif event.key == K_PAGEDOWN : 
                        self.nframes -= 1 
                        if self.nframes<1 : self.nframes=1
                    elif event.key == K_PAGEUP : 
                        self.nframes += 1 
                        if self.nframes>self.w : self.nframes=self.w

                    # skip frames
                    elif event.key == K_KP1 :
                        self.step= 1
                    elif event.key == K_KP2 :
                        self.step= 2
                    elif event.key == K_KP3 :
                        self.step= 3
                    elif event.key == K_KP4 :
                        self.step= 4


                    elif event.key == K_UP :
                        anim_id +=1
                        anim_id %= self.h

                    elif event.key == K_DOWN :
                        anim_id -=1
                        anim_id %= self.h

                    elif event.key== K_KP_PLUS and self.zoom<len(self.zooms)-1:
                        self.zoom += 1
                        self.reload()

                    elif event.key== K_KP_MINUS and self.zoom>0:
                        self.zoom -= 1
                        self.reload()

                    elif event.key in(K_KP_MULTIPLY,K_SPACE) :
                        self.info = not self.info

                elif event.type == USEREVENT+1 :
                    # check changed
                    if os.stat(self.file) != self.stat :
                        self.reload()

                # PGUP/DN : next file


            # update screen
            self.screen.fill(BGCOLOR)
            self.screen.blit(self.image,(0,0),(
                frame*self.image_w/self.w,
                anim_id*self.image_h/self.h,
                self.image_w/self.w,
                self.image_h/self.h)
            )
            if self.info :
                self.screen.blit(self.font.render("%dx %dfps %dfrm +%d"%(
                    self.zooms[self.zoom],
                    self.framerates[self.framerate],
                    self.nframes,
                    self.step
                    ),True,(255,255,0)),(0,0))
                self.screen.blit(self.font.render('keys: spc  +-UDLR1234',True,(255,0,255)),(0,self.image_w-8))
            pygame.display.flip()

from spritesheet import EX

parser = argparse.ArgumentParser(description='Process a spritesheet files to anz file')
parser.add_argument('file', nargs='?',default="rooms/"+EX[0]+'.png',help='input spritesheet')
parser.add_argument('w', nargs='?',type=int,default=EX[1],help='number sprites horizontally')
parser.add_argument('h', nargs='?', type=int,default=EX[2],help='number sprites vertically')
parser.add_argument('wbits', nargs='?', type=int,default=9,help='window size for heatshrink compression')
args = parser.parse_args()

print "playing %s with %dx%d images"%(args.file,args.w,args.h)



pygame.init()
a=Anim(args.file,args.w,args.h)
a.run()

