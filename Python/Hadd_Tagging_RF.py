#!/usr/bin/env python3

import argparse
import os

parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-a', dest='Analyzer', default="Tagging_RF_Flavor")
parser.add_argument('-e', dest='Era', default="2018")
parser.add_argument('-ch', dest='Channel', default="Mu")
parser.add_argument('-tagger', dest='Tagger', default="C")
parser.add_argument('-validation', action='store_true', default=False)
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

path = os.environ['Vcb_Post_Analysis_WD']
if args.Analyzer == "Tagging_RF_Flavor":
    path = f"{path}/Workplace/Tagging_RF_Flavor"
elif args.Analyzer == "Tagging_RF":  
    path = f"{path}/Workplace/Tagging_RF"
    
target=f"{path}/Vcb_{args.Analyzer}_Validation_{args.Era}_{args.Channel}_{args.Tagger}_Tagger.root"

try: os.remove(target)
except OSError:
    pass

root_list = [os.path.join(path, root_list) for root_list in os.listdir(path)]
root_list = [root_file for root_file in root_list if root_file.endswith(".root")] 
if args.validation == True:
    root_list = [root_file for root_file in root_list if f"Vcb_{args.Analyzer}_Validation_{args.Era}_{args.Channel}" in root_file]  

hadd_cmd = f"hadd -v 99 -j 3 {target} "
for root_file in root_list:
    hadd_cmd += root_file + " " 
    
#print(hadd_cmd)
os.system(hadd_cmd)
