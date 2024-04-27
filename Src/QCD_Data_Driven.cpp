#include "QCD_Data_Driven.h"

ClassImp(QCD_Data_Driven);

//////////

QCD_Data_Driven::QCD_Data_Driven(const TString &a_era, const TString &a_channel, const TString &a_run_flag, const TString &a_tagger)
    : samples(a_era, a_channel), event(a_era, a_channel), tagging_rf(a_era)
{
  cout << "[QCD_Data_Driven::QCD_Data_Driven]: Init analysis" << endl;

  reduction = 1000;

  era = a_era;
  channel = a_channel;
  tagger = a_tagger;
  run_flag = a_run_flag;

  path_base = getenv("Vcb_Post_Analysis_WD");

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    // since it is QCD data driven, QCD MC samples should be excluded
    if (it->first.Contains("QCD"))
      continue;

    cout << it->first << " " << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ"));
  vec_short_name_mc.push_back("TTLJ_2");    // TTLJ, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_4");    // TTLJ, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_CC_2"); // TTLJ+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_CC_4"); // TTLJ+cc, w->cd or w->cs
  vec_short_name_mc.push_back("TTLJ_BB_2"); // TTLJ+bb, w->ud or w->us
  vec_short_name_mc.push_back("TTLJ_BB_4"); // TTLJ+bb, w->cd or w->cs

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB"));
  vec_short_name_mc.push_back("TTLJ_45");    // TTLJ, w->cb
  vec_short_name_mc.push_back("TTLJ_CC_45"); // TTLJ+cc, w->cb
  vec_short_name_mc.push_back("TTLJ_BB_45"); // TTLJ+cc, w->cb

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL"));
  vec_short_name_mc.push_back("TTLL");    // TTLL+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLL_CC"); // TTLL+cc, w->ud or w->us
  vec_short_name_mc.push_back("TTLL_BB"); // TTLL+bb, w->cd or w->cs

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  // Systs
  syst_name = {"Nominal",
               "Jet_En_Down", "Jet_En_Up",
               "Jet_Res_Down", "Jet_Res_Up",
               "UE_Down", "UE_Up",
               "PDF_Alternative", "PDF_As_Down", "PDF_As_Up",
               "Pileup_Down", "Pileup_Up",
               "PU_Jet_Veto_Down", "PU_Jet_Veto_Up",
               "Prefire_Down", "Prefire_Up",
               "MuF_Down", "MuF_Up",
               "MuR_Down", "MuR_Up",
               //"MuF_MuR_Down", "MuR_MuR_Up",
               "FSR_Down", "FSR_Up",
               "ISR_Down", "ISR_Up",
               "Top_Pt_Reweight",
               "Trig_Down", "Trig_Up",
               "CP5_Down", "CP5_Up",
               "hDamp_Down", "hDamp_Up",
               "mTop_171p5", "mTop_173p5"};

  if (tagger == "B")
  {
    vector<TString> syst_tagger = {//"B_Tag_JES_Down", "B_Tag_JES_Up",
                                   "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up",
                                   "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up",
                                   "B_Tag_HF_Down", "B_Tag_HF_Up",
                                   "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up",
                                   "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up",
                                   "B_Tag_LF_Down", "B_Tag_LF_Up",
                                   "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up",
                                   "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up"};

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

  if (channel == "Mu")
  {
    syst_name.push_back("Mu_Id_Down");
    syst_name.push_back("Mu_Id_Up");
    syst_name.push_back("Mu_Iso_Down");
    syst_name.push_back("Mu_Iso_Up");
  }
  else if (channel == "El")
  {
    syst_name.push_back("El_Id_Down");
    syst_name.push_back("El_Id_Up");
    syst_name.push_back("El_Reco_Down");
    syst_name.push_back("El_Reco_Up");
    //   syst_name.push_back("El_En_Down");
    //   syst_name.push_back("El_En_Up");
    //   syst_name.push_back("El_Res_Down");
    //   syst_name.push_back("El_Res_Up");
  }

  for (int i = 0; i < sizeof(event.weight_pdf_error_set) / sizeof(float); i++)
  {
    TString pdf_error_set = "PDF_Error_Set_" + to_string(i);
    syst_name.push_back(pdf_error_set);
  }

  n_syst = syst_name.size();
  cout << "N_Syst = " << n_syst << endl;

  Init();
} //

//////////

QCD_Data_Driven::~QCD_Data_Driven()
{

  if (run_flag == "Data" || run_flag == "All")
  {
    cout << "Data" << endl;

    TDirectory *dir_data = fout->mkdir("Data");
    for (int i = 0; i < n_b_region; i++)
    {
      dir_data->cd();
      histo_2d_data[i]->Write();
    }
  } // if(run_flag == "Data" || run_flag == "All")

  // for MC
  for (int i = 0; i < n_syst; i++)
  {
    cout << "syst_name = " << syst_name[i] << endl;

    if (run_flag != "All" && run_flag != "Central" && run_flag != syst_name[i])
      continue;

    TDirectory *dir_syst = fout->mkdir(syst_name[i]);

    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      // cout << vec_short_name_mc[k] << endl;

      TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[j]);

      for (int k = 0; k < n_b_region; k++)
      {

      } // loop voer n_region
    }   // loop over sample
  }     // loop over syst
  cout << "MC done" << endl;

} // QCD_Data_Driven::~QCD_Data_Driven()

//////////

void QCD_Data_Driven::Run()
{
  Read_Tree();

  return;
} // void QCD_Data_Driven::Run()

//////////

void QCD_Data_Driven::Read_Tree()
{
  cout << "[QCD_Data_Driven::Read_Tree]: Start to read trees" << endl;

  // for MC
  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString syst_loop_name = it_map_map->first;
    cout << syst_loop_name << endl;

    TString syst_fix;
    if (syst_loop_name == "Central")
      syst_fix = "None";
    else
      syst_fix = it_map_map->first;

    map<TString, TTree *> *map_tree = it_map_map->second;

    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
      cout << it->first << endl;

      TString sample_name_short;
      if (it->first.Contains("TTLJ") || it->first.Contains("TTLL"))
        sample_name_short = it->first;
      else
        sample_name_short = samples.map_short_name_mc[it->first];

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      // split
      Long64_t init;
      Long64_t end;

      if (run_flag == "Central")
      {
        Long64_t step = n_entries / n_split;
        init = step * index_split;
        end = step * (index_split + 1);
        if (index_split + 1 == n_split)
          end = n_entries;

        cout << "N_Entries:" << n_entries << " N_Split:" << n_split << " Index_Split:" << index_split << " Step:" << step << " Init:" << init << " End:" << end << endl;
        cout << endl;
      }
      else
      {
        init = 0;
        end = n_entries;
      }

      for (Long64_t i = init; i < end; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        // event veto
        if (100 < TMath::Abs(event.weight_pdf_alternative))
        {
          cout << "strange weight_pdf_alternative is detected. " << syst_loop_name << ", " << i << ", weight_pdf_alternative = " << event.weight_pdf_alternative << endl;
          cout << "Continue" << endl;

          continue;
        }

        Fill_Histo_MC(sample_name_short, syst_fix);

      } // loop over n_entries
    }   // loop over map_tree_mc
  }     // loop over map_map_tree_mc

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

      Fill_Histo_Data();
    } // loop over n_entries
  }   //  loop over data period

  cout << "[QCD_Data_Driven::Read_Tree]: Done" << endl;

  return;
} // void QCD_Data_Driven::Read_Tree()

//////////

void QCD_Data_Driven::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)
{
  cout << "[Histo_Syst::Register_Top_Syst]:" << type << endl;

  map_fin_syst = map_fin_mc;
  map_tree_syst = map_tree_mc;

  map_fin_syst.erase("TTLJ");
  map_tree_syst.erase("TTLJ");

  map_fin_syst.erase("TTLL");
  map_tree_syst.erase("TTLL");

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";
  TString tree_name = "/Result_Tree";

  map_fin_syst["TTLJ_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_" + type]);
  map_tree_syst["TTLJ_" + type] = (TTree *)map_fin_syst["TTLJ_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLJ_" + type], Syst::Central);

  map_fin_syst["TTLL_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_" + type]);
  map_tree_syst["TTLL_" + type] = (TTree *)map_fin_syst["TTLL_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLL_" + type], Syst::Central);

  return;
} // void QCD_Data_Driven::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)

//////////

void QCD_Data_Driven::Init()
{
  cout << "[QCD_Data_Driven::Init]: Init" << endl;

  // job splitting for central and syst_reweight
  if (run_flag.Contains("Central"))
  {
    index_split = TString(run_flag[7]).Atoi();
    n_split = TString(run_flag[8]).Atoi();
    run_flag = "Central";
  }

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";
  TString tree_name = "/Result_Tree";

  if (run_flag != "Data")
  {
    for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
    {
      // since it's QCD data driven, QCD MC samples should be excluded
      if (it->first.Contains("QCD"))
        continue;

      cout << it->first << endl;

      map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);

      if (run_flag == "Central" ||
          run_flag == "CP5_Down" || run_flag == "CP5_Up" ||
          run_flag == "hDamp_Down" || run_flag == "hDamp_Up" ||
          run_flag == "mTop_171p5" || run_flag == "mTop_173p5" ||
          run_flag == "All")
      {
        map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "Central" + tree_name);
        event.Setup_Tree(map_tree_mc[it->first], Syst::Central, true);
      }

      if (run_flag == "Jet_En_Down" || run_flag == "All")
      {
        map_tree_mc_jec_down[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetEnDown" + tree_name);
        event.Setup_Tree(map_tree_mc_jec_down[it->first], Syst::JetEnDown, false);
      }

      if (run_flag == "Jet_En_Up" || run_flag == "All")
      {
        map_tree_mc_jec_up[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetEnUp" + tree_name);
        event.Setup_Tree(map_tree_mc_jec_up[it->first], Syst::JetEnUp, false);
      }

      if (run_flag == "Jet_Res_Down" || run_flag == "All")
      {
        map_tree_mc_jer_down[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetResDown" + tree_name);
        event.Setup_Tree(map_tree_mc_jer_down[it->first], Syst::JetResDown, false);
      }

      if (run_flag == "Jet_Res_Up" || run_flag == "All")
      {
        map_tree_mc_jer_up[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "JetResUp" + tree_name);
        event.Setup_Tree(map_tree_mc_jer_up[it->first], Syst::JetResUp, false);
      }

      if (run_flag == "UE_Down" || run_flag == "All")
      {
        map_tree_mc_ue_down[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "UEDown" + tree_name);
        event.Setup_Tree(map_tree_mc_ue_down[it->first], Syst::Central, false);
      }

      if (run_flag == "UE_Up" || run_flag == "All")
      {
        map_tree_mc_ue_up[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "UEUp" + tree_name);
        event.Setup_Tree(map_tree_mc_ue_up[it->first], Syst::Central, false);
      }
    }

    if (run_flag == "CP5_Down" || run_flag == "All")
      Register_Top_Syst(map_fin_mc_cp5_down, map_tree_mc_cp5_down, "CP5Down");

    if (run_flag == "CP5_Up" || run_flag == "All")
      Register_Top_Syst(map_fin_mc_cp5_up, map_tree_mc_cp5_up, "CP5Up");

    if (run_flag == "hDamp_Down" || run_flag == "All")
      Register_Top_Syst(map_fin_mc_hdamp_down, map_tree_mc_hdamp_down, "hdampDown");

    if (run_flag == "hDamp_Up" || run_flag == "All")
      Register_Top_Syst(map_fin_mc_hdamp_up, map_tree_mc_hdamp_up, "hdampUp");

    if (run_flag == "mTop_171p5" || run_flag == "All")
      Register_Top_Syst(map_fin_mc_mtop_171p5, map_tree_mc_mtop_171p5, "mtop171p5");

    if (run_flag == "mTop_173p5" || run_flag == "All")
      Register_Top_Syst(map_fin_mc_mtop_173p5, map_tree_mc_mtop_173p5, "mtop173p5");

    // TFile and TTree handlers
    if (run_flag == "Central" || run_flag == "All")
      map_map_tree_mc["Central"] = &map_tree_mc;

    if (run_flag == "Jet_En_Down" || run_flag == "All")
      map_map_tree_mc["Jet_En_Down"] = &map_tree_mc_jec_down;

    if (run_flag == "Jet_En_Up" || run_flag == "All")
      map_map_tree_mc["Jet_En_Up"] = &map_tree_mc_jec_up;

    if (run_flag == "Jet_Res_Down" || run_flag == "All")
      map_map_tree_mc["Jet_Res_Down"] = &map_tree_mc_jer_down;

    if (run_flag == "Jet_Res_Up" || run_flag == "All")
      map_map_tree_mc["Jet_Res_Up"] = &map_tree_mc_jer_up;

    if (run_flag == "UE_Down" || run_flag == "All")
      map_map_tree_mc["UE_Down"] = &map_tree_mc_ue_down;

    if (run_flag == "UE_Up" || run_flag == "All")
      map_map_tree_mc["UE_Up"] = &map_tree_mc_ue_up;

    if (run_flag == "CP5_Down" || run_flag == "All")
      map_map_tree_mc["CP5_Down"] = &map_tree_mc_cp5_down;

    if (run_flag == "CP5_Up" || run_flag == "All")
      map_map_tree_mc["CP5_Up"] = &map_tree_mc_cp5_up;

    if (run_flag == "hDamp_Down" || run_flag == "All")
      map_map_tree_mc["hDamp_Down"] = &map_tree_mc_hdamp_down;

    if (run_flag == "hDamp_Up" || run_flag == "All")
      map_map_tree_mc["hDamp_Up"] = &map_tree_mc_hdamp_up;

    if (run_flag == "mTop_171p5" || run_flag == "All")
      map_map_tree_mc["mTop_171p5"] = &map_tree_mc_mtop_171p5;

    if (run_flag == "mTop_173p5" || run_flag == "All")
      map_map_tree_mc["mTop_173p5"] = &map_tree_mc_mtop_173p5;
  } // if (run_flag != "Data")

  // data
  if (run_flag == "Data" || run_flag == "All")
  {
    result_path += "DATA/";
    for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
    {
      cout << it->first << endl;

      if (it == samples.map_data.begin())
        data_short_name = samples.map_short_name_data[it->first];

      map_fin_data[it->first] = new TFile(result_path + it->second);
      map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_data[it->first], Syst::Central, false);
    }
  } // if (run_flag == "Data" || run_flag == "All")

  // histo for MC

  // 2d histo for MC
  histo_2d_mc = new TH2D ***[n_syst];
  for (int i = 0; i < n_syst; i++)
  {
    histo_2d_mc[i] = new TH2D **[n_sample_merge_mc];
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      histo_2d_mc[i][j] = new TH2D *[n_b_region];
      for (int k = 0; k < n_b_region; k++)
      {
        TString histo_name = syst_name[i] + "_" + vec_short_name_mc[j] + "_2D_" + to_string(k + 2);
        histo_2d_mc[i][j][k] = new TH2D(histo_name, histo_name, 100, 0, 400, 100, 0, 1);
      }
    }
  }

  // histo for data
  // n_region-1 to exclude signal region

  // 2d histo for data
  histo_2d_data = new TH2D *[n_b_region];
  for (int i = 0; i < n_b_region; i++)
  {
    TString histo_name = "Data_2D_" + to_string(i + 2);
    histo_2d_data[i] = new TH2D(histo_name, histo_name, 100, 0, 400, 100, 0, 1);
  }

  TString fout_name = "Vcb_QCD_Data_Driven_" + era + "_" + channel + "_";
  if (run_flag == "Central")
    fout_name += run_flag + to_string(index_split) + to_string(n_split);
  else
    fout_name += run_flag;
  fout_name += ".root";
  fout = new TFile(fout_name, "RECREATE");

  cout << "[QCD_Data_Driven::Init]: Done" << endl;

  return;
} // void QCD_Data_Driven::Init()

//////////

int QCD_Data_Driven::Set_B_Region()
{
  if (event.n_bjets == 2)
    return 0;
  else
    return 1;

  return -1;
} // int QCD_Data_Driven::Set_B_Region()

//////////

int QCD_Data_Driven::Set_Region()
{
  float rel_iso_cut;
  if (channel == "Mu")
    rel_iso_cut = REL_ISO_MUON;
  else if (channel == "El")
    rel_iso_cut = REL_ISO_ELECTRON;

  if (MET_PT < event.met_pt)
  {
    if (event.lepton_rel_iso < rel_iso_cut)
      return 3; //"D"
    else
      return 1; //"B"
  }
  else
  {
    if (event.lepton_rel_iso < rel_iso_cut)
      return 2; //"C"
    else
      return 0; //"A"
  }

  return -1;
} // TString QCD_Data_Driven::Set_Region()

//////////

void QCD_Data_Driven::Fill_Histo_Data()
{
  int region_index = Set_Region();
  int b_region_index = Set_B_Region();

  histo_2d_data[b_region_index]->Fill(event.met_pt, event.lepton_rel_iso);

  return;
} // void QCD_Data_Driven::Fill_Histo_Data()

//////////

void QCD_Data_Driven::Fill_Histo_MC(const TString &sample_name, const TString &syst_fix)
{
  int histo_index = Histo_Index(sample_name);
  TString histo_name_rf = Histo_Name_RF(sample_name);

  for (int i = 0; i < n_syst; i++)
  {
    TString syst_type = syst_name[i];

    // for central tree, don't consider the following syst
    if (syst_fix == "None" &&
        (syst_type == "Jet_En_Down" || syst_type == "Jet_En_Up" ||
         syst_type == "Jet_Res_Down" || syst_type == "Jet_Res_Up" ||
         syst_type == "El_En_Down" || syst_type == "El_En_Up" ||
         syst_type == "El_Res_Down" || syst_type == "El_Res_Up" ||
         syst_type == "UE_Down" || syst_type == "UE_Up" ||
         syst_type == "CP5_Down" || syst_type == "CP5_Up" ||
         syst_type == "hDamp_Down" || syst_type == "hDamp_Up" ||
         syst_type == "mTop_171p5" || syst_type == "mTop_173p5"))
      continue;

    // if syst_fix is fixed, only nominal is considered
    if (syst_fix != "None" && syst_fix != syst_type)
      continue;

    int region_index = Set_Region();
    int b_region_index = Set_B_Region();

    event.weight = 1;
    event.weight *= event.weight_lumi;
    event.weight *= event.weight_mc;
    event.weight *= event.weight_hem_veto;

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
    if (syst_type == "FSR_Down")
      event.weight *= event.weight_ps[0];
    else if (syst_type == "FSR_Up")
      event.weight *= event.weight_ps[1];
    else if (syst_type == "ISR_Down")
      event.weight *= event.weight_ps[2];
    else if (syst_type == "ISR_Up")
      event.weight *= event.weight_ps[3];

    // scale variation
    if (syst_type == "MuF_Down")
      event.weight *= event.weight_scale_variation_2;
    else if (syst_type == "MuF_Up")
      event.weight *= event.weight_scale_variation_1;
    else if (syst_type == "MuR_Down")
      event.weight *= event.weight_scale_variation_6;
    else if (syst_type == "MuR_Up")
      event.weight *= event.weight_scale_variation_3;
    else if (syst_type == "MuF_MuR_Down")
      event.weight *= event.weight_scale_variation_8;
    else if (syst_type == "MuF_MuR_Up")
      event.weight *= event.weight_scale_variation_4;

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

    // pileup jet veto id
    if (syst_type == "PU_Jet_Veto_Down")
      event.weight *= event.weight_pujet_veto_down;
    else if (syst_type == "PU_Jet_Veto_Up")
      event.weight *= event.weight_pujet_veto_up;
    else
      event.weight *= event.weight_pujet_veto;

    if (syst_type == "Top_Pt_Reweight")
      event.weight *= 1;
    else
      event.weight *= event.weight_top_pt;

    if (syst_type == "Trig_Down")
      event.weight *= event.weight_sl_trig_down;
    else if (syst_type == "Trig_Up")
      event.weight *= event.weight_sl_trig_up;
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

      if (syst_type == "B_Tag_CFErr1_Down")
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
      else if (syst_type == "B_Tag_HF_Down")
      {
        event.weight *= event.weight_b_tag_hf_down;
        b_tagging_rf_type = syst_type;
      }
      else if (syst_type == "B_Tag_HF_Up")
      {
        event.weight *= event.weight_b_tag_hf_up;
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
      // fully correlated c-tagging scale factors
      else if (syst_type == "Jet_En_Down")
      {
        event.weight *= event.weight_b_tag_jes_down;
        b_tagging_rf_type = "B_Tag_JES_Down";
      }
      else if (syst_type == "Jet_En_Up")
      {
        event.weight *= event.weight_b_tag_jes_up;
        b_tagging_rf_type = "B_Tag_JES_Up";
      }
      else
      {
        event.weight *= event.weight_b_tag;
        b_tagging_rf_type = "B_Tag_Nominal";
      }

      // b-tag renormalization factor
      event.weight *= tagging_rf.Get_Tagging_RF_B_Tag(region_name[region_index], histo_name_rf, b_tagging_rf_type, event.n_jets, event.ht);
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
      // fully correlated c-tagging scale factors
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
      else if (syst_type == "Jet_En_Down")
      {
        event.weight *= event.weight_c_tag_jes_total_down;
        c_tagging_rf_type = "C_Tag_JES_Total_Down";
      }
      else if (syst_type == "Jet_En_Up")
      {
        event.weight *= event.weight_c_tag_jes_total_up;
        c_tagging_rf_type = "C_Tag_JES_Total_Up";
      }
      else if (syst_type == "MuF_Down")
      {
        event.weight *= event.weight_c_tag_lhe_scale_muf_down;
        c_tagging_rf_type = "C_Tag_LHE_Scale_MuF_Down";
      }
      else if (syst_type == "MuF_Up")
      {
        event.weight *= event.weight_c_tag_lhe_scale_muf_up;
        c_tagging_rf_type = "C_Tag_LHE_Scale_MuF_Up";
      }
      else if (syst_type == "MuR_Down")
      {
        event.weight *= event.weight_c_tag_lhe_scale_mur_down;
        c_tagging_rf_type = "C_Tag_LHE_Scale_MuR_Down";
      }
      else if (syst_type == "MuR_Up")
      {
        event.weight *= event.weight_c_tag_lhe_scale_mur_up;
        c_tagging_rf_type = "C_Tag_LHE_Scale_MuR_Up";
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
      else if (syst_type == "ISR_Down")
      {
        event.weight *= event.weight_c_tag_ps_isr_fixed_down;
        c_tagging_rf_type = "C_Tag_PS_ISR_Fixed_Down";
      }
      else if (syst_type == "ISR_Up")
      {
        event.weight *= event.weight_c_tag_ps_isr_fixed_up;
        c_tagging_rf_type = "C_Tag_PS_ISR_Fixed_Up";
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

      // c-tag renormalization factor
      event.weight *= tagging_rf.Get_Tagging_RF_C_Tag(region_name[region_index], histo_name_rf, c_tagging_rf_type, event.n_pv, event.ht);
    } // else if (tagger == "C")
 
    histo_2d_mc[i][histo_index][b_region_index]->Fill(event.met_pt, event.lepton_rel_iso, event.weight);
  } // loop over n_syst

  return;
} // void QCD_Data_Driven::Fill_Histo_MC()

//////////

int QCD_Data_Driven::Histo_Index(const TString &sample_name)
{
  int index = -999;

  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")))
  {
    bool chk_b = false;
    bool chk_c = false;

    // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    {
      int flavour = event.vec_sel_gen_hf_flavour->at(i);
      int origin = event.vec_sel_gen_hf_origin->at(i);

      if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      {
        chk_b = true;
        break;
      }
      else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
        chk_c = true;
    }

    TString histo_name = sample_name;
    if (histo_name.Contains("WtoCB"))
      histo_name = "TTLJ";

    if (histo_name.Contains("CP5") || histo_name.Contains("hdamp") || histo_name.Contains("mtop"))
    {
      if (histo_name.Contains("TTLJ"))
        histo_name = "TTLJ";
      else if (histo_name.Contains("TTLL"))
        histo_name = "TTLL";
    }

    if (chk_b)
      histo_name += "_BB";
    else if (chk_c)
      histo_name += "_CC";

    if (event.decay_mode == 21 || event.decay_mode == 23)
      histo_name += "_2";
    else if (event.decay_mode == 41 || event.decay_mode == 43)
      histo_name += "_4";
    else if (event.decay_mode == 45)
      histo_name += "_45";

    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();

  // cout << "test Histo_Index: " << sample_name << " " << index << endl;

  return index;
} // int QCD_Data_Driven::Histo_Index(const TString& sample_name)

//////////

TString QCD_Data_Driven::Histo_Name_RF(const TString &sample_name)
{
  TString histo_name_rf;
  // = samples.map_short_name_mc[sample_name];

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    histo_name_rf = sample_name;

    bool chk_b = false;
    bool chk_c = false;

    // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    {
      int flavour = event.vec_sel_gen_hf_flavour->at(i);
      int origin = event.vec_sel_gen_hf_origin->at(i);

      if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      {
        chk_b = true;
        break;
      }
      else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
        chk_c = true;
    }

    if (histo_name_rf.Contains("WtoCB"))
      histo_name_rf = "TTLJ";

    if (chk_b)
      histo_name_rf += "_BB";
    else if (chk_c)
      histo_name_rf += "_CC";

    if (event.decay_mode == 21 || event.decay_mode == 23)
      histo_name_rf += "_2";
    else if (event.decay_mode == 41 || event.decay_mode == 43)
      histo_name_rf += "_4";
    else if (event.decay_mode == 45)
      histo_name_rf += "_45";
  }
  else
    histo_name_rf = samples.map_short_short_name[sample_name];

  return histo_name_rf;
} // TString Histo_Syst::Histo_name_RF(const TString &sample_name)

//////////