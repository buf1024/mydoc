#!/usr/bin/env python

import sys
from bencode import *

def dumpbtlist(l, sp):
    for i in l:
        if type(i) == type({}):
            dumpbtkeys(i, sp + "-")
        elif type(i) == type([]):
            dumpbtlist(i, sp + "-")
        #else:
        #    print sp + str(i).decode("utf-8")


def dumpbtkeys(d, sp = ""):
    for k in d.keys():
        print sp + k
        if type(d[k]) == type({}):
            dumpbtkeys(d[k], sp + "-")
        elif type(d[k]) == type([]):
            dumpbtlist(d[k], sp + "-")

def main():
    if len(sys.argv) < 1:
        print "needs bt file as the first arguments"
        return
    fname = sys.argv[1]

    cnt = ""

    with open(fname, "rb") as f:
        cnt = f.read()

    d = bdecode(cnt)

    dumpbtkeys(d, "|")
    
if __name__ == "__main__":
    main()
