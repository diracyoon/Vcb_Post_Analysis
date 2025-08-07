#!/usr/bin/env python3

import argparse
import os
import subprocess

parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2018")
parser.add_argument('-ch', dest='channel', default="Mu")
parser.add_argument('-tagger', dest='tagger', default="C")
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

path_base = os.environ['Vcb_Post_Analysis_WD']
path = f"{path_base}/Workplace/Histo_Syst"

target=f"{path}/Vcb_2D_{args.era}_{args.channel}_{args.tagger}_tagger_All.root"

try: os.remove(target)
except OSError:
    pass

root_list = [os.path.join(path, root_list) for root_list in os.listdir(path)]
root_list = [root_file for root_file in root_list if root_file.endswith(".root")] 
root_list = [root_file for root_file in root_list if f"Vcb_2D_{args.era}_{args.channel}_{args.tagger}" in root_file]

hadd_cmd = f"hadd -d {path_base}/Tmp -j 5 {target} "
for root_file in root_list:
    hadd_cmd += root_file + " "

print(hadd_cmd)
os.system(hadd_cmd)
