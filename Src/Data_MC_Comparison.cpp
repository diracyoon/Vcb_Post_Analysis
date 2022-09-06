#include "Data_MC_Comparison.h"

ClassImp(Data_MC_Comparison);

//////////

Data_MC_Comparison::Data_MC_Comparison(const TString& a_era, const TString& a_channel, const TString& a_swap_mode) : samples(a_era, a_channel), event(a_era, a_channel, a_swap_mode)
{
  cout << "[Data_MC_Comparison::Data_MC_Comparison]: Init analysis" << endl;

  era = a_era;
  channel = a_channel;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunResult/";
  
  if(samples.map_mc.size()!=samples.map_short_name_mc.size() || samples.map_data.size()!=samples.map_short_name_data.size())
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
  
  path_base += "DATA/";
  for(auto it=samples.map_data.begin(); it!=samples.map_data.end(); it++)
    {
      cout << it->first << endl;
      if(it==samples.map_data.begin()) data_short_name = samples.map_short_name_data[it->first];
      
      map_fin_data[it->first] = new TFile(path_base+it->second);
      map_tree_data[it->first] = (TTree*)map_fin_data[it->first]->Get("Result_Tree");

      event.Setup_Tree(map_tree_data[it->first]);
    }
  
  //setup histos
  histo_conf = {{"N_Vertex", 80, 0, 80},
                {"Lepton_Pt", 50, 0, 300},
		{"Lepton_Eta", 50, -3, 3},
		{"N_Jets", 30, 0, 30},
		{"N_BJets", 30, 0, 30},
		{"N_CJets", 30, 0, 30},
		{"Pt_Leading_Jet", 50, 0, 300},
		{"Pt_Subleading_Jet", 50, 0, 300},
		{"Eta_Leading_Jet", 50, -3, 3},
		{"Eta_Subleading_jet", 50, -3, 3},
		{"Met_Pt", 50, 0, 300},
		{"Met_Phi", 80, -4, 4},
		{"Best_MVA_Score", 50, 0, 1}};
  n_histo_conf = histo_conf.size();
  cout << "n_histo_conf = " << n_histo_conf << endl;

  for(auto it=samples.map_short_name_mc.begin(); it!=samples.map_short_name_mc.end(); it++)
    {
      cout << it->second << endl;
      vec_short_name_mc.push_back(it->second);
    }
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " <<  n_sample_merge_mc << endl;
  
  histo_mc = new TH1D**[n_histo_conf];
  for(int i=0; i<n_histo_conf; i++)
    {
      histo_mc[i] = new TH1D*[n_sample_merge_mc];
      for(int j=0; j<n_sample_merge_mc; j++)
	{
	  TString histo_title = histo_conf[i].variable_title + "_" + vec_short_name_mc[j];
	  cout << histo_title << endl;
	  histo_mc[i][j] = new TH1D(histo_title, histo_title, histo_conf[i].n_bin, histo_conf[i].x_low, histo_conf[i].x_up);
	}//loop over n_sample_merge_mc
    }//n_histo_conf 

  histo_data = new TH1D*[n_histo_conf];
  for(int i=0; i<n_histo_conf; i++)
    {
      TString histo_title = histo_conf[i].variable_title + "_" + data_short_name;
      cout << histo_title << endl;
      histo_data[i] = new TH1D(histo_title, histo_title, histo_conf[i].n_bin, histo_conf[i].x_low, histo_conf[i].x_up);
    }
}//Data_MC_Comparison::Data_MC_Comparison(const TString& a_era, const TString& a_channel))

//////////

Data_MC_Comparison::~Data_MC_Comparison()
{
}//Data_MC_Comparison::~Data_MC_Comparison()

//////////

void Data_MC_Comparison::Run()
{
  Read_Tree();
  Draw();
}//void Data_MC_Comparison::Run()

//////////

void Data_MC_Comparison::Set_Region(const TString& a_region_type)
{
  cout << "DataMC_Comparison::Set_Region" << endl;
  
  region_type = a_region_type;
  
  if(region_type!="Control0" && region_type!="Control1" && region_type!="Control2")
    {
      cout << "[Data_MC_Comparison::Set_Region] unknown region_type = " << region_type << ". Check it." << endl;   
      exit(EXIT_FAILURE);
    }
  
  return;
}//void Data_MC_Comparision::Set_Region()

//////////

bool Data_MC_Comparison::Cut()
{
  //common cut
  if(event.pt_had_t_b<25) return true;
  if(event.pt_lep_t_b<25) return true;
  if(event.bvsc_had_t_b<bvsc_wp_m_2018) return true;
  if(event.bvsc_lep_t_b<bvsc_wp_m_2018) return true;

  //b-inversion
  if(region_type=="Control0") 
    {
      //permutation MVA
      if(event.best_mva_score<0.80) return true;
      
      //only 2 b-jets
      if(event.n_bjets!=2) return true;
      
      //c-jet on w_u
      if(event.cvsb_w_u<cvsb_wp_m_2018 || event.cvsl_w_u<cvsl_wp_m_2018) return true;
      
      //no more c-jet
      if(event.n_cjets!=1) return true;
    }
  //c-inversion
  else if(region_type=="Control1")
    {
      //permutation MVA
      if(event.best_mva_score<0.80) return true;

      //b-jet on w_d
      if(event.bvsc_w_d<bvsc_wp_m_2018) return true;

      //only 0 c-jet
      if(event.n_cjets==1) return true;
    }
  //permutation mva socre invsersion
  else if(region_type=="Control2")
    {
      //permutation MVA
      if(0.8<event.best_mva_score) return true;
  
      //b-jet on w_d
      if(event.bvsc_w_d<bvsc_wp_m_2018) return true;

      //c-jet on w_u
      if(event.cvsb_w_u<cvsb_wp_m_2018 || event.cvsl_w_u<cvsl_wp_m_2018) return true;

      //no more c-jet
      if(event.n_cjets!=1) return true;
    }
  
  
  return false;
}//bool Data_MC_Comparison::Cut()

//////////

void Data_MC_Comparison::Draw()
{
  cout << "Data_MC_Comparison::Draw" << endl;
  
  TLegend* tl[n_histo_conf];
  THStack* stack[n_histo_conf];
  for(int i=0; i<n_histo_conf; i++)
    {
      tl[i] = new TLegend(0.1, 0.6, 0.4, 0.9);
      
      TString stack_name = region_type + "_" + histo_conf[i].variable_title;
      cout << stack_name << endl;
      
      stack[i] = new THStack(stack_name, stack_name);
      
      for(int j=0; j<n_sample_merge_mc; j++)
	{
	  histo_mc[i][j]->SetFillColorAlpha(j+2, 0.10);
	  tl[i]->AddEntry(histo_mc[i][j], vec_short_name_mc[j], "f");
	  stack[i]->Add(histo_mc[i][j]);
	}
    }
  
  histo_ratio = new TH1D*[n_histo_conf];
  for(int i=0; i<n_histo_conf; i++)
    {
      TString histo_title = histo_conf[i].variable_title + "_Ratio";
      cout << histo_title << endl;
      histo_ratio[i] = new TH1D(histo_title, histo_title, histo_conf[i].n_bin, histo_conf[i].x_low, histo_conf[i].x_up);
      
      histo_ratio[i]->Divide(histo_data[i], (TH1D*)stack[i]->GetStack()->Last());
    }
  
  TCanvas* canvas[n_histo_conf];
  TPad* pad[n_histo_conf][2];
  for(int i=0; i<n_histo_conf; i++)
    {
      TString name = region_type + "_" + histo_conf[i].variable_title;
      canvas[i] = new TCanvas(name, name, 1200, 800);
      canvas[i]->Draw();
      
      pad[i][0] = new TPad(name, name, 0, 0.4, 1, 1);
      canvas[i]->cd();
      pad[i][0]->Draw();
      pad[i][0]->cd();

      float maximum = histo_data[i]->GetMaximum();
      histo_data[i]->SetMarkerStyle(21);
      histo_data[i]->GetYaxis()->SetRangeUser(0, 2*maximum);
      histo_data[i]->Draw("EP");

      stack[i]->Draw("SAME BAR");
      
      histo_data[i]->Draw("EP SAME");
      
      tl[i]->AddEntry(histo_data[i], data_short_name, "lep");
      tl[i]->Draw();

      TString pad_name = name + "_Ratio";
      pad[i][1] = new TPad(pad_name, pad_name, 0, 0, 1, 0.4);
      canvas[i]->cd();
      pad[i][1]->Draw();
      pad[i][1]->cd();
      histo_ratio[i]->GetYaxis()->SetRangeUser(0.5, 1.5);
      histo_ratio[i]->Draw();
      
      canvas[i]->Modified();
    }

  TString fout_name = region_type + ".root";
  TFile* fout = new TFile(fout_name, "RECREATE");
  for(int i=0; i<n_histo_conf; i++)
    {
      TString name = region_type + "_" + histo_conf[i].variable_title + ".png";
      canvas[i]->Print(name, "png");
      
      fout->cd();
      canvas[i]->Write();
    }
  fout->Close();

  return;
}//void Data_MC_Comparison::Draw()

//////////

void Data_MC_Comparison::Fill_Histo_Data()
{
  histo_data[0]->Fill(event.n_vertex, event.weight);
  histo_data[1]->Fill(event.lepton_pt, event.weight);
  histo_data[2]->Fill(event.lepton_eta, event.weight);
  histo_data[3]->Fill(event.n_jets, event.weight);
  histo_data[4]->Fill(event.n_bjets, event.weight);
  histo_data[5]->Fill(event.n_cjets, event.weight);
  histo_data[6]->Fill(event.pt_leading_jet, event.weight);
  histo_data[7]->Fill(event.pt_subleading_jet, event.weight);
  histo_data[8]->Fill(event.eta_leading_jet, event.weight);
  histo_data[9]->Fill(event.eta_subleading_jet, event.weight);
  histo_data[10]->Fill(event.met_pt, event.weight);
  histo_data[11]->Fill(event.met_phi, event.weight);
  histo_data[12]->Fill(event.best_mva_score, event.weight);

  return;
}//void Data_MC_Comparison::Fill_Histo_Data()

//////////

void Data_MC_Comparison::Fill_Histo_MC(const int& sample_index)
{
  histo_mc[0][sample_index]->Fill(event.n_vertex, event.weight);
  histo_mc[1][sample_index]->Fill(event.lepton_pt, event.weight);
  histo_mc[2][sample_index]->Fill(event.lepton_eta, event.weight);
  histo_mc[3][sample_index]->Fill(event.n_jets, event.weight);
  histo_mc[4][sample_index]->Fill(event.n_bjets, event.weight);
  histo_mc[5][sample_index]->Fill(event.n_cjets, event.weight);
  histo_mc[6][sample_index]->Fill(event.pt_leading_jet, event.weight);
  histo_mc[7][sample_index]->Fill(event.pt_subleading_jet, event.weight);
  histo_mc[8][sample_index]->Fill(event.eta_leading_jet, event.weight);
  histo_mc[9][sample_index]->Fill(event.eta_subleading_jet, event.weight);
  histo_mc[10][sample_index]->Fill(event.met_pt, event.weight);
  histo_mc[11][sample_index]->Fill(event.met_phi, event.weight);
  histo_mc[12][sample_index]->Fill(event.best_mva_score, event.weight);
  
  return;
}//void Data_MC_Comparison::Fill_Histo()

//////////

void Data_MC_Comparison::Read_Tree()
{
  cout << "Data_MC_Comparison::Read_Tree: Start to read trees" << endl;
  
  for(auto it=map_tree_mc.begin(); it!=map_tree_mc.end(); it++)
    {
      int sample_index =  find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[it->first]) - vec_short_name_mc.begin();
      cout << it->first << " Sample_Index = " << sample_index << endl;
      
      Long64_t n_entries = it->second->GetEntries();
      for(Long64_t i=0; i<n_entries; i++)
	{
	  if(i%5000000==0) cout << "Processing... " << i << "/" << n_entries << endl;
	 
	  it->second->GetEntry(i);
	  
	  event.Swap();
	  
	  if(Cut()) continue;
	  
	  Fill_Histo_MC(sample_index);

	  //if(20<i) break;
	}
    }//loop over mc
  
  for(auto it=map_tree_data.begin(); it!=map_tree_data.end(); it++)
    {
      cout << it->first << endl;
      
      Long64_t n_entries = it->second->GetEntries();
      for(Long64_t i=0; i<n_entries; i++)
        {
	  if(i%5000000==0) cout << "Processing... " << i << "/n_entries." << endl;
	  
          it->second->GetEntry(i);

          event.Swap();

          if(Cut()) continue;

          Fill_Histo_Data();

          //if(100<i) break;
        }
    }//loop over data period

  return;
}//void Data_MC_Comparison::Read_Tree()

//////////

