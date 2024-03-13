import ROOT, re
merge_dict = {}
target_var = ''
regions = []
rebinN = 0
era = ''

class mydir:
    def __init__(self, name, tdir=None, subdirs = []):
        self.name = name
        self.tdir = tdir
        self.subdirs = subdirs 

def reBin(h):
    return h.Rebin(rebinN)



def set_var(input_root_file):
    
    global merge_dict
    global target_var
    global regions
    global era
    global rebinN
    
    merge_dict['JJ_TTLJ_2'] = {"TTLJ_2"}
    merge_dict['JJ_TTLJ_4'] = {"TTLJ_4"}
    merge_dict['BB_TTLJ_2'] = {"TTLJ_BB_2"}
    merge_dict['BB_TTLJ_4'] = {"TTLJ_BB_4"}
    merge_dict['CC_TTLJ_2'] = {"TTLJ_CC_2"}
    merge_dict['CC_TTLJ_4'] = {"TTLJ_CC_4"}

    merge_dict['JJ_TTLL'] = {"TTLL"} 
    merge_dict['BB_TTLL'] = {"TTLL_BB"}
    merge_dict['CC_TTLL'] = {"TTLL_CC"}

    merge_dict['VJets'] = ['DYJets','WJets']
    merge_dict['QCD_bEn'] = ["QCD_bEn"]
    merge_dict['VV'] = ["VV"]
    merge_dict['ST'] = ["ST_tw","ST_tch","ST_sch"]
    merge_dict['ttV'] = ["ttZ","ttW","ttH"]
    
    merge_dict['WtoCB'] = ["TTLJ_CC_45","TTLJ_BB_45","TTLJ_45"]
    
    target_var = 'Template_MVA_Score'
    
    regions = ['Control0', 'Signal']
    
    pattern = r'_(201[^_]+)_'
    match = re.search(pattern, input_root_file)
    
    if match:
        era = match.group(1)
    
    rebinN = 2
    
    if 'DL' in input_root_file: #harcoded
        merge_dict['JJ_TTLL'] = {"TTLL_JJ"} 
        merge_dict['JJ_TTLJ_2'] = {"TTLJ_JJ_2"}
        merge_dict['JJ_TTLJ_4'] = {"TTLJ_JJ_4"}
        merge_dict['WtoCB'] = ["TTLJ_CC_45","TTLJ_BB_45","TTLJ_JJ_45"]
        target_var = 'BvsC_3rd_4th_Jets_Unrolled'
        regions = ['Control_DL']
        rebinN = 1

def find_mydir(mydir_list, folder_name):
    for my_dir in mydir_list:
        if my_dir.name == folder_name:
            return my_dir
    print(f"no {folder_name} in list")
    return None

def get_short_procs_name(proc):
    for short, procs in merge_dict.items():
        for p in procs:
            if p == proc:
                return short
    return proc
        
def get_syst_name(syst):
    decorr_systs = ['B_Tag_HFStats1','B_Tag_HFStats2','B_Tag_LFStats1','B_Tag_LFStats2','Jet_Res']

    for decorr_syst in decorr_systs:
        if decorr_syst in syst:
            syst = syst.replace(decorr_syst,decorr_syst+"_"+era)
    if syst == 'Top_Pt_Reweight':
        return'Top_Pt_Reweight_Up'
    elif syst == 'mTop_171p5':
        return 'mTop_Down'
    elif syst == 'mTop_173p5':
        return 'mTop_Up'
    elif syst == 'PDF_Alternative':
        return'PDF_Alternative_Up'
    else:
        return syst
    
    
        
def postprocessing(input_root_file_path,output_root_file_path):
    input_root_file = ROOT.TFile(input_root_file_path,'READ')
    output_root_file = ROOT.TFile(output_root_file_path,'RECREATE')
    for region in regions:
        print(region)
        main_dir =input_root_file.GetDirectory(region)
        out_dir  = output_root_file.mkdir(region)
        ##write data_obs
        nominal_out_dir = out_dir.mkdir('Nominal')
        data_out_dir = nominal_out_dir.mkdir('data_obs')
        
        if region != 'Signal':
            h = main_dir.Get(f'Data/{target_var}')
            data_out_dir.WriteObject(reBin(h),target_var)
        else:
            h = main_dir.Get(f'Nominal/VV/{target_var}').Clone("Data")
            h.Scale(0)
            data_out_dir.WriteObject(reBin(h),target_var)
            
        
            
        for syst_key in main_dir.GetListOfKeys():
            
            syst_folder = syst_key.ReadObj()
            syst_name = syst_folder.GetName() 
            if re.compile(r'^PDF_Error_Set_\d+$').match(syst_name) or syst_name == 'Data':
                continue
            
            if syst_name=='Nominal':
                syst_folder_out = nominal_out_dir 
            else:
                syst_name = get_syst_name(syst_name)
                syst_folder_out = out_dir.mkdir(syst_name)
                
            print(f'    {syst_name}') 
                
                
            for proc_name, procs in merge_dict.items():
                #print(f"        {proc_name}")
                procs_folder_out = syst_folder_out.mkdir(proc_name)
                h_dict = {} 
                for i, proc in enumerate(procs):
                    #print(f"            {proc}")
                    procs_folder = syst_folder.GetDirectory(proc)
                    for var_key in procs_folder.GetListOfKeys():
                        if var_key.GetName() != target_var: continue
                        if i == 0:
                            h_dict[var_key.ReadObj().GetName()] = reBin(var_key.ReadObj().Clone())
                        else:
                            h_dict[var_key.ReadObj().GetName()].Add(reBin(var_key.ReadObj()))

                for var, h in h_dict.items():
                    procs_folder_out.WriteObject(h,var)
        
        syst_folder_out = out_dir.mkdir('Top_Pt_Reweight_Down')
        print(' Top_Pt_Reweight_Down')
        for proc_name, procs in merge_dict.items():
            procs_folder_out = syst_folder_out.mkdir(proc_name)
            nominal_hist = out_dir.Get(f'Nominal/{proc_name}/{target_var}')
            up_hist = out_dir.Get(f'Top_Pt_Reweight_Up/{proc_name}/{target_var}') 
            down_hist = nominal_hist.Clone()
            for i in range(1,nominal_hist.GetNbinsX()+1):
                binVal = 2*nominal_hist.GetBinContent(i)-up_hist.GetBinContent(i)
                down_hist.SetBinContent(i,binVal)
            procs_folder_out.WriteObject(reBin(down_hist),var)

        syst_folder_out = out_dir.mkdir('PDF_Alternative_Down')
        print(' PDF_Alternative_Down') 
        for proc_name, procs in merge_dict.items():
            procs_folder_out = syst_folder_out.mkdir(proc_name)
            nominal_hist = out_dir.Get(f'Nominal/{proc_name}/{target_var}')
            up_hist = out_dir.Get(f'Top_Pt_Reweight_Up/{proc_name}/{target_var}') 
            down_hist = nominal_hist.Clone()
            for i in range(1,nominal_hist.GetNbinsX()+1):
                binVal = 2*nominal_hist.GetBinContent(i)-up_hist.GetBinContent(i)
                down_hist.SetBinContent(i,binVal)
            procs_folder_out.WriteObject(reBin(down_hist),var)

            
    input_root_file.Close()
    output_root_file.Close()
                
    
if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-i', dest='input')
    #parser.add_argument('-o', dest='output')
    args = parser.parse_args()
 
    input_root_file_path = args.input  # Replace with the path to your input ROOT file
    output_root_file_path = input_root_file_path.replace('.root','_processed.root')  # Replace with the desired output ROOT file name
    set_var(input_root_file_path)
    postprocessing(input_root_file_path,output_root_file_path)
