#!/usr/bin/env python3

import argparse
import os

parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-a', dest='Analyzer', default="Tagging_RF_Flavor_DL")
parser.add_argument('-e', dest='Era', default="2018")
parser.add_argument('-ch', dest='Channel', default="MM")
parser.add_argument('-tagger', dest='Tagger', default="C")
parser.add_argument('-analysis', action='store_true', default=False)
parser.add_argument('-validation', action='store_true', default=False)
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

path_base = os.environ['Vcb_Post_Analysis_WD']
if args.Analyzer == "Tagging_RF_Flavor_DL":
    path = f"{path_base}/Workplace/Tagging_RF_Flavor_DL"
elif args.Analyzer == "Tagging_RF_DL":  
    path = f"{path_base}/Workplace/Tagging_RF_DL"

if args.analysis == True:
    target=f"{path}/Vcb_{args.Analyzer}_{args.Era}_{args.Channel}.root"
elif args.validation == True:
    target=f"{path}/Vcb_{args.Analyzer}_Validation_{args.Era}_{args.Channel}_{args.Tagger}_Tagger.root"
    

try: os.remove(target)
except OSError:
    pass

root_list = [os.path.join(path, root_list) for root_list in os.listdir(path)]
root_list = [root_file for root_file in root_list if root_file.endswith(".root")] 
if args.analysis == True:
    root_list = [root_file for root_file in root_list if f"Vcb_{args.Analyzer}_{args.Era}_{args.Channel}" in root_file]  
elif args.validation == True:
    root_list = [root_file for root_file in root_list if f"Vcb_{args.Analyzer}_Validation_{args.Era}_{args.Channel}_{args.Tagger}_Tagger" in root_file]
    
hadd_cmd = f"hadd -d {path_base}/Tmp {target} "
for root_file in root_list:
    hadd_cmd += root_file + " " 
    
print(hadd_cmd)
os.system(hadd_cmd)
