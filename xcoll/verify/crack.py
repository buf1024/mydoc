#!/bin/env python
#coding: utf-8

from PIL import Image
import os
import os.path
import shutil

g_orig = "./test/original"
g_crack = "./test/crack"
g_good = "./good"

g_size = (13, 20)
g_map = {}

g_equal_factor = 12
g_stop_factor = 9900

def load():
    files = os.listdir(g_good)
    for file in files:
        if os.path.isfile(g_good + "/" + file) and file.endswith(".jpg"):
            name = file.split(".")[0]
            if "-" in name:
                name = name.split("-")[0]
            g_map[g_good + "/" + file] = name

def compare_img(img1, img2):
    w = min(img1.size[0], img2.size[0])
    h = min(img1.size[1], img2.size[1])
    
    tpt = 0
    ept = 0
    
    for wp in range(w):
        for hp in range(h):
            tpt = tpt + 1
            p1 = img1.getpixel((wp, hp))
            p2 = img2.getpixel((wp, hp))
            
            if abs(p1[0] - p2[0]) < g_equal_factor and \
                abs(p1[1] - p2[1]) < g_equal_factor and \
                abs(p1[2] - p2[2]) < g_equal_factor:
                    ept = ept + 1
            
    return int(((float)(ept)/tpt)*10000)
   
def crackone(img):
    maxfactor = None
    name = None
    for k, v in g_map.items():
        img_cmp = Image.open(k)
        f = compare_img(img, img_cmp)
        if f >= g_stop_factor:
            return v
        if maxfactor == None:
            maxfactor = f
            name = v
        else:
            if f > maxfactor:
                maxfactor = f
                name = v
    return name

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

def analyse(src_img, size):
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

    return proc_img

def crack(file, size):
    img = analyse(Image.open(file), size)
    sub_imgs = []
    # 切分4个图
    for i in range(4):
        save_size = (i*size[0], 0, (i+1)*size[0], size[1])
        sub_img = img.crop(save_size)
        sub_imgs.append(sub_img)
        
    name = ""    
    for sub_img in sub_imgs:
        name = name + crackone(sub_img)
        
    shutil.copyfile(file, g_crack + "/" + name + ".jpg")

def main():
    load()
    
    files = os.listdir(g_orig)
    for file in files:
        if os.path.isfile(g_orig + "/" + file) and file.endswith(".jpg"):
            print "crack: " + g_orig + "/" + file
            crack(g_orig + "/" + file, g_size)
    
if __name__ == "__main__":
    main()