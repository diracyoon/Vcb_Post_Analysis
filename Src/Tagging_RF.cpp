#include "Tagging_RF.h"

ClassImp(Tagging_RF);

//////////

Tagging_RF::Tagging_RF(const TString &a_era, const TString &a_mode, const TString &a_channel, const TString &a_extension) : samples(a_era, a_channel, "Vcb_Tagging_RF")
{
  cout << "[Tagging_RF::Tagging_RF]: Init analysis" << endl;

  era = a_era;
  mode = a_mode;
  channel = a_channel;
  extension = a_extension;

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

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    cout << it->second << endl;
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
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_hdampDown"));
  vec_short_name_mc.push_back("TTLL_hdampDown");
  vec_short_name_mc.push_back("TTLL_hdampDown_CC");
  vec_short_name_mc.push_back("TTLL_hdampDown_BB");

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_hdampUp"));
  vec_short_name_mc.push_back("TTLL_hdampUp");
  vec_short_name_mc.push_back("TTLL_hdampUp_CC");
  vec_short_name_mc.push_back("TTLL_hdampUp_BB");

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_CP5Down"));
  vec_short_name_mc.push_back("TTLL_CP5Down");
  vec_short_name_mc.push_back("TTLL_CP5Down_CC");
  vec_short_name_mc.push_back("TTLL_CP5Down_BB");

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_CP5Up"));
  vec_short_name_mc.push_back("TTLL_CP5Up");
  vec_short_name_mc.push_back("TTLL_CP5Up_CC");
  vec_short_name_mc.push_back("TTLL_CP5Up_BB");

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_mtop171p5"));
  vec_short_name_mc.push_back("TTLL_mtop171p5");
  vec_short_name_mc.push_back("TTLL_mtop171p5_CC");
  vec_short_name_mc.push_back("TTLL_mtop171p5_BB");

  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL_mtop173p5"));
  vec_short_name_mc.push_back("TTLL_mtop173p5");
  vec_short_name_mc.push_back("TTLL_mtop173p5_CC");
  vec_short_name_mc.push_back("TTLL_mtop173p5_BB");

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

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

} // Tagging_RF::Tagging_RF(const TString& a_era, const TString& a_channel)

//////////

Tagging_RF::~Tagging_RF()
{
  cout << "[Tagging_RF::~Tagging_RF]: Init" << endl;

  if (mode == "Analysis")
  {
    TString fout_name = "Vcb_Tagging_RF_" + era + "_" + channel + ".root";
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
  }
  else if (mode == "Validation")
  {
    TString fout_name = "Vcb_Tagging_RF_Validation_" + era + "_" + channel + ".root";
    fout = new TFile(fout_name, "RECREATE");

    fout->cd();
    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      TDirectory *dir = fout->mkdir(vec_short_name_mc[i]);

      for (int j = 0; j < n_syst_c; j++)
      {
        for (int k = 0; k < 3; k++)
        {
          dir->cd();

          histo_closure_n_jet[i][j][k]->Write();
          histo_closure_ht[i][j][k]->Write();
          histo_closure_n_pv[i][j][k]->Write();

          histo_closure_bvsc[i][j][k]->Write();
          histo_closure_cvsb[i][j][k]->Write();
          histo_closure_cvsl[i][j][k]->Write();

          histo_closure_eta[i][j][k]->Write();
          histo_closure_pt[i][j][k]->Write();
        }
      }
    }

    fout->Close();

  } // else if (mode == "Validation")
  else if (mode == "Application")
  {
    fin->Close();
  } // else if (mode == "Application")
} // Tagging_RF::~Tagging_RF()

//////////

float Tagging_RF::Get_Tagging_RF_B_Tag(const TString &sample, const TString &syst, const int &n_jet, const float &ht)
{
  if (!syst.Contains("B_Tag"))
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Input B_Tag" << endl;
    exit(1);

    return -1;
  }

  int sample_index = find(sample_name.begin(), sample_name.end(), sample) - sample_name.begin();
  int syst_index = find(syst_name_b.begin(), syst_name_b.end(), syst) - syst_name_b.begin();

  if (sample_index == sample_name.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_b.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  int bin = ratio_b[sample_index][syst_index]->FindBin(n_jet, ht);
  return ratio_b[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF::Get_Tagging_RF(const TString &syst_name, const int &n_jet)

//////////

float Tagging_RF::Get_Tagging_RF_C_Tag(const TString &sample, const TString &syst, const int &n_pv_, const float &ht_)
{
  if (!syst.Contains("C_Tag"))
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Input C_Tag" << endl;
    exit(1);
  }

  int sample_index = find(vec_sample_tagging_rf.begin(), vec_sample_tagging_rf.end(), sample) - vec_sample_tagging_rf.begin();
  int syst_index = find(syst_name_c.begin(), syst_name_c.end(), syst) - syst_name_c.begin();

  if (sample_index == vec_sample_tagging_rf.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_c.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  float n_pv = n_pv_;
  float ht = ht_;

  // handle underflow
  if (n_pv_ < bin_npv[0])
    n_pv = bin_npv[0] + 0.1;
  if (ht_ < bin_ht[0])
    ht = bin_ht[0] + 0.1;

  // handle overflow
  if (bin_npv[bin_npv.size() - 1] < n_pv_)
    n_pv = bin_npv[bin_npv.size() - 1] - 0.1;
  if (bin_ht[bin_ht.size() - 1] < ht_)
    ht = bin_ht[bin_ht.size() - 1] - 0.1;

  int bin = ratio_c[sample_index][syst_index]->FindBin(n_pv, ht);

  return ratio_c[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF::Get_Tagging_RF_C_Tag(const TString &syst_name, const int &n_pv, const float& ht)

//////////

void Tagging_RF::Combine_Decay_Mode()
{
  // Combine decay mode of W
  cout << "[Tagging_RF::Combine_Decay_Mode]: Init" << endl;

  vector<TString> vec_sample = {"TTLJ", "TTLJ_CC", "TTLJ_BB"};

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

      cout << Form("%s_2/Ratio_%s_2_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()) << endl;

      TH2D *histo_2 = (TH2D *)fout->Get(Form("%s_2/Ratio_%s_2_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_4 = (TH2D *)fout->Get(Form("%s_4/Ratio_%s_4_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));
      TH2D *histo_45 = (TH2D *)fout->Get(Form("%s_45/Ratio_%s_45_%s", vec_sample[i].Data(), vec_sample[i].Data(), vec_syst_name[j].Data()));

      if (vec_syst_name[j].Contains("C_Tag"))
      {
        // cout << "KS (2, 4) = " << histo_2->KolmogorovTest(histo_4, "N") << ", KS (2, 45) = " << histo_2->KolmogorovTest(histo_45, "N") << ", S (4, 45) = " << histo_4->KolmogorovTest(histo_45, "N") << endl;
        cout << "Chi2 (2, 4) = " << histo_2->Chi2Test(histo_4, "WW") << ", Chi2 (2, 45) = " << histo_2->Chi2Test(histo_45, "WW") << ", Chi2 (4, 45) = " << histo_4->Chi2Test(histo_45, "WW") << endl;
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

  return;
} // void Tagging_RF::Combine_Decay_Mode()

//////////

void Tagging_RF::Combine_Lepton_Channel()
{
  // Combine
  cout << "[Tagging_RF::Combine_Lepton_Channel]: Init" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF/";

  fin_mu = new TFile(path_base + "Vcb_Tagging_RF_" + era + "_Mu.root");
  fin_el = new TFile(path_base + "Vcb_Tagging_RF_" + era + "_El.root");

  TString fout_name = "Vcb_Tagging_RF_" + era + ".root";
  fout = new TFile(fout_name, "RECREATE");

  TList *list_dir_mu = fin_mu->GetListOfKeys();
  for (int i = 0; i < list_dir_mu->GetEntries(); i++)
  {
    TObject *dir = (TObject *)list_dir_mu->At(i);
    TString dir_name = dir->GetName();
    TList *list_key = ((TDirectory *)fin_mu->Get(dir_name))->GetListOfKeys();

    fout->cd();
    TDirectory *dir_fout = fout->mkdir(dir_name);

    for (int j = 0; j < list_key->GetEntries(); j++)
    {
      TObject *key = (TObject *)list_key->At(j);
      TString key_name = key->GetName();

      if (key_name.Contains("Ratio_"))
      {
        TString histo_name = dir_name + "/" + key_name;
        cout << dir_name << " " << histo_name << endl;

        TH2D *ratio_mu;
        TH2D *ratio_el;

        ratio_mu = (TH2D *)fin_mu->Get(histo_name);
        ratio_el = (TH2D *)fin_el->Get(histo_name);

        // consistency check
        if (key_name.Contains("C_Tag"))
          cout << ratio_mu->Chi2Test(ratio_el, "WW") << endl;

        // combine
        TString sub = key_name.Remove(0, 6);
        sub = dir_name + "/" + sub;

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

        dir_fout->cd();
        histo_before_combine->Write();
        histo_after_combine->Write();
        ratio_average->Write();
      }
    }
  } // loop over mc_merged

  return;
} // void Tagging_RF::Combine_Lepton_Channel()

//////////

void Tagging_RF::Draw_Result()
{
  cout << "[Tagging_RF::Draw_Result]: Init" << endl;

  gStyle->SetPaintTextFormat("0.3f");
  gStyle->SetOptStat(0);

  // draw results
  vector<TString> sample_to_draw = {"TTLJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
                                    "TTLJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
                                    "TTLJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
                                    "TTLL", "TTLL_CC", "TTLL_BB"};
  vector<TString> syst_to_draw = {"C_Tag_Nominal",
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

  for (unsigned int i = 0; i < sample_to_draw.size(); i++)
  {
    for (unsigned int j = 0; j < syst_to_draw.size(); j++)
    {
      TH2D *histo = (TH2D *)fout->Get(Form("%s/Ratio_%s_%s", sample_to_draw[i].Data(), sample_to_draw[i].Data(), syst_to_draw[j].Data()));

      TString can_name = Form("Tagging_RF_%s_%s_%s", sample_to_draw[i].Data(), syst_to_draw[j].Data(), era.Data());
      TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
      canvas->Draw();

      histo->GetXaxis()->SetTitle("Number of Primary Vertex");
      histo->GetYaxis()->SetTitle("HT [GeV]");
      histo->GetZaxis()->SetRangeUser(0.6, 1.1);
      histo->Draw("COLZ texte");

      canvas->Print(can_name + "." + extension, extension);
    }
  }

  // compare decay mode
  sample_to_draw = {"TTLJ", "TTLJ_CC", "TTLJ_BB"};

  for (unsigned int i = 0; i < sample_to_draw.size(); i++)
  {
    cout << sample_to_draw[i] << endl;

    for (unsigned int j = 0; j < syst_to_draw.size(); j++)
    // for (unsigned int j = 0; j < 1; j++)
    {
      cout << syst_to_draw[j] << endl;

      TString can_name = Form("Comp_Tagging_RF_Decay_Mode_%s_%s", sample_to_draw[i].Data(), syst_to_draw[j].Data());
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

      canvas->cd(1);

      TLegend *tl = new TLegend(0.65, 0.66, 0.85, 0.85);
      tl->SetBorderSize(0);
      tl->AddEntry(histo_proj_x[0], "W#rightarrow ud(us)", "lep");
      tl->AddEntry(histo_proj_x[1], "W#rightarrow cd(cs)", "lep");
      tl->AddEntry(histo_proj_x[2], "W#rightarrow cb", "lep");
      tl->Draw("SAME");

      canvas->Print(can_name + "." + extension, extension);
    } // loop over syst_to_draw
  }   // loop over sample_to_draw

  // compare TTbar syst
  sample_to_draw = {"TTLJ", "TTLL"};

  for (unsigned int i = 0; i < sample_to_draw.size(); i++)
  {
    // TTLJ, TTLJ_CC, TTLJ_BB for norminal

    TH2D *histo_before = (TH2D *)fout->Get(Form("%s/%s_C_Tag_Nominal_Before", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    TH2D *histo_cc_before = (TH2D *)fout->Get(Form("%s_CC/%s_CC_C_Tag_Nominal_Before", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    TH2D *histo_bb_before = (TH2D *)fout->Get(Form("%s_BB/%s_BB_C_Tag_Nominal_Before", sample_to_draw[i].Data(), sample_to_draw[i].Data()));

    TH2D *histo_after = (TH2D *)fout->Get(Form("%s/%s_C_Tag_Nominal_After", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    TH2D *histo_cc_after = (TH2D *)fout->Get(Form("%s_CC/%s_CC_C_Tag_Nominal_After", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    TH2D *histo_bb_after = (TH2D *)fout->Get(Form("%s_BB/%s_BB_C_Tag_Nominal_After", sample_to_draw[i].Data(), sample_to_draw[i].Data()));

    TH2D *histo_combine_before = (TH2D *)histo_before->Clone();
    histo_combine_before->Add(histo_cc_before);
    histo_combine_before->Add(histo_bb_before);

    TH2D *histo_combine_after = (TH2D *)histo_after->Clone();
    histo_combine_after->Add(histo_cc_after);
    histo_combine_after->Add(histo_bb_after);

    TH2D *ratio_nominal = (TH2D *)histo_combine_before->Clone();
    ratio_nominal->Divide(histo_combine_after);

    TH2D *histo[7];
    histo[0] = ratio_nominal;
    histo[1] = (TH2D *)fout->Get(Form("%s_hdampDown/Ratio_%s_hdampDown_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    histo[2] = (TH2D *)fout->Get(Form("%s_hdampUp/Ratio_%s_hdampUp_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    histo[3] = (TH2D *)fout->Get(Form("%s_CP5Down/Ratio_%s_CP5Down_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    histo[4] = (TH2D *)fout->Get(Form("%s_CP5Up/Ratio_%s_CP5Up_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    histo[5] = (TH2D *)fout->Get(Form("%s_mtop171p5/Ratio_%s_mtop171p5_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
    histo[6] = (TH2D *)fout->Get(Form("%s_mtop173p5/Ratio_%s_mtop173p5_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));

    TString can_name = Form("Comp_Tagging_RF_%s_Syst_%s", sample_to_draw[i].Data(), era.Data());
    TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
    canvas->Divide(2, 1);
    canvas->Draw();

    TH1D *histo_proj_x[7];
    TH1D *histo_proj_y[7];
    for (unsigned int j = 0; j < 7; j++)
    {
      histo_proj_x[j] = (TH1D *)histo[j]->ProjectionX()->Clone();
      histo_proj_x[j]->Scale(1. / histo[j]->GetNbinsY());
      histo_proj_x[j]->SetLineColor(j + 1);

      histo_proj_y[j] = (TH1D *)histo[j]->ProjectionY()->Clone();
      histo_proj_y[j]->Scale(1. / histo[j]->GetNbinsX());
      histo_proj_y[j]->SetLineColor(j + 1);

      canvas->cd(1);
      if (j == 0)
      {
        histo_proj_x[j]->SetTitle(Form("%s Tagging Patch Projection X", sample_to_draw[i].Data()));
        histo_proj_x[j]->GetXaxis()->SetTitle("Number of Primary Vertex");
        histo_proj_x[j]->GetYaxis()->SetTitle("r");

        histo_proj_x[j]->Draw();
      }
      else
        histo_proj_x[j]->Draw("SAME");

      canvas->cd(2);
      if (j == 0)
      {
        histo_proj_y[j]->SetTitle(Form("%s Tagging Patch Projection Y", sample_to_draw[i].Data()));
        histo_proj_y[j]->GetXaxis()->SetTitle("HT [GeV]");
        histo_proj_y[j]->GetYaxis()->SetTitle("r");

        histo_proj_y[j]->Draw();
      }
      else
        histo_proj_y[j]->Draw("SAME");
    }

    TLegend *tl = new TLegend(0.65, 0.66, 0.85, 0.85);
    tl->SetBorderSize(0);
    tl->AddEntry(histo_proj_x[0], "Nominal", "lep");
    tl->AddEntry(histo_proj_x[1], "hdampDown", "lep");
    tl->AddEntry(histo_proj_x[2], "hdampUp", "lep");
    tl->AddEntry(histo_proj_x[3], "CP5Down", "lep");
    tl->AddEntry(histo_proj_x[4], "CP5up", "lep");
    tl->AddEntry(histo_proj_x[5], "mTop171p5", "lep");
    tl->AddEntry(histo_proj_x[6], "mTop173p5", "lep");

    canvas->cd(1);
    tl->Draw();

    canvas->Print(can_name + "." + extension, extension);
  } // TTLJ or TTLL

  return;
} // void Tagging_RF::Draw_Result()

//////////

void Tagging_RF::Draw_Validation()
{
  cout << "Tagging_RF::Draw_Validation" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base = path_base + "/Workplace/Tagging_RF";

  TFile *fin = new TFile(Form("%s/Vcb_Tagging_RF_Validation_%s_%s.root", path_base.Data(), era.Data(), channel.Data()));

  gStyle->SetOptStat(0);

  vector<TString> vec_sample_to_draw = {"TTLJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
                                        "TTLJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
                                        "TTLJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
                                        "TTLL", "TTLL_CC", "TTLL_BB"};

  TLatex *latex = new TLatex();
  latex->SetTextSize(0.03);

  for (int i = 0; i < vec_sample_to_draw.size(); i++)
  {
    TString sample_to_draw = vec_sample_to_draw[i];
    cout << sample_to_draw << endl;

    for (int j = 0; j < n_syst_c; j++)
    // for (int j = 0; j < 3; j++)
    {
      TString can_name = "Validation_" + era + "_" + channel + "_" + syst_name_c[j] + "_" + sample_to_draw;
      cout << can_name << endl;

      TCanvas *canvas = new TCanvas(can_name, can_name, 1600, 1000);
      canvas->Divide(3, 1);
      canvas->Draw();

      TPad *pad[3][2];
      TH1D *histo_closure[3];
      for (int k = 0; k < 3; k++)
      {
        TString score_type;
        if (k == 0)
          score_type = "BvsC";
        else if (k == 1)
          score_type = "CvsB";
        else if (k == 2)
          score_type = "CvsL";

        for (int l = 0; l < 3; l++)
        {
          TString pad_name = to_string(k) + "_" + to_string(l);
          if (l == 0)
            pad[k][l] = new TPad(pad_name, pad_name, 0, 0.3, 1, 1);
          else if (l == 1)
            pad[k][l] = new TPad(pad_name, pad_name, 0, 0, 1, 0.3);

          TString histo_name = "Closure_" + sample_to_draw + "_" + syst_name_c[j] + "_Leading_Jet_" + score_type + "_" + to_string(l);
          cout << histo_name << endl;
          histo_closure[l] = (TH1D *)fin->Get(sample_to_draw + "/" + histo_name);
        }

        canvas->cd(k + 1);
        pad[k][0]->Draw();
        pad[k][0]->cd();

        for (int l = 0; l < 3; l++)
        {
          histo_closure[l]->SetLineColor(l + 2);

          if (l == 0)
          {
            if (k == 0)
              histo_closure[l]->GetXaxis()->SetTitle("BvsC");
            else if (k == 1)
              histo_closure[l]->GetXaxis()->SetTitle("CvsB");
            else if (k == 2)
              histo_closure[l]->GetXaxis()->SetTitle("CvsL");

            histo_closure[l]->Draw();
          }
          else
            histo_closure[l]->Draw("SAME");
        }

        float count_raw = histo_closure[0]->Integral();
        float count_sf = histo_closure[1]->Integral();
        float count_rf = histo_closure[2]->Integral();

        cout << "Count_Raw = " << count_raw << ", Count_SF = " << count_sf << ", Count_RF = " << count_rf << endl;

        if (k == 0)
        {
          latex->DrawLatexNDC(0.3, 0.5, Form("Yield Raw = %.2f", count_raw));
          latex->DrawLatexNDC(0.3, 0.45, Form("Yield SF = %.2f", count_sf));
          latex->DrawLatexNDC(0.3, 0.4, Form("Yield Patched = %.2f", count_rf));

          TLegend *tl = new TLegend(0.3, 0.6, 0.8, 0.8);
          tl->SetBorderSize(0);
          tl->AddEntry(histo_closure[0], "Raw", "lep");
          tl->AddEntry(histo_closure[1], "+ C-Tagging SF", "lep");
          tl->AddEntry(histo_closure[2], "+ C-Tagging SF + C-Tagging Patch", "lep");
          tl->Draw("same");
        }

        canvas->cd(k + 1);
        pad[k][1]->Draw();
        pad[k][1]->cd();

        TH1D *histo_closure_ratio[3];
        for (int l = 0; l < 3; l++)
        {
          histo_closure_ratio[l] = (TH1D *)histo_closure[l]->Clone();
          histo_closure_ratio[l]->Divide(histo_closure[1]);

          histo_closure_ratio[l]->SetLineColor(l + 2);
          if (l == 0)
          {
            if (k == 0)
              histo_closure_ratio[l]->GetXaxis()->SetTitle("BvsC");
            else if (k == 1)
              histo_closure_ratio[l]->GetXaxis()->SetTitle("CvsB");
            else if (k == 2)
              histo_closure_ratio[l]->GetXaxis()->SetTitle("CvsL");

            histo_closure_ratio[l]->GetYaxis()->SetTitle("Ratio");
            histo_closure_ratio[l]->GetYaxis()->SetRangeUser(0.4, 1.6);

            histo_closure_ratio[l]->Draw();
          }
          else if (l == 2)
            histo_closure_ratio[l]->Draw("SAME");
        }
      }

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
  }   // loop over vec_sample_to_draw

  return;
} // void Tagging_RF::Draw_Validation()

//////////

void Tagging_RF::Fill_Histo_MC(const TString &sample_name, const TString &syst_type)
{
  int sample_index = Histo_Index(sample_name);

  float weight = 1;
  weight *= weight_hem_veto;
  weight *= weight_lumi;
  weight *= weight_mc;
  weight *= weight_pileup;
  weight *= weight_prefire;
  weight *= weight_top_pt;

  weight *= sf_sl_trig;

  weight *= sf_mu_id;
  weight *= sf_mu_iso;

  weight *= sf_el_id;
  weight *= sf_el_reco;

  // cout << weight_lumi << " " << weight_mc << " " << weight_pileup << " " << weight_prefire << " " << weight_top_pt << " " << sf_sl_trig << " " << sf_el_id << " " << sf_el_reco << endl;
  if (TMath::IsNaN(weight))
  {
    cout << "Nan detected" << weight_lumi << " " << weight_mc << " " << weight_pileup << " " << weight_prefire << " " << weight_top_pt << " " << sf_sl_trig << " " << sf_el_id << " " << sf_el_reco << endl;
  }

  if (syst_type == "Central")
  {
    // norminal
    histo_mc_before_b[sample_index][0]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][0]->Fill(n_jets, ht, weight * weight_b_tag);

    // hf
    histo_mc_before_b[sample_index][1]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][1]->Fill(n_jets, ht, weight * weight_b_tag_hf_down);

    histo_mc_before_b[sample_index][2]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][2]->Fill(n_jets, ht, weight * weight_b_tag_hf_up);
  }

  // jet en down
  if (syst_type == "JetEnDown")
  {
    histo_mc_before_b[sample_index][3]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][3]->Fill(n_jets, ht, weight * weight_b_tag_jes_down);
  }

  // jet en up
  if (syst_type == "JetEnUp")
  {
    histo_mc_before_b[sample_index][4]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][4]->Fill(n_jets, ht, weight * weight_b_tag_jes_up);
  }

  if (syst_type == "Central")
  {
    // lfstats1
    histo_mc_before_b[sample_index][5]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][5]->Fill(n_jets, ht, weight * weight_b_tag_lfstats1_down);

    histo_mc_before_b[sample_index][6]->Fill(n_jets, ht, weight);
    histo_mc_after_b[sample_index][6]->Fill(n_jets, ht, weight * weight_b_tag_lfstats1_up);

    // lfstats1
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

  if (syst_type == "Central")
  {
    // norminal
    histo_mc_before_c[sample_index][0]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][0]->Fill(n_pv, ht, weight * weight_c_tag);

    // extrap
    histo_mc_before_c[sample_index][1]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][1]->Fill(n_pv, ht, weight * weight_c_tag_extrap_down);

    histo_mc_before_c[sample_index][2]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][2]->Fill(n_pv, ht, weight * weight_c_tag_extrap_up);

    // interp
    histo_mc_before_c[sample_index][3]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][3]->Fill(n_pv, ht, weight * weight_c_tag_interp_down);

    histo_mc_before_c[sample_index][4]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][4]->Fill(n_pv, ht, weight * weight_c_tag_interp_up);

    // muf
    histo_mc_before_c[sample_index][5]->Fill(n_pv, ht, weight * weight_scale_variation_2);
    histo_mc_after_c[sample_index][5]->Fill(n_pv, ht, weight * weight_scale_variation_2 * weight_c_tag_lhe_scale_muf_down);

    histo_mc_before_c[sample_index][6]->Fill(n_pv, ht, weight * weight_scale_variation_1);
    histo_mc_after_c[sample_index][6]->Fill(n_pv, ht, weight * weight_scale_variation_1 * weight_c_tag_lhe_scale_muf_up);

    // mur
    histo_mc_before_c[sample_index][7]->Fill(n_pv, ht, weight * weight_scale_variation_6);
    histo_mc_after_c[sample_index][7]->Fill(n_pv, ht, weight * weight_scale_variation_6 * weight_c_tag_lhe_scale_mur_down);

    histo_mc_before_c[sample_index][8]->Fill(n_pv, ht, weight * weight_scale_variation_3);
    histo_mc_after_c[sample_index][8]->Fill(n_pv, ht, weight * weight_scale_variation_3 * weight_c_tag_lhe_scale_mur_up);

    // fsr
    histo_mc_before_c[sample_index][9]->Fill(n_pv, ht, weight * weight_ps[0]);
    histo_mc_after_c[sample_index][9]->Fill(n_pv, ht, weight * weight_ps[0] * weight_c_tag_ps_fsr_fixed_down);

    histo_mc_before_c[sample_index][10]->Fill(n_pv, ht, weight * weight_ps[1]);
    histo_mc_after_c[sample_index][10]->Fill(n_pv, ht, weight * weight_ps[1] * weight_c_tag_ps_fsr_fixed_up);

    // isr
    histo_mc_before_c[sample_index][11]->Fill(n_pv, ht, weight * weight_ps[2]);
    histo_mc_after_c[sample_index][11]->Fill(n_pv, ht, weight * weight_ps[2] * weight_c_tag_ps_isr_fixed_down);

    histo_mc_before_c[sample_index][12]->Fill(n_pv, ht, weight * weight_ps[3]);
    histo_mc_after_c[sample_index][12]->Fill(n_pv, ht, weight * weight_ps[3] * weight_c_tag_ps_isr_fixed_up);

    // pu
    histo_mc_before_c[sample_index][13]->Fill(n_pv, ht, weight * weight_pileup_down / weight_pileup);
    histo_mc_after_c[sample_index][13]->Fill(n_pv, ht, weight * weight_pileup_down / weight_pileup * weight_c_tag_pu_down);

    histo_mc_before_c[sample_index][14]->Fill(n_pv, ht, weight * weight_pileup_up / weight_pileup);
    histo_mc_after_c[sample_index][14]->Fill(n_pv, ht, weight * weight_pileup_up / weight_pileup * weight_c_tag_pu_up);

    // stat
    histo_mc_before_c[sample_index][15]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][15]->Fill(n_pv, ht, weight * weight_c_tag_stat_down);

    histo_mc_before_c[sample_index][16]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][16]->Fill(n_pv, ht, weight * weight_c_tag_stat_up);

    // unc_dyjets_b
    histo_mc_before_c[sample_index][17]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][17]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_b_down);

    histo_mc_before_c[sample_index][18]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][18]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_b_up);

    // unc_dyjets_c
    histo_mc_before_c[sample_index][19]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][19]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_c_down);

    histo_mc_before_c[sample_index][20]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][20]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_c_up);

    // unc_wjets_c
    histo_mc_before_c[sample_index][21]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][21]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_wjets_c_down);

    histo_mc_before_c[sample_index][22]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][22]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_wjets_c_up);
  }

  // jer
  if (syst_type == "JetResDown")
  {
    histo_mc_before_c[sample_index][23]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][23]->Fill(n_pv, ht, weight * weight_c_tag_jer_down);
  }

  if (syst_type == "JetResUp")
  {
    histo_mc_before_c[sample_index][24]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][24]->Fill(n_pv, ht, weight * weight_c_tag_jer_up);
  }

  // jes
  if (syst_type == "JetEnDown")
  {
    histo_mc_before_c[sample_index][25]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][25]->Fill(n_pv, ht, weight * weight_c_tag_jes_total_down);
  }

  if (syst_type == "JetEnUp")
  {
    histo_mc_before_c[sample_index][26]->Fill(n_pv, ht, weight);
    histo_mc_after_c[sample_index][26]->Fill(n_pv, ht, weight * weight_c_tag_jes_total_up);
  }

  return;
} // void Tagging_RF::Fill_Histo_MC(const int &sample_index, const TString &syst_type)

//////////

void Tagging_RF::Fill_Histo_Validation_MC(const TString &sample_name, const TString &syst_type)
{
  int sample_index = Histo_Index(sample_name);
  int sample_index_rf = Histo_Index_RF(sample_name);

  for (int i = 0; i < n_syst_c; i++)
  {
    TString c_tag_type = syst_name_c[i];

    if ((c_tag_type == "C_Tag_JER_Down" && syst_type != "JetResDown") || (c_tag_type != "C_Tag_JER_Down" && syst_type == "JetResDown"))
      continue;

    if ((c_tag_type == "C_Tag_JER_Up" && syst_type != "JetResUp") || (c_tag_type != "C_Tag_JER_Up" && syst_type == "JetResUp"))
      continue;

    if ((c_tag_type == "C_Tag_JES_Total_Down" && syst_type != "JetEnDown") || (c_tag_type != "C_Tag_JES_Total_Down" && syst_type == "JetEnDown"))
      continue;

    if ((c_tag_type == "C_Tag_JES_Total_Up" && syst_type != "JetEnUp") || (c_tag_type != "C_Tag_JES_Total_Up" && syst_type == "JetEnUp"))
      continue;

    float weight_raw = weight_hem_veto;
    weight_raw *= weight_lumi;
    weight_raw *= weight_mc;
    weight_raw *= weight_pileup;
    weight_raw *= weight_prefire;
    weight_raw *= weight_top_pt;

    weight_raw *= sf_sl_trig;

    weight_raw *= sf_mu_id;
    weight_raw *= sf_mu_iso;

    weight_raw *= sf_el_id;
    weight_raw *= sf_el_reco;

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
    else if (c_tag_type == "C_Tag_JES_Total_Down")
      weight_c_sf = weight_c_tag_jes_total_down;
    else if (c_tag_type == "C_Tag_JES_Total_Up")
      weight_c_sf = weight_c_tag_jes_total_up;

    float weight_sf = weight_raw * weight_c_sf;

    float c_rf = Get_Tagging_RF_C_Tag(vec_sample_tagging_rf[sample_index_rf], c_tag_type, n_pv, ht);
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

      histo_closure_n_jet[sample_index][i][j]->Fill(n_jets, weight);
      histo_closure_ht[sample_index][i][j]->Fill(ht, weight);
      histo_closure_n_pv[sample_index][i][j]->Fill(n_pv, weight);

      histo_closure_bvsc[sample_index][i][j]->Fill(leading_jet_bvsc, weight);
      histo_closure_cvsb[sample_index][i][j]->Fill(leading_jet_cvsb, weight);
      histo_closure_cvsl[sample_index][i][j]->Fill(leading_jet_cvsl, weight);

      histo_closure_eta[sample_index][i][j]->Fill(leading_jet_eta, weight);
      histo_closure_pt[sample_index][i][j]->Fill(leading_jet_pt, weight);
    }
  }

  return;
} // void Tagging_RF::Fill_Histo_Validation_MC(const int& sample_index)

//////////

int Tagging_RF::Histo_Index(const TString &sample_name)
{
  int index = -999;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    bool chk_b = false;
    bool chk_c = false;

    // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    for (unsigned int i = 0; i < vec_sel_gen_hf_flavour->size(); i++)
    {
      int flavour = vec_sel_gen_hf_flavour->at(i);
      int origin = vec_sel_gen_hf_origin->at(i);

      if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      {
        chk_b = true;
        break;
      }
      else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
        chk_c = true;
    }

    TString histo_name = samples.map_short_name_mc[sample_name];
    if (histo_name.Contains("WtoCB"))
      histo_name = "TTLJ";

    if (chk_b)
      histo_name += "_BB";
    else if (chk_c)
      histo_name += "_CC";

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
} // int Tagging_RF::Histo_Index(const TString &sample_name)

//////////

int Tagging_RF::Histo_Index_RF(const TString &sample_name)
{
  int index = -999;

  if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  {
    bool chk_b = false;
    bool chk_c = false;

    // for (unsigned int i = 0; i < vec_gen_hf_flavour->size(); i++)
    for (unsigned int i = 0; i < vec_sel_gen_hf_flavour->size(); i++)
    {
      int flavour = vec_sel_gen_hf_flavour->at(i);
      int origin = vec_sel_gen_hf_origin->at(i);

      if (flavour == 5 && abs(origin) != 6 && abs(origin) != 24)
      {
        chk_b = true;
        break;
      }
      else if (flavour == 4 && abs(origin) != 6 && abs(origin) != 24)
        chk_c = true;
    }

    TString histo_name = samples.map_short_name_mc[sample_name];

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

    if (decay_mode == 21 || decay_mode == 23)
      histo_name += "_2";
    else if (decay_mode == 41 || decay_mode == 43)
      histo_name += "_4";
    else if (decay_mode == 45)
      histo_name += "_45";

    index = find(vec_sample_tagging_rf.begin(), vec_sample_tagging_rf.end(), histo_name) - vec_sample_tagging_rf.begin();
  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_sample_tagging_rf.begin(), vec_sample_tagging_rf.end(), samples.map_short_name_mc[sample_name]) - vec_sample_tagging_rf.begin();

  return index;
} // int Tagging_RF::Histo_Index_RF(const TString &sample_name)

//////////

void Tagging_RF::Ratio()
{
  cout << "[Tagging_RF::Ratio] Init" << endl;

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

  ratio_c = new TH2D **[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    ratio_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      TString ratio_name = "Ratio_" + vec_short_name_mc[i] + "_" + syst_name_c[j];
      ratio_c[i][j] = new TH2D(ratio_name, ratio_name, bin_npv.size() - 1, bin_npv.data(), bin_ht.size() - 1, bin_ht.data());

      ratio_c[i][j]->Divide(histo_mc_before_c[i][j], histo_mc_after_c[i][j]);
    }
  }

  return;
} // void Tagging_RF::Ratio()

//////////

void Tagging_RF::Read_Tree()
{
  cout << "[Tagging_RF::Read_Tree]: Starts to read trees" << endl;

  for (auto it_map_map = map_map_tree_mc.begin(); it_map_map != map_map_tree_mc.end(); it_map_map++)
  {
    TString syst_type = it_map_map->first;
    cout << "Syst_Type = " << syst_type << endl;

    map<TString, TTree *> *map_tree = it_map_map->second;

    for (auto it = map_tree->begin(); it != map_tree->end(); it++)
    {
      cout << it->first << endl;

      int sample_index = 0;

      Long64_t n_entries = it->second->GetEntries();
      n_entries /= reduction;
      cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

      for (Long64_t i = 0; i < n_entries; i++)
      {
        if (i % 500000 == 0)
          cout << "Processing... " << i << "/" << n_entries << endl;

        it->second->GetEntry(i);

        if (mode == "Analysis")
          Fill_Histo_MC(it->first, syst_type);
        else if (mode == "Validation")
          Fill_Histo_Validation_MC(it->first, syst_type);
      } // loop over entries
    }   // loop over map_mc
  }     // loop over map_map_mc

  return;
} // void Tagging_RF::Read_Tree()

//////////

void Tagging_RF::Run_Analysis()
{
  cout << "[Tagging_RF::Run_Analysis]: Init" << endl;

  ROOT::EnableImplicitMT(20);

  Setup_Analysis();
  Setup_Binning();
  Setup_Histo();
  Read_Tree();
  // Stack_MC();
  Ratio();

  return;
} // void Tagging_RF::Run_Analysis()

//////////

void Tagging_RF::Run_Combine()
{
  Combine_Lepton_Channel();
  Combine_Decay_Mode();
  Draw_Result();

  return;
} // void Tagging_RF::Run_Combine()

//////////

void Tagging_RF::Run_Draw_Validation()
{
  Draw_Validation();

  return;
} // void Tagging_RF::Run_Draw_Validatiion()

//////////

void Tagging_RF::Run_Validation()
{
  cout << "[Tagging_RF::Run_Validation]: Init" << endl;

  ROOT::EnableImplicitMT(20);

  Setup_Application();
  Setup_Analysis();
  Setup_Histo_Validation();
  Read_Tree();
  // Draw_Validation();

  return;
} // void Tagging_RF::Run_Validation()

//////////

void Tagging_RF::Setup_Analysis()
{
  cout << "[Tagging_RF::Setup_Analysis]: Init analysis" << endl;

  reduction = 1;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/Tagging_RF/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size() || samples.map_data.size() != samples.map_short_name_data.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }
  n_sample = samples.map_mc.size();

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;

    map_fin_mc[it->first] = new TFile(path_base + it->second);

    // Central
    map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get("Central/Result_Tree");
    Setup_Tree(map_tree_mc[it->first], "Central");

    if (it->first.Contains("CP5") || it->first.Contains("hdamp") || it->first.Contains("mtop17"))
      continue;

    // JetEnDown
    map_tree_mc_jec_down[it->first] = (TTree *)map_fin_mc[it->first]->Get("JetEnDown/Result_Tree");
    Setup_Tree(map_tree_mc_jec_down[it->first], "JetEnDown");

    // JetEnUp
    map_tree_mc_jec_up[it->first] = (TTree *)map_fin_mc[it->first]->Get("JetEnUp/Result_Tree");
    Setup_Tree(map_tree_mc_jec_up[it->first], "JetEnUp");

    // JetResDown
    map_tree_mc_jer_down[it->first] = (TTree *)map_fin_mc[it->first]->Get("JetResDown/Result_Tree");
    Setup_Tree(map_tree_mc_jer_down[it->first], "JetResDown");

    // JetResUp
    map_tree_mc_jer_up[it->first] = (TTree *)map_fin_mc[it->first]->Get("JetResUp/Result_Tree");
    Setup_Tree(map_tree_mc_jer_up[it->first], "JetResUp");
  }

  // TTree handlers
  map_map_tree_mc["Central"] = &map_tree_mc;
  map_map_tree_mc["JetEnDown"] = &map_tree_mc_jec_down;
  map_map_tree_mc["JetEnUp"] = &map_tree_mc_jec_up;
  map_map_tree_mc["JetResDown"] = &map_tree_mc_jer_down;
  map_map_tree_mc["JetResUp"] = &map_tree_mc_jer_up;

  return;
} // void Tagging_RF::Setup_Analysis()

//////////

void Tagging_RF::Setup_Application()
{
  cout << "[Tagging_RF::Setup_Application]: Init" << endl;

  Setup_Binning();

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  fin = new TFile(path_base + "/Corrections/Vcb_Tagging_RF_" + era + ".root");

  // set this manually
  vec_sample_tagging_rf = {"ttV", "VV", "VJets", "ST", "QCD_bEn",
                           "TTLJ_2", "TTLJ_CC_2", "TTLJ_BB_2",
                           "TTLJ_4", "TTLJ_CC_4", "TTLJ_BB_4",
                           "TTLJ_45", "TTLJ_CC_45", "TTLJ_BB_45",
                           //"TTLJ", "TTLJ_CC", "TTLJ_BB",
                           "TTLL", "TTLL_CC", "TTLL_BB"};
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

  cout << "[Tagging_RF::Setup_Application]: Done" << endl;

  return;
} // void Tagging_RF::Setup_Application()

//////////

void Tagging_RF::Setup_Binning()
{
  bin_njet = {4, 5, 6, 7, 8, 10, 12, 14, 20};
  bin_ht = {80, 140, 180, 220, 260, 300, 350, 400, 500, 1000};
  bin_npv = {0, 15, 20, 25, 30, 35, 40, 50, 70};

  return;
} // void Tagging_RF::Setup_Binning()

//////////

void Tagging_RF::Setup_Histo()
{
  cout << "[Tagging_RF::Setup_Histo]: Init" << endl;

  histo_mc_before_b = new TH2D **[n_sample_merge_mc];
  histo_mc_after_b = new TH2D **[n_sample_merge_mc];

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
      histo_mc_before_c[i][j] = new TH2D(histo_name, histo_name, bin_npv.size() - 1, bin_npv.data(), bin_ht.size() - 1, bin_ht.data());

      histo_name = vec_short_name_mc[i] + "_" + syst_name_c[j] + "_After";
      histo_mc_after_c[i][j] = new TH2D(histo_name, histo_name, bin_npv.size() - 1, bin_npv.data(), bin_ht.size() - 1, bin_ht.data());
    }
  }

  return;
} // void Tagging_RF::Setup_Histo()

//////////

void Tagging_RF::Setup_Histo_Validation()
{
  cout << "[Tagging_RF::Setup_Histo_Validation]: Init" << endl;

  histo_closure_n_jet = new TH1D ***[n_sample_merge_mc];
  histo_closure_ht = new TH1D ***[n_sample_merge_mc];
  histo_closure_n_pv = new TH1D ***[n_sample_merge_mc];
  histo_closure_bvsc = new TH1D ***[n_sample_merge_mc];
  histo_closure_cvsb = new TH1D ***[n_sample_merge_mc];
  histo_closure_cvsl = new TH1D ***[n_sample_merge_mc];
  histo_closure_eta = new TH1D ***[n_sample_merge_mc];
  histo_closure_pt = new TH1D ***[n_sample_merge_mc];

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    histo_closure_n_jet[i] = new TH1D **[n_syst_c];
    histo_closure_ht[i] = new TH1D **[n_syst_c];
    histo_closure_n_pv[i] = new TH1D **[n_syst_c];
    histo_closure_bvsc[i] = new TH1D **[n_syst_c];
    histo_closure_cvsb[i] = new TH1D **[n_syst_c];
    histo_closure_cvsl[i] = new TH1D **[n_syst_c];
    histo_closure_eta[i] = new TH1D **[n_syst_c];
    histo_closure_pt[i] = new TH1D **[n_syst_c];

    for (int j = 0; j < n_syst_c; j++)
    {
      histo_closure_n_jet[i][j] = new TH1D *[3];
      histo_closure_ht[i][j] = new TH1D *[3];
      histo_closure_n_pv[i][j] = new TH1D *[3];
      histo_closure_bvsc[i][j] = new TH1D *[3];
      histo_closure_cvsb[i][j] = new TH1D *[3];
      histo_closure_cvsl[i][j] = new TH1D *[3];
      histo_closure_eta[i][j] = new TH1D *[3];
      histo_closure_pt[i][j] = new TH1D *[3];

      for (int k = 0; k < 3; k++)
      {
        TString histo_name_base = "Closure_" + vec_short_name_mc[i] + "_" + syst_name_c[j];

        TString histo_name = histo_name_base + "_N_Jet_" + to_string(k);
        histo_closure_n_jet[i][j][k] = new TH1D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data());

        histo_name = histo_name_base + "_HT_" + to_string(k);
        histo_closure_ht[i][j][k] = new TH1D(histo_name, histo_name, bin_ht.size() - 1, bin_ht.data());

        histo_name = histo_name_base + "_N_PV_" + to_string(k);
        histo_closure_n_pv[i][j][k] = new TH1D(histo_name, histo_name, bin_npv.size() - 1, bin_npv.data());

        histo_name = histo_name_base + "_Leading_Jet_BvsC_" + to_string(k);
        histo_closure_bvsc[i][j][k] = new TH1D(histo_name, histo_name, 100, 0, 1);

        histo_name = histo_name_base + "_Leading_Jet_CvsB_" + to_string(k);
        histo_closure_cvsb[i][j][k] = new TH1D(histo_name, histo_name, 100, 0, 1);

        histo_name = histo_name_base + "_Leading_Jet_CvsL_" + to_string(k);
        histo_closure_cvsl[i][j][k] = new TH1D(histo_name, histo_name, 100, 0, 1);

        histo_name = histo_name_base + "_Leading_Jet_Eta_" + to_string(k);
        histo_closure_eta[i][j][k] = new TH1D(histo_name, histo_name, 60, -3, 3);

        histo_name = histo_name_base + "_Leading_Jet_Pt_" + to_string(k);
        histo_closure_pt[i][j][k] = new TH1D(histo_name, histo_name, 100, 0, 500);
      }
    }
  }

  cout << "[Tagging_RF::Setup_Histo_Validation]: Done" << endl;

  return;
} // void Tagging_RF::Setup_Histo_Validation()

//////////

void Tagging_RF::Setup_Tree(TTree *tree, const TString &syst)
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
  else if (syst == "JetEnDown")
    tree->SetBranchAddress("weight_c_tag_down_jes_total", &weight_c_tag_jes_total_down);
  else if (syst == "JetEnUp")
    tree->SetBranchAddress("weight_c_tag_up_jes_total", &weight_c_tag_jes_total_up);
  else if (syst == "JetResDown")
    tree->SetBranchAddress("weight_c_tag_down_jer", &weight_c_tag_jer_down);
  else if (syst == "JetResUp")
    tree->SetBranchAddress("weight_c_tag_up_jer", &weight_c_tag_jer_up);

  tree->SetBranchAddress("n_jets", &n_jets);
  tree->SetBranchAddress("ht", &ht);
  tree->SetBranchAddress("n_vertex", &n_pv);

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

  tree->SetBranchAddress("decay_mode", &decay_mode);

  tree->SetBranchAddress("Gen_HF_Flavour", &vec_gen_hf_flavour);
  tree->SetBranchAddress("Gen_HF_Origin", &vec_gen_hf_origin);
  tree->SetBranchAddress("Sel_Gen_HF_Flavour", &vec_sel_gen_hf_flavour);
  tree->SetBranchAddress("Sel_Gen_HF_Origin", &vec_sel_gen_hf_origin);

  return;
} // void Tagging_RF::Setup_Tree()

//////////
/*
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
*/
//////////