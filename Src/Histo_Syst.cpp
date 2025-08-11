#include "Histo_Syst.h"

ClassImp(Histo_Syst);

//////////

Histo_Syst::Histo_Syst(const TString &a_era, const TString &a_channel, const TString &a_mode, const int &a_index_tree_type, const int &a_last_index_tree_type, const TString &a_tagger, const TString &a_swap_mode)
    : samples(a_era, a_channel, (a_mode == "Cal_TF") ? "Vcb_Cal_TF" : "Vcb"), event(a_era, a_channel, (a_mode == "Cal_TF") ? "Vcb_Cal_TF" : "Vcb", a_swap_mode), tagging_rf(a_era), modelling_patch("Application")
{
  TH1::AddDirectory(kFALSE);

  cout << "[Histo_Syst::Histo_Syst]: Init analysis" << endl;

  debug = false;

  reduction = 1;

  era = a_era;
  channel = a_channel;
  mode = a_mode;
  index_tree_type = a_index_tree_type;
  last_index_tree_type = a_last_index_tree_type;
  tagger = a_tagger;

  cout << "[Histo_Syst::Histo_Syst]: mode = " << mode << endl;

  if (era.Contains("2016"))
    year = "2016";
  else
    year = era;

  // chk_bin_optimizer = true;
  chk_bin_optimizer = false;

  // syst_tree
  vec_tree_type = {"Data",
                   "UEDown", "UEUp",
                   "JetResDown", "JetResUp",
                   "JetEnDown", "JetEnUp",
                   "CP5Down", "CP5Up",
                   "hdampDown", "hdampUp",
                   "mtop171p5", "mtop173p5"};

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

  chk_cut_best_mva_score_pre = false;
  if (chk_cut_best_mva_score_pre)
    cut_best_mva_score_pre = 0.2;
  else
    cut_best_mva_score_pre = 0;

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
  if (mode == "Cal_TF")
    region_name = {"TwoB"};
  else
  {
    if (chk_bin_optimizer)
      region_name = {"TwoB", "ThreeB_CR"};
    else
      region_name = {"TwoB", "ThreeB_CR", "Signal"};
  }
  n_region = region_name.size();

  if (mode == "Cal_TF")
  {
    ROOT::EnableImplicitMT(2);

    Config_Sample();
    Config_Syst();
    Config_Variable();
    Register_Sample_TF();
    Init_Histo_TF();
    Read_Tree();
    Calculate_TF();
    Draw_TF();
  }
  else if (mode == "2D")
  {
    ROOT::EnableImplicitMT(4);

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
    // output file
    TString fout_name = "Vcb_TF_" + era + "_" + channel + "_" + tagger + "_tagger.root";
    fout = new TFile(fout_name, "RECREATE");

    // save raw histo
    for (int i = 0; i < n_region; i++)
    {
      TDirectory *dir_region = fout->mkdir(region_name[i]);

      for (int j = 0; j < n_abcd_region; j++)
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
      } // loop over n_abcd_region

      dir_region->cd();
      histo_tf[i]->SetTitle("TF");
      histo_tf[i]->SetName("TF");
      histo_tf[i]->Write();

      histo_tf_qcd_mc[i]->SetTitle("TF_QCD_Sample");
      histo_tf_qcd_mc[i]->SetName("TF_QCD_Sample");
      histo_tf_qcd_mc[i]->Write();
    } // loop over n_region

    cout << "[Histo_Syst::~Histo_Syst]: Closing root file" << endl;
    fout->Close();
    delete fout;
  } //  if (mode == "Cal_TF")

  else if (mode == "2D")
  {
    // output file
    TString fout_name = "Vcb_2D_" + era + "_" + channel + "_" + tagger + "_tagger_";
    if (index_tree_type != -1)
    {
      if (vec_tree_type[0] == "Central")
        fout_name += "Central" + to_string(index_split) + to_string(n_split);
      else
        fout_name += vec_tree_type[0];
    }
    else
      fout_name += "All";
    fout_name += ".root";
    fout = new TFile(fout_name, "RECREATE");

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

        // for Data
        // if (find(vec_tree_type.begin(), vec_tree_type.end(), "Data") != vec_tree_type.end() && (abcd_region_name[i] != "D" || region_name[j] != "Signal"))
        if (find(vec_tree_type.begin(), vec_tree_type.end(), "Data") != vec_tree_type.end())
        {
          cout << "Data" << endl;

          TDirectory *dir_data = dir_region->mkdir("Data");
          for (int k = 0; k < n_variable; k++)
          {
            dir_data->cd();
            histo_data[i - 2][j][k]->SetTitle(variable_conf[k].variable_title);
            histo_data[i - 2][j][k]->SetName(variable_conf[k].variable_title);
            histo_data[i - 2][j][k]->Write();
          } // loop over variable
        } // if (find(vec_tree_type.begin(), vec_tree_type.end(), "Data") != vec_tree_type.end())

        // for MC
        for (int k = 0; k < n_syst; k++)
        {
          if (syst_name[k] == "Jet_Res_Down" || syst_name[k] == "Jet_Res_Up" ||
              syst_name[k] == "UE_Down" || syst_name[k] == "UE_Up" ||
              syst_name[k] == "El_En_Down" || syst_name[k] == "El_En_Up" ||
              syst_name[k] == "El_Res_Down" || syst_name[k] == "El_Res_Up" ||
              syst_name[k] == "CP5_Down" || syst_name[k] == "CP5_Up" ||
              syst_name[k] == "hdamp_Down" || syst_name[k] == "hdamp_Up" ||
              syst_name[k] == "mtop_171p5" || syst_name[k] == "mtop_173p5" ||
              syst_name[k].Contains("Jet_En"))
          {
            TString temp = syst_name[k];
            temp.ReplaceAll("_", "");

            if (find(vec_tree_type.begin(), vec_tree_type.end(), temp) == vec_tree_type.end())
              continue;
          }
          else
          {
            if (find(vec_tree_type.begin(), vec_tree_type.end(), "Central") == vec_tree_type.end())
              continue;
          }

          cout << "syst_name = " << syst_name[k] << endl;

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
    delete fout;
  } // else if (mode == "2D")
  else if (mode == "Data_Driven")
  {
    cout << "[Histo_Syst::~Histo_Syst]: Writing fout." << endl;

    auto it = std::find(syst_name.begin(), syst_name.end(), "Nominal");
    int nominal_index = std::distance(syst_name.begin(), it);

    TString fout_name = "Vcb_Histos_" + era + "_" + channel + "_" + tagger + "_tagger.root";
    fout = new TFile(fout_name, "RECREATE");

    for (int i = 0; i < n_region; i++)
    {
      // cout << region_name[i] << endl;

      TDirectory *dir_region = fout->mkdir(region_name[i]);

      // MC
      for (int j = 0; j < n_syst; j++)
      {
        // cout << "\t" << syst_name[j] << endl;

        TDirectory *dir_syst = dir_region->mkdir(syst_name[j]);

        for (int k = 0; k < n_sample_merge_mc; k++)
        {
          // cout << "\t\t" << vec_short_name_mc[k] << endl;

          TDirectory *dir_sample = dir_syst->mkdir(vec_short_name_mc[k]);
          for (int l = 0; l < n_variable; l++)
          {
            TH1D *histo_proj = (TH1D *)(dynamic_cast<TH3D *>(histo_mc_dd[1][i][j][k][l])->ProjectionX()->Clone());

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
          TH1D *histo_proj = (TH1D *)histo_tf_corrected[i][nominal_index][k]->ProjectionX()->Clone();

          histo_proj->SetTitle(variable_conf[k].variable_title);
          histo_proj->SetName(variable_conf[k].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // loop over n_syst

      for (int j = 0; j < bin_eta_tf.size() - 1; j++)
      {
        for (int k = 0; k < bin_pt_tf.size() - 1; k++)
        {
          /* QCD TF Down */
          cout << "[Histo_Syst::~Histo_Syst]: Writing QCD TF Down" << endl;

          TString dir_name_down = "QCD_TF_Eta_Bin" + to_string(j) + "_Pt_Bin" + to_string(k) + "_Down";
          TDirectory *dir_qcd_tf_down = dir_region->mkdir(dir_name_down);
          for (int l = 0; l < n_sample_merge_mc; l++)
          {
            TDirectory *dir_sample = dir_qcd_tf_down->mkdir(vec_short_name_mc[l]);
            for (int m = 0; m < n_variable; m++)
            {
              TH1D *histo_proj = (TH1D *)(dynamic_cast<TH3D *>(histo_mc_dd[1][i][0][l][m])->ProjectionX()->Clone());

              dir_sample->cd();
              histo_proj->SetTitle(variable_conf[m].variable_title);
              histo_proj->SetName(variable_conf[m].variable_title);
              histo_proj->Write();
            } // loop over n_variable
          } // loop over n_sample

          TDirectory *dir_qcd_data_driven_tf_down = dir_qcd_tf_down->mkdir("QCD_Data_Driven");
          dir_qcd_data_driven_tf_down->cd();
          for (int l = 0; l < n_variable; l++)
          {
            TH1D *histo_proj = (TH1D *)histo_tf_corrected_down[i][l][j][k]->ProjectionX()->Clone();

            histo_proj->SetTitle(variable_conf[l].variable_title);
            histo_proj->SetName(variable_conf[l].variable_title);
            histo_proj->Write();
          } // loop over n_variable

          /* QCD TF Up */
          cout << "[Histo_Syst::~Histo_Syst]: Writing QCD TF Up" << endl;

          TString dir_name_up = "QCD_TF_Eta_Bin" + to_string(j) + "_Pt_Bin" + to_string(k) + "_Up";
          TDirectory *dir_qcd_tf_up = dir_region->mkdir(dir_name_up);
          for (int l = 0; l < n_sample_merge_mc; l++)
          {
            TDirectory *dir_sample = dir_qcd_tf_up->mkdir(vec_short_name_mc[l]);
            for (int m = 0; m < n_variable; m++)
            {
              TH1D *histo_proj = (TH1D *)(dynamic_cast<TH3D *>(histo_mc_dd[1][i][0][l][m])->ProjectionX()->Clone());

              dir_sample->cd();
              histo_proj->SetTitle(variable_conf[m].variable_title);
              histo_proj->SetName(variable_conf[m].variable_title);
              histo_proj->Write();
            } // loop over n_variable
          } // loop over n_sample

          TDirectory *dir_qcd_data_driven_tf_up = dir_qcd_tf_up->mkdir("QCD_Data_Driven");
          dir_qcd_data_driven_tf_up->cd();
          for (int l = 0; l < n_variable; l++)
          {
            TH1D *histo_proj = (TH1D *)histo_tf_corrected_up[i][l][j][k]->ProjectionX()->Clone();

            histo_proj->SetTitle(variable_conf[l].variable_title);
            histo_proj->SetName(variable_conf[l].variable_title);
            histo_proj->Write();
          } // loop over n_variable

        } // loop over bin_pt_tf
      } // loop over bin_eta_tf

      /* QCD Envelop Down */
      cout << "[Histo_Syst::~Histo_Syst]: Writing QCD Envelop Down" << endl;
      TDirectory *dir_qcd_envelop_down = dir_region->mkdir("QCD_Envelop_Down");
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_qcd_envelop_down->mkdir(vec_short_name_mc[j]);
        for (int k = 0; k < n_variable; k++)
        {
          TH1D *histo_proj = (TH1D *)(dynamic_cast<TH3D *>(histo_mc_dd[1][i][0][j][k])->ProjectionX()->Clone());

          dir_sample->cd();
          histo_proj->SetTitle(variable_conf[k].variable_title);
          histo_proj->SetName(variable_conf[k].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // loop over n_sample

      TDirectory *dir_qcd_data_driven_envelop_down = dir_qcd_envelop_down->mkdir("QCD_Data_Driven");
      dir_qcd_data_driven_envelop_down->cd();
      for (int j = 0; j < n_variable; j++)
      {
        histo_envelop_down[i][j]->SetTitle(variable_conf[j].variable_title);
        histo_envelop_down[i][j]->SetName(variable_conf[j].variable_title);
        histo_envelop_down[i][j]->Write();
      } // loop over n_variable

      /* QCD Envelop Up */
      cout << "[Histo_Syst::~Histo_Syst]: Writing QCD Envelop Up" << endl;
      TDirectory *dir_qcd_envelop_up = dir_region->mkdir("QCD_Envelop_Up");
      for (int j = 0; j < n_sample_merge_mc; j++)
      {
        TDirectory *dir_sample = dir_qcd_envelop_up->mkdir(vec_short_name_mc[j]);
        for (int k = 0; k < n_variable; k++)
        {
          TH1D *histo_proj = (TH1D *)(dynamic_cast<TH3D *>(histo_mc_dd[1][i][0][j][k])->ProjectionX()->Clone());

          dir_sample->cd();
          histo_proj->SetTitle(variable_conf[k].variable_title);
          histo_proj->SetName(variable_conf[k].variable_title);
          histo_proj->Write();
        } // loop over n_variable
      } // loop over n_sample

      TDirectory *dir_qcd_data_driven_envelop_up = dir_qcd_envelop_up->mkdir("QCD_Data_Driven");
      dir_qcd_data_driven_envelop_up->cd();
      for (int j = 0; j < n_variable; j++)
      {
        histo_envelop_up[i][j]->SetTitle(variable_conf[j].variable_title);
        histo_envelop_up[i][j]->SetName(variable_conf[j].variable_title);
        histo_envelop_up[i][j]->Write();
      } // loop over n_variable

      // Data
      cout << "Data" << endl;

      TDirectory *dir_data = dir_region->mkdir("Data");
      dir_data->cd();
      for (int j = 0; j < n_variable; j++)
      {
        TH1D *histo_proj = (TH1D *)(dynamic_cast<TH3D *>(histo_data_dd[1][i][j])->ProjectionX()->Clone());

        histo_proj->SetTitle(variable_conf[j].variable_title);
        histo_proj->SetName(variable_conf[j].variable_title);
        histo_proj->Write();
      } // loop over n_variable
    } // loop over n_reigon

    cout << "[Histo_Syst::~Histo_Syst]: Writing fout. Done." << endl;
    cout << "[Histo_Syst::~Histo_Syst]: Closing root file" << endl;

    fin_2d->Close();
    fin_tf->Close();
    fout->Close();

    delete fin_2d;
    delete fin_tf;
    delete fout;

    cout << "[Histo_Syst::~Histo_Syst]: Closing root file. Done." << endl;
  } // else if (mode == "Data_Driven")

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
    delete fout;
  } // else

  cout << "[Histo_Syst::~Histo_Syst]: Done" << endl;
} // Histo_Syst::~Histo_Syst()

//////////

void Histo_Syst::Calculate_TF()
{
  cout << "[Histo_Syst::Calculate_TF]: Init" << endl;

  for (int i = 0; i < n_abcd_region; i++)
  {
    for (int j = 0; j < n_region; j++)
    {
      // data
      histo_subtracted_tf[i][j]->Add(histo_data[i][j][0]);

      // subtract mc
      for (int k = 0; k < n_sample_merge_mc; k++)
      {
        if (vec_short_name_mc[k] == "QCD_bEn" || vec_short_name_mc[k] == "QCD")
          continue;

        // TTbb_4f and bbDPS should be empty
        // However let's make it explicitly
        if (vec_short_name_mc[k].Contains("TTbb") || vec_short_name_mc[k].Contains("bbDPS"))
          continue;

        histo_subtracted_tf[i][j]->Add(histo_mc[i][j][0][k][0], -1);
      }
    } // loop over n_region
  } // loop over n_abcd_region-2

  // TF from QCD sample
  int index_syst_nominal = find(syst_name.begin(), syst_name.end(), "Nominal") - syst_name.begin();
  int index_qcd;
  if (chk_qcd_ben)
    index_qcd = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), "QCD_bEn") - vec_short_name_mc.begin();
  else
    index_qcd = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), "QCD") - vec_short_name_mc.begin();

  for (int i = 0; i < n_region; i++)
  {
    histo_tf[i]->Divide(histo_subtracted_tf[1][i], histo_subtracted_tf[0][i]);

    TH2D *histo_qcd_mc_b = (TH2D *)histo_mc[1][i][index_syst_nominal][index_qcd][0]->Clone();
    TH2D *histo_qcd_mc_a = (TH2D *)histo_mc[0][i][index_syst_nominal][index_qcd][0]->Clone();

    histo_qcd_mc_b->RebinX(histo_qcd_mc_b->GetXaxis()->GetNbins());
    histo_qcd_mc_b->RebinY(histo_qcd_mc_b->GetYaxis()->GetNbins());
    // histo_qcd_mc_b->RebinY(2);

    histo_qcd_mc_a->RebinX(histo_qcd_mc_a->GetXaxis()->GetNbins());
    histo_qcd_mc_a->RebinY(histo_qcd_mc_a->GetYaxis()->GetNbins());
    // histo_qcd_mc_a->RebinY(2);

    histo_tf_qcd_mc[i]->RebinX(histo_tf_qcd_mc[i]->GetXaxis()->GetNbins());
    histo_tf_qcd_mc[i]->RebinY(histo_tf_qcd_mc[i]->GetYaxis()->GetNbins());
    // histo_tf_qcd_mc[i]->RebinY(2);

    histo_tf_qcd_mc[i]->Divide(histo_qcd_mc_b, histo_qcd_mc_a);
  }

  cout << "[Histo_Syst::Calculate_TF]: Done" << endl;

  return;
} // void Histo_Syst::Calculate_TF()

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
  vec_short_name_mc.push_back("TTLL");    // TTLL+cc
  vec_short_name_mc.push_back("TTLL_CC"); // TTLL+cc
  vec_short_name_mc.push_back("TTLL_BB"); // TTLL+bb

  // TTbb 4f
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_TTbb_4f"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ_bbDPS"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_TTbb_4f"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_bbDPS"));

  if (chk_use_ttbb_4f)
  {
    vec_short_name_mc.push_back("TTLJ_bbDPS_BB_2");
    vec_short_name_mc.push_back("TTLJ_bbDPS_BB_4");
    vec_short_name_mc.push_back("TTLL_bbDPS_BB");
  }

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  // for(auto it = vec_short_name_mc.begin(); it != vec_short_name_mc.end(); it++)
  // {
  //   cout << "vec_short_name_mc: " << *it << endl;
  // }

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
                   "MuF_TT_Down", "MuF_TT_Up", "MuF_ST_Down", "MuF_ST_Up", "MuF_WJets_Down", "MuF_WJets_Up", "MuF_DYJets_Down", "MuF_DYJets_Up",
                   "MuR_TT_Down", "MuR_TT_Up", "MuR_ST_Down", "MuR_ST_Up", "MuR_WJets_Down", "MuR_WJets_Up", "MuR_DYJets_Down", "MuR_DYJets_Up",
                   //"MuF_MuR_Down", "MuR_MuR_Up",
                   "FSR_Down", "FSR_Up",
                   "ISR_TT_Down", "ISR_TT_Up", "ISR_ST_Down", "ISR_ST_Up", "ISR_WJets_Down", "ISR_WJets_Up", "ISR_DYJets_Down", "ISR_DYJets_Up",
                   "Top_Pt_Reweight",
                   "Trig_Down", "Trig_Up",
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
  // for (int i = 0; i < n_syst; i++)
  //   cout << syst_name[i] << endl;
  cout << "N_Syst = " << n_syst << endl;

  return;
} // void Histo_Syst::Config_Syst()

//////////

void Histo_Syst::Config_Variable()
{
  if (channel == "Mu")
  {
    // bin_eta_tf = {0, 1.479, MUON_ETA};
    // bin_eta_tf = {-1 * MUON_ETA, -1.479, 0, 1.479, MUON_ETA};
    // bin_eta_tf = {-1 * MUON_ETA, -1.479, 1.479, MUON_ETA};
    bin_eta_tf = {-1 * MUON_ETA, MUON_ETA};

    // if (era == "2016preVFP")
    //    bin_pt_tf = {26.0, 30.0, 34.0, 38, 42, 46, 50, 1000};
    //  else if (era == "2016postVFP")
    //   bin_pt_tf = {26.0, 30.0, 34.0, 38, 42, 46, 50, 1000};
    // else if (era == "2017")
    //   bin_pt_tf = {30.0, 34.0, 38.0, 42.0, 46.0, 50, 1000};
    // else if (era == "2018")
    //   bin_pt_tf = {26.0, 30.0, 34.0, 38, 42, 46, 50, 1000};

    // if (era == "2016preVFP")
    //   bin_pt_tf = {26.0, 30, 35.0, 50, 100, 2000};
    // else if (era == "2016postVFP")
    //   bin_pt_tf = {26.0, 30, 35.0, 50, 100, 2000};
    // else if (era == "2017")
    //   bin_pt_tf = {30, 35.0, 50, 100, 2000};
    // else if (era == "2018")
    //   bin_pt_tf = {26.0, 30, 35.0, 50, 100, 2000};

    if (era == "2016preVFP")
      bin_pt_tf = {26.0, 35.0, 60, 2000};
    else if (era == "2016postVFP")
      bin_pt_tf = {26.0, 35.0, 60, 2000};
    else if (era == "2017")
      bin_pt_tf = {30, 35.0, 60, 2000};
    else if (era == "2018")
      bin_pt_tf = {26.0, 35.0, 60, 2000};
  }

  else if (channel == "El")
  {
    // bin_eta_tf = {0, 1.479, ELECTRON_ETA};
    // bin_eta_tf = {-1 * ELECTRON_ETA, -1.479, 0, 1.479, ELECTRON_ETA};
    // bin_eta_tf = {-1 * ELECTRON_ETA, -1.479, 1.479, ELECTRON_ETA};
    bin_eta_tf = {-1 * ELECTRON_ETA, ELECTRON_ETA};

    // if (era == "2016preVFP")
    //   bin_pt_tf = {30.0, 35.0, 40.0, 45.0, 50.0, 1000};
    // else if (era == "2016postVFP")
    //   bin_pt_tf = {30.0, 35.0, 40.0, 45.0, 50.0, 1000};
    // else if (era == "2017")
    //   bin_pt_tf = {35.0, 40.0, 45.0, 50.0, 1000};
    // else if (era == "2018")
    //   bin_pt_tf = {35.0, 40.0, 45.0, 50.0, 100};

    if (era == "2016preVFP")
      bin_pt_tf = {30, 35, 40, 50.0, 200, 2000};
    else if (era == "2016postVFP")
      bin_pt_tf = {30, 35, 40, 50.0, 200, 2000};
    else if (era == "2017")
      bin_pt_tf = {35, 40, 50.0, 200, 2000};
    else if (era == "2018")
      bin_pt_tf = {35, 40, 50.0, 200, 2000};
  }

  if (mode != "Cal_TF" && !chk_bin_optimizer)
  {
    bin_template_mva_score.resize(n_region);

    // Data Driven
    if (era == "2016preVFP")
    {
      if (channel == "Mu")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.13, 0.15, 0.17, 0.19, 0.22, 0.26, 0.3, 0.36, 0.44, 0.57, 1};
        bin_template_mva_score[0] = {0.1, 0.11, 0.13, 0.15, 0.17, 0.19, 0.22, 0.26, 0.3, 0.36, 0.44, 0.57, 1};
        // bin_template_mva_score[1] = {0, 0.04, 0.09, 0.16, 0.23, 0.31, 0.41, 0.54, 0.72, 0.95};
        bin_template_mva_score[1] = {0.1, 0.16, 0.23, 0.31, 0.41, 0.54, 0.72, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
      }
      else if (channel == "El")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.09, 0.11, 0.14, 0.17, 0.21, 0.26, 0.34, 0.48, 1};
        bin_template_mva_score[0] = {0.1, 0.14, 0.17, 0.21, 0.26, 0.34, 0.48, 1};
        // bin_template_mva_score[1] = {0, 0.1, 0.22, 0.37, 0.59, 0.95};
        bin_template_mva_score[1] = {0.1, 0.22, 0.37, 0.59, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
      }
    }
    else if (era == "2016postVFP")
    {
      if (channel == "Mu")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.13, 0.15, 0.17, 0.2, 0.23, 0.27, 0.32, 0.4, 0.54, 1};
        bin_template_mva_score[0] = {0.1, 0.11, 0.13, 0.15, 0.17, 0.2, 0.23, 0.27, 0.32, 0.4, 0.54, 1};
        // bin_template_mva_score[1] = {0, 0.04, 0.1, 0.18, 0.26, 0.36, 0.49, 0.67, 0.95};
        bin_template_mva_score[1] = {0.1, 0.18, 0.26, 0.36, 0.49, 0.67, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
      }
      else if (channel == "El")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.09, 0.11, 0.14, 0.18, 0.23, 0.3, 0.44, 1};
        bin_template_mva_score[0] = {0.10, 0.14, 0.18, 0.23, 0.3, 0.44, 1};
        // bin_template_mva_score[1] = {0.0, 0.05, 0.17, 0.32, 0.54, 0.95};
        bin_template_mva_score[1] = {0.1, 0.17, 0.32, 0.54, 0.95};
        bin_template_mva_score[2] = {0.95, 0.97, 1};
      }
    }
    else if (era == "2017")
    {
      if (channel == "Mu")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.21, 0.23, 0.25, 0.27, 0.29, 0.32, 0.35, 0.39, 0.44, 0.5, 0.58, 0.7, 1};
        bin_template_mva_score[0] = {0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.21, 0.23, 0.25, 0.27, 0.29, 0.32, 0.35, 0.39, 0.44, 0.5, 0.58, 0.7, 1};
        // bin_template_mva_score[1] = {0.0, 0.01, 0.02, 0.04, 0.07, 0.11, 0.15, 0.19, 0.23, 0.28, 0.33, 0.39, 0.46, 0.54, 0.63, 0.73, 0.84, 0.95};
        bin_template_mva_score[1] = {0.10, 0.15, 0.19, 0.23, 0.28, 0.33, 0.39, 0.46, 0.54, 0.63, 0.73, 0.84, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 0.99, 1};
      }
      else if (channel == "El")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.14, 0.16, 0.18, 0.2, 0.22, 0.25, 0.29, 0.34, 0.4, 0.48, 0.62, 1};
        bin_template_mva_score[0] = {0.1, 0.11, 0.12, 0.14, 0.16, 0.18, 0.2, 0.22, 0.25, 0.29, 0.34, 0.4, 0.48, 0.62, 1};
        // bin_template_mva_score[1] = {0, 0.03, 0.08, 0.14, 0.21, 0.28, 0.37, 0.47, 0.61, 0.77, 0.95};
        bin_template_mva_score[1] = {0.1, 0.14, 0.21, 0.28, 0.37, 0.47, 0.61, 0.77, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
      }
    }
    else if (era == "2018")
    {
      if (channel == "Mu")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.24, 0.26, 0.28, 0.3, 0.32, 0.34, 0.37, 0.4, 0.43, 0.47, 0.52, 0.58, 0.65, 0.75, 1};
        bin_template_mva_score[0] = {0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.24, 0.26, 0.28, 0.3, 0.32, 0.34, 0.37, 0.4, 0.43, 0.47, 0.52, 0.58, 0.65, 0.75, 1};
        // bin_template_mva_score[1] = {0.0, 0.01, 0.02, 0.03, 0.05, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17, 0.19, 0.22, 0.25, 0.28, 0.31, 0.34, 0.38, 0.42, 0.46, 0.51, 0.56, 0.61, 0.67, 0.74, 0.81, 0.88, 0.95};
        bin_template_mva_score[1] = {0.10, 0.13, 0.15, 0.17, 0.19, 0.22, 0.25, 0.28, 0.31, 0.34, 0.38, 0.42, 0.46, 0.51, 0.56, 0.61, 0.67, 0.74, 0.81, 0.88, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 0.99, 1};
      }
      else if (channel == "El")
      {
        // bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.18, 0.2, 0.22, 0.24, 0.26, 0.29, 0.32, 0.36, 0.41, 0.47, 0.55, 0.67, 1};
        bin_template_mva_score[0] = {0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.18, 0.2, 0.22, 0.24, 0.26, 0.29, 0.32, 0.36, 0.41, 0.47, 0.55, 0.67, 1};
        // bin_template_mva_score[1] = {0.0, 0.01, 0.02, 0.03, 0.06, 0.09, 0.13, 0.17, 0.21, 0.25, 0.3, 0.36, 0.43, 0.51, 0.6, 0.7, 0.82, 0.95};
        bin_template_mva_score[1] = {0.10, 0.17, 0.21, 0.25, 0.3, 0.36, 0.43, 0.51, 0.6, 0.7, 0.82, 0.95};
        bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 0.99, 1};
      }
    }

    //   // QCD MC
    //   if (era == "2016preVFP")
    //   {
    //     if (channel == "Mu")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.13, 0.15, 0.17, 0.19, 0.22, 0.25, 0.29, 0.35, 0.43, 0.56, 1};
    //       bin_template_mva_score[1] = {0, 0.03, 0.08, 0.15, 0.22, 0.3, 0.4, 0.53, 0.71, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
    //     }
    //     else if (channel == "El")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.09, 0.11, 0.13, 0.16, 0.2, 0.25, 0.33, 0.47, 1};
    //       bin_template_mva_score[1] = {0, 0.1, 0.22, 0.36, 0.58, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
    //     }
    //   }
    //   else if (era == "2016postVFP")
    //   {
    //     if (channel == "Mu")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.11, 0.13, 0.15, 0.17, 0.2, 0.23, 0.27, 0.32, 0.4, 0.53, 1};
    //       bin_template_mva_score[1] = {0, 0.04, 0.1, 0.18, 0.26, 0.36, 0.49, 0.67, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
    //     }
    //     else if (channel == "El")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.09, 0.11, 0.14, 0.18, 0.23, 0.3, 0.43, 1};
    //       bin_template_mva_score[1] = {0.0, 0.04, 0.16, 0.31, 0.53, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.97, 1};
    //     }
    //   }
    //   else if (era == "2017")
    //   {
    //     if (channel == "Mu")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.2, 0.22, 0.24, 0.26, 0.28, 0.31, 0.34, 0.38, 0.43, 0.49, 0.57, 0.69, 1};
    //       bin_template_mva_score[1] = {0.0, 0.01, 0.03, 0.06, 0.1, 0.14, 0.18, 0.22, 0.27, 0.32, 0.38, 0.45, 0.53, 0.62, 0.72, 0.83, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 0.99, 1};
    //     }
    //     else if (channel == "El")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.15, 0.17, 0.19, 0.22, 0.25, 0.29, 0.34, 0.4, 0.48, 0.61, 1};
    //       bin_template_mva_score[1] = {0.0, 0.02, 0.05, 0.08, 0.14, 0.21, 0.28, 0.36, 0.46, 0.59, 0.76, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 1};
    //     }
    //   }
    //   else if (era == "2018")
    //   {
    //     if (channel == "Mu")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.21, 0.22, 0.23, 0.25, 0.27, 0.29, 0.31, 0.33, 0.36, 0.39, 0.42, 0.46, 0.51, 0.57, 0.64, 0.75, 1};
    //       bin_template_mva_score[1] = {0.0, 0.01, 0.02, 0.03, 0.05, 0.07, 0.09, 0.11, 0.13, 0.15, 0.17, 0.19, 0.22, 0.25, 0.28, 0.31, 0.34, 0.38, 0.42, 0.46, 0.51, 0.56, 0.62, 0.68, 0.75, 0.82, 0.88, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 0.99, 1};
    //     }
    //     else if (channel == "El")
    //     {
    //       bin_template_mva_score[0] = {0.0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.19, 0.21, 0.23, 0.25, 0.28, 0.31, 0.35, 0.4, 0.46, 0.54, 0.66, 1};
    //       bin_template_mva_score[1] = {0.0, 0.01, 0.02, 0.03, 0.05, 0.08, 0.12, 0.16, 0.2, 0.24, 0.29, 0.35, 0.42, 0.5, 0.59, 0.7, 0.82, 0.95};
    //       bin_template_mva_score[2] = {0.95, 0.96, 0.97, 0.98, 0.99, 1};
    //     }
    //   }

    // // Old
    // bin_template_mva_score[0] = {0, 0.02, 0.04, 0.06, 0.08, 0.1, 0.15, 0.2, 0.3, 0.5, 1};
    // bin_template_mva_score[1] = {0, 0.02, 0.06, 0.10, 0.15, 0.20, 0.3, threeb_cr_cut};
    // bin_template_mva_score[2] = {threeb_cr_cut, 0.65, 0.85, 0.90, 0.94, 0.96, 0.98, 1};
  } // if (!chk_bin_optimizer)

  if (mode == "Cal_TF")
  {
    variable_conf = {{"Lepton_Eta"}};
  }
  else
  {
    if (debug)
      variable_conf = {{"N_Vertex", 40, 0, 80}};
    else
    {
      variable_conf = {{"N_Vertex", 40, 0, 80},
                       {"Lepton_Pt", 50, 0, 400},
                       {"Lepton_Eta", 30, -3, 3},
                       {"N_Jets", 20, 0, 20},
                       {"N_BJets", 10, 0, 10},
                       {"N_CJets", 15, 0, 15},
                       {"Pt_Leading_Jet", 50, 0, 400},
                       {"Pt_Subleading_Jet", 50, 0, 400},
                       {"Eta_Leading_Jet", 50, -3, 3},
                       {"Eta_Subleading_jet", 50, -3, 3},
                       {"BvsC_Leading_Jet", 25, 0, 1},
                       {"CvsB_Leading_Jet", 25, 0, 1},
                       {"CvsL_Leading_Jet", 25, 0, 1},
                       {"BvsC_Subleading_Jet", 25, 0, 1},
                       {"CvsB_Subleading_Jet", 25, 0, 1},
                       {"CvsL_Subleading_Jet", 25, 0, 1},
                       {"Met_Pt", 50, 0, 400},
                       {"Met_Phi", 40, -4, 4},
                       {"Best_MVA_Score_Pre", 25, 0, 1},
                       {"Best_MVA_Score", 25, 0, 1},
                       {"Had_T", 50, 70, 270},
                       {"Had_W", 50, 30, 130},
                       {"Lep_T", 50, 70, 270},
                       {"Lep_W", 50, 30, 130},
                       {"BvsC_W_u", 25, 0, 1},
                       {"CvsB_W_u", 25, 0, 1},
                       {"CvsL_W_u", 25, 0, 1},
                       {"BvsC_W_d", 25, 0, 1},
                       {"CvsB_W_d", 25, 0, 1},
                       {"CvsL_W_d", 25, 0, 1}};

      if (chk_bin_optimizer)
        variable_conf.push_back({"Template_MVA_Score", 100, 0, 1});
      else
        variable_conf.push_back({"Template_MVA_Score"});

      variable_conf.push_back({"Total", 1, 0, 1});
    } // else
  } // else

  n_variable = variable_conf.size();
  cout << "Variable Size = " << n_variable << endl;

  return;
} // void Histo_Syst::Config_Variable()

//////////

void Histo_Syst::Data_Driven()
{
  cout << "[Histo_Syst::Data_Driven]: Init" << endl;

  // subtraction
  cout << "[Histo_Syst::Data_Driven]: Subtracting..." << endl;

  for (int i = 0; i < n_region; i++)
  {
    for (int j = 0; j < n_syst; j++)
    {
      // do not consider syst due to TTbb_4f
      // do not consider PDF_Error_Set_* which is so small and bulky
      if (syst_name[j].Contains("PDF_Error_Set_"))
        continue;

      for (int k = 0; k < n_variable; k++)
      {
        // data
        histo_subtracted_data_driven[i][j][k]->Add(histo_data_dd[0][i][k]);

        // subtract mc
        for (int l = 0; l < n_sample_merge_mc; l++)
        {
          if (vec_short_name_mc[l] == "QCD_bEn" || vec_short_name_mc[l] == "QCD" || vec_short_name_mc[l].Contains("TTbb") || vec_short_name_mc[l].Contains("bbDPS"))
            continue;

          histo_subtracted_data_driven[i][j][k]->Add(histo_mc_dd[0][i][j][l][k], -1);
        } // loop over n_sample
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  auto it = std::find(syst_name.begin(), syst_name.end(), "Nominal");
  int nominal_index = std::distance(syst_name.begin(), it);

  // apply TF
  cout << "[Histo_Syst::Data_Driven]: Applying TF..." << endl;

  for (int i = 0; i < n_region; i++)
  {
    // cout << region_name[i] << endl;
    for (int j = 0; j < n_syst; j++)
    {
      // do not consider syst due to TTbb_4f
      if (syst_name[j].Contains("PDF_Error_Set_"))
        continue;

      // cout << syst_name[j] << endl;
      for (int k = 0; k < n_variable; k++)
      {
        // cout << variable_conf[k].variable_title << endl;

        int n_bin;
        if (variable_conf[k].chk_equal_interval == false)
          n_bin = bin_template_mva_score[i].size() - 1;
        else
          n_bin = variable_conf[k].n_bin;

        for (int l = 0; l < n_bin; l++)
        {
          // cout << "l = " << l << endl;

          for (int m = 0; m < bin_eta_tf.size() - 1; m++)
          {
            for (int n = 0; n < bin_pt_tf.size() - 1; n++)
            {
              // cout << "m = " << m << ", n = " << n << endl;

              // mc variations are not propagated to QCD data driven
              float content = histo_subtracted_data_driven[i][nominal_index][k]->GetBinContent(l + 1, m + 1, n + 1);
              float error = histo_subtracted_data_driven[i][nominal_index][k]->GetBinError(l + 1, m + 1, n + 1);

              float tf = histo_tf_combine->GetBinContent(m + 1, n + 1);
              float tf_error = histo_tf_combine->GetBinError(m + 1, n + 1);

              if (tf < 0 || 0.2 < tf_error)
                continue;

              // if (content < 0)
              //   content = 0;

              histo_tf_corrected[i][j][k]->SetBinContent(l + 1, m + 1, n + 1, content * tf);
              histo_tf_corrected[i][j][k]->SetBinError(l + 1, m + 1, n + 1, error * tf);

            } // loop over bin_eta_tf
          } // loop over bin_pt_tf
        } // loop over n_bin
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  // TF up/down
  cout << "[Histo_Syst::Data_Driven]: TF up/down..." << endl;

  for (int i = 0; i < n_region; i++)
  {
    // cout << region_name[i] << endl;

    for (int j = 0; j < n_variable; j++)
    {
      // cout << variable_conf[j].variable_title << endl;

      int n_bin;
      if (variable_conf[j].chk_equal_interval == false)
        n_bin = bin_template_mva_score[i].size() - 1;
      else
        n_bin = variable_conf[j].n_bin;

      for (int k = 0; k < bin_eta_tf.size() - 1; k++)
      {
        for (int l = 0; l < bin_pt_tf.size() - 1; l++)
        {
          histo_tf_corrected_up[i][j][k][l] = (TH3D *)histo_tf_corrected[i][nominal_index][j]->Clone();
          histo_tf_corrected_down[i][j][k][l] = (TH3D *)histo_tf_corrected[i][nominal_index][j]->Clone();
        } // loop over bin_pt_tf
      } // loop over tf_eta_bin

      for (int k = 0; k < n_bin; k++)
      {
        // cout << "k = " << k << endl;

        for (int l = 0; l < bin_eta_tf.size() - 1; l++)
        {
          for (int m = 0; m < bin_pt_tf.size() - 1; m++)
          {
            // cout << "l = " << l << ", m = " << m << endl;

            float content = histo_subtracted_data_driven[i][nominal_index][j]->GetBinContent(k + 1, l + 1, m + 1);
            float error = histo_subtracted_data_driven[i][nominal_index][j]->GetBinError(k + 1, l + 1, m + 1);

            float tf = histo_tf_combine->GetBinContent(l + 1, m + 1);
            float tf_error = histo_tf_combine->GetBinError(l + 1, m + 1);

            // tf bin by bin up/down
            float tf_up = tf + tf_error;
            histo_tf_corrected_up[i][j][l][m]->SetBinContent(k + 1, l + 1, m + 1, content * tf_up);
            histo_tf_corrected_up[i][j][l][m]->SetBinError(k + 1, l + 1, m + 1, error * tf_up);

            float tf_down = tf - tf_error;
            histo_tf_corrected_down[i][j][l][m]->SetBinContent(k + 1, l + 1, m + 1, content * tf_down);
            histo_tf_corrected_down[i][j][l][m]->SetBinError(k + 1, l + 1, m + 1, error * tf_down);
          } // loop over bin_pt_tf
        } // loop over tf_eta_bin
      } // loop over n_bin
    } // loop over n_variable
  } // loop over n_region

  /* Envelop */
  cout << "[Histo_Syst::Data_Driven]: Taking Syst Envelop..." << endl;
  for (int i = 0; i < n_region; i++)
  {
    // cout << region_name[i] << endl;

    // do not consider syst due to TTbb_4f
    for (int j = 0; j < n_syst; j++)
    {
      // cout << syst_name[j] << endl;

      // It's confusing to include "PDF_Error_Set" in the envelope
      // Howeever, pratically, it is not a problem since PDF_Error_Set is way too small
      if (syst_name[j].Contains("PDF_Error_Set"))
        continue;

      for (int k = 0; k < n_variable; k++)
      {
        int n_bin;
        if (variable_conf[k].chk_equal_interval == false)
          n_bin = bin_template_mva_score[i].size() - 1;
        else
          n_bin = variable_conf[k].n_bin;

        for (int l = 0; l < n_bin; l++)
        {
          if (j == nominal_index)
          {
            float error = histo_tf_corrected[i][nominal_index][k]->ProjectionX()->GetBinError(l + 1);

            histo_envelop_up[i][k]->SetBinError(l + 1, error);
            histo_envelop_down[i][k]->SetBinError(l + 1, error);
          }

          float content = histo_tf_corrected[i][j][k]->ProjectionX()->GetBinContent(l + 1);
          float content_up = histo_envelop_up[i][k]->GetBinContent(l + 1);
          float content_down = histo_envelop_down[i][k]->GetBinContent(l + 1);

          // if (i == 0 && variable_conf[k].variable_title == "Template_MVA_Score" && l == 0)
          //   cout << syst_name[j] << " " << content << " " << content_up << " " << content_down << endl;

          if (content_up < content)
          {
            // if (i == 0 && variable_conf[k].variable_title == "Template_MVA_Score" && l == 0)
            //   cout << "Updating Up..." << syst_name[j] << " " << content << " " << content_up << endl;
            histo_envelop_up[i][k]->SetBinContent(l + 1, content);
          }

          if (content < content_down)
          {
            // if (i == 0 && variable_conf[k].variable_title == "Template_MVA_Score" && l == 0)
            //   cout << "Updating Down..." << syst_name[j] << " " << content << " " << content_down << endl;
            histo_envelop_down[i][k]->SetBinContent(l + 1, content);
          }
        } // loop over n_bin
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  cout << "[Histo_Syst::Data_Driven]: Done" << endl;

  return;
} // void Histo_Syst::Data_Driven()

//////////

void Histo_Syst::Draw_TF()
{
  cout << "[Histo_Syst::Draw_TF]: Init" << endl;

  gROOT->SetBatch(kTRUE);
  gStyle->SetPaintTextFormat(".2f");
  gStyle->SetOptStat(0);

  TCanvas **canvas = new TCanvas *[n_region];
  TLegend **tl = new TLegend *[n_region];
  for (int i = 0; i < n_region; i++)
  {
    canvas[i] = new TCanvas("", "", 800, 500);
    canvas[i]->Divide(2, 1);
    canvas[i]->Draw();

    double max = histo_tf[i]->GetMaximum();
    max = max > histo_tf_qcd_mc[i]->GetMaximum() ? max : histo_tf_qcd_mc[i]->GetMaximum();
    max *= 1.1;

    double min = histo_tf[i]->GetMinimum();
    min = min < histo_tf_qcd_mc[i]->GetMinimum() ? min : histo_tf_qcd_mc[i]->GetMinimum();
    min *= 0.9;

    canvas[i]->cd(1);
    canvas[i]->cd(1)->SetLogy();
    histo_tf[i]->SetTitle("TF Data Driven");
    histo_tf[i]->GetXaxis()->SetTitle("Lepton #eta");
    histo_tf[i]->GetYaxis()->SetTitle("Lepton P_{T}");
    histo_tf[i]->Draw("colz texte");

    canvas[i]->cd(2);
    canvas[i]->cd(2)->SetLogy();
    TString qcd_mc_name;
    if (chk_qcd_ben)
      qcd_mc_name = "TF QCD bEn MC Sample";
    else
      qcd_mc_name = "TF QCD MC Sample";
    histo_tf_qcd_mc[i]->SetTitle(qcd_mc_name);
    histo_tf_qcd_mc[i]->GetXaxis()->SetTitle("Lepton #eta");
    histo_tf_qcd_mc[i]->GetYaxis()->SetTitle("Lepton P_{T}");
    histo_tf_qcd_mc[i]->Draw("colz texte");

    // tl[i] = new TLegend(0.7, 0.7, 0.9, 0.9);
    // tl[i]->AddEntry(histo_tf[i], "TF", "lep");

    // TString qcd_mc_name;
    // if (chk_qcd_ben)
    //   qcd_mc_name = "TF QCD bEn MC Sample";
    // else
    //   qcd_mc_name = "TF QCD MC Sample";
    // tl[i]->AddEntry(histo_tf_qcd_mc[i], qcd_mc_name, "lep");
    // tl[i]->Draw("SAME");

    TString canvas_name = "Transfer_Function_" + region_name[i] + "_" + era + "_" + channel + ".png";
    canvas[i]->Print(canvas_name, "png");
  } // TCanvas *canvas = new TCanvas *[n_region];

  cout << "[Histo_Syst::Draw_TF]: Done" << endl;

  return;
} // void Histo_Syst::Draw_TF()

//////////

void Histo_Syst::Fill_Histo_Data()
{
  int abcd_region_index = Set_ABCD_Region();
  if (abcd_region_index < 0)
    return;

  int region_index = Set_Region();

  // to save time
  if (mode == "2D" && (abcd_region_name[abcd_region_index] == "A" || abcd_region_name[abcd_region_index] == "B"))
    return;

  // this part is not satisfactory, but don't waste time
  if (mode == "Cal_TF")
  {
    dynamic_cast<TH2D *>(histo_data[abcd_region_index][region_index][0])->Fill(event.lepton_eta, event.lepton_pt, 1.);
  }
  else
  {
    if (debug)
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][0])->Fill(event.n_pv, event.lepton_eta, event.lepton_pt, 1.);
    else
    {
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][0])->Fill(event.n_pv, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][1])->Fill(event.lepton_pt, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][2])->Fill(event.lepton_eta, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][3])->Fill(event.n_jets, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][4])->Fill(event.n_bjets, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][5])->Fill(event.n_cjets, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][6])->Fill(event.pt_leading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][7])->Fill(event.pt_subleading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][8])->Fill(event.eta_leading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][9])->Fill(event.eta_subleading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][10])->Fill(event.bvsc_leading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][11])->Fill(event.cvsb_leading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][12])->Fill(event.cvsl_leading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][13])->Fill(event.bvsc_subleading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][14])->Fill(event.cvsb_subleading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][15])->Fill(event.cvsl_subleading_jet, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][16])->Fill(event.met_pt, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][17])->Fill(event.met_phi, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][18])->Fill(event.best_mva_score_pre, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][19])->Fill(event.best_mva_score, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][20])->Fill(event.m_had_t, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][21])->Fill(event.m_had_w, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][22])->Fill(event.m_lep_t, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][23])->Fill(event.m_lep_w, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][24])->Fill(event.bvsc_w_u, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][25])->Fill(event.cvsb_w_u, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][26])->Fill(event.cvsl_w_u, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][27])->Fill(event.bvsc_w_d, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][28])->Fill(event.cvsb_w_d, event.lepton_eta, event.lepton_pt, 1.);
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][29])->Fill(event.cvsl_w_d, event.lepton_eta, event.lepton_pt, 1.);

      // for blind
      if (abcd_region_name[abcd_region_index] == "D")
      {
        if (region_name[region_index] != "Signal")
          dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][30])->Fill(event.template_score, event.lepton_eta, event.lepton_pt, 1);
      }
      else
        dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][30])->Fill(event.template_score, event.lepton_eta, event.lepton_pt, 1);

      // for C-tag SF debug
      dynamic_cast<TH3D *>(histo_data[abcd_region_index - 2][region_index][31])->Fill(0.5, event.lepton_eta, event.lepton_pt, 1.);

    } // else
  } // else

  return;
} // void Histo_Syst::Fill_Histo_Data()

//////////

void Histo_Syst::Fill_Histo_MC(const TString &sample_name, const TString &sample_name_short, const TString &tree_type)
{
  // int sample_index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();
  int abcd_region_index = Set_ABCD_Region();
  int region_index = Set_Region();

  // to save time
  if (mode == "2D" && (abcd_region_name[abcd_region_index] == "A" || abcd_region_name[abcd_region_index] == "B"))
    return;

  bool chk_discarded = false;
  int histo_index = Histo_Index(sample_name_short, chk_discarded);
  if (chk_discarded)
    return;

  TString histo_name_rf = Histo_Name_RF(sample_name);

  float weight_ckm = Reweight_CKM(sample_name);
  float weight_tthf = Reweight_TTHF(sample_name_short);

  // cout << "test " << sample_name << " " << sample_index << " " << histo_name_rf << endl;

  for (int i = 0; i < n_syst; i++)
  {
    TString syst_type = syst_name[i];

    // for central tree, don't consider the following syst
    if (tree_type == "Central")
    {
      if (syst_type == "Jet_Res_Down" || syst_type == "Jet_Res_Up" ||
          syst_type == "El_En_Down" || syst_type == "El_En_Up" ||
          syst_type == "El_Res_Down" || syst_type == "El_Res_Up" ||
          syst_type == "UE_Down" || syst_type == "UE_Up" ||
          syst_type == "CP5_Down" || syst_type == "CP5_Up" ||
          syst_type == "hdamp_Down" || syst_type == "hdamp_Up" ||
          syst_type == "mtop_171p5" || syst_type == "mtop_173p5" ||
          syst_type == "TTbb_4f" ||
          syst_type.Contains("Jet_En"))
        continue;
    }

    // if syst_fix is fixed, consider only the corresponding tree
    else
    {
      TString temp = syst_type;
      if (tree_type != temp.ReplaceAll("_", ""))
        continue;
    }

    event.weight = 1;
    event.weight *= event.weight_baseline;
    event.weight *= event.weight_lumi;
    event.weight *= event.weight_mc;
    event.weight *= event.weight_hem_veto;

    // ckm reweight
    event.weight *= weight_ckm;

    // tt+hf reweight
    event.weight *= weight_tthf;

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
      {
        event.weight *= event.weight_scale_variation_2;
      }
      else if (syst_type.Contains("Up"))
      {
        event.weight *= event.weight_scale_variation_1;
      }
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
      // JES Breakdown
      else if (syst_type.Contains("Jet_En_"))
      {
        cerr << "Will be written later" << endl;
        exit(1);
      }
      else
      {
        event.weight *= event.weight_b_tag;
        b_tagging_rf_type = "B_Tag_Nominal";
      }

      // b-tag renormalization factor
      // cout << "test " << histo_name_rf << " " << b_tagging_rf_type << endl;
      event.weight *= tagging_rf.Get_Tagging_RF_B_Tag(abcd_region_name[abcd_region_index], histo_name_rf, b_tagging_rf_type, event.vec_jet_pt, event.vec_jet_eta, event.vec_jet_flavor);
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

      // c-tag renormalization factor
      // c_tagging_rf_type = "C_Tag_Nominal";
      event.weight *= tagging_rf.Get_Tagging_RF_C_Tag(abcd_region_name[abcd_region_index], histo_name_rf, c_tagging_rf_type, event.vec_jet_pt, event.vec_jet_eta, event.vec_jet_flavor);
    } // else if (tagger == "C")

    // this part is not satisfactory, but don't waste time
    if (mode == "Cal_TF")
    {
      dynamic_cast<TH2D *>(histo_mc[abcd_region_index][region_index][i][histo_index][0])->Fill(event.lepton_eta, event.lepton_pt, event.weight);
    }
    else
    {
      if (debug)
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][0])->Fill(event.n_pv, event.lepton_eta, event.lepton_pt, event.weight);
      else
      {
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][0])->Fill(event.n_pv, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][1])->Fill(event.lepton_pt, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][2])->Fill(event.lepton_eta, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][3])->Fill(event.n_jets, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][4])->Fill(event.n_bjets, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][5])->Fill(event.n_cjets, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][6])->Fill(event.pt_leading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][7])->Fill(event.pt_subleading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][8])->Fill(event.eta_leading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][9])->Fill(event.eta_subleading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][10])->Fill(event.bvsc_leading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][11])->Fill(event.cvsb_leading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][12])->Fill(event.cvsl_leading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][13])->Fill(event.bvsc_subleading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][14])->Fill(event.cvsb_subleading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][15])->Fill(event.cvsl_subleading_jet, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][16])->Fill(event.met_pt, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][17])->Fill(event.met_phi, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][18])->Fill(event.best_mva_score_pre, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][19])->Fill(event.best_mva_score, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][20])->Fill(event.m_had_t, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][21])->Fill(event.m_had_w, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][22])->Fill(event.m_lep_t, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][23])->Fill(event.m_lep_w, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][24])->Fill(event.bvsc_w_u, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][25])->Fill(event.cvsb_w_u, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][26])->Fill(event.cvsl_w_u, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][27])->Fill(event.bvsc_w_d, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][28])->Fill(event.cvsb_w_d, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][29])->Fill(event.cvsl_w_d, event.lepton_eta, event.lepton_pt, event.weight);
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][30])->Fill(event.template_score, event.lepton_eta, event.lepton_pt, event.weight);

        // for C-tag SF debug
        dynamic_cast<TH3D *>(histo_mc[abcd_region_index - 2][region_index][i][histo_index][31])->Fill(0.5, event.lepton_eta, event.lepton_pt, event.weight);
      } // else
    } // else
  } // loop over n_syst

  return;
} // void Histo_Syst::Fill_Histo_MC(const int& sample_index)

//////////

int Histo_Syst::Histo_Index(const TString &sample_name, bool &chk_discarded)
{
  int index = -999;

  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")))
  {
    TString histo_name = sample_name;

    if (histo_name.Contains("WtoCB"))
      histo_name.ReplaceAll("TTLJ_WtoCB", "TTLJ");

    bool chk_b = false;
    bool chk_c = false;

    // // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    // for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    // {
    //   int flavour = event.vec_sel_gen_hf_flavour->at(i);
    //   int origin = event.vec_sel_gen_hf_origin->at(i);

    //   // if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
    //   if (flavour == 5 && abs(origin) == 21)
    //   {
    //     chk_b = true;
    //     break;
    //   }
    //   // else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
    //   else if (flavour == 4 && abs(origin) == 21)
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

    if (chk_use_ttbb_4f)
    {
      // if (sample_name.Contains("TTLJ") || sample_name.Contains("TTLL"))
      //   cout << "test 0 Histo_Index: " << sample_name << " " << histo_name << endl;

      if (histo_name.Contains("_TTbb_4f_BB"))
        histo_name.ReplaceAll("_TTbb_4f_BB", "_BB");

      else if (histo_name.Contains("_bbDPS_BB"))
      {
        // histo_name.ReplaceAll("_bbDPS_BB", "_bbDPS_BB");
      }

      else if (histo_name.Contains("TTLJ_BB"))
      {
        histo_name.ReplaceAll("TTLJ_BB", "Discard");
        chk_discarded = true;
      }

      else if (histo_name.Contains("TTLL_BB"))
      {
        histo_name.ReplaceAll("TTLL_BB", "Discard");
        chk_discarded = true;
      }

      // reject other 4f events
      else if (histo_name.Contains("TTbb") || histo_name.Contains("bbDPS"))
        chk_discarded = true;

      // if (sample_name.Contains("TTLJ") || sample_name.Contains("TTLL"))
      //   cout << "test 1 Histo_Index: " << sample_name << " " << histo_name << endl;
    }
    else
    {
      if (histo_name.Contains("TTbb") || histo_name.Contains("bbDPS"))
        chk_discarded = true;
    }

    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();

    // if (sample_name.Contains("TTLJ") || sample_name.Contains("TTLL"))
    //   cout << "test 2 Histo_Index: " << sample_name << " " << histo_name << " " << index << endl;
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

      //   // if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      //   if (flavour == 5 && abs(origin) == 21)
      //   {
      //     chk_b = true;
      //     break;
      //   }
      //   // else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
      //   else if (flavour == 4 && abs(origin) == 21)
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
    } // if (chk_rf_tthf_breakdown)

    if (event.decay_mode == 21 || event.decay_mode == 23)
      histo_name_rf += "_2";
    else if (event.decay_mode == 41 || event.decay_mode == 43)
      histo_name_rf += "_4";
    else if (event.decay_mode == 45)
      histo_name_rf += "_45";
  }
  else
    histo_name_rf = samples.map_short_name_mc[sample_name];

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

            if (variable_conf[m].chk_equal_interval == false)
              histo_mc[i][j][k][l][m] = new TH3D(histo_name, variable_conf[m].variable_title, bin_template_mva_score[j].size() - 1, bin_template_mva_score[j].data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
            else
              histo_mc[i][j][k][l][m] = new TH3D(histo_name, variable_conf[m].variable_title, variable_conf[m].vec_bin.size() - 1, variable_conf[m].vec_bin.data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
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

        if (variable_conf[k].chk_equal_interval == false)
          histo_data[i][j][k] = new TH3D(histo_name, variable_conf[k].variable_title, bin_template_mva_score[j].size() - 1, bin_template_mva_score[j].data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
        else
          histo_data[i][j][k] = new TH3D(histo_name, variable_conf[k].variable_title, variable_conf[k].vec_bin.size() - 1, variable_conf[k].vec_bin.data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
      } // loop over n_variable
    } // loop over n_region
  } // loop over n_abcd_region

  return;
} // void Histo_Syst::Init_Histo()

//////////

void Histo_Syst::Init_Histo_Data_Driven()
{
  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Init" << endl;

  TString fin_2d_name = path_base + "/Workplace/Histo_Syst/Vcb_2D_" + era + "_" + channel + "_" + tagger + "_tagger_All.root";
  fin_2d = new TFile(fin_2d_name);

  // for mc
  cout << "[Histo_Syst::Init_Histo_Data_Driven]: MC" << endl;

  histo_mc_dd = new TH3D *****[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_mc_dd[i] = new TH3D ****[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_mc_dd[i][j] = new TH3D ***[n_syst];
      for (int k = 0; k < n_syst; k++)
      {
        histo_mc_dd[i][j][k] = new TH3D **[n_sample_merge_mc];
        for (int l = 0; l < n_sample_merge_mc; l++)
        {
          histo_mc_dd[i][j][k][l] = new TH3D *[n_variable];
          for (int m = 0; m < n_variable; m++)
          {
            TString histo_name = abcd_region_name[i + 2] + "/" + region_name[j] + "/" + syst_name[k] + "/" + vec_short_name_mc[l] + "/" + variable_conf[m].variable_title;
            histo_mc_dd[i][j][k][l][m] = (TH3D *)fin_2d->Get(histo_name);

            // cout << histo_name << " " << histo_mc_dd[i][j][k][l][m] << endl;

          } // loop over n_variable
        } // loop over n_sample_merge_mc
      } // loop over n_syst
    } // loop over n_region
  } // loop over n_abcd_region -2

  // for data
  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Data" << endl;

  histo_data_dd = new TH3D ***[n_abcd_region - 2];
  for (int i = 0; i < n_abcd_region - 2; i++)
  {
    histo_data_dd[i] = new TH3D **[n_region];
    for (int j = 0; j < n_region; j++)
    {
      histo_data_dd[i][j] = new TH3D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name = abcd_region_name[i + 2] + "/" + region_name[j] + "/Data/" + variable_conf[k].variable_title;
        histo_data_dd[i][j][k] = (TH3D *)fin_2d->Get(histo_name);

        // cout << histo_name << " " << histo_data_dd[i][j][k] << endl;
      } // loop over n_variable
    } // loop over n_region
  } // loop over n_abcd_region

  // for histo_subtracted_data_driven
  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Subtracted" << endl;

  histo_subtracted_data_driven = new TH3D ***[n_region];
  histo_tf_corrected = new TH3D ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_subtracted_data_driven[i] = new TH3D **[n_syst];
    histo_tf_corrected[i] = new TH3D **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      // do not consider syst due to TTbb_4f
      // do not consider PDF_Error_Set_* which is so small and bulky
      if (syst_name[j].Contains("PDF_Error_Set_"))
        continue;

      histo_subtracted_data_driven[i][j] = new TH3D *[n_variable];
      histo_tf_corrected[i][j] = new TH3D *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString histo_name_subtracted = "Subtracted_Data_Driven_" + region_name[i] + "_" + syst_name[j] + "_" + variable_conf[k].variable_title;
        TString histo_name_tf_corr = "TF_Corrected_" + region_name[i] + "_" + syst_name[j] + "_" + variable_conf[k].variable_title;

        // only for template_mva_score
        if (variable_conf[k].chk_equal_interval == false)
        {
          histo_subtracted_data_driven[i][j][k] = new TH3D(histo_name_subtracted, histo_name_subtracted, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
          histo_tf_corrected[i][j][k] = new TH3D(histo_name_tf_corr, histo_name_tf_corr, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
        }
        // others
        else
        {
          histo_subtracted_data_driven[i][j][k] = new TH3D(histo_name_subtracted, histo_name_subtracted, variable_conf[k].vec_bin.size() - 1, variable_conf[k].vec_bin.data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
          histo_tf_corrected[i][j][k] = new TH3D(histo_name_tf_corr, histo_name_tf_corr, variable_conf[k].vec_bin.size() - 1, variable_conf[k].vec_bin.data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
        }
      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  cout << "[Histo_Syst::Init_Histo_Data_Driven]: TF up/down" << endl;

  histo_tf_corrected_up = new TH3D ****[n_region];
  histo_tf_corrected_down = new TH3D ****[n_region];

  for (int i = 0; i < n_region; i++)
  {
    histo_tf_corrected_up[i] = new TH3D ***[n_variable];
    histo_tf_corrected_down[i] = new TH3D ***[n_variable];

    for (int j = 0; j < n_variable; j++)
    {
      histo_tf_corrected_up[i][j] = new TH3D **[bin_eta_tf.size() - 1];
      histo_tf_corrected_down[i][j] = new TH3D **[bin_eta_tf.size() - 1];

      for (int k = 0; k < bin_eta_tf.size() - 1; k++)
      {
        histo_tf_corrected_up[i][j][k] = new TH3D *[bin_pt_tf.size() - 1];
        histo_tf_corrected_down[i][j][k] = new TH3D *[bin_pt_tf.size() - 1];

        for (int l = 0; l < bin_pt_tf.size() - 1; l++)
        {
          TString histo_name_up = "TF_Corrected_Up_" + region_name[i] + "_" + variable_conf[j].variable_title + "_Eta_Bin" + to_string(k) + "_Pt_Bin" + to_string(l);
          TString histo_name_down = "TF_Corrected_Down_" + region_name[i] + "_" + variable_conf[j].variable_title + "_Eta_Bin" + to_string(k) + "_Pt_Bin" + to_string(l);

          if (variable_conf[j].chk_equal_interval == false)
          {
            histo_tf_corrected_up[i][j][k][l] = new TH3D(histo_name_up, histo_name_up, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
            histo_tf_corrected_down[i][j][k][l] = new TH3D(histo_name_down, histo_name_down, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
          }
          else
          {
            histo_tf_corrected_up[i][j][k][l] = new TH3D(histo_name_up, histo_name_up, variable_conf[j].vec_bin.size() - 1, variable_conf[j].vec_bin.data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
            histo_tf_corrected_down[i][j][k][l] = new TH3D(histo_name_down, histo_name_down, variable_conf[j].vec_bin.size() - 1, variable_conf[j].vec_bin.data(), bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
          }
        } // loop over n_bin_pt_tf
      } // loop over n_bin_eta_tf
    } // loop over n_variable
  } // loop over n_region

  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Syst Envelop up/down" << endl;
  histo_envelop_up = new TH1D **[n_region];
  histo_envelop_down = new TH1D **[n_region];

  for (int i = 0; i < n_region; i++)
  {
    histo_envelop_up[i] = new TH1D *[n_variable];
    histo_envelop_down[i] = new TH1D *[n_variable];

    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name_envelop_up = "TF_Corrected_Envelop_Up_" + region_name[i] + "_" + variable_conf[j].variable_title;
      TString histo_name_envelop_down = "TF_Corrected_Envelop_Down_" + region_name[i] + "_" + variable_conf[j].variable_title;

      if (variable_conf[j].chk_equal_interval == false)
      {
        histo_envelop_up[i][j] = new TH1D(histo_name_envelop_up, histo_name_envelop_up, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data());
        histo_envelop_down[i][j] = new TH1D(histo_name_envelop_down, histo_name_envelop_down, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data());
      }
      else
      {
        histo_envelop_up[i][j] = new TH1D(histo_name_envelop_up, histo_name_envelop_up, variable_conf[j].vec_bin.size() - 1, variable_conf[j].vec_bin.data());
        histo_envelop_down[i][j] = new TH1D(histo_name_envelop_down, histo_name_envelop_down, variable_conf[j].vec_bin.size() - 1, variable_conf[j].vec_bin.data());
      }

      // initialization of histo_envelop_up and histo_envelop_down
      for (int k = 0; k < variable_conf[j].vec_bin.size() - 1; k++)
      {
        histo_envelop_up[i][j]->SetBinContent(k + 1, -9999999999);
        histo_envelop_down[i][j]->SetBinContent(k + 1, 9999999999);
      } // loop over n_bin
    } // loop over n_variable
  } // loop over n_region

  cout << "[Histo_Syst::Init_Histo_Data_Driven]: Reading TF..." << endl;
  TString fin_tf_name = path_base + "/Workplace/Histo_Syst/Vcb_TF_" + era + "_" + channel + "_" + tagger + "_tagger.root";
  fin_tf = new TFile(fin_tf_name);

  histo_tf = new TH2D *[n_region];
  for (int i = 0; i < n_region; i++)
  {
    // let's use TF from 2b region for all
    TString histo_name = region_name[0] + "/TF";
    histo_tf[i] = (TH2D *)fin_tf->Get(histo_name);

    // histo_tf[i]->SetBit(TH1::kIsAverage);

    // // combine transfer function
    // if (i == 0)
    //   histo_tf_combine = (TH1D *)histo_tf[0]->Clone();
    // else
    //   histo_tf_combine->Add(histo_tf[i]);

    // // cout << histo_name << " " << histo_tf[i] << endl;

    // let's use TF from 2b region for all
    if (i == 0)
      histo_tf_combine = (TH2D *)histo_tf[i]->Clone();
  }

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

            histo_mc[i][j][k][l][m] = new TH2D(histo_name, variable_conf[m].variable_title, bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
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
        histo_data[i][j][k] = new TH2D(histo_name, variable_conf[k].variable_title, bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
      } // loop over n_variable
    } // loop over n_region
  } // loop over n_abcd_region

  histo_subtracted_tf = new TH1 **[n_abcd_region];
  for (int i = 0; i < n_abcd_region; i++)
  {
    histo_subtracted_tf[i] = new TH1 *[n_region];
    for (int j = 0; j < n_region; j++)
    {
      TString histo_name = "Subtrated_" + abcd_region_name[i] + "_" + region_name[j];

      histo_subtracted_tf[i][j] = new TH2D(histo_name, histo_name, bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
    } // loop over n_region
  } // loop over n_abcd_region

  histo_tf = new TH2D *[n_region];
  histo_tf_qcd_mc = new TH2D *[n_region];
  for (int i = 0; i < n_region; i++)
  {
    TString histo_name = "TF_" + region_name[i];
    histo_tf[i] = new TH2D(histo_name, histo_name, bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());

    histo_name = "TF_QCD_MC_" + region_name[i];
    histo_tf_qcd_mc[i] = new TH2D(histo_name, histo_name, bin_eta_tf.size() - 1, bin_eta_tf.data(), bin_pt_tf.size() - 1, bin_pt_tf.data());
  } // loop over n_region

  return;
} // void Histo_Syst::Init_Histo_TF()

//////////

void Histo_Syst::Init_Merge_PDF_Error_Set()
{
  cout << "[Histo_Syst::Init_Merge_PDF_Error_Set]: Init" << endl;

  TString fout_name = path_base + "/Workplace/Histo_Syst/Vcb_Histos_" + era + "_" + channel + "_" + tagger + "_tagger.root";
  fout = new TFile(fout_name, "UPDATE");

  histo_mc_merge = new TH1D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc_merge[i] = new TH1D ***[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      if (!syst_name[j].Contains("PDF_Error_Set_"))
        continue; // do not consider syst due to PDF_Error_Set

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
        TString histo_name_pdf_error_down = "PDF_Error_Set_Down_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;
        TString histo_name_pdf_error_up = "PDF_Error_Set_Up_" + region_name[i] + "_" + vec_short_name_mc[j] + "_" + variable_conf[k].variable_title;

        if (variable_conf[k].chk_equal_interval == false)
        {
          histo_mc_pdf_error_set_down[i][j][k] = new TH1D(histo_name_pdf_error_down, variable_conf[k].variable_title, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data());
          histo_mc_pdf_error_set_up[i][j][k] = new TH1D(histo_name_pdf_error_up, variable_conf[k].variable_title, bin_template_mva_score[i].size() - 1, bin_template_mva_score[i].data());
        }
        else
        {
          histo_mc_pdf_error_set_down[i][j][k] = new TH1D(histo_name_pdf_error_down, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);
          histo_mc_pdf_error_set_up[i][j][k] = new TH1D(histo_name_pdf_error_up, variable_conf[k].variable_title, variable_conf[k].n_bin, variable_conf[k].x_low, variable_conf[k].x_up);
        }
      } // loop over n_variable
    } // loop over n_sample_merge_mc
  } // loop over n_region

  // find min and max and fill histo
  for (int i = 0; i < n_region; i++)
  {
    // cout << region_name[i] << endl;
    for (int j = 0; j < n_sample_merge_mc; j++)
    {
      // cout << vec_short_name_mc[j] << endl;
      for (int k = 0; k < n_variable; k++)
      {
        int n_bin;
        if (variable_conf[k].chk_equal_interval == false)
          n_bin = bin_template_mva_score[i].size() - 1;
        else
          n_bin = variable_conf[k].n_bin;

        for (int l = 0; l < n_bin; l++)
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
    TString tree_type = it_map_map->first;
    cout << "Tree_Type = " << tree_type << endl;

    TString syst_fix;
    if (tree_type == "Central")
      syst_fix = "None";
    else
      syst_fix = it_map_map->first;

    map<TString, TTree *> *map_tree = it_map_map->second;
    cout << "N_Sample in map_tree = " << map_tree->size() << endl;
    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
      TString sample_name = it->first;
      cout << "Sample_Name = " << sample_name << endl;

      TString sample_name_short;
      if (it->first.Contains("TTLJ") || it->first.Contains("TTLL"))
        sample_name_short = it->first;
      else
        sample_name_short = samples.map_short_name_mc[it->first];

      cout << sample_name << " " << sample_name_short << endl;

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      // split
      Long64_t init;
      Long64_t end;

      Long64_t step = n_entries / n_split;
      init = step * index_split;
      end = step * (index_split + 1);
      if (index_split + 1 == n_split)
        end = n_entries;

      cout << "N_Entries:" << n_entries << " N_Split:" << n_split << " Index_Split:" << index_split << " Step:" << step << " Init:" << init << " End:" << end << endl;

      float modelling_patch_baseline = modelling_patch.Get_Modelling_Patch(sample_name_short, "Baseline");
      float modelling_patch_top_pt = modelling_patch.Get_Modelling_Patch(sample_name_short, "Top_Pt_Reweight");
      float modelling_patch_scale_variation_1 = modelling_patch.Get_Modelling_Patch(sample_name_short, "Scale_Variation_1");
      float modelling_patch_scale_variation_2 = modelling_patch.Get_Modelling_Patch(sample_name_short, "Scale_Variation_2");
      float modelling_patch_scale_variation_3 = modelling_patch.Get_Modelling_Patch(sample_name_short, "Scale_Variation_3");
      float modelling_patch_scale_variation_4 = modelling_patch.Get_Modelling_Patch(sample_name_short, "Scale_Variation_4");
      float modelling_patch_scale_variation_6 = modelling_patch.Get_Modelling_Patch(sample_name_short, "Scale_Variation_6");
      float modelling_patch_scale_variation_8 = modelling_patch.Get_Modelling_Patch(sample_name_short, "Scale_Variation_8");
      float modelling_patch_ps_0 = modelling_patch.Get_Modelling_Patch(sample_name_short, "PS_0");
      float modelling_patch_ps_1 = modelling_patch.Get_Modelling_Patch(sample_name_short, "PS_1");
      float modelling_patch_ps_2 = modelling_patch.Get_Modelling_Patch(sample_name_short, "PS_2");
      float modelling_patch_ps_3 = modelling_patch.Get_Modelling_Patch(sample_name_short, "PS_3");

      // cout << "test " << sample_name_short << " " << modelling_patch_baseline << endl;

      for (Long64_t i = init; i < end; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        // // cut on best_mva_score_pre
        // if (chk_cut_best_mva_score_pre && event.best_mva_score_pre < cut_best_mva_score_pre)
        //   continue;

        // // cut on template_score
        if (chk_cut_template_score && event.template_score < cut_template_score)
          continue;

        // event veto
        if (100 < TMath::Abs(event.weight_pdf_alternative))
        {
          cout << "strange weight_pdf_alternative is detected. " << tree_type << ", " << i << ", weight_pdf_alternative = " << event.weight_pdf_alternative << endl;

          continue;
        }

        if (TMath::IsNaN(event.weight_scale_variation_1) || !TMath::Finite(event.weight_scale_variation_1) || TMath::IsNaN(event.weight_scale_variation_2) || !TMath::Finite(event.weight_scale_variation_2))
        {
          cout << "strange weight_scale_variation is detected. " << tree_type << ", " << i << ", weight_scale_variation_1 = " << event.weight_scale_variation_1 << ", weight_scale_variation_2 = " << event.weight_scale_variation_2 << endl;

          continue;
        }

        // if (event.n_jets == 3)
        //   continue;

        // if (event.met_pt < 20)
        //   continue;

        // event.Swap();

        if (!chk_bin_optimizer && mode == "2D" && event.template_score < 0.1)
          continue;

        event.weight_baseline = modelling_patch_baseline;
        event.weight_top_pt *= modelling_patch_top_pt;
        event.weight_scale_variation_1 *= modelling_patch_scale_variation_1;
        event.weight_scale_variation_2 *= modelling_patch_scale_variation_2;
        event.weight_scale_variation_3 *= modelling_patch_scale_variation_3;
        event.weight_scale_variation_4 *= modelling_patch_scale_variation_4;
        event.weight_scale_variation_6 *= modelling_patch_scale_variation_6;
        event.weight_scale_variation_8 *= modelling_patch_scale_variation_8;
        event.weight_ps[0] *= modelling_patch_ps_0;
        event.weight_ps[1] *= modelling_patch_ps_1;
        event.weight_ps[2] *= modelling_patch_ps_2;
        event.weight_ps[3] *= modelling_patch_ps_3;

        Fill_Histo_MC(sample_name, sample_name_short, tree_type);
        // if (syst_fix == "None") Fill_Histo_Weight(region_index, sample_index);
      } // loop over entries
    } // loop over map_tree_mc
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
      // if (chk_cut_template_score && event.template_score < cut_template_score)
      //   continue;

      // if (event.n_jets == 3)
      //   continue;

      // if (event.met_pt < 20)
      //   continue;

      // event.Swap();

      if (!chk_bin_optimizer && mode == "2D" && event.template_score < 0.1)
        continue;

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
  } // if (index_tree_type != -1)
  else
  {
    cout << "Iterate all tree_type" << endl;

    index_split = 0;
    n_split = 1;
  } // else

  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    map<TString, TTree *> map_tree_mc;
    vec_map_tree_mc.push_back(map_tree_mc);
  }

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");
    TString result_path = path_sample_base + era + "/Vcb/";

    map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);

    for (unsigned int i = 0; i < vec_tree_type.size(); i++)
    {
      TString tree_type = vec_tree_type[i];

      if (tree_type == "Data")
        continue;

      if (tree_type.Contains("CP5") || tree_type.Contains("hdamp") || tree_type.Contains("mtop"))
        continue;

      cout << tree_type << endl;

      vec_map_tree_mc[i][it->first] = (TTree *)map_fin_mc[it->first]->Get(channel + "/" + tree_type + "/Result_Tree");

      bool chk_all = false;
      if (tree_type == "Central")
        chk_all = true;

      event.Setup_Tree(vec_map_tree_mc[i][it->first], StringToSyst(tree_type), chk_all);
    } //  for (unsigned int i = 0; i < vec_tree_type.size(); i++)

    // For TT Syst
    // For CP5, hdamp, and mtop samples, central trees are required for non-TT samples
    // For TTbb4f, other samples are not required
    if (find(vec_tree_type.begin(), vec_tree_type.end(), "CP5Down") != vec_tree_type.end() ||
        find(vec_tree_type.begin(), vec_tree_type.end(), "CP5Up") != vec_tree_type.end() ||
        find(vec_tree_type.begin(), vec_tree_type.end(), "hdampDown") != vec_tree_type.end() ||
        find(vec_tree_type.begin(), vec_tree_type.end(), "hdampUp") != vec_tree_type.end() ||
        find(vec_tree_type.begin(), vec_tree_type.end(), "mtop171p5") != vec_tree_type.end() ||
        find(vec_tree_type.begin(), vec_tree_type.end(), "mtop173p5") != vec_tree_type.end())
    {
      map_tree_mc_central[it->first] = (TTree *)map_fin_mc[it->first]->Get(channel + "/Central/Result_Tree");
      event.Setup_Tree(map_tree_mc_central[it->first], StringToSyst("Central"), false);
    }
  } // for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)

  for (unsigned int i = 0; i < vec_tree_type.size(); i++)
  {
    TString tree_type = vec_tree_type[i];
    map_map_tree_mc[tree_type] = &vec_map_tree_mc[i];
  }

  if (find(vec_tree_type.begin(), vec_tree_type.end(), "CP5Down") != vec_tree_type.end())
    Register_Top_Syst(map_fin_mc_cp5_down, map_tree_mc_cp5_down, "CP5Down");

  if (find(vec_tree_type.begin(), vec_tree_type.end(), "CP5Up") != vec_tree_type.end())
    Register_Top_Syst(map_fin_mc_cp5_up, map_tree_mc_cp5_up, "CP5Up");

  if (find(vec_tree_type.begin(), vec_tree_type.end(), "hdampDown") != vec_tree_type.end())
    Register_Top_Syst(map_fin_mc_hdamp_down, map_tree_mc_hdamp_down, "hdampDown");

  if (find(vec_tree_type.begin(), vec_tree_type.end(), "hdampUp") != vec_tree_type.end())
    Register_Top_Syst(map_fin_mc_hdamp_up, map_tree_mc_hdamp_up, "hdampUp");

  if (find(vec_tree_type.begin(), vec_tree_type.end(), "mtop171p5") != vec_tree_type.end())
    Register_Top_Syst(map_fin_mc_mtop_171p5, map_tree_mc_mtop_171p5, "mtop171p5");

  if (find(vec_tree_type.begin(), vec_tree_type.end(), "mtop173p5") != vec_tree_type.end())
    Register_Top_Syst(map_fin_mc_mtop_173p5, map_tree_mc_mtop_173p5, "mtop173p5");

  // if (find(vec_tree_type.begin(), vec_tree_type.end(), "TTbb4f") != vec_tree_type.end())
  //   Register_Top_Syst(map_fin_mc_tt_4f, map_tree_mc_tt_4f, "TTbb4f");

  // TFile and TTree handlers
  // if (run_flag == "Central" || run_flag == "All")
  //   map_map_tree_mc["Central"] = &map_tree_mc;

  // if (run_flag == "Jet_En_Down" || run_flag == "All")
  //   map_map_tree_mc["Jet_En_Down"] = &map_tree_mc_jec_down;

  // if (run_flag == "Jet_En_Up" || run_flag == "All")
  //   map_map_tree_mc["Jet_En_Up"] = &map_tree_mc_jec_up;

  // if (run_flag == "Jet_Res_Down" || run_flag == "All")
  //   map_map_tree_mc["Jet_Res_Down"] = &map_tree_mc_jer_down;

  // if (run_flag == "Jet_Res_Up" || run_flag == "All")
  //   map_map_tree_mc["Jet_Res_Up"] = &map_tree_mc_jer_up;

  // if (run_flag == "UE_Down" || run_flag == "All")
  //   map_map_tree_mc["UE_Down"] = &map_tree_mc_ue_down;

  // if (run_flag == "UE_Up" || run_flag == "All")
  //   map_map_tree_mc["UE_Up"] = &map_tree_mc_ue_up;

  // if (run_flag == "CP5_Down" || run_flag == "All")
  //   map_map_tree_mc["CP5_Down"] = &map_tree_mc_cp5_down;

  // if (run_flag == "CP5_Up" || run_flag == "All")
  //   map_map_tree_mc["CP5_Up"] = &map_tree_mc_cp5_up;

  // if (run_flag == "hDamp_Down" || run_flag == "All")
  //   map_map_tree_mc["hDamp_Down"] = &map_tree_mc_hdamp_down;

  // if (run_flag == "hDamp_Up" || run_flag == "All")
  //   map_map_tree_mc["hDamp_Up"] = &map_tree_mc_hdamp_up;

  // if (run_flag == "mTop_171p5" || run_flag == "All")
  //   map_map_tree_mc["mTop_171p5"] = &map_tree_mc_mtop_171p5;

  // if (run_flag == "mTop_173p5" || run_flag == "All")
  //   map_map_tree_mc["mTop_173p5"] = &map_tree_mc_mtop_173p5;

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

  // data
  if (find(vec_tree_type.begin(), vec_tree_type.end(), "Data") != vec_tree_type.end())
  {
    TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");
    TString result_path = path_sample_base + era + "/Vcb/DATA/";

    for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
    {
      cout << it->first << endl;

      if (it == samples.map_data.begin())
        data_short_name = samples.map_short_name_data[it->first];

      map_fin_data[it->first] = new TFile(result_path + it->second);
      map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get(channel + "/Central/Result_Tree");
      event.Setup_Tree(map_tree_data[it->first], Syst::Central, false);
    }
  } // if (find(vec_tree_type.begin(), vec_tree_type.end(), "Data") != vec_tree_type.end())

  cout << "[Histo_Syst::Register_Sample]: Done" << endl;
} // void Histo_Syst::Register_Sample()

//////////

void Histo_Syst::Register_Sample_TF()
{
  cout << "[Histo_Syst::Register_Sample_TF]: Init" << endl;

  TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  TString result_path = path_sample_base + era + "/Vcb_Cal_TF/";

  index_split = 0;
  n_split = 1;

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    map_fin_mc[it->first] = new TFile(result_path + "Central_Syst/" + it->second);
    map_tree_mc_central[it->first] = (TTree *)map_fin_mc[it->first]->Get(channel + "/Central/Result_Tree");
    event.Setup_Tree_Cal_TF(map_tree_mc_central[it->first], Syst::Central, false);
  }
  map_map_tree_mc["Central"] = &map_tree_mc_central;

  result_path += "DATA/";
  for (auto it = samples.map_data.begin(); it != samples.map_data.end(); it++)
  {
    cout << it->first << endl;
    if (it == samples.map_data.begin())
      data_short_name = samples.map_short_name_data[it->first];

    map_fin_data[it->first] = new TFile(result_path + it->second);
    map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get(channel + "/Central/Result_Tree");
    event.Setup_Tree_Cal_TF(map_tree_data[it->first], Syst::Central, false);
  }

  cout << "[Histo_Syst::Register_Sample_TF]: Done" << endl;

  return;
} // void Histo_Syst::Register_Sample_TF()

//////////

void Histo_Syst::Register_Top_Syst(map<TString, TFile *> &map_fin_syst, map<TString, TTree *> &map_tree_syst, const TString &type)
{
  cout << "[Histo_Syst::Register_Top_Syst]:" << type << endl;

  TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  TString result_path = path_sample_base + era + "/Vcb/Top_Syst/";

  if (type != "TTbb4f")
  {
    map_tree_syst = map_tree_mc_central;

    map_tree_syst.erase("TTLJ");
    map_tree_syst.erase("TTLL");
    map_tree_syst.erase("TTLJ_WtoCB");

    cout << "TTLJ" << endl;
    map_fin_syst["TTLJ_" + type] = new TFile(result_path + samples.map_top_syst["TTLJ_" + type]);
    map_tree_syst["TTLJ_" + type] = (TTree *)map_fin_syst["TTLJ_" + type]->Get(channel + "/Central/Result_Tree");
    event.Setup_Tree(map_tree_syst["TTLJ_" + type], Syst::Central);

    cout << "TTLL" << endl;
    map_fin_syst["TTLL_" + type] = new TFile(result_path + samples.map_top_syst["TTLL_" + type]);
    map_tree_syst["TTLL_" + type] = (TTree *)map_fin_syst["TTLL_" + type]->Get(channel + "/Central/Result_Tree");
    event.Setup_Tree(map_tree_syst["TTLL_" + type], Syst::Central);

    cout << "TTLJ_WtoCB" << endl;
    map_fin_syst["TTLJ_WtoCB_" + type] = new TFile(result_path + samples.map_top_syst["TTLJ_WtoCB_" + type]);
    map_tree_syst["TTLJ_WtoCB_" + type] = (TTree *)map_fin_syst["TTLJ_WtoCB_" + type]->Get(channel + "/Central/Result_Tree");
    event.Setup_Tree(map_tree_syst["TTLJ_WtoCB_" + type], Syst::Central);
  }
  // else
  // {
  //   cout << "TTLJ_TTbb_4f" << endl;
  //   map_fin_syst["TTLJ_TTbb_4f"] = new TFile(result_path + samples.map_top_syst["TTLJ_TTbb_4f"]);
  //   map_tree_syst["TTLJ_TTbb_4f"] = (TTree *)map_fin_syst["TTLJ_TTbb_4f"]->Get(channel + "/Central/Result_Tree");
  //   event.Setup_Tree(map_tree_syst["TTLJ_TTbb_4f"], Syst::Central);

  //   cout << "TTLJ_bbDPS" << endl;
  //   map_fin_syst["TTLJ_bbDPS"] = new TFile(result_path + samples.map_top_syst["TTLJ_bbDPS"]);
  //   map_tree_syst["TTLJ_bbDPS"] = (TTree *)map_fin_syst["TTLJ_bbDPS"]->Get(channel + "/Central/Result_Tree");
  //   event.Setup_Tree(map_tree_syst["TTLJ_bbDPS"], Syst::Central);

  //   cout << "TTLL_TTbb_4f" << endl;
  //   map_fin_syst["TTLL_TTbb_4f"] = new TFile(result_path + samples.map_top_syst["TTLL_TTbb_4f"]);
  //   map_tree_syst["TTLL_TTbb_4f"] = (TTree *)map_fin_syst["TTLL_TTbb_4f"]->Get(channel + "/Central/Result_Tree");
  //   event.Setup_Tree(map_tree_syst["TTLL_TTbb_4f"], Syst::Central);

  //   cout << "TTLL_bbDPS" << endl;
  //   map_fin_syst["TTLL_bbDPS"] = new TFile(result_path + samples.map_top_syst["TTLL_bbDPS"]);
  //   map_tree_syst["TTLL_bbDPS"] = (TTree *)map_fin_syst["TTLL_bbDPS"]->Get(channel + "/Central/Result_Tree");
  //   event.Setup_Tree(map_tree_syst["TTLL_bbDPS"], Syst::Central);
  // }

  map_map_tree_mc[type] = &map_tree_syst;

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

float Histo_Syst::Reweight_TTHF(const TString &sample_name)
{
  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")))
  {
    if (sample_name.Contains("WtoCB") || sample_name.Contains("TTbb") || sample_name.Contains("bbDPS"))
      return 1.0;

    bool chk_b = false;
    bool chk_c = false;

    // // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    // for (unsigned int i = 0; i < event.vec_sel_gen_hf_flavour->size(); i++)
    // {
    //   int flavour = event.vec_sel_gen_hf_flavour->at(i);
    //   int origin = event.vec_sel_gen_hf_origin->at(i);

    //   // if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
    //   if (flavour == 5 && abs(origin) == 21)
    //   {
    //     chk_b = true;
    //     break;
    //   }
    //   // else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
    //   else if (flavour == 4 && abs(origin) == 21)
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
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    return 1.;

  return -1;
} // float Histo_Syst::Reweight_TTHF(const TString &sample_name)

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
  bool chk_pass_iso; // For muon, iso. For electron, mva
  if (channel == "Mu")
  {
    if (event.lepton_rel_iso < REL_ISO_MUON)
      chk_pass_iso = true;
    else
      chk_pass_iso = false;
  }
  else if (channel == "El")
  {
    /* Isolation */
    // float rel_iso_electron_a;
    // float rel_iso_electron_b;
    // if (TMath::Abs(event.lepton_eta) <= 1.479)
    // {
    //   rel_iso_electron_a = REL_ISO_ELECTRON_BARREL_A;
    //   rel_iso_electron_b = REL_ISO_ELECTRON_BARREL_B;
    // }
    // else
    // {
    //   rel_iso_electron_a = REL_ISO_ELECTRON_ENDCAP_A;
    //   rel_iso_electron_b = REL_ISO_ELECTRON_ENDCAP_B;
    // }

    // if (event.lepton_rel_iso < rel_iso_electron_a + rel_iso_electron_b / event.lepton_pt_uncorr)
    //   chk_pass_iso = true;
    // else
    //   chk_pass_iso = false;

    /* Tight Loose */
    unsigned int mva_iso_wp80_bit_mask = 1 << 4;
    unsigned int mva_iso_wp90_bit_mask = 1 << 5;

    if ((event.electron_id_bit & mva_iso_wp80_bit_mask) == mva_iso_wp80_bit_mask)
      chk_pass_iso = true;
    else
      chk_pass_iso = false;

    // cout << "Electron ID Bit: " << event.electron_id_bit << ", MVA ISO WP80: " << ((event.electron_id_bit & mva_iso_wp80_bit_mask) == mva_iso_wp80_bit_mask) << ", MVA ISO WP90: " << ((event.electron_id_bit & mva_iso_wp90_bit_mask) == mva_iso_wp90_bit_mask) << endl;
  }

  if (MET_PT < event.met_pt)
  {
    if (chk_pass_iso)
      return 3; //"D"
    else
      return 2; //"C"
  }
  else
  {
    if (chk_pass_iso)
      return 1; //"B"
    else
      return 0; //"A"
  }

  // if (4 <= event.n_jets)
  // {
  //   if (chk_pass_iso)
  //     return 3; //"D"
  //   else
  //     return 2; //"C"
  // }
  // else
  // {
  //   if (chk_pass_iso)
  //     return 1; //"B"
  //   else
  //     return 0; //"A"
  // }

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

  if (mode == "Cal_TF")
    return 0;
  else
  {
    if (chk_bin_optimizer)
    {
      if (event.n_bjets == 2)
        return 0;
      else
        return 1;
    }
    else
    {
      if (event.n_bjets == 2)
        return 0;
      else
      {
        if (event.template_score < threeb_cr_cut)
          return 1;
        else
          return 2;
      }
    }
  }

  return 0;

} // void Histo_Syst::Set_Region()

//////////