#include "Histo_Syst.h"

ClassImp(Histo_Syst);

//////////

Histo_Syst::Histo_Syst(const TString &a_era, const TString &a_channel, const TString &a_mode, const TString &a_run_flag, const TString &a_tagger, const TString &a_swap_mode)
    : samples(a_era, a_channel), event(a_era, a_channel, a_swap_mode), tagging_rf(a_era)
{
  ROOT::EnableImplicitMT(5);
  TH1::AddDirectory(kFALSE);

  cout << "[Histo_Syst::Histo_Syst]: Init analysis" << endl;

  debug = false;
  reduction = 1;

  era = a_era;
  channel = a_channel;
  mode = a_mode;
  run_flag = a_run_flag;
  tagger = a_tagger;

  cout << "[Histo_Syst::Histo_Syst]: mode = " << mode << endl;

  chk_cut_best_mva_score_pre = false;
  if (chk_cut_best_mva_score_pre)
    cut_best_mva_score_pre = 0.0;
  else
    cut_best_mva_score_pre = -1;

  chk_cut_template_score = false;
  if (chk_cut_template_score)
    cut_template_score = 0.2;

  if (era == "2016preVFP")
  {
    bvsc_wp_m = bvsc_wp_m_2016preVFP;
    cvsb_wp_m = cvsb_wp_m_2016preVFP;
    cvsl_wp_m = cvsl_wp_m_2016preVFP;
  }
  else if (era == "2016postVFP")
  {
    bvsc_wp_m = bvsc_wp_m_2016postVFP;
    cvsb_wp_m = cvsb_wp_m_2016postVFP;
    cvsl_wp_m = cvsl_wp_m_2016postVFP;
  }
  else if (era == "2017")
  {
    bvsc_wp_m = bvsc_wp_m_2017;
    cvsb_wp_m = cvsb_wp_m_2017;
    cvsl_wp_m = cvsl_wp_m_2017;
  }
  else if (era == "2018")
  {
    bvsc_wp_m = bvsc_wp_m_2018;
    cvsb_wp_m = cvsb_wp_m_2018;
    cvsl_wp_m = cvsl_wp_m_2018;
  }

  path_base = getenv("Vcb_Post_Analysis_WD");

  // Region
  // Region for ABCD method for QCD data driven
  abcd_region_name = {"A", "B", "C", "D"};

  // Signal should be last
  region_name = {"Control0", "Signal"};
  n_region = region_name.size();

  if (mode == "Cal_TF")
  {
    Config_Sample();
    Config_Syst();
    Config_Variable();
    Register_Sample_TF();
    Init_Histo_TF();
    Read_Tree();
    Calculate_TF();
  }
  else if (mode == "2D")
  {
    Config_Sample();
    Config_Syst();
    Config_Variable();
    Register_Sample();
    Init_Histo();
    Read_Tree();
  }
  else if (mode == "Data_Driven")
  {
    Config_Sample();
    Config_Syst();
    Config_Variable();
    Init_Histo_Data_Driven();
    Data_Driven();
  }
  else if (mode == "Merge")
  {
    chk_qcd_data_driven = true;
    cout << "[Histo_Syst::Hist_Syst]: Mode of QCD_Data_Driven = " << chk_qcd_data_driven << endl;

    Config_Sample();
    Config_Syst();
    Config_Variable();
    Init_Merge_PDF_Error_Set();
    Merge_PDF_Error_Set();
  }
} // Histo_Syst::Histo_Syst(const TString& a_era, const TString& a_channel, const TString& a_swap_mode)

//////////

Histo_Syst::~Histo_Syst()
{
  cout << "[Histo_Syst::~Histo_Syst]: Init" << endl;

  if (mode == "Cal_TF")
  {
    // save raw histo
    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = fout->mkdir(region_name[i]);

      for (int j = 0; j < n_abcd_region - 2; j++)
      {
        TDirectory *dir_abcd_region = dir_region->mkdir(abcd_region_name[j]);

        // data
        dir_abcd_region->cd();
        histo_data[j][i][0]->SetTitle("Data");
        histo_data[j][i][0]->SetName("Data");
        histo_data[j][i][0]->Write();

        // mc
        for (int k = 0; k < n_sample_merge_mc; k++)
        {
          dir_abcd_region->cd();
          histo_mc[j][i][0][k][0]->SetTitle(vec_short_name_mc[k]);
          histo_mc[j][i][0][k][0]->SetName(vec_short_name_mc[k]);
          histo_mc[j][i][0][k][0]->Write();
        } // loop over n_sample_merge_mc

        // subtracted
        dir_abcd_region->cd();
        histo_subtracted_tf[j][i]->SetTitle("Subtracted");
        histo_subtracted_tf[j][i]->SetName("Subtracted");
        histo_subtracted_tf[j][i]->Write();
      } // loop over n_abcd_region -2

      dir_region->cd();
      histo_tf[i]->SetTitle("TF");
      histo_tf[i]->SetName("TF");
      histo_tf[i]->Write();
    } // loop over n_region

    cout << "[Histo_Syst::~Histo_Syst]: Closing root file" << endl;
    fout->Close();
  } //  if (mode == "Cal_TF")
  else if (mode == "2D")
  {
    // Save histos
    for (int i = 0; i < n_abcd_region; i++)
    {
      // to save time
      if (abcd_region_name[i] == "A" || abcd_region_name[i] == "B")
        continue;

      cout << "ABCD Region " << abcd_region_name[i] << endl;
      TDirectory *dir_abcd_region = fout->mkdir(abcd_region_name[i]);

      for (int j = 0; j < n_region; j++)
      {
        cout << "\tRegion " << region_name[j] << endl;

        TDirectory *dir_region = dir_abcd_region->mkdir(region_name[j]);

        if ((run_flag == "Data" || run_flag == "All") && (abcd_region_name[i] != "D" || region_name[j] != "Signal"))
        {
          cout << "Data" << endl;

          // for Data
          TDirectory *dir_data = dir_region->mkdir("Data");
          for (int k = 0; k < n_variable; k++)
          {
            dir_data->cd();
            histo_data[i - 2][j][k]->SetTitle(variable_conf[k].variable_title);
            histo_data[i - 2][j][k]->SetName(variable_conf[k].variable_title);
            histo_data[i - 2][j][k]->Write();
          } // loop over variable
        }

        // for MC
        for (int k = 0; k < n_syst; k++)
        {
          cout << "syst_name = " << syst_name[k] << endl;

          if (run_flag != "All" && run_flag != "Central" && run_flag != syst_name[k])
            continue;

          TDirectory *dir_syst = dir_region->mkdir(syst_name[k]);

          for (int l = 0; l < n_sample_merge_mc; l++)
          {
            // cout << vec_short_name_mc[k] << endl;

            TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[l]);

            for (int m = 0; m < n_variable; m++)
            {
              dir_sample->cd();

              histo_mc[i - 2][j][k][l][m]->SetTitle(variable_conf[m].variable_title);
              histo_mc[i - 2][j][k][l][m]->SetName(variable_conf[m].variable_title);
              histo_mc[i - 2][j][k][l][m]->Write();
            } // loop over variable
          } // loop over sample
        } // loop over syst
        cout << "MC done" << endl;
      } // loop over n_region
      cout << "Region done" << endl;
    } // loop over n_abcd_region

    cout << "[Histo_Syst::~Histo_Syst]: Closing root file" << endl;
    fout->Close();
  } //  if (mode=="Fill")

  else if (mode == "Data_Driven")
  {
    // cout << "[Histo_Syst::~Histo_Syst]: Updating subtracted histograms" << endl;
    // // Update subtracted histo
    // for (int i = 0; i < n_region; i++)
    // {
    //   for (int j = 0; j < n_syst; j++)
    //   {
    //     TDirectory *dir = (TDirectory *)fin_2d->Get("C/" + region_name[i] + "/" + syst_name[j]);
    //     TDirectory *dir_sub = dir->mkdir("Subtracted", "Subtracted", true);

    //     dir_sub->cd();
    //     for (int k = 0; k < n_variable; k++)
    //     {
    //       histo_subtracted_data_driven[i][j][k]->SetTitle(variable_conf[k].variable_title);
    //       histo_subtracted_data_driven[i][j][k]->SetName(variable_conf[k].variable_title);
    //       histo_subtracted_data_driven[i][j][k]->Write();
    //     } // loop over n_variable
    //   }   // loop over n_syst
    // }     // loop over n_region
    // cout << "[Histo_Syst::~Histo_Syst]: Updating subtracted histograms. Done." << endl;

    cout << "[Histo_Syst::~Histo_Syst]: Writing fout." << endl;
    for (int i = 0; i < n_region; i++)
    {
      cout << region_name[i] << endl;

      TDirectory *dir_region = fout->mkdir(region_name[i]);

      // MC
      for (int j = 0; j < n_syst; j++)
      {
        cout << "\t" << syst_name[j] << endl;

        TDirectory *dir_syst = dir_region->mkdir(syst_name[j]);
        for (int k = 0; k < n_sample_merge_mc; k++)
        {
          TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[k]);
          for (int l = 0; l < n_variable; l++)
          {
            TH1D *histo_proj = (TH1D *)(dynamic_cast<TH2D *>(histo_mc_dd[1][i][j][k][l])->ProjectionX()->Clone());

            dir_sample->cd();
            histo_proj->SetTitle(variable_conf[l].variable_title);
            histo_proj->SetName(variable_conf[l].variable_title);
            histo_proj->Write();
          } // loop over n_variable
        } // n_sample

        TDirectory *dir_qcd_data_driven = dir_syst->mkdir("QCD_Data_Driven");
        dir_qcd_data_driven->cd();
        for (int k = 0; k < n_variable; k++)
        {
          TH1D *histo_proj = (TH1D *)histo_tf_corrected[i][j][k]->ProjectionX()->Clone();
          histo_proj->SetTitle(variable_conf[k].variable_title);
          histo_proj->SetName(variable_conf[k].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // loop over n_syst

      // QCD TF Up
      TDirectory *dir_qcd_tf_up = dir_region->mkdir("QCD_TF_Up");
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_qcd_tf_up->mkdir(vec_short_name_mc[j]);
        for (int k = 0; k < n_variable; k++)
        {
          TH1D *histo_proj = (TH1D *)(dynamic_cast<TH2D *>(histo_mc_dd[1][i][0][j][k])->ProjectionX()->Clone());

          dir_sample->cd();
          histo_proj->SetTitle(variable_conf[k].variable_title);
          histo_proj->SetName(variable_conf[k].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // loop over n_sample

      TDirectory *dir_qcd_data_driven_tf_up = dir_qcd_tf_up->mkdir("QCD_Data_Driven");
      dir_qcd_data_driven_tf_up->cd();
      for (int j = 0; j < n_variable; j++)
      {
        TH1D *histo_proj = (TH1D *)histo_tf_corrected_up[i][j]->ProjectionX()->Clone();
        histo_proj->SetTitle(variable_conf[j].variable_title);
        histo_proj->SetName(variable_conf[j].variable_title);
        histo_proj->Write();
      } // loop over n_variable

      // QCD TF Down
      TDirectory *dir_qcd_tf_down = dir_region->mkdir("QCD_TF_Down");
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_qcd_tf_down->mkdir(vec_short_name_mc[j]);
        for (int k = 0; k < n_variable; k++)
        {
          TH1D *histo_proj = (TH1D *)(dynamic_cast<TH2D *>(histo_mc_dd[1][i][0][j][k])->ProjectionX()->Clone());

          dir_sample->cd();
          histo_proj->SetTitle(variable_conf[k].variable_title);
          histo_proj->SetName(variable_conf[k].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // loop over n_sample

      TDirectory *dir_qcd_data_driven_tf_down = dir_qcd_tf_down->mkdir("QCD_Data_Driven");
      dir_qcd_data_driven_tf_down->cd();
      for (int j = 0; j < n_variable; j++)
      {
        TH1D *histo_proj = (TH1D *)histo_tf_corrected_down[i][j]->ProjectionX()->Clone();
        histo_proj->SetTitle(variable_conf[j].variable_title);
        histo_proj->SetName(variable_conf[j].variable_title);
        histo_proj->Write();
      } // loop over n_variable

      // Data
      if (region_name[i] != "Signal")
      {
        cout << "\tData" << endl;

        TDirectory *dir_data = dir_region->mkdir("Data");
        dir_data->cd();
        for (int j = 0; j < n_variable; j++)
        {
          TH1D *histo_proj = (TH1D *)(dynamic_cast<TH2D *>(histo_data_dd[1][i][j])->ProjectionX()->Clone());
          histo_proj->SetTitle(variable_conf[j].variable_title);
          histo_proj->SetName(variable_conf[j].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // if(region_name[i]!="Signal")
    } // loop over n_reigon
    cout << "[Histo_Syst::~Histo_Syst]: Writing fout. Done." << endl;

    cout << "[Histo_Syst::~Histo_Syst]: Closing root file" << endl;

    fin_2d->Close();
    fin_tf->Close();
    fout->Close();

    cout << "[Histo_Syst::~Histo_Syst]: Closing root file. Done." << endl;
  }
  // PDF error set
  else
  {
    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = (TDirectory *)fout->Get(region_name[i]);

      TDirectory *dir_syst = dir_region->mkdir("PDF_Error_Set_Down", "PDF_Error_Set_Down", true);
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[j], vec_short_name_mc[j], true);
        for (int k = 0; k < n_variable; k++)
        {
          dir_sample->cd();
          histo_mc_pdf_error_set_down[i][j][k]->SetTitle(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_down[i][j][k]->SetName(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_down[i][j][k]->Write();
        } // loop over n_variable
      } // loop over n_sample_merge_mc;

      dir_syst = dir_region->mkdir("PDF_Error_Set_Up", "PDF_Error_Set_Up", true);
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[j], vec_short_name_mc[j], true);
        for (int k = 0; k < n_variable; k++)
        {
          dir_sample->cd();
          histo_mc_pdf_error_set_up[i][j][k]->SetTitle(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_up[i][j][k]->SetName(variable_conf[k].variable_title);
          histo_mc_pdf_error_set_up[i][j][k]->Write();
        } // loop over n_variable
      } // loop over n_sample_merge_mc;
    } // loop over n_region

    fout->Close();
  } // else

  cout << "[Histo_Syst::~Histo_Syst]: Done" << endl;
} // Histo_Syst::~Histo_Syst()

//////////

void Histo_Syst::Calculate_TF()
{
  cout << "[Histo_Syst::Calculate_TF]: Init" << endl;

  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    for (int j = 0; j < n_region; j++)
    {
      // data
      histo_subtracted_tf[i][j]->Add(histo_data[i][j][0]);

      // subtract mc
      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        if (vec_short_name_mc[k] == "QCD_bEn")
          continue;

        histo_subtracted_tf[i][j]->Add(histo_mc[i][j][0][k][0], -1);
      }
    } // loop over n_region
  } // loop over n_abcd_region-2

  for (int i = 0; i < 2; i++)
    histo_tf[i]->Divide(histo_subtracted_tf[1][i], histo_subtracted_tf[0][i]);

  cout << "[Histo_Syst::Calculate_TF]: Done" << endl;

  return;
} // void Histo_Syst::Calculate_TF(

//////////

void Histo_Syst::Config_Sample()
{
  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
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

  if (mode == "Merge" && chk_qcd_data_driven == true)
  {
    // vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "QCD_bEn"));
    vec_short_name_mc.push_back("QCD_Data_Driven");
  }

  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  return;
} // void Histo_Syst::Config_Sample()

//////////

void Histo_Syst::Config_Syst()
{
  if (mode == "Cal_TF")
    syst_name = {"Nominal"};
  else
  {
    if (debug)
      syst_name = {"Nominal"};
    else
    {
      syst_name = {"Nominal",
                   "Jet_En_Down", "Jet_En_Up",
                   "Jet_Res_Down", "Jet_Res_Up",
                   "UE_Down", "UE_Up",
                   "PDF_Alternative", "PDF_As_Down", "PDF_As_Up",
                   "Pileup_Down", "Pileup_Up",
                   "PU_Jet_Veto_Down", "PU_Jet_Veto_Up",
                   "Prefire_Down", "Prefire_Up",
                   "MuF_TT_Down", "MuF_TT_Up", "MuF_ST_Down", "MuF_ST_Up",
                   "MuR_TT_Down", "MuR_TT_Up", "MuR_ST_Down", "MuR_ST_Up",
                   //"MuF_MuR_Down", "MuR_MuR_Up",
                   "FSR_Down", "FSR_Up",
                   "ISR_TT_Down", "ISR_TT_Up", "ISR_ST_Down", "ISR_ST_Up",
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
      // for (int i = 0; i < 10; i++)
      {
        TString pdf_error_set = "PDF_Error_Set_" + to_string(i);
        syst_name.push_back(pdf_error_set);
      }

      if (mode == "Merge" && chk_qcd_data_driven == true)
      {
        syst_name.push_back("QCD_TF_Up");
        syst_name.push_back("QCD_TF_Down");
      } // if (mode == "Merge" && chk_qcd_data_driven)
    } // else
  } // else

  n_syst = syst_name.size();
  cout << "N_Syst = " << n_syst << endl;

  return;
} // void Histo_Syst::Config_Syst()

//////////

void Histo_Syst::Config_Variable()
{
  if (mode == "Cal_TF")
  {
    variable_conf = {{"Lepton_Eta", histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up}};
  }
  else
  {
    if (debug)
      variable_conf = {{"N_Vertex", 40, 0, 80}};
    else
    {
      variable_conf = {{"N_Vertex", 40, 0, 80},
                       {"Lepton_Pt", 50, 0, 300},
                       {"Lepton_Eta", 30, -3, 3},
                       {"N_Jets", 30, 0, 30},
                       {"N_BJets", 30, 0, 30},
                       {"N_CJets", 30, 0, 30},
                       {"Pt_Leading_Jet", 50, 0, 300},
                       {"Pt_Subleading_Jet", 50, 0, 300},
                       {"Eta_Leading_Jet", 50, -3, 3},
                       {"Eta_Subleading_jet", 50, -3, 3},
                       {"BvsC_Leading_Jet", 50, 0, 1},
                       {"CvsB_Leading_Jet", 50, 0, 1},
                       {"CvsL_Leading_Jet", 50, 0, 1},
                       {"BvsC_Subleading_Jet", 50, 0, 1},
                       {"CvsB_Subleading_Jet", 50, 0, 1},
                       {"CvsL_Subleading_Jet", 50, 0, 1},
                       {"Met_Pt", 50, 0, 300},
                       {"Met_Phi", 80, -4, 4},
                       {"Best_MVA_Score_Pre", 100, -1, 1},
                       {"Best_MVA_Score", 100, -1, 1},
                       {"Had_T", 100, 70, 270},
                       {"Had_W", 60, 20, 140},
                       {"Lep_T", 100, 70, 270},
                       {"Lep_W", 60, 20, 140},
                       {"BvsC_W_u", 50, 0, 1},
                       {"CvsB_W_u", 50, 0, 1},
                       {"CvsL_W_u", 50, 0, 1},
                       {"BvsC_W_d", 50, 0, 1},
                       {"CvsB_W_d", 50, 0, 1},
                       {"CvsL_W_d", 50, 0, 1},
                       {"Template_MVA_Score", 100, 0, 1}};
    } // else
  } // else

  n_variable = variable_conf.size();

  return;
} // void Histo_Syst::Config_Variable()

//////////

void Histo_Syst::Data_Driven()
{
  cout << "[Histo_Syst::Data_Driven]: Init" << endl;

  // subtraction
  for (int i = 0; i < n_region; i++)
  {
    for (int j = 0; j < n_syst; j++)
    {
      for (int k = 0; k < n_variable; k++)
      {
        // data
        histo_subtracted_data_driven[i][j][k]->Add(histo_data_dd[0][i][k]);

        // subtract mc
        for (int l = 0; l < n_sample_merge_mc; l++)
        {
          if (vec_short_name_mc[l].Contains("QCD_bEn"))
            continue;

          histo_subtracted_data_driven[i][j][k]->Add(histo_mc_dd[0][i][j][l][k], -1);
        } // loop over n_sample
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region
  cout << "test test " << endl;
  // apply TF
  for (int i = 0; i < n_region; i++)
  {
    cout << region_name[i] << endl;
    for (int j = 0; j < n_syst; j++)
    {
      cout << syst_name[j] << endl;
      for (int k = 0; k < n_variable; k++)
      {
        cout << variable_conf[k].variable_title << endl;
        for (int l = 0; l < variable_conf[k].n_bin; l++)
        {
          cout << "l = " << l << endl;

          for (int m = 0; m < histo_tf_n_bin; m++)
          {
            cout << "m = " << m << endl;

            float content = histo_subtracted_data_driven[i][j][k]->GetBinContent(l + 1, m + 1);
            float error = histo_subtracted_data_driven[i][j][k]->GetBinError(l + 1, m + 1);
            cout << "test 0" << endl;
            cout << "test 0 " << histo_tf[i] << endl;
            float tf = histo_tf[i]->GetBinContent(m + 1);
            float tf_error = histo_tf[i]->GetBinError(m + 1);
            cout << "test 1" << endl;
            float tf_up = tf + tf_error;
            float tf_down = tf - tf_error;
            cout << "test 2" << endl;
            if (content < 0)
              content = 0;
            cout << "test 3" << endl;
            histo_tf_corrected[i][j][k]->SetBinContent(l + 1, m + 1, content * tf);
            histo_tf_corrected[i][j][k]->SetBinError(l + 1, m + 1, error * tf);

            // cout << l << " " << m << " " << content << " " << tf << endl;

            if (j == 0)
            {
              histo_tf_corrected_up[i][k]->SetBinContent(l + 1, m + 1, content * tf_up);
              histo_tf_corrected_up[i][k]->SetBinError(l + 1, m + 1, error * tf_up);

              histo_tf_corrected_down[i][k]->SetBinContent(l + 1, m + 1, content * tf_down);
              histo_tf_corrected_down[i][k]->SetBinError(l + 1, m + 1, error * tf_down);
            } // nominal
            cout << "test 4" << endl;
          } // loop over tf_n_bin
        } // loop over n_bin
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  cout << "[Histo_Syst::Data_Driven]: Done" << endl;

  return;
} // void Histo_Syst::Data_Driven()

//////////

void Histo_Syst::Fill_Histo_Data()
{
  int abcd_region_index = Set_ABCD_Region();
  int region_index = Set_Region();

  // to save time
  if (mode == "2D" && (abcd_region_name[abcd_region_index] == "A" || abcd_region_name[abcd_region_index] == "B"))
    return;

  // for blind
  if (abcd_region_name[abcd_region_index] == "D" && region_name[region_index] == "3B")
    return;

  // this part is not satisfactory, but don't waste time
  if (mode == "Cal_TF")
  {
    dynamic_cast<TH1D *>(histo_data[abcd_region_index][region_index][0])->Fill(event.lepton_eta, 1.);
  }
  else
  {
    if (debug)
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][0])->Fill(event.n_pv, event.lepton_eta, 1.);
    else
    {
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][0])->Fill(event.n_pv, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][1])->Fill(event.lepton_pt, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][2])->Fill(event.lepton_eta, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][3])->Fill(event.n_jets, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][4])->Fill(event.n_bjets, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][5])->Fill(event.n_cjets, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][6])->Fill(event.pt_leading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][7])->Fill(event.pt_subleading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][8])->Fill(event.eta_leading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][9])->Fill(event.eta_subleading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][10])->Fill(event.bvsc_leading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][11])->Fill(event.cvsb_leading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][12])->Fill(event.cvsl_leading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][13])->Fill(event.bvsc_subleading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][14])->Fill(event.cvsb_subleading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][15])->Fill(event.cvsl_subleading_jet, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][16])->Fill(event.met_pt, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][17])->Fill(event.met_phi, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][18])->Fill(event.best_mva_score_pre, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][19])->Fill(event.best_mva_score, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][20])->Fill(event.m_had_t, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][21])->Fill(event.m_had_w, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][22])->Fill(event.m_lep_t, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][23])->Fill(event.m_lep_w, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][24])->Fill(event.bvsc_w_u, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][25])->Fill(event.cvsb_w_u, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][26])->Fill(event.cvsl_w_u, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][27])->Fill(event.bvsc_w_d, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][28])->Fill(event.cvsb_w_d, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][29])->Fill(event.cvsl_w_d, event.lepton_eta, 1.);
      dynamic_cast<TH2D *>(histo_data[abcd_region_index - 2][region_index][30])->Fill(event.template_score, event.lepton_eta, 1);
    } // else
  } // else

  return;
} // void Histo_Syst::Fill_Histo_Data()

//////////

void Histo_Syst::Fill_Histo_MC(const TString &sample_name, const TString &syst_fix)
{
  // int sample_index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();
  int abcd_region_index = Set_ABCD_Region();
  int region_index = Set_Region();

  // to save time
  if (mode == "2D" && (abcd_region_name[abcd_region_index] == "A" || abcd_region_name[abcd_region_index] == "B"))
    return;

  int histo_index = Histo_Index(sample_name);
  TString histo_name_rf = Histo_Name_RF(sample_name);

  float weight_ckm = Reweight_CKM(sample_name);

  // cout << "test " << sample_name << " " << sample_index << " " << histo_name_rf << endl;

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

    event.weight = 1;
    event.weight *= event.weight_lumi;
    event.weight *= event.weight_mc;
    event.weight *= event.weight_hem_veto;

    // ckm reweight
    event.weight *= weight_ckm;

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
    if (syst_type == "FSR_Down" && (sample_name.Contains("TT") || sample_name.Contains("ST")))
      event.weight *= event.weight_ps[0];
    else if (syst_type == "FSR_Up" && (sample_name.Contains("TT") || sample_name.Contains("ST")))
      event.weight *= event.weight_ps[1];
    else if (syst_type == "ISR_TT_Down" && sample_name.Contains("TT"))
      event.weight *= event.weight_ps[2];
    else if (syst_type == "ISR_TT_Up" && sample_name.Contains("TT"))
      event.weight *= event.weight_ps[3];
    else if (syst_type == "ISR_ST_Down" && sample_name.Contains("ST"))
      event.weight *= event.weight_ps[2];
    else if (syst_type == "ISR_ST_Up" && sample_name.Contains("ST"))
      event.weight *= event.weight_ps[3];

    // scale variation
    if (syst_type == "MuF_TT_Down" && sample_name.Contains("TT"))
      event.weight *= event.weight_scale_variation_2;
    else if (syst_type == "MuF_TT_Up" && sample_name.Contains("TT"))
      event.weight *= event.weight_scale_variation_1;
    else if (syst_type == "MuF_ST_Down" && sample_name.Contains("ST"))
      event.weight *= event.weight_scale_variation_2;
    else if (syst_type == "MuF_ST_Up" && sample_name.Contains("ST"))
      event.weight *= event.weight_scale_variation_1;
    else if (syst_type == "MuR_TT_Down" && sample_name.Contains("TT"))
      event.weight *= event.weight_scale_variation_6;
    else if (syst_type == "MuR_TT_Up" && sample_name.Contains("TT"))
      event.weight *= event.weight_scale_variation_3;
    else if (syst_type == "MuR_ST_Down" && sample_name.Contains("ST"))
      event.weight *= event.weight_scale_variation_6;
    else if (syst_type == "MuR_ST_Up" && sample_name.Contains("ST"))
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
      // cout << "test " << histo_name_rf << " " << b_tagging_rf_type << endl;
      event.weight *= tagging_rf.Get_Tagging_RF_B_Tag(abcd_region_name[abcd_region_index], histo_name_rf, b_tagging_rf_type, event.n_jets, event.ht);
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
      event.weight *= tagging_rf.Get_Tagging_RF_C_Tag(abcd_region_name[abcd_region_index], histo_name_rf, c_tagging_rf_type, event.n_pv, event.ht);
    } // else if (tagger == "C")

    // this part is not satisfactory, but don't waste time
    if (mode == "Cal_TF")
    {
      dynamic_cast<TH1D *>(histo_mc[abcd_region_index][region_index][i][histo_index][0])->Fill(event.lepton_eta, event.weight);
    }
    else
    {
      if (debug)
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][0])->Fill(event.n_pv, event.lepton_eta, event.weight);
      else
      {
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][0])->Fill(event.n_pv, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][1])->Fill(event.lepton_pt, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][2])->Fill(event.lepton_eta, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][3])->Fill(event.n_jets, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][4])->Fill(event.n_bjets, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][5])->Fill(event.n_cjets, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][6])->Fill(event.pt_leading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][7])->Fill(event.pt_subleading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][8])->Fill(event.eta_leading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][9])->Fill(event.eta_subleading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][10])->Fill(event.bvsc_leading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][11])->Fill(event.cvsb_leading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][12])->Fill(event.cvsl_leading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][13])->Fill(event.bvsc_subleading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][14])->Fill(event.cvsb_subleading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][15])->Fill(event.cvsl_subleading_jet, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][16])->Fill(event.met_pt, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][17])->Fill(event.met_phi, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][18])->Fill(event.best_mva_score_pre, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][19])->Fill(event.best_mva_score, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][20])->Fill(event.m_had_t, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][21])->Fill(event.m_had_w, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][22])->Fill(event.m_lep_t, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][23])->Fill(event.m_lep_w, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][24])->Fill(event.bvsc_w_u, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][25])->Fill(event.cvsb_w_u, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][26])->Fill(event.cvsl_w_u, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][27])->Fill(event.bvsc_w_d, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][28])->Fill(event.cvsb_w_d, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][29])->Fill(event.cvsl_w_d, event.lepton_eta, event.weight);
        dynamic_cast<TH2D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][30])->Fill(event.template_score, event.lepton_eta, event.weight);
      } // else
    } // else
  } // loop over n_syst

  return;
} // void Histo_Syst::Fill_Histo_MC(const int& sample_index)

//////////

int Histo_Syst::Histo_Index(const TString &sample_name)
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
} // int Histo_Syst::Histo_Index(const TString& sample_name)

//////////

TString Histo_Syst::Histo_Name_RF(const TString &sample_name)
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

int Histo_Syst::Get_Region_Index(const TString &region)
{
  return find(region_name.begin(), region_name.end(), region) - region_name.begin();
} // Region_Index Histo_Syst::Get_Region_Index(const TString& region)

//////////

void Histo_Syst::Init_Histo()
{
  // histo for MC
  // to save time, do not make unnecessary region A and B
  histo_mc = new TH1 *****[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_mc[i] = new TH1 ****[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_mc[i][j] = new TH1 ***[n_syst];
      for (int k = 0; k < n_syst; k++)
      {
        histo_mc[i][j][k] = new TH1 **[n_sample_merge_mc];
        for (int l = 0; l < n_sample_merge_mc; l++)
        {
          histo_mc[i][j][k][l] = new TH1 *[n_variable];
          for (int m = 0; m < n_variable; m++)
          {
            TString histo_name = abcd_region_name[i + 2] + "_" + region_name[j] + "_" + syst_name[k] + "_" + vec_short_name_mc[l] + "_" + variable_conf[m].variable_title;
            histo_mc[i][j][k][l][m] = new TH2D(histo_name, variable_conf[m].variable_title, variable_conf[m].n_bin, variable_conf[m].x_low, variable_conf[m].x_up, histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up);
          } // loop over n_variable
        } // loop over n_sample_merge_mc
      } // loop over n_syst
    } // loop over n_region
  } // loop over n_abcd_region-2

  // histo for data
  histo_data = new TH1 ***[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_data[i] = new TH1 **[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_data[i][j] = new TH1 *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = abcd_region_name[i + 2] + "_" + region_name[j] + "_" + variable_conf[k].variable_title;
        histo_data[i][j][k] = new TH2D(histo_name, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up, histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up);
      } // loop over n_variable
    } // loop over n_region
  } // loop over n_abcd_region

  // output file
  TString fout_name = "Vcb_2D_" + era + "_" + channel + "_";
  if (run_flag == "Central")
    fout_name += run_flag + to_string(index_split) + to_string(n_split);
  else
    fout_name += run_flag;
  fout_name += ".root";
  fout = new TFile(fout_name, "RECREATE");

  return;
} // void Histo_Syst::Init_Histo()

//////////

void Histo_Syst::Init_Histo_Data_Driven()
{
  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Init" << endl;

  TString fin_2d_name = path_base + "/Workplace/Histo_Syst/Vcb_2D_" + era + "_" + channel + "_All.root";
  fin_2d = new TFile(fin_2d_name);

  // for mc
  histo_mc_dd = new TH2D *****[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_mc_dd[i] = new TH2D ****[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_mc_dd[i][j] = new TH2D ***[n_syst];
      for (int k = 0; k < n_syst; k++)
      {
        histo_mc_dd[i][j][k] = new TH2D **[n_sample_merge_mc];
        for (int l = 0; l < n_sample_merge_mc; l++)
        {
          histo_mc_dd[i][j][k][l] = new TH2D *[n_variable];
          for (int m = 0; m < n_variable; m++)
          {
            TString histo_name = abcd_region_name[i + 2] + "/" + region_name[j] + "/" + syst_name[k] + "/" + vec_short_name_mc[l] + "/" + variable_conf[m].variable_title;
            histo_mc_dd[i][j][k][l][m] = (TH2D *)fin_2d->Get(histo_name);

            // cout << histo_name << " " << histo_mc_dd[i][j][k][l][m] << endl;

          } // loop over n_variable
        } // loop over n_sample_merge_mc
      } // loop over n_syst
    } // loop over n_region
  } // loop over n_abcd_region -2

  // for data
  histo_data_dd = new TH2D ***[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_data_dd[i] = new TH2D **[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_data_dd[i][j] = new TH2D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = abcd_region_name[i + 2] + "/" + region_name[j] + "/Data/" + variable_conf[k].variable_title;
        histo_data_dd[i][j][k] = (TH2D *)fin_2d->Get(histo_name);

        // cout << histo_name << " " << histo_data_dd[i][j][k] << endl;
      } // loop over n_variable
    } // loop over n_region
  } // loop over n_abcd_region -2

  // for histo_subtracted_data_driven
  histo_subtracted_data_driven = new TH2D ***[n_region];
  histo_tf_corrected = new TH2D ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_subtracted_data_driven[i] = new TH2D **[n_syst];
    histo_tf_corrected[i] = new TH2D **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      histo_subtracted_data_driven[i][j] = new TH2D *[n_variable];
      histo_tf_corrected[i][j] = new TH2D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = "Subtracted_Data_Driven_" + region_name[i] + "_" + syst_name[j] + "_" + variable_conf[k].variable_title;
        histo_subtracted_data_driven[i][j][k] = new TH2D(histo_name, histo_name, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up, histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up);

        histo_name = "TF_Corrected_" + region_name[i] + "_" + syst_name[j] + "_" + variable_conf[k].variable_title;
        histo_tf_corrected[i][j][k] = new TH2D(histo_name, histo_name, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up, histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up);
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  histo_tf_corrected_up = new TH2D **[n_region];
  histo_tf_corrected_down = new TH2D **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_tf_corrected_up[i] = new TH2D *[n_variable];
    histo_tf_corrected_down[i] = new TH2D *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = "TF_Corrected_Up_" + region_name[i] + "_" + variable_conf[j].variable_title;
      histo_tf_corrected_up[i][j] = new TH2D(histo_name, histo_name, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up, histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up);

      histo_name = "TF_Corrected_Down_" + region_name[i] + "_" + variable_conf[j].variable_title;
      histo_tf_corrected_down[i][j] = new TH2D(histo_name, histo_name, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up, histo_tf_n_bin, histo_tf_x_low, histo_tf_x_up);
    } // loop over n_variable
  } // loop over n_region

  TString fin_tf_name = path_base + "/Workplace/Histo_Syst/Vcb_TF_" + era + "_" + channel + ".root";
  fin_tf = new TFile(fin_tf_name);

  histo_tf = new TH1D *[n_region];
  for (int i = 0; i < n_region; i++)
  {
    TString histo_name = region_name[0] + "/TF"; // let's use TF from 2b region for all
    histo_tf[i] = (TH1D *)fin_tf->Get(histo_name);
    cout << "test test test " << fin_tf << endl;
    cout << histo_name << endl;
    cout << "test test test " << histo_tf[i] << endl;
  }

  TString fout_name = "Vcb_Histos_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "RECREATE");

  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Done" << endl;

  return;
} // void Histo_Syst::Init_Histo_Data_Driven()

//////////

void Histo_Syst::Init_Histo_TF()
{
  // histo for MC
  histo_mc = new TH1 *****[n_abcd_region];
  for (int i = 0; i < n_abcd_region; i++)
  {
    histo_mc[i] = new TH1 ****[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_mc[i][j] = new TH1 ***[n_syst];
      for (int k = 0; k < n_syst; k++)
      {
        histo_mc[i][j][k] = new TH1 **[n_sample_merge_mc];
        for (int l = 0; l < n_sample_merge_mc; l++)
        {
          histo_mc[i][j][k][l] = new TH1 *[n_variable];
          for (int m = 0; m < n_variable; m++)
          {
            TString histo_name = abcd_region_name[i] + "_" + region_name[j] + "_" + syst_name[k] + "_" + vec_short_name_mc[l] + "_" + variable_conf[m].variable_title;
            histo_mc[i][j][k][l][m] = new TH1D(histo_name, variable_conf[m].variable_title, variable_conf[m].n_bin, variable_conf[m].x_low, variable_conf[m].x_up);
          } // loop over n_variable
        } // loop over n_sample_merge_mc
      } // loop over n_syst
    } // loop over n_region
  } // loop over n_abcd_region

  // histo for data
  histo_data = new TH1 ***[n_abcd_region];
  for (int i = 0; i < n_abcd_region; i++)
  {
    histo_data[i] = new TH1 **[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_data[i][j] = new TH1 *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = abcd_region_name[i] + "_" + region_name[j] + "_" + variable_conf[k].variable_title;
        histo_data[i][j][k] = new TH1D(histo_name, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);
      } // loop over n_variable
    } // loop over n_region
  } // loop over n_abcd_region

  histo_subtracted_tf = new TH1D **[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_subtracted_tf[i] = new TH1D *[n_region];
    for (int j = 0; j < n_region; j++)
    {
      TString histo_name = "Subtrated_" + abcd_region_name[i] + "_" + region_name[j];
      histo_subtracted_tf[i][j] = new TH1D(histo_name, histo_name, variable_conf[0].n_bin, variable_conf[0].x_low, variable_conf[0].x_up);
    } // loop over n_region
  } // loop over n_abcd_region - 2

  histo_tf = new TH1D *[n_region];
  for (int i = 0; i < n_region; i++)
  {
    TString histo_name = "TF_" + region_name[i];
    histo_tf[i] = new TH1D(histo_name, histo_name, variable_conf[0].n_bin, variable_conf[0].x_low, variable_conf[0].x_up);
  } // loop over n_region

  // output file
  TString fout_name = "Vcb_TF_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "RECREATE");

  return;
} // void Histo_Syst::Init_Histo_TF()

//////////

void Histo_Syst::Init_Merge_PDF_Error_Set()
{
  cout << "[Histo_Syst::Init_Merge_PDF_Error_Set]: Init" << endl;

  TString fout_name = path_base + "/Workplace/Histo_Syst/Vcb_Histos_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "UPDATE");

  histo_mc_merge = new TH1D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc_merge[i] = new TH1D ***[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      histo_mc_merge[i][j] = new TH1D **[n_sample_merge_mc];
      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        histo_mc_merge[i][j][k] = new TH1D *[n_variable];
        for (int l = 0; l < n_variable; l++)
        {
          TString histo_name = region_name[i] + "/" + syst_name[j] + "/" + vec_short_name_mc[k] + "/" + variable_conf[l].variable_title;
          histo_mc_merge[i][j][k][l] = (TH1D *)fout->Get(histo_name);
          // cout << histo_name << " " << histo_mc_merge[i][j][k][l] << endl;
        } // loop over n_variable
      } // loop over n_sample_merge_mc
    } // loop over n_syst
  } // loop over n_region

  cout << "Histo_Syst::Init_Merge_PDF_Error_Set: Done" << endl;

  return;
} // void Histo_Syst::Init_Merge_PDF_Error_Set()

//////////

void Histo_Syst::Merge_PDF_Error_Set()
{
  cout << "[Histo_Syst::Merge_PDF_Error_Set]: Init" << endl;
  // chk_merge_pdf_error_set = true;

  histo_mc_pdf_error_set_down = new TH1D ***[n_region];
  histo_mc_pdf_error_set_up = new TH1D ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc_pdf_error_set_down[i] = new TH1D **[n_sample_merge_mc];
    histo_mc_pdf_error_set_up[i] = new TH1D **[n_sample_merge_mc];
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      histo_mc_pdf_error_set_down[i][j] = new TH1D *[n_variable];
      histo_mc_pdf_error_set_up[i][j] = new TH1D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name_pdf_error = "PDF_Error_Set_Down_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        histo_mc_pdf_error_set_down[i][j][k] = new TH1D(histo_name_pdf_error, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);

        histo_name_pdf_error = "PDF_Error_Set_Up_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        histo_mc_pdf_error_set_up[i][j][k] = new TH1D(histo_name_pdf_error, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);
      } // loop over n_variable
    } // loop over n_sample_merge_mc
  } // loop over n_region

  // find min and max and fill histo
  for (int i = 0; i < n_region; i++)
  {
    cout << region_name[i] << endl;
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      cout << vec_short_name_mc[j] << endl;
      for (int k = 0; k < n_variable; k++)
      {
        for (int l = 0; l < variable_conf[k].n_bin; l++)
        {
          float max = -999999999999999999;
          float min = 999999999999999999;
          for (int m = 0; m < n_pdf_error_set; m++)
          {
            int index;
            auto it = find(syst_name.begin(), syst_name.end(), "PDF_Error_Set_" + to_string(m));
            if (it != syst_name.end())
              index = distance(syst_name.begin(), it);
            else
            {
              cout << "Can't find the corresponding PDF_Error_Set. Check it." << endl;
              exit(1);
            }

            float content = histo_mc_merge[i][index][j][k]->GetBinContent(l + 1);

            // if(variable_conf[k].variable_title=="N_BJets" && content!=0) cout << content << " " << min << " " << max << endl;

            if (content < min)
              min = content;
            if (max < content)
              max = content;

          } // loop over n_pdf_error_set

          histo_mc_pdf_error_set_down[i][j][k]->SetBinContent(l + 1, min);
          histo_mc_pdf_error_set_up[i][j][k]->SetBinContent(l + 1, max);
        } // loop over n_bin

        histo_mc_pdf_error_set_down[i][j][k]->Sumw2();
        histo_mc_pdf_error_set_up[i][j][k]->Sumw2();
      } // loop over n_variable
    } // loop over n_sample_merge_mc
  } // loop over n_region

  cout << "[Histo_Syst::Merge_PDF_Error_Set]: Done" << endl;

  return;
} // void Histo_Syst::Merge_PDF_Error_Set()

//////////

void Histo_Syst::Read_Tree()
{
  cout << "[Histo_Syst::Read_Tree]: Start to read trees" << endl;

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

        // cut on best_mva_score_pre
        if (chk_cut_best_mva_score_pre && event.best_mva_score_pre < cut_best_mva_score_pre)
          continue;

        // cut on template_score
        if (chk_cut_template_score && event.template_score < cut_template_score)
          continue;

        // event veto
        if (100 < TMath::Abs(event.weight_pdf_alternative))
        {
          cout << "strange weight_pdf_alternative is detected. " << syst_loop_name << ", " << i << ", weight_pdf_alternative = " << event.weight_pdf_alternative << endl;
          cout << "Continue" << endl;

          continue;
        }

        // event.Swap();

        Fill_Histo_MC(sample_name_short, syst_fix);
        // if (syst_fix == "None") Fill_Histo_Weight(region_index, sample_index);
      } // loop over entries
    } // loop over map_tree_mc

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

      // cut on best_mva_score_pre
      if (chk_cut_best_mva_score_pre && event.best_mva_score_pre < cut_best_mva_score_pre)
        continue;

      // cut on template_score
      if (chk_cut_template_score && event.template_score < cut_template_score)
        continue;

      // event.Swap();

      Fill_Histo_Data();
    } // loop over n_entries

    // map_fin_data[fin_name]->Close();
  } // loop over data period

  return;
} // void Histo_Syst::Read_Tree()

//////////

void Histo_Syst::Register_Sample()
{
  cout << "[Histo_Syst::Register_Sample]: Init" << endl;

  // job splitting for central and syst_reweight
  if (run_flag.Contains("Central"))
  {
    index_split = TString(run_flag(7, 2)).Atoi();
    n_split = TString(run_flag(9, 2)).Atoi();
    run_flag = "Central";
  }

  cout << "[Histo_Syst::Register_Sample]: run_flag = " << run_flag << endl;
  if (run_flag == "Central")
    cout << "Index_Split = " << index_split << ", N_Split = " << n_split << endl;

  if (channel == "Mu" && run_flag.Contains("Electron"))
    throw "Input run_flag is not compatible with Mu channel. Abort process.";

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  tree_name = "/Result_Tree";

  if (run_flag != "Data")
  {
    for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
    {
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

      /*
      if (channel == "El")
      {
        if (run_flag == "El_En_Down" || run_flag == "All")
        {
          map_fin_mc_eec_down[it->first] = new TFile(result_path + "ElectronEnDown/" + it->second);
          map_tree_mc_eec_down[it->first] = (TTree *)map_fin_mc_eec_down[it->first]->Get(key_base + "ElectronEnDown" + tree_name);

          event.Setup_Tree(map_tree_mc_eec_down[it->first], Syst::ElectronEnDown, false);
        }

        if (run_flag == "El_En_Up" || run_flag == "All")
        {
          map_fin_mc_eec_up[it->first] = new TFile(result_path + "ElectronEnUp/" + it->second);
          map_tree_mc_eec_up[it->first] = (TTree *)map_fin_mc_eec_up[it->first]->Get(key_base + "ElectronEnUp" + tree_name);

          event.Setup_Tree(map_tree_mc_eec_up[it->first], Syst::ElectronEnUp, false);
        }

        if (run_flag == "El_Res_Down")
        {
          map_fin_mc_eer_down[it->first] = new TFile(result_path + "ElectronResDown/" + it->second);
          map_tree_mc_eer_down[it->first] = (TTree *)map_fin_mc_eer_down[it->first]->Get(key_base + "ElectronResDown" + tree_name);

          event.Setup_Tree(map_tree_mc_eer_down[it->first], Syst::ElectronResDown, false);
        }

        if (run_flag == "El_Res_Up" || run_flag == "All")
        {
          map_fin_mc_eer_up[it->first] = new TFile(result_path + "ElectronResUp/" + it->second);
          map_tree_mc_eer_up[it->first] = (TTree *)map_fin_mc_eer_up[it->first]->Get(key_base + "ElectronResUp" + tree_name);

          event.Setup_Tree(map_tree_mc_eer_up[it->first], Syst::ElectronResUp, false);
        }
      }
      */
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

    /*
    if (channel == "El")
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
    */
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

  cout << "[Histo_Syst::Register_Sample]: Done" << endl;
} // void Histo_Syst::Register_Sample()

//////////

void Histo_Syst::Register_Sample_TF()
{
  cout << "[Histo_Syst::Register_Sample_TF]: Init" << endl;

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";
  tree_name = "/Result_Tree";

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);
    map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get(key_base + "Central" + tree_name);
    event.Setup_Tree(map_tree_mc[it->first], Syst::Central, true);
  }
  map_map_tree_mc["Central"] = &map_tree_mc;

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

  cout << "[Histo_Syst::Register_Sample_TF]: Done" << endl;

  return;
} // void Histo_Syst::Register_Sample_TF()

//////////

void Histo_Syst::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)
{
  cout << "[Histo_Syst::Register_Top_Syst]:" << type << endl;

  map_fin_syst = map_fin_mc;
  map_tree_syst = map_tree_mc;

  map_fin_syst.erase("TTLJ");
  map_tree_syst.erase("TTLJ");

  map_fin_syst.erase("TTLL");
  map_tree_syst.erase("TTLL");

  TString result_path = path_base + "/Sample/" + era + "/" + channel + "/RunResult/";

  map_fin_syst["TTLJ_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLJ_" + type]);
  map_tree_syst["TTLJ_" + type] = (TTree *)map_fin_syst["TTLJ_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLJ_" + type], Syst::Central);

  map_fin_syst["TTLL_" + type] = new TFile(result_path + "Top_Syst/" + samples.map_top_syst["TTLL_" + type]);
  map_tree_syst["TTLL_" + type] = (TTree *)map_fin_syst["TTLL_" + type]->Get(key_base + "Central" + tree_name);
  event.Setup_Tree(map_tree_syst["TTLL_" + type], Syst::Central);

  return;
} // void Histo_Syst::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)

//////////

float Histo_Syst::Reweight_CKM(const TString &sample_name)
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
} // float Histo_Syst::Reweight_CKM(const TString& sample_name)

//////////

void Histo_Syst::Setup_Template_Reader()
{
  reader_template = new TMVA::Reader("!Color:!Silent");
  reader_template->AddVariable("best_mva_score", &event.best_mva_score);
  reader_template->AddVariable("n_bjets", &n_bjets_f);
  reader_template->AddVariable("n_cjets", &n_cjets_f);
  reader_template->AddVariable("bvsc_w_u", &event.bvsc_w_u);
  reader_template->AddVariable("cvsb_w_u", &event.cvsb_w_u);
  reader_template->AddVariable("cvsl_w_u", &event.cvsl_w_u);
  reader_template->AddVariable("bvsc_w_d", &event.bvsc_w_d);
  reader_template->AddVariable("cvsb_w_d", &event.cvsb_w_d);
  reader_template->AddVariable("cvsl_w_d", &event.cvsl_w_d);

  template_mva_name = "Classification_BDTB";

  TString weight_file = getenv("Vcb_Post_Analysis_WD");
  weight_file += "/Macro/Template/";
  weight_file += era;
  weight_file += "/";
  weight_file += channel;
  weight_file += "/weights/TMVA";
  weight_file += template_mva_name;
  weight_file += ".weights.xml";

  reader_template->BookMVA(template_mva_name, weight_file);

  return;
} // void Histo_Syst::Setup_Template_Reader()

//////////

int Histo_Syst::Set_ABCD_Region()
{
  bool chk_pass_iso;
  if (channel == "Mu")
  {
    if (event.lepton_rel_iso < REL_ISO_MUON)
      chk_pass_iso = true;
    else
      chk_pass_iso = false;
  }
  else if (channel == "El")
  {
    float rel_iso_electron_a;
    float rel_iso_electron_b;
    if (TMath::Abs(event.lepton_eta) <= 1.479)
    {
      rel_iso_electron_a = REL_ISO_ELECTRON_BARREL_A;
      rel_iso_electron_b = REL_ISO_ELECTRON_BARREL_B;
    }
    else
    {
      rel_iso_electron_a = REL_ISO_ELECTRON_ENDCAP_A;
      rel_iso_electron_b = REL_ISO_ELECTRON_ENDCAP_B;
    }

    if (event.lepton_rel_iso < rel_iso_electron_a + rel_iso_electron_b / event.lepton_pt_uncorr)
      chk_pass_iso = true;
    else
      chk_pass_iso = false;
  }

  if (MET_PT < event.met_pt)
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
} // int Histo_Syst::Set_ABCD_Region()

//////////

int Histo_Syst::Set_Region()
{
  // bool chk_b_tagged_had_t_b = bvsc_wp_m < event.bvsc_had_t_b ? true : false;
  // bool chk_b_tagged_lep_t_b = bvsc_wp_m < event.bvsc_lep_t_b ? true : false;
  // bool chk_best_mva_score = 0.9 < event.best_mva_score ? true : false;
  // bool chk_n_b_tagged = 3 <= event.n_bjets ? true : false;
  // bool chk_n_c_tagged = event.n_cjets == 1 ? true : false;
  // bool chk_c_tagged_w_u = (cvsb_wp_m < event.cvsb_w_u && cvsl_wp_m < event.cvsl_w_u) ? true : false;
  // bool chk_b_tagged_w_d = bvsc_wp_m < event.bvsc_w_d ? true : false;

  if (event.n_bjets == 2)
    return 0;
  else
    return 1;

} // void Histo_Syst::Set_Region()

//////////