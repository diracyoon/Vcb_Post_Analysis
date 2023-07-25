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

  if (mode == "Analysis")
    Run_Analysis();
  else if (mode == "Combine")
    Run_Combine();
  else if (mode == "Validation")
    Run_Validation();
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

      histo_mc_before_b[i]->Write();
      histo_mc_before_c[i]->Write();

      for (int j = 0; j < n_syst_b; j++)
        histo_mc_after_b[i][j]->Write();
      for (int j = 0; j < n_syst_c; j++)
        histo_mc_after_c[i][j]->Write();

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

      for (int j = 0; j < 3; j++)
      {
        dir->cd();

        histo_closure_n_jet[i][j]->Write();
        histo_closure_ht[i][j]->Write();
        histo_closure_n_pv[i][j]->Write();

        histo_closure_bvsc[i][j]->Write();
        histo_closure_cvsb[i][j]->Write();
        histo_closure_cvsl[i][j]->Write();

        histo_closure_eta[i][j]->Write();
        histo_closure_pt[i][j]->Write();
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

float Tagging_RF::Get_Tagging_RF_C_Tag(const TString &sample, const TString &syst, const int &n_pv, const float &ht)
{
  if (!syst.Contains("C_Tag"))
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Input C_Tag" << endl;
    exit(1);
  }

  int sample_index = find(sample_name.begin(), sample_name.end(), sample) - sample_name.begin();
  int syst_index = find(syst_name_c.begin(), syst_name_c.end(), syst) - syst_name_c.begin();

  if (sample_index == sample_name.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_c.size())
  {
    cout << "[Tagging_RF::Get_Tagging_RF] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  int bin = ratio_c[sample_index][syst_index]->FindBin(n_pv, ht);
  return ratio_c[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF::Get_Tagging_RF_C_Tag(const TString &syst_name, const int &n_pv, const float& ht)

//////////

void Tagging_RF::Combine()
{
  cout << "[Tagging_RF::Combine]: Init" << endl;

  fin_mu = new TFile("Vcb_Tagging_RF_" + era + "_Mu.root");
  fin_el = new TFile("Vcb_Tagging_RF_" + era + "_El.root");

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
        cout << dir_name << " " << key_name << " " << histo_name << endl;

        TH1 *ratio_mu;
        TH1 *ratio_el;
        TH1 *ratio_averaged;

        // B-Tag
        if (key_name.Contains("B_Tag"))
        {
          ratio_mu = (TH2D *)fin_mu->Get(histo_name);
          ratio_el = (TH2D *)fin_el->Get(histo_name);

          ratio_mu->SetBit(TH2::kIsAverage);
          ratio_el->SetBit(TH2::kIsAverage);

          ratio_averaged = (TH2D *)ratio_mu->Clone();
        }
        // C-Tag
        else if (key_name.Contains("C_Tag"))
        {
          ratio_mu = (TH2D *)fin_mu->Get(histo_name);
          ratio_el = (TH2D *)fin_el->Get(histo_name);

          ratio_mu->SetBit(TH1::kIsAverage);
          ratio_el->SetBit(TH1::kIsAverage);

          ratio_averaged = (TH2D *)ratio_mu->Clone();
        }

        cout << "KS test result: " << ratio_mu->KolmogorovTest(ratio_el) << endl;

        ratio_averaged->SetBit(TH1::kIsAverage);
        ratio_averaged->Add(ratio_el);

        dir_fout->cd();
        ratio_averaged->Write();
      }
    }
  } // loop over mc_merged

  return;
} // void Tagging_RF::Combine()

//////////

void Tagging_RF::Draw_Result()
{
  cout << "[Tagging_RF::Draw_Result]: Init" << endl;

  gStyle->SetPaintTextFormat("0.3f");
  gStyle->SetOptStat(0);

  // draw results
  vector<TString> sample_to_draw = {"TTLJ", "TTLL"};
  vector<TString> syst_to_draw = {
      "C_Tag_Nominal",
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
      "C_Tag_JES_Total_Down", "C_Tag_JES_Total_Up"
  };

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
      histo->Draw("COLZ texte");

      canvas->Print(can_name + "." + extension, extension);
    }
  }

  // compare TTbar syst
  for (unsigned int i = 0; i < sample_to_draw.size(); i++)
  {
    TH2D *histo[7];
    histo[0] = (TH2D *)fout->Get(Form("%s/Ratio_%s_C_Tag_Nominal", sample_to_draw[i].Data(), sample_to_draw[i].Data()));
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

  /*
    canvas_ratio_b = new TCanvas **[n_sample_merge_mc];
    canvas_ratio_c = new TCanvas **[n_sample_merge_mc];

    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      canvas_ratio_b[i] = new TCanvas *[n_syst_b];
      for (int j = 0; j < n_syst_b; j++)
      {
        TString canvas_name = "Canvas_Ratio_" + vec_short_name_mc[i] + "_" + syst_name_b[j] + "_" + era;
        canvas_ratio_b[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
        canvas_ratio_b[i][j]->Draw();

        ratio_b[i][j]->GetXaxis()->SetTitle("Number of Jets");
        ratio_b[i][j]->GetYaxis()->SetTitle("HT");
        ratio_b[i][j]->Draw("COLZ texte");

        canvas_ratio_b[i][j]->Print(canvas_name + "." + extension, extension);
      }

      canvas_ratio_c[i] = new TCanvas *[n_syst_c];
      for (int j = 0; j < n_syst_c; j++)
      {
        TString canvas_name = "Canvas_Ratio_" + vec_short_name_mc[i] + "_" + syst_name_c[j] + "_" + era;
        canvas_ratio_c[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
        canvas_ratio_c[i][j]->Draw();

        ratio_c[i][j]->GetXaxis()->SetTitle("Number of Primary Vertext");
        ratio_c[i][j]->GetYaxis()->SetTitle("HT");
        ratio_c[i][j]->Draw();

        canvas_ratio_c[i][j]->Print(canvas_name + "." + extension, extension);
      }
    }
  */
  return;
} // void Tagging_RF::Draw_Result()

//////////

void Tagging_RF::Draw_Validation()
{
  cout << "Tagging_RF::Draw_Validation" << endl;

  return;
} // void Tagging_RF::Draw_Validation()

//////////

void Tagging_RF::Fill_Histo_MC(const int &sample_index)
{
  float weight = 1;

  weight *= weight_lumi;
  weight *= weight_mc;
  weight *= weight_pileup;
  weight *= weight_prefire;
  weight *= weight_top_pt;

  weight *= sf_sl_trig;

  if (channel == "Mu")
  {
    weight *= sf_mu_id;
    weight *= sf_mu_iso;
  }
  else if (channel = "El")
  {
    weight *= sf_el_id;
    weight *= sf_el_reco;
  }

  // cout << weight_lumi << " " << weight_mc << " " << weight_pileup << " " << weight_prefire << " " << weight_top_pt << " " << sf_sl_trig << " " << sf_el_id << " " << sf_el_reco << endl;
  if (TMath::IsNaN(weight))
  {
    cout << "Nan detected" << weight_lumi << " " << weight_mc << " " << weight_pileup << " " << weight_prefire << " " << weight_top_pt << " " << sf_sl_trig << " " << sf_el_id << " " << sf_el_reco << endl;
  }

  histo_mc_before_b[sample_index]->Fill(n_jets, ht, weight);
  histo_mc_after_b[sample_index][0]->Fill(n_jets, ht, weight * weight_b_tag);
  histo_mc_after_b[sample_index][1]->Fill(n_jets, ht, weight * weight_b_tag_hf_down);
  histo_mc_after_b[sample_index][2]->Fill(n_jets, ht, weight * weight_b_tag_hf_up);
  histo_mc_after_b[sample_index][3]->Fill(n_jets, ht, weight * weight_b_tag_jes_down);
  histo_mc_after_b[sample_index][4]->Fill(n_jets, ht, weight * weight_b_tag_jes_up);
  histo_mc_after_b[sample_index][5]->Fill(n_jets, ht, weight * weight_b_tag_lfstats1_down);
  histo_mc_after_b[sample_index][6]->Fill(n_jets, ht, weight * weight_b_tag_lfstats1_up);
  histo_mc_after_b[sample_index][7]->Fill(n_jets, ht, weight * weight_b_tag_lfstats2_down);
  histo_mc_after_b[sample_index][8]->Fill(n_jets, ht, weight * weight_b_tag_lfstats2_up);
  histo_mc_after_b[sample_index][9]->Fill(n_jets, ht, weight * weight_b_tag_cferr1_down);
  histo_mc_after_b[sample_index][10]->Fill(n_jets, ht, weight * weight_b_tag_cferr1_up);
  histo_mc_after_b[sample_index][11]->Fill(n_jets, ht, weight * weight_b_tag_cferr2_down);
  histo_mc_after_b[sample_index][12]->Fill(n_jets, ht, weight * weight_b_tag_cferr2_up);
  histo_mc_after_b[sample_index][13]->Fill(n_jets, ht, weight * weight_b_tag_hfstats1_down);
  histo_mc_after_b[sample_index][14]->Fill(n_jets, ht, weight * weight_b_tag_hfstats1_up);
  histo_mc_after_b[sample_index][15]->Fill(n_jets, ht, weight * weight_b_tag_hfstats2_down);
  histo_mc_after_b[sample_index][16]->Fill(n_jets, ht, weight * weight_b_tag_hfstats2_up);

  histo_mc_before_c[sample_index]->Fill(n_pv, ht, weight);
  histo_mc_after_c[sample_index][0]->Fill(n_pv, ht, weight * weight_c_tag);
  histo_mc_after_c[sample_index][1]->Fill(n_pv, ht, weight * weight_c_tag_extrap_down);
  histo_mc_after_c[sample_index][2]->Fill(n_pv, ht, weight * weight_c_tag_extrap_up);
  histo_mc_after_c[sample_index][3]->Fill(n_pv, ht, weight * weight_c_tag_interp_down);
  histo_mc_after_c[sample_index][4]->Fill(n_pv, ht, weight * weight_c_tag_interp_up);
  histo_mc_after_c[sample_index][5]->Fill(n_pv, ht, weight * weight_c_tag_lhe_scale_muf_down);
  histo_mc_after_c[sample_index][6]->Fill(n_pv, ht, weight * weight_c_tag_lhe_scale_muf_up);
  histo_mc_after_c[sample_index][7]->Fill(n_pv, ht, weight * weight_c_tag_lhe_scale_mur_down);
  histo_mc_after_c[sample_index][8]->Fill(n_pv, ht, weight * weight_c_tag_lhe_scale_mur_up);
  histo_mc_after_c[sample_index][9]->Fill(n_pv, ht, weight * weight_c_tag_ps_fsr_fixed_down);
  histo_mc_after_c[sample_index][10]->Fill(n_pv, ht, weight * weight_c_tag_ps_fsr_fixed_up);
  histo_mc_after_c[sample_index][11]->Fill(n_pv, ht, weight * weight_c_tag_ps_isr_fixed_down);
  histo_mc_after_c[sample_index][12]->Fill(n_pv, ht, weight * weight_c_tag_ps_isr_fixed_up);
  histo_mc_after_c[sample_index][13]->Fill(n_pv, ht, weight * weight_c_tag_pu_down);
  histo_mc_after_c[sample_index][14]->Fill(n_pv, ht, weight * weight_c_tag_pu_up);
  histo_mc_after_c[sample_index][15]->Fill(n_pv, ht, weight * weight_c_tag_stat_down);
  histo_mc_after_c[sample_index][16]->Fill(n_pv, ht, weight * weight_c_tag_stat_up);
  histo_mc_after_c[sample_index][17]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_b_down);
  histo_mc_after_c[sample_index][18]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_b_up);
  histo_mc_after_c[sample_index][19]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_c_down);
  histo_mc_after_c[sample_index][20]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_dyjets_c_up);
  histo_mc_after_c[sample_index][21]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_wjets_c_down);
  histo_mc_after_c[sample_index][22]->Fill(n_pv, ht, weight * weight_c_tag_xsec_br_unc_wjets_c_up);
  histo_mc_after_c[sample_index][23]->Fill(n_pv, ht, weight * weight_c_tag_jer_down);
  histo_mc_after_c[sample_index][24]->Fill(n_pv, ht, weight * weight_c_tag_jer_up);
  histo_mc_after_c[sample_index][25]->Fill(n_pv, ht, weight * weight_c_tag_jes_total_down);
  histo_mc_after_c[sample_index][26]->Fill(n_pv, ht, weight * weight_c_tag_jes_total_up);

  return;
} // void Tagging_RF::Fill_Histo_MC()

//////////

void Tagging_RF::Fill_Histo_Validation_MC(const int &sample_index)
{
  float weight_raw = 1;

  weight_raw *= weight_lumi;
  weight_raw *= weight_mc;
  weight_raw *= weight_pileup;
  weight_raw *= weight_prefire;
  weight_raw *= weight_top_pt;

  weight_raw *= sf_sl_trig;

  if (channel == "Mu")
  {
    weight_raw *= sf_mu_id;
    weight_raw *= sf_mu_iso;
  }
  else if (channel = "El")
  {
    weight_raw *= sf_el_id;
    weight_raw *= sf_el_reco;
  }

  float b_rf = Get_Tagging_RF_B_Tag(vec_short_name_mc[sample_index], "B_Tag_Nominal", n_jets, ht);
  float c_rf = Get_Tagging_RF_C_Tag(vec_short_name_mc[sample_index], "C_Tag_Nominal", n_pv, ht);

  // float weight_sf = weight_raw * weight_b_tag;
  // float weight_rf = weight_sf * b_rf;

  float weight_sf = weight_raw * weight_c_tag;
  float weight_rf = weight_sf * c_rf * b_rf;

  for (int i = 0; i < 3; i++)
  {
    float weight;
    if (i == 0)
      weight = weight_raw;
    else if (i == 1)
      weight = weight_sf;
    else if (i == 2)
      weight = weight_rf;

    histo_closure_n_jet[sample_index][i]->Fill(n_jets, weight);
    histo_closure_ht[sample_index][i]->Fill(ht, weight);
    histo_closure_n_pv[sample_index][i]->Fill(n_pv, weight);

    histo_closure_bvsc[sample_index][i]->Fill(leading_jet_bvsc, weight);
    histo_closure_cvsb[sample_index][i]->Fill(leading_jet_cvsb, weight);
    histo_closure_cvsl[sample_index][i]->Fill(leading_jet_cvsl, weight);

    histo_closure_eta[sample_index][i]->Fill(leading_jet_eta, weight);
    histo_closure_pt[sample_index][i]->Fill(leading_jet_pt, weight);
  }

  return;
} // void Tagging_RF::Fill_Histo_Validation_MC(const int& sample_index)

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

      ratio_b[i][j]->Divide(histo_mc_before_b[i], histo_mc_after_b[i][j]);
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

      ratio_c[i][j]->Divide(histo_mc_before_c[i], histo_mc_after_c[i][j]);
    }
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

      if (mode == "Analysis")
        Fill_Histo_MC(sample_index);
      else if (mode == "Validation")
        Fill_Histo_Validation_MC(sample_index);
    } // loop over entries
  }   // loop over map_mc

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
  Combine();
  Draw_Result();

  return;
} // void Tagging_RF::Run_Combine()

//////////

void Tagging_RF::Run_Validation()
{
  cout << "[Tagging_RF::Run_Validation]: Init" << endl;

  ROOT::EnableImplicitMT(4);

  Setup_Application();
  Setup_Analysis();
  Setup_Binning();
  Setup_Histo_Validation();
  Read_Tree();
  Draw_Validation();

  return;
} // void Tagging_RF::Run_Validation()

//////////

void Tagging_RF::Setup_Analysis()
{
  cout << "[Tagging_RF::Setup_Analysis]: Init analysis" << endl;

  reduction = 1000;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/Tagging_RF/";

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
    map_tree_mc[it->first] = (TTree *)map_fin_mc[it->first]->Get("Result_Tree");

    Setup_Tree(map_tree_mc[it->first]);
  }

  // number of merged MC
  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  return;
} // void Tagging_RF::Setup_Analysis()

//////////

void Tagging_RF::Setup_Application()
{
  cout << "[Tagging_RF::Setup_Application]" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  fin = new TFile(path_base + "/Corrections/Vcb_Tagging_RF_" + era + ".root");

  TList *list_dir = fin->GetListOfKeys();

  n_syst_b = 0;
  n_syst_c = 0;

  for (int i = 0; i < list_dir->GetEntries(); i++)
  {
    TObject *obj = list_dir->At(i);
    TString dir_name = obj->GetName();

    sample_name.push_back(dir_name);

    if (i == 0)
    {
      TList *key_list = ((TDirectory *)fin->Get(dir_name))->GetListOfKeys();

      for (int j = 0; j < key_list->GetEntries(); j++)
      {
        TObject *key = key_list->At(j);
        TString name = key->GetName();

        if (name.Contains("B_Tag_"))
        {
          int index = name.First('_');
          name = name.Remove(0, index + 1);

          index = name.First('_');
          name = name.Remove(0, index + 1);

          syst_name_b.push_back(name);
        }
        else if (name.Contains("C_Tag_"))
        {
          int index = name.First('_');
          name = name.Remove(0, index + 1);

          index = name.First('_');
          name = name.Remove(0, index + 1);

          syst_name_c.push_back(name);
        }
      }
    }
  }

  n_sample = sample_name.size();
  n_syst_b = syst_name_b.size();
  n_syst_c = syst_name_c.size();

  // cout << "n_syst_b = " << n_syst_b << ", n_syst_c = " << n_syst_c << endl;

  ratio_b = new TH2D **[n_sample];
  ratio_c = new TH2D **[n_sample];
  for (int i = 0; i < n_sample; i++)
  {
    ratio_b[i] = new TH2D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      TString histo_name = sample_name[i] + "/Ratio_" + sample_name[i] + "_" + syst_name_b[j];
      ratio_b[i][j] = (TH2D *)fin->Get(histo_name);
    }

    ratio_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      TString histo_name = sample_name[i] + "/Ratio_" + sample_name[i] + "_" + syst_name_c[j];
      ratio_c[i][j] = (TH2D *)fin->Get(histo_name);
    }
  }

  return;
} // void Tagging_RF::Setup_Application()

//////////

void Tagging_RF::Setup_Binning()
{
  bin_njet = {4, 5, 6, 7, 8, 10, 12, 14, 20};
  bin_ht = {80, 100, 120, 140, 160, 200, 250, 300, 350, 400, 500, 600};
  bin_npv = {0, 10, 15, 20, 25, 28, 30, 32, 35, 40, 60};

  return;
} // void Tagging_RF::Setup_Binning()

//////////

void Tagging_RF::Setup_Histo()
{
  histo_mc_before_b = new TH2D *[n_sample_merge_mc];
  histo_mc_after_b = new TH2D **[n_sample_merge_mc];

  histo_mc_before_c = new TH2D *[n_sample_merge_mc];
  histo_mc_after_c = new TH2D **[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString histo_name = vec_short_name_mc[i] + "_Before_B_Tag";
    histo_mc_before_b[i] = new TH2D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data(), bin_ht.size() - 1, bin_ht.data());

    histo_mc_after_b[i] = new TH2D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      histo_name = vec_short_name_mc[i] + "_After_" + syst_name_b[j];
      histo_mc_after_b[i][j] = new TH2D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data(), bin_ht.size() - 1, bin_ht.data());
    }

    histo_name = vec_short_name_mc[i] + "_Before_C_Tag";
    histo_mc_before_c[i] = new TH2D(histo_name, histo_name, bin_npv.size() - 1, bin_npv.data(), bin_ht.size() - 1, bin_ht.data());

    histo_mc_after_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      histo_name = vec_short_name_mc[i] + "_After_" + syst_name_c[j];
      histo_mc_after_c[i][j] = new TH2D(histo_name, histo_name, bin_npv.size() - 1, bin_npv.data(), bin_ht.size() - 1, bin_ht.data());
    }
  }

  return;
} // void Tagging_RF::Setup_Histo()

//////////

void Tagging_RF::Setup_Histo_Validation()
{
  histo_closure_n_jet = new TH1D **[n_sample_merge_mc];
  histo_closure_ht = new TH1D **[n_sample_merge_mc];
  histo_closure_n_pv = new TH1D **[n_sample_merge_mc];
  histo_closure_bvsc = new TH1D **[n_sample_merge_mc];
  histo_closure_cvsb = new TH1D **[n_sample_merge_mc];
  histo_closure_cvsl = new TH1D **[n_sample_merge_mc];
  histo_closure_eta = new TH1D **[n_sample_merge_mc];
  histo_closure_pt = new TH1D **[n_sample_merge_mc];

  for (int i = 0; i < n_sample_merge_mc; i++)
  {

    TString histo_name_base = "Closure_" + vec_short_name_mc[i];

    histo_closure_n_jet[i] = new TH1D *[3];
    histo_closure_ht[i] = new TH1D *[3];
    histo_closure_n_pv[i] = new TH1D *[3];
    histo_closure_bvsc[i] = new TH1D *[3];
    histo_closure_cvsb[i] = new TH1D *[3];
    histo_closure_cvsl[i] = new TH1D *[3];
    histo_closure_eta[i] = new TH1D *[3];
    histo_closure_pt[i] = new TH1D *[3];

    for (int j = 0; j < 3; j++)
    {
      TString histo_name = histo_name_base + "_N_Jet_" + to_string(j);
      histo_closure_n_jet[i][j] = new TH1D(histo_name, histo_name, bin_njet.size() - 1, bin_njet.data());

      histo_name = histo_name_base + "_HT_" + to_string(j);
      histo_closure_ht[i][j] = new TH1D(histo_name, histo_name, bin_ht.size() - 1, bin_ht.data());

      histo_name = histo_name_base + "_N_PV_" + to_string(j);
      histo_closure_n_pv[i][j] = new TH1D(histo_name, histo_name, bin_npv.size() - 1, bin_npv.data());

      histo_name = histo_name_base + "_Leading_Jet_BvsC_" + to_string(j);
      histo_closure_bvsc[i][j] = new TH1D(histo_name, histo_name, 100, 0, 1);

      histo_name = histo_name_base + "_Leading_Jet_CvsB_" + to_string(j);
      histo_closure_cvsb[i][j] = new TH1D(histo_name, histo_name, 100, 0, 1);

      histo_name = histo_name_base + "_Leading_Jet_CvsL_" + to_string(j);
      histo_closure_cvsl[i][j] = new TH1D(histo_name, histo_name, 100, 0, 1);

      histo_name = histo_name_base + "_Leading_Jet_Eta_" + to_string(j);
      histo_closure_eta[i][j] = new TH1D(histo_name, histo_name, 60, -3, 3);

      histo_name = histo_name_base + "_Leading_Jet_Pt_" + to_string(j);
      histo_closure_pt[i][j] = new TH1D(histo_name, histo_name, 100, 0, 500);
    }
  }

  return;
} // void Tagging_RF::Setup_Histo_Validation()

//////////

void Tagging_RF::Setup_Tree(TTree *tree)
{
  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);
  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);
  tree->SetBranchAddress("weight_pujet_veto", &weight_pujet_veto);

  tree->SetBranchAddress("sf_mu_id", &sf_mu_id);
  tree->SetBranchAddress("sf_mu_iso", &sf_mu_iso);
  tree->SetBranchAddress("sf_el_id", &sf_el_id);
  tree->SetBranchAddress("sf_el_reco", &sf_el_reco);

  tree->SetBranchAddress("sf_sl_trig", &sf_sl_trig);

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