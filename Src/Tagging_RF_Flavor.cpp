#include "Tagging_RF_Flavor.h"

ClassImp(Tagging_RF_Flavor);

//////////

Tagging_RF_Flavor::Tagging_RF_Flavor(const TString &a_era, const TString &a_mode, const TString &a_channel, const TString &a_tagger, const int &a_index_tree_type, const int &a_last_index_tree_type, const TString &a_extension)
    : samples(a_era, a_channel, "Vcb_Tagging_RF")
{
  cout << "[Tagging_RF::Tagging_RF]: Init analysis" << endl;

  TH1::AddDirectory(kFALSE);

  reduction = 1;

  era = a_era;
  mode = a_mode;
  channel = a_channel;
  tagger = a_tagger;
  extension = a_extension;

  index_tree_type = a_index_tree_type;
  last_index_tree_type = a_last_index_tree_type;

  if (tagger.Contains("C"))
    tagger = "C_Tagger";
  else if (tagger.Contains("B"))
    tagger = "B_Tagger";

  chk_tthf_breakdown = false;
  chk_jes_breakdown = false;

  // syst_tree
  vec_tree_type = {"JetResDown", "JetResUp",
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
  } // if (chk_jes_breakdown == true)

  // Central should be the last
  vec_tree_type.push_back("Central");

  n_tree_type = vec_tree_type.size();

  syst_name_b = {"B_Tag_Nominal",
                 "B_Tag_HF_Down", "B_Tag_HF_Up",
                 "B_Tag_LF_Down", "B_Tag_LF_Up",
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

  if (chk_jes_breakdown == true)
  {
    vector<TString> syst_name_c_jes_breakdown = {"C_Tag_JES_Absolute_Down", "C_Tag_JES_Absolute_Up",
                                                 "C_Tag_JES_BBEC1_Down", "C_Tag_JES_BBEC1_Up",
                                                 "C_Tag_JES_EC2_Down", "C_Tag_JES_EC2_Up",
                                                 "C_Tag_JES_FlavorQCD_Down", "C_Tag_JES_FlavorQCD_Up",
                                                 "C_Tag_JES_HF_Down", "C_Tag_JES_HF_Up",
                                                 "C_Tag_JES_RelativeBal_Down", "C_Tag_JES_RelativeBal_Up"};

    vector<TString> syst_name_c_jes_breakdown_year = {"C_Tag_JES_Absolute", "C_Tag_JES_BBEC1", "C_Tag_JES_EC2", "C_Tag_JES_HF", "C_Tag_JES_RelativeSample"};
    for (unsigned int i = 0; i < syst_name_c_jes_breakdown_year.size(); i++)
    {
      syst_name_c_jes_breakdown.push_back(syst_name_c_jes_breakdown_year[i] + year + "_Down");
      syst_name_c_jes_breakdown.push_back(syst_name_c_jes_breakdown_year[i] + year + "_Up");
    }

    syst_name_c.insert(syst_name_c.end(), syst_name_c_jes_breakdown.begin(), syst_name_c_jes_breakdown.end());
  }
  n_syst_c = syst_name_c.size();

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    // cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  if (chk_tthf_breakdown)
  {
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
    vec_short_name_mc.push_back("TTLJ_BB_45"); // TTLJ+bb, w->cb

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL"));
    vec_short_name_mc.push_back("TTLL");    // TTLL+cc, w->ud or w->us
    vec_short_name_mc.push_back("TTLL_CC"); // TTLL+cc, w->ud or w->us
    vec_short_name_mc.push_back("TTLL_BB"); // TTLL+bb, w->cd or w->cs

    // TT systematic
    // TTLJ
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_hdampDown"));
    vec_short_name_mc.push_back("TTLJ_hdampDown_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_4");
    vec_short_name_mc.push_back("TTLJ_hdampDown_CC_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_CC_4");
    vec_short_name_mc.push_back("TTLJ_hdampDown_BB_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_hdampUp"));
    vec_short_name_mc.push_back("TTLJ_hdampUp_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_4");
    vec_short_name_mc.push_back("TTLJ_hdampUp_CC_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_CC_4");
    vec_short_name_mc.push_back("TTLJ_hdampUp_BB_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_CP5Down"));
    vec_short_name_mc.push_back("TTLJ_CP5Down_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_4");
    vec_short_name_mc.push_back("TTLJ_CP5Down_CC_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_CC_4");
    vec_short_name_mc.push_back("TTLJ_CP5Down_BB_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_CP5Up"));
    vec_short_name_mc.push_back("TTLJ_CP5Up_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_4");
    vec_short_name_mc.push_back("TTLJ_CP5Up_CC_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_CC_4");
    vec_short_name_mc.push_back("TTLJ_CP5Up_BB_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_mtop171p5"));
    vec_short_name_mc.push_back("TTLJ_mtop171p5_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_4");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_CC_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_CC_4");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_BB_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_mtop173p5"));
    vec_short_name_mc.push_back("TTLJ_mtop173p5_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_4");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_CC_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_CC_4");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_BB_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_BB_4");

    // TTLL
    // vec_short_name_mc.push_back("TTLL_hdampDown");
    vec_short_name_mc.push_back("TTLL_hdampDown_CC");
    vec_short_name_mc.push_back("TTLL_hdampDown_BB");

    // vec_short_name_mc.push_back("TTLL_hdampUp");
    vec_short_name_mc.push_back("TTLL_hdampUp_CC");
    vec_short_name_mc.push_back("TTLL_hdampUp_BB");

    // vec_short_name_mc.push_back("TTLL_CP5Down");
    vec_short_name_mc.push_back("TTLL_CP5Down_CC");
    vec_short_name_mc.push_back("TTLL_CP5Down_BB");

    // vec_short_name_mc.push_back("TTLL_CP5Up");
    vec_short_name_mc.push_back("TTLL_CP5Up_CC");
    vec_short_name_mc.push_back("TTLL_CP5Up_BB");

    // vec_short_name_mc.push_back("TTLL_mtop171p5");
    vec_short_name_mc.push_back("TTLL_mtop171p5_CC");
    vec_short_name_mc.push_back("TTLL_mtop171p5_BB");

    // vec_short_name_mc.push_back("TTLL_mtop173p5");
    vec_short_name_mc.push_back("TTLL_mtop173p5_CC");
    vec_short_name_mc.push_back("TTLL_mtop173p5_BB");

    // TTLJ_WtoCB
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_hdampDown"));
    vec_short_name_mc.push_back("TTLJ_hdampDown_45");
    vec_short_name_mc.push_back("TTLJ_hdampDown_CC_45");
    vec_short_name_mc.push_back("TTLJ_hdampDown_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_hdampUp"));
    vec_short_name_mc.push_back("TTLJ_hdampUp_45");
    vec_short_name_mc.push_back("TTLJ_hdampUp_CC_45");
    vec_short_name_mc.push_back("TTLJ_hdampUp_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_CP5Down"));
    vec_short_name_mc.push_back("TTLJ_CP5Down_45");
    vec_short_name_mc.push_back("TTLJ_CP5Down_CC_45");
    vec_short_name_mc.push_back("TTLJ_CP5Down_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_CP5Up"));
    vec_short_name_mc.push_back("TTLJ_CP5Up_45");
    vec_short_name_mc.push_back("TTLJ_CP5Up_CC_45");
    vec_short_name_mc.push_back("TTLJ_CP5Up_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_mtop171p5"));
    vec_short_name_mc.push_back("TTLJ_mtop171p5_45");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_CC_45");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_mtop173p5"));
    vec_short_name_mc.push_back("TTLJ_mtop173p5_45");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_CC_45");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_BB_45");
  }
  else
  {
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ"));
    vec_short_name_mc.push_back("TTLJ_2"); // TTLJ, w->ud or w->us
    vec_short_name_mc.push_back("TTLJ_4"); // TTLJ, w->cd or w->cs

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB"));
    vec_short_name_mc.push_back("TTLJ_45"); // TTLJ, w->cb

    // TT systematic
    // TTLJ
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_hdampDown"));
    vec_short_name_mc.push_back("TTLJ_hdampDown_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_hdampUp"));
    vec_short_name_mc.push_back("TTLJ_hdampUp_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_CP5Down"));
    vec_short_name_mc.push_back("TTLJ_CP5Down_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_CP5Up"));
    vec_short_name_mc.push_back("TTLJ_CP5Up_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_mtop171p5"));
    vec_short_name_mc.push_back("TTLJ_mtop171p5_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_mtop173p5"));
    vec_short_name_mc.push_back("TTLJ_mtop173p5_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_4");

    // TTLJ_WtoCB
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_hdampDown"));
    vec_short_name_mc.push_back("TTLJ_hdampDown_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_hdampUp"));
    vec_short_name_mc.push_back("TTLJ_hdampUp_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_CP5Down"));
    vec_short_name_mc.push_back("TTLJ_CP5Down_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_CP5Up"));
    vec_short_name_mc.push_back("TTLJ_CP5Up_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_mtop171p5"));
    vec_short_name_mc.push_back("TTLJ_mtop171p5_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_mtop173p5"));
    vec_short_name_mc.push_back("TTLJ_mtop173p5_45");
  }

  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  if (mode == "Analysis")
    Run_Analysis();
  else if (mode == "Ratio")
    Run_Ratio();
  else if (mode == "Combine")
    Run_Combine();
  else if (mode == "Validation")
    Run_Validation();
  else if (mode == "Draw_Validation")
    Run_Draw_Validation();
  else if (mode == "Application")
    Setup_Application();
  else
  {
    cout << "Wrong mode. Check it." << endl;
    exit(1);
  }
} // Tagging_RF_Flavor::Tagging_RF_Flavor(const TString &a_era, const TString &a_mode, const TString &a_channel, const TString &a_tagger, const int &a_index_tree_type, const int &a_last_index_tree_type, const TString &a_extension)

//////////

Tagging_RF_Flavor::~Tagging_RF_Flavor()
{
  cout << "[Tagging_RF_Flavor::~Tagging_RF_Flavor]: Init" << endl;

  if (mode == "Analysis")
  {
    TString fout_name;
    if (index_tree_type == -1)
      fout_name = "Vcb_Tagging_RF_Flavor_" + era + "_" + channel + ".root";
    else
    {
      if (vec_tree_type[0] == "Central")
        fout_name = "Vcb_Tagging_RF_Flavor_" + era + "_" + channel + "_" + vec_tree_type[0] + index_split + n_split + ".root";
      else
        fout_name = "Vcb_Tagging_RF_Flavor_" + era + "_" + channel + "_" + vec_tree_type[0] + ".root";
    }
    fout = new TFile(fout_name, "RECREATE");

    fout->cd();

    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = fout->mkdir(region_name[i]);
      dir_region->cd();

      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_region->mkdir(vec_short_name_mc[j]);
        dir_sample->cd();

        for (int k = 0; k < n_syst_c; k++)
        {
          for (int l = 0; l < n_flavor; l++)
          {
            histo_mc_before_c[i][j][k][l]->Write();
            histo_mc_after_c[i][j][k][l]->Write();

            // ratio_c[i][j][k][l]->Write();
          } // loop over n_flavor
        } // loop over n_syst_c
      } // loop over n_sample
    } // loop over n_region

    fout->Close();
  } // if (mode=="Analysis")
  else if (mode == "Ratio")
  {

    for (int i = 0; i < n_region; i++)
    {

      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TString dir_name = region_name[i] + "/" + vec_short_name_mc[j];
        TDirectory *dir_sample = (TDirectory *)fin->Get(dir_name);
        dir_sample->cd();

        for (int k = 0; k < n_syst_c; k++)
        {
          for (int l = 0; l < n_flavor; l++)
          {
            // existence check
            TString ratio_name = ratio_c[i][j][k][l]->GetName();
            cout << ratio_name << endl;

            if (dir_sample->Get(ratio_name))
              dir_sample->Delete(ratio_name);

            ratio_c[i][j][k][l]->Write();
          } // loop over n_flavor
        } // loop over n_syst_c
      } // loop over n_sample
    } // loop over n_region

    fin->Close();
  } // else if (mode == "Ratio")
  else if (mode == "Combine")
  {
    fout->Close();
  } // else if (mode == "Combine")
  else if (mode == "Validation")
  {
    TString fout_name;
    if (index_tree_type == -1)
      fout_name = "Vcb_Tagging_RF_Flavor_Validation_" + era + "_" + channel + "_" + tagger + ".root";
    else
      fout_name = "Vcb_Tagging_RF_Flavor_Validation_" + era + "_" + channel + "_" + tagger + "_" + vec_tree_type[0] + "_" + index_split + n_split + ".root";
    fout = new TFile(fout_name, "RECREATE");

    fout->cd();
    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = fout->mkdir(region_name[i]);

      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_region->mkdir(vec_short_name_mc[j]);
        dir_sample->cd();

        int n_syst;
        if (tagger == "B_Tagger")
          n_syst = n_syst_b;
        else if (tagger == "C_Tagger")
          n_syst = n_syst_c;

        for (int k = 0; k < n_syst; k++)
        {
          for (int l = 0; l < n_flavor; l++)
          {
            for (int m = 0; m < 3; m++)
            {
              dir_sample->cd();

              histo_closure_bvsc[i][j][k][l][m]->Write();
              histo_closure_cvsb[i][j][k][l][m]->Write();
              histo_closure_cvsl[i][j][k][l][m]->Write();

              histo_closure_eta[i][j][k][l][m]->Write();
              histo_closure_pt[i][j][k][l][m]->Write();
            } // loop over 3
          } // loop over n_flavor
        } // loop over n_syst
      } // loop over n_sample

      TDirectory *dir_rf_distribution = dir_region->mkdir("RF_Distribution");
      dir_rf_distribution->cd();
      for (int j = 0; j < 2; j++)
      {
        for (int k = 0; k < n_syst_c; k++)
        {
          histo_rf[i][j][k]->Write();
        } // loop over n_syst_c
      } // loop over 2
    } // loop over n_region

    fout->Close();
  } // else if (mode == "Validation")

  cout << "[Tagging_RF_Flavor::~Tagging_RF_Flavor]: Done" << endl;
}

//////////

float Tagging_RF_Flavor::Get_Tagging_RF_C_Tag(const TString &region, const TString &sample, const TString &syst, const vector<float> *vec_jet_pt, const vector<float> *vec_jet_eta, const vector<int> *vec_jet_flavor)
{
  if (!syst.Contains("C_Tag"))
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Input C_Tag" << endl;
    exit(1);
  }

  int region_index = find(region_name.begin(), region_name.end(), region) - region_name.begin();
  int sample_index = find(vec_sample_tagging_rf.begin(), vec_sample_tagging_rf.end(), sample) - vec_sample_tagging_rf.begin();
  int syst_index = find(syst_name_c.begin(), syst_name_c.end(), syst) - syst_name_c.begin();

  if (region_index == region_name.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF_C_Tag] Can not find " << region << " Check it." << endl;
    exit(1);
  }

  if (sample_index == vec_sample_tagging_rf.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF_C_Tag] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_c.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  float weight_rf = 1;
  for (unsigned i = 0; i < vec_jet_pt->size(); i++)
  {
    float pt = vec_jet_pt->at(i);
    float eta = vec_jet_eta->at(i);

    // handle underflow
    if (pt < bin_pt[0])
      pt = bin_pt[0] + 0.1;
    if (eta < bin_eta[0])
      ht = bin_eta[0] + 0.01;

    // handle overflow
    if (bin_pt[bin_pt.size() - 1] < pt)
      pt = bin_pt[bin_pt.size() - 1] - 0.1;
    if (bin_eta[bin_eta.size() - 1] < eta)
      eta = bin_eta[bin_eta.size() - 1] - 0.01;

    int flavor = vec_jet_flavor->at(i);
    int flavor_index = Flavor_Index(flavor);

    int bin = ratio_c[region_index][sample_index][syst_index][flavor_index]->FindBin(pt, eta);
    float rf = ratio_c[region_index][sample_index][syst_index][flavor_index]->GetBinContent(bin);

    weight_rf *= rf;
  }

  return weight_rf;
} // float Tagging_RF_Flavor::Get_Tagging_RF_C_Tag(const TString &region, const TString &sample, const TString &syst, const vector<float>& vec_jet_pt, const vector<float>& vec_jet_eta, const vector<int>& vec_jet_flavor)

//////////

void Tagging_RF_Flavor::Combine_Lepton_Channel()
{
  cout << "[Tagging_RF_Flavor::Combine_Lepton_Channel]: Init" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF_Flavor/";

  fin_mu = new TFile(path_base + "Vcb_Tagging_RF_Flavor_" + era + "_Mu.root");
  fin_el = new TFile(path_base + "Vcb_Tagging_RF_Flavor_" + era + "_El.root");

  TString fout_name = "Vcb_Tagging_RF_Flavor_" + era + ".root";
  fout = new TFile(fout_name, "RECREATE");

  for (int i = 0; i < n_region; i++)
  {
    TDirectory *dir_region = (TDirectory *)fin_mu->Get(region_name[i]);
    TList *list_dir_sample = dir_region->GetListOfKeys();

    TDirectory *fout_dir_region = fout->mkdir(region_name[i]);
    for (int j = 0; j < list_dir_sample->GetEntries(); j++)
    {
      TObject *dir_sample = (TObject *)list_dir_sample->At(j);
      TString sample_name = dir_sample->GetName();
      TList *list_key = ((TDirectory *)dir_region->Get(sample_name))->GetListOfKeys();

      fout_dir_region->cd();
      TDirectory *fout_dir_sample = fout_dir_region->mkdir(sample_name);

      for (int k = 0; k < list_key->GetEntries(); k++)
      {
        TObject *key = (TObject *)list_key->At(k);
        TString key_name = key->GetName();

        if (key_name.Contains("Ratio_"))
        {
          TString histo_name = region_name[i] + "/" + sample_name + "/" + key_name;
          cout << region_name[i] << " " << sample_name << " " << histo_name << endl;

          TH2D *ratio_mu;
          TH2D *ratio_el;

          ratio_mu = (TH2D *)fin_mu->Get(histo_name);
          ratio_el = (TH2D *)fin_el->Get(histo_name);

          // consistency check
          if (key_name.Contains("C_Tag"))
            cout << ratio_mu->Chi2Test(ratio_el, "WW") << endl;

          // combine
          TString sub = key_name.Remove(0, 6);
          sub = region_name[i] + "/" + sample_name + "/" + sub;

          TH2D *histo_before_mu = (TH2D *)fin_mu->Get(sub + "_Before");
          TH2D *histo_after_mu = (TH2D *)fin_mu->Get(sub + "_After");

          TH2D *histo_before_el = (TH2D *)fin_el->Get(sub + "_Before");
          TH2D *histo_after_el = (TH2D *)fin_el->Get(sub + "_After");

          TH2D *histo_before_combine = (TH2D *)histo_before_mu->Clone();
          histo_before_combine->Add(histo_before_el);

          TH2D *histo_after_combine = (TH2D *)histo_after_mu->Clone();
          histo_after_combine->Add(histo_after_el);

          TH2D *ratio_average = (TH2D *)ratio_mu->Clone();
          ratio_average->Reset();

          ratio_average->Divide(histo_before_combine, histo_after_combine);

          fout_dir_sample->cd();
          histo_before_combine->Write();
          histo_after_combine->Write();
          ratio_average->Write();
        } // if (key_name.Contains("Ratio_"))
      } // loop over keys
    } // loop over mc_merged
  } // loop over n_region

  cout << "[Tagging_RF_Flavor::Combine_Lepton_Channel]: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Combine_Lepton_Channel()

//////////

void Tagging_RF_Flavor::Draw_Result()
{
  cout << "[Tagging_RF_Flavor::Draw_Result]: Init" << endl;

  gStyle->SetPaintTextFormat("0.3f");
  gStyle->SetOptStat(0);

  // draw results
  vector<TString> region_to_draw = {"D"};

  vector<TString> sample_to_draw = {"TTLJ_2", "TTLJ_4", "TTLJ_45"};

  // vector<TString> sample_to_draw = {"TTLJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
  //                                   "TTLJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
  //                                   "TTLJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
  //                                   "TTLL", "TTLL_CC", "TTLL_BB"};

  vector<TString> syst_to_draw = {"C_Tag_Nominal",
                                  "C_Tag_PU_Up", "C_Tag_PU_Down",
                                  "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up",
                                  "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up",
                                  "C_Tag_XSec_Br_Unc_WJets_C_Up", "C_Tag_XSec_Br_Unc_WJets_C_Down"};

  vector<TString> flavor_to_draw = {"L", "C", "B"};

  for (unsigned int i = 0; i < region_to_draw.size(); i++)
  {
    for (unsigned int j = 0; j < sample_to_draw.size(); j++)
    {
      for (unsigned int k = 0; k < syst_to_draw.size(); k++)
      {
        for (unsigned int l = 0; l < flavor_to_draw.size(); l++)
        {
          TH2D *histo = (TH2D *)fout->Get(Form("%s/%s/Ratio_%s_%s_%s_%s", region_to_draw[i].Data(), sample_to_draw[j].Data(), region_to_draw[i].Data(), sample_to_draw[j].Data(), syst_to_draw[k].Data(), flavor_to_draw[l].Data()));

          TString can_name = Form("Tagging_RF_%s_%s_%s_%s_%s", region_to_draw[i].Data(), sample_to_draw[j].Data(), syst_to_draw[k].Data(), era.Data(), flavor_to_draw[l].Data());
          TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
          canvas->Draw();

          if (syst_to_draw[k].Contains("C_Tag"))
          {
            histo->GetXaxis()->SetTitle("P_{T} [GeV]");
            histo->GetYaxis()->SetTitle("Eta [#eta]");
            histo->GetZaxis()->SetRangeUser(0.6, 1.1);
          }
          histo->Draw("COLZ texte");

          canvas->Print(can_name + "." + extension, extension);
        } // loop over flavor_to_draw
      } // loop over syst_to_draw
    } // loop over sample_to_draw
  } // loop over region_to_draw

  cout << "[Tagging_RF_Flavor::Draw_Result]: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Draw_Result()

//////////

void Tagging_RF_Flavor::Draw_Validation()
{
  cout << "[Tagging_RF_Flavor::Draw_Validation}: Init" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF_Flavor";

  TFile *fin = new TFile(Form("%s/Vcb_Tagging_RF_Flavor_Validation_%s_%s_%s.root", path_base.Data(), era.Data(), channel.Data(), tagger.Data()));

  gStyle->SetOptStat(0);

  vector<TString> vec_sample_to_draw = {
      //"TTLJ_CP5Down_2", "TTLJ_CP5Down_CC_2", "TTLJ_CP5Down_BB_2",
      //"TTLJ_CP5Up_2", "TTLJ_CP5Up_CC_2", "TTLJ_CP5Up_BB_2",
      //"TTLJ_mtop171p5_2", "TTLJ_mtop171p5_CC_2", "TTLJ_mtop171p5_BB_2",
      //"TTLJ_mtop173p5_2", "TTLJ_mtop173p5_CC_2", "TTLJ_mtop173p5_BB_2",
      "TTLJ_2",  //"TTLJ_CC_2", "TTLJ_BB_2",
      "TTLJ_4",  //"TTLJ_CC_4", "TTLJ_BB_4",
      "TTLJ_45", // "TTLJ_CC_45", "TTLJ_BB_45",
                 //"TTLL", "TTLL_CC", "TTLL_BB"
  };

  vector<TString> vec_syst_to_draw = {"C_Tag_Nominal",
                                      "C_Tag_PU_Up", "C_Tag_PU_Down",
                                      "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up",
                                      "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up",
                                      "C_Tag_XSec_Br_Unc_WJets_C_Up", "C_Tag_XSec_Br_Unc_WJets_C_Down"};

  TLatex *latex = new TLatex();
  latex->SetTextSize(0.03);

  for (int i = 0; i < vec_sample_to_draw.size(); i++)
  {
    TString sample_to_draw = vec_sample_to_draw[i];
    cout << sample_to_draw << endl;

    int n_syst;
    if (tagger == "B_Tagger")
      n_syst = n_syst_b;
    else if (tagger == "C_Tagger")
      n_syst = n_syst_c;

    for (int j = 0; j < vec_syst_to_draw.size(); j++)
    {
      TString syst_name = vec_syst_to_draw[j];

      TString can_name = "Validation_" + channel + "_" + syst_name + "_" + sample_to_draw + "_" + era;
      cout << can_name << endl;

      TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
      if (tagger == "C_Tagger")
        canvas->Divide(3, 1);
      canvas->Draw();

      int n_scores;
      if (tagger == "B_Tagger")
        n_scores = 1;
      else if (tagger == "C_Tagger")
        n_scores = 3;

      TPad *pad[n_scores][2];
      TH1D *histo_closure[n_flavor][3];

      for (int k = 0; k < n_scores; k++)
      {
        TString score_type;
        if (k == 0)
          score_type = "BvsC";
        else if (k == 1)
          score_type = "CvsB";
        else if (k == 2)
          score_type = "CvsL";

        TString pad_name = to_string(k) + "_Absolute";
        pad[k][0] = new TPad(pad_name, pad_name, 0, 0.3, 1, 1);

        pad_name = to_string(k) + "_Ratio";
        pad[k][1] = new TPad(pad_name, pad_name, 0, 0, 1, 0.3);

        for (int l = 0; l < n_flavor; l++)
        {
          for (int m = 0; m < 3; m++)
          {
            TString histo_name = "Closure_D_" + sample_to_draw + "_" + syst_name + "_" + flavor_name[l] + "_Leading_Jet_" + score_type + "_" + to_string(m);
            cout << histo_name << endl;

            histo_closure[l][m] = (TH1D *)fin->Get("D/" + sample_to_draw + "/" + histo_name);
          } // loop over 3
        } // loop over n_flavor

        // Absolute
        if (tagger == "B_Tagger")
          canvas->cd();
        else if (tagger == "C_Tagger")
          canvas->cd(k + 1);

        pad[k][0]->Draw();
        pad[k][0]->cd();

        for (int l = 0; l < n_flavor; l++)
        {
          for (int m = 0; m < 3; m++)
          {
            if (l == 0)
              histo_closure[l][m]->SetMarkerStyle(4);
            else if (l == 1)
              histo_closure[l][m]->SetMarkerStyle(33);
            else if (l == 2)
              histo_closure[l][m]->SetMarkerStyle(47);

            histo_closure[l][m]->SetMarkerColor(m + 2);
            histo_closure[l][m]->SetLineColor(m + 2);

            histo_closure[l][m]->SetMarkerSize(1.5);

            if (l == 0 && m == 0)
            {
              if (k == 0)
                histo_closure[l][m]->GetXaxis()->SetTitle("BvsC");
              else if (k == 1)
                histo_closure[l][m]->GetXaxis()->SetTitle("CvsB");
              else if (k == 2)
                histo_closure[l][m]->GetXaxis()->SetTitle("CvsL");

              histo_closure[l][m]->Draw();
            }
            else
              histo_closure[l][m]->Draw("SAME");
          } // loop over 3
        } // loop over n_flavor

        // legend & yield table
        if (k == 0)
        {
          TLegend *tl = new TLegend(0.2, 0.55, 0.8, 0.8);
          tl->SetBorderSize(0);
          tl->SetNColumns(3);

          for (int l = 0; l < n_flavor; l++)
          {
            tl->AddEntry(histo_closure[l][0], Form("%s: Raw", flavor_name[l].Data()), "lep");
            tl->AddEntry(histo_closure[l][1], Form("%s: + SF", flavor_name[l].Data()), "lep");
            tl->AddEntry(histo_closure[l][2], Form("%s: + SF + Patch", flavor_name[l].Data()), "lep");

            float count_raw = histo_closure[l][0]->Integral();
            float count_sf = histo_closure[l][1]->Integral();
            float count_rf = histo_closure[l][2]->Integral();

            cout << flavor_name[l] << ": Count_Raw = " << count_raw << ", Count_SF = " << count_sf << ", Count_RF = " << count_rf << endl;

            float y;
            if (l == 0)
              y = 0.5;
            else if (l == 1)
              y = 0.45;
            else if (l == 2)
              y = 0.4;

            latex->DrawLatexNDC(0.3, y, Form("%s: %.2f, %.2f, %.2f", flavor_name[l].Data(), count_raw, count_sf, count_rf));
          }

          tl->Draw("same");
        } //   if (k == 0)

        // Ratio
        if (tagger == "B_Tagger")
          canvas->cd();
        else if (tagger == "C_Tagger")
          canvas->cd(k + 1);

        pad[k][1]->Draw();
        pad[k][1]->cd();

        TH1D *histo_closure_ratio[n_flavor][3];

        for (int l = 0; l < n_flavor; l++)
        {
          for (int m = 0; m < 3; m++)
          {
            histo_closure_ratio[l][m] = (TH1D *)histo_closure[l][m]->Clone();
            histo_closure_ratio[l][m]->Divide(histo_closure[l][1]);

            if (l == 0)
              histo_closure[l][m]->SetMarkerStyle(4);
            else if (l == 1)
              histo_closure[l][m]->SetMarkerStyle(33);
            else if (l == 2)
              histo_closure[l][m]->SetMarkerStyle(47);

            histo_closure[l][m]->SetMarkerColor(m + 2);
            histo_closure[l][m]->SetLineColor(m + 2);

            if (l == 0 && m == 0)
            {
              if (k == 0)
                histo_closure_ratio[l][m]->GetXaxis()->SetTitle("BvsC");
              else if (k == 1)
                histo_closure_ratio[l][m]->GetXaxis()->SetTitle("CvsB");
              else if (k == 2)
                histo_closure_ratio[l][m]->GetXaxis()->SetTitle("CvsL");

              histo_closure_ratio[l][m]->GetYaxis()->SetTitle("Ratio");
              histo_closure_ratio[l][m]->GetYaxis()->SetRangeUser(0.0, 2.0);

              histo_closure_ratio[l][m]->Draw();
            }
            else if (m == 0 || m == 2)
              histo_closure_ratio[l][m]->Draw("SAME");
          } // loop over 3
        } // loop over n_flavor
      } // loop over n_scores

      /*
              float lumi;
              if (era == "2016preVFP")
                lumi = lumi_2016a;
              else if (era == "2016postVFP")
                lumi = lumi_2016a;
              else if (era == "2017")
                lumi = lumi_2017;
              if (era == "2018")
                lumi = lumi_2018;

              canvas->cd();
              latex->DrawLatexNDC(0.1, 0.9, "CMS work in progress");
              latex->DrawLatexNDC(0.9, 0.9, Form("%f fb^{-1}", lumi));
      */

      canvas->Print(can_name + "." + extension, extension);
    } // loop over n_syst_c
  } // loop over vec_sample_to_draw

  cout << "[Tagging_RF_Flavor::Draw_Validation}: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Draw_Validation()

//////////

void Tagging_RF_Flavor::Fill_Histo_MC(const int &region_index, const TString &sample_name, const TString &tree_type)
{
  int sample_index = Histo_Index(sample_name);

  float weight = 1;
  weight *= weight_lumi;
  weight *= weight_mc;
  weight *= weight_hem_veto;
  weight *= weight_pileup;
  weight *= weight_prefire;
  weight *= weight_pujet_veto;
  weight *= weight_top_pt;

  weight *= sf_sl_trig;

  weight *= sf_mu_id;
  weight *= sf_mu_iso;

  weight *= sf_el_id;
  weight *= sf_el_reco;

  if (TMath::IsNaN(weight))
    return;

  for (int i = 0; i < vec_jet_pt->size(); i++)
  {

    float pt = vec_jet_pt->at(i);
    float eta = vec_jet_eta->at(i);

    int flavor = vec_jet_flavor->at(i);
    int flavor_index = Flavor_Index(flavor);

    double cvsb = vec_jet_cvsb->at(i);
    double cvsl = vec_jet_cvsl->at(i);

    if (tree_type == "Central")
    {
      // norminal
      float c_tag_sf_nominal = correction_ref_ctag->evaluate({"central", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][0][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][0][flavor_index]->Fill(pt, eta, weight * c_tag_sf_nominal);

      // extrap
      float c_tag_sf_extrap_down = correction_ref_ctag->evaluate({"down_Extrap", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][1][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][1][flavor_index]->Fill(pt, eta, weight * c_tag_sf_extrap_down);

      float c_tag_sf_extrap_up = correction_ref_ctag->evaluate({"up_Extrap", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][2][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][2][flavor_index]->Fill(pt, eta, weight * c_tag_sf_extrap_up);

      // interp
      float c_tag_sf_interp_down = correction_ref_ctag->evaluate({"down_Interp", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][3][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][3][flavor_index]->Fill(pt, eta, weight * c_tag_sf_interp_down);

      float c_tag_sf_interp_up = correction_ref_ctag->evaluate({"up_Interp", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][4][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][4][flavor_index]->Fill(pt, eta, weight * c_tag_sf_interp_up);

      // muf
      float c_tag_sf_muf_down = correction_ref_ctag->evaluate({"down_LHEScaleWeight_muF", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][5][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_2);
      histo_mc_after_c[region_index][sample_index][5][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_2 * c_tag_sf_muf_down);

      float c_tag_sf_muf_up = correction_ref_ctag->evaluate({"up_LHEScaleWeight_muF", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][6][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_1);
      histo_mc_after_c[region_index][sample_index][6][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_1 * c_tag_sf_muf_up);

      // mur
      float c_tag_sf_mur_down = correction_ref_ctag->evaluate({"down_LHEScaleWeight_muR", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][7][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_6);
      histo_mc_after_c[region_index][sample_index][7][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_6 * c_tag_sf_mur_down);

      float c_tag_sf_mur_up = correction_ref_ctag->evaluate({"up_LHEScaleWeight_muR", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][8][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_3);
      histo_mc_after_c[region_index][sample_index][8][flavor_index]->Fill(pt, eta, weight * weight_scale_variation_3 * c_tag_sf_mur_up);

      // fsr
      float c_tag_sf_fsr_down = correction_ref_ctag->evaluate({"down_PSWeightFSR", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][9][flavor_index]->Fill(pt, eta, weight * weight_ps[0]);
      histo_mc_after_c[region_index][sample_index][9][flavor_index]->Fill(pt, eta, weight * weight_ps[0] * c_tag_sf_fsr_down);

      float c_tag_sf_fsr_up = correction_ref_ctag->evaluate({"up_PSWeightFSR", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][10][flavor_index]->Fill(pt, eta, weight * weight_ps[1]);
      histo_mc_after_c[region_index][sample_index][10][flavor_index]->Fill(pt, eta, weight * weight_ps[1] * c_tag_sf_fsr_up);

      // isr
      float c_tag_sf_isr_down = correction_ref_ctag->evaluate({"down_PSWeightISR", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][11][flavor_index]->Fill(pt, eta, weight * weight_ps[2]);
      histo_mc_after_c[region_index][sample_index][11][flavor_index]->Fill(pt, eta, weight * weight_ps[2] * c_tag_sf_isr_down);

      float c_tag_sf_isr_up = correction_ref_ctag->evaluate({"up_PSWeightISR", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][12][flavor_index]->Fill(pt, eta, weight * weight_ps[3]);
      histo_mc_after_c[region_index][sample_index][12][flavor_index]->Fill(pt, eta, weight * weight_ps[3] * c_tag_sf_isr_up);

      // pu
      float c_tag_sf_pu_down = correction_ref_ctag->evaluate({"down_PUWeight", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][13][flavor_index]->Fill(pt, eta, weight * weight_pileup_down / weight_pileup);
      histo_mc_after_c[region_index][sample_index][13][flavor_index]->Fill(pt, eta, weight * weight_pileup_down / weight_pileup * c_tag_sf_pu_down);

      float c_tag_sf_pu_up = correction_ref_ctag->evaluate({"up_PUWeight", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][14][flavor_index]->Fill(pt, eta, weight * weight_pileup_up / weight_pileup);
      histo_mc_after_c[region_index][sample_index][14][flavor_index]->Fill(pt, eta, weight * weight_pileup_up / weight_pileup * c_tag_sf_pu_up);

      // stat
      float c_tag_sf_stat_down = correction_ref_ctag->evaluate({"down_Stat", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][15][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][15][flavor_index]->Fill(pt, eta, weight * c_tag_sf_stat_down);

      float c_tag_sf_stat_up = correction_ref_ctag->evaluate({"up_Stat", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][16][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][16][flavor_index]->Fill(pt, eta, weight * c_tag_sf_stat_up);

      // unc_dyjets_b
      float c_tag_sf_xsec_br_unc_dyjets_b_down = correction_ref_ctag->evaluate({"down_XSec_BRUnc_DYJets_b", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][17][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][17][flavor_index]->Fill(pt, eta, weight * c_tag_sf_xsec_br_unc_dyjets_b_down);

      float c_tag_sf_xsec_br_unc_dyjets_b_up = correction_ref_ctag->evaluate({"up_XSec_BRUnc_DYJets_b", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][18][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][18][flavor_index]->Fill(pt, eta, weight * c_tag_sf_xsec_br_unc_dyjets_b_up);

      // unc_dyjets_c
      float c_tag_sf_xsec_br_unc_dyjets_c_down = correction_ref_ctag->evaluate({"down_XSec_BRUnc_DYJets_c", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][19][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][19][flavor_index]->Fill(pt, eta, weight * c_tag_sf_xsec_br_unc_dyjets_c_down);

      float c_tag_sf_xsec_br_unc_dyjets_c_up = correction_ref_ctag->evaluate({"up_XSec_BRUnc_DYJets_c", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][20][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][20][flavor_index]->Fill(pt, eta, weight * c_tag_sf_xsec_br_unc_dyjets_c_up);

      // unc_wjets_c
      float c_tag_sf_xsec_br_unc_wjets_c_down = correction_ref_ctag->evaluate({"down_XSec_BRUnc_WJets_c", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][21][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][21][flavor_index]->Fill(pt, eta, weight * c_tag_sf_xsec_br_unc_wjets_c_down);

      float c_tag_sf_xsec_br_unc_wjets_c_up = correction_ref_ctag->evaluate({"up_XSec_BRUnc_WJets_c", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][22][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][22][flavor_index]->Fill(pt, eta, weight * c_tag_sf_xsec_br_unc_wjets_c_up);
    } // if (tree_type == "Central")

    // jer
    else if (tree_type == "JetResDown")
    {
      float c_tag_sf_jer_down = correction_ref_ctag->evaluate({"down_jer", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][23][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][23][flavor_index]->Fill(pt, eta, weight * c_tag_sf_jer_down);
    }

    else if (tree_type == "JetResUp")
    {
      float c_tag_sf_jer_up = correction_ref_ctag->evaluate({"up_jer", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][24][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][24][flavor_index]->Fill(pt, eta, weight * c_tag_sf_jer_up);
    }

    // jes
    else if (tree_type == "JetEnDown")
    {
      float c_tag_sf_jes_down = correction_ref_ctag->evaluate({"down_jesTotal", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][25][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][25][flavor_index]->Fill(pt, eta, weight * c_tag_sf_jes_down);
    }

    else if (tree_type == "JetEnUp")
    {
      float c_tag_sf_jes_up = correction_ref_ctag->evaluate({"up_jesTotal", flavor, cvsl, cvsb});
      histo_mc_before_c[region_index][sample_index][26][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][26][flavor_index]->Fill(pt, eta, weight * c_tag_sf_jes_up);
    }

    // jes break down
    if (chk_jes_breakdown == true)
    {
      int histo_index;
      if (tree_type == "JetEnAbsoluteDown")
        histo_index = 27;
      else if (tree_type == "JetEnAbsoluteUp")
        histo_index = 28;
      else if (tree_type == "JetEnBBEC1Down")
        histo_index = 29;
      else if (tree_type == "JetEnBBEC1Up")
        histo_index = 30;
      else if (tree_type == "JetEnEC2Down")
        histo_index = 31;
      else if (tree_type == "JetEnEC2Up")
        histo_index = 32;
      else if (tree_type == "JetEnFlavorQCDDown")
        histo_index = 33;
      else if (tree_type == "JetEnFlavorQCDUp")
        histo_index = 34;
      else if (tree_type == "JetEnHFDown")
        histo_index = 35;
      else if (tree_type == "JetEnHFUp")
        histo_index = 36;
      else if (tree_type == "JetEnRelativeBalDown")
        histo_index = 37;
      else if (tree_type == "JetEnRelativeBalUp")
        histo_index = 38;
      else if (tree_type == "JetEnAbsolute2016Down" || tree_type == "JetEnAbsolute2017Down" || tree_type == "JetEnAbsolute2018Down")
        histo_index = 39;
      else if (tree_type == "JetEnAbsolute2016Up" || tree_type == "JetEnAbsolute2017Up" || tree_type == "JetEnAbsolute2018Up")
        histo_index = 40;
      else if (tree_type == "JetEnBBEC12016Down" || tree_type == "JetEnBBEC12017Down" || tree_type == "JetEnBBEC12018Down")
        histo_index = 41;
      else if (tree_type == "JetEnBBEC12016Up" || tree_type == "JetEnBBEC12017Up" || tree_type == "JetEnBBEC12018Up")
        histo_index = 42;
      else if (tree_type == "JetEnEC22016Down" || tree_type == "JetEnEC22017Down" || tree_type == "JetEnEC22018Down")
        histo_index = 43;
      else if (tree_type == "JetEnEC22016Up" || tree_type == "JetEnEC22017Up" || tree_type == "JetEnEC22018Up")
        histo_index = 44;
      else if (tree_type == "JetEnHF2016Down" || tree_type == "JetEnHF2017Down" || tree_type == "JetEnHF2018Down")
        histo_index = 45;
      else if (tree_type == "JetEnHF2016Up" || tree_type == "JetEnHF2017Up" || tree_type == "JetEnHF2018Up")
        histo_index = 46;
      else if (tree_type == "JetEnRelativeSample2016Down" || tree_type == "JetEnRelativeSample2017Down" || tree_type == "JetEnRelativeSample2018Down")
        histo_index = 47;
      else if (tree_type == "JetEnRelativeSample2016Up" || tree_type == "JetEnRelativeSample2017Up" || tree_type == "JetEnRelativeSample2018Up")
        histo_index = 48;

      float c_tag_sf_jes;
      if (tree_type.Contains("Down"))
        c_tag_sf_jes = correction_ref_ctag->evaluate({"down_jesTotal", flavor, cvsl, cvsb});
      else if (tree_type.Contains("Up"))
        c_tag_sf_jes = correction_ref_ctag->evaluate({"up_jesTotal", flavor, cvsl, cvsb});

      histo_mc_before_c[region_index][sample_index][histo_index][flavor_index]->Fill(pt, eta, weight);
      histo_mc_after_c[region_index][sample_index][histo_index][flavor_index]->Fill(pt, eta, weight * c_tag_sf_jes);
    } // if (chk_jes_breakdown == true)
  } // loop over jets

  return;
} // void Tagging_RF_Flavor::Fill_Histo_MC()

//////////

void Tagging_RF_Flavor::Fill_Histo_Validation_MC_C_Tagger(const int &region_index, const TString &sample_name, const TString &tree_type)
{
  int sample_index = Histo_Index(sample_name);
  TString histo_name_rf = Histo_Name_RF(sample_name);
  int rf_distribution_index = TTHF_Breakdown_Index(sample_name);

  int leading_jet_flavor = vec_jet_flavor->at(0);
  int index_leading_jet_flavor = Flavor_Index(leading_jet_flavor);

  for (int i = 0; i < n_syst_c; i++)
  {
    TString c_tag_type = syst_name_c[i];

    if (tree_type == "Central")
    {
      // if (c_tag_type == "C_Tag_JER_Down" || c_tag_type == "C_Tag_JER_Up" || c_tag_type == "C_Tag_JES_Total_Down" || c_tag_type == "C_Tag_JES_Total_Up")
      if (c_tag_type.Contains("C_Tag_JER") || c_tag_type.Contains("C_Tag_JES"))
        continue;
    }
    else if (tree_type == "JetResDown")
    {
      if (c_tag_type != "C_Tag_JER_Down")
        continue;
    }
    else if (tree_type == "JetResUp")
    {
      if (c_tag_type != "C_Tag_JER_Up")
        continue;
    }
    else if (tree_type == "JetEnDown")
    {
      if (c_tag_type != "C_Tag_JES_Total_Down")
        continue;
    }
    else if (tree_type == "JetEnUp")
    {
      if (c_tag_type != "C_Tag_JES_Total_Up")
        continue;
    }

    // JES Breakdown
    else if (tree_type.Contains("JetEn"))
    {
      TString direction;
      if (tree_type.Contains("Down"))
        direction = "Down";
      else if (tree_type.Contains("Up"))
        direction = "Up";

      TString tmp = tree_type;
      tmp.ReplaceAll(direction, "");
      tmp.ReplaceAll("JetEn", "");

      if (c_tag_type != "C_Tag_JES_" + tmp + "_" + direction)
        continue;
    }

    float weight_raw = 1;

    weight_raw *= weight_lumi;
    weight_raw *= weight_mc;
    weight_raw *= weight_hem_veto;

    if (c_tag_type == "C_Tag_PS_FSR_Fixed_Down")
      weight_raw *= weight_ps[0];
    else if (c_tag_type == "C_Tag_PS_FSR_Fixed_Up")
      weight_raw *= weight_ps[1];
    else if (c_tag_type == "C_Tag_PS_ISR_Fixed_Down")
      weight_raw *= weight_ps[2];
    else if (c_tag_type == "C_Tag_PS_ISR_Fixed_Up")
      weight_raw *= weight_ps[3];

    if (c_tag_type == "C_Tag_LHE_Scale_MuF_Down")
      weight_raw *= weight_scale_variation_2;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuF_Up")
      weight_raw *= weight_scale_variation_1;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuR_Down")
      weight_raw *= weight_scale_variation_6;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuR_Up")
      weight_raw *= weight_scale_variation_3;

    if (c_tag_type == "C_Tag_PU_Down")
      weight_raw *= weight_pileup_down;
    else if (c_tag_type == "C_Tag_PU_Up")
      weight_raw *= weight_pileup_up;
    else
      weight_raw *= weight_pileup;

    weight_raw *= weight_prefire;
    weight_raw *= weight_pujet_veto;
    weight_raw *= weight_top_pt;

    weight_raw *= sf_sl_trig;

    weight_raw *= sf_mu_id;
    weight_raw *= sf_mu_iso;

    weight_raw *= sf_el_id;
    weight_raw *= sf_el_reco;

    float weight_c_sf;
    if (c_tag_type == "C_Tag_Nominal")
      weight_c_sf = weight_c_tag;
    else if (c_tag_type == "C_Tag_Extrap_Down")
      weight_c_sf = weight_c_tag_extrap_down;
    else if (c_tag_type == "C_Tag_Extrap_Up")
      weight_c_sf = weight_c_tag_extrap_up;
    else if (c_tag_type == "C_Tag_Interp_Down")
      weight_c_sf = weight_c_tag_interp_down;
    else if (c_tag_type == "C_Tag_Interp_Up")
      weight_c_sf = weight_c_tag_interp_up;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuF_Down")
      weight_c_sf = weight_c_tag_lhe_scale_muf_down;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuF_Up")
      weight_c_sf = weight_c_tag_lhe_scale_muf_up;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuR_Down")
      weight_c_sf = weight_c_tag_lhe_scale_mur_down;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuR_Up")
      weight_c_sf = weight_c_tag_lhe_scale_mur_up;
    else if (c_tag_type == "C_Tag_PS_FSR_Fixed_Down")
      weight_c_sf = weight_c_tag_ps_fsr_fixed_down;
    else if (c_tag_type == "C_Tag_PS_FSR_Fixed_Up")
      weight_c_sf = weight_c_tag_ps_fsr_fixed_up;
    else if (c_tag_type == "C_Tag_PS_ISR_Fixed_Down")
      weight_c_sf = weight_c_tag_ps_isr_fixed_down;
    else if (c_tag_type == "C_Tag_PS_ISR_Fixed_Up")
      weight_c_sf = weight_c_tag_ps_isr_fixed_up;
    else if (c_tag_type == "C_Tag_PU_Down")
      weight_c_sf = weight_c_tag_pu_down;
    else if (c_tag_type == "C_Tag_PU_Up")
      weight_c_sf = weight_c_tag_pu_up;
    else if (c_tag_type == "C_Tag_Stat_Down")
      weight_c_sf = weight_c_tag_stat_down;
    else if (c_tag_type == "C_Tag_Stat_Up")
      weight_c_sf = weight_c_tag_stat_up;
    else if (c_tag_type == "C_Tag_XSec_Br_Unc_DYJets_B_Down")
      weight_c_sf = weight_c_tag_xsec_br_unc_dyjets_b_down;
    else if (c_tag_type == "C_Tag_XSec_Br_Unc_DYJets_B_Up")
      weight_c_sf = weight_c_tag_xsec_br_unc_dyjets_b_up;
    else if (c_tag_type == "C_Tag_XSec_Br_Unc_DYJets_C_Down")
      weight_c_sf = weight_c_tag_xsec_br_unc_dyjets_c_down;
    else if (c_tag_type == "C_Tag_XSec_Br_Unc_DYJets_C_Up")
      weight_c_sf = weight_c_tag_xsec_br_unc_dyjets_c_up;
    else if (c_tag_type == "C_Tag_XSec_Br_Unc_WJets_C_Down")
      weight_c_sf = weight_c_tag_xsec_br_unc_wjets_c_down;
    else if (c_tag_type == "C_Tag_XSec_Br_Unc_WJets_C_Up")
      weight_c_sf = weight_c_tag_xsec_br_unc_wjets_c_up;
    else if (c_tag_type == "C_Tag_JER_Down")
      weight_c_sf = weight_c_tag_jer_down;
    else if (c_tag_type == "C_Tag_JER_Up")
      weight_c_sf = weight_c_tag_jer_up;
    else if (c_tag_type.Contains("C_Tag_JES") && c_tag_type.Contains("Down"))
      weight_c_sf = weight_c_tag_jes_total_down;
    else if (c_tag_type.Contains("C_Tag_JES") && c_tag_type.Contains("Up"))
      weight_c_sf = weight_c_tag_jes_total_up;

    float weight_sf = weight_raw * weight_c_sf;

    float c_rf = Get_Tagging_RF_C_Tag(region_name[region_index], histo_name_rf, c_tag_type, vec_jet_pt, vec_jet_eta, vec_jet_flavor);

    if (rf_distribution_index > -1)
      histo_rf[region_index][rf_distribution_index][i]->Fill(c_rf, weight_raw);

    float weight_rf = weight_sf * c_rf;

    for (int j = 0; j < 3; j++)
    {
      float weight;
      if (j == 0)
        weight = weight_raw;
      else if (j == 1)
        weight = weight_sf;
      else if (j == 2)
        weight = weight_rf;

      histo_closure_bvsc[region_index][sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_bvsc, weight);
      histo_closure_cvsb[region_index][sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_cvsb, weight);
      histo_closure_cvsl[region_index][sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_cvsl, weight);

      histo_closure_eta[region_index][sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_eta, weight);
      histo_closure_pt[region_index][sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_pt, weight);
    } // loop over 3

  } // loop over n_syst_c

  return;
} // void Tagging_RF_Flavor::Fill_Histo_Validation_MC_C_Tagger()

//////////

int Tagging_RF_Flavor::Flavor_Index(const int &flavor)
{
  if (TMath::Abs(flavor) == 4)
    return 1;
  else if (TMath::Abs(flavor) == 5)
    return 2;
  else
    return 0;
} // int Tagging_RF_Flavor::Flavor_Index()

//////////

int Tagging_RF_Flavor::Histo_Index(const TString &sample_name)
{
  int index = -999;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    TString histo_name = sample_name;

    if (histo_name.Contains("WtoCB"))
      histo_name.ReplaceAll("TTLJ_WtoCB", "TTLJ");

    if (chk_tthf_breakdown)
    {
      bool chk_b = false;
      bool chk_c = false;

      // // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
      // for (unsigned int i = 0; i < vec_sel_gen_hf_flavour->size(); i++)
      // {
      //   int flavour = vec_sel_gen_hf_flavour->at(i);
      //   int origin = vec_sel_gen_hf_origin->at(i);

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

      if (51 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 55)
        chk_b = true;
      else if (41 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 45)
        chk_c = true;

      if (chk_b)
        histo_name += "_BB";
      else if (chk_c)
        histo_name += "_CC";
    } // if(chk_tthf_breakdown)

    if (decay_mode == 21 || decay_mode == 23)
      histo_name += "_2";
    else if (decay_mode == 41 || decay_mode == 43)
      histo_name += "_4";
    else if (decay_mode == 45)
      histo_name += "_45";

    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name]) - vec_short_name_mc.begin();

  // cout << "test Histo_Index: " << sample_name << " " << index << endl;

  return index;
} // int Tagging_RF_Flavor::Histo_Index()

//////////

TString Tagging_RF_Flavor::Histo_Name_RF(const TString &sample_name)
{
  TString histo_name_rf;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    histo_name_rf = sample_name;

    if (histo_name_rf.Contains("WtoCB"))
      histo_name_rf.ReplaceAll("TTLJ_WtoCB", "TTLJ");

    if (chk_tthf_breakdown)
    {
      bool chk_b = false;
      bool chk_c = false;

      // // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
      // for (unsigned int i = 0; i < vec_sel_gen_hf_flavour->size(); i++)
      // {
      //   int flavour = vec_sel_gen_hf_flavour->at(i);
      //   int origin = vec_sel_gen_hf_origin->at(i);

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

      if (51 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 55)
        chk_b = true;
      else if (41 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 45)
        chk_c = true;

      // if (histo_name_rf.Contains("CP5") || histo_name_rf.Contains("hdamp") || histo_name_rf.Contains("mtop"))
      // {
      //   if (histo_name_rf.Contains("TTLJ"))
      //     histo_name_rf = "TTLJ";
      //   else if (histo_name_rf.Contains("TTLL"))
      //     histo_name_rf = "TTLL";
      // }

      if (chk_b)
        histo_name_rf += "_BB";
      else if (chk_c)
        histo_name_rf += "_CC";
    } // if(chk_tthf_breakdown)

    if (decay_mode == 21 || decay_mode == 23)
      histo_name_rf += "_2";
    else if (decay_mode == 41 || decay_mode == 43)
      histo_name_rf += "_4";
    else if (decay_mode == 45)
      histo_name_rf += "_45";
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    histo_name_rf = samples.map_short_name_mc[sample_name];

  return histo_name_rf;
} // int Tagging_RF_Flavor::Histo_Name_RF(const TString &sample_name)

//////////

int Tagging_RF_Flavor::N_BJets()
{
  float b_tag_cut;
  if (era == "2016preVFP")
    b_tag_cut = bvsc_wp_t_2016preVFP;
  else if (era == "2016postVFP")
    b_tag_cut = bvsc_wp_t_2016postVFP;
  else if (era == "2017")
    b_tag_cut = bvsc_wp_t_2017;
  else if (era == "2018")
    b_tag_cut = bvsc_wp_t_2018;

  int n_bjets = 0;
  for (int i = 0; i < vec_jet_bvsc->size(); i++)
  {
    if (vec_jet_bvsc->at(i) > b_tag_cut)
      n_bjets++;
  }

  return n_bjets;
} // int Tagging_RF_Flavor::N_BJets()

//////////

int Tagging_RF_Flavor::N_CJets()
{
  return -1;
} // int Tagging_RF_Flavor::N_CJets()

//////////

void Tagging_RF_Flavor::Ratio()
{
  cout << "[Tagging_RF_Flavor::Ratio] Init" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF_Flavor/";

  fin = new TFile(path_base + "/Vcb_Tagging_RF_Flavor_" + era + "_" + channel + ".root", "UPDATE");

  ratio_c = new TH2D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    ratio_c[i] = new TH2D ***[n_sample_merge_mc];

    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      ratio_c[i][j] = new TH2D **[n_syst_c];

      for (int k = 0; k < n_syst_c; k++)
      {
        ratio_c[i][j][k] = new TH2D *[n_flavor];

        for (int l = 0; l < n_flavor; l++)
        {
          TString ratio_name = "Ratio_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + syst_name_c[k] + "_" + flavor_name[l];
          ratio_c[i][j][k][l] = new TH2D(ratio_name, ratio_name, bin_pt.size() - 1, bin_pt.data(), bin_eta.size() - 1, bin_eta.data());

          TString histo_name = region_name[i] + "/" + vec_short_name_mc[j] + "/" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + syst_name_c[k] + "_" + flavor_name[l];

          TH2D *histo_mc_before = (TH2D *)fin->Get(histo_name + "_Before");
          TH2D *histo_mc_after = (TH2D *)fin->Get(histo_name + "_After");

          ratio_c[i][j][k][l]->Divide(histo_mc_before, histo_mc_after);
        } // loop over n_flavor
      } // loop over n_syst_c
    } // loop over n_sample
  } // loop over n_region

  cout << "[Tagging_RF_Flavor::Ratio] Done" << endl;

  return;
} // void Tagging_RF_Flavor::Ratio()

//////////

void Tagging_RF_Flavor::Read_Tree()
{
  cout << "[Tagging_RF_Flavor::Read_Tree]: Starts to read trees" << endl;

  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString tree_type = it_map_map->first;
    cout << "Tree_Type = " << tree_type << endl;

    map<TString, TTree *> *map_tree = it_map_map->second;

    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
      TString sample_name = it->first;
      TString sample_name_short = samples.map_short_name_mc[it->first];
      cout << sample_name << ", " << sample_name_short << endl;

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      Long64_t init;
      Long64_t end;

      Long64_t step = n_entries / n_split;
      init = step * index_split;
      end = step * (index_split + 1);
      if (index_split + 1 == n_split)
        end = n_entries;

      cout << "N_Entries:" << n_entries << " N_Split:" << n_split << " Index_Split:" << index_split << " Step:" << step << " Init:" << init << " End:" << end << endl;

      for (Long64_t i = init; i < end; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        if (TMath::IsNaN(weight_scale_variation_1) ||
            TMath::IsNaN(weight_scale_variation_2) ||
            TMath::IsNaN(weight_scale_variation_3) ||
            TMath::IsNaN(weight_scale_variation_4) ||
            TMath::IsNaN(weight_scale_variation_6) ||
            TMath::IsNaN(weight_scale_variation_8))
          continue;

        int region_index = Set_ABCD_Region();

        // int n_bjets = N_BJets();
        // int n_cjets = N_CJets();

        // if (n_bjets < 3)
        //   continue;

        bool chk_b = false;
        if (51 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 55)
          chk_b = true;

        bool chk_c = false;
        if (41 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 45)
          chk_c = true;

        // if (!chk_b)
        //   continue;

        if (mode == "Analysis")
          Fill_Histo_MC(region_index, sample_name, tree_type);
        else if (mode == "Validation")
        {
          // if (tagger == "B_Tagger")
          //   Fill_Histo_Validation_MC_B_Tagger(region_index, sample_name, tree_type);
          if (tagger == "C_Tagger")
            Fill_Histo_Validation_MC_C_Tagger(region_index, sample_name, tree_type);
        }
      } // loop over entries
    } // loop over map_mc
  } // loop over map_map_mc

  cout << "[Tagging_RF_Flavor::Read_Tree]: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Read_Tree()

//////////

void Tagging_RF_Flavor::Run_Analysis()
{
  cout << "[Tagging_RF_Flavor::Run_Analysis]: Init" << endl;

  ROOT::EnableImplicitMT(4);

  Setup_Analysis();
  Setup_Binning();
  Setup_Histo();
  Read_Tree();
  // Ratio();

  return;
} // Tagging_RF_Flavor::Run_Analysis()

//////////

void Tagging_RF_Flavor::Run_Combine()
{
  Combine_Lepton_Channel();
  // Combine_Decay_Mode();
  Draw_Result();

  return;
} // void Tagging_RF_Flavor::Run_Combine()

//////////

void Tagging_RF_Flavor::Run_Draw_Validation()
{
  gROOT->SetBatch(kTRUE);

  Draw_Validation();

  return;
} // void Tagging_RF_Flavor::Run_Draw_Validatiion()

//////////

void Tagging_RF_Flavor::Run_Ratio()
{
  cout << "[Tagging_RF_Flavor::Run_Ratio]: Init" << endl;

  Setup_Binning();
  Ratio();

  cout << "[Tagging_RF_Flavor::Run_Ratio]: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Run_Ratio()

//////////

void Tagging_RF_Flavor::Run_Validation()
{
  cout << "[Tagging_RF_Flavor::Run_Validation]: Init" << endl;

  ROOT::EnableImplicitMT(4);

  Setup_Application();
  Setup_Analysis();
  Setup_Histo_Validation();
  Read_Tree();

  cout << "[Tagging_RF_Flavor::Run_Validation]: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Run_Validation()

//////////

void Tagging_RF_Flavor::Setup_Analysis()
{
  cout << "[Tagging_RF_Flavor::Setup_Analysis]: Init analysis" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  path_base += era + "/" + "/Tagging_RF/";

  n_sample = samples.map_mc.size();

  // fix tree
  if (index_tree_type != -1)
  {
    TString tree_type_fixed;
    if (n_tree_type - 1 <= index_tree_type)
    {
      tree_type_fixed = "Central";

      index_split = index_tree_type - n_tree_type + 1;
      n_split = last_index_tree_type - n_tree_type + 1;
    }
    else
    {
      tree_type_fixed = vec_tree_type[index_tree_type];

      index_split = 0;
      n_split = 1;
    }

    cout << "Fixing tree_type. Index = " << index_tree_type << " Tree_Type " << tree_type_fixed << " N_Tree_Type " << n_tree_type << " Index_Split " << index_split << " N_Split " << n_split << endl;

    vec_tree_type.clear();
    vec_tree_type.shrink_to_fit();

    vec_tree_type.push_back(tree_type_fixed);
  }
  else
  {
    cout << "Iterate all tree_type" << endl;

    index_split = 0;
    n_split = 1;
  }

  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    map<TString, TTree *> map_tree_mc;
    vec_map_tree_mc.push_back(map_tree_mc);
  }

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    map_fin_mc[it->first] = new TFile(path_base + it->second);

    for (unsigned int i = 0; i < vec_tree_type.size(); i++)
    {
      TString tree_type = vec_tree_type[i];

      if ((it->first.Contains("CP5") || it->first.Contains("hdamp") || it->first.Contains("mtop17")) && tree_type != "Central")
        continue;

      cout << tree_type << endl;

      vec_map_tree_mc[i][it->first] = (TTree *)map_fin_mc[it->first]->Get(channel + "/" + tree_type + "/Result_Tree");
      Setup_Tree(vec_map_tree_mc[i][it->first], tree_type);
    } // for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  } // for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)

  // TTree handlers
  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    TString tree_type = vec_tree_type[i];
    map_map_tree_mc[tree_type] = &vec_map_tree_mc[i];
  }

  // Get CTagging Json
  TString ctag_json_path = "/data6/Users/isyoon/SKFlatAnalyzer/data/Run2UltraLegacy_v3/" + era + "/CTag/ctagging.json.gz";
  cout << "Reading JSON for CTagging SF... " << ctag_json_path << endl;

  unique_ptr<CorrectionSet> cset_ctag = CorrectionSet::from_file(ctag_json_path.Data());
  correction_ref_ctag = cset_ctag->at("deepJet_shape");

  cout << "[Tagging_RF_Flavor::Setup_Analysis]: Done" << endl;

  return;
} // Tagging_RF_Flavor::Setup_Analysis()

//////////

void Tagging_RF_Flavor::Setup_Application()
{
  cout << "[Tagging_RF_Flavor::Setup_Application]: Init" << endl;

  Setup_Binning();

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  fin = new TFile(path_base + "/Corrections/Vcb_Tagging_RF_Flavor_" + era + ".root");

  // set this manually
  if (chk_tthf_breakdown)
  {
    vec_sample_tagging_rf = {//"ttV", "VV", "VJets", "ST", "QCD_bEn",
                             "ST_sch", "ST_tch", "ST_tw",
                             "WJets", "DYJets",
                             "QCD_bEn",
                             "ttHTobb", "ttHToNonbb",
                             "ttWToLNu", "ttWToQQ",
                             "ttZToLLNuNu", "ttZToQQ_ll", "ttZToQQ",
                             "WW", "WZ", "ZZ",
                             "TTLJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
                             "TTLJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
                             "TTLJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
                             /* CP5 */
                             "TTLJ_CP5Down_2", "TTLJ_CP5Down_CC_2", "TTLJ_CP5Down_BB_2",
                             "TTLJ_CP5Down_4", "TTLJ_CP5Down_CC_4", "TTLJ_CP5Down_BB_4",
                             //"TTLJ_CP5Down_45", "TTLJ_CP5Down_CC_45", "TTLJ_CP5Down_BB_45",
                             "TTLJ_CP5Up_2", "TTLJ_CP5Up_CC_2", "TTLJ_CP5Up_BB_2",
                             "TTLJ_CP5Up_4", "TTLJ_CP5Up_CC_4", "TTLJ_CP5Up_BB_4",
                             //"TTLJ_CP5Up_45", "TTLJ_CP5Up_CC_45", "TTLJ_CP5Up_BB_45",
                             /* mtop */
                             "TTLJ_mtop171p5_2", "TTLJ_mtop171p5_CC_2", "TTLJ_mtop171p5_BB_2",
                             "TTLJ_mtop171p5_4", "TTLJ_mtop171p5_CC_4", "TTLJ_mtop171p5_BB_4",
                             //"TTLJ_mtop171p5_45", "TTLJ_mtop171p5_CC_45", "TTLJ_mtop171p5_BB_45",
                             "TTLJ_mtop173p5_2", "TTLJ_mtop173p5_CC_2", "TTLJ_mtop173p5_BB_2",
                             "TTLJ_mtop173p5_4", "TTLJ_mtop173p5_CC_4", "TTLJ_mtop173p5_BB_4",
                             //"TTLJ_mtop173p5_45", "TTLJ_mtop173p5_CC_45", "TTLJ_mtop173p5_BB_45",
                             /* hdamp */
                             "TTLJ_hdampDown_2", "TTLJ_hdampDown_CC_2", "TTLJ_hdampDown_BB_2",
                             "TTLJ_hdampDown_4", "TTLJ_hdampDown_CC_4", "TTLJ_hdampDown_BB_4",
                             //"TTLJ_hdampDown_45", "TTLJ_hdampDown_CC_45", "TTLJ_hdampDown_BB_45",
                             "TTLJ_hdampUp_2", "TTLJ_hdampUp_CC_2", "TTLJ_hdampUp_BB_2",
                             "TTLJ_hdampUp_4", "TTLJ_hdampUp_CC_4", "TTLJ_hdampUp_BB_4",
                             //"TTLJ_hdampUp_45", "TTLJ_hdampUp_CC_45", "TTLJ_hdampUp_BB_45",
                             "TTLL", "TTLL_CC", "TTLL_BB",
                             /* CP5 */
                             "TTLL_CP5Down", "TTLL_CP5Down_CC", "TTLL_CP5Down_BB",
                             "TTLL_CP5Up", "TTLL_CP5Up_CC", "TTLL_CP5Up_BB",
                             /* mtop */
                             "TTLL_mtop171p5", "TTLL_mtop171p5_CC", "TTLL_mtop171p5_BB",
                             "TTLL_mtop173p5", "TTLL_mtop173p5_CC", "TTLL_mtop173p5_BB",
                             /* hdamp */
                             "TTLL_hdampDown", "TTLL_hdampDown_CC", "TTLL_hdampDown_BB",
                             "TTLL_hdampUp", "TTLL_hdampUp_CC", "TTLL_hdampUp_BB"};
  }
  else
  {
    vec_sample_tagging_rf = {//"ttV", "VV", "VJets", "ST", "QCD_bEn",
                             "ST_sch", "ST_tch", "ST_tw",
                             "WJets", "DYJets",
                             "QCD_bEn",
                             "ttHTobb", "ttHToNonbb",
                             "ttWToLNu", "ttWToQQ",
                             "ttZToLLNuNu", "ttZToQQ_ll", "ttZToQQ",
                             "WW", "WZ", "ZZ",
                             "TTLJ_2", "TTLJ_4", "TTLJ_45",
                             /* CP5 */
                             "TTLJ_CP5Down_2", "TTLJ_CP5Down_4", "TTLJ_CP5Down_45",
                             "TTLJ_CP5Up_2", "TTLJ_CP5Up_4", "TTLJ_CP5Up_45",
                             /* mtop */
                             "TTLJ_mtop171p5_2", "TTLJ_mtop171p5_4", "TTLJ_mtop171p5_45",
                             "TTLJ_mtop173p5_2", "TTLJ_mtop173p5_4", "TTLJ_mtop173p5_45",
                             /* hdamp */
                             "TTLJ_hdampDown_2", "TTLJ_hdampDown_4", "TTLJ_hdampDown_45",
                             "TTLJ_hdampUp_2", "TTLJ_hdampUp_4", "TTLJ_hdampUp_45",
                             "TTLL",
                             /* CP5 */
                             "TTLL_CP5Down", "TTLL_CP5Up",
                             /* mtop */
                             "TTLL_mtop171p5", "TTLL_mtop173p5",
                             /* hdamp */
                             "TTLL_hdampDown", "TTLL_hdampUp"};
  }

  n_sample_tagging_rf = vec_sample_tagging_rf.size();

  ratio_c = new TH2D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    ratio_c[i] = new TH2D ***[n_sample_tagging_rf];

    for (int j = 0; j < n_sample_tagging_rf; j++)
    {
      ratio_c[i][j] = new TH2D **[n_syst_c];

      for (int k = 0; k < n_syst_c; k++)
      {
        ratio_c[i][j][k] = new TH2D *[n_flavor];

        for (int l = 0; l < n_flavor; l++)
        {
          TString ratio_name = region_name[i] + "/" + vec_sample_tagging_rf[j] + "/Ratio_" + region_name[i] + "_" + vec_sample_tagging_rf[j] + "_" + syst_name_c[k] + "_" + flavor_name[l];
          ratio_c[i][j][k][l] = (TH2D *)fin->Get(ratio_name);
        } // loop over n_flavor
      } // loop over n_syst_c
    } // loop over n_sample
  } // loop over n_region

  cout << "[Tagging_RF_Flavor::Setup_Application]: Done" << endl;

  return;
} // Tagging_RF_Flavor::Setup_Application()

//////////

void Tagging_RF_Flavor::Setup_Binning()
{
  bin_pt = {20, 30, 40, 60, 90, 150, 250};
  bin_eta = {-2.4, -2.1, -1.6, -1.1, -0.6, 0, 0.6, 1.1, 1.6, 2.1, 2.4};

  return;
} // Tagging_RF_Flavor::Setup_Binning()

//////////

void Tagging_RF_Flavor::Setup_Histo()
{
  cout << "[Tagging_RF_Flavor::Setup_Histo]: Init" << endl;

  histo_mc_before_c = new TH2D ****[n_region];
  histo_mc_after_c = new TH2D ****[n_region];

  for (int i = 0; i < n_region; i++)
  {
    histo_mc_before_c[i] = new TH2D ***[n_sample_merge_mc];
    histo_mc_after_c[i] = new TH2D ***[n_sample_merge_mc];

    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      histo_mc_before_c[i][j] = new TH2D **[n_syst_c];
      histo_mc_after_c[i][j] = new TH2D **[n_syst_c];

      for (int k = 0; k < n_syst_c; k++)
      {
        histo_mc_before_c[i][j][k] = new TH2D *[n_flavor];
        histo_mc_after_c[i][j][k] = new TH2D *[n_flavor];

        for (int l = 0; l < n_flavor; l++)
        {
          TString histo_name = region_name[i] + "_" + vec_short_name_mc[j] + "_" + syst_name_c[k] + "_" + flavor_name[l] + "_Before";
          histo_mc_before_c[i][j][k][l] = new TH2D(histo_name, histo_name, bin_pt.size() - 1, bin_pt.data(), bin_eta.size() - 1, bin_eta.data());

          histo_name = region_name[i] + "_" + vec_short_name_mc[j] + "_" + syst_name_c[k] + "_" + flavor_name[l] + "_After";
          histo_mc_after_c[i][j][k][l] = new TH2D(histo_name, histo_name, bin_pt.size() - 1, bin_pt.data(), bin_eta.size() - 1, bin_eta.data());
        } // loop over n_flavor
      } // loop over n_syst_c
    } // loop over n_sample_merge_mc
  } // loop over n_region

  cout << "[Tagging_RF_Flavor::Setup_Histo]: Done" << endl;

  return;
} // Tagging_RF_Flavor::Setup_Histo()

//////////

void Tagging_RF_Flavor::Setup_Histo_Validation()
{
  cout << "[Tagging_RF_Flavor::Setup_Histo_Validation]: Init" << endl;

  histo_closure_bvsc = new TH1D *****[n_region];
  histo_closure_cvsb = new TH1D *****[n_region];
  histo_closure_cvsl = new TH1D *****[n_region];
  histo_closure_eta = new TH1D *****[n_region];
  histo_closure_pt = new TH1D *****[n_region];

  for (int i = 0; i < n_region; i++)
  {
    histo_closure_bvsc[i] = new TH1D ****[n_sample_merge_mc];
    histo_closure_cvsb[i] = new TH1D ****[n_sample_merge_mc];
    histo_closure_cvsl[i] = new TH1D ****[n_sample_merge_mc];
    histo_closure_eta[i] = new TH1D ****[n_sample_merge_mc];
    histo_closure_pt[i] = new TH1D ****[n_sample_merge_mc];

    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      int n_syst;
      if (tagger == "B_Tagger")
        n_syst = n_syst_b;
      else if (tagger == "C_Tagger")
        n_syst = n_syst_c;

      histo_closure_bvsc[i][j] = new TH1D ***[n_syst];
      histo_closure_cvsb[i][j] = new TH1D ***[n_syst];
      histo_closure_cvsl[i][j] = new TH1D ***[n_syst];
      histo_closure_eta[i][j] = new TH1D ***[n_syst];
      histo_closure_pt[i][j] = new TH1D ***[n_syst];

      for (int k = 0; k < n_syst; k++)
      {
        histo_closure_bvsc[i][j][k] = new TH1D **[n_flavor];
        histo_closure_cvsb[i][j][k] = new TH1D **[n_flavor];
        histo_closure_cvsl[i][j][k] = new TH1D **[n_flavor];
        histo_closure_eta[i][j][k] = new TH1D **[n_flavor];
        histo_closure_pt[i][j][k] = new TH1D **[n_flavor];

        for (int l = 0; l < n_flavor; l++)
        {
          histo_closure_bvsc[i][j][k][l] = new TH1D *[3];
          histo_closure_cvsb[i][j][k][l] = new TH1D *[3];
          histo_closure_cvsl[i][j][k][l] = new TH1D *[3];
          histo_closure_eta[i][j][k][l] = new TH1D *[3];
          histo_closure_pt[i][j][k][l] = new TH1D *[3];

          for (int m = 0; m < 3; m++)
          {
            TString syst_name;
            if (tagger == "B_Tagger")
              syst_name = syst_name_b[k];
            else if (tagger == "C_Tagger")
              syst_name = syst_name_c[k];

            TString histo_name_base = "Closure_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + syst_name + "_" + flavor_name[l];

            TString histo_name = histo_name_base + "_Leading_Jet_BvsC_" + to_string(m);
            histo_closure_bvsc[i][j][k][l][m] = new TH1D(histo_name, histo_name, 25, 0, 1);

            histo_name = histo_name_base + "_Leading_Jet_CvsB_" + to_string(m);
            histo_closure_cvsb[i][j][k][l][m] = new TH1D(histo_name, histo_name, 25, 0, 1);

            histo_name = histo_name_base + "_Leading_Jet_CvsL_" + to_string(m);
            histo_closure_cvsl[i][j][k][l][m] = new TH1D(histo_name, histo_name, 25, 0, 1);

            histo_name = histo_name_base + "_Leading_Jet_Eta_" + to_string(m);
            histo_closure_eta[i][j][k][l][m] = new TH1D(histo_name, histo_name, 12, -3, 3);

            histo_name = histo_name_base + "_Leading_Jet_Pt_" + to_string(m);
            histo_closure_pt[i][j][k][l][m] = new TH1D(histo_name, histo_name, 25, 0, 500);
          } // loop over 3
        } // loop over n_flavor
      } // loop over n_syst
    } // loop over n_sample_merge_mc
  } // loop over n_region

  histo_rf = new TH1D ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_rf[i] = new TH1D **[2];
    for (int j = 0; j < 2; j++)
    {
      histo_rf[i][j] = new TH1D *[n_syst_c];
      for (int k = 0; k < n_syst_c; k++)
      {
        TString process_name;
        if (j == 0)
          process_name = "ttbb";
        else if (j == 1)
          process_name = "ttcc";

        TString histo_name = "RF_" + region_name[i] + "_" + process_name + "_" + syst_name_c[k];
        histo_rf[i][j][k] = new TH1D(histo_name, histo_name, 80, 0, 2);
      } // loop over n_syst
    } // loop over ttbb or ttcc
  } // loop over n_region

  cout << "[Tagging_RF_Flavor::Setup_Histo_Validation]: Done" << endl;

  return;
} // void Tagging_RF_Flavor::Setup_Histo_Validation()

//////////

void Tagging_RF_Flavor::Setup_Tree(TTree *tree, const TString &syst)
{
  tree->SetBranchAddress("weight_hem_veto", &weight_hem_veto);
  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);

  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  if (syst == "Central")
  {
    tree->SetBranchAddress("weight_pileup_down", &weight_pileup_down);
    tree->SetBranchAddress("weight_pileup_up", &weight_pileup_up);
  }

  if (syst == "Central")
    tree->SetBranchAddress("weight_ps", weight_ps);

  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);
  tree->SetBranchAddress("weight_pujet_veto", &weight_pujet_veto);

  if (syst == "Central")
  {
    tree->SetBranchAddress("weight_scale_variation_1", &weight_scale_variation_1);
    tree->SetBranchAddress("weight_scale_variation_2", &weight_scale_variation_2);
    tree->SetBranchAddress("weight_scale_variation_3", &weight_scale_variation_3);
    tree->SetBranchAddress("weight_scale_variation_4", &weight_scale_variation_4);
    tree->SetBranchAddress("weight_scale_variation_6", &weight_scale_variation_6);
    tree->SetBranchAddress("weight_scale_variation_8", &weight_scale_variation_8);
  }

  tree->SetBranchAddress("sf_mu_id", &sf_mu_id);
  tree->SetBranchAddress("sf_mu_iso", &sf_mu_iso);
  tree->SetBranchAddress("sf_el_id", &sf_el_id);
  tree->SetBranchAddress("sf_el_reco", &sf_el_reco);

  tree->SetBranchAddress("sf_sl_trig", &sf_sl_trig);

  if (syst == "Central")
  {
    tree->SetBranchAddress("weight_b_tag", &weight_b_tag);
    tree->SetBranchAddress("weight_b_tag_down_hf", &weight_b_tag_hf_down);
    tree->SetBranchAddress("weight_b_tag_up_hf", &weight_b_tag_hf_up);
    tree->SetBranchAddress("weight_b_tag_down_lf", &weight_b_tag_lf_down);
    tree->SetBranchAddress("weight_b_tag_up_lf", &weight_b_tag_lf_up);
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
  }
  else if (syst == "JetEnDown")
    tree->SetBranchAddress("weight_b_tag_down_jes", &weight_b_tag_jes_down);
  else if (syst == "JetEnUp")
    tree->SetBranchAddress("weight_b_tag_up_jes", &weight_b_tag_jes_up);

  if (syst == "Central")
  {
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
  }
  else if (syst == "JetResDown")
    tree->SetBranchAddress("weight_c_tag_down_jer", &weight_c_tag_jer_down);
  else if (syst == "JetResUp")
    tree->SetBranchAddress("weight_c_tag_up_jer", &weight_c_tag_jer_up);
  else if (syst.Contains("JetEn") && syst.Contains("Down"))
    tree->SetBranchAddress("weight_c_tag_down_jes_total", &weight_c_tag_jes_total_down);
  else if (syst.Contains("JetEn") && syst.Contains("Up"))
    tree->SetBranchAddress("weight_c_tag_up_jes_total", &weight_c_tag_jes_total_up);

  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("ht", &ht);

  tree->SetBranchAddress("n_vertex", &n_pv);
  tree->SetBranchAddress("n_pileup", &n_pileup);

  tree->SetBranchAddress("lepton_eta", &lepton_eta);
  tree->SetBranchAddress("lepton_rel_iso", &lepton_rel_iso);
  tree->SetBranchAddress("lepton_pt_uncorr", &lepton_pt_uncorr);

  tree->SetBranchAddress("leading_jet_bvsc", &leading_jet_bvsc);
  tree->SetBranchAddress("leading_jet_cvsb", &leading_jet_cvsb);
  tree->SetBranchAddress("leading_jet_cvsl", &leading_jet_cvsl);
  tree->SetBranchAddress("leading_jet_eta", &leading_jet_eta);
  tree->SetBranchAddress("leading_jet_pt", &leading_jet_pt);

  tree->SetBranchAddress("subleading_jet_bvsc", &subleading_jet_bvsc);
  tree->SetBranchAddress("subleading_jet_cvsb", &subleading_jet_cvsb);
  tree->SetBranchAddress("subleading_jet_cvsl", &subleading_jet_cvsl);
  tree->SetBranchAddress("subleading_jet_eta", &subleading_jet_eta);
  tree->SetBranchAddress("subleading_jet_pt", &subleading_jet_pt);

  tree->SetBranchAddress("met_pt", &met_pt);

  tree->SetBranchAddress("decay_mode", &decay_mode);

  tree->SetBranchAddress("genTtbarId", &gen_ttbar_id);

  // tree->SetBranchAddress("Gen_HF_Flavour", &vec_gen_hf_flavour);
  // tree->SetBranchAddress("Gen_HF_Origin", &vec_gen_hf_origin);
  // tree->SetBranchAddress("Sel_Gen_HF_Flavour", &vec_sel_gen_hf_flavour);
  // tree->SetBranchAddress("Sel_Gen_HF_Origin", &vec_sel_gen_hf_origin);

  tree->SetBranchAddress("Jet_Pt", &vec_jet_pt);
  tree->SetBranchAddress("Jet_Eta", &vec_jet_eta);
  tree->SetBranchAddress("Jet_Flavor", &vec_jet_flavor);
  tree->SetBranchAddress("Jet_BvsC", &vec_jet_bvsc);
  tree->SetBranchAddress("Jet_CvsB", &vec_jet_cvsb);
  tree->SetBranchAddress("Jet_CvsL", &vec_jet_cvsl);

  return;
} // void Tagging_RF_Flavor::Setup_Tree()

//////////

int Tagging_RF_Flavor::Set_ABCD_Region()
{
  bool chk_pass_iso;
  if (channel == "Mu")
  {
    if (lepton_rel_iso < REL_ISO_MUON)
      chk_pass_iso = true;
    else
      chk_pass_iso = false;
  }
  else if (channel == "El")
  {
    float rel_iso_electron_a;
    float rel_iso_electron_b;
    if (TMath::Abs(lepton_eta) <= 1.479)
    {
      rel_iso_electron_a = REL_ISO_ELECTRON_BARREL_A;
      rel_iso_electron_b = REL_ISO_ELECTRON_BARREL_B;
    }
    else
    {
      rel_iso_electron_a = REL_ISO_ELECTRON_ENDCAP_A;
      rel_iso_electron_b = REL_ISO_ELECTRON_ENDCAP_B;
    }

    if (lepton_rel_iso < rel_iso_electron_a + rel_iso_electron_b / lepton_pt_uncorr)
      chk_pass_iso = true;
    else
      chk_pass_iso = false;
  }

  if (MET_PT < met_pt)
  {
    if (chk_pass_iso)
      return 3; //"D"
    else
      return 1; //"B"
  }
  else
  {
    if (chk_pass_iso)
      return 2; //"C"
    else
      return 0; //"A"
  }

  return -1;
} // int Tagging_RF_Flavor::Set_ABCD_Region()

//////////

int Tagging_RF_Flavor::TTHF_Breakdown_Index(const TString &sample_name)
{
  int index = -999;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    bool chk_b = false;
    bool chk_c = false;

    cout << gen_ttbar_id << endl;

    if (51 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 55)
      chk_b = true;
    else if (41 <= gen_ttbar_id % 100 && gen_ttbar_id % 100 <= 45)
      chk_c = true;

    if (chk_b)
      return 0;
    else if (chk_c)
      return 1;
    else
      return -1;
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = -1;
  // cout << "test Histo_Index: " << sample_name << " " << index << endl;

  return index;
} // int Tagging_RF_Flavor::TTHF_Breakdown_Index(const TString &sample_name)

//////////