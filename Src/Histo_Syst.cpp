#include "Histo_Syst.h"

ClassImp(Histo_Syst);

//////////

Histo_Syst::Histo_Syst(const TString &a_era, const TString &a_channel, const TString &a_swap_mode) : samples(a_era, a_channel), event(a_era, a_channel, a_swap_mode)
{
  cout << "[Histo_Syst::Histo_Syst]: Init analysis" << endl;

  reduction = 10;

  era = a_era;
  channel = a_channel;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunResult/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    // central and syst weights
    map_fin_mc[it->first] = new TFile(path_base + "Central_Syst/" + it->second);
    map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get("POGTightWithTightIso_Central/Result_Tree");

    event.Setup_Tree(map_tree_mc[it->first], true);

    //   // jec down
    //   map_fin_mc_jec_down[it->first] = new TFile(path_base + "JetEnDown/" + it->second);
    //   map_tree_mc_jec_down[it->first] = (TTree *)map_fin_mc_jec_down[it->first]->Get("POGTightWithTightIso_JetEnDown/Result_Tree");

    //   event.Setup_Tree(map_tree_mc_jec_down[it->first], false);

    //   // jec up
    //   map_fin_mc_jec_up[it->first] = new TFile(path_base + "JetEnUp/" + it->second);
    //   map_tree_mc_jec_up[it->first] = (TTree *)map_fin_mc_jec_up[it->first]->Get("POGTightWithTightIso_JetEnUp/Result_Tree");

    //   event.Setup_Tree(map_tree_mc_jec_up[it->first], false);

    //   // jes down
    //   map_fin_mc_jes_down[it->first] = new TFile(path_base + "JesDown/" + it->second);
    //   map_tree_mc_jes_down[it->first] = (TTree *)map_fin_mc_jes_down[it->first]->Get("POGTightWithTightIso_JetResDown/Result_Tree");

    //   event.Setup_Tree(map_tree_mc_jes_down[it->first], false);

    //   // jes up
    //   map_fin_mc_jes_up[it->first] = new TFile(path_base + "JesUp/" + it->second);
    //   map_tree_mc_jes_up[it->first] = (TTree *)map_fin_mc_jes_up[it->first]->Get("POGTightWithTightIso_JetResUp/Result_Tree");

    //   event.Setup_Tree(map_tree_mc_jes_up[it->first], false);
  }

  // syst trees
  map_map_fin_mc["Central"] = &map_fin_mc;
  map_map_tree_mc["Central"] = &map_tree_mc;

  // map_map_fin_mc["JesDown"] = &map_fin_mc_jes_down;
  // map_map_tree_mc["JesDown"] = &map_tree_mc_jes_down;

  // map_map_fin_mc["JesUp"] = &map_fin_mc_jes_up;
  // map_map_tree_mc["JesUp"] = &map_tree_mc_jes_up;

  // map_map_fin_mc["JetEnDown"] = &map_fin_mc_jec_down;
  // map_map_tree_mc["JetEnDown"] = &map_tree_mc_jec_down;

  // map_map_fin_mc["JetEnUp"] = &map_fin_mc_jec_up;
  // map_map_tree_mc["JetEnUp"] = &map_tree_mc_jec_up;

  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  path_base += "DATA/";
  for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
  {
    cout << it->first << endl;
    if (it == samples.map_data.begin())
      data_short_name = samples.map_short_name_data[it->first];

    map_fin_data[it->first] = new TFile(path_base + it->second);
    map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get("POGTightWithTightIso_Central/Result_Tree");

    event.Setup_Tree(map_tree_data[it->first], false);
  }

  // Region
  region_name = {"Control0"};
  n_region = region_name.size();

  // Systs
  syst_name = {"Nominal",
               "B_Tag_HF_Down", "B_Tag_HF_Up", "B_Tag_JES_Down", "B_Tag_JES_Up", "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up", "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up", "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up", "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up", "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up", "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up",
               "C_Tag_Extrap_Down", "C_Tag_Extrap_Up", "C_Tag_Interp_Down", "C_Tag_Interp_Up", "C_Tag_LHE_Scale_MuF_Down", "C_Tag_LHE_Scale_MuF_Up", "C_Tag_LHE_Scale_MuR_Down", "C_Tag_LHE_Scale_MuR_Up", "C_Tag_PS_FSR_Fixed_Down", "C_Tag_PS_FSR_Fixed_Up", "C_Tag_PS_ISR_Fixed_Down", "C_Tag_PS_ISR_Fixed_Up", "C_Tag_PU_Down", "C_Tag_PU_Up", "C_Tag_Stat_Down", "C_Tag_Stat_Up",
               "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up", "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up", "C_Tag_XSec_Br_Unc_WJets_C_Down", "C_Tag_XSec_Br_Unc_WJets_C_Up",
               "C_Tag_JER_Down", "C_Tag_JER_Up", "C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up",
               "JesDown", "JesUp",
               "JetEnDown", "JetEnUp",
               "Mu_Id_Down", "Mu_Id_Up",
               "Mu_Iso_Down", "Mu_Iso_Up",
               "Mu_Trig_Down", "Mu_Trig_Up",
               "PDF_Alternative", "PDF_As_Down", "PDF_As_Up",
               "Pileup_Down", "Pileup_Up",
               "Pileup_Veto_Down", "Pileup_Veto_Up",
               "Prefire_Down", "Prefire_Up",
               "Scale_Variation_1", "Scale_Variation_2", "Scale_Variation_3", "Scale_Variation_4", "Scale_Variation_6", "Scale_Variation_8",
               "Top_Pt_Reweight"};
  for (int i = 0; i < sizeof(event.weight_pdf_error_set) / sizeof(float); i++)
  {
    TString pdf_error_set = "PDF_Error_Set_" + to_string(i);
    syst_name.push_back(pdf_error_set);
  }

  n_syst = syst_name.size();
  cout << "N_Syst = " << n_syst << endl;

  variable_conf = {{"N_Vertex", 80, 0, 80},
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
                   {"Best_MVA_Score", 100, -1, 1}};
  n_variable = variable_conf.size();

  histo_mc = new TH1D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc[i] = new TH1D ***[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      histo_mc[i][j] = new TH1D **[n_sample_merge_mc];
      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        histo_mc[i][j][k] = new TH1D *[n_variable];
        for (int l = 0; l < n_variable; l++)
        {
          TString histo_name = region_name[i] + "_" + syst_name[j] + "_" + vec_short_name_mc[k] + "_" + variable_conf[l].variable_title;

          histo_mc[i][j][k][l] = new TH1D(histo_name, variable_conf[l].variable_title, variable_conf[l].n_bin, variable_conf[l].x_low, variable_conf[l].x_up);
        } // loop over n_variable
      }   // loop over n_sample_merge_mc
    }     // loop over n_syst
  }       // loop over n_regions

  histo_data = new TH1D **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_data[i] = new TH1D *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = region_name[i] + "_" + variable_conf[j].variable_title;
      histo_data[i][j] = new TH1D(histo_name, variable_conf[j].variable_title, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up);
    } // loop over n_variable
  }   // loop over n_region

  TString fout_name = "Vcb_Histos_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "RECREATE");
} // Histo_Syst::Histo_Syst(const TString& a_era, const TString& a_channel, const TString& a_swap_mode)

//////////

Histo_Syst::~Histo_Syst()
{
  // Save histos
  for (int i = 0; i < n_region; i++)
  {
    TDirectory *dir_region = fout->mkdir(region_name[i]);

    // for Data
    TDirectory *dir_data = dir_region->mkdir("Data");
    for (int j = 0; j < n_variable; j++)
    {
      dir_data->cd();
      histo_data[i][j]->SetTitle(variable_conf[j].variable_title);
      histo_data[i][j]->SetName(variable_conf[j].variable_title);
      histo_data[i][j]->Write();
    } // loop over variable

    // for MC
    for (int j = 0; j < n_syst; j++)
    {
      TDirectory *dir_syst = dir_region->mkdir(syst_name[j]);

      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[k]);

        for (int l = 0; l < n_variable; l++)
        {
          dir_sample->cd();
          histo_mc[i][j][k][l]->SetTitle(variable_conf[l].variable_title);
          histo_mc[i][j][k][l]->SetName(variable_conf[l].variable_title);
          histo_mc[i][j][k][l]->Write();
        } // loop over variable
      }   // loop over sample
    }     // loop over syst
  }       // loop over region

  fout->Close();
} // Histo_Syst::~Histo_Syst()

//////////

void Histo_Syst::Run()
{
  Read_Tree();

  return;
} // void Histo_Syst::Run()

//////////

void Histo_Syst::Fill_Histo_Data(const int &region_index)
{
  // this part is not satisfactory, but don't waste time
  histo_data[region_index][0]->Fill(event.n_vertex, 1.);
  histo_data[region_index][1]->Fill(event.lepton_pt, 1.);
  histo_data[region_index][2]->Fill(event.lepton_eta, 1.);
  histo_data[region_index][3]->Fill(event.n_jets, 1.);
  histo_data[region_index][4]->Fill(event.n_bjets, 1.);
  histo_data[region_index][5]->Fill(event.n_cjets, 1.);
  histo_data[region_index][6]->Fill(event.pt_leading_jet, 1.);
  histo_data[region_index][7]->Fill(event.pt_subleading_jet, 1.);
  histo_data[region_index][8]->Fill(event.eta_leading_jet, 1.);
  histo_data[region_index][9]->Fill(event.eta_subleading_jet, 1.);
  histo_data[region_index][10]->Fill(event.met_pt, 1.);
  histo_data[region_index][11]->Fill(event.met_phi, 1.);
  histo_data[region_index][12]->Fill(event.best_mva_score, 1.);

  return;
} // void Histo_Syst::Fill_Histo_Data()

//////////

void Histo_Syst::Fill_Histo_MC(const int &region_index, const int &sample_index, const Long64_t &entry, const TString &syst_fix)
{
  for (int i = 0; i < n_syst; i++)
  {
    TString syst_type = syst_name[i];

    // for central tree, don't consider the following syst
    if (syst_fix == "None" &&
        (syst_type == "JesDown" || syst_type == "JesUp" ||
         syst_type == "JetEnDown" || syst_type == "JetEnUp"))
      continue;

    // if syst_fix is fixed, only nominal is considered
    if (syst_fix != "None" && syst_fix != syst_type)
      continue;

    event.weight = 1;
    event.weight *= event.weight_lumi;
    event.weight *= event.weight_mc;

    if (syst_type == "PDF_Alternative")
      event.weight *= event.weight_pdf_alternative;
    else if (syst_type.Contains("PDF_Error_Set_"))
    {
      // TObjArray *arr = syst_type.Tokenize('_');
      // TString index = ((TObjString *)arr->At(3))->CopyString();
      // event.weight *= event.weight_pdf_error_set[index.Atoi()];

      // arr->Delete();
      // it caused memory leak

      TString temp = syst_type;

      int first = temp.First('_');
      temp = temp.Remove(0, first + 1);

      int second = temp.First('_');
      temp = temp.Remove(0, second + 1);

      int third = temp.First('_');
      TString index = temp.Remove(0, third + 1);

      event.weight *= event.weight_pdf_error_set[index.Atoi()];
    }
    else if (syst_type == "PDF_As_Down")
      event.weight *= event.weight_pdf_as_down;
    else if (syst_type == "PDF_As_Up")
      event.weight *= event.weight_pdf_as_up;

    if (syst_type == "Pileup_Down")
      event.weight *= event.weight_pileup_down;
    else if (syst_type == "Pileup_Up")
      event.weight *= event.weight_pileup_up;
    else
      event.weight *= event.weight_pileup;

    if (syst_type == "Prefire_Down")
      event.weight *= event.weight_prefire_down;
    else if (syst_type == "Prefire_Up")
      event.weight *= event.weight_prefire_up;
    else
      event.weight *= event.weight_prefire;

    if (syst_type == "Top_Pt_Reweight")
      event.weight *= 1;
    else
      event.weight *= event.weight_top_pt;

    if (syst_type == "Mu_Trig_Down")
      event.weight *= event.sf_mu_trig_down;
    else if (syst_type == "Mu_Trig_Up")
      event.weight *= event.sf_mu_trig_up;
    else
      event.weight *= event.sf_mu_trig;

    if (syst_type == "Mu_Id_Down")
      event.weight *= event.sf_mu_id_down;
    else if (syst_type == "Mu_Id_Up")
      event.weight *= event.sf_mu_id_up;
    else
      event.weight *= event.sf_mu_id;

    if (syst_type == "Mu_Iso_Down")
      event.weight *= event.sf_mu_iso_down;
    else if (syst_type == "Mu_Iso_Up")
      event.weight *= event.sf_mu_iso_up;
    else
      event.weight *= event.sf_mu_iso;

    // event.weight *= event.weight_pujet_veto;

    if (syst_type == "B_Tag_HF_Down")
      event.weight *= event.weight_b_tag_hf_down;
    else if (syst_type == "B_Tag_HF_Up")
      event.weight *= event.weight_b_tag_hf_up;
    else if (syst_type == "B_Tag_JES_Down")
      event.weight *= event.weight_b_tag_jes_down;
    else if (syst_type == "B_Tag_JES_Up")
      event.weight *= event.weight_b_tag_jes_up;
    else if (syst_type == "B_Tag_LFStats1_Down")
      event.weight *= event.weight_b_tag_lfstats1_down;
    else if (syst_type == "B_Tag_LFStats1_Up")
      event.weight *= event.weight_b_tag_lfstats1_up;
    else if (syst_type == "B_Tag_LFStats2_Down")
      event.weight *= event.weight_b_tag_lfstats2_down;
    else if (syst_type == "B_Tag_LFStats2_Up")
      event.weight *= event.weight_b_tag_lfstats2_up;
    else if (syst_type == "B_Tag_CFErr1_Down")
      event.weight *= event.weight_b_tag_cferr1_down;
    else if (syst_type == "B_Tag_CFErr1_Up")
      event.weight *= event.weight_b_tag_cferr1_up;
    else if (syst_type == "B_Tag_CFErr2_Down")
      event.weight *= event.weight_b_tag_cferr2_down;
    else if (syst_type == "B_Tag_CFErr2_Up")
      event.weight *= event.weight_b_tag_cferr2_up;
    else if (syst_type == "B_Tag_HFStats1_Down")
      event.weight *= event.weight_b_tag_hfstats1_down;
    else if (syst_type == "B_Tag_HFStats1_Up")
      event.weight *= event.weight_b_tag_hfstats1_up;
    else if (syst_type == "B_Tag_HFStats2_Down")
      event.weight *= event.weight_b_tag_hfstats2_down;
    else if (syst_type == "B_Tag_HFStats2_Up")
      event.weight *= event.weight_b_tag_hfstats2_up;
    else
      event.weight *= event.weight_b_tag;
    // event.weight *= 1;

    if (syst_type == "C_Tag_Extrap_Down")
      event.weight *= event.weight_c_tag_extrap_down;
    else if (syst_type == "C_Tag_Extrap_Up")
      event.weight *= event.weight_c_tag_extrap_up;
    else if (syst_type == "C_Tag_Interp_Down")
      event.weight *= event.weight_c_tag_interp_down;
    else if (syst_type == "C_Tag_Interp_Up")
      event.weight *= event.weight_c_tag_interp_up;
    else if (syst_type == "C_Tag_LHE_Scale_MuF_Down")
      event.weight *= event.weight_c_tag_lhe_scale_muf_down;
    else if (syst_type == "C_Tag_LHE_Scale_MuF_Up")
      event.weight *= event.weight_c_tag_lhe_scale_muf_up;
    else if (syst_type == "C_Tag_LHE_Scale_MuR_Down")
      event.weight *= event.weight_c_tag_lhe_scale_mur_down;
    else if (syst_type == "C_Tag_LHE_Scale_MuR_Up")
      event.weight *= event.weight_c_tag_lhe_scale_mur_up;
    else if (syst_type == "C_Tag_PS_FSR_Fixed_Down")
      event.weight *= event.weight_c_tag_ps_fsr_fixed_down;
    else if (syst_type == "C_Tag_PS_FSR_Fixed_Up")
      event.weight *= event.weight_c_tag_ps_fsr_fixed_up;
    else if (syst_type == "C_Tag_PS_ISR_Fixed_Down")
      event.weight *= event.weight_c_tag_ps_isr_fixed_down;
    else if (syst_type == "C_Tag_PS_ISR_Fixed_Up")
      event.weight *= event.weight_c_tag_ps_isr_fixed_up;
    else if (syst_type == "C_Tag_PU_Down")
      event.weight *= event.weight_c_tag_pu_down;
    else if (syst_type == "C_Tag_PU_Up")
      event.weight *= event.weight_c_tag_pu_up;
    else if (syst_type == "C_Tag_Stat_Down")
      event.weight *= event.weight_c_tag_stat_down;
    else if (syst_type == "C_Tag_Stat_Up")
      event.weight *= event.weight_c_tag_stat_up;
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_B_Down")
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_b_down;
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_B_Up")
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_b_up;
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_C_Down")
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_c_down;
    else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_C_Up")
      event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_c_up;
    else if (syst_type == "C_Tag_XSec_Br_Unc_WJets_C_Down")
      event.weight *= event.weight_c_tag_xsec_br_unc_wjets_c_down;
    else if (syst_type == "C_Tag_XSec_Br_Unc_WJets_C_Up")
      event.weight *= event.weight_c_tag_xsec_br_unc_wjets_c_up;
    else if (syst_type == "C_Tag_JER_Down")
      event.weight *= event.weight_c_tag_jer_down;
    else if (syst_type == "C_Tag_JER_Up")
      event.weight *= event.weight_c_tag_jer_up;
    else if (syst_type == "C_Tag_JES_Total_Down")
      event.weight *= event.weight_c_tag_jes_total_down;
    else if (syst_type == "C_Tag_JES_Total_Up")
      event.weight *= event.weight_c_tag_jes_total_up;
    else
      // event.weight *= 1;
      event.weight *= event.weight_c_tag;

    // this part is not satisfactory, but don't waste time
    histo_mc[region_index][i][sample_index][0]->Fill(event.n_vertex, event.weight);
    histo_mc[region_index][i][sample_index][1]->Fill(event.lepton_pt, event.weight);
    histo_mc[region_index][i][sample_index][2]->Fill(event.lepton_eta, event.weight);
    histo_mc[region_index][i][sample_index][3]->Fill(event.n_jets, event.weight);
    histo_mc[region_index][i][sample_index][4]->Fill(event.n_bjets, event.weight);
    histo_mc[region_index][i][sample_index][5]->Fill(event.n_cjets, event.weight);
    histo_mc[region_index][i][sample_index][6]->Fill(event.pt_leading_jet, event.weight);
    histo_mc[region_index][i][sample_index][7]->Fill(event.pt_subleading_jet, event.weight);
    histo_mc[region_index][i][sample_index][8]->Fill(event.eta_leading_jet, event.weight);
    histo_mc[region_index][i][sample_index][9]->Fill(event.eta_subleading_jet, event.weight);
    histo_mc[region_index][i][sample_index][10]->Fill(event.met_pt, event.weight);
    histo_mc[region_index][i][sample_index][11]->Fill(event.met_phi, event.weight);
    histo_mc[region_index][i][sample_index][12]->Fill(event.best_mva_score, event.weight);
  }

  return;
} // void Histo_Syst::Fill_Histo_MC(const int& sample_index)

//////////

int Histo_Syst::Get_Region_Index(const TString &region)
{
  return find(region_name.begin(), region_name.end(), region) - region_name.begin();
} // int Histo_Syst::Get_Region_Index(const TString& region)

//////////

void Histo_Syst::Read_Tree()
{
  cout << "[Histo_Syst::Read_Tree]: Start to read trees" << endl;

  // for MC
  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString fin_name = it_map_map->first;
    cout << fin_name << endl;

    TString syst_fix;
    if (fin_name == "Central")
      syst_fix = "None";
    else
      syst_fix = it_map_map->first;

    map<TString, TTree *> *map_tree = it_map_map->second;

    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
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

        // event veto
        if (100 < TMath::Abs(event.weight_pdf_alternative))
        {
          cout << "strange weight_pdf_alternative is detected. " << fin_name << ", " << i << ", weight_pdf_alternative = " << event.weight_pdf_alternative << endl;
          cout << "Continue" << endl;

          continue;
        }

        event.Swap();

        // Cut();
        if (!Set_Region())
          continue;
        // cout << "Test region = " << region << endl;

        int region_index = Get_Region_Index(region);

        Fill_Histo_MC(region_index, sample_index, i, syst_fix);
      } // loop over entries
    }   // loop over map_tree_mc

    // close fin to save memory
    // map<TString, TFile *> *map_fin = map_map_fin_mc[fin_name];
    // map_fin->at(fin_name)->Close();
  } // loop over map_map_tree_mc

  // for data
  for (auto it = map_tree_data.begin(); it != map_tree_data.end(); it++)
  {
    TString fin_name = it->first;
    cout << fin_name << endl;

    Long64_t n_entries = it->second->GetEntries();
    n_entries /= reduction;
    cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

    for (Long64_t i = 0; i < n_entries; i++)
    {
      if (i % 500000 == 0)
        cout << "Processing... " << i << "/" << n_entries << "." << endl;

      it->second->GetEntry(i);

      event.Swap();

      if (!Set_Region())
        continue;
      int region_index = 0;

      Fill_Histo_Data(region_index);
    }

    // map_fin_data[fin_name]->Close();
  } // loop over data period

  return;
} // void Histo_Syst::Read_Tree()

//////////

bool Histo_Syst::Set_Region()
{
  bool chk_b_tagged_had_t_b = bvsc_wp_m_2018 < event.bvsc_had_t_b ? true : false;
  bool chk_b_tagged_lep_t_b = bvsc_wp_m_2018 < event.bvsc_lep_t_b ? true : false;
  bool chk_best_mva_score = 0.9 < event.best_mva_score ? true : false;
  bool chk_n_b_tagged = 3 <= event.n_bjets ? true : false;
  bool chk_n_c_tagged = event.n_cjets == 1 ? true : false;
  bool chk_c_tagged_w_u = (cvsb_wp_m_2018 < event.cvsb_w_u && cvsl_wp_m_2018 < event.cvsl_w_u) ? true : false;
  bool chk_b_tagged_w_d = bvsc_wp_m_2018 < event.bvsc_w_d ? true : false;

  // Control 0 : b-inversion
  if (chk_b_tagged_had_t_b && chk_b_tagged_lep_t_b &&
      // chk_best_mva_score &&
      // chk_n_c_tagged &&
      // chk_c_tagged_w_u &&
      !chk_n_b_tagged &&
      !chk_b_tagged_w_d)
    region = "Control0";

  // //Control 1 : c-inversion
  // else if(chk_b_tagged_had_t_b && chk_b_tagged_lep_t_b &&
  //         chk_best_mva_score &&
  //         !chk_n_c_tagged &&
  //         !chk_c_tagged_w_u &&
  //         chk_n_b_tagged &&
  //         chk_b_tagged_w_d) region = "Control1";

  // //Control 2 : score inversion
  // else if(chk_b_tagged_had_t_b && chk_b_tagged_lep_t_b &&
  //         !chk_best_mva_score &&
  //         chk_n_c_tagged &&
  //         chk_c_tagged_w_u &&
  //         chk_n_b_tagged &&
  //         chk_b_tagged_w_d) region = "Control2";

  else
    return false;

  return true;
} // void Histo_Syst::Set_Region()

//////////
