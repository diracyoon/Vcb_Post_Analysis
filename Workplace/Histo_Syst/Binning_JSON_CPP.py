#!/usr/bin/env python3

import json
import sys
import os
import argparse

def generate_cpp_code(era, channel):
    base = "/data6/Users/yeonjoon_public/Binning_20260403"
    filename = f"{base}/bins_{era}_{channel}.json"

    if not os.path.exists(filename):
        print(f"오류: '{filename}' 파일을 찾을 수 없습니다.")
        return

    with open(filename, 'r', encoding='utf-8') as f:
        data = json.load(f)

    edges = data["edges"]
    thr_edge_u = data["t_equal_meta"]["thr_edge_u"]

    group_0, group_1 = [], []

    for val in edges:
        formatted_val = f"{val:.3f}"
        
        # thr_edge_u 이하인 경우 group_0에 추가
        if val <= thr_edge_u:
            group_0.append(formatted_val)
            
        # thr_edge_u 이상인 경우 group_1에 추가 (경계값 중복 포함)
        if val >= thr_edge_u:
            group_1.append(formatted_val)

    # 결과 출력
    print(f"// Era: {era}, Channel: {channel}")
    print(f"bin_template_mva_score[0] = {{{', '.join(group_0)}}};")
    print(f"bin_template_mva_score[1] = {{{', '.join(group_1)}}};")
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Vcb_Post_Analysis Command')
    parser.add_argument('-e', dest='Era', default="2017")
    parser.add_argument('-ch', dest='Channel', default="Mu")
    args = parser.parse_args()

    if args.Era=="2016a": args.Era="2016preVFP"
    if args.Era=="2016b": args.Era="2016postVFP"
    
    generate_cpp_code(args.Era, args.Channel)
