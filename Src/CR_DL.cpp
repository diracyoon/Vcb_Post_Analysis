#include "CR_DL.h"

ClassImp(CR_DL);

//////////

CR_DL::CR_DL(const TString &a_era, const TString &a_channel, const TString &a_tagger, const int &a_index_tree_type)
    : samples(a_era, a_channel, "Vcb_DL"), tagging_rf_dl(a_era)
{
  ROOT::EnableImplicitMT(2);

  TH1::AddDirectory(kFALSE);

  cout << "[CR_DL::CR_DL]: Init analysis" << endl;

  reduction = 1;

  era = a_era;
  channel = a_channel;
  tagger = a_tagger;
  index_tree_type = a_index_tree_type; // not used

  if (era.Contains("2016"))
    year = "2016";
  else
    year = era;

  chk_rf_tthf_breakdown = false;
  chk_jes_breakdown = false;

  // chk_bin_optimizer = true;
  chk_bin_optimizer = false;

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  // syst_tree
  vec_tree_type = {"Central",
                   "UEDown", "UEUp",
                   "JetResDown", "JetResUp",
                   "JetEnDown", "JetEnUp"};

  if (chk_jes_breakdown == true)
  {
    vector<TString> vec_tree_type_jes_breakdown = {"JetEnAbsoluteDown", "JetEnAbsoluteUp",
                                                   "JetEnBBEC1Down", "JetEnBBEC1Up",
                                                   "JetEnEC2Down", "JetEnEC2Up",
                                                   "JetEnFlavorQCDDown", "JetEnFlavorQCDUp",
                                                   "JetEnHFDown", "JetEnHFUp",
                                                   "JetEnRelativeBalDown", "JetEnRelativeBalUp"};

    vector<TString> vec_tree_type_jes_breakdown_year = {"JetEnAbsolute", "JetEnBBEC1", "JetEnEC2", "JetEnHF", "JetEnRelativeSample"};
    for (unsigned int i = 0; i < vec_tree_type_jes_breakdown_year.size(); i++)
    {
      vec_tree_type_jes_breakdown.push_back(vec_tree_type_jes_breakdown_year[i] + year + "Down");
      vec_tree_type_jes_breakdown.push_back(vec_tree_type_jes_breakdown_year[i] + year + "Up");
    }

    vec_tree_type.insert(vec_tree_type.end(), vec_tree_type_jes_breakdown.begin(), vec_tree_type_jes_breakdown.end());
  } //  if (chk_jes_breakdown == true)

  // number of MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    cout << it->first << " " << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ"));
  vec_short_name_mc.push_back("TTLJ_JJ_2"); // TTLJ, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_JJ_4"); // TTLJ, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_CC_2"); // TTLJ+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_CC_4"); // TTLJ+cc, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_BB_2"); // TTLJ+bb, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_BB_4"); // TTLJ+bb, w->cd or w->cs

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB"));
  vec_short_name_mc.push_back("TTLJ_JJ_45"); // TTLJ, w->cb
  vec_short_name_mc.push_back("TTLJ_CC_45"); // TTLJ+cc, w->cb
  vec_short_name_mc.push_back("TTLJ_BB_45"); // TTLJ+cc, w->cb

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL"));
  vec_short_name_mc.push_back("TTLL_JJ"); // TTLL+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLL_CC"); // TTLL+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLL_BB"); // TTLL+bb, w->cd or w->cs

  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;
} // CR_DL(const TString &a_era = "2018", const TString &a_channel = "MM", const TString &a_run_flag = "All");

//////////

CR_DL::~CR_DL()
{
  cout << "[CR_DL::~CR_DL]: Init" << endl;

  TString fout_name = "Vcb_DL_Histos_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "RECREATE");

  // Save histos
  TDirectory *dir_region = fout->mkdir("Control_DL");

  // for data
  TDirectory *dir_data = dir_region->mkdir("Data");
  for (int i = 0; i < n_variable; i++)
  {
    dir_data->cd();

    histo_data[i]->SetTitle(variable_conf[i].variable_title);
    histo_data[i]->SetName(variable_conf[i].variable_title);
    histo_data[i]->Write();

    // histo_data_2d[i]->SetTitle(histo_type[i]);
    // histo_data_2d[i]->SetName(histo_type[i]);
    // histo_data_2d[i]->Write();
  }

  for (int i = 0; i < n_variable_2d; i++)
  {
    dir_data->cd();

    histo_data_2d[i]->SetTitle(variable_conf_2d[i].variable_title);
    histo_data_2d[i]->SetName(variable_conf_2d[i].variable_title);
    histo_data_2d[i]->Write();
  }

  // for MC
  for (int i = 0; i < n_syst; i++)
  {
    TDirectory *dir_syst = dir_region->mkdir(syst_name[i]);
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[j]);
      for (int k = 0; k < n_variable; k++)
      {
        dir_sample->cd();

        histo_mc[i][j][k]->SetTitle(variable_conf[k].variable_title);
        histo_mc[i][j][k]->SetName(variable_conf[k].variable_title);
        histo_mc[i][j][k]->Write();

        // histo_mc_2d[i][j][k]->SetTitle(histo_type[k]);
        // histo_mc_2d[i][j][k]->SetName(histo_type[k]);
        // histo_mc_2d[i][j][k]->Write();
      }

      for (int k = 0; k < n_variable_2d; k++)
      {
        dir_sample->cd();

        histo_mc_2d[i][j][k]->SetTitle(variable_conf_2d[k].variable_title);
        histo_mc_2d[i][j][k]->SetName(variable_conf_2d[k].variable_title);
        histo_mc_2d[i][j][k]->Write();
      }
    }
  }

  // PDF error set
  if (chk_merge_pdf_error_set)
  {
    TDirectory *dir_syst = dir_region->mkdir("PDF_Error_Set_Down");
    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[i]);
      for (int j = 0; j < n_variable; j++)
      {
        dir_sample->cd();
        histo_mc_pdf_error_set_down[i][j]->SetTitle(variable_conf[j].variable_title);
        histo_mc_pdf_error_set_down[i][j]->SetName(variable_conf[j].variable_title);
        histo_mc_pdf_error_set_down[i][j]->Write();
      }
    }

    dir_syst = dir_region->mkdir("PDF_Error_Set_Up");
    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[i]);
      for (int j = 0; j < n_variable; j++)
      {
        dir_sample->cd();
        histo_mc_pdf_error_set_up[i][j]->SetTitle(variable_conf[j].variable_title);
        histo_mc_pdf_error_set_up[i][j]->SetName(variable_conf[j].variable_title);
        histo_mc_pdf_error_set_up[i][j]->Write();
      }
    }
  } //  if (chk_merge_pdf_error_set)

  cout << "[CR_DL::~CR_DL]: Done" << endl;
} // CR_DL::~CR_DL()

//////////

void CR_DL::Run()
{
  Setup_Analysis();
  Setup_Syst();
  Setup_Variable();
  Setup_Histo();
  Read_Tree();
  Unroller();
  Merge_PDF_Error_Set();

  return;
} // CR_DL::Run()

//////////

void CR_DL::Read_Tree()
{
  cout << "[Histo_Syst::Read_Tree]: Start to read trees" << endl;

  // for MC
  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString tree_type = it_map_map->first;

    cout << endl;
    cout << "Tree_Type = " << tree_type << endl;

    TString syst_fix;
    if (tree_type == "Central")
      syst_fix = "None";
    else
      syst_fix = tree_type;

    map<TString, TTree *> *map_tree = it_map_map->second;

    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
      TString sample_name = it->first;

      TString sample_name_short;
      if (it->first.Contains("TTLJ") || it->first.Contains("TTLL"))
        sample_name_short = it->first;
      else
        sample_name_short = samples.map_short_name_mc[it->first];

      cout << sample_name << " " << sample_name_short << endl;

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      chk_print = true; // only for debug

      for (Long64_t i = 0; i < n_entries; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        // if ((channel == "MM" || channel == "EE") && abs(event.dilepton_mass - Z_MASS) < 15)
        //   continue;

        if (TMath::IsNaN(event.weight_scale_variation_6) || !TMath::Finite(event.weight_scale_variation_6))
          cout << "IsNaN scale variation 6 " << event.weight_scale_variation_6 << endl;

        Fill_Histo_MC(sample_name, sample_name_short, syst_fix);
      } // loop over entries
    } // for (auto it = map_tree->begin(); it != map_tree->end(); it++)
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

      // if ((channel == "MM" || channel == "EE") && abs(event.dilepton_mass - Z_MASS) < 15)
      //   continue;

      Fill_Histo_Data();
    }
  } // loop over data period

  return;
} // void CR_DL::Read_Tree()

//////////

void CR_DL::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)
{
  cout << "[CR_DL::Register_Top_Syst]:" << type << endl;

  // map_fin_syst = map_fin_mc;
  // map_fin_syst.erase("TTLJ");
  // map_fin_syst.erase("TTLL");

  map_tree_syst = map_tree_mc_central;
  map_tree_syst.erase("TTLJ");
  map_tree_syst.erase("TTLL");
  map_tree_syst.erase("TTLJ_WtoCB");

  TString path_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  TString result_path = path_base + era + "/Vcb_DL/";
  TString key_base = channel + "/";
  TString tree_name = "/Result_Tree";

  map_fin_syst["TTLJ_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_" + type]);
  map_tree_syst["TTLJ_" + type] = (TTree *)map_fin_syst["TTLJ_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLJ_" + type], Syst::Central);

  map_fin_syst["TTLL_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_" + type]);
  map_tree_syst["TTLL_" + type] = (TTree *)map_fin_syst["TTLL_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLL_" + type], Syst::Central);

  map_fin_syst["TTLJ_WtoCB_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_WtoCB_" + type]);
  map_tree_syst["TTLJ_WtoCB_" + type] = (TTree *)map_fin_syst["TTLJ_WtoCB_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLJ_WtoCB_" + type], Syst::Central);

  map_map_tree_mc[type] = &map_tree_syst;

  return;
} // void CR_DL::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)

//////////

float CR_DL::Reweight_CKM(const TString &sample_name)
{
  float reweight = 1;

  if (sample_name.Contains("TTLJ"))
  {
    if (event.decay_mode == 21)
      reweight = Vud_pdg * Vud_pdg / Vud_squared_powheg;
    else if (event.decay_mode == 23)
      reweight = Vus_pdg * Vus_pdg / Vus_squared_powheg;
    else if (event.decay_mode == 41)
      reweight = Vcd_pdg * Vcd_pdg / Vcd_squared_powheg;
    else if (event.decay_mode == 43)
      reweight = Vcs_pdg * Vcs_pdg / Vcs_squared_powheg;
  }
  else
    return 1.0;

  return reweight;
} // void CR_DL::Reweight_CKM(const TString& sample_name)

//////////

float CR_DL::Reweight_TTHF(const TString &sample_name)
{
  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")))
  {
    if (sample_name.Contains("WtoCB"))
      return 1.0;

    bool chk_b = false;
    bool chk_c = false;

    // //  for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    // for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    // {
    //   int flavour = event.vec_sel_gen_hf_flavour->at(i);
    //   int origin = event.vec_sel_gen_hf_origin->at(i);

    //   if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
    //   // if (flavour == 5 && abs(origin) == 21)
    //   {
    //     chk_b = true;
    //     break;
    //   }
    //   else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
    //     // else if (flavour == 4 && abs(origin) == 21)
    //     chk_c = true;
    // }

    if (chk_include_pseudo_additional)
    {
      if (51 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 56)
        chk_b = true;
      else if (41 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 46)
        chk_c = true;
    }
    else
    {
      if (51 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 55)
        chk_b = true;
      else if (41 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 45)
        chk_c = true;
    }

    if (chk_b == true)
      return TTBB_Scale;
    else if (chk_c == true)
      return TTCC_Scale;
    else
      return (Cross_Section_Total_TT - Cross_Section_Total_TTBB * TTBB_Scale - Cross_Section_Total_TTCC * TTCC_Scale) / Cross_Seciton_Total_TT_JJ;
    // return 1;
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    return 1.;

  return -1;
} // float CR_DL::Reweight_TTHF(const TString &sample_name)

//////////

void CR_DL::Setup_Analysis()
{
  cout << "[CR_DL::Setup_Analysis]: Init analysis" << endl;

  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    map<TString, TTree *> map_tree_mc;
    vec_map_tree_mc.push_back(map_tree_mc);
  }

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    TString path_base = getenv("Vcb_Post_Analysis_Sample_Dir");
    TString result_path = path_base + era + "/Vcb_DL/";

    map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);

    for (unsigned int i = 0; i < vec_tree_type.size(); i++)
    {
      TString tree_type = vec_tree_type[i];

      // cout << tree_type << endl;

      vec_map_tree_mc[i][it->first] = (TTree *)map_fin_mc[it->first]->Get(channel + "/" + tree_type + "/Result_Tree");

      bool chk_all = false;
      if (tree_type == "Central")
        chk_all = true;

      event.Setup_Tree(vec_map_tree_mc[i][it->first], StringToSyst(tree_type), chk_all);
    } // for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  } // for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)

  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    TString tree_type = vec_tree_type[i];

    // for Top_Syst
    if (tree_type == "Central")
      map_tree_mc_central = vec_map_tree_mc[i];

    map_map_tree_mc[tree_type] = &vec_map_tree_mc[i];
  }

  // CP5 down
  Register_Top_Syst(map_fin_mc_cp5_down, map_tree_mc_cp5_down, "CP5Down");

  // CP5 up
  Register_Top_Syst(map_fin_mc_cp5_up, map_tree_mc_cp5_up, "CP5Up");

  // hDamp down
  Register_Top_Syst(map_fin_mc_hdamp_down, map_tree_mc_hdamp_down, "hdampDown");

  // hDamp up
  Register_Top_Syst(map_fin_mc_hdamp_up, map_tree_mc_hdamp_up, "hdampUp");

  // mTop 171.5
  Register_Top_Syst(map_fin_mc_mtop_171p5, map_tree_mc_mtop_171p5, "mtop171p5");

  // mTop 173.5
  Register_Top_Syst(map_fin_mc_mtop_173p5, map_tree_mc_mtop_173p5, "mtop173p5");

  // data
  for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
  {
    cout << it->first << endl;

    TString path_base = getenv("Vcb_Post_Analysis_Sample_Dir");
    TString result_path = path_base + era + "/Vcb_DL/DATA/";
    TString key_base = channel + "/";
    TString tree_name = "/Result_Tree";

    map_fin_data[it->first] = new TFile(result_path + it->second);
    map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get(key_base + "Central" + tree_name);
    event.Setup_Tree(map_tree_data[it->first], Syst::Central, false, true);
  }

  cout << "[CR_DL::Setup_Analysis]: Done" << endl;

  return;
} // void CR_DL::Setup_Analysis()

//////////

void CR_DL::Setup_Histo()
{
  cout << "[CR_DL::Setup_Histo]: Init" << endl;

  // histo for MC
  histo_mc = new TH1D ***[n_syst];
  histo_mc_2d = new TH2D ***[n_syst];
  for (int i = 0; i < n_syst; i++)
  {
    histo_mc[i] = new TH1D **[n_sample_merge_mc];
    histo_mc_2d[i] = new TH2D **[n_sample_merge_mc];
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      histo_mc[i][j] = new TH1D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = syst_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        histo_mc[i][j][k] = new TH1D(histo_name, histo_name, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);
      }

      histo_mc_2d[i][j] = new TH2D *[n_variable_2d];
      for (int k = 0; k < n_variable_2d; k++)
      {
        TString histo_name = syst_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf_2d[k].variable_title;
        histo_mc_2d[i][j][k] = new TH2D(histo_name, histo_name, variable_conf_2d[k].n_bin_x, variable_conf_2d[k].binning_x, variable_conf_2d[k].n_bin_y, variable_conf_2d[k].binning_y);
      }
    } // loop over n_sample_merge_mc
  } // loop over n_syst

  // histo for data
  histo_data = new TH1D *[n_variable];
  for (int i = 0; i < n_variable; i++)
  {
    TString histo_name = variable_conf[i].variable_title;
    histo_data[i] = new TH1D(histo_name, histo_name, variable_conf[i].n_bin, variable_conf[i].x_low, variable_conf[i].x_up);
  }

  histo_data_2d = new TH2D *[n_variable_2d];
  for (int i = 0; i < n_variable_2d; i++)
  {
    TString histo_name = variable_conf_2d[i].variable_title;
    histo_data_2d[i] = new TH2D(histo_name, histo_name, variable_conf_2d[i].n_bin_x, variable_conf_2d[i].binning_x, variable_conf_2d[i].n_bin_y, variable_conf_2d[i].binning_y);
  }

  cout << "[CR_DL::Setup_Histo]: Done" << endl;

  return;
} // void CR_DL::Setup_Histo()

//////////

void CR_DL::Setup_Syst()
{
  cout << "[CR_DL::Setup_Syst]: Init" << endl;

  // Systs
  syst_name = {"Nominal",
               "Jet_En_Down", "Jet_En_Up",
               "Jet_Res_Down", "Jet_Res_Up",
               "UE_Down", "UE_Up",
               "PDF_Alternative", "PDF_As_Down", "PDF_As_Up",
               "Pileup_Down", "Pileup_Up",
               "PU_Jet_Veto_Down", "PU_Jet_Veto_Up",
               "Prefire_Down", "Prefire_Up",
               "MuF_TT_Down", "MuF_TT_Up", "MuF_ST_Down", "MuF_ST_Up", "MuF_WJets_Down", "MuF_WJets_Up", "MuF_DYJets_Down", "MuF_DYJets_Up",
               "MuR_TT_Down", "MuR_TT_Up", "MuR_ST_Down", "MuR_ST_Up", "MuR_WJets_Down", "MuR_WJets_Up", "MuR_DYJets_Down", "MuR_DYJets_Up",
               //"MuF_MuR_Down", "MuR_MuR_Up",
               "FSR_Down", "FSR_Up",
               "ISR_TT_Down", "ISR_TT_Up", "ISR_ST_Down", "ISR_ST_Up", "ISR_WJets_Down", "ISR_WJets_Up", "ISR_DYJets_Down", "ISR_DYJets_Up",
               "Top_Pt_Reweight",
               "CP5_Down", "CP5_Up",
               "hdamp_Down", "hdamp_Up",
               "mtop_171p5", "mtop_173p5"};

  if (chk_jes_breakdown == true)
  {
    vector<TString> syst_name_jes_breakdown = {"Jet_En_Absolute_Down", "Jet_En_Absolute_Up",
                                               "Jet_En_BBEC1_Down", "Jet_En_BBEC1_Up",
                                               "Jet_En_EC2_Down", "Jet_En_EC2_Up",
                                               "Jet_En_FlavorQCD_Down", "Jet_En_FlavorQCD_Up",
                                               "Jet_En_HF_Down", "Jet_En_HF_Up",
                                               "Jet_En_RelativeBal_Down", "Jet_En_RelativeBal_Up"};

    vector<TString> syst_name_jes_breakdown_year = {"Jet_En_Absolute", "Jet_En_BBEC1", "Jet_En_EC2", "Jet_En_HF", "Jet_En_RelativeSample"};
    for (unsigned int i = 0; i < syst_name_jes_breakdown_year.size(); i++)
    {
      syst_name_jes_breakdown.push_back(syst_name_jes_breakdown_year[i] + year + "_Down");
      syst_name_jes_breakdown.push_back(syst_name_jes_breakdown_year[i] + year + "_Up");
    }

    syst_name.insert(syst_name.end(), syst_name_jes_breakdown.begin(), syst_name_jes_breakdown.end());
  }

  if (tagger == "B")
  {
    vector<TString> syst_tagger = {"B_Tag_HF_Down", "B_Tag_HF_Up",
                                   "B_Tag_LF_Down", "B_Tag_LF_Up",
                                   // "B_Tag_JES_Down", "B_Tag_JES_Up",
                                   "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up",
                                   "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up",
                                   "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up",
                                   "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up",
                                   "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up",
                                   "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up"};

    syst_name.insert(syst_name.end(), syst_tagger.begin(), syst_tagger.end());
  }
  else if (tagger == "C")
  {
    vector<TString> syst_tagger = {"C_Tag_Extrap_Down", "C_Tag_Extrap_Up",
                                   "C_Tag_Interp_Down", "C_Tag_Interp_Up",
                                   //"C_Tag_LHE_Scale_MuF_Down", "C_Tag_LHE_Scale_MuF_Up",
                                   //"C_Tag_LHE_Scale_MuR_Down", "C_Tag_LHE_Scale_MuR_Up",
                                   //"C_Tag_PS_FSR_Fixed_Down", "C_Tag_PS_FSR_Fixed_Up",
                                   //"C_Tag_PS_ISR_Fixed_Down", "C_Tag_PS_ISR_Fixed_Up",
                                   //"C_Tag_PU_Down", "C_Tag_PU_Up",
                                   "C_Tag_Stat_Down", "C_Tag_Stat_Up",
                                   "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up",
                                   "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up",
                                   "C_Tag_XSec_Br_Unc_WJets_C_Down", "C_Tag_XSec_Br_Unc_WJets_C_Up"};
    //"C_Tag_JER_Down", "C_Tag_JER_Up",
    //"C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up",};

    syst_name.insert(syst_name.end(), syst_tagger.begin(), syst_tagger.end());
  }

  if (channel == "MM")
  {
    syst_name.push_back("Mu_Trig_Down");
    syst_name.push_back("Mu_Trig_Up");
    syst_name.push_back("Mu_Id_Down");
    syst_name.push_back("Mu_Id_Up");
    syst_name.push_back("Mu_Iso_Down");
    syst_name.push_back("Mu_Iso_Up");
  }
  else if (channel == "ME")
  {
    syst_name.push_back("Mu_Trig_Down");
    syst_name.push_back("Mu_Trig_Up");
    syst_name.push_back("El_Trig_Down");
    syst_name.push_back("El_Trig_Up");

    syst_name.push_back("Mu_Id_Down");
    syst_name.push_back("Mu_Id_Up");
    syst_name.push_back("Mu_Iso_Down");
    syst_name.push_back("Mu_Iso_Up");

    syst_name.push_back("El_Id_Down");
    syst_name.push_back("El_Id_Up");
    syst_name.push_back("El_Reco_Down");
    syst_name.push_back("El_Reco_Up");
    // syst_name.push_back("El_En_Down");
    // syst_name.push_back("El_En_Up");
    // syst_name.push_back("El_Res_Down");
    // syst_name.push_back("El_Res_Up");
  }
  else if (channel == "EE")
  {
    syst_name.push_back("El_Trig_Down");
    syst_name.push_back("El_Trig_Up");

    syst_name.push_back("El_Id_Down");
    syst_name.push_back("El_Id_Up");
    syst_name.push_back("El_Reco_Down");
    syst_name.push_back("El_Reco_Up");
    // syst_name.push_back("El_En_Down");
    // syst_name.push_back("El_En_Up");
    // syst_name.push_back("El_Res_Down");
    // syst_name.push_back("El_Res_Up");
  }

  for (int i = 0; i < sizeof(event.weight_pdf_error_set) / sizeof(float); i++)
  {
    TString pdf_error_set = "PDF_Error_Set_" + to_string(i);
    syst_name.push_back(pdf_error_set);
  }

  n_syst = syst_name.size();
  cout << "N_Syst = " << n_syst << endl;

  cout << "[CR_DL::Setup_Syst]: Done" << endl;

  return;
} // void CR_DL::Setup_Syst()

//////////

void CR_DL::Setup_Variable()
{
  cout << "[CR_DL::Setup_Variable]: Init" << endl;

  if (chk_bin_optimizer)
  {
    int n_bin = 40;
    for (int i = 0; i < n_bin; i++)
      bin_bvsc.push_back(1. / n_bin * i);
    bin_bvsc.push_back(1);
  } // if(chk_bin_optimizer)
  else
  {
    // Data Driven
    if (era == "2016preVFP")
    {
      if (channel == "MM")
        bin_bvsc = {0.0, 0.025, 0.075, 0.25, 1.0};
      else if (channel == "ME")
        bin_bvsc = {0.0, 0.025, 0.05, 0.125, 0.35, 1.0};
      else if (channel == "EE")
        bin_bvsc = {0.0, 0.025, 0.125, 1.0};
    }
    else if (era == "2016postVFP")
    {
      if (channel == "MM")
        bin_bvsc = {0.0, 0.025, 0.075, 0.2, 1.0};
      else if (channel == "ME")
        bin_bvsc = {0.0, 0.025, 0.05, 0.125, 0.325, 1.0};
      else if (channel == "EE")
        bin_bvsc = {0.0, 0.025, 0.125, 1.0};
    }
    else if (era == "2017")
    {
      if (channel == "MM")
        bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.2, 0.525, 1.0};
      else if (channel == "ME")
        bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.15, 0.3, 0.7, 1.0};
      else if (channel == "EE")
        bin_bvsc = {0.0, 0.025, 0.05, 0.1, 0.3, 1.0};
    }
    else if (era == "2018")
    {
      if (channel == "MM")
        bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.15, 0.3, 0.7, 1.0};
      else if (channel == "ME")
        bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.125, 0.225, 0.425, 0.8, 1.0};
      else if (channel == "EE")
        bin_bvsc = {0.0, 0.025, 0.05, 0.125, 0.4, 1.0};
    }

    // // MC
    // if (era == "2016preVFP")
    // {
    //   if (channel == "MM")
    //     bin_bvsc = {0.0, 0.025, 0.075, 0.25, 1.0};
    //   else if (channel == "ME")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.125, 0.35, 1.0};
    //   else if (channel == "EE")
    //     bin_bvsc = {0.0, 0.025, 0.1, 1.0};
    // }
    // else if (era == "2016postVFP")
    // {
    //   if (channel == "MM")
    //     bin_bvsc = {0.0, 0.025, 0.075, 0.2, 1.0};
    //   else if (channel == "ME")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.125, 0.3, 1.0};
    //   else if (channel == "EE")
    //     bin_bvsc = {0.0, 0.025, 0.1, 1.0};
    // }
    // else if (era == "2017")
    // {
    //   if (channel == "MM")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.2, 0.525, 1.0};
    //   else if (channel == "ME")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.125, 0.275, 0.675, 1.0};
    //   else if (channel == "EE")
    //     bin_bvsc = {0.0, 0.025, 0.075, 0.275, 1.0};
    // }
    // else if (era == "2018")
    // {
    //   if (channel == "MM")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.15, 0.3, 0.7, 1.0};
    //   else if (channel == "ME")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.075, 0.125, 0.2, 0.4, 0.775, 1.0};
    //   else if (channel == "EE")
    //     bin_bvsc = {0.0, 0.025, 0.05, 0.1, 0.35, 1.0};
    // }

    // // Old
    // bin_bvsc.push_back(0);
    // if (era == "2016preVFP")
    // {
    //   bin_bvsc.push_back(bvsc_wp_l_2016preVFP);
    //   bin_bvsc.push_back(bvsc_wp_m_2016preVFP);
    //   bin_bvsc.push_back(bvsc_wp_t_2016preVFP);
    // }
    // else if (era == "2016postVFP")
    // {
    //   bin_bvsc.push_back(bvsc_wp_l_2016postVFP);
    //   bin_bvsc.push_back(bvsc_wp_m_2016postVFP);
    //   bin_bvsc.push_back(bvsc_wp_t_2016postVFP);
    // }
    // else if (era == "2017")
    // {
    //   bin_bvsc.push_back(bvsc_wp_l_2017);
    //   bin_bvsc.push_back(bvsc_wp_m_2017);
    //   bin_bvsc.push_back(bvsc_wp_t_2017);
    // }
    // else if (era == "2018")
    // {
    //   bin_bvsc.push_back(bvsc_wp_l_2018);
    //   bin_bvsc.push_back(bvsc_wp_m_2018);
    //   bin_bvsc.push_back(bvsc_wp_t_2018);
    // }
    // bin_bvsc.push_back(1);
  } // else

  sort(bin_bvsc.begin(), bin_bvsc.end());
  int n_bin_bvsc = bin_bvsc.size() - 1;

  // set variables to be drawn
  variable_conf = {{"BvsC_3rd_4th_Jets_Unrolled", TMath::Binomial(n_bin_bvsc, 2), 0., TMath::Binomial(n_bin_bvsc, 2)},
                   {"N_Vertex", 40, 0, 80},
                   {"Leading_Lepton_Eta", 30, -3, 3},
                   {"Leading_Lepton_Pt", 50, 0, 400},
                   {"Subleading_Lepton_Eta", 50, -3, 3},
                   {"Subleading_Lepton_Pt", 50, 0, 400},
                   {"DiLepton_Mass", 50, 0, 500},
                   {"N_Jet", 20, 0, 20},
                   {"N_B_Jet", 10, 0, 10},
                   {"N_C_Jet", 15, 0, 15},
                   {"HT", 50, 0, 1000},
                   {"Leading_Jet_BvsC", 20, 0, 1},
                   {"Leading_Jet_CvsB", 20, 0, 1},
                   {"Leading_Jet_CvsL", 20, 0, 1},
                   {"Subleading_Jet_BvsC", 20, 0, 1},
                   {"Subleading_Jet_CvsB", 20, 0, 1},
                   {"Subleading_Jet_CvsL", 20, 0, 1},
                   {"Met_Pt", 50, 0, 500},
                   {"Met_Phi", 40, -4, 4},
                   {"Leading_Jet_Charge", 30, -1.5, 1.5},
                   {"Subleading_jet_Charge", 30, -1.5, 1.5},
                   {"Leading_Jet_Pt", 50, 0, 400},
                   {"Subleading_Jet_Pt", 50, 0, 400},
                   {"BvsC_Third_Pt", 50, 0, 400},
                   {"BvsC_Third_Eta", 50, -3, 3},
                   {"BvsC_Fourth_Pt", 50, 0, 400},
                   {"BvsC_Fourth_Eta", 50, -3, 3}};
  n_variable = variable_conf.size();

  variable_conf_2d = {{"BvsC_3rd_4th_Jets", n_bin_bvsc, bin_bvsc.data(), n_bin_bvsc, bin_bvsc.data()}};
  n_variable_2d = variable_conf_2d.size();

  cout << "[CR_DL::Setup_Variable]: Done" << endl;

  return;
} // void CR_DL::Setup_Variable()

//////////

void CR_DL::Fill_Histo_Data()
{
  int unrolled_bin = Unroller(event.bvsc_third, event.bvsc_fourth);

  // histo_data[0]->Fill(unrolled_bin, 1.);
  histo_data[1]->Fill(event.n_pv, 1.);
  histo_data[2]->Fill(event.leading_lepton_eta, 1.);
  histo_data[3]->Fill(event.leading_lepton_pt, 1.);
  histo_data[4]->Fill(event.subleading_lepton_eta, 1.);
  histo_data[5]->Fill(event.subleading_lepton_pt, 1.);
  histo_data[6]->Fill(event.dilepton_mass, 1.);
  histo_data[7]->Fill(event.n_jet, 1.);
  histo_data[8]->Fill(event.n_b_jet, 1.);
  histo_data[9]->Fill(event.n_c_jet, 1);
  histo_data[10]->Fill(event.ht, 1.);
  histo_data[11]->Fill(event.leading_jet_bvsc, 1.);
  histo_data[12]->Fill(event.leading_jet_cvsb, 1.);
  histo_data[13]->Fill(event.leading_jet_cvsl, 1.);
  histo_data[14]->Fill(event.subleading_jet_bvsc, 1.);
  histo_data[15]->Fill(event.subleading_jet_cvsb, 1.);
  histo_data[16]->Fill(event.subleading_jet_cvsl, 1.);
  histo_data[17]->Fill(event.met_pt, 1.);
  histo_data[18]->Fill(event.met_phi, 1.);
  histo_data[19]->Fill(event.leading_jet_charge, 1);
  histo_data[20]->Fill(event.subleading_jet_charge, 1);
  histo_data[21]->Fill(event.leading_jet_pt, 1);
  histo_data[22]->Fill(event.subleading_jet_pt, 1);
  histo_data[23]->Fill(event.bvsc_third_pt, 1);
  histo_data[24]->Fill(event.bvsc_third_eta, 1);
  histo_data[25]->Fill(event.bvsc_fourth_pt, 1);
  histo_data[26]->Fill(event.bvsc_fourth_eta, 1);

  histo_data_2d[0]->Fill(event.bvsc_third, event.bvsc_fourth, 1.);

  return;
} // void CR_DL::Fill_Histo_Data()

//////////

void CR_DL::Fill_Histo_MC(const TString &sample_name, const TString &sample_name_short, const TString &syst_fix)
{
  int unrolled_bin = Unroller(event.bvsc_third, event.bvsc_fourth);

  int histo_index = Histo_Index(sample_name_short);
  TString histo_name_rf = Histo_Name_RF(sample_name);

  float weight_ckm = Reweight_CKM(sample_name);
  float weight_tthf = Reweight_TTHF(sample_name_short);

  // cout << "test sample_name = " << sample_name << ", histo_index = " << histo_index << ", histo_index_rf = " << histo_name_rf << endl;

  for (int i = 0; i < n_syst; i++)
  {
    TString syst_type = syst_name[i];

    // for central tree, don't consider the following syst
    if (syst_fix == "None" &&
        (syst_type == "Jet_Res_Down" || syst_type == "Jet_Res_Up" ||
         syst_type == "UE_Down" || syst_type == "UE_Up" ||
         syst_type == "El_En_Down" || syst_type == "El_En_Up" ||
         syst_type == "El_Res_Down" || syst_type == "El_Res_Up" ||
         syst_type == "CP5_Down" || syst_type == "CP5_Up" ||
         syst_type == "hdamp_Down" || syst_type == "hdamp_Up" ||
         syst_type == "mtop_171p5" || syst_type == "mtop_173p5" ||
         syst_type.Contains("Jet_En")))
      continue;

    // if syst_fix is fixed, consider only the corresponding tree
    if (syst_fix != "None")
    {
      TString temp = syst_type;
      if (syst_fix != temp.ReplaceAll("_", ""))
        continue;
    }

    event.weight = 1;
    event.weight *= event.weight_lumi;
    event.weight *= event.weight_mc;
    event.weight *= event.weight_hem_veto;

    // ckm reweight
    event.weight *= weight_ckm;

    // tt+hf reweight
    event.weight *= weight_tthf;

    // PDF errors
    if (syst_type == "PDF_Alternative")
      event.weight *= event.weight_pdf_alternative;
    else if (syst_type.Contains("PDF_Error_Set_"))
    {
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

    // parton shower
    if (syst_type == "FSR_Down" && (sample_name.Contains("TT") || sample_name.Contains("SingleTop")))
      event.weight *= event.weight_ps[0];
    else if (syst_type == "FSR_Up" && (sample_name.Contains("TT") || sample_name.Contains("SingleTop")))
      event.weight *= event.weight_ps[1];
    else if (syst_type.Contains("ISR") &&
             ((syst_type.Contains("TT") && sample_name.Contains("TT")) ||
              (syst_type.Contains("ST") && sample_name.Contains("SingleTop")) ||
              (syst_type.Contains("WJets") && sample_name.Contains("WJets")) ||
              (syst_type.Contains("DYJets") && sample_name.Contains("DYJets"))))
    {
      if (syst_type.Contains("Down"))
        event.weight *= event.weight_ps[2];
      else if (syst_type.Contains("Up"))
        event.weight *= event.weight_ps[3];
    }

    // scale variation
    if (syst_type == "MuF_MuR_Down")
      event.weight *= event.weight_scale_variation_8;
    else if (syst_type == "MuF_MuR_Up")
      event.weight *= event.weight_scale_variation_4;
    else if (syst_type.Contains("MuF") &&
             ((syst_type.Contains("TT") && sample_name.Contains("TT")) ||
              (syst_type.Contains("ST") && sample_name.Contains("SingleTop")) ||
              (syst_type.Contains("WJets") && sample_name.Contains("WJets")) ||
              (syst_type.Contains("DYJets") && sample_name.Contains("DYJets"))))
    {
      if (syst_type.Contains("Down"))
        event.weight *= event.weight_scale_variation_2;
      else if (syst_type.Contains("Up"))
        event.weight *= event.weight_scale_variation_1;
    }
    else if (syst_type.Contains("MuR") &&
             ((syst_type.Contains("TT") && sample_name.Contains("TT")) ||
              (syst_type.Contains("ST") && sample_name.Contains("SingleTop")) ||
              (syst_type.Contains("WJets") && sample_name.Contains("WJets")) ||
              (syst_type.Contains("DYJets") && sample_name.Contains("DYJets"))))
    {
      if (syst_type.Contains("Down"))
        event.weight *= event.weight_scale_variation_6;
      else if (syst_type.Contains("Up"))
        event.weight *= event.weight_scale_variation_3;
    }

    // pileup
    if (syst_type == "Pileup_Down")
      event.weight *= event.weight_pileup_down;
    else if (syst_type == "Pileup_Up")
      event.weight *= event.weight_pileup_up;
    else
      event.weight *= event.weight_pileup;

    // prefire
    if (syst_type == "Prefire_Down")
      event.weight *= event.weight_prefire_down;
    else if (syst_type == "Prefire_Up")
      event.weight *= event.weight_prefire_up;
    else
      event.weight *= event.weight_prefire;

    // pileup jet veto id
    if (syst_type == "PU_Jet_Veto_Down")
      event.weight *= event.weight_pujet_veto_down;
    else if (syst_type == "PU_Jet_Veto_Up")
      event.weight *= event.weight_pujet_veto_up;
    else
      event.weight *= event.weight_pujet_veto;

    // top pt
    if (syst_type == "Top_Pt_Reweight")
      event.weight *= 1;
    else
      event.weight *= event.weight_top_pt;

    // single lepton trigger
    if (syst_type == "Mu_Trig_Down")
      event.weight *= event.weight_sl_trig_mu_down;
    else if (syst_type == "Mu_Trig_Up")
      event.weight *= event.weight_sl_trig_mu_up;
    else if (syst_type == "El_Trig_Up")
      event.weight *= event.weight_sl_trig_el_up;
    else if (syst_type == "El_Trig_Down")
      event.weight *= event.weight_sl_trig_el_down;
    else
      event.weight *= event.weight_sl_trig;

    if (syst_type == "Mu_Id_Down")
      event.weight *= event.weight_mu_id_down;
    else if (syst_type == "Mu_Id_Up")
      event.weight *= event.weight_mu_id_up;
    else
      event.weight *= event.weight_mu_id;

    if (syst_type == "Mu_Iso_Down")
      event.weight *= event.weight_mu_iso_down;
    else if (syst_type == "Mu_Iso_Up")
      event.weight *= event.weight_mu_iso_up;
    else
      event.weight *= event.weight_mu_iso;

    if (syst_type == "El_Id_Down")
      event.weight *= event.weight_el_id_down;
    else if (syst_type == "El_Id_Up")
      event.weight *= event.weight_el_id_up;
    else
      event.weight *= event.weight_el_id;

    if (syst_type == "El_Reco_Down")
      event.weight *= event.weight_el_reco_down;
    else if (syst_type == "El_Reco_Up")
      event.weight *= event.weight_el_reco_up;
    else
      event.weight *= event.weight_el_reco;

    if (tagger == "B")
    {
      TString b_tagging_rf_type;

      if (syst_type == "B_Tag_HF_Down")
      {
        event.weight *= event.weight_b_tag_hf_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_HF_Up")
      {
        event.weight *= event.weight_b_tag_hf_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_LF_Down")
      {
        event.weight *= event.weight_b_tag_lf_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_LF_Up")
      {
        event.weight *= event.weight_b_tag_lf_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_LFStats1_Down")
      {
        event.weight *= event.weight_b_tag_lfstats1_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_LFStats1_Up")
      {
        event.weight *= event.weight_b_tag_lfstats1_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_LFStats2_Down")
      {
        event.weight *= event.weight_b_tag_lfstats2_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_LFStats2_Up")
      {
        event.weight *= event.weight_b_tag_lfstats2_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_CFErr1_Down")
      {
        event.weight *= event.weight_b_tag_cferr1_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_CFErr1_Up")
      {
        event.weight *= event.weight_b_tag_cferr1_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_CFErr2_Down")
      {
        event.weight *= event.weight_b_tag_cferr2_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_CFErr2_Up")
      {
        event.weight *= event.weight_b_tag_cferr2_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_HFStats1_Down")
      {
        event.weight *= event.weight_b_tag_hfstats1_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_HFStats1_Up")
      {
        event.weight *= event.weight_b_tag_hfstats1_up;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_HFStats2_Down")
      {
        event.weight *= event.weight_b_tag_hfstats2_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_HFStats2_Up")
      {
        event.weight *= event.weight_b_tag_hfstats2_up;
        b_tagging_rf_type = syst_type;
      }
      // fully correlated b-tagging scale factors
      else if (syst_type.Contains("Jet_En") && syst_type.Contains("Down"))
      {
        event.weight *= event.weight_b_tag_jes_down;
        b_tagging_rf_type = "B_Tag_JES_Down";
      }
      else if (syst_type.Contains("Jet_En") && syst_type.Contains("Up"))
      {
        event.weight *= event.weight_b_tag_jes_up;
        b_tagging_rf_type = "B_Tag_JES_Up";
      }
      else
      {
        event.weight *= event.weight_b_tag;
        b_tagging_rf_type = "B_Tag_Nominal";
      }

      // cout << "test " << histo_name_rf << " " << b_tagging_rf_type << endl;
      event.weight *= tagging_rf_dl.Get_Tagging_RF_DL_B_Tag(histo_name_rf, b_tagging_rf_type, event.vec_jet_pt, event.vec_jet_eta, event.vec_jet_flavor);
    } // if (tagger == "B")
    else if (tagger == "C")
    {
      TString c_tagging_rf_type;
      if (syst_type == "C_Tag_Extrap_Down")
      {
        event.weight *= event.weight_c_tag_extrap_down;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_Extrap_Up")
      {
        event.weight *= event.weight_c_tag_extrap_up;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_Interp_Down")
      {
        event.weight *= event.weight_c_tag_interp_down;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_Interp_Up")
      {
        event.weight *= event.weight_c_tag_interp_up;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_Stat_Down")
      {
        event.weight *= event.weight_c_tag_stat_down;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_Stat_Up")
      {
        event.weight *= event.weight_c_tag_stat_up;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_B_Down")
      {
        event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_b_down;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_B_Up")
      {
        event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_b_up;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_C_Down")
      {
        event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_c_down;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_XSec_Br_Unc_DYJets_C_Up")
      {
        event.weight *= event.weight_c_tag_xsec_br_unc_dyjets_c_up;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_XSec_Br_Unc_WJets_C_Down")
      {
        event.weight *= event.weight_c_tag_xsec_br_unc_wjets_c_down;
        c_tagging_rf_type = syst_type;
      }
      else if (syst_type == "C_Tag_XSec_Br_Unc_WJets_C_Up")
      {
        event.weight *= event.weight_c_tag_xsec_br_unc_wjets_c_up;
        c_tagging_rf_type = syst_type;
      }

      //////////////////////////////////////////////
      /* fully correlated c-tagging scale factors */
      //////////////////////////////////////////////

      // JER
      else if (syst_type == "Jet_Res_Down")
      {
        event.weight *= event.weight_c_tag_jer_down;
        c_tagging_rf_type = "C_Tag_JER_Down";
      }
      else if (syst_type == "Jet_Res_Up")
      {
        event.weight *= event.weight_c_tag_jer_up;
        c_tagging_rf_type = "C_Tag_JER_Up";
      }
      // JES Down Total & Breakdown
      else if (syst_type.Contains("Jet_En") && syst_type.Contains("Down"))
      {
        event.weight *= event.weight_c_tag_jes_total_down;

        if (syst_type == "Jet_En_Down")
          c_tagging_rf_type = "C_Tag_JES_Total_Down";
        else
        {
          c_tagging_rf_type = syst_type;
          c_tagging_rf_type.ReplaceAll("Jet_En", "C_Tag_JES");
        }
      }
      // JES Up Total & Breakdown
      else if (syst_type.Contains("Jet_En") && syst_type.Contains("Up"))
      {
        event.weight *= event.weight_c_tag_jes_total_up;

        if (syst_type == "Jet_En_Up")
          c_tagging_rf_type = "C_Tag_JES_Total_Up";
        else
        {
          c_tagging_rf_type = syst_type;
          c_tagging_rf_type.ReplaceAll("Jet_En", "C_Tag_JES");
        }
      }
      else if (syst_type.Contains("MuF") &&
               ((syst_type.Contains("TT") && sample_name.Contains("TT")) ||
                (syst_type.Contains("ST") && sample_name.Contains("SingleTop")) ||
                (syst_type.Contains("WJets") && sample_name.Contains("WJets")) ||
                (syst_type.Contains("DYJets") && sample_name.Contains("DYJets"))))
      {
        if (syst_type.Contains("Down"))
        {
          event.weight *= event.weight_c_tag_lhe_scale_muf_down;
          c_tagging_rf_type = "C_Tag_LHE_Scale_MuF_Down";
        }
        else if (syst_type.Contains("Up"))
        {
          event.weight *= event.weight_c_tag_lhe_scale_muf_up;
          c_tagging_rf_type = "C_Tag_LHE_Scale_MuF_Up";
        }
      }
      else if (syst_type.Contains("MuR") &&
               ((syst_type.Contains("TT") && sample_name.Contains("TT")) ||
                (syst_type.Contains("ST") && sample_name.Contains("SingleTop")) ||
                (syst_type.Contains("WJets") && sample_name.Contains("WJets")) ||
                (syst_type.Contains("DYJets") && sample_name.Contains("DYJets"))))
      {
        if (syst_type.Contains("Down"))
        {
          event.weight *= event.weight_c_tag_lhe_scale_mur_down;
          c_tagging_rf_type = "C_Tag_LHE_Scale_MuR_Down";
        }
        else if (syst_type.Contains("Up"))
        {
          event.weight *= event.weight_c_tag_lhe_scale_mur_up;
          c_tagging_rf_type = "C_Tag_LHE_Scale_MuR_Up";
        }
      }
      else if (syst_type == "FSR_Down")
      {
        event.weight *= event.weight_c_tag_ps_fsr_fixed_down;
        c_tagging_rf_type = "C_Tag_PS_FSR_Fixed_Down";
      }
      else if (syst_type == "FSR_Up")
      {
        event.weight *= event.weight_c_tag_ps_fsr_fixed_up;
        c_tagging_rf_type = "C_Tag_PS_FSR_Fixed_Up";
      }
      else if (syst_type.Contains("ISR") &&
               ((syst_type.Contains("TT") && sample_name.Contains("TT")) ||
                (syst_type.Contains("ST") && sample_name.Contains("SingleTop")) ||
                (syst_type.Contains("WJets") && sample_name.Contains("WJets")) ||
                (syst_type.Contains("DYJets") && sample_name.Contains("DYJets"))))
      {
        if (syst_type.Contains("Down"))
        {
          event.weight *= event.weight_c_tag_ps_isr_fixed_down;
          c_tagging_rf_type = "C_Tag_PS_ISR_Fixed_Down";
        }
        else if (syst_type.Contains("Up"))
        {
          event.weight *= event.weight_c_tag_ps_isr_fixed_up;
          c_tagging_rf_type = "C_Tag_PS_ISR_Fixed_Up";
        }
      }
      else if (syst_type == "Pileup_Down")
      {
        event.weight *= event.weight_c_tag_pu_down;
        c_tagging_rf_type = "C_Tag_PU_Down";
      }
      else if (syst_type == "Pileup_Up")
      {
        event.weight *= event.weight_c_tag_pu_up;
        c_tagging_rf_type = "C_Tag_PU_Up";
      }
      else
      {
        event.weight *= event.weight_c_tag;
        c_tagging_rf_type = "C_Tag_Nominal";
      }

      // if (chk_print)
      // {
      //   cout << "test " << histo_name_rf << " " << c_tagging_rf_type << endl;
      //  chk_print = false;
      // }
      // c_tagging_rf_type = "C_Tag_Nominal";

      event.weight *= tagging_rf_dl.Get_Tagging_RF_DL_C_Tag(histo_name_rf, c_tagging_rf_type, event.vec_jet_pt, event.vec_jet_eta, event.vec_jet_flavor);
    }

    // histo_mc[i][histo_index][0]->Fill(unrolled_bin, event.weight);
    histo_mc[i][histo_index][1]->Fill(event.n_pv, event.weight);
    histo_mc[i][histo_index][2]->Fill(event.leading_lepton_eta, event.weight);
    histo_mc[i][histo_index][3]->Fill(event.leading_lepton_pt, event.weight);
    histo_mc[i][histo_index][4]->Fill(event.subleading_lepton_eta, event.weight);
    histo_mc[i][histo_index][5]->Fill(event.subleading_lepton_pt, event.weight);
    histo_mc[i][histo_index][6]->Fill(event.dilepton_mass, event.weight);
    histo_mc[i][histo_index][7]->Fill(event.n_jet, event.weight);
    histo_mc[i][histo_index][8]->Fill(event.n_b_jet, event.weight);
    histo_mc[i][histo_index][9]->Fill(event.n_c_jet, event.weight);
    histo_mc[i][histo_index][10]->Fill(event.ht, event.weight);
    histo_mc[i][histo_index][11]->Fill(event.leading_jet_bvsc, event.weight);
    histo_mc[i][histo_index][12]->Fill(event.leading_jet_cvsb, event.weight);
    histo_mc[i][histo_index][13]->Fill(event.leading_jet_cvsl, event.weight);
    histo_mc[i][histo_index][14]->Fill(event.subleading_jet_bvsc, event.weight);
    histo_mc[i][histo_index][15]->Fill(event.subleading_jet_cvsb, event.weight);
    histo_mc[i][histo_index][16]->Fill(event.subleading_jet_cvsl, event.weight);
    histo_mc[i][histo_index][17]->Fill(event.met_pt, event.weight);
    histo_mc[i][histo_index][18]->Fill(event.met_phi, event.weight);
    histo_mc[i][histo_index][19]->Fill(event.leading_jet_charge, event.weight);
    histo_mc[i][histo_index][20]->Fill(event.subleading_jet_charge, event.weight);
    histo_mc[i][histo_index][21]->Fill(event.leading_jet_pt, event.weight);
    histo_mc[i][histo_index][22]->Fill(event.subleading_jet_pt, event.weight);
    histo_mc[i][histo_index][23]->Fill(event.bvsc_third_pt, event.weight);
    histo_mc[i][histo_index][24]->Fill(event.bvsc_third_eta, event.weight);
    histo_mc[i][histo_index][25]->Fill(event.bvsc_fourth_pt, event.weight);
    histo_mc[i][histo_index][26]->Fill(event.bvsc_fourth_eta, event.weight);

    histo_mc_2d[i][histo_index][0]->Fill(event.bvsc_third, event.bvsc_fourth, event.weight);
  } // loop over n_syst

  return;
} // void CR_DL::Fill_Histo_MC(const int &sample_index, const TString &syst_fix)

//////////

int CR_DL::Histo_Index(const TString &sample_name)
{
  int index = -999;

  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")))
  {

    TString histo_name = sample_name;

    if (histo_name.Contains("WtoCB"))
      histo_name.ReplaceAll("TTLJ_WtoCB", "TTLJ");

    if (histo_name.Contains("CP5") || histo_name.Contains("hdamp") || histo_name.Contains("mtop"))
    {
      if (histo_name.Contains("TTLJ"))
        histo_name = "TTLJ";
      else if (histo_name.Contains("TTLL"))
        histo_name = "TTLL";
    }

    bool chk_b = false;
    bool chk_c = false;

    // // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    // for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    // {
    //   int flavour = event.vec_sel_gen_hf_flavour->at(i);
    //   int origin = event.vec_sel_gen_hf_origin->at(i);

    //   if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
    //   // if (flavour == 5 && abs(origin) == 21)
    //   {
    //     chk_b = true;
    //     break;
    //   }
    //   else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
    //     // else if (flavour == 4 && abs(origin) == 21)
    //     chk_c = true;
    // }

    if (chk_include_pseudo_additional)
    {
      if (51 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 56)
        chk_b = true;
      else if (41 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 46)
        chk_c = true;
    }
    else
    {
      if (51 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 55)
        chk_b = true;
      else if (41 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 45)
        chk_c = true;
    }

    if (chk_b)
      histo_name += "_BB";
    else if (chk_c)
      histo_name += "_CC";
    else
      histo_name += "_JJ";

    if (event.decay_mode == 21 || event.decay_mode == 23)
      histo_name += "_2";
    else if (event.decay_mode == 41 || event.decay_mode == 43)
      histo_name += "_4";
    else if (event.decay_mode == 45)
      histo_name += "_45";

    index = index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();

  // cout << "test Histo_Index: " << sample_name << " " << index << endl;

  return index;
} // int CR_DL::Histo_Index(const TString &sample_name)

//////////

TString CR_DL::Histo_Name_RF(const TString &sample_name)
{
  TString histo_name_rf;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    histo_name_rf = sample_name;

    if (histo_name_rf.Contains("WtoCB"))
      histo_name_rf.ReplaceAll("TTLJ_WtoCB", "TTLJ");

    if (chk_rf_tthf_breakdown)
    {
      bool chk_b = false;
      bool chk_c = false;

      // // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
      // for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
      // {
      //   int flavour = event.vec_sel_gen_hf_flavour->at(i);
      //   int origin = event.vec_sel_gen_hf_origin->at(i);

      //   if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      //   // if (flavour == 5 && abs(origin) == 21)
      //   {
      //     chk_b = true;
      //     break;
      //   }
      //   else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
      //     // else if (flavour == 4 && abs(origin) == 21)
      //     chk_c = true;
      // }

      if (chk_include_pseudo_additional)
      {
        if (51 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 56)
          chk_b = true;
        else if (41 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 46)
          chk_c = true;
      }
      else
      {
        if (51 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 55)
          chk_b = true;
        else if (41 <= event.gen_ttbar_id % 100 && event.gen_ttbar_id % 100 <= 45)
          chk_c = true;
      }

      if (chk_b)
        histo_name_rf += "_BB";
      else if (chk_c)
        histo_name_rf += "_CC";
      else
        histo_name_rf += "_JJ";
    } // if(chk_rf_tthf_breakdown)

    if (event.decay_mode == 21 || event.decay_mode == 23)
      histo_name_rf += "_2";
    else if (event.decay_mode == 41 || event.decay_mode == 43)
      histo_name_rf += "_4";
    else if (event.decay_mode == 45)
      histo_name_rf += "_45";
  }
  else
    histo_name_rf = samples.map_short_name_mc[sample_name];

  // cout << "test " << histo_name_rf << endl;

  return histo_name_rf;
} // TString CR_DL::Histo_name_RF(const TString &sample_name)

//////////

void CR_DL::Merge_PDF_Error_Set()
{
  // Merge histos for 100 pdf_error_sets
  cout << "Merging histograms for 100 pdf_error_sets" << endl;

  chk_merge_pdf_error_set = true;

  /*
    // stack
    stack_pdf_error_set = new THStack **[n_pdf_error_set];
    for (int i = 0; i < n_pdf_error_set; i++)
    {
      stack_pdf_error_set[i] = new THStack *[n_variable];

      int index;
      auto it = find(syst_name.begin(), syst_name.end(), "PDF_Error_Set_" + to_string(i));
      if (it != syst_name.end())
        index = distance(syst_name.begin(), it);
      else
      {
        cout << "Can't find the corresponding PDF_Error_Set. Check it." << endl;
        exit(1);
      }

      for (int j = 0; j < n_variable; j++)
      {
        TString stack_name = "PDF_Error_Set_" + to_string(i) + "_" + histo_type[j];
        stack_pdf_error_set[i][j] = new THStack(stack_name, stack_name);

        for (int k = 0; k < n_sample_merge_mc; k++)
        {
          stack_pdf_error_set[i][j]->Add(histo_mc[index][k][j]);
        } // loop  over n_sample_merge_mc
      }   // loop over n_variable
    }     // loop over n_pdf_error_set

    histo_pdf_error_set_down = new TH1D *[n_variable];
    histo_pdf_error_set_up = new Th1D *[n_variable];
    for (int i = 0; i < n_variable; i++)
    {
      TString histo_name_pdf_error = "PDF_Error_Set_" + histo_type[i];
      histo_pdf_error_set_down[i] = new TH1D(histo_name_pdf_error + "_Down", 16, 0, 16);
      histo_pdf_error_set_up[i] = new TH1D(histo_name_pdf_error + "_Up", 16, 0, 16);
    } // loop over n_variable

    // find min and max and fill histo
    for (int i = 0; i < n_variable; i++)
    {
      cout << histo_type[i] << endl;

      int n_bin = (bin_bvsc.size() - 1) * (bin_bvsc.size() - 1);
      for (int j = 0; j < n_bin; j++)
      {
        float max = -99999;
        float min = 99999;

        for (int k = 0; k < n_pdf_error_set; k++)
        {
          TH1D *histo_stacked = (TH1D *)(stack_pdf_error_set[k][i]->GetStack()->Last());
          float content = histo_stacked->GetBinContent(j + 1);

          if (content < min)
            min = content;
          if (max < content)
            max = content;
        } // loop over n_pdf_error_set

        histo_pdf_error_set_down[i]->SetBinContent(j + 1, min);
        histo_pdf_error_set_up[i]->SetBinContent(j + 1, max);
      } // loop over n_bin
    }   // loop over n_variable
  */

  histo_mc_pdf_error_set_down = new TH1D **[n_sample_merge_mc];
  histo_mc_pdf_error_set_up = new TH1D **[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    histo_mc_pdf_error_set_down[i] = new TH1D *[n_variable];
    histo_mc_pdf_error_set_up[i] = new TH1D *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name_pdf_error = "PDF_Error_Set_Down_" + vec_short_name_mc[i] + "_" + variable_conf[j].variable_title;
      histo_mc_pdf_error_set_down[i][j] = new TH1D(histo_name_pdf_error, histo_name_pdf_error, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up);

      histo_name_pdf_error = "PDF_Error_Set_Up_" + vec_short_name_mc[i] + "_" + variable_conf[j].variable_title;
      histo_mc_pdf_error_set_up[i][j] = new TH1D(histo_name_pdf_error, histo_name_pdf_error, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up);
    }
  }

  // find min and max and fill histo
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    // cout << vec_short_name_mc[i] << endl;
    for (int j = 0; j < n_variable; j++)
    {
      // cout << variable_conf[j].variable_title << endl;

      for (int k = 0; k < variable_conf[j].n_bin; k++)
      {
        float max = -99999;
        float min = 99999;

        for (int l = 0; l < n_pdf_error_set; l++)
        {
          int index;
          auto it = find(syst_name.begin(), syst_name.end(), "PDF_Error_Set_" + to_string(l));
          if (it != syst_name.end())
            index = distance(syst_name.begin(), it);
          else
          {
            cout << "Can't find the corresponding PDF_Error_Set. Check it." << endl;
            exit(1);
          }

          float content = histo_mc[index][i][j]->GetBinContent(k + 1);

          if (content < min)
            min = content;
          if (max < content)
            max = content;
        } // loop over n_pdf_error_set

        histo_mc_pdf_error_set_down[i][j]->SetBinContent(k + 1, min);
        histo_mc_pdf_error_set_up[i][j]->SetBinContent(k + 1, max);
      } // loop over n_bin
    } // loop over n_variable
  } // loop over n_sample_merge_mc

  return;
} // void CR_DL::Merge_PDF_Error_Set()

//////////

void CR_DL::Unroller()
{
  // unroll and remove empty bins
  cout << "[CR_DL::Unroller]: Init" << endl;

  // mc
  for (int i = 0; i < n_syst; i++)
  {
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      int bin_index = 1;
      for (int k = 0; k < histo_mc_2d[i][j][0]->GetNbinsX(); k++)
      {
        for (int l = 0; l < histo_mc_2d[i][j][0]->GetNbinsY(); l++)
        {
          if (k < l)
            continue;

          float content = histo_mc_2d[i][j][0]->GetBinContent(k + 1, l + 1);
          float error = histo_mc_2d[i][j][0]->GetBinError(k + 1, l + 1);

          // int unrolled_bin = (bin_bvsc.size() - 1) * k + l + 1;
          // histo_mc[i][j][0]->SetBinContent(unrolled_bin, content);
          // histo_mc[i][j][0]->SetBinError(unrolled_bin, error);
      

          histo_mc[i][j][0]->SetBinContent(bin_index, content);
          histo_mc[i][j][0]->SetBinError(bin_index, error);

          bin_index++;

        } // loop over nbiny
      } // loop over nbinx
    } // loop over n_sample_merge_mc
  } // loop over n_syst

  // data
  int bin_index = 1;
  for (int i = 0; i < histo_data_2d[0]->GetNbinsX(); i++)
  {
    for (int j = 0; j < histo_data_2d[0]->GetNbinsY(); j++)
    {
      if (i < j)
        continue;

      float content = histo_data_2d[0]->GetBinContent(i + 1, j + 1);
      float error = histo_data_2d[0]->GetBinError(i + 1, j + 1);

      // int unrolled_bin = (bin_bvsc.size() - 1) * i + j + 1;
      // histo_data[0]->SetBinContent(unrolled_bin, content);
      // histo_data[0]->SetBinError(unrolled_bin, error);

      histo_data[0]->SetBinContent(bin_index, content);
      histo_data[0]->SetBinError(bin_index, error);

      bin_index++;

    } // loop over nbiny
  } // loop over nbinx

  cout << "[CR_DL::Unroller]: Done" << endl;

  return;
} // void CR_DL::Unroller()

//////////

int CR_DL::Unroller(const float &bvsc_third, const float &bvsc_fourth)
{
  int bin_third = -1;
  for (unsigned int i = 0; i < bin_bvsc.size(); i++)
  {
    if (bvsc_third < bin_bvsc[i])
      break;

    bin_third++;
  }

  int bin_fourth = -1;
  for (unsigned int i = 0; i < bin_bvsc.size(); i++)
  {
    if (bvsc_fourth < bin_bvsc[i])
      break;

    bin_fourth++;
  }

  int bin_index = bin_third + (bin_bvsc.size() - 1) * bin_fourth + 1;

  return bin_index;
} // int CR_DL::Unroller(const float &bvsc_third, const float &bvsc_fourth)

//////////
