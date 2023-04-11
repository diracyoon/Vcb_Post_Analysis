#include "CR_DL.h"

ClassImp(CR_DL);

//////////

CR_DL::CR_DL(const TString &a_era, const TString &a_channel, const TString &a_run_flag)
    : samples(a_era, a_channel, "Vcb_DL"), tagging_rf(a_era)
{
  ROOT::EnableImplicitMT(4);

  cout << "[CR_DL::CR_DL]: Init analysis" << endl;

  reduction = 1;

  era = a_era;
  channel = a_channel;
  run_flag = a_run_flag;

  path_base = getenv("Vcb_Post_Analysis_WD");

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  key_base = "";
  tree_name = "/Result_Tree";

  if (run_flag != "Data")
  {
    for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
    {
      cout << it->first << endl;

      if (run_flag == "Central" ||
          run_flag == "CP5_Down" || run_flag == "CP5_Up" ||
          run_flag == "hDamp_Down" || run_flag == "hDamp_Up" ||
          run_flag == "mTop_171p5" || run_flag == "mTop_173p5" ||
          run_flag == "All")
      {
        map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);
        map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "Central" + tree_name);

        event.Setup_Tree(map_tree_mc[it->first], true);
      }

      if (run_flag == "Jet_En_Down" || run_flag == "All")
      {
        map_fin_mc_jec_down[it->first] = new TFile(result_path + "JetEnDown/" + it->second);
        map_tree_mc_jec_down[it->first] = (TTree *)map_fin_mc_jec_down[it->first]->Get(key_base + "JetEnDown" + tree_name);

        event.Setup_Tree(map_tree_mc_jec_down[it->first], false);
      }

      if (run_flag == "Jet_En_Up" || run_flag == "All")
      {
        map_fin_mc_jec_up[it->first] = new TFile(result_path + "JetEnUp/" + it->second);
        map_tree_mc_jec_up[it->first] = (TTree *)map_fin_mc_jec_up[it->first]->Get(key_base + "JetEnUp" + tree_name);

        event.Setup_Tree(map_tree_mc_jec_up[it->first], false);
      }

      if (run_flag == "Jet_Res_Down" || run_flag == "All")
      {
        map_fin_mc_jer_down[it->first] = new TFile(result_path + "JetResDown/" + it->second);
        map_tree_mc_jer_down[it->first] = (TTree *)map_fin_mc_jer_down[it->first]->Get(key_base + "JetResDown" + tree_name);

        event.Setup_Tree(map_tree_mc_jer_down[it->first], false);
      }

      if (run_flag == "Jet_Res_Up" || run_flag == "All")
      {
        map_fin_mc_jer_up[it->first] = new TFile(result_path + "JetResUp/" + it->second);
        map_tree_mc_jer_up[it->first] = (TTree *)map_fin_mc_jer_up[it->first]->Get(key_base + "JetResUp" + tree_name);

        event.Setup_Tree(map_tree_mc_jer_up[it->first], false);
      }

      if (channel != "MM")
      {
        if (run_flag == "El_En_Down" || run_flag == "All")
        {
          map_fin_mc_eec_down[it->first] = new TFile(result_path + "ElectronEnDown/" + it->second);
          map_tree_mc_eec_down[it->first] = (TTree *)map_fin_mc_eec_down[it->first]->Get(key_base + "ElectronEnDown" + tree_name);

          event.Setup_Tree(map_tree_mc_eec_down[it->first], false);
        }

        if (run_flag == "El_En_Up" || run_flag == "All")
        {
          map_fin_mc_eec_up[it->first] = new TFile(result_path + "ElectronEnUp/" + it->second);
          map_tree_mc_eec_up[it->first] = (TTree *)map_fin_mc_eec_up[it->first]->Get(key_base + "ElectronEnUp" + tree_name);

          event.Setup_Tree(map_tree_mc_eec_up[it->first], false);
        }

        if (run_flag == "El_Res_Down")
        {
          map_fin_mc_eer_down[it->first] = new TFile(result_path + "ElectronResDown/" + it->second);
          map_tree_mc_eer_down[it->first] = (TTree *)map_fin_mc_eer_down[it->first]->Get(key_base + "ElectronResDown" + tree_name);

          event.Setup_Tree(map_tree_mc_eer_down[it->first], false);
        }

        if (run_flag == "El_Res_Up" || run_flag == "All")
        {
          map_fin_mc_eer_up[it->first] = new TFile(result_path + "ElectronResUp/" + it->second);
          map_tree_mc_eer_up[it->first] = (TTree *)map_fin_mc_eer_up[it->first]->Get(key_base + "ElectronResUp" + tree_name);

          event.Setup_Tree(map_tree_mc_eer_up[it->first], false);
        }
      }
    }

    if (run_flag == "CP5_Down" || run_flag == "All")
    {
      map_fin_mc_cp5_down = map_fin_mc;
      map_fin_mc_cp5_down.erase("TTLJ_powheg");
      map_fin_mc_cp5_down["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_CP5down"]);

      map_tree_mc_cp5_down = map_tree_mc;
      map_tree_mc_cp5_down.erase("TTLJ_powheg");
      map_tree_mc_cp5_down["TTLJ_powheg"] = (TTree *)map_fin_mc_cp5_down["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);

      event.Setup_Tree(map_tree_mc_cp5_down["TTLJ_powheg"], false);
    }

    if (run_flag == "CP5_Up" || run_flag == "All")
    {
      map_fin_mc_cp5_up = map_fin_mc;
      map_fin_mc_cp5_up.erase("TTLJ_powheg");
      map_fin_mc_cp5_up["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_CP5up"]);

      map_tree_mc_cp5_up = map_tree_mc;
      map_tree_mc_cp5_up.erase("TTLJ_powheg");
      map_tree_mc_cp5_up["TTLJ_powheg"] = (TTree *)map_fin_mc_cp5_up["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);

      event.Setup_Tree(map_tree_mc_cp5_up["TTLJ_powheg"], false);
    }

    if (run_flag == "hDamp_Down" || run_flag == "All")
    {
      map_fin_mc_hdamp_down = map_fin_mc;
      map_fin_mc_hdamp_down.erase("TTLJ_powheg");
      map_fin_mc_hdamp_down["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_hdampdown"]);

      map_tree_mc_hdamp_down = map_tree_mc;
      map_tree_mc_hdamp_down.erase("TTLJ_powheg");
      map_tree_mc_hdamp_down["TTLJ_powheg"] = (TTree *)map_fin_mc_hdamp_down["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);

      event.Setup_Tree(map_tree_mc_hdamp_down["TTLJ_powheg"], false);
    }

    if (run_flag == "hDamp_Up" || run_flag == "All")
    {
      map_fin_mc_hdamp_up = map_fin_mc;
      map_fin_mc_hdamp_up.erase("TTLJ_powheg");
      map_fin_mc_hdamp_up["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_hdampup"]);

      map_tree_mc_hdamp_up = map_tree_mc;
      map_tree_mc_hdamp_up.erase("TTLJ_powheg");
      map_tree_mc_hdamp_up["TTLJ_powheg"] = (TTree *)map_fin_mc_hdamp_up["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);

      event.Setup_Tree(map_tree_mc_hdamp_up["TTLJ_powheg"], false);
    }

    if (run_flag == "mTop_171p5" || run_flag == "All")
    {
      map_fin_mc_mtop_171p5 = map_fin_mc;
      map_fin_mc_mtop_171p5.erase("TTLJ_powheg");
      map_fin_mc_mtop_171p5["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_mtop_171p5"]);

      map_tree_mc_mtop_171p5 = map_tree_mc;
      map_tree_mc_mtop_171p5.erase("TTLJ_powheg");
      map_tree_mc_mtop_171p5["TTLJ_powheg"] = (TTree *)map_fin_mc_mtop_171p5["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);

      event.Setup_Tree(map_tree_mc_mtop_171p5["TTLJ_powheg"], false);
    }

    if (run_flag == "mTop_173p5" || run_flag == "All")
    {
      map_fin_mc_mtop_173p5 = map_fin_mc;
      map_fin_mc_mtop_173p5.erase("TTLJ_powheg");
      map_fin_mc_mtop_173p5["TTLJ_powheg"] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_powheg_mtop_173p5"]);

      map_tree_mc_mtop_173p5 = map_tree_mc;
      map_tree_mc_mtop_173p5.erase("TTLJ_powheg");
      map_tree_mc_mtop_173p5["TTLJ_powheg"] = (TTree *)map_fin_mc_mtop_173p5["TTLJ_powheg"]->Get(key_base + "Central" + tree_name);

      event.Setup_Tree(map_tree_mc_mtop_173p5["TTLJ_powheg"], false);
    }

    // TFile and TTree handlers
    if (run_flag == "Central" || run_flag == "All")
    {
      map_map_fin_mc["Central"] = &map_fin_mc;
      map_map_tree_mc["Central"] = &map_tree_mc;
    }

    if (run_flag == "Jet_En_Down" || run_flag == "All")
    {
      map_map_fin_mc["Jet_En_Down"] = &map_fin_mc_jec_down;
      map_map_tree_mc["Jet_En_Down"] = &map_tree_mc_jec_down;
    }

    if (run_flag == "Jet_En_Up" || run_flag == "All")
    {
      map_map_fin_mc["Jet_En_Up"] = &map_fin_mc_jec_up;
      map_map_tree_mc["Jet_En_Up"] = &map_tree_mc_jec_up;
    }

    if (run_flag == "Jet_Res_Down" || run_flag == "All")
    {
      map_map_fin_mc["Jet_Res_Down"] = &map_fin_mc_jer_down;
      map_map_tree_mc["Jet_Res_Down"] = &map_tree_mc_jer_down;
    }

    if (run_flag == "Jet_Res_Up" || run_flag == "All")
    {
      map_map_fin_mc["Jet_Res_Up"] = &map_fin_mc_jer_up;
      map_map_tree_mc["Jet_Res_Up"] = &map_tree_mc_jer_up;
    }
    if (run_flag == "CP5_Down" || run_flag == "All")
    {
      map_map_fin_mc["CP5_Down"] = &map_fin_mc_cp5_down;
      map_map_tree_mc["CP5_Down"] = &map_tree_mc_cp5_down;
    }

    if (run_flag == "CP5_Up" || run_flag == "All")
    {
      map_map_fin_mc["CP5_Up"] = &map_fin_mc_cp5_up;
      map_map_tree_mc["CP5_Up"] = &map_tree_mc_cp5_up;
    }

    if (run_flag == "hDamp_Down" || run_flag == "All")
    {
      map_map_fin_mc["hDamp_Down"] = &map_fin_mc_hdamp_down;
      map_map_tree_mc["hDamp_Down"] = &map_tree_mc_hdamp_down;
    }

    if (run_flag == "hDamp_Up" || run_flag == "All")
    {
      map_map_fin_mc["hDamp_Up"] = &map_fin_mc_hdamp_up;
      map_map_tree_mc["hDamp_Up"] = &map_tree_mc_hdamp_up;
    }

    if (run_flag == "mTop_171p5" || run_flag == "All")
    {
      map_map_fin_mc["mTop_171p5"] = &map_fin_mc_mtop_171p5;
      map_map_tree_mc["mTop_171p5"] = &map_tree_mc_mtop_171p5;
    }

    if (run_flag == "mTop_173p5" || run_flag == "All")
    {
      map_map_fin_mc["mTop_173p5"] = &map_fin_mc_mtop_173p5;
      map_map_tree_mc["mTop_173p5"] = &map_tree_mc_mtop_173p5;
    }

    if (channel != "MM")
    {
      if (run_flag == "El_En_Down" || run_flag == "All")
      {
        map_map_fin_mc["El_En_Down"] = &map_fin_mc_eec_down;
        map_map_tree_mc["El_En_Down"] = &map_tree_mc_eec_down;
      }

      if (run_flag == "El_En_Up" || run_flag == "All")
      {
        map_map_fin_mc["El_En_Up"] = &map_fin_mc_eec_up;
        map_map_tree_mc["El_En_Up"] = &map_tree_mc_eec_up;
      }

      if (run_flag == "El_Res_Down" || run_flag == "All")
      {
        map_map_fin_mc["El_Res_Down"] = &map_fin_mc_eer_down;
        map_map_tree_mc["El_Res_Down"] = &map_tree_mc_eer_down;
      }

      if (run_flag == "El_Res_Up" || run_flag == "All")
      {
        map_map_fin_mc["El_Res_Up"] = &map_fin_mc_eer_up;
        map_map_tree_mc["El_Res_Up"] = &map_tree_mc_eer_up;
      }
    } // if (channel == "El")
  }   // if (run_flag != "Data")

  // data
  if (run_flag == "Data" || run_flag == "All")
  {
    result_path += "DATA/";
    for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
    {
      cout << it->first << endl;

      map_fin_data[it->first] = new TFile(result_path + it->second);
      map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get(key_base + "Central" + tree_name);
      event.Setup_Tree(map_tree_data[it->first], false);
    }
  } // if (run_flag == "Data" || run_flag == "All")

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    // cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL"));
  vec_short_name_mc.push_back("TTJJ");
  vec_short_name_mc.push_back("TTCC");
  vec_short_name_mc.push_back("TTBB");
  n_sample_merge_mc = vec_short_name_mc.size();
  // cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  // Systs
  syst_name = {"Nominal",
               "B_Tag_HF_Down", "B_Tag_HF_Up", "B_Tag_JES_Down", "B_Tag_JES_Up", "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up", "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up", "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up", "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up", "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up", "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up",
               "C_Tag_Extrap_Down", "C_Tag_Extrap_Up", "C_Tag_Interp_Down", "C_Tag_Interp_Up", "C_Tag_LHE_Scale_MuF_Down", "C_Tag_LHE_Scale_MuF_Up", "C_Tag_LHE_Scale_MuR_Down", "C_Tag_LHE_Scale_MuR_Up", "C_Tag_PS_FSR_Fixed_Down", "C_Tag_PS_FSR_Fixed_Up", "C_Tag_PS_ISR_Fixed_Down", "C_Tag_PS_ISR_Fixed_Up", "C_Tag_PU_Down", "C_Tag_PU_Up", "C_Tag_Stat_Down", "C_Tag_Stat_Up",
               "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up", "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up", "C_Tag_XSec_Br_Unc_WJets_C_Down", "C_Tag_XSec_Br_Unc_WJets_C_Up",
               "C_Tag_JER_Down", "C_Tag_JER_Up", "C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up",
               "Jet_En_Down", "Jet_En_Up",
               "Jet_Res_Down", "Jet_Res_Up",
               "PDF_Alternative", "PDF_As_Down", "PDF_As_Up",
               "Pileup_Down", "Pileup_Up",
               "Pileup_Veto_Down", "Pileup_Veto_Up",
               "Prefire_Down", "Prefire_Up",
               "Scale_Variation_1", "Scale_Variation_2", "Scale_Variation_3", "Scale_Variation_4", "Scale_Variation_6", "Scale_Variation_8",
               "Top_Pt_Reweight",
               "CP5_Down", "CP5_Up",
               "hDamp_Down", "hDamp_Up",
               "mTop_171p5", "mTop_173p5"};

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
    syst_name.push_back("El_En_Down");
    syst_name.push_back("El_En_Up");
    syst_name.push_back("El_Res_Down");
    syst_name.push_back("El_Res_Up");
  }
  else if (channel == "EE")
  {
    syst_name.push_back("El_Trig_Down");
    syst_name.push_back("El_Trig_Up");

    syst_name.push_back("El_Id_Down");
    syst_name.push_back("El_Id_Up");
    syst_name.push_back("El_Reco_Down");
    syst_name.push_back("El_Reco_Up");
    syst_name.push_back("El_En_Down");
    syst_name.push_back("El_En_Up");
    syst_name.push_back("El_Res_Down");
    syst_name.push_back("El_Res_Up");
  }

  for (int i = 0; i < sizeof(event.weight_pdf_error_set) / sizeof(float); i++)
  {
    TString pdf_error_set = "PDF_Error_Set_" + to_string(i);
    syst_name.push_back(pdf_error_set);
  }

  n_syst = syst_name.size();
  cout << "N_Syst = " << n_syst << endl;

  bin_bvsc.push_back(0);
  bin_bvsc.push_back(1);
  if (era == "2016preVFP")
  {
    bin_bvsc.push_back(bvsc_wp_l_2016preVFP);
    bin_bvsc.push_back(bvsc_wp_m_2016preVFP);
    bin_bvsc.push_back(bvsc_wp_t_2016preVFP);
  }
  else if (era == "2016postVFP")
  {
    bin_bvsc.push_back(bvsc_wp_l_2016postVFP);
    bin_bvsc.push_back(bvsc_wp_m_2016postVFP);
    bin_bvsc.push_back(bvsc_wp_t_2016postVFP);
  }
  else if (era == "2017")
  {
    bin_bvsc.push_back(bvsc_wp_l_2017);
    bin_bvsc.push_back(bvsc_wp_m_2017);
    bin_bvsc.push_back(bvsc_wp_t_2017);
  }
  if (era == "2018")
  {
    bin_bvsc.push_back(bvsc_wp_l_2018);
    bin_bvsc.push_back(bvsc_wp_m_2018);
    bin_bvsc.push_back(bvsc_wp_t_2018);
  }
  sort(bin_bvsc.begin(), bin_bvsc.end());
  int n_bin_bvsc = bin_bvsc.size() - 1;

  // set variables to be drawn

  variable_conf = {{"BvsC_3rd_4th_Jets_Unrolled", n_bin_bvsc * n_bin_bvsc, 0., (float)n_bin_bvsc * n_bin_bvsc},
                   {"DiLepton_Mass", 100, 0, 300}};
  n_variable = variable_conf.size();

  // histo for MC
  histo_mc = new TH1D ***[n_syst];
  // histo_mc_2d = new TH2D ***[n_syst];
  for (int i = 0; i < n_syst; i++)
  {
    histo_mc[i] = new TH1D **[n_sample_merge_mc];
    // histo_mc_2d[i] = new TH2D **[n_sample_merge_mc];
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      histo_mc[i][j] = new TH1D *[n_variable];
      // histo_mc_2d[i][j] = new TH2D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = syst_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        histo_mc[i][j][k] = new TH1D(histo_name, histo_name, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);

        // histo_name = syst_name[i] + "_" + vec_short_name_mc[j] + "_" + histo_type[k];
        // histo_mc_2d[i][j][k] = new TH2D(histo_name, histo_name, bin_bvsc.size() - 1, bin_bvsc.data(), bin_bvsc.size() - 1, bin_bvsc.data());
      }
    } // loop over n_sample_merge_mc
  }   // loop over n_syst

  // histo for data
  histo_data = new TH1D *[n_variable];
  // histo_data_2d = new TH2D *[n_variable];
  for (int i = 0; i < n_variable; i++)
  {
    TString histo_name = variable_conf[i].variable_title;
    histo_data[i] = new TH1D(histo_name, histo_name, variable_conf[i].n_bin, variable_conf[i].x_low, variable_conf[i].x_up);

    // histo_name = histo_type[i];
    // histo_data_2d[i] = new TH2D(histo_name, histo_name, bin_bvsc.size() - 1, bin_bvsc.data(), bin_bvsc.size() - 1, bin_bvsc.data());
  }

  TString fout_name = "Vcb_DL_Histos_" + era + "_" + channel;
  fout_name += ".root";
  fout = new TFile(fout_name, "RECREATE");
} // CR_DL(const TString &a_era = "2018", const TString &a_channel = "MM", const TString &a_run_flag = "All");

//////////

CR_DL::~CR_DL()
{
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
} // CR_DL::~CR_DL()

//////////

void CR_DL::Run()
{
  Read_Tree();
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
      cout << it->first << endl;

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      for (Long64_t i = 0; i < n_entries; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        Fill_Histo_MC(it->first, syst_fix);
      } // loop over entries
    }   // for (auto it = map_tree->begin(); it != map_tree->end(); it++)
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
      int region_index = 0;

      Fill_Histo_Data();
    }
  } // loop over data period

  return;
} // void CR_DL::Read_Tree()

//////////

void CR_DL::Fill_Histo_Data()
{
  int unrolled_bin = Unroller(event.bvsc_third, event.bvsc_fourth);

  histo_data[0]->Fill(unrolled_bin, 1.);
  histo_data[1]->Fill(event.dilepton_mass, 1.);

  // histo_data_2d[0]->Fill(event.bvsc_third, event.bvsc_fourth, 1.);

  return;
} // void CR_DL::Fill_Histo_Data()

//////////

void CR_DL::Fill_Histo_MC(const TString &sample_name, const TString &syst_fix)
{
  int unrolled_bin = Unroller(event.bvsc_third, event.bvsc_fourth);
  int histo_index = Histo_Index(sample_name);

  for (int i = 0; i < n_syst; i++)
  {
    TString syst_type = syst_name[i];

    // for central tree, don't consider the following syst
    if (syst_fix == "None" &&
        (syst_type == "Jet_En_Down" || syst_type == "Jet_En_Up" ||
         syst_type == "Jet_Res_Down" || syst_type == "Jet_Res_Up" ||
         syst_type == "El_En_Down" || syst_type == "El_En_Up" ||
         syst_type == "El_Res_Down" || syst_type == "El_Res_Up" ||
         syst_type == "CP5_Down" || syst_type == "CP5_Up" ||
         syst_type == "hDamp_Down" || syst_type == "hDamp_Up" ||
         syst_type == "mTop_171p5" || syst_type == "mTop_173p5"))
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
      event.weight *= event.weight_sl_trig_mu_down;
    else if (syst_type == "Mu_Trig_Up")
      event.weight *= event.weight_sl_trig_mu_up;
    else if (syst_type == "El_Trig_Up")
      event.weight *= event.weight_sl_trig_el_down;
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

    /*
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

        // b-tag renormalization factor
        TString short_name_mc = samples.map_short_name_mc[sample_name];
        TString b_tag_syst_name;
        if (syst_type.Contains("B_Tag"))
          b_tag_syst_name = syst_type;
        else
          b_tag_syst_name = "B_Tag_Nominal";

        b_tag_rf = tagging_rf.Get_Tagging_RF(short_name_mc, b_tag_syst_name, event.n_jet, event.ht);
        // cout << "test " << short_name_mc << " " << c_tag_syst_name << " " << event.n_pv << " " << c_tag_rf << endl;

        event.weight *= b_tag_rf;
    */

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
      event.weight *= event.weight_c_tag;

    // c-tag renormalization factor
    TString short_name_mc = samples.map_short_name_mc[sample_name];
    TString c_tag_syst_name;
    if (syst_type.Contains("C_Tag"))
      c_tag_syst_name = syst_type;
    else
      c_tag_syst_name = "C_Tag_Nominal";

    c_tag_rf = tagging_rf.Get_Tagging_RF(short_name_mc, c_tag_syst_name, event.n_pv);
    // cout << "test " << short_name_mc << " " << c_tag_syst_name << " " << event.n_pv << " " << c_tag_rf << endl;

    event.weight *= c_tag_rf;

    histo_mc[i][histo_index][0]->Fill(unrolled_bin, event.weight);
    histo_mc[i][histo_index][1]->Fill(event.dilepton_mass, event.weight);

    // histo_mc_2d[i][histo_index][0]->Fill(event.bvsc_third, event.bvsc_fourth, event.weight);

  } // loop over n_syst

  return;
} // void CR_DL::Fill_Histo_MC(const int &sample_index, const TString &syst_fix)

//////////

int CR_DL::Histo_Index(const TString &sample_name)
{
  int index = -999;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    if (event.process == 0)
      index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTJJ") - vec_short_name_mc.begin();
    else if (event.process == 1)
      index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTCC") - vec_short_name_mc.begin();
    else if (event.process == 2)
      index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTBB") - vec_short_name_mc.begin();
  }
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name]) - vec_short_name_mc.begin();

  return index;
} // int CR_DL::Histo_Index(const TString &sample_name)

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
    cout << vec_short_name_mc[i] << endl;
    for (int j = 0; j < n_variable; j++)
    {
      cout << variable_conf[j].variable_title << endl;

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
    }   // loop over n_variable
  }     // loop over n_sample_merge_mc

  return;
} // void CR_DL::Merge_PDF_Error_Set()

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
