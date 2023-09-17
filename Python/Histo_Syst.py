#!/usr/bin/env python3

import argparse
parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
parser.add_argument('-e', dest='era', default="2017")
parser.add_argument('-ch', dest='channel', default="El")
parser.add_argument('-merge', dest='merge', type=bool, default=False)
parser.add_argument('-run_flag', dest='run_flag', type=int, default=1)
parser.add_argument('-template_on', dest='template_on', type=int, default=0)
args = parser.parse_args()

if args.era=="2016a": args.era="2016preVFP"
if args.era=="2016b": args.era="2016postVFP"

run_flag=""
if args.run_flag == 0: run_flag="Data"
elif args.run_flag == 1: run_flag="Central09"
elif args.run_flag == 2: run_flag="Central19"
elif args.run_flag == 3: run_flag="Central29"
elif args.run_flag == 4: run_flag="Central39"
elif args.run_flag == 5: run_flag="Central49"
elif args.run_flag == 6: run_flag="Central59"
elif args.run_flag == 7: run_flag="Central69"
elif args.run_flag == 8: run_flag="Central79"
elif args.run_flag == 9: run_flag="Central89"
elif args.run_flag == 10: run_flag="Jet_En_Down"
elif args.run_flag == 11: run_flag="Jet_En_Up" 
elif args.run_flag == 12: run_flag="Jet_Res_Down"
elif args.run_flag == 13: run_flag="Jet_Res_Up"
elif args.run_flag == 14: run_flag="UE_Down"
elif args.run_flag == 15: run_flag="UE_Up"
elif args.run_flag == 16: run_flag="CP5_Down"
elif args.run_flag == 17: run_flag="CP5_Up"
elif args.run_flag == 18: run_flag="hDamp_Down"
elif args.run_flag == 19: run_flag="hDamp_Up"
elif args.run_flag == 20: run_flag="mTop_171p5"
elif args.run_flag == 21: run_flag="mTop_173p5" 
#elif args.run_flag == 20: run_flag="El_En_Down"
#elif args.run_flag == 21: run_flag="El_En_Up"
#elif args.run_flag == 22: run_flag="El_Res_Down"
#elif args.run_flag == 23: run_flag="El_Res_Up"
else: run_flag="All" 

if args.channel == "Mu":
    if "El_" in run_flag: exit()

#if "Jet_Res_" in run_flag: exit()

import ROOT
import os
path=os.environ['Vcb_Post_Analysis_WD']
ROOT.gSystem.Load(f"{path}/Build/libVcb_Post_Analysis.so")

try:
    histo_syst = ROOT.Histo_Syst(args.era, args.channel, args.merge, run_flag, args.template_on)
except:
    print("Exception")
else:
    if args.merge == False: histo_syst.Run()
    else: histo_syst.Run_Merge()
