#!/usr/bin/env python3

import argparse
import os
import subprocess

parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2018")
parser.add_argument('-ch', dest='channel', default="Mu")
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

target=f"Vcb_Histos_{args.era}_{args.channel}_All.root"

try: os.remove(target)
except OSError:
    pass

root_list = os.listdir(".")
root_list = [root_file for root_file in root_list if root_file.endswith(".root")] 
root_list = [root_file for root_file in root_list if not root_file.find(f"Vcb_Histos_{args.era}_{args.channel}_")]
print(root_list)

hadd_cmd = f"hadd {target} "
for root_file in root_list:
    hadd_cmd += root_file + " "

print(hadd_cmd)
os.system(hadd_cmd)
