#!/usr/bin/env python3

#chk_jes_breakdown = False
chk_jes_breakdown = True

Era = ['2016preVFP', '2016postVFP', '2017', '2018']
#Era = ['2016preVFP', '2016postVFP',]
#Era = ['2018']
#Era = ['2016preVFP']

Channel = ['Mu', 'El']
#Channel = ['Mu']
#Channel = ['El']

Tagger = 'B'
#Tagger = 'C'

if chk_jes_breakdown:
    last_index_type_type = 58
else:
    last_index_type_type = 36
    
## Submit_2D
with open("external_2d.dat", "w") as two_d:
    for era in Era:
        if era == "2016preVFP":
            Req_Memory = "24*1024"
        elif era == "2016postVFP":
            Req_Memory = "24*1024"
        elif era == "2017":
            Req_Memory = "26*1024"
        elif era == "2018":
            Req_Memory = "30*1024"
            
        for channel in Channel:
            for index_tree_type in range(0, last_index_type_type):
                    two_d.write(f"{era}, {channel}, {index_tree_type}, {last_index_type_type}, {Tagger}, {Req_Memory}\n")
                    
## Submit_Cal_TF
with open ("external.dat", "w") as cal_tf:
    for era in Era:
        for channel in Channel:
            cal_tf.write(f"{era}, {channel}, {Tagger}\n")
