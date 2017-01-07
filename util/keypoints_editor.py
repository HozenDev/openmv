#!/usr/bin/env python2

import time
import struct
import pygame
import sys
from math import sin, cos, radians

SCALE = 3
RED   = (255,0,0)
GREEN = (0,255,0)
GRAY  = (127, 127, 127)
KEYPOINTS_SIZE = 24

def load_keypoints(path):
    kpts = []
    with open(path, "rb") as f:
        size = struct.unpack("<I", f.read(4))[0]
        for i in range(0, size):
            x, y, score, octave, angle = struct.unpack("<HHHHH", f.read(10))
            kpts.append([x, y, score, octave, angle, False, f.read(32)])
    return kpts

if __name__ == '__main__':
    if len(sys.argv)!= 3:
        print "usage: kpts_editor.py [image] [descriptor]"
        sys.exit(1)

    selection = False
    save_selected = False
    rect = pygame.Rect(0, 0, 0, 0)
    clock = pygame.time.Clock() 

    kpts = load_keypoints(sys.argv[2])
    img = pygame.image.load(sys.argv[1])
    img = pygame.transform.scale(img, (img.get_width()*SCALE, img.get_height()*SCALE))
    screen = pygame.display.set_mode((img.get_width(), img.get_height()))
    pygame.mouse.set_visible(True)
    
    while True:
        msElapsed = clock.tick(60)
        event = pygame.event.poll()
        if event.type == pygame.QUIT:
            break
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_s:
                save_selected = True
        elif event.type == pygame.MOUSEBUTTONDOWN:
            selection = True
            rect = pygame.Rect(0, 0, 0, 0)
            rect.x, rect.y = pygame.mouse.get_pos()
        elif event.type == pygame.MOUSEMOTION:
            if (selection):
                rect.w, rect.h = pygame.mouse.get_pos()
                rect.w -= rect.x
                rect.h -= rect.y
        elif event.type == pygame.MOUSEBUTTONUP:
            if (selection):
                selection = False
                rect.w, rect.h = pygame.mouse.get_pos()
                rect.w -= rect.x
                rect.h -= rect.y
    
        screen.blit(img,(0,0))

        if selection == True:
            pygame.draw.rect(screen, RED, rect, 1)
    
        for kp in kpts:
            x, y, score, octave, angle, selected, desc = kp
            x1 = x*SCALE
            y1 = y*SCALE
            angle = radians(angle)

            size = KEYPOINTS_SIZE/octave
            x2 = x1 + sin(angle) * size
            y2 = y1 + cos(angle) * size

            kp[5] = selected = rect.collidepoint((x1, y1)) 
            color = GREEN if selected else GRAY
            pygame.draw.circle(screen, color, (x1, y1), size, 1)
            pygame.draw.line(screen, color, (x1, y1), (x2, y2))
    
        if save_selected == True:
            kpts_sel = []  
            for kp in kpts:
                if (kp[5]): kpts_sel.append(kp) 
    
            with open("desc_out.orb", "wb") as f:
                f.write(struct.pack("<I", len(kpts_sel)))
                for kp in kpts_sel:
                    x, y, score, octave, angle, selected, desc = kp
                    f.write(struct.pack("<HHHHH", x, y, score, octave, angle))
                    f.write(desc)
            break
    
        pygame.display.update()
    
    pygame.quit()
