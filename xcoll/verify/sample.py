#!/bin/env python
#coding: utf-8

import urllib
import time

#为保护此网站，URL隐藏掉了，可以设置为可写验证码的URL
g_url = "xxxxxxxx"
g_dir = "./sample"
g_sleep = 0

def get_sample(url, path):
    image = urllib.urlopen(url).read()
    with open(path, "wb") as f:
        f.write(image)
        
    print 'write: ' + path
        
def get_samples():
    count = 0
    while True:
        count = count + 1
        path = g_dir + "/" + str(count) + ".jpg"
        get_sample(g_url, path)
        
        time.sleep(g_sleep)
        
if __name__ == "__main__":
    get_samples()