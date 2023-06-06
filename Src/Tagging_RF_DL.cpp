#include "Tagging_RF_DL.h"

ClassImp(Tagging_RF_DL);

//////////

Tagging_RF_DL::Tagging_RF_DL(const TString &a_era, const TString &a_mode, const TString &a_channel, const TString &a_extension) : samples(a_era, a_channel, "Vcb_Tagging_RF_DL")
{
  ROOT::EnableImplicitMT(4);

  cout << "[Tagging_RF_DL::Tagging_RF_DL]: Init analysis" << endl;

  era = a_era;
  mode = a_mode;
  channel = a_channel;
  extension = a_extension;

  if (mode == "Analysis")
    Run_Analysis();
  else if (mode == "Combine")
    Combine();
  else if (mode == "Validation")
    Run_Validation();
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
    TString fout_name = "Vcb_Tagging_RF_DL_" + era + "_" + channel + ".root";
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
    TString fout_name = "Vcb_Tagging_RF_DL_Validation_" + era + "_" + channel + ".root";
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
} // Tagging_RF_DL::~Tagging_RF_DL()

//////////

float Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag(const TString &sample, const TString &syst, const int &n_jet, const float &ht)
{
  if (!syst.Contains("B_Tag"))
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL] Input B_Tag" << endl;
    exit(1);

    return -1;
  }

  int sample_index = find(sample_name.begin(), sample_name.end(), sample) - sample_name.begin();
  int syst_index = find(syst_name_b.begin(), syst_name_b.end(), syst) - syst_name_b.begin();

  if (sample_index == sample_name.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_b.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  int bin = ratio_b[sample_index][syst_index]->FindBin(n_jet, ht);
  return ratio_b[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF_DL::Get_Tagging_RF_DL_B_Tag(const TString &syst_name, const int &n_jet)

//////////

float Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag(const TString &sample, const TString &syst, const int &n_pv, const float &ht)
{
  if (!syst.Contains("C_Tag"))
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL] Input C_Tag" << endl;
    exit(1);
  }

  int sample_index = find(sample_name.begin(), sample_name.end(), sample) - sample_name.begin();
  int syst_index = find(syst_name_c.begin(), syst_name_c.end(), syst) - syst_name_c.begin();

  if (sample_index == sample_name.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL] Can not find " << sample << " Check it." << endl;
    exit(1);
  }

  if (syst_index == syst_name_c.size())
  {
    cout << "[Tagging_RF_DL::Get_Tagging_RF_DL] Can not find " << syst << " Check it." << endl;
    exit(1);
  }

  int bin = ratio_c[sample_index][syst_index]->FindBin(n_pv, ht);
  return ratio_c[sample_index][syst_index]->GetBinContent(bin);
} // float Tagging_RF_DL::Get_Tagging_RF_DL_C_Tag(const TString &syst_name, const int &n_pv, const float& ht)

//////////

void Tagging_RF_DL::Combine()
{
  cout << "[Tagging_RF_DL::Combine]: Init" << endl;

  fin_mm = new TFile("Vcb_Tagging_RF_DL_" + era + "_MM.root");
  fin_me = new TFile("Vcb_Tagging_RF_DL_" + era + "_ME.root");
  fin_ee = new TFile("Vcb_Tagging_RF_DL_" + era + "_EE.root");

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

        TH1 *ratio_mm;
        TH1 *ratio_me;
        TH1 *ratio_ee;
        TH1 *ratio_averaged;

        // B-Tag
        if (key_name.Contains("B_Tag"))
        {
          ratio_mm = (TH2D *)fin_mm->Get(histo_name);
          ratio_me = (TH2D *)fin_me->Get(histo_name);
          ratio_ee = (TH2D *)fin_ee->Get(histo_name);

          ratio_mm->SetBit(TH2::kIsAverage);
          ratio_me->SetBit(TH2::kIsAverage);
          ratio_ee->SetBit(TH2::kIsAverage);

          ratio_averaged = (TH2D *)ratio_mm->Clone();
        }

        // C-Tag
        else if (key_name.Contains("C_Tag"))
        {
          ratio_mm = (TH1D *)fin_mm->Get(histo_name);
          ratio_me = (TH1D *)fin_me->Get(histo_name);
          ratio_ee = (TH1D *)fin_ee->Get(histo_name);

          ratio_mm->SetBit(TH1::kIsAverage);
          ratio_me->SetBit(TH1::kIsAverage);
          ratio_ee->SetBit(TH1::kIsAverage);

          ratio_averaged = (TH1D *)ratio_mm->Clone();
        }

        cout << "KS test result (MM and EE): " << ratio_mm->KolmogorovTest(ratio_ee) << endl;
        cout << "KS test result (MM and ME): " << ratio_mm->KolmogorovTest(ratio_me) << endl;
        cout << "KS test result (EE and ME): " << ratio_ee->KolmogorovTest(ratio_me) << endl;

        ratio_averaged->SetBit(TH1::kIsAverage);
        ratio_averaged->Add(ratio_ee);
        ratio_averaged->Add(ratio_me);

        dir_fout->cd();
        ratio_averaged->Write();
      } // if (key_name.Contains("Ratio_"))
    }   // loop over key entries
  }     // loop over dir entries

  Combine_TT();

  fout->Close();

  return;
} // void Tagging_RF_DL::Combine()

//////////

void Tagging_RF_DL::Combine_TT()
{
  cout << "Tagging_RF_DL::Combine_TT" << endl;

  TDirectory *dir_ttlj = fout->mkdir("TTLJ_HF");
  TDirectory *dir_ttll = fout->mkdir("TTLL_HF");

  TDirectory *dir_jj = (TDirectory *)fout->Get("TTLL_JJ");
  TList *list = dir_jj->GetListOfKeys();
  for (int i = 0; i < list->GetEntries(); i++)
  {
    TObject *key = (TObject *)list->At(i);
    TString key_name = key->GetName();

    cout << key_name << endl;

    TString temp = key_name;
    int first = temp.First('_');
    temp.Remove(0, first + 1);

    int second = temp.First('_');
    temp.Remove(0, second + 1);

    int third = temp.First('_');
    TString syst = temp.Remove(0, third + 1);

    for (int j = 0; j < 2; j++)
    {
      TString species;
      if (j == 0)
        species = "TTLJ";
      else
        species = "TTLL";

      cout << species << " " << syst << endl;

      TH1 *ratio_jj;
      TH1 *ratio_cc;
      TH1 *ratio_bb;

      if (syst.Contains("B_Tag"))
      {
        ratio_jj = (TH2D *)fout->Get(species + "_JJ/Ratio_" + species + "_JJ_" + syst)->Clone();
        ratio_cc = (TH2D *)fout->Get(species + "_CC/Ratio_" + species + "_CC_" + syst)->Clone();
        ratio_bb = (TH2D *)fout->Get(species + "_BB/Ratio_" + species + "_BB_" + syst)->Clone();
      }
      else if (syst.Contains("C_Tag"))
      {
        // ratio_jj = (TH1D *)fout->Get(species + "_JJ/Ratio_" + species + "_JJ_" + syst)->Clone();
        // ratio_cc = (TH1D *)fout->Get(species + "_CC/Ratio_" + species + "_CC_" + syst)->Clone();
        // ratio_bb = (TH1D *)fout->Get(species + "_BB/Ratio_" + species + "_BB_" + syst)->Clone();
        ratio_jj = (TH2D *)fout->Get(species + "_JJ/Ratio_" + species + "_JJ_" + syst)->Clone();
        ratio_cc = (TH2D *)fout->Get(species + "_CC/Ratio_" + species + "_CC_" + syst)->Clone();
        ratio_bb = (TH2D *)fout->Get(species + "_BB/Ratio_" + species + "_BB_" + syst)->Clone();
      }

      cout << "KS test result (JJ and CC): " << ratio_jj->KolmogorovTest(ratio_cc) << endl;
      cout << "KS test result (JJ and BB): " << ratio_jj->KolmogorovTest(ratio_bb) << endl;
      cout << "KS test result (CC and BB): " << ratio_cc->KolmogorovTest(ratio_bb) << endl;

      ratio_jj->SetBit(TH1::kIsAverage);
      ratio_cc->SetBit(TH1::kIsAverage);
      ratio_bb->SetBit(TH1::kIsAverage);

      // Average all
      //  TH1 *ratio_averaged = ratio_jj;
      //  ratio_averaged->Add(ratio_cc);
      //  ratio_averaged->Add(ratio_bb);

      // Average TTBB+TTCC
      TH1 *ratio_averaged = ratio_cc;
      ratio_averaged->Add(ratio_bb);

      if (species == "TTLJ")
      {
        TString name = "Ratio_TTLJ_HF_" + syst;
        ratio_averaged->SetName(name);
        ratio_averaged->SetTitle(name);

        dir_ttlj->cd();
        ratio_averaged->Write();
      }
      else if (species == "TTLL")
      {
        TString name = "Ratio_TTLL_HF_" + syst;
        ratio_averaged->SetName(name);
        ratio_averaged->SetTitle(name);

        dir_ttll->cd();
        ratio_averaged->Write();
      }
    }
  }

  return;
} // void Tagging_RF_DL::Combine_TT()

//////////

void Tagging_RF_DL::Draw()
{
  cout << "[Tagging_RF_DL::Draw]: Init" << endl;
  chk_draw_called = true;

  // legend = new TLegend(0.65, 0.65, 0.85, 0.85);
  // for (int i = 0; i < n_sample_merge_mc; i++)
  // {
  //   legend->AddEntry(histo_mc_before_b[i], vec_short_name_mc[i], "f");
  // }
  // legend->SetBorderSize(0);

  canvas_before_b = new TCanvas *[n_sample_merge_mc];
  canvas_before_c = new TCanvas *[n_sample_merge_mc];
  canvas_after_b = new TCanvas **[n_sample_merge_mc];
  canvas_after_c = new TCanvas **[n_sample_merge_mc];
  canvas_ratio_b = new TCanvas **[n_sample_merge_mc];
  canvas_ratio_c = new TCanvas **[n_sample_merge_mc];

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString canvas_name = "Canvas_Before_B_Tag_" + vec_short_name_mc[i];
    canvas_before_b[i] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
    canvas_before_b[i]->Draw();
    histo_mc_before_b[i]->Draw("COLZ Texte");
    canvas_before_b[i]->Print(canvas_name + "." + extension, extension);

    canvas_name = "Canvas_Before_C_Tag" + vec_short_name_mc[i];
    canvas_before_c[i] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
    canvas_before_c[i]->Draw();
    histo_mc_before_c[i]->Draw();
    canvas_before_c[i]->Print(canvas_name + "." + extension, extension);

    canvas_after_b[i] = new TCanvas *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      canvas_name = "Canvas_After_" + vec_short_name_mc[i] + "_" + syst_name_b[j];
      canvas_after_b[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas_after_b[i][j]->Draw();
      histo_mc_after_b[i][j]->Draw("COLZ Texte");
      canvas_after_b[i][j]->Print(canvas_name + "." + extension, extension);
    }

    canvas_after_c[i] = new TCanvas *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      canvas_name = "Canvas_After_" + vec_short_name_mc[i] + "_" + syst_name_c[j];
      canvas_after_c[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas_after_c[i][j]->Draw();
      histo_mc_after_c[i][j]->Draw();
      canvas_after_c[i][j]->Print(canvas_name + "." + extension, extension);
    }

    canvas_ratio_b[i] = new TCanvas *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      canvas_name = "Canvas_Ratio_" + vec_short_name_mc[i] + "_" + syst_name_b[j];
      canvas_ratio_b[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas_ratio_b[i][j]->Draw();
      ratio_b[i][j]->Draw("COLZ texte");
      canvas_ratio_b[i][j]->Print(canvas_name + "." + extension, extension);
    }

    canvas_ratio_c[i] = new TCanvas *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      canvas_name = "Canvas_Ratio_" + vec_short_name_mc[i] + "_" + syst_name_c[j];
      canvas_ratio_c[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas_ratio_c[i][j]->Draw();
      ratio_c[i][j]->Draw();
      canvas_ratio_c[i][j]->Print(canvas_name + "." + extension, extension);
    }
  }

  return;
} // void Tagging_RF_DL::Draw()

//////////

void Tagging_RF_DL::Draw_Validation()
{
  return;
}

//////////

void Tagging_RF_DL::Fill_Histo_MC(const TString &sample_name)
{
  int sample_index = Histo_Index(sample_name);

  float weight = 1;

  weight *= weight_lumi;
  weight *= weight_mc;
  weight *= weight_pileup;
  weight *= weight_prefire;
  weight *= weight_top_pt;

  weight *= weight_sl_trig;

  weight *= weight_mu_id;
  weight *= weight_mu_iso;

  weight *= weight_el_id;
  weight *= weight_el_reco;

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

  /*
    histo_mc_before_c[sample_index]->Fill(n_pv, weight);
    histo_mc_after_c[sample_index][0]->Fill(n_pv, weight * weight_c_tag);
    histo_mc_after_c[sample_index][1]->Fill(n_pv, weight * weight_c_tag_extrap_down);
    histo_mc_after_c[sample_index][2]->Fill(n_pv, weight * weight_c_tag_extrap_up);
    histo_mc_after_c[sample_index][3]->Fill(n_pv, weight * weight_c_tag_interp_down);
    histo_mc_after_c[sample_index][4]->Fill(n_pv, weight * weight_c_tag_interp_up);
    histo_mc_after_c[sample_index][5]->Fill(n_pv, weight * weight_c_tag_lhe_scale_muf_down);
    histo_mc_after_c[sample_index][6]->Fill(n_pv, weight * weight_c_tag_lhe_scale_muf_up);
    histo_mc_after_c[sample_index][7]->Fill(n_pv, weight * weight_c_tag_lhe_scale_mur_down);
    histo_mc_after_c[sample_index][8]->Fill(n_pv, weight * weight_c_tag_lhe_scale_mur_up);
    histo_mc_after_c[sample_index][9]->Fill(n_pv, weight * weight_c_tag_ps_fsr_fixed_down);
    histo_mc_after_c[sample_index][10]->Fill(n_pv, weight * weight_c_tag_ps_fsr_fixed_up);
    histo_mc_after_c[sample_index][11]->Fill(n_pv, weight * weight_c_tag_ps_isr_fixed_down);
    histo_mc_after_c[sample_index][12]->Fill(n_pv, weight * weight_c_tag_ps_isr_fixed_up);
    histo_mc_after_c[sample_index][13]->Fill(n_pv, weight * weight_c_tag_pu_down);
    histo_mc_after_c[sample_index][14]->Fill(n_pv, weight * weight_c_tag_pu_up);
    histo_mc_after_c[sample_index][15]->Fill(n_pv, weight * weight_c_tag_stat_down);
    histo_mc_after_c[sample_index][16]->Fill(n_pv, weight * weight_c_tag_stat_up);
    histo_mc_after_c[sample_index][17]->Fill(n_pv, weight * weight_c_tag_xsec_br_unc_dyjets_b_down);
    histo_mc_after_c[sample_index][18]->Fill(n_pv, weight * weight_c_tag_xsec_br_unc_dyjets_b_up);
    histo_mc_after_c[sample_index][19]->Fill(n_pv, weight * weight_c_tag_xsec_br_unc_dyjets_c_down);
    histo_mc_after_c[sample_index][20]->Fill(n_pv, weight * weight_c_tag_xsec_br_unc_dyjets_c_up);
    histo_mc_after_c[sample_index][21]->Fill(n_pv, weight * weight_c_tag_xsec_br_unc_wjets_c_down);
    histo_mc_after_c[sample_index][22]->Fill(n_pv, weight * weight_c_tag_xsec_br_unc_wjets_c_up);
    histo_mc_after_c[sample_index][23]->Fill(n_pv, weight * weight_c_tag_jer_down);
    histo_mc_after_c[sample_index][24]->Fill(n_pv, weight * weight_c_tag_jer_up);
    histo_mc_after_c[sample_index][25]->Fill(n_pv, weight * weight_c_tag_jes_total_down);
    histo_mc_after_c[sample_index][26]->Fill(n_pv, weight * weight_c_tag_jes_total_up);
  */

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
} // void Tagging_RF_DL::Fill_Histo_MC()

//////////

void Tagging_RF_DL::Fill_Histo_Validation_MC(const int &sample_index)
{
  float weight_raw = 1;

  weight_raw *= weight_lumi;
  weight_raw *= weight_mc;
  weight_raw *= weight_pileup;
  weight_raw *= weight_prefire;
  weight_raw *= weight_top_pt;

  weight_raw *= weight_sl_trig;

  weight_raw *= weight_mu_id;
  weight_raw *= weight_mu_iso;

  weight_raw *= weight_el_id;
  weight_raw *= weight_el_reco;

  float b_rf = Get_Tagging_RF_DL_B_Tag(vec_short_name_mc[sample_index], "B_Tag_Nominal", n_jets, ht);
  float c_rf = Get_Tagging_RF_DL_C_Tag(vec_short_name_mc[sample_index], "C_Tag_Nominal", n_pv, ht);

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
} // void Tagging_RF_DL::Fill_Histo_Validation_MC(const int& sample_index)

//////////

int Tagging_RF_DL::Histo_Index(const TString &sample_name)
{
  int index = -999;

  if ((sample_name.Contains("TTLL") || sample_name.Contains("TTLJ")) && !sample_name.Contains("TTLJ_WtoCB"))
  {
    bool chk_b = false;
    bool chk_c = false;

    for (size_t i = 0; i < gen_hf_flavour->size(); i++)
    {
      // cout << gen_hf_flavour->at(i) << " " << gen_hf_origin->at(i) << endl;

      if (gen_hf_flavour->at(i) == 5 && abs(gen_hf_origin->at(i)) != 6 && abs(gen_hf_origin->at(i)) != 24)
      {
        chk_b = true;
        break;
      }
      else if (gen_hf_flavour->at(i) == 4 && abs(gen_hf_origin->at(i)) != 6 && abs(gen_hf_origin->at(i)) != 24)
        chk_c = true;

    } // for(auto it=gen_hf_flavour->begin(); it!=gen_hf_flavour->end(); it++)

    if (chk_b)
      index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name] + "_BB") - vec_short_name_mc.begin();
    else if (chk_c)
      index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name] + "_CC") - vec_short_name_mc.begin();
    else
      index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name] + "_JJ") - vec_short_name_mc.begin();

  } // if (sample_name.Contains("TTLL") || sample_name.Contains("TTLJ"))
  else
    index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), samples.map_short_name_mc[sample_name]) - vec_short_name_mc.begin();

  // cout << index << endl;

  return index;
} // int Tagging_RF_DL::Histo_Index(const TString &sample_name)

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

      ratio_b[i][j]->Divide(histo_mc_before_b[i], histo_mc_after_b[i][j]);
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
        ratio_c[i][j] = new TH1D(ratio_name, ratio_name, bin_npv.size() - 1, bin_npv.data());

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
      ratio_c[i][j] = new TH2D(ratio_name, ratio_name, bin_npv.size() - 1, bin_npv.data(), bin_ht.size() - 1, bin_ht.data());

      ratio_c[i][j]->Divide(histo_mc_before_c[i], histo_mc_after_c[i][j]);
    }
  }

  return;
} // void Tagging_RF_DL::Ratio()

//////////

void Tagging_RF_DL::Read_Tree()
{
  cout << "[Tagging_RF_DL::Read_Tree]: Starts to read trees" << endl;

  // for MC
  for (auto it = map_tree_mc.begin(); it != map_tree_mc.end(); it++)
  {
    TString fin_name = it->first;
    cout << fin_name << endl;

    Long64_t n_entries = it->second->GetEntries();
    n_entries /= reduction;
    cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

    for (Long64_t i = 0; i < n_entries; i++)
    {
      if (i % 500000 == 0)
        cout << "Processing... " << i << "/" << n_entries << endl;

      it->second->GetEntry(i);

      int sample_index = 0;

      if (mode == "Analysis")
        Fill_Histo_MC(it->first);
      else if (mode == "Validation")
        Fill_Histo_Validation_MC(sample_index);
    } // loop over entries
  }   // loop over map_mc

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
  // // Stack_MC();
  Ratio();
  // // Draw();

  return;
} // void Tagging_RF_DL::Run_Analysis()

//////////

void Tagging_RF_DL::Run_Validation()
{
  cout << "[Tagging_RF_DL::Run_Validation]: Init" << endl;

  ROOT::EnableImplicitMT(4);

  Setup_Application();
  Setup_Analysis();
  Setup_Binning();
  Setup_Histo_Validation();
  Read_Tree();
  Draw_Validation();

  return;
} // void Tagging_RF_DL::Run_Validation()

//////////

void Tagging_RF_DL::Setup_Analysis()
{
  cout << "[Tagging_RF_DL::Setup_Analysis]: Init analysis" << endl;

  reduction = 1;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/Tagging_RF_DL/";

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
    // cout << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLJ"));
  vec_short_name_mc.erase(remove(vec_short_name_mc.begin(), vec_short_name_mc.end(), "TTLL"));
  vec_short_name_mc.push_back("TTLL_JJ");
  vec_short_name_mc.push_back("TTLL_CC");
  vec_short_name_mc.push_back("TTLL_BB");
  vec_short_name_mc.push_back("TTLJ_JJ");
  vec_short_name_mc.push_back("TTLJ_CC");
  vec_short_name_mc.push_back("TTLJ_BB");
  n_sample_merge_mc = vec_short_name_mc.size();
  // cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  return;
} // void Tagging_RF_DL::Setup_Analysis()

//////////

void Tagging_RF_DL::Setup_Application()
{
  cout << "[Tagging_RF_DL::Setup_Application]" << endl;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  fin = new TFile(path_base + "/Corrections/Vcb_Tagging_RF_DL_" + era + ".root");

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
  // ratio_c = new TH1D **[n_sample];
  ratio_c = new TH2D **[n_sample];
  for (int i = 0; i < n_sample; i++)
  {
    ratio_b[i] = new TH2D *[n_syst_b];
    for (int j = 0; j < n_syst_b; j++)
    {
      TString histo_name = sample_name[i] + "/Ratio_" + sample_name[i] + "_" + syst_name_b[j];
      ratio_b[i][j] = (TH2D *)fin->Get(histo_name);
    }

    /*
        ratio_c[i] = new TH1D *[n_syst_c];
        for (int j = 0; j < n_syst_c; j++)
        {
          TString histo_name = sample_name[i] + "/Ratio_" + sample_name[i] + "_" + syst_name_c[j];
          ratio_c[i][j] = (TH1D *)fin->Get(histo_name);
        }
        */
    ratio_c[i] = new TH2D *[n_syst_c];
    for (int j = 0; j < n_syst_c; j++)
    {
      TString histo_name = sample_name[i] + "/Ratio_" + sample_name[i] + "_" + syst_name_c[j];
      ratio_c[i][j] = (TH2D *)fin->Get(histo_name);
    }
  }

  return;
} // void Tagging_RF_DL::Setup_Application()

//////////

void Tagging_RF_DL::Setup_Binning()
{
  bin_njet = {4, 5, 6, 7, 8, 10, 12, 14, 20};
  bin_ht = {80, 140, 160, 200, 250, 300, 400, 600};
  bin_npv = {0, 10, 15, 20, 25, 28, 30, 32, 35, 40, 60};

  return;
} // void Tagging_RF_DL::Setup_Binning()

//////////

void Tagging_RF_DL::Setup_Histo()
{
  cout << "[Tagging_RF_DL::Setup_Histo]: Init" << endl;

  histo_mc_before_b = new TH2D *[n_sample_merge_mc];
  histo_mc_after_b = new TH2D **[n_sample_merge_mc];

  // histo_mc_before_c = new TH1D *[n_sample_merge_mc];
  // histo_mc_after_c = new TH1D **[n_sample_merge_mc];
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
} // void Tagging_RF_DL::Setup_Histo()

//////////

void Tagging_RF_DL::Setup_Histo_Validation()
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
} // void Tagging_RF_DL::Setup_Histo_Validation()

//////////

void Tagging_RF_DL::Setup_Tree(TTree *tree)
{
  tree->SetBranchAddress("weight_lumi", &weight_lumi);
  tree->SetBranchAddress("weight_mc", &weight_mc);
  tree->SetBranchAddress("weight_pileup", &weight_pileup);
  tree->SetBranchAddress("weight_prefire", &weight_prefire);
  tree->SetBranchAddress("weight_top_pt", &weight_top_pt);
  tree->SetBranchAddress("weight_pujet_veto", &weight_pujet_veto);

  tree->SetBranchAddress("weight_mu_id", &weight_mu_id);
  tree->SetBranchAddress("weight_mu_iso", &weight_mu_iso);
  tree->SetBranchAddress("weight_el_id", &weight_el_id);
  tree->SetBranchAddress("weight_el_reco", &weight_el_reco);

  tree->SetBranchAddress("weight_sl_trig", &weight_sl_trig);

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

  tree->SetBranchAddress("Gen_HF_Flavour", &gen_hf_flavour);
  tree->SetBranchAddress("Gen_HF_Origin", &gen_hf_origin);

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
