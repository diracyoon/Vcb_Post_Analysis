#include "Template.h"

ClassImp(Template);

//////////

Template::Template(const TString& a_era, const TString& a_channel, const TString& a_swap_mode, const TString& a_draw_extension) : samples(a_era, a_channel), event(a_era, a_channel, a_swap_mode), n_bin_histo_score(50)
{
  cout << "[Template::Template]: Init analysis" << endl;

  era = a_era;
  channel = a_channel;
  draw_extension = a_draw_extension;

  if(era=="2018")
    {
      bvsc_wp_m = bvsc_wp_m_2018;
      cvsb_wp_m = cvsb_wp_m_2018;
      cvsl_wp_m = cvsl_wp_m_2018;
    }
  
  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunResult/";
  
  if(samples.map_mc.size()!=samples.map_short_name_mc.size())
    {
      cout << "size of samples::maps is not matched. Check it first." << endl;
      exit(EXIT_FAILURE);
    }

  for(auto it=samples.map_mc.begin(); it!=samples.map_mc.end(); it++)
    {
      cout << it->first << endl;

      map_fin_mc[it->first] = new TFile(path_base+it->second);
      map_tree_mc[it->first] = (TTree*)map_fin_mc[it->first]->Get("Result_Tree");

      event.Setup_Tree(map_tree_mc[it->first]);
    }
  
  for(auto it=samples.map_short_name_mc.begin(); it!=samples.map_short_name_mc.end(); it++)
    {
      cout << it->second << endl;
      vec_short_name_mc.push_back(it->second);
    }
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " <<  n_sample_merge_mc << endl;

  for(auto it=vec_short_name_mc.begin(); it!=vec_short_name_mc.end(); it++)
    {
      if(*it!="TTLJ" && *it!="TTLJ_WtoCB") vec_histo_sample.push_back(*it);
    }
  vec_histo_sample.push_back("TTLJ_ud");
  vec_histo_sample.push_back("TTLJ_us");
  vec_histo_sample.push_back("TTLJ_cd");
  vec_histo_sample.push_back("TTLJ_cs");
  vec_histo_sample.push_back("TTLJ_cb");
  n_histo_sample = vec_histo_sample.size();

  cout << "n_sample_merge_mc = " <<  n_sample_merge_mc << ", n_histo_sample = " << n_histo_sample << endl;

  n_fail_reason = sizeof(fail_reason)/sizeof(TString);
  n_histo_type = n_fail_reason + 1;

  histo_template = new TH1D**[n_histo_sample];
  stack_template = new THStack*[n_histo_sample];

  for(int i=0; i<n_histo_sample; i++)
    {
      histo_template[i] = new TH1D*[n_histo_sample];
      stack_template[i] = new THStack(vec_histo_sample[i], vec_histo_sample[i]);
      
      for(int j=0; j<n_histo_type; j++)
	{
	  TString name;
	  name = vec_histo_sample[i] + "_Correct";
	  if(j!=0) name = vec_histo_sample[i] + "_Wrong_" + fail_reason[j-1];
	  
	  histo_template[i][j] = new TH1D(name, name , n_bin_histo_score, 0, 0.5);
	}
    }

  reader = new TMVA::Reader("!Color:!Silent");
  reader->AddSpectator("best_mva_score", &event.best_mva_score);
  reader->AddVariable("bvsc_w_u", &event.bvsc_w_u);
  reader->AddVariable("cvsb_w_u", &event.cvsb_w_u);
  reader->AddVariable("cvsl_w_u", &event.cvsl_w_u);
  reader->AddVariable("m_w_u", &event.m_w_u);
  reader->AddVariable("bvsc_w_d", &event.bvsc_w_d);
  reader->AddVariable("cvsb_w_d", &event.cvsb_w_d);
  reader->AddVariable("cvsl_w_d", &event.cvsl_w_d);
  reader->AddVariable("m_w_d", &event.m_w_d);

  TString weight_file;
  
  weight_file = "dataset/weights/TMVAClassification_DNN_CPU.weights.xml";
  reader->BookMVA("DNN_CPU", weight_file);
}//Template::Template()

//////////

Template::~Template()
{
}//Template::~Template()

//////////

void Template::Run()
{
  Read_Tree();
  Draw();
  
  return;
}//void Template::Run()

//////////

void Template::Draw()
{
  cout << "[Template::Draw] Init Draw" << endl;  
  
  canvas[0] = new TCanvas("Canvas_TTLJ", "Canvas_TTLJ", 1200, 800);
  canvas[1] = new TCanvas("Canvas_Others", "Canvas_Others", 1200, 800);
  
  canvas[0]->Divide(3, 2);
  canvas[1]->Divide(3, 2);

  for(int i=0; i<n_histo_sample; i++)
    {
      cout << vec_histo_sample[i] << endl;
      
      for(int j=0; j<n_histo_type; j++)
	{
	  histo_template[i][j]->SetFillColorAlpha(color[j], 0.20);
	  stack_template[i]->Add(histo_template[i][j]);
	}
      
      int canvas_index;
      if(vec_histo_sample[i].Contains("TTLJ_")) canvas_index = 0;
      else canvas_index = 1;
      
      int index;
      if(vec_histo_sample[i]=="TTLJ_ud") index = 1;
      else if(vec_histo_sample[i]=="TTLJ_us") index = 2;
      else if(vec_histo_sample[i]=="TTLJ_cd") index = 4;
      else if(vec_histo_sample[i]=="TTLJ_cs") index = 5;
      else if(vec_histo_sample[i]=="TTLJ_cb") index = 6;
      else if(vec_histo_sample[i]=="TTLL") index = 1;
      else if(vec_histo_sample[i]=="ST") index = 2;
      else if(vec_histo_sample[i]=="DYJets") index = 3;
      else if(vec_histo_sample[i]=="WJets") index = 4;
      else if(vec_histo_sample[i]=="QCD_bEn") index = 5;

      canvas[canvas_index]->cd(index);
      stack_template[i]->Draw("BAR");
    }//n_histo_sample
  
  canvas[0]->Print("Canvas_TTLJ."+draw_extension, draw_extension);
  canvas[1]->Print("Canvas_Others."+draw_extension, draw_extension);

  return;
}//void Template::Draw()

//////////

void Template::Fill_Histo(const TString& short_name, const int& index_decay_mode, const int& index_fail_reason)
{
  int index = Get_Index(short_name, index_decay_mode);
  
  float weight = 1;
  weight *= event.weight_lumi;
  weight *= event.weight_mc;
  weight *= event.weight_pileup;
  weight *= event.weight_prefire;
  weight *= event.weight_top_pt;
  weight *= event.sf_mu_trig;
  weight *= event.sf_mu_id;
  weight *= event.sf_mu_iso;
  weight *= event.sf_pujet_veto;
  weight *= event.sf_b_tag;
  weight *= event.sf_c_tag;
  
  histo_template[index][index_fail_reason]->Fill(mva_score, weight);

  return;
}//void Template::Fill_Histo(const TString& short_name, const int& index_decay_mode, const int& index_fail_reason)

//////////

int Template::Get_Index(const TString& name)
{
  int index;
  for(auto it=vec_histo_sample.begin(); it!=vec_histo_sample.end(); it++)
    {
      if(*it==name)
        {
          index = it - vec_histo_sample.begin();
          break;
        }
    }

  return index;
}//int Template::Get_Index(const TString& name)

//////////

int Template::Get_Index(const TString& short_name, const int& index_decay_mode)
{
  TString name;
  if(short_name=="TTLJ" || short_name=="TTLJ_WtoCB")
    {
      if(index_decay_mode==21) name = "TTLJ_ud";
      else if(index_decay_mode==23) name = "TTLJ_us";
      else if(index_decay_mode==41) name = "TTLJ_cd";
      else if(index_decay_mode==43) name = "TTLJ_cs";
      else if(index_decay_mode==45) name = "TTLJ_cb";
    }
  else name = short_name;

  int index = Get_Index(name);

  return index;
}//int Template::Get_Index(const TString& short_name, const int& index_decay_mode)

//////////

void Template::Read_Tree()
{
  cout << "[Template::Read_Tree]: Start to read trees " << endl;

  for(auto it=map_tree_mc.begin(); it!=map_tree_mc.end(); it++)
    {
      TString short_name = samples.map_short_name_mc[it->first];

      cout << it->first << " " << short_name << endl;
      TTree* tree = map_tree_mc[it->first];
      
      Long64_t n_entries = tree->GetEntries();
      for(Long64_t i=0; i<n_entries; i++)
     	{
	  tree->GetEntry(i);
	  if(i%5000000==0) cout << "Processing... " << i << "/" << n_entries << endl;

	  event.Swap();

	  int index_fail_reason = -1;
	  if(event.chk_reco_correct) index_fail_reason = 0;//correct
	  else
	    {
	      if(event.chk_included==true && event.chk_hf_contamination==false) index_fail_reason = 1;
	      else if(event.chk_included==false && event.chk_hf_contamination==false) index_fail_reason = 2;
	      else if(event.chk_included==true && event.chk_hf_contamination==true) index_fail_reason = 3;
	      else if(event.chk_included==false && event.chk_hf_contamination==true) index_fail_reason = 4;
	    }

	  // //ttbar pt cut
	  // if(event.pt_had_t_b<25 || event.pt_lep_t_b<25) continue;
	    
	  //b-tagging on ttbar
	  if(event.bvsc_had_t_b<bvsc_wp_m || event.bvsc_lep_t_b<bvsc_wp_m) continue;

	  //mva score cut
	  if(event.best_mva_score<0.9) continue;

	  //b-tagging on w_d
	  if(event.bvsc_w_d<bvsc_wp_m) continue;

	  //c-tagging on w_u
	  if(event.cvsb_w_u<cvsb_wp_m || event.cvsl_w_u<cvsl_wp_m) continue;

	  //no additional c-tagging
	  if(event.n_cjets!=1) continue;

	  mva_score = reader->EvaluateMVA("DNN_CPU");
	  
	  Fill_Histo(short_name, event.decay_mode, index_fail_reason);

	  //if(100000<i) break;
	}//loop over entries 
    }//loop over map_fin
  
  return;
}//void Template::Read_Tree()

//////////
