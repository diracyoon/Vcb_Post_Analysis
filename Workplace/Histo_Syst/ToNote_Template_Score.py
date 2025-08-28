#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='Era', default="2017")
parser.add_argument('-ch', dest='Channel', default="Mu")
args = parser.parse_args()

if args.Era=="2016a": args.Era="2016preVFP"
if args.Era=="2016b": args.Era="2016postVFP"

import os
path=os.environ['Vcb_Post_Analysis_WD']
path=f"{path}/Workplace/Histo_Syst"

import shutil

## To BRExtraction  ##
pic_list = ['Template_MVA_Score',
]

for pic in pic_list:
    origin = f"Signal_{args.Era}_{args.Channel}_{pic}.png"
    origin = f"{path}/{origin}"
    print(origin)

    des = f"/data6/Users/isyoon/CMS_Note/AN-23-046/Figs/BRExtraction/"

    if os.path.isfile(origin):
        shutil.copy(origin, des)
