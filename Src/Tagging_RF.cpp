#include "Tagging_RF.h"

ClassImp(Tagging_RF);

//////////

Tagging_RF::Tagging_RF(const TString &a_era, const TString &a_mode, const TString &a_extension) : samples(a_era, "Mu")
{
  cout << "[Tagging_RF::Tagging_RF]: Init analysis" << endl;

  era = a_era;
  mode = a_mode;
  extension = a_extension;

  if (mode == "Analysis")
    Run_Analysis();
  else if (mode == "Application")
    Setup_Application();
  else
  {
    cout << "Wrong mode. Check it." << endl;
    exit(1);
  }

} // Tagging_RF::Tagging_RF(const TString& a_era, const TString& a_channel)

//////////

Tagging_RF::~Tagging_RF()
{
  if (mode == "Analysis")
  {
    fout->cd();
    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      histo_mc_before[i]->Write();

      for (int j = 0; j < n_syst_b; j++)
        histo_mc_after_b[i][j]->Write();
      for (int j = 0; j < n_syst_c; j++)
        histo_mc_after_c[i][j]->Write();
    }

    stack_mc_before->Write();
    for (int i = 0; i < n_syst_b; i++)
      stack_mc_after_b[i]->Write();
    for (int i = 0; i < n_syst_c; i++)
      stack_mc_after_c[i]->Write();

    for (int i = 0; i < n_syst_b; i++)
      ratio_b[i]->Write();
    for (int i = 0; i < n_syst_c; i++)
      ratio_c[i]->Write();

    canvas_before->Write();
    for (int i = 0; i < n_syst_b; i++)
      canvas_after_b[i]->Write();
    for (int i = 0; i < n_syst_c; i++)
      canvas_after_c[i]->Write();

    for (int i = 0; i < n_syst_b; i++)
      canvas_ratio_b[i]->Write();
    for (int i = 0; i < n_syst_c; i++)
      canvas_ratio_c[i]->Write();

    fout->Close();
  } // if (mode=="Analysis")
  else if (mode == "Application")
  {
    fin->Close();
  }
} // Tagging_RF::~Tagging_RF()

//////////

float Tagging_RF::Get_Tagging_RF(const TString &syst_name, const int &n_jet)
{
  if (syst_name.Contains("B_Tag"))
  {
    for (int i = 0; i < n_syst_b; i++)
    {
      if (syst_name == syst_name_b[i])
      {
        int bin = ratio_b[i]->FindBin(n_jet);
        return ratio_b[i]->GetBinContent(bin);
      }
    }
  }
  else if (syst_name.Contains("C_Tag"))
  {
    for (int i = 0; i < n_syst_c; i++)
    {
      if (syst_name == syst_name_c[i])
      {
        int bin = ratio_c[i]->FindBin(n_jet);
        return ratio_c[i]->GetBinContent(bin);
      }
    }
  }

  cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << syst_name << " Check it." << endl;
  exit(1);

  return -1;
} // float Tagging_RF::Get_Tagging_RF(const TString &syst_name, const int &n_jet)

//////////

void Tagging_RF::Run_Analysis()
{
  Setup_Analysis();
  Read_Tree();
  Stack_MC();
  Ratio();
  Draw();

  return;
} // void Tagging_RF::Run_Analysis()

//////////

void Tagging_RF::Draw()
{
  legend = new TLegend(0.65, 0.65, 0.85, 0.85);
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    legend->AddEntry(histo_mc_before[i], vec_short_name_mc[i], "f");
  }
  legend->SetBorderSize(0);

  TString canvas_name = "Canvas_Before";
  canvas_before = new TCanvas(canvas_name, canvas_name, 1400, 1000);
  canvas_before->Draw();
  stack_mc_before->Draw("BAR");
  legend->Draw("SAME");
  canvas_before->Print(canvas_name + "." + extension, extension);

  canvas_after_b = new TCanvas *[n_syst_b];
  for (int i = 0; i < n_syst_b; i++)
  {
    canvas_name = "Canvas_After_" + syst_name_b[i];
    canvas_after_b[i] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
    canvas_after_b[i]->Draw();
    stack_mc_after_b[i]->Draw("BAR");
    legend->Draw("SAME");
    canvas_after_b[i]->Print(canvas_name + "." + extension, extension);
  }

  canvas_after_c = new TCanvas *[n_syst_c];
  for (int i = 0; i < n_syst_c; i++)
  {
    canvas_name = "Canvas_After_" + syst_name_c[i];
    canvas_after_c[i] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
    canvas_after_c[i]->Draw();
    stack_mc_after_c[i]->Draw("BAR");
    legend->Draw("SAME");
    canvas_after_c[i]->Print(canvas_name + "." + extension, extension);
  }

  canvas_ratio_b = new TCanvas *[n_syst_b];
  for (int i = 0; i < n_syst_b; i++)
  {
    canvas_name = "Canvas_Ratio_" + syst_name_b[i];
    canvas_ratio_b[i] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
    canvas_ratio_b[i]->Draw();
    ratio_b[i]->Draw();
    canvas_ratio_b[i]->Print(canvas_name + "." + extension, extension);
  }

  canvas_ratio_c = new TCanvas *[n_syst_c];
  for (int i = 0; i < n_syst_c; i++)
  {
    canvas_name = "Canvas_Ratio_" + syst_name_c[i];
    canvas_ratio_c[i] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
    canvas_ratio_c[i]->Draw();
    ratio_c[i]->Draw();
    canvas_ratio_c[i]->Print(canvas_name + "." + extension, extension);
  }

  return;
} // void Tagging_RF::Draw()

//////////

void Tagging_RF::Fill_Histo_MC(const int &sample_index)
{
  float weight = 1;

  weight *= weight_lumi;
  weight *= weight_mc;
  weight *= weight_pileup;
  weight *= weight_prefire;
  weight *= weight_top_pt;
  weight *= sf_mu_trig;
  weight *= sf_mu_id;
  weight *= sf_mu_iso;

  histo_mc_before[sample_index]->Fill(n_jets, weight);
  histo_mc_after_b[sample_index][0]->Fill(n_jets, weight * weight_b_tag);
  histo_mc_after_b[sample_index][1]->Fill(n_jets, weight * weight_b_tag_hf_down);
  histo_mc_after_b[sample_index][2]->Fill(n_jets, weight * weight_b_tag_hf_up);
  histo_mc_after_b[sample_index][3]->Fill(n_jets, weight * weight_b_tag_jes_down);
  histo_mc_after_b[sample_index][4]->Fill(n_jets, weight * weight_b_tag_jes_up);
  histo_mc_after_b[sample_index][5]->Fill(n_jets, weight * weight_b_tag_lfstats1_down);
  histo_mc_after_b[sample_index][6]->Fill(n_jets, weight * weight_b_tag_lfstats1_up);
  histo_mc_after_b[sample_index][7]->Fill(n_jets, weight * weight_b_tag_lfstats2_down);
  histo_mc_after_b[sample_index][8]->Fill(n_jets, weight * weight_b_tag_lfstats2_up);
  histo_mc_after_b[sample_index][9]->Fill(n_jets, weight * weight_b_tag_cferr1_down);
  histo_mc_after_b[sample_index][10]->Fill(n_jets, weight * weight_b_tag_cferr1_up);
  histo_mc_after_b[sample_index][11]->Fill(n_jets, weight * weight_b_tag_cferr2_down);
  histo_mc_after_b[sample_index][12]->Fill(n_jets, weight * weight_b_tag_cferr2_up);
  histo_mc_after_b[sample_index][13]->Fill(n_jets, weight * weight_b_tag_hfstats1_down);
  histo_mc_after_b[sample_index][14]->Fill(n_jets, weight * weight_b_tag_hfstats1_up);
  histo_mc_after_b[sample_index][15]->Fill(n_jets, weight * weight_b_tag_hfstats2_down);
  histo_mc_after_b[sample_index][16]->Fill(n_jets, weight * weight_b_tag_hfstats2_up);

  histo_mc_after_c[sample_index][0]->Fill(n_jets, weight * weight_c_tag);
  histo_mc_after_c[sample_index][1]->Fill(n_jets, weight * weight_c_tag_extrap_down);
  histo_mc_after_c[sample_index][2]->Fill(n_jets, weight * weight_c_tag_extrap_up);
  histo_mc_after_c[sample_index][3]->Fill(n_jets, weight * weight_c_tag_interp_down);
  histo_mc_after_c[sample_index][4]->Fill(n_jets, weight * weight_c_tag_interp_up);
  histo_mc_after_c[sample_index][5]->Fill(n_jets, weight * weight_c_tag_lhe_scale_muf_down);
  histo_mc_after_c[sample_index][6]->Fill(n_jets, weight * weight_c_tag_lhe_scale_muf_up);
  histo_mc_after_c[sample_index][7]->Fill(n_jets, weight * weight_c_tag_lhe_scale_mur_down);
  histo_mc_after_c[sample_index][8]->Fill(n_jets, weight * weight_c_tag_lhe_scale_mur_up);
  histo_mc_after_c[sample_index][9]->Fill(n_jets, weight * weight_c_tag_ps_fsr_fixed_down);
  histo_mc_after_c[sample_index][10]->Fill(n_jets, weight * weight_c_tag_ps_fsr_fixed_up);
  histo_mc_after_c[sample_index][11]->Fill(n_jets, weight * weight_c_tag_ps_isr_fixed_down);
  histo_mc_after_c[sample_index][12]->Fill(n_jets, weight * weight_c_tag_ps_isr_fixed_up);
  histo_mc_after_c[sample_index][13]->Fill(n_jets, weight * weight_c_tag_pu_down);
  histo_mc_after_c[sample_index][14]->Fill(n_jets, weight * weight_c_tag_pu_up);
  histo_mc_after_c[sample_index][15]->Fill(n_jets, weight * weight_c_tag_stat_down);
  histo_mc_after_c[sample_index][16]->Fill(n_jets, weight * weight_c_tag_stat_up);
  histo_mc_after_c[sample_index][17]->Fill(n_jets, weight * weight_c_tag_xsec_br_unc_dyjets_b_down);
  histo_mc_after_c[sample_index][18]->Fill(n_jets, weight * weight_c_tag_xsec_br_unc_dyjets_b_up);
  histo_mc_after_c[sample_index][19]->Fill(n_jets, weight * weight_c_tag_xsec_br_unc_dyjets_c_down);
  histo_mc_after_c[sample_index][20]->Fill(n_jets, weight * weight_c_tag_xsec_br_unc_dyjets_c_up);
  histo_mc_after_c[sample_index][21]->Fill(n_jets, weight * weight_c_tag_xsec_br_unc_wjets_c_down);
  histo_mc_after_c[sample_index][22]->Fill(n_jets, weight * weight_c_tag_xsec_br_unc_wjets_c_up);
  histo_mc_after_c[sample_index][23]->Fill(n_jets, weight * weight_c_tag_jer_down);
  histo_mc_after_c[sample_index][24]->Fill(n_jets, weight * weight_c_tag_jer_up);
  histo_mc_after_c[sample_index][25]->Fill(n_jets, weight * weight_c_tag_jes_total_down);
  histo_mc_after_c[sample_index][26]->Fill(n_jets, weight * weight_c_tag_jes_total_up);

  return;
} // void Tagging_RF::Fill_Histo_MC()

//////////

void Tagging_RF::Ratio()
{
  cout << "[Tagging_RF::Ratio] Init" << endl;

  ratio_b = new TH1D *[n_syst_b];
  for (int i = 0; i < n_syst_b; i++)
  {
    TString ratio_name = "Ratio_" + syst_name_b[i];
    ratio_b[i] = new TH1D(ratio_name, ratio_name, n_bin, 0, n_bin);

    ratio_b[i]->Divide((TH1D *)(stack_mc_before->GetStack()->Last()), (TH1D *)(stack_mc_after_b[i]->GetStack()->Last()));
  }

  ratio_c = new TH1D *[n_syst_c];
  for (int i = 0; i < n_syst_c; i++)
  {
    TString ratio_name = "Ratio_" + syst_name_c[i];
    ratio_c[i] = new TH1D(ratio_name, ratio_name, n_bin, 0, n_bin);

    ratio_c[i]->Divide((TH1D *)(stack_mc_before->GetStack()->Last()), (TH1D *)(stack_mc_after_c[i]->GetStack()->Last()));
  }

  return;
} // void Tagging_RF::Ratio()

//////////

void Tagging_RF::Read_Tree()
{
  cout << "[Tagging_RF::Read_Tree]: Starts to read trees" << endl;

  // for MC
  for (auto it = map_tree_mc.begin(); it != map_tree_mc.end(); it++)
  {
    TString fin_name = it->first;
    cout << fin_name << endl;

    int sample_index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[it->first]) - vec_short_name_mc.begin();
    cout << it->first << " Sample_Index = " << sample_index << endl;

    Long64_t n_entries = it->second->GetEntries();
    n_entries /= reduction;
    cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

    for (Long64_t i = 0; i < n_entries; i++)
    {
      if (i % 500000 == 0)
        cout << "Processing... " << i << "/" << n_entries << endl;

      it->second->GetEntry(i);

      Fill_Histo_MC(sample_index);
    } // loop over entries
  }   // loop over map_mc

  // for data

  return;
} // void Tagging_RF::Read_Tree()

//////////

void Tagging_RF::Setup_Analysis()
{
  cout << "[Tagging_RF::Setup_Analysis]: Init analysis" << endl;

  reduction = 1;
  n_bin = 20;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/Tagging_RF/";

  syst_name_b = {"B_Tag_Nominal",
                 "B_Tag_HF_Down", "B_Tag_HF_Up",
                 "B_Tag_JES_Down", "B_Tag_JES_Up",
                 "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up",
                 "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up",
                 "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up",
                 "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up",
                 "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up",
                 "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up"};
  n_syst_b = syst_name_b.size();

  syst_name_c = {"C_Tag_Nominal",
                 "C_Tag_Extrap_Down", "C_Tag_Extrap_Up",
                 "C_Tag_Interp_Down", "C_Tag_Interp_Up",
                 "C_Tag_LHE_Scale_MuF_Down", "C_Tag_LHE_Scale_MuF_Up",
                 "C_Tag_LHE_Scale_MuR_Down", "C_Tag_LHE_Scale_MuR_Up",
                 "C_Tag_PS_FSR_Fixed_Down", "C_Tag_PS_FSR_Fixed_Up",
                 "C_Tag_PS_ISR_Fixed_Down", "C_Tag_PS_ISR_Fixed_Up",
                 "C_Tag_PU_Down", "C_Tag_PU_Up",
                 "C_Tag_Stat_Down", "C_Tag_Stat_Up",
                 "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up",
                 "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up",
                 "C_Tag_XSec_Br_Unc_WJets_C_Down", "C_Tag_XSec_Br_Unc_WJets_C_Up",
                 "C_Tag_JER_Down", "C_Tag_JER_Up",
                 "C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up"};
  n_syst_c = syst_name_c.size();

  if (samples.map_mc_tagging_rf.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }
  n_sample = samples.map_mc_tagging_rf.size();

  for (auto it = samples.map_mc_tagging_rf.begin(); it != samples.map_mc_tagging_rf.end(); it++)
  {
    cout << it->first << endl;

    map_fin_mc[it->first] = new TFile(path_base + it->second);
    map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get("Result_Tree");

    Setup_Tree(map_tree_mc[it->first]);
  }

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    // cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  // cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  Setup_Histo();

  TString fout_name = "Vcb_Tagging_RF_" + era + ".root";
  fout = new TFile(fout_name, "RECREATE");
  return;
} // void Tagging_RF::Setup_Analysis()

//////////

void Tagging_RF::Setup_Application()
{
  cout << "[Tagging_RF::Setup_Application]" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  fin = new TFile(path_base + "/Corrections/Vcb_Tagging_RF_" + era + ".root");

  TList *list = fin->GetListOfKeys();

  n_syst_b = 0;
  n_syst_c = 0;

  int n_entries = list->GetEntries();
  for (int i = 0; i < n_entries; i++)
  {
    TObject *obj = list->At(i);
    TString name = obj->GetName();

    if (name.Contains("Ratio_B_Tag_"))
      syst_name_b.push_back(name);
    else if (name.Contains("Ratio_C_Tag_"))
      syst_name_c.push_back(name);
  }
  n_syst_b = syst_name_b.size();
  n_syst_c = syst_name_c.size();

  cout << "n_syst_b = " << n_syst_b << ", n_syst_c = " << n_syst_c << endl;

  ratio_b = new TH1D *[n_syst_b];
  for (int i = 0; i < n_syst_b; i++)
    ratio_b[i] = (TH1D *)fin->Get(syst_name_b[i]);

  ratio_c = new TH1D *[n_syst_c];
  for (int i = 0; i < n_syst_c; i++)
    ratio_c[i] = (TH1D *)fin->Get(syst_name_c[i]);

  return;
} // void Tagging_RF::Setup_Application()

//////////

void Tagging_RF::Setup_Histo()
{
  histo_mc_before = new TH1D *[n_sample_merge_mc];
  histo_mc_after_b = new TH1D **[n_sample_merge_mc];
  histo_mc_after_c = new TH1D **[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString histo_name = vec_short_name_mc[i] + "_Before";
    histo_mc_before[i] = new TH1D(histo_name, histo_name, n_bin, 0, n_bin);

    histo_mc_after_b[i] = new TH1D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      histo_name = vec_short_name_mc[i] + "_After_" + syst_name_b[j];
      histo_mc_after_b[i][j] = new TH1D(histo_name, histo_name, n_bin, 0, n_bin);
    }

    histo_mc_after_c[i] = new TH1D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      histo_name = vec_short_name_mc[i] + "_After_" + syst_name_c[j];
      histo_mc_after_c[i][j] = new TH1D(histo_name, histo_name, n_bin, 0, n_bin);
    }
  }

  return;
} // void Tagging_RF::Setup_Histo()

//////////

void Tagging_RF::Setup_Tree(TTree *tree)
{
  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("sf_mu_id", &sf_mu_id);
  tree->SetBranchAddress("sf_mu_iso", &sf_mu_iso);
  tree->SetBranchAddress("sf_mu_trig", &sf_mu_trig);
  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);
  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);
  tree->SetBranchAddress("weight_pujet_veto", &weight_pujet_veto);

  tree->SetBranchAddress("weight_b_tag", &weight_b_tag);
  tree->SetBranchAddress("weight_b_tag_down_hf", &weight_b_tag_hf_down);
  tree->SetBranchAddress("weight_b_tag_up_hf", &weight_b_tag_hf_up);
  tree->SetBranchAddress("weight_b_tag_down_jes", &weight_b_tag_jes_down);
  tree->SetBranchAddress("weight_b_tag_up_jes", &weight_b_tag_jes_up);
  tree->SetBranchAddress("weight_b_tag_down_lfstats1", &weight_b_tag_lfstats1_down);
  tree->SetBranchAddress("weight_b_tag_up_lfstats1", &weight_b_tag_lfstats1_up);
  tree->SetBranchAddress("weight_b_tag_down_lfstats2", &weight_b_tag_lfstats2_down);
  tree->SetBranchAddress("weight_b_tag_up_lfstats2", &weight_b_tag_lfstats2_up);
  tree->SetBranchAddress("weight_b_tag_down_cferr1", &weight_b_tag_cferr1_down);
  tree->SetBranchAddress("weight_b_tag_up_cferr1", &weight_b_tag_cferr1_up);
  tree->SetBranchAddress("weight_b_tag_down_cferr2", &weight_b_tag_cferr2_down);
  tree->SetBranchAddress("weight_b_tag_up_cferr2", &weight_b_tag_cferr2_up);
  tree->SetBranchAddress("weight_b_tag_down_hfstats1", &weight_b_tag_hfstats1_down);
  tree->SetBranchAddress("weight_b_tag_up_hfstats1", &weight_b_tag_hfstats1_up);
  tree->SetBranchAddress("weight_b_tag_down_hfstats2", &weight_b_tag_hfstats2_down);
  tree->SetBranchAddress("weight_b_tag_up_hfstats2", &weight_b_tag_hfstats2_up);

  tree->SetBranchAddress("weight_c_tag", &weight_c_tag);
  tree->SetBranchAddress("weight_c_tag_down_extrap", &weight_c_tag_extrap_down);
  tree->SetBranchAddress("weight_c_tag_up_extrap", &weight_c_tag_extrap_up);
  tree->SetBranchAddress("weight_c_tag_down_interp", &weight_c_tag_interp_down);
  tree->SetBranchAddress("weight_c_tag_up_interp", &weight_c_tag_interp_up);
  tree->SetBranchAddress("weight_c_tag_down_lhe_scale_muf", &weight_c_tag_lhe_scale_muf_down);
  tree->SetBranchAddress("weight_c_tag_up_lhe_scale_muf", &weight_c_tag_lhe_scale_muf_up);
  tree->SetBranchAddress("weight_c_tag_down_lhe_scale_mur", &weight_c_tag_lhe_scale_mur_down);
  tree->SetBranchAddress("weight_c_tag_up_lhe_scale_mur", &weight_c_tag_lhe_scale_mur_up);
  tree->SetBranchAddress("weight_c_tag_down_ps_fsr_fixed", &weight_c_tag_ps_fsr_fixed_down);
  tree->SetBranchAddress("weight_c_tag_up_ps_fsr_fixed", &weight_c_tag_ps_fsr_fixed_up);
  tree->SetBranchAddress("weight_c_tag_down_ps_isr_fixed", &weight_c_tag_ps_isr_fixed_down);
  tree->SetBranchAddress("weight_c_tag_up_ps_isr_fixed", &weight_c_tag_ps_isr_fixed_up);
  tree->SetBranchAddress("weight_c_tag_down_pu", &weight_c_tag_pu_down);
  tree->SetBranchAddress("weight_c_tag_up_pu", &weight_c_tag_pu_up);
  tree->SetBranchAddress("weight_c_tag_down_stat", &weight_c_tag_stat_down);
  tree->SetBranchAddress("weight_c_tag_up_stat", &weight_c_tag_stat_up);
  tree->SetBranchAddress("weight_c_tag_down_xsec_brunc_dyjets_b", &weight_c_tag_xsec_br_unc_dyjets_b_down);
  tree->SetBranchAddress("weight_c_tag_up_xsec_brunc_dyjets_b", &weight_c_tag_xsec_br_unc_dyjets_b_up);
  tree->SetBranchAddress("weight_c_tag_down_xsec_brunc_dyjets_c", &weight_c_tag_xsec_br_unc_dyjets_c_down);
  tree->SetBranchAddress("weight_c_tag_up_xsec_brunc_dyjets_c", &weight_c_tag_xsec_br_unc_dyjets_c_up);
  tree->SetBranchAddress("weight_c_tag_down_xsec_brunc_wjets_c", &weight_c_tag_xsec_br_unc_wjets_c_down);
  tree->SetBranchAddress("weight_c_tag_up_xsec_brunc_wjets_c", &weight_c_tag_xsec_br_unc_wjets_c_up);
  tree->SetBranchAddress("weight_c_tag_down_jer", &weight_c_tag_jer_down);
  tree->SetBranchAddress("weight_c_tag_up_jer", &weight_c_tag_jer_up);
  tree->SetBranchAddress("weight_c_tag_down_jes_total", &weight_c_tag_jes_total_down);
  tree->SetBranchAddress("weight_c_tag_up_jes_total", &weight_c_tag_jes_total_up);

  return;
} // void Tagging_RF::Setup_Tree()

//////////

void Tagging_RF::Stack_MC()
{
  cout << "[Vcb_Tagging_RF::Stack_MC] Init" << endl;

  stack_mc_before = new THStack("Stack_Before", "Stack_Before");

  stack_mc_after_b = new THStack *[n_syst_b];
  for (int i = 0; i < n_syst_b; i++)
  {
    TString stack_name = "Stack_After_" + syst_name_b[i];
    stack_mc_after_b[i] = new THStack(stack_name, stack_name);
  }

  stack_mc_after_c = new THStack *[n_syst_c];
  for (int i = 0; i < n_syst_c; i++)
  {
    TString stack_name = "Stack_After_" + syst_name_c[i];
    stack_mc_after_c[i] = new THStack(stack_name, stack_name);
  }

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    histo_mc_before[i]->SetFillColorAlpha(color[i], 0.2);
    stack_mc_before->Add(histo_mc_before[i]);

    for (int j = 0; j < n_syst_b; j++)
    {
      histo_mc_after_b[i][j]->SetFillColorAlpha(color[i], 0.2);
      stack_mc_after_b[j]->Add(histo_mc_after_b[i][j]);
    }

    for (int j = 0; j < n_syst_c; j++)
    {
      histo_mc_after_c[i][j]->SetFillColorAlpha(color[i], 0.2);
      stack_mc_after_c[j]->Add(histo_mc_after_c[i][j]);
    }
  }

  cout << "[Vcb_Tagging_RF::Stack_MC]: Done" << endl;

  return;
} // void Vcb_Tagging_RF::Stack_MC()

//////////