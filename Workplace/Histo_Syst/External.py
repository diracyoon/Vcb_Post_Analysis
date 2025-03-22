#!/usr/bin/env python3

chk_jes_breakdown = False

#Era = ['2016preVFP', '2016postVFP', '2017', '2018']
Era = ['2017']

Channel = ['Mu', 'El']

if chk_jes_breakdown:
    last_index_type_type = 50
else:
    last_index_type_type = 28
    
## Submit_2D
with open("external_2d.dat", "w") as two_d:
    for era in Era:
        for channel in Channel:
            for index_tree_type in range(0, last_index_type_type):
                    two_d.write(f"{era}, {channel}, {index_tree_type}, {last_index_type_type}\n")
                    
## Submit_Cal_TF
with open ("external.dat", "w") as cal_tf:
    for era in Era:
        for channel in Channel:
            cal_tf.write(f"{era}, {channel}\n")