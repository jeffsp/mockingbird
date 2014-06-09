#! /usr/bin/env python
#
# @file mkchains.py
# @brief make command chains for input to xargs
# @author Jeff Perry <jeffsp@gmail.com>
# @version 1.0
# @date 2014-06-09

import sys

def parse_args():

    import argparse
    parser=argparse.ArgumentParser(description="Build command chains for input to 'xargs -I{} sh -c {}'")
    parser.add_argument('--fs', '-f',action='store',dest='fs',type=str,default='*.*',help="quoted file input file specificiation")
    parser.add_argument('--dir','-d',action='store',dest='dir',type=str,default='/tmp',help="output directory")
    parser.add_argument('--ext','-x',action='store',dest='ext',type=str,default='.txt',help="output file extension")
    parser.add_argument('cmd',metavar='cmd',nargs='*',help="commands")
    args=parser.parse_args()

    if not args.cmd:
        print "No commands were specified"
        parser.print_help()
        sys.exit(-1)

    return args

import glob
import os.path

if __name__ =='__main__':

    args=parse_args()

    sys.stderr.write("fs=%s\n"%args.fs)
    sys.stderr.write("dir=%s\n"%args.dir)
    sys.stderr.write("ext=%s\n"%args.ext)
    sys.stderr.write("cmd=[")
    for c in args.cmd:
        sys.stderr.write("%s,"%c)
    sys.stderr.write("]\n")

    for f in glob.glob (args.fs):
        (dn,fn)=os.path.split(f)
        (bn,en)=os.path.splitext(fn)
        sys.stdout.write(args.cmd[0])
        sys.stdout.write(" < ")
        sys.stdout.write(f)
        for c in args.cmd[1:]:
            sys.stdout.write(" | ")
            sys.stdout.write(c)
        sys.stdout.write(" > ")
        sys.stdout.write(args.dir)
        sys.stdout.write("/")
        sys.stdout.write(bn)
        sys.stdout.write(args.ext)
        sys.stdout.write("\n")
