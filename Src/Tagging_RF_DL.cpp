#include "Tagging_RF_DL.h"

ClassImp(Tagging_RF_DL);

//////////

Tagging_RF_DL::Tagging_RF_DL(const TString &a_era, const TString &a_mode, const TString &a_channel, const TString &a_tagger, const int &a_index_tree_type, const TString &a_extension) : samples(a_era, a_channel, "Vcb_Tagging_RF_DL")
{
  ROOT::EnableImplicitMT(10);

  TH1::AddDirectory(kFALSE);

  cout << "[Tagging_RF_DL::Tagging_RF_DL]: Init analysis" << endl;

  reduction = 1;

  era = a_era;
  mode = a_mode;
  channel = a_channel;
  tagger = a_tagger;
  extension = a_extension;

  index_tree_type = a_index_tree_type;

  if (tagger.Contains("C"))
    tagger = "C_Tagger";
  else if (tagger.Contains("B"))
    tagger = "B_Tagger";

  if (era.Contains("2016"))
    year = "2016";
  else
    year = era;

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
  } //  if (chk_jes_breakdown == true)

  // Central should be the last
  vec_tree_type.push_back("Central");

  n_tree_type = vec_tree_type.size();

  syst_name_b = {"B_Tag_Nominal",
                 "B_Tag_HF_Down", "B_Tag_HF_Up",
                 "B_Tag_LF_Down", "B_Tag_LF_Up",
                 "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up",
                 "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up",
                 "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up",
                 "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up",
                 "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up",
                 "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up",
                 "B_Tag_JES_Total_Down", "B_Tag_JES_Total_Up"};
  if (chk_jes_breakdown == true)
  {
    vector<TString> syst_name_b_jes_breakdown = {"B_Tag_JES_Absolute_Down", "B_Tag_JES_Absolute_Up",
                                                 "B_Tag_JES_BBEC1_Down", "B_Tag_JES_BBEC1_Up",
                                                 "B_Tag_JES_EC2_Down", "B_Tag_JES_EC2_Up",
                                                 "B_Tag_JES_FlavorQCD_Down", "B_Tag_JES_FlavorQCD_Up",
                                                 "B_Tag_JES_HF_Down", "B_Tag_JES_HF_Up",
                                                 "B_Tag_JES_RelativeBal_Down", "B_Tag_JES_RelativeBal_Up"};

    vector<TString> syst_name_b_jes_breakdown_year = {"B_Tag_JES_Absolute", "B_Tag_JES_BBEC1", "B_Tag_JES_EC2", "B_Tag_JES_HF", "B_Tag_JES_RelativeSample"};
    for (unsigned int i = 0; i < syst_name_b_jes_breakdown_year.size(); i++)
    {
      syst_name_b_jes_breakdown.push_back(syst_name_b_jes_breakdown_year[i] + year + "_Down");
      syst_name_b_jes_breakdown.push_back(syst_name_b_jes_breakdown_year[i] + year + "_Up");
    }

    syst_name_b.insert(syst_name_b.end(), syst_name_b_jes_breakdown.begin(), syst_name_b_jes_breakdown.end());
  }
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
    cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  if (chk_tthf_breakdown)
  {
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

    // TT systematic
    // TTLJ
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_hdampDown"));
    vec_short_name_mc.push_back("TTLJ_hdampDown_JJ_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_JJ_4");
    vec_short_name_mc.push_back("TTLJ_hdampDown_CC_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_CC_4");
    vec_short_name_mc.push_back("TTLJ_hdampDown_BB_2");
    vec_short_name_mc.push_back("TTLJ_hdampDown_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_hdampUp"));
    vec_short_name_mc.push_back("TTLJ_hdampUp_JJ_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_JJ_4");
    vec_short_name_mc.push_back("TTLJ_hdampUp_CC_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_CC_4");
    vec_short_name_mc.push_back("TTLJ_hdampUp_BB_2");
    vec_short_name_mc.push_back("TTLJ_hdampUp_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_CP5Down"));
    vec_short_name_mc.push_back("TTLJ_CP5Down_JJ_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_JJ_4");
    vec_short_name_mc.push_back("TTLJ_CP5Down_CC_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_CC_4");
    vec_short_name_mc.push_back("TTLJ_CP5Down_BB_2");
    vec_short_name_mc.push_back("TTLJ_CP5Down_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_CP5Up"));
    vec_short_name_mc.push_back("TTLJ_CP5Up_JJ_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_JJ_4");
    vec_short_name_mc.push_back("TTLJ_CP5Up_CC_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_CC_4");
    vec_short_name_mc.push_back("TTLJ_CP5Up_BB_2");
    vec_short_name_mc.push_back("TTLJ_CP5Up_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_mtop171p5"));
    vec_short_name_mc.push_back("TTLJ_mtop171p5_JJ_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_JJ_4");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_CC_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_CC_4");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_BB_2");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_BB_4");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_mtop173p5"));
    vec_short_name_mc.push_back("TTLJ_mtop173p5_JJ_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_JJ_4");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_CC_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_CC_4");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_BB_2");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_BB_4");

    // TTLL
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_hdampDown"));
    vec_short_name_mc.push_back("TTLL_hdampDown_JJ");
    vec_short_name_mc.push_back("TTLL_hdampDown_CC");
    vec_short_name_mc.push_back("TTLL_hdampDown_BB");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_hdampUp"));
    vec_short_name_mc.push_back("TTLL_hdampUp_JJ");
    vec_short_name_mc.push_back("TTLL_hdampUp_CC");
    vec_short_name_mc.push_back("TTLL_hdampUp_BB");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_CP5Down"));
    vec_short_name_mc.push_back("TTLL_CP5Down_JJ");
    vec_short_name_mc.push_back("TTLL_CP5Down_CC");
    vec_short_name_mc.push_back("TTLL_CP5Down_BB");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_CP5Up"));
    vec_short_name_mc.push_back("TTLL_CP5Up_JJ");
    vec_short_name_mc.push_back("TTLL_CP5Up_CC");
    vec_short_name_mc.push_back("TTLL_CP5Up_BB");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_mtop171p5"));
    vec_short_name_mc.push_back("TTLL_mtop171p5_JJ");
    vec_short_name_mc.push_back("TTLL_mtop171p5_CC");
    vec_short_name_mc.push_back("TTLL_mtop171p5_BB");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_mtop173p5"));
    vec_short_name_mc.push_back("TTLL_mtop173p5_JJ");
    vec_short_name_mc.push_back("TTLL_mtop173p5_CC");
    vec_short_name_mc.push_back("TTLL_mtop173p5_BB");

    // TTLJ_WtoCB
    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_hdampDown"));
    vec_short_name_mc.push_back("TTLJ_hdampDown_JJ_45");
    vec_short_name_mc.push_back("TTLJ_hdampDown_CC_45");
    vec_short_name_mc.push_back("TTLJ_hdampDown_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_hdampUp"));
    vec_short_name_mc.push_back("TTLJ_hdampUp_JJ_45");
    vec_short_name_mc.push_back("TTLJ_hdampUp_CC_45");
    vec_short_name_mc.push_back("TTLJ_hdampUp_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_CP5Down"));
    vec_short_name_mc.push_back("TTLJ_CP5Down_JJ_45");
    vec_short_name_mc.push_back("TTLJ_CP5Down_CC_45");
    vec_short_name_mc.push_back("TTLJ_CP5Down_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_CP5Up"));
    vec_short_name_mc.push_back("TTLJ_CP5Up_JJ_45");
    vec_short_name_mc.push_back("TTLJ_CP5Up_CC_45");
    vec_short_name_mc.push_back("TTLJ_CP5Up_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_mtop171p5"));
    vec_short_name_mc.push_back("TTLJ_mtop171p5_JJ_45");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_CC_45");
    vec_short_name_mc.push_back("TTLJ_mtop171p5_BB_45");

    vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_WtoCB_mtop173p5"));
    vec_short_name_mc.push_back("TTLJ_mtop173p5_JJ_45");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_CC_45");
    vec_short_name_mc.push_back("TTLJ_mtop173p5_BB_45");
  } // if (chk_tthf_breakdown)
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
  for (int i = 0; i < vec_short_name_mc.size(); i++)
  {
    cout << vec_short_name_mc[i] << endl;
  }

  if (mode == "Analysis")
    Run_Analysis();
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

} // Tagging_RF_DL::Tagging_RF_DL(const TString& a_era, const TString& a_channel)

//////////

Tagging_RF_DL::~Tagging_RF_DL()
{
  cout << "[Tagging_RF_DL::~Tagging_RF_DL]: Init" << endl;

  if (mode == "Analysis")
  {
    TString fout_name;
    if (index_tree_type == -1)
      fout_name = "Vcb_Tagging_RF_DL_" + era + "_" + channel + ".root";
    else
      fout_name = "Vcb_Tagging_RF_DL_" + era + "_" + channel + "_" + vec_tree_type[0] + ".root";
    fout = new TFile(fout_name, "RECREATE");

    fout->cd();

    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      TDirectory *dir = fout->mkdir(vec_short_name_mc[i]);
      dir->cd();

      for (int j = 0; j < n_syst_b; j++)
      {
        histo_mc_before_b[i][j]->Write();
        histo_mc_after_b[i][j]->Write();
      }
      for (int j = 0; j < n_syst_c; j++)
      {
        histo_mc_before_c[i][j]->Write();
        histo_mc_after_c[i][j]->Write();
      }

      for (int j = 0; j < n_syst_b; j++)
        ratio_b[i][j]->Write();
      for (int j = 0; j < n_syst_c; j++)
        ratio_c[i][j]->Write();

      if (chk_draw_called)
      {
        canvas_before_b[i]->Write();
        canvas_before_c[i]->Write();

        for (int j = 0; j < n_syst_b; j++)
        {
          canvas_after_b[i][j]->Write();
          canvas_ratio_b[i][j]->Write();
        }

        for (int j = 0; j < n_syst_c; j++)
        {
          canvas_after_c[i][j]->Write();
          canvas_ratio_c[i][j]->Write();
        }
      }
    }
    // stack_mc_before->Write();
    // for (int i = 0; i < n_syst_b; i++)
    //   stack_mc_after_b[i]->Write();
    // for (int i = 0; i < n_syst_c; i++)
    //   stack_mc_after_c[i]->Write();

    fout->Close();
  } // if (mode=="Analysis")
  else if (mode == "Combine")
  {
    fout->Close();
  }
  else if (mode == "Validation")
  {
    TString fout_name = "Vcb_Tagging_RF_DL_Validation_" + era + "_" + channel + "_" + tagger + ".root";
    fout = new TFile(fout_name, "RECREATE");

    fout->cd();
    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      TDirectory *dir = fout->mkdir(vec_short_name_mc[i]);

      int n_syst;
      if (tagger == "B_Tagger")
        n_syst = n_syst_b;
      else if (tagger == "C_Tagger")
        n_syst = n_syst_c;

      for (int j = 0; j < n_syst; j++)
      {
        for (int k = 0; k < n_flavor; k++)
        {
          for (int l = 0; l < 3; l++)
          {
            dir->cd();

            histo_closure_n_jet[i][j][k][l]->Write();
            histo_closure_ht[i][j][k][l]->Write();
            histo_closure_n_pileup[i][j][k][l]->Write();

            histo_closure_bvsc[i][j][k][l]->Write();
            histo_closure_cvsb[i][j][k][l]->Write();
            histo_closure_cvsl[i][j][k][l]->Write();

            histo_closure_eta[i][j][k][l]->Write();
            histo_closure_pt[i][j][k][l]->Write();
          }
        }
      }
    }

    fout->Close();
  } // else if (mode == "Validation")
  else if (mode == "Application")
  {
    fin->Close();
  } // else if (mode == "Application")
} // Tagging_RF_DL::~Tagging_RF_DL()

//////////

float Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag(const TString &sample, const TString &syst, const int &n_jets_, const float &ht_)
{
  if (!syst.Contains("B_Tag"))
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag] Input B_Tag" << endl;
    exit(1);

    return -1;
  }

  int sample_index = find(vec_sample_tagging_rf.begin(), vec_sample_tagging_rf.end(), sample) - vec_sample_tagging_rf.begin();
  int syst_index = find(syst_name_b.begin(), syst_name_b.end(), syst) - syst_name_b.begin();

  if (sample_index == vec_sample_tagging_rf.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_b.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  float n_jets = n_jets_;
  float ht = ht_;

  // handle underflow
  if (n_jets_ < bin_njet[0])
    n_jets = bin_njet[0] + 0.1;
  if (ht_ < bin_ht[0])
    ht = bin_ht[0] + 0.1;

  // handle overflow
  if (bin_njet[bin_njet.size() - 1] < n_jets_)
    n_jets = bin_njet[bin_njet.size() - 1] - 0.1;
  if (bin_ht[bin_ht.size() - 1] < ht_)
    ht = bin_ht[bin_ht.size() - 1] - 0.1;

  int bin = ratio_b[sample_index][syst_index]->FindBin(n_jets, ht);

  return ratio_b[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag(const TString &syst_name, const int &n_jet)

//////////

float Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag(const TString &sample, const TString &syst, const int &n_pileup_, const float &ht_)
{
  if (!syst.Contains("C_Tag"))
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag] Input C_Tag" << endl;
    exit(1);
  }

  int sample_index = find(vec_sample_tagging_rf.begin(), vec_sample_tagging_rf.end(), sample) - vec_sample_tagging_rf.begin();
  int syst_index = find(syst_name_c.begin(), syst_name_c.end(), syst) - syst_name_c.begin();

  if (sample_index == vec_sample_tagging_rf.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_c.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  float n_pileup = n_pileup_;
  float ht = ht_;

  // handle underflow
  if (n_pileup_ < bin_n_pileup[0])
    n_pileup = bin_n_pileup[0] + 0.1;
  if (ht_ < bin_ht[0])
    ht = bin_ht[0] + 0.1;

  // handle overflow
  if (bin_n_pileup[bin_n_pileup.size() - 1] < n_pileup_)
    n_pileup = bin_n_pileup[bin_n_pileup.size() - 1] - 0.1;
  if (bin_ht[bin_ht.size() - 1] < ht_)
    ht = bin_ht[bin_ht.size() - 1] - 0.1;

  int bin = ratio_c[sample_index][syst_index]->FindBin(n_pileup, ht);

  return ratio_c[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag(const TString &syst_name, const int &n_pileup, const float& ht)

//////////

void Tagging_RF_DL::Combine_Decay_Mode()
{
  cout << "[Tagging_RF_DL::Combine_Decay_Mode]: Init" << endl;

  vector<TString> vec_sample = {"TTLJ_JJ", "TTLJ_CC", "TTLJ_BB"};

  vector<TString> vec_syst_name;
  for (unsigned i = 0; i < syst_name_b.size(); i++)
    vec_syst_name.push_back(syst_name_b[i]);
  for (unsigned i = 0; i < syst_name_c.size(); i++)
    vec_syst_name.push_back(syst_name_c[i]);

  for (unsigned int i = 0; i < vec_sample.size(); i++)
  {
    TDirectory *dir = fout->mkdir(vec_sample[i]);

    for (unsigned int j = 0; j < vec_syst_name.size(); j++)
    {
      cout << vec_sample[i] << " " << vec_syst_name[j] << endl;

      TH2D *histo_2 = (TH2D *)fout->Get(Form("%s_2/Ratio_%s_2_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_4 = (TH2D *)fout->Get(Form("%s_4/Ratio_%s_4_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_45 = (TH2D *)fout->Get(Form("%s_45/Ratio_%s_45_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));

      // consistency check
      if (vec_syst_name[j].Contains("C_Tag"))
      {
        cout << "Chi2 Prob. (2, 4) = " << histo_2->Chi2Test(histo_4, "WW") << ", KS (2, 45) = " << histo_2->Chi2Test(histo_45, "WW") << ", S (4, 45) = " << histo_4->Chi2Test(histo_45, "WW") << endl;
      }

      TH2D *histo_2_before = (TH2D *)fout->Get(Form("%s_2/%s_2_%s_Before", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_4_before = (TH2D *)fout->Get(Form("%s_4/%s_4_%s_Before", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_45_before = (TH2D *)fout->Get(Form("%s_45/%s_45_%s_Before", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));

      TH2D *histo_2_after = (TH2D *)fout->Get(Form("%s_2/%s_2_%s_After", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_4_after = (TH2D *)fout->Get(Form("%s_4/%s_4_%s_After", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_45_after = (TH2D *)fout->Get(Form("%s_45/%s_45_%s_After", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));

      TH2D *histo_before_combine = (TH2D *)histo_2_before->Clone();
      histo_before_combine->Add(histo_4_before);
      histo_before_combine->Add(histo_45_before);

      TH2D *histo_after_combine = (TH2D *)histo_2_after->Clone();
      histo_after_combine->Add(histo_4_after);
      histo_after_combine->Add(histo_45_after);

      TH1 *ratio_average = (TH2 *)histo_2->Clone();
      ratio_average->Reset();

      ratio_average->Divide(histo_before_combine, histo_after_combine);

      dir->cd();
      ratio_average->SetTitle("Ratio_" + vec_sample[i] + "_" + vec_syst_name[j]);
      ratio_average->SetName("Ratio_" + vec_sample[i] + "_" + vec_syst_name[j]);
      ratio_average->Write();

      histo_before_combine->SetTitle(vec_sample[i] + "_" + vec_syst_name[j] + "_Before");
      histo_before_combine->SetName(vec_sample[i] + "_" + vec_syst_name[j] + "_Before");
      histo_before_combine->Write();

      histo_after_combine->SetTitle(vec_sample[i] + "_" + vec_syst_name[j] + "_After");
      histo_after_combine->SetName(vec_sample[i] + "_" + vec_syst_name[j] + "_After");
      histo_after_combine->Write();
    }
  }

  cout << "[Tagging_RF_DL::Combine_Decay_Mode]: Done" << endl;

  return;
} // void Tagging_RF_DL::Combine_Decay_Mode()

//////////

void Tagging_RF_DL::Combine_Lepton_Channel()
{
  cout << "[Tagging_RF_DL::Combine_Lepton_Channel]: Init" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF_DL/";

  fin_mm = new TFile(path_base + "Vcb_Tagging_RF_DL_" + era + "_MM.root");
  fin_me = new TFile(path_base + "Vcb_Tagging_RF_DL_" + era + "_ME.root");
  fin_ee = new TFile(path_base + "Vcb_Tagging_RF_DL_" + era + "_EE.root");

  TString fout_name = "Vcb_Tagging_RF_DL_" + era + ".root";
  fout = new TFile(fout_name, "RECREATE");

  TList *list_dir_mm = fin_mm->GetListOfKeys();
  for (int i = 0; i < list_dir_mm->GetEntries(); i++)
  {
    TObject *dir = (TObject *)list_dir_mm->At(i);
    TString dir_name = dir->GetName();
    TList *list_key = ((TDirectory *)fin_mm->Get(dir_name))->GetListOfKeys();

    TDirectory *dir_fout;

    fout->cd();
    dir_fout = fout->mkdir(dir_name);

    for (int j = 0; j < list_key->GetEntries(); j++)
    {
      TObject *key = (TObject *)list_key->At(j);
      TString key_name = key->GetName();

      if (key_name.Contains("Ratio_"))
      {
        TString histo_name = dir_name + "/" + key_name;
        cout << dir_name << " " << key_name << " " << histo_name << endl;

        TH2D *ratio_mm;
        TH2D *ratio_me;
        TH2D *ratio_ee;

        ratio_mm = (TH2D *)fin_mm->Get(histo_name);
        ratio_me = (TH2D *)fin_me->Get(histo_name);
        ratio_ee = (TH2D *)fin_ee->Get(histo_name);

        // consistency check
        if (key_name.Contains("C_Tag"))
        {
          cout << "Chi2 Prob. (MM and EE): " << ratio_mm->Chi2Test(ratio_ee, "WW") << endl;
          cout << "Chi2 Prob. (MM and ME): " << ratio_mm->Chi2Test(ratio_me, "WW") << endl;
          cout << "Chi2 Prob. (EE and ME): " << ratio_ee->Chi2Test(ratio_me, "WW") << endl;
        }

        // combine
        TString sub = key_name.Remove(0, 6);
        sub = dir_name + "/" + sub;

        TH2D *histo_before_mm = (TH2D *)fin_mm->Get(sub + "_Before");
        TH2D *histo_after_mm = (TH2D *)fin_mm->Get(sub + "_After");

        TH2D *histo_before_me = (TH2D *)fin_me->Get(sub + "_Before");
        TH2D *histo_after_me = (TH2D *)fin_me->Get(sub + "_After");

        TH2D *histo_before_ee = (TH2D *)fin_ee->Get(sub + "_Before");
        TH2D *histo_after_ee = (TH2D *)fin_ee->Get(sub + "_After");

        TH2D *histo_before_combine = (TH2D *)histo_before_mm->Clone();
        histo_before_combine->Add(histo_before_me);
        histo_before_combine->Add(histo_before_ee);

        TH2D *histo_after_combine = (TH2D *)histo_after_mm->Clone();
        histo_after_combine->Add(histo_after_me);
        histo_after_combine->Add(histo_after_ee);

        TH2D *ratio_average = (TH2D *)ratio_mm->Clone();
        ratio_average->Reset();

        ratio_average->Divide(histo_before_combine, histo_after_combine);

        dir_fout->cd();
        histo_before_combine->Write();
        histo_after_combine->Write();
        ratio_average->Write();
      } // if (key_name.Contains("Ratio_"))
    } // loop over key entries
  } // loop over dir entries

  return;
} // void Tagging_RF_DL::Combine()

//////////

void Tagging_RF_DL::Draw_Result()
{
  cout << "[Tagging_RF_DL::Draw_Result]: Init" << endl;

  gStyle->SetPaintTextFormat("0.3f");
  gStyle->SetOptStat(0);

  /*
    vector<TString> syst_to_draw = {"B_Tag_Nominal",
                                    "B_Tag_HF_Down", "B_Tag_HF_Up",
                                    "B_Tag_JES_Down", "B_Tag_JES_Up",
                                    "B_Tag_LF_Down", "B_Tag_LF_Up",
                                    "B_Tag_LFStats1_Down", "B_Tag_LFStats1_Up",
                                    "B_Tag_LFStats2_Down", "B_Tag_LFStats2_Up",
                                    "B_Tag_CFErr1_Down", "B_Tag_CFErr1_Up",
                                    "B_Tag_CFErr2_Down", "B_Tag_CFErr2_Up",
                                    "B_Tag_HFStats1_Down", "B_Tag_HFStats1_Up",
                                    "B_Tag_HFStats2_Down", "B_Tag_HFStats2_Up"};
  */

  vector<TString> syst_to_draw = {"C_Tag_Nominal"};
  // "C_Tag_Extrap_Down", "C_Tag_Extrap_Up",
  // "C_Tag_Interp_Down", "C_Tag_Interp_Up",
  // "C_Tag_LHE_Scale_MuF_Down", "C_Tag_LHE_Scale_MuF_Up",
  // "C_Tag_LHE_Scale_MuR_Down", "C_Tag_LHE_Scale_MuR_Up",
  // "C_Tag_PS_FSR_Fixed_Down", "C_Tag_PS_FSR_Fixed_Up",
  // "C_Tag_PS_ISR_Fixed_Down", "C_Tag_PS_ISR_Fixed_Up",
  // "C_Tag_PU_Down", "C_Tag_PU_Up",
  // "C_Tag_Stat_Down", "C_Tag_Stat_Up",
  // "C_Tag_XSec_Br_Unc_DYJets_B_Down", "C_Tag_XSec_Br_Unc_DYJets_B_Up",
  // "C_Tag_XSec_Br_Unc_DYJets_C_Down", "C_Tag_XSec_Br_Unc_DYJets_C_Up",
  // "C_Tag_XSec_Br_Unc_WJets_C_Down", "C_Tag_XSec_Br_Unc_WJets_C_Up",
  // "C_Tag_JER_Down", "C_Tag_JER_Up",
  // "C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up"};

  vector<TString> sample_to_draw;

  // /* compare TTHF */
  // for (unsigned i = 0; i < syst_to_draw.size(); i++)
  // {
  //   TString can_name = Form("Comp_Tagging_RF_DL_TTHF_%s_%s", syst_to_draw[i].Data(), era.Data());
  //   TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
  //   canvas->Divide(2, 1);
  //   canvas->Draw();

  //   TH2D *histo[3];
  //   histo[0] = (TH2D *)fout->Get(Form("TTLL_JJ/Ratio_TTLL_JJ_%s", syst_to_draw[i].Data()));
  //   histo[1] = (TH2D *)fout->Get(Form("TTLL_CC/Ratio_TTLL_CC_%s", syst_to_draw[i].Data()));
  //   histo[2] = (TH2D *)fout->Get(Form("TTLL_BB/Ratio_TTLL_BB_%s", syst_to_draw[i].Data()));

  //   TH1D *histo_proj_x[3];
  //   TH1D *histo_proj_y[3];

  //   for (unsigned int j = 0; j < 3; j++)
  //   {
  //     histo_proj_x[j] = (TH1D *)histo[j]->ProjectionX()->Clone();
  //     histo_proj_y[j] = (TH1D *)histo[j]->ProjectionY()->Clone();

  //     histo_proj_x[j]->Scale(1. / histo[j]->GetNbinsY());
  //     histo_proj_y[j]->Scale(1. / histo[j]->GetNbinsX());

  //     histo_proj_x[j]->SetLineColor(j + 1);
  //     histo_proj_y[j]->SetLineColor(j + 1);

  //     canvas->cd(1);
  //     if (j == 0)
  //     {
  //       if (tagger == "B_Tagger")
  //         histo_proj_x[j]->GetXaxis()->SetTitle("Jet Multiplicity");
  //       else if (tagger == "C_Tagger")
  //         histo_proj_x[j]->GetXaxis()->SetTitle("NPV");
  //       histo_proj_x[j]->GetYaxis()->SetRangeUser(0.6, 1.4);
  //       histo_proj_x[j]->Draw();
  //     }
  //     histo_proj_x[j]->Draw("same");

  //     canvas->cd(2);
  //     if (j == 0)
  //     {
  //       histo_proj_y[j]->GetXaxis()->SetTitle("HT [GeV]");
  //       histo_proj_y[j]->GetYaxis()->SetRangeUser(0.6, 1.4);
  //       histo_proj_y[j]->Draw();
  //     }
  //     histo_proj_y[j]->Draw("same");
  //   } // loop over TTHH

  //   canvas->Print(can_name + "." + extension, extension);
  // } // for(unsigned i=0; i<syst_to_draw.size(); i++)

  /* compare decay mode */
  // w decay mode

  if (chk_tthf_breakdown)
    sample_to_draw = {"TTLJ_JJ", "TTLJ_CC", "TTLJ_BB"};
  else
    sample_to_draw = {"TTLJ"};

  for (unsigned int i = 0; i < sample_to_draw.size(); i++)
  {
    cout << sample_to_draw[i] << endl;

    for (unsigned int j = 0; j < syst_to_draw.size(); j++)
    // for (unsigned int j = 0; j < 1; j++)
    {
      cout << syst_to_draw[j] << endl;

      TString can_name = Form("Comp_Tagging_RF_DL_Decay_Mode_%s_%s_%s", sample_to_draw[i].Data(), syst_to_draw[j].Data(), era.Data());
      TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
      canvas->Divide(2, 1);
      canvas->Draw();

      TH2D *histo[3];
      histo[0] = (TH2D *)fout->Get(Form("%s_2/Ratio_%s_2_%s", sample_to_draw[i].Data(), sample_to_draw[i].Data(), syst_to_draw[j].Data()));
      histo[1] = (TH2D *)fout->Get(Form("%s_4/Ratio_%s_4_%s", sample_to_draw[i].Data(), sample_to_draw[i].Data(), syst_to_draw[j].Data()));
      histo[2] = (TH2D *)fout->Get(Form("%s_45/Ratio_%s_45_%s", sample_to_draw[i].Data(), sample_to_draw[i].Data(), syst_to_draw[j].Data()));

      TH1D *histo_proj_x[3];
      TH1D *histo_proj_y[3];

      for (unsigned int k = 0; k < 3; k++)
      {
        histo_proj_x[k] = (TH1D *)histo[k]->ProjectionX()->Clone();
        histo_proj_y[k] = (TH1D *)histo[k]->ProjectionY()->Clone();

        histo_proj_x[k]->Scale(1. / histo[k]->GetNbinsY());
        histo_proj_y[k]->Scale(1. / histo[k]->GetNbinsX());

        histo_proj_x[k]->SetLineColor(k + 1);
        histo_proj_y[k]->SetLineColor(k + 1);

        canvas->cd(1);
        if (k == 0)
        {
          if (tagger == "B_Tagger")
            histo_proj_x[k]->GetXaxis()->SetTitle("Jet Multiplicity");
          else if (tagger == "C_Tagger")
            histo_proj_x[k]->GetXaxis()->SetTitle("NPV");
          histo_proj_x[k]->GetYaxis()->SetRangeUser(0.6, 1.4);
          histo_proj_x[k]->Draw();
        }
        histo_proj_x[k]->Draw("same");

        canvas->cd(2);
        if (k == 0)
        {
          histo_proj_y[k]->GetXaxis()->SetTitle("HT [GeV]");
          histo_proj_y[k]->GetYaxis()->SetRangeUser(0.6, 1.4);
          histo_proj_y[k]->Draw();
        }
        histo_proj_y[k]->Draw("same");
      } // loop over decay mode

      canvas->Print(can_name + "." + extension, extension);
    }
  } // loop over sample

  if (chk_tthf_breakdown)
    sample_to_draw = {"TTLL_JJ", "TTLL_CC", "TTLL_BB",
                      "TTLJ_JJ", "TTLJ_CC", "TTLJ_BB"};
  else
    sample_to_draw = {"TTLL", "TTLJ_2"};

  for (unsigned int i = 0; i < sample_to_draw.size(); i++)
  {
    for (unsigned int j = 0; j < syst_to_draw.size(); j++)
    // for (unsigned int j = 0; j < 1; j++)
    {
      TH2D *histo = (TH2D *)fout->Get(Form("%s/Ratio_%s_%s", sample_to_draw[i].Data(), sample_to_draw[i].Data(), syst_to_draw[j].Data()));

      TString can_name = Form("Tagging_RF_DL_%s_%s_%s", sample_to_draw[i].Data(), syst_to_draw[j].Data(), era.Data());
      TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
      canvas->Draw();

      if (tagger == "B_Tagger")
        histo->GetXaxis()->SetTitle("Jet Multiplicity");
      else if (tagger == "C_Tagger")
        histo->GetXaxis()->SetTitle("Number of Primary Vertex");
      histo->GetYaxis()->SetTitle("HT [GeV]");
      histo->GetZaxis()->SetRangeUser(0.6, 1.1);
      histo->Draw("COLZ texte");

      canvas->Print(can_name + "." + extension, extension);
    }
  }

  /*
  //for (unsigned int i = 0; i < syst_to_draw.size(); i++)
  for (unsigned int i = 0; i < 1; i++)
  {
    TH2D *histo_jj = (TH2D *)fout->Get(Form("TTLL_JJ/Ratio_TTLL_JJ_%s", syst_to_draw[i].Data()));
    TH2D *histo_cc = (TH2D *)fout->Get(Form("TTLL_CC/Ratio_TTLL_CC_%s", syst_to_draw[i].Data()));
    TH2D *histo_bb = (TH2D *)fout->Get(Form("TTLL_BB/Ratio_TTLL_BB_%s", syst_to_draw[i].Data()));

    TH1D *histo_jj_proj_x = (TH1D *)histo_jj->ProjectionX()->Clone();
    TH1D *histo_cc_proj_x = (TH1D *)histo_cc->ProjectionX()->Clone();
    TH1D *histo_bb_proj_x = (TH1D *)histo_bb->ProjectionX()->Clone();

    TH1D *histo_jj_proj_y = (TH1D *)histo_jj->ProjectionY()->Clone();
    TH1D *histo_cc_proj_y = (TH1D *)histo_cc->ProjectionY()->Clone();
    TH1D *histo_bb_proj_y = (TH1D *)histo_bb->ProjectionY()->Clone();

    histo_jj_proj_x->Scale(1. / histo_jj->GetNbinsY());
    histo_cc_proj_x->Scale(1. / histo_cc->GetNbinsY());
    histo_bb_proj_x->Scale(1. / histo_bb->GetNbinsY());

    histo_jj_proj_y->Scale(1. / histo_jj->GetNbinsX());
    histo_cc_proj_y->Scale(1. / histo_cc->GetNbinsX());
    histo_bb_proj_y->Scale(1. / histo_bb->GetNbinsX());

    histo_jj_proj_x->SetLineColor(2);
    histo_cc_proj_x->SetLineColor(3);
    histo_bb_proj_x->SetLineColor(4);

    histo_jj_proj_y->SetLineColor(2);
    histo_cc_proj_y->SetLineColor(3);
    histo_bb_proj_y->SetLineColor(4);

    TString can_name = "Comp_Tagging_Patch_DL_" + syst_to_draw[i] + "_" + era;
    TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
    canvas->Divide(2, 1);

    histo_jj_proj_x->SetTitle(Form("Tagging Patch DL %s Projection to NPV", syst_to_draw[i].Data()));
    histo_jj_proj_x->GetXaxis()->SetTitle("Number of Primary Vertex");
    histo_jj_proj_x->GetYaxis()->SetRangeUser(0.7, 1.3);

    TLegend *tl = new TLegend(0.2, 0.2, 0.6, 0.35);
    tl->SetBorderSize(0);
    tl->AddEntry(histo_jj_proj_x, "TT+JJ", "lep");
    tl->AddEntry(histo_cc_proj_x, "TT+CC", "lep");
    tl->AddEntry(histo_bb_proj_x, "TT+BB", "lep");

    canvas->cd(1);
    histo_jj_proj_x->Draw();
    histo_cc_proj_x->Draw("SAMEP");
    histo_bb_proj_x->Draw("SAMEP");
    tl->Draw("SAME");

    histo_jj_proj_y->SetTitle(Form("Tagging Patch DL %s Projection to HT", syst_to_draw[i].Data()));
    histo_jj_proj_y->GetXaxis()->SetTitle("HT [GeV]");
    histo_jj_proj_y->GetYaxis()->SetRangeUser(0.7, 1.3);

    canvas->cd(2);
    histo_jj_proj_y->Draw();
    histo_cc_proj_y->Draw("SAMEP");
    histo_bb_proj_y->Draw("SAMEP");

    canvas->Print(can_name + "." + extension, extension);
  }
  */

  return;
} // void Tagging_RF_DL::Draw()

//////////

void Tagging_RF_DL::Draw_Validation()
{
  cout << "[Tagging_RF_DL::Draw_Validation]: Init" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF_DL";

  TFile *fin = new TFile(Form("%s/Vcb_Tagging_RF_DL_Validation_%s_%s_%s.root", path_base.Data(), era.Data(), channel.Data(), tagger.Data()));

  gStyle->SetOptStat(0);

  // vector<TString> vec_sample_to_draw = {"TTLJ_JJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
  //                                       "TTLJ_JJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
  //                                       "TTLJ_JJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
  //                                       "TTLL_JJ", "TTLL_CC", "TTLL_BB",
  //                                       "TTLL_hdampDown_JJ", "TTLL_hdampDown_CC", "TTLL_hdampDown_BB",
  //                                       "TTLL_hdampUp_JJ", "TTLL_hdampUp_CC", "TTLL_hdampUp_BB",
  //                                       "TTLL_CP5Down_JJ", "TTLL_CP5Down_CC", "TTLL_CP5Down_BB",
  //                                       "TTLL_CP5Up_JJ", "TTLL_CP5Up_CC", "TTLL_CP5Up_BB",
  //                                       "TTLL_mtop171p5_JJ", "TTLL_mtop171p5_CC", "TTLL_mtop171p5_BB",
  //                                       "TTLL_mtop173p5_JJ", "TTLL_mtop173p5_CC", "TTLL_mtop173p5_BB"};
  vector<TString> vec_sample_to_draw = {"TTLL"};

  vector<TString> vec_syst_to_draw;
  vec_syst_to_draw = {"C_Tag_Nominal",
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
    // for (int j = 0; j < 3; j++)
    {
      // TString syst_name;
      // if (tagger == "B_Tagger")
      //   syst_name = syst_name_b[j];
      // else if (tagger == "C_Tagger")
      //   syst_name = syst_name_c[j];

      TString syst_name = vec_syst_to_draw[j];

      TString can_name = "Validation_DL_" + channel + "_" + syst_name + "_" + sample_to_draw + "_" + era;
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
            TString histo_name = "Closure_" + sample_to_draw + "_" + syst_name + "_" + flavor_name[l] + "_Leading_Jet_" + score_type + "_" + to_string(m);
            cout << histo_name << endl;

            histo_closure[l][m] = (TH1D *)fin->Get(sample_to_draw + "/" + histo_name);
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

            if (m == 0 && l == 0)
            {
              if (k == 0)
                histo_closure_ratio[l][m]->GetXaxis()->SetTitle("BvsC");
              else if (k == 1)
                histo_closure_ratio[l][m]->GetXaxis()->SetTitle("CvsB");
              else if (k == 2)
                histo_closure_ratio[l][m]->GetXaxis()->SetTitle("CvsL");

              histo_closure_ratio[l][m]->GetYaxis()->SetTitle("Ratio");
              histo_closure_ratio[l][m]->GetYaxis()->SetRangeUser(0.4, 1.6);

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

  return;
} // void Tagging_RF_DL::Draw_Validation()

//////////

void Tagging_RF_DL::Fill_Histo_MC(const TString &sample_name, const TString &tree_type)
{
  int sample_index = Histo_Index(sample_name);

  float weight = 1;

  weight *= weight_hem_veto;
  weight *= weight_lumi;
  weight *= weight_mc;
  weight *= weight_pileup;
  weight *= weight_prefire;
  weight *= weight_top_pt;
  weight *= weight_pujet_veto;

  weight *= weight_sl_trig;

  weight *= weight_mu_id;
  weight *= weight_mu_iso;

  weight *= weight_el_id;
  weight *= weight_el_reco;

  if (TMath::IsNaN(weight) || TMath::IsNaN(weight_scale_variation_1) || TMath::IsNaN(weight_scale_variation_2) || TMath::IsNaN(weight_scale_variation_3) || TMath::IsNaN(weight_scale_variation_6))
    return;

  if (tree_type == "Central")
  {
    // norminal
    histo_mc_before_b[sample_index][0]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][0]->Fill(n_jets, ht, weight * weight_b_tag);

    // hf
    histo_mc_before_b[sample_index][1]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][1]->Fill(n_jets, ht, weight * weight_b_tag_hf_down);

    histo_mc_before_b[sample_index][2]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][2]->Fill(n_jets, ht, weight * weight_b_tag_hf_up);

    // lf
    histo_mc_before_b[sample_index][3]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][3]->Fill(n_jets, ht, weight * weight_b_tag_lf_down);

    histo_mc_before_b[sample_index][4]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][4]->Fill(n_jets, ht, weight * weight_b_tag_lf_up);

    // lfstats1
    histo_mc_before_b[sample_index][5]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][5]->Fill(n_jets, ht, weight * weight_b_tag_lfstats1_down);

    histo_mc_before_b[sample_index][6]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][6]->Fill(n_jets, ht, weight * weight_b_tag_lfstats1_up);

    // lfstats2
    histo_mc_before_b[sample_index][7]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][7]->Fill(n_jets, ht, weight * weight_b_tag_lfstats2_down);

    histo_mc_before_b[sample_index][8]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][8]->Fill(n_jets, ht, weight * weight_b_tag_lfstats2_up);

    // cferr1
    histo_mc_before_b[sample_index][9]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][9]->Fill(n_jets, ht, weight * weight_b_tag_cferr1_down);

    histo_mc_before_b[sample_index][10]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][10]->Fill(n_jets, ht, weight * weight_b_tag_cferr1_up);

    // cferr2
    histo_mc_before_b[sample_index][11]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][11]->Fill(n_jets, ht, weight * weight_b_tag_cferr2_down);

    histo_mc_before_b[sample_index][12]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][12]->Fill(n_jets, ht, weight * weight_b_tag_cferr2_up);

    // hfstats1
    histo_mc_before_b[sample_index][13]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][13]->Fill(n_jets, ht, weight * weight_b_tag_hfstats1_down);

    histo_mc_before_b[sample_index][14]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][14]->Fill(n_jets, ht, weight * weight_b_tag_hfstats1_up);

    // hfstats2
    histo_mc_before_b[sample_index][15]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][15]->Fill(n_jets, ht, weight * weight_b_tag_hfstats2_down);

    histo_mc_before_b[sample_index][16]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][16]->Fill(n_jets, ht, weight * weight_b_tag_hfstats2_up);
  }

  // jet en down
  else if (tree_type == "JetEnDown")
  {
    histo_mc_before_b[sample_index][17]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][17]->Fill(n_jets, ht, weight * weight_b_tag_jes_down);
  }

  // jet en up
  else if (tree_type == "JetEnUp")
  {
    histo_mc_before_b[sample_index][18]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][18]->Fill(n_jets, ht, weight * weight_b_tag_jes_up);
  }

  // jes break down
  if (chk_jes_breakdown == true)
  {
    if (tree_type == "JetEnAbsoluteDown")
    {
      histo_mc_before_b[sample_index][19]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][19]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnAbsoluteUp")
    {
      histo_mc_before_b[sample_index][20]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][20]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnBBEC1Down")
    {
      histo_mc_before_b[sample_index][21]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][21]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnBBEC1Up")
    {
      histo_mc_before_b[sample_index][22]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][22]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnEC2Down")
    {
      histo_mc_before_b[sample_index][23]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][23]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnEC2Up")
    {
      histo_mc_before_b[sample_index][24]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][24]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnFlavorQCDDown")
    {
      histo_mc_before_b[sample_index][25]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][25]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnFlavorQCDUp")
    {
      histo_mc_before_b[sample_index][26]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][26]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnHFDown")
    {
      histo_mc_before_b[sample_index][27]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][27]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnHFUp")
    {
      histo_mc_before_b[sample_index][28]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][28]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnRelativeBalDown")
    {
      histo_mc_before_b[sample_index][29]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][29]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnRelativeBalUp")
    {
      histo_mc_before_b[sample_index][30]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][30]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnAbsolute2016Down" || tree_type == "JetEnAbsolute2017Down" || tree_type == "JetEnAbsolute2018Down")
    {
      histo_mc_before_b[sample_index][31]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][31]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnAbsolute2016Up" || tree_type == "JetEnAbsolute2017Up" || tree_type == "JetEnAbsolute2018Up")
    {
      histo_mc_before_b[sample_index][32]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][32]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnBBEC12016Down" || tree_type == "JetEnBBEC12017Down" || tree_type == "JetEnBBEC12018Down")
    {
      histo_mc_before_b[sample_index][33]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][33]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnBBEC12016Up" || tree_type == "JetEnBBEC12017Up" || tree_type == "JetEnBBEC12018Up")
    {
      histo_mc_before_b[sample_index][34]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][34]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnEC22016Down" || tree_type == "JetEnEC22017Down" || tree_type == "JetEnEC22018Down")
    {
      histo_mc_before_b[sample_index][35]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][35]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnEC22016Up" || tree_type == "JetEnEC22017Up" || tree_type == "JetEnEC22018Up")
    {
      histo_mc_before_b[sample_index][36]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][36]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnHF2016Down" || tree_type == "JetEnHF2017Down" || tree_type == "JetEnHF2018Down")
    {
      histo_mc_before_b[sample_index][37]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][37]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnHF2016Up" || tree_type == "JetEnHF2017Up" || tree_type == "JetEnHF2018Up")
    {
      histo_mc_before_b[sample_index][38]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][38]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }

    else if (tree_type == "JetEnRelativeSample2016Down" || tree_type == "JetEnRelativeSample2017Down" || tree_type == "JetEnRelativeSample2018Down")
    {
      histo_mc_before_b[sample_index][39]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][39]->Fill(n_pileup, ht, weight * weight_b_tag_jes_down);
    }

    else if (tree_type == "JetEnRelativeSample2016Up" || tree_type == "JetEnRelativeSample2017Up" || tree_type == "JetEnRelativeSample2018Up")
    {
      histo_mc_before_b[sample_index][40]->Fill(n_pileup, ht, weight);
      histo_mc_after_b[sample_index][40]->Fill(n_pileup, ht, weight * weight_b_tag_jes_up);
    }
  } // if (chk_jes_breakdown == true)

  if (tree_type == "Central")
  {
    // norminal
    histo_mc_before_c[sample_index][0]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][0]->Fill(n_pileup, ht, weight * weight_c_tag);

    // extrap
    histo_mc_before_c[sample_index][1]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][1]->Fill(n_pileup, ht, weight * weight_c_tag_extrap_down);

    histo_mc_before_c[sample_index][2]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][2]->Fill(n_pileup, ht, weight * weight_c_tag_extrap_up);

    // interp
    histo_mc_before_c[sample_index][3]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][3]->Fill(n_pileup, ht, weight * weight_c_tag_interp_down);

    histo_mc_before_c[sample_index][4]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][4]->Fill(n_pileup, ht, weight * weight_c_tag_interp_up);

    // muf
    histo_mc_before_c[sample_index][5]->Fill(n_pileup, ht, weight * weight_scale_variation_2);
    histo_mc_after_c[sample_index][5]->Fill(n_pileup, ht, weight * weight_scale_variation_2 * weight_c_tag_lhe_scale_muf_down);

    histo_mc_before_c[sample_index][6]->Fill(n_pileup, ht, weight * weight_scale_variation_1);
    histo_mc_after_c[sample_index][6]->Fill(n_pileup, ht, weight * weight_scale_variation_1 * weight_c_tag_lhe_scale_muf_up);

    // mur
    histo_mc_before_c[sample_index][7]->Fill(n_pileup, ht, weight * weight_scale_variation_6);
    histo_mc_after_c[sample_index][7]->Fill(n_pileup, ht, weight * weight_scale_variation_6 * weight_c_tag_lhe_scale_mur_down);

    histo_mc_before_c[sample_index][8]->Fill(n_pileup, ht, weight * weight_scale_variation_3);
    histo_mc_after_c[sample_index][8]->Fill(n_pileup, ht, weight * weight_scale_variation_3 * weight_c_tag_lhe_scale_mur_up);

    // fsr
    histo_mc_before_c[sample_index][9]->Fill(n_pileup, ht, weight * weight_ps[0]);
    histo_mc_after_c[sample_index][9]->Fill(n_pileup, ht, weight * weight_ps[0] * weight_c_tag_ps_fsr_fixed_down);

    histo_mc_before_c[sample_index][10]->Fill(n_pileup, ht, weight * weight_ps[1]);
    histo_mc_after_c[sample_index][10]->Fill(n_pileup, ht, weight * weight_ps[1] * weight_c_tag_ps_fsr_fixed_up);

    // isr
    histo_mc_before_c[sample_index][11]->Fill(n_pileup, ht, weight * weight_ps[2]);
    histo_mc_after_c[sample_index][11]->Fill(n_pileup, ht, weight * weight_ps[2] * weight_c_tag_ps_isr_fixed_down);

    histo_mc_before_c[sample_index][12]->Fill(n_pileup, ht, weight * weight_ps[3]);
    histo_mc_after_c[sample_index][12]->Fill(n_pileup, ht, weight * weight_ps[3] * weight_c_tag_ps_isr_fixed_up);

    // pu
    histo_mc_before_c[sample_index][13]->Fill(n_pileup, ht, weight * weight_pileup_down / weight_pileup);
    histo_mc_after_c[sample_index][13]->Fill(n_pileup, ht, weight * weight_pileup_down / weight_pileup * weight_c_tag_pu_down);

    histo_mc_before_c[sample_index][14]->Fill(n_pileup, ht, weight * weight_pileup_up / weight_pileup);
    histo_mc_after_c[sample_index][14]->Fill(n_pileup, ht, weight * weight_pileup_up / weight_pileup * weight_c_tag_pu_up);

    // stat
    histo_mc_before_c[sample_index][15]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][15]->Fill(n_pileup, ht, weight * weight_c_tag_stat_down);

    histo_mc_before_c[sample_index][16]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][16]->Fill(n_pileup, ht, weight * weight_c_tag_stat_up);

    // unc_dyjets_b
    histo_mc_before_c[sample_index][17]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][17]->Fill(n_pileup, ht, weight * weight_c_tag_xsec_br_unc_dyjets_b_down);

    histo_mc_before_c[sample_index][18]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][18]->Fill(n_pileup, ht, weight * weight_c_tag_xsec_br_unc_dyjets_b_up);

    // unc_dyjets_c
    histo_mc_before_c[sample_index][19]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][19]->Fill(n_pileup, ht, weight * weight_c_tag_xsec_br_unc_dyjets_c_down);

    histo_mc_before_c[sample_index][20]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][20]->Fill(n_pileup, ht, weight * weight_c_tag_xsec_br_unc_dyjets_c_up);

    // unc_wjets_c
    histo_mc_before_c[sample_index][21]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][21]->Fill(n_pileup, ht, weight * weight_c_tag_xsec_br_unc_wjets_c_down);

    histo_mc_before_c[sample_index][22]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][22]->Fill(n_pileup, ht, weight * weight_c_tag_xsec_br_unc_wjets_c_up);
  }

  // jer
  else if (tree_type == "JetResDown")
  {
    histo_mc_before_c[sample_index][23]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][23]->Fill(n_pileup, ht, weight * weight_c_tag_jer_down);
  }

  else if (tree_type == "JetResUp")
  {
    histo_mc_before_c[sample_index][24]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][24]->Fill(n_pileup, ht, weight * weight_c_tag_jer_up);
  }

  // jes
  else if (tree_type == "JetEnDown")
  {
    histo_mc_before_c[sample_index][25]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][25]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
  }

  else if (tree_type == "JetEnUp")
  {
    histo_mc_before_c[sample_index][26]->Fill(n_pileup, ht, weight);
    histo_mc_after_c[sample_index][26]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
  }

  // jes break down
  if (chk_jes_breakdown == true)
  {
    if (tree_type == "JetEnAbsoluteDown")
    {
      histo_mc_before_c[sample_index][27]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][27]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnAbsoluteUp")
    {
      histo_mc_before_c[sample_index][28]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][28]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnBBEC1Down")
    {
      histo_mc_before_c[sample_index][29]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][29]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnBBEC1Up")
    {
      histo_mc_before_c[sample_index][30]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][30]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnEC2Down")
    {
      histo_mc_before_c[sample_index][31]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][31]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnEC2Up")
    {
      histo_mc_before_c[sample_index][32]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][32]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnFlavorQCDDown")
    {
      histo_mc_before_c[sample_index][33]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][33]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnFlavorQCDUp")
    {
      histo_mc_before_c[sample_index][34]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][34]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnHFDown")
    {
      histo_mc_before_c[sample_index][35]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][35]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnHFUp")
    {
      histo_mc_before_c[sample_index][36]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][36]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnRelativeBalDown")
    {
      histo_mc_before_c[sample_index][37]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][37]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnRelativeBalUp")
    {
      histo_mc_before_c[sample_index][38]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][38]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnAbsolute2016Down" || tree_type == "JetEnAbsolute2017Down" || tree_type == "JetEnAbsolute2018Down")
    {
      histo_mc_before_c[sample_index][39]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][39]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnAbsolute2016Up" || tree_type == "JetEnAbsolute2017Up" || tree_type == "JetEnAbsolute2018Up")
    {
      histo_mc_before_c[sample_index][40]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][40]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnBBEC12016Down" || tree_type == "JetEnBBEC12017Down" || tree_type == "JetEnBBEC12018Down")
    {
      histo_mc_before_c[sample_index][41]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][41]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnBBEC12016Up" || tree_type == "JetEnBBEC12017Up" || tree_type == "JetEnBBEC12018Up")
    {
      histo_mc_before_c[sample_index][42]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][42]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnEC22016Down" || tree_type == "JetEnEC22017Down" || tree_type == "JetEnEC22018Down")
    {
      histo_mc_before_c[sample_index][43]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][43]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnEC22016Up" || tree_type == "JetEnEC22017Up" || tree_type == "JetEnEC22018Up")
    {
      histo_mc_before_c[sample_index][44]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][44]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnHF2016Down" || tree_type == "JetEnHF2017Down" || tree_type == "JetEnHF2018Down")
    {
      histo_mc_before_c[sample_index][45]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][45]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnHF2016Up" || tree_type == "JetEnHF2017Up" || tree_type == "JetEnHF2018Up")
    {
      histo_mc_before_c[sample_index][46]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][46]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }

    else if (tree_type == "JetEnRelativeSample2016Down" || tree_type == "JetEnRelativeSample2017Down" || tree_type == "JetEnRelativeSample2018Down")
    {
      histo_mc_before_c[sample_index][47]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][47]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_down);
    }

    else if (tree_type == "JetEnRelativeSample2016Up" || tree_type == "JetEnRelativeSample2017Up" || tree_type == "JetEnRelativeSample2018Up")
    {
      histo_mc_before_c[sample_index][48]->Fill(n_pileup, ht, weight);
      histo_mc_after_c[sample_index][48]->Fill(n_pileup, ht, weight * weight_c_tag_jes_total_up);
    }
  } // if (chk_jes_breakdown == true)

  return;
} // void Tagging_RF_DL::Fill_Histo_MC()

//////////

void Tagging_RF_DL::Fill_Histo_Validation_MC_B_Tagger(const TString &sample_name, const TString &tree_type)
{
  int sample_index = Histo_Index(sample_name);
  TString histo_name_rf = Histo_Name_RF(sample_name);

  int leading_jet_flavor = vec_jet_flavor->at(0);
  int index_leading_jet_flavor = Flavor_Index(leading_jet_flavor);

  for (int i = 0; i < n_syst_b; i++)
  {
    TString b_tag_type = syst_name_b[i];

    if (tree_type == "Central")
    {
      if (b_tag_type.Contains("B_Tag_JES"))
        continue;
    }
    else if (tree_type.Contains("JetRes"))
      continue;
    else if (tree_type == "JetEnDown")
    {
      if (b_tag_type != "B_Tag_JES_Total_Down")
        continue;
    }
    else if (tree_type == "JetEnUp")
    {
      if (b_tag_type != "B_Tag_JES_Total_Up")
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

      if (b_tag_type != "B_Tag_JES_" + tmp + "_" + direction)
        continue;
    }

    float weight_raw = weight_hem_veto;
    weight_raw *= weight_lumi;
    weight_raw *= weight_mc;
    weight_raw *= weight_prefire;
    weight_raw *= weight_pujet_veto;
    weight_raw *= weight_top_pt;

    weight_raw *= weight_sl_trig;

    weight_raw *= weight_mu_id;
    weight_raw *= weight_mu_iso;

    weight_raw *= weight_el_id;
    weight_raw *= weight_el_reco;

    float weight_b_sf;
    if (b_tag_type == "B_Tag_Nominal")
      weight_b_sf = weight_b_tag;
    else if (b_tag_type == "B_Tag_HF_Down")
      weight_b_sf = weight_b_tag_hf_down;
    else if (b_tag_type == "B_Tag_HF_Up")
      weight_b_sf = weight_b_tag_hf_up;
    else if (b_tag_type == "B_Tag_LF_Down")
      weight_b_sf = weight_b_tag_lf_down;
    else if (b_tag_type == "B_Tag_LF_Up")
      weight_b_sf = weight_b_tag_lf_up;
    else if (b_tag_type == "B_Tag_LFStats1_Down")
      weight_b_sf = weight_b_tag_lfstats1_down;
    else if (b_tag_type == "B_Tag_LFStats1_Up")
      weight_b_sf = weight_b_tag_lfstats1_up;
    else if (b_tag_type == "B_Tag_LFStats2_Down")
      weight_b_sf = weight_b_tag_lfstats2_down;
    else if (b_tag_type == "B_Tag_LFStats2_Up")
      weight_b_sf = weight_b_tag_lfstats2_up;
    else if (b_tag_type == "B_Tag_CFErr1_Down")
      weight_b_sf = weight_b_tag_cferr1_down;
    else if (b_tag_type == "B_Tag_CFErr1_Up")
      weight_b_sf = weight_b_tag_cferr1_up;
    else if (b_tag_type == "B_Tag_CFErr2_Down")
      weight_b_sf = weight_b_tag_cferr2_down;
    else if (b_tag_type == "B_Tag_CFErr2_Up")
      weight_b_sf = weight_b_tag_cferr2_up;
    else if (b_tag_type == "B_Tag_HFStats1_Down")
      weight_b_sf = weight_b_tag_hfstats1_down;
    else if (b_tag_type == "B_Tag_HFStats1_Up")
      weight_b_sf = weight_b_tag_hfstats1_up;
    else if (b_tag_type == "B_Tag_HFStats2_Down")
      weight_b_sf = weight_b_tag_hfstats2_down;
    else if (b_tag_type == "B_Tag_HFStats2_Up")
      weight_b_sf = weight_b_tag_hfstats2_up;
    else if (b_tag_type.Contains("B_Tag_JES") && b_tag_type.Contains("Down"))
      weight_b_sf = weight_b_tag_jes_down;
    else if (b_tag_type.Contains("B_Tag_JES") && b_tag_type.Contains("Up"))
      weight_b_sf = weight_b_tag_jes_up;

    float weight_sf = weight_raw * weight_b_sf;

    float b_rf = Get_Tagging_RF_DL_B_Tag(histo_name_rf, b_tag_type, n_jets, ht);
    float weight_rf = weight_sf * b_rf;

    for (int j = 0; j < 3; j++)
    {
      float weight;
      if (j == 0)
        weight = weight_raw;
      else if (j == 1)
        weight = weight_sf;
      else if (j == 2)
        weight = weight_rf;

      histo_closure_n_jet[sample_index][i][index_leading_jet_flavor][j]->Fill(n_jets, weight);
      histo_closure_ht[sample_index][i][index_leading_jet_flavor][j]->Fill(ht, weight);
      histo_closure_n_pileup[sample_index][i][index_leading_jet_flavor][j]->Fill(n_pileup, weight);

      histo_closure_bvsc[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_bvsc, weight);
      histo_closure_cvsb[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_cvsb, weight);
      histo_closure_cvsl[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_cvsl, weight);

      histo_closure_eta[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_eta, weight);
      histo_closure_pt[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_pt, weight);
    }
  }

  return;
} // void Tagging_RF_DL::Fill_Histo_Validation_MC_B_Tagger(const TString &sample_name, const TString &tree_type)

//////////

void Tagging_RF_DL::Fill_Histo_Validation_MC_C_Tagger(const TString &sample_name, const TString &tree_type)
{
  int sample_index = Histo_Index(sample_name);
  TString histo_name_rf = Histo_Name_RF(sample_name);

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

    // JES breakdown
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

    float weight_raw = weight_hem_veto;
    weight_raw *= weight_lumi;
    weight_raw *= weight_mc;
    weight_raw *= weight_prefire;
    weight_raw *= weight_pujet_veto;
    weight_raw *= weight_top_pt;

    weight_raw *= weight_sl_trig;

    weight_raw *= weight_mu_id;
    weight_raw *= weight_mu_iso;

    weight_raw *= weight_el_id;
    weight_raw *= weight_el_reco;

    if (c_tag_type == "C_Tag_LHE_Scale_MuF_Down")
      weight_raw *= weight_scale_variation_2;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuF_Up")
      weight_raw *= weight_scale_variation_1;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuR_Down")
      weight_raw *= weight_scale_variation_6;
    else if (c_tag_type == "C_Tag_LHE_Scale_MuR_Up")
      weight_raw *= weight_scale_variation_3;

    if (c_tag_type == "C_Tag_PS_FSR_Fixed_Down")
      weight_raw *= weight_ps[0];
    else if (c_tag_type == "C_Tag_PS_FSR_Fixed_Up")
      weight_raw *= weight_ps[1];
    else if (c_tag_type == "C_Tag_PS_ISR_Fixed_Down")
      weight_raw *= weight_ps[2];
    else if (c_tag_type == "C_Tag_PS_ISR_Fixed_Up")
      weight_raw *= weight_ps[3];

    if (c_tag_type == "C_Tag_PU_Down")
      weight_raw *= weight_pileup_down;
    else if (c_tag_type == "C_Tag_PU_Up")
      weight_raw *= weight_pileup_up;
    else
      weight_raw *= weight_pileup;

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

    float c_rf = Get_Tagging_RF_DL_C_Tag(histo_name_rf, c_tag_type, n_pileup, ht);
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

      histo_closure_n_jet[sample_index][i][index_leading_jet_flavor][j]->Fill(n_jets, weight);
      histo_closure_ht[sample_index][i][index_leading_jet_flavor][j]->Fill(ht, weight);
      histo_closure_n_pileup[sample_index][i][index_leading_jet_flavor][j]->Fill(n_pileup, weight);

      histo_closure_bvsc[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_bvsc, weight);
      histo_closure_cvsb[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_cvsb, weight);
      histo_closure_cvsl[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_cvsl, weight);

      histo_closure_eta[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_eta, weight);
      histo_closure_pt[sample_index][i][index_leading_jet_flavor][j]->Fill(leading_jet_pt, weight);
    }
  }

  return;
} // void Tagging_RF_DL::Fill_Histo_Validation_MC_C_Tagger(const TString &sample_name, const TString &tree_type)

//////////

int Tagging_RF_DL::Flavor_Index(const int &flavor)
{
  if (TMath::Abs(flavor) == 5)
    return 2;
  else if (TMath::Abs(flavor) == 4)
    return 1;
  else
    return 0;
} // int Tagging_RF_DL::Flavor_Index()

//////////

int Tagging_RF_DL::Histo_Index(const TString &sample_name)
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
      else
        histo_name += "_JJ";
    } // if (chk_tthf_breakdown)

    if (decay_mode == 21 || decay_mode == 23)
      histo_name += "_2";
    else if (decay_mode == 41 || decay_mode == 43)
      histo_name += "_4";
    else if (decay_mode == 45)
      histo_name += "_45";

    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();

    // cout << "test Histo_Index: " << sample_name << " " << histo_name << " " << index << " " << vec_short_name_mc.size() << endl;
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name]) - vec_short_name_mc.begin();

  return index;
} // int Tagging_RF_DL::Histo_Index(const TString &sample_name)

//////////

TString Tagging_RF_DL::Histo_Name_RF(const TString &sample_name)
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
      else
        histo_name_rf += "_JJ";
    } // if (chk_tthf_breakdown)

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
} // int Tagging_RF_DL::Histo_Name_RF(const TString &sample_name)

//////////

void Tagging_RF_DL::Ratio()
{
  cout << "[Tagging_RF_DL::Ratio] Init" << endl;

  /*
    ratio_b = new TH2D *[n_syst_b];
    for (int i = 0; i < n_syst_b; i++)
    {
      TString ratio_name = "Ratio_" + syst_name_b[i];
      ratio_b[i] = new TH2D(ratio_name, ratio_name, n_bin, 0, n_bin);

      ratio_b[i]->Divide((TH2D *)(stack_mc_before->GetStack()->Last()), (TH1D *)(stack_mc_after_b[i]->GetStack()->Last()));
    }

    ratio_c = new TH1D *[n_syst_c];
    for (int i = 0; i < n_syst_c; i++)
    {
      TString ratio_name = "Ratio_" + syst_name_c[i];
      ratio_c[i] = new TH1D(ratio_name, ratio_name, n_bin, 0, n_bin);

      ratio_c[i]->Divide((TH1D *)(stack_mc_before->GetStack()->Last()), (TH1D *)(stack_mc_after_c[i]->GetStack()->Last()));
    }
  */

  ratio_b = new TH2D **[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    ratio_b[i] = new TH2D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      TString ratio_name = "Ratio_" + vec_short_name_mc[i] + "_" + syst_name_b[j];
      ratio_b[i][j] = new TH2D(ratio_name, ratio_name, bin_njet.size() - 1, bin_njet.data(), bin_ht.size() - 1, bin_ht.data());

      ratio_b[i][j]->Divide(histo_mc_before_b[i][j], histo_mc_after_b[i][j]);
    }
  }

  /*
    ratio_c = new TH1D **[n_sample_merge_mc];
    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      ratio_c[i] = new TH1D *[n_syst_c];
      for (int j = 0; j < n_syst_c; j++)
      {
        TString ratio_name = "Ratio_" + vec_short_name_mc[i] + "_" + syst_name_c[j];
        ratio_c[i][j] = new TH1D(ratio_name, ratio_name, bin_n_pileup.size() - 1, bin_n_pileup.data());

        ratio_c[i][j]->Divide(histo_mc_before_c[i], histo_mc_after_c[i][j]);
      }
    }
    */

  ratio_c = new TH2D **[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    ratio_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      TString ratio_name = "Ratio_" + vec_short_name_mc[i] + "_" + syst_name_c[j];
      ratio_c[i][j] = new TH2D(ratio_name, ratio_name, bin_n_pileup.size() - 1, bin_n_pileup.data(), bin_ht.size() - 1, bin_ht.data());

      ratio_c[i][j]->Divide(histo_mc_before_c[i][j], histo_mc_after_c[i][j]);
    }
  }

  return;
} // void Tagging_RF_DL::Ratio()

//////////

void Tagging_RF_DL::Read_Tree()
{
  cout << "[Tagging_RF_DL::Read_Tree]: Starts to read trees" << endl;

  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString tree_type = it_map_map->first;

    cout << endl;
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

      for (Long64_t i = 0; i < n_entries; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        // if ((channel == "MM" || channel == "EE") && abs(event.dilepton_mass - Z_MASS) < 15)
        //   continue;

        if (mode == "Analysis")
          Fill_Histo_MC(sample_name, tree_type);
        else if (mode == "Validation")
        {
          // if (n_bjets < 2)
          //   continue;

          if (tagger == "B_Tagger")
            Fill_Histo_Validation_MC_B_Tagger(sample_name, tree_type);
          else if (tagger == "C_Tagger")
            Fill_Histo_Validation_MC_C_Tagger(sample_name, tree_type);
        }
      } // loop over entries
    } // loop over map_mc
  } // loop over map_map_mc

  return;
} // void Tagging_RF_DL::Read_Tree()

//////////

void Tagging_RF_DL::Run_Analysis()
{
  cout << "[Tagging_RF_DL::Run_Analysis]: Init" << endl;

  ROOT::EnableImplicitMT(4);

  Setup_Analysis();
  Setup_Binning();
  Setup_Histo();
  Read_Tree();
  //  Stack_MC();
  Ratio();
  //  Draw();

  return;
} // void Tagging_RF_DL::Run_Analysis()

//////////

void Tagging_RF_DL::Run_Combine()
{
  Combine_Lepton_Channel();
  // Combine_Decay_Mode();
  Draw_Result();

  return;
} // void Tagging_RF_DL::Run_Combine()

//////////

void Tagging_RF_DL::Run_Draw_Validation()
{
  gROOT->SetBatch(kTRUE);

  Draw_Validation();

  return;
} // void Tagging_RF_DL::Run_Draw_Validation()

//////////

void Tagging_RF_DL::Run_Validation()
{
  cout << "[Tagging_RF_DL::Run_Validation]: Init" << endl;

  ROOT::EnableImplicitMT(4);

  Setup_Application();
  Setup_Analysis();
  Setup_Histo_Validation();
  Read_Tree();

  return;
} // void Tagging_RF_DL::Run_Validation()

//////////

void Tagging_RF_DL::Setup_Analysis()
{
  cout << "[Tagging_RF_DL::Setup_Analysis]: Init analysis" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  path_base += era + "/" + "/Tagging_RF_DL/";

  n_sample = samples.map_mc.size();

  // fix tree
  if (index_tree_type != -1)
  {
    TString tree_type_fixed;

    // To split jobs for Central tree
    if (n_tree_type <= index_tree_type)
    {
      cerr << "Out of Tree_Type" << endl;
      exit(1);
    }
    else
      tree_type_fixed = vec_tree_type[index_tree_type];

    cout << "Fixing tree_type. Index = " << index_tree_type << " Tree_Type " << tree_type_fixed << " " << endl;

    vec_tree_type.clear();
    vec_tree_type.shrink_to_fit();

    vec_tree_type.push_back(tree_type_fixed);
  }
  else
    cout << "Iterate all tree_type" << endl;

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

      // cout << tree_type << endl;

      vec_map_tree_mc[i][it->first] = (TTree *)map_fin_mc[it->first]->Get(channel + "/" + tree_type + "/Result_Tree");
      Setup_Tree(vec_map_tree_mc[i][it->first], tree_type);
    } // for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  }

  // TTree handlers
  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    TString tree_type = vec_tree_type[i];
    map_map_tree_mc[tree_type] = &vec_map_tree_mc[i];
  }

  return;
} // void Tagging_RF_DL::Setup_Analysis()

//////////

void Tagging_RF_DL::Setup_Application()
{
  cout << "[Tagging_RF_DL::Setup_Application]: Init" << endl;

  Setup_Binning();

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  fin = new TFile(path_base + "/Corrections/Vcb_Tagging_RF_DL_" + era + ".root");

  /*TList *list_dir = fin->GetListOfKeys();

  for (int i = 0; i < list_dir->GetEntries(); i++)
  {
    TObject *obj = list_dir->At(i);
    TString dir_name = obj->GetName();

    vec_sample_tagging_rf.push_back(dir_name);
  }
  n_sample_tagging_rf = vec_sample_tagging_rf.size();
  */

  // set this manually
  if (chk_tthf_breakdown)
  {
    vec_sample_tagging_rf = {//"ttV", "VV", "VJets", "ST", "QCD_bEn",
                             "ST_sch", "ST_tch", "ST_tw",
                             "WJets", "DYJets",
                             "QCD_bEn",
                             "ttHTobb", "ttHToNonbb", "ttWToLNu", "ttWToQQ", "ttZToLLNuNu", "ttZToQQ",
                             "WW", "WZ", "ZZ",
                             "TTLJ_JJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
                             "TTLJ_JJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
                             "TTLJ_JJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
                             /* CP5 */
                             "TTLJ_CP5Down_JJ_2", "TTLJ_CP5Down_CC_2", "TTLJ_CP5Down_BB_2",
                             "TTLJ_CP5Down_JJ_4", "TTLJ_CP5Down_CC_4", "TTLJ_CP5Down_BB_4",
                             "TTLJ_CP5Down_JJ_45", "TTLJ_CP5Down_CC_45", "TTLJ_CP5Down_BB_45",
                             "TTLJ_CP5Up_JJ_2", "TTLJ_CP5Up_CC_2", "TTLJ_CP5Up_BB_2",
                             "TTLJ_CP5Up_JJ_4", "TTLJ_CP5Up_CC_4", "TTLJ_CP5Up_BB_4",
                             "TTLJ_CP5Up_JJ_45", "TTLJ_CP5Up_CC_45", "TTLJ_CP5Up_BB_45",
                             /* mtop */
                             "TTLJ_mtop171p5_JJ_2", "TTLJ_mtop171p5_CC_2", "TTLJ_mtop171p5_BB_2",
                             "TTLJ_mtop171p5_JJ_4", "TTLJ_mtop171p5_CC_4", "TTLJ_mtop171p5_BB_4",
                             "TTLJ_mtop171p5_JJ_45", "TTLJ_mtop171p5_CC_45", "TTLJ_mtop171p5_BB_45",
                             "TTLJ_mtop173p5_JJ_2", "TTLJ_mtop173p5_CC_2", "TTLJ_mtop173p5_BB_2",
                             "TTLJ_mtop173p5_JJ_4", "TTLJ_mtop173p5_CC_4", "TTLJ_mtop173p5_BB_4",
                             "TTLJ_mtop173p5_JJ_45", "TTLJ_mtop173p5_CC_45", "TTLJ_mtop173p5_BB_45",
                             /* hdamp */
                             "TTLJ_hdampDown_JJ_2", "TTLJ_hdampDown_CC_2", "TTLJ_hdampDown_BB_2",
                             "TTLJ_hdampDown_JJ_4", "TTLJ_hdampDown_CC_4", "TTLJ_hdampDown_BB_4",
                             "TTLJ_hdampDown_JJ_45", "TTLJ_hdampDown_CC_45", "TTLJ_hdampDown_BB_45",
                             "TTLJ_hdampUp_JJ_2", "TTLJ_hdampUp_CC_2", "TTLJ_hdampUp_BB_2",
                             "TTLJ_hdampUp_JJ_4", "TTLJ_hdampUp_CC_4", "TTLJ_hdampUp_BB_4",
                             "TTLJ_hdampUp_JJ_45", "TTLJ_hdampUp_CC_45", "TTLJ_hdampUp_BB_45",
                             "TTLL_JJ", "TTLL_CC", "TTLL_BB",
                             /* CP5 */
                             "TTLL_CP5Down_JJ", "TTLL_CP5Down_CC", "TTLL_CP5Down_BB",
                             "TTLL_CP5Up_JJ", "TTLL_CP5Up_CC", "TTLL_CP5Up_BB",
                             /* mtop */
                             "TTLL_mtop171p5_JJ", "TTLL_mtop171p5_CC", "TTLL_mtop171p5_BB",
                             "TTLL_mtop173p5_JJ", "TTLL_mtop173p5_CC", "TTLL_mtop173p5_BB",
                             /* hdamp */
                             "TTLL_hdampDown_JJ", "TTLL_hdampDown_CC", "TTLL_hdampDown_BB",
                             "TTLL_hdampUp_JJ", "TTLL_hdampUp_CC", "TTLL_hdampUp_BB"};
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

  ratio_b = new TH2D **[n_sample_tagging_rf];
  ratio_c = new TH2D **[n_sample_tagging_rf];
  for (int i = 0; i < n_sample_tagging_rf; i++)
  {
    ratio_b[i] = new TH2D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      TString histo_name = vec_sample_tagging_rf[i] + "/Ratio_" + vec_sample_tagging_rf[i] + "_" + syst_name_b[j];
      ratio_b[i][j] = (TH2D *)fin->Get(histo_name);
    }

    ratio_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      TString histo_name = vec_sample_tagging_rf[i] + "/Ratio_" + vec_sample_tagging_rf[i] + "_" + syst_name_c[j];
      ratio_c[i][j] = (TH2D *)fin->Get(histo_name);
    }
  }

  cout << "[Tagging_RF_DL::Setup_Application]: Done" << endl;

  return;
} // void Tagging_RF_DL::Setup_Application()

//////////

void Tagging_RF_DL::Setup_Binning()
{
  if (era == "2017" || era == "2018")
  {
    bin_njet = {4, 5, 6, 7, 8, 10, 12, 14, 20};
    bin_ht = {80, 180, 230, 280, 330, 400, 500, 1000};
    bin_n_pileup = {0, 20, 25, 30, 35, 40, 50, 70};
  }
  else if (era == "2016preVFP" || era == "2016postVFP")
  {
    bin_njet = {4, 5, 6, 7, 8, 10, 12, 14, 20};
    bin_ht = {80, 230, 330, 500, 1000};
    bin_n_pileup = {0, 25, 35, 70};
  }

  return;
} // void Tagging_RF_DL::Setup_Binning()

//////////

void Tagging_RF_DL::Setup_Histo()
{
  cout << "[Tagging_RF_DL::Setup_Histo]: Init" << endl;

  histo_mc_before_b = new TH2D **[n_sample_merge_mc];
  histo_mc_after_b = new TH2D **[n_sample_merge_mc];

  // histo_mc_before_c = new TH1D *[n_sample_merge_mc];
  // histo_mc_after_c = new TH1D **[n_sample_merge_mc];
  histo_mc_before_c = new TH2D **[n_sample_merge_mc];
  histo_mc_after_c = new TH2D **[n_sample_merge_mc];

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    histo_mc_before_b[i] = new TH2D *[n_syst_b];
    histo_mc_after_b[i] = new TH2D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      TString histo_name = vec_short_name_mc[i] + "_" + syst_name_b[j] + "_Before";
      histo_mc_before_b[i][j] = new TH2D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data(), bin_ht.size() - 1, bin_ht.data());

      histo_name = vec_short_name_mc[i] + "_" + syst_name_b[j] + "_After";
      histo_mc_after_b[i][j] = new TH2D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data(), bin_ht.size() - 1, bin_ht.data());
    }

    histo_mc_before_c[i] = new TH2D *[n_syst_c];
    histo_mc_after_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      TString histo_name = vec_short_name_mc[i] + "_" + syst_name_c[j] + "_Before";
      histo_mc_before_c[i][j] = new TH2D(histo_name, histo_name, bin_n_pileup.size() - 1, bin_n_pileup.data(), bin_ht.size() - 1, bin_ht.data());

      histo_name = vec_short_name_mc[i] + "_" + syst_name_c[j] + "_After";
      histo_mc_after_c[i][j] = new TH2D(histo_name, histo_name, bin_n_pileup.size() - 1, bin_n_pileup.data(), bin_ht.size() - 1, bin_ht.data());
    }
  }

  return;
} // void Tagging_RF_DL::Setup_Histo()

//////////

void Tagging_RF_DL::Setup_Histo_Validation()
{
  histo_closure_n_jet = new TH1D ****[n_sample_merge_mc];
  histo_closure_ht = new TH1D ****[n_sample_merge_mc];
  histo_closure_n_pileup = new TH1D ****[n_sample_merge_mc];
  histo_closure_bvsc = new TH1D ****[n_sample_merge_mc];
  histo_closure_cvsb = new TH1D ****[n_sample_merge_mc];
  histo_closure_cvsl = new TH1D ****[n_sample_merge_mc];
  histo_closure_eta = new TH1D ****[n_sample_merge_mc];
  histo_closure_pt = new TH1D ****[n_sample_merge_mc];

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString histo_name_base = "Closure_" + vec_short_name_mc[i];

    histo_closure_n_jet[i] = new TH1D ***[n_syst_c];
    histo_closure_ht[i] = new TH1D ***[n_syst_c];
    histo_closure_n_pileup[i] = new TH1D ***[n_syst_c];
    histo_closure_bvsc[i] = new TH1D ***[n_syst_c];
    histo_closure_cvsb[i] = new TH1D ***[n_syst_c];
    histo_closure_cvsl[i] = new TH1D ***[n_syst_c];
    histo_closure_eta[i] = new TH1D ***[n_syst_c];
    histo_closure_pt[i] = new TH1D ***[n_syst_c];

    int n_syst;
    if (tagger == "B_Tagger")
      n_syst = n_syst_b;
    else if (tagger == "C_Tagger")
      n_syst = n_syst_c;

    for (int j = 0; j < n_syst; j++)
    {
      histo_closure_n_jet[i][j] = new TH1D **[n_flavor];
      histo_closure_ht[i][j] = new TH1D **[n_flavor];
      histo_closure_n_pileup[i][j] = new TH1D **[n_flavor];
      histo_closure_bvsc[i][j] = new TH1D **[n_flavor];
      histo_closure_cvsb[i][j] = new TH1D **[n_flavor];
      histo_closure_cvsl[i][j] = new TH1D **[n_flavor];
      histo_closure_eta[i][j] = new TH1D **[n_flavor];
      histo_closure_pt[i][j] = new TH1D **[n_flavor];

      for (int k = 0; k < n_flavor; k++)
      {
        TString syst_name;
        if (tagger == "B_Tagger")
          syst_name = syst_name_b[j];
        else if (tagger == "C_Tagger")
          syst_name = syst_name_c[j];

        histo_closure_n_jet[i][j][k] = new TH1D *[3];
        histo_closure_ht[i][j][k] = new TH1D *[3];
        histo_closure_n_pileup[i][j][k] = new TH1D *[3];
        histo_closure_bvsc[i][j][k] = new TH1D *[3];
        histo_closure_cvsb[i][j][k] = new TH1D *[3];
        histo_closure_cvsl[i][j][k] = new TH1D *[3];
        histo_closure_eta[i][j][k] = new TH1D *[3];
        histo_closure_pt[i][j][k] = new TH1D *[3];

        for (int l = 0; l < 3; l++)
        {
          TString histo_name_base = "Closure_" + vec_short_name_mc[i] + "_" + syst_name + "_" + flavor_name[k];
          ;

          TString histo_name = histo_name_base + "_N_Jet_" + to_string(l);
          histo_closure_n_jet[i][j][k][l] = new TH1D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data());

          histo_name = histo_name_base + "_HT_" + to_string(l);
          histo_closure_ht[i][j][k][l] = new TH1D(histo_name, histo_name, bin_ht.size() - 1, bin_ht.data());

          histo_name = histo_name_base + "_N_PV_" + to_string(l);
          histo_closure_n_pileup[i][j][k][l] = new TH1D(histo_name, histo_name, bin_n_pileup.size() - 1, bin_n_pileup.data());

          histo_name = histo_name_base + "_Leading_Jet_BvsC_" + to_string(l);
          histo_closure_bvsc[i][j][k][l] = new TH1D(histo_name, histo_name, 25, 0, 1);

          histo_name = histo_name_base + "_Leading_Jet_CvsB_" + to_string(l);
          histo_closure_cvsb[i][j][k][l] = new TH1D(histo_name, histo_name, 25, 0, 1);

          histo_name = histo_name_base + "_Leading_Jet_CvsL_" + to_string(l);
          histo_closure_cvsl[i][j][k][l] = new TH1D(histo_name, histo_name, 25, 0, 1);

          histo_name = histo_name_base + "_Leading_Jet_Eta_" + to_string(l);
          histo_closure_eta[i][j][k][l] = new TH1D(histo_name, histo_name, 12, -3, 3);

          histo_name = histo_name_base + "_Leading_Jet_Pt_" + to_string(l);
          histo_closure_pt[i][j][k][l] = new TH1D(histo_name, histo_name, 25, 0, 500);
        }
      }
    }
  }

  return;
} // void Tagging_RF_DL::Setup_Histo_Validation()

//////////

void Tagging_RF_DL::Setup_Tree(TTree *tree, const TString &syst)
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

  tree->SetBranchAddress("weight_mu_id", &weight_mu_id);
  tree->SetBranchAddress("weight_mu_iso", &weight_mu_iso);
  tree->SetBranchAddress("weight_el_id", &weight_el_id);
  tree->SetBranchAddress("weight_el_reco", &weight_el_reco);

  tree->SetBranchAddress("weight_sl_trig", &weight_sl_trig);

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
  else
    tree->SetBranchAddress("weight_c_tag", &weight_c_tag);

  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("n_bjets", &n_bjets);
  tree->SetBranchAddress("ht", &ht);

  tree->SetBranchAddress("n_vertex", &n_pileup);
  tree->SetBranchAddress("n_pileup", &n_pileup);

  tree->SetBranchAddress("decay_mode", &decay_mode);

  tree->SetBranchAddress("genTtbarId", &gen_ttbar_id);

  // tree->SetBranchAddress("Gen_HF_Flavour", &vec_gen_hf_flavour);
  // tree->SetBranchAddress("Gen_HF_Origin", &vec_gen_hf_origin);
  // tree->SetBranchAddress("Sel_Gen_HF_Flavour", &vec_sel_gen_hf_flavour);
  // tree->SetBranchAddress("Sel_Gen_HF_Origin", &vec_sel_gen_hf_origin);

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

  tree->SetBranchAddress("Jet_Flavor", &vec_jet_flavor);

  return;
} // void Tagging_RF_DL::Setup_Tree()

//////////
/*
void Tagging_RF_DL::Stack_MC()
{
  cout << "[Vcb_Tagging_RF_DL::Stack_MC] Init" << endl;

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

  cout << "[Vcb_Tagging_RF_DL::Stack_MC]: Done" << endl;

  return;
} // void Vcb_Tagging_RF_DL::Stack_MC()
*/
//////////
