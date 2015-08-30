#!/bin/env python
#coding: utf-8

from PIL import Image
import os
import os.path

g_sample = "./sample"
g_dest = "./analyse"
g_size = (13, 20)

def corner(img, pos):
    h = img.size[1]
    w = img.size[0]
    for po in pos:
        p = img.getpixel(po)
        for hp in range(h):
            for wp in range(w):
                px = img.getpixel((wp, hp))
                if abs(px[0] - p[0]) <= 50 and \
                    abs(px[1] - p[1]) <= 50 and \
                    abs(px[2] - p[2]) <= 50:
                    img.putpixel((wp, hp), (255, 255, 255))

def save(dest_path, dest_file, img, size):    
    val = dest_file.split(".")[0]
    if len(val) < 4:
        print "file not right"
        return
        
    for i in range(4):
        file = ""
        j = 0
        while True:
            j = j+1
            file = dest_path + "/" + val[i] + "-" + str(j) + ".jpg"
            if not os.path.exists(file):
                break
        save_size = (i*size[0], 0, (i+1)*size[0], size[1])
        sub_img = img.crop(save_size)
        sub_img.save(file)
    
def analyse(src_path, src_file, dest_path, size):
    src_img = Image.open(src_path + "/" + src_file)
    proc_img = src_img.copy()
    
    h = proc_img.size[1]
    w = proc_img.size[0]
    
    #4个角 3*3 都变成白色
    left_top = [(0,0),(1,0),(2,0),(0,1),(1,1),(2,1),(0,2),(1,2),(2,2)]
    left_bottom = [(0,h-3),(1,h-3),(2,h-3),(0,h-2),(1,h-2),(2,h-2),(0,h-1),(1,h-1),(2,h-1)]
    right_top = [(w-3,0),(w-2,0),(w-1,0),(w-3,1),(w-2,1),(w-1,1),(w-3,2),(w-2,2),(w-1,2)]
    right_bottom = [(w-3,h-3),(w-2,h-3),(w-1,h-3),(w-3,h-2),(w-2,h-2),(w-1,h-2),(w-3,h-1),(w-2,h-1),(w-1,h-1)]
    
    corner(proc_img, left_bottom)
    corner(proc_img, left_top)
    corner(proc_img, right_bottom)
    corner(proc_img, right_top)
    
    px = (255, 255, 255)

    for hp in range(h):
        for wp in range(w):
            p = proc_img.getpixel((wp, hp))
            if abs(px[0] - p[0]) > 50 and \
                abs(px[1] - p[1]) > 50 and \
                abs(px[2] - p[2]) > 50:
                proc_img.putpixel((wp, hp), (0, 0, 0))
            else:
                proc_img.putpixel((wp, hp), (255, 255, 255))

    save(dest_path, src_file, proc_img, g_size)
    
def main():
    files = os.listdir(g_sample)
    for file in files:
        if os.path.isfile(g_sample + "/" + file) and file.endswith(".jpg"):
            print "analyse: " + g_sample + "/" + file
            analyse(g_sample, file, g_dest, (13, 20))
    
if __name__ == "__main__":
    main()