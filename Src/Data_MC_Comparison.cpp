#include "Data_MC_Comparison.h"

ClassImp(Data_MC_Comparison);

//////////

Data_MC_Comparison::Data_MC_Comparison(const TString &a_era, const TString &a_channel, const TString &a_analyser, const TString &a_tagger, const TString &a_extension)
    : samples(a_era, a_channel, a_analyser)
{
  cout << "[Data_MC_Comparison::Data_MC_Comparison]: Init analysis" << endl;

  ROOT::EnableImplicitMT(1);
  TH1::AddDirectory(kFALSE);

  gROOT->SetBatch(kTRUE);

  era = a_era;
  channel = a_channel;
  analyser = a_analyser;
  tagger = a_tagger;
  extension = a_extension;

  if (era == "2016preVFP")
    lumi = lumi_2016a;
  else if (era == "2016postVFP")
    lumi = lumi_2016b;
  else if (era == "2017")
    lumi = lumi_2017;
  else if (era == "2018")
    lumi = lumi_2018;

  TString path_base = getenv("Vcb_Post_Analysis_WD");

  // QCD data driven or MC
  chk_qcd_data_driven = true;
  // chk_qcd_data_driven = false;

  // simplified drawing
  chk_simple = true;

  if (analyser == "Vcb")
    signal_scale = 5;
  else if (analyser == "Vcb_DL")
  {
    signal_scale = 1000;
    chk_qcd_data_driven = false;
  }

  TString fin_name = path_base;
  if (analyser == "Vcb")
  {
    fin_name += "/Workplace/Histo_Syst/Vcb_Histos_" + era + "_" + channel + "_" + tagger + "_tagger.root";

    if (chk_simple)
    {
      color = {{"TTLJ_2", kOrange},
               {"TTLJ_CC_2", kOrange},
               {"TTLJ_BB_2", kOrange},
               {"TTLJ_4", kYellow},
               {"TTLJ_CC_4", kYellow},
               {"TTLJ_BB_4", kYellow},
               {"TTLJ_45", kRed},
               {"TTLJ_CC_45", kRed},
               {"TTLJ_BB_45", kRed},
               {"TTLL", kViolet},
               {"TTLL_CC", kViolet},
               {"TTLL_BB", kViolet},
               {"ST", kGreen},
               {"ttV", kTeal},
               {"VV", kCyan},
               {"VJets", kAzure},
               {"QCD_bEn", kBlack},
               {"QCD_Data_Driven", kBlack}};
    }
    else
    {
      color = {{"TTLJ_2", kOrange - 9},
               {"TTLJ_4", kOrange - 7},
               {"TTLJ_45", kOrange - 4},
               {"TTLL", kOrange},
               {"TTLJ_CC_2", kRed - 9},
               {"TTLJ_CC_4", kRed - 7},
               {"TTLJ_CC_45", kRed - 4},
               {"TTLL_CC", kRed},
               {"TTLJ_BB_2", kPink - 9},
               {"TTLJ_BB_4", kPink - 7},
               {"TTLJ_BB_45", kPink - 4},
               {"TTLL_BB", kMagenta},
               {"ST", kGreen},
               {"ttV", kTeal},
               {"VV", kCyan},
               {"VJets", kAzure},
               {"QCD", kBlack},
               {"QCD_Data_Driven", kBlack}};
    }
  }
  else if (analyser == "Vcb_DL")
  {
    fin_name += "/Workplace/CR_DL/Vcb_DL_Histos_" + era + "_" + channel + "_" + tagger + "_tagger.root";

    if (chk_simple)
    {
      color = {{"TTLJ_JJ_2", kYellow - 7},
               {"TTLJ_JJ_4", kYellow - 7},
               {"TTLL_JJ", kYellow - 7},
               {"TTLJ_CC_2", kOrange + 7},
               {"TTLJ_CC_4", kOrange + 7},
               {"TTLL_CC", kOrange + 7},
               {"TTLJ_BB_2", kViolet},
               {"TTLL_BB_4", kViolet},
               {"TTLL_BB", kViolet},
               {"TTLJ_JJ_45", kRed},
               {"TTLJ_CC_45", kRed},
               {"TTLJ_BB_45", kRed},
               {"ST", kGreen},
               {"ttV", kTeal},
               {"VV", kCyan},
               {"VJets", kAzure},
               {"QCD_bEn", kBlack}};
    }
    else
    {
      color = {{"TTLJ_JJ_2", kOrange - 9},
               {"TTLJ_JJ_4", kOrange - 7},
               {"TTLJ_JJ_45", kOrange - 4},
               {"TTLL_JJ", kOrange},
               {"TTLJ_CC_2", kRed - 9},
               {"TTLJ_CC_4", kRed - 7},
               {"TTLJ_CC_45", kRed - 4},
               {"TTLL_CC", kRed},
               {"TTLJ_BB_2", kPink - 9},
               {"TTLJ_BB_4", kPink - 7},
               {"TTLJ_BB_45", kPink - 4},
               {"TTLL_BB", kMagenta},
               {"ST", kGreen},
               {"ttV", kTeal},
               {"VV", kCyan},
               {"VJets", kAzure},
               {"QCD_bEn", kBlack},
               {"TTLJ_45", kBlue}};
    }
  }

  fin = new TFile(fin_name);
  if (fin == NULL)
  {
    cerr << "Can not find " << fin_name << ". Abort the process." << endl;
    exit(1);
  }

  // get region_name
  TList *list_region = fin->GetListOfKeys();
  Setup_Name(list_region, region_name);
  // region_name.clear();
  // region_name = {"Control0"};
  n_region = region_name.size();
  cout << "n_region = " << n_region << endl;

  // get syst_name
  TList *list_syst = ((TDirectory *)fin->Get(region_name[0]))->GetListOfKeys();
  Setup_Name(list_syst, syst_name);
  syst_name.erase(find(syst_name.begin(), syst_name.end(), "Data"));
  // if (find(syst_name.begin(), syst_name.end(), "") != syst_name.end())
  //   syst_name.erase(find(syst_name.begin(), syst_name.end(), ""));

  auto it_remove = std::remove_if(syst_name.begin(), syst_name.end(), [](const TString &s)
                                  { return s.Contains("_Smoothed"); });
  syst_name.erase(it_remove, syst_name.end());

  for (const auto &syst : syst_name)
    cout << syst << endl;

  // when jes breakdown is considered, jes_total should be removed
  if (chk_jes_break_down)
  {
    syst_name.erase(find(syst_name.begin(), syst_name.end(), "Jet_En_Down"));
    syst_name.erase(find(syst_name.begin(), syst_name.end(), "Jet_En_Up"));
  }

  // for debug
  // syst_name.push_back("Nominal");
  // syst_name.push_back("B_Tag_HF_Down");
  // syst_name.push_back("B_Tag_HF_Up");
  // syst_name.push_back("B_Tag_JES_Down");
  // syst_name.push_back("B_Tag_JES_Up");
  // syst_name.push_back("B_Tag_LFStats1_Down");
  // syst_name.push_back("B_Tag_LFStats1_Up");
  // syst_name.push_back("B_Tag_LFStats2_Down");
  // syst_name.push_back("B_Tag_LFStats2_Up");
  // syst_name.push_back("B_Tag_CFErr1_Down");
  // syst_name.push_back("B_Tag_CFErr1_Up");
  // syst_name.push_back("B_Tag_CFErr2_Down");
  // syst_name.push_back("B_Tag_CFErr2_Up");
  // syst_name.push_back("B_Tag_HFStats1_Down");
  // syst_name.push_back("B_Tag_HFStats1_Up");
  // syst_name.push_back("B_Tag_HFStats2_Down");
  // syst_name.push_back("B_Tag_HFStats2_Up");
  // syst_name.push_back("C_Tag_Extrap_Down");
  // syst_name.push_back("C_Tag_Extrap_Up");
  // syst_name.push_back("C_Tag_Interp_Down");
  // syst_name.push_back("C_Tag_Interp_Up");
  // syst_name.push_back("C_Tag_LHE_Scale_MuF_Down");
  // syst_name.push_back("C_Tag_LHE_Scale_MuF_Up");
  // syst_name.push_back("C_Tag_LHE_Scale_MuR_Down");
  // syst_name.push_back("C_Tag_LHE_Scale_MuR_Up");
  // syst_name.push_back("C_Tag_PS_FSR_Fixed_Down");
  // syst_name.push_back("C_Tag_PS_FSR_Fixed_Up");
  // syst_name.push_back("C_Tag_PS_ISR_Fixed_Down");
  // syst_name.push_back("C_Tag_PU_Down");
  // syst_name.push_back("C_Tag_PU_Up");
  // syst_name.push_back("C_Tag_Stat_Down");
  // syst_name.push_back("C_Tag_Stat_Up");
  // syst_name.push_back("C_Tag_XSec_Br_Unc_DYJets_B_Down");
  // syst_name.push_back("C_Tag_XSec_Br_Unc_DYJets_B_Up");
  // syst_name.push_back("C_Tag_XSec_Br_Unc_DYJets_C_Down");
  // syst_name.push_back("C_Tag_XSec_Br_Unc_DYJets_C_Up");
  // syst_name.push_back("C_Tag_XSec_Br_Unc_WJets_C_Down");
  // syst_name.push_back("C_Tag_XSec_Br_Unc_WJets_C_Up");
  // syst_name.push_back("C_Tag_JER_Down");
  // syst_name.push_back("C_Tag_JER_Up");
  // syst_name.push_back("C_Tag_JES_Total_Down");
  // syst_name.push_back("C_Tag_JES_Total_Up");
  // // syst_name.push_back("Jes_Down");
  // syst_name.push_back("Jes_Up");
  // syst_name.push_back("Jet_En_Down");
  // syst_name.push_back("Jet_En_Up");
  // syst_name.push_back("Mu_Id_Down");
  // syst_name.push_back("Mu_Id_Up");
  // syst_name.push_back("Mu_Iso_Down");
  // syst_name.push_back("Mu_Iso_Up");
  // syst_name.push_back("Mu_Trig_Down");
  // syst_name.push_back("Mu_Trig_Up");
  // syst_name.push_back("PDF_Alternative");
  // syst_name.push_back("PDF_As_Down");
  // syst_name.push_back("PDF_As_Up");
  // syst_name.push_back("Pileup_Down");
  // syst_name.push_back("Pileup_Up");
  // syst_name.push_back("Pileup_Veto_Down");
  // syst_name.push_back("Pileup_Veto_Up");
  // syst_name.push_back("Prefire_Down");
  // syst_name.push_back("Prefire_Up");
  // syst_name.push_back("Scale_Variation_1");
  // syst_name.push_back("Scale_Variation_2");
  // syst_name.push_back("Scale_Variation_3");
  // syst_name.push_back("Scale_Variation_4");
  // syst_name.push_back("Scale_Variation_6");
  // syst_name.push_back("Scale_Variation_8");
  // syst_name.push_back("Top_Pt_Reweight");
  // syst_name.push_back("CP5_Down");
  // syst_name.push_back("CP5_Up");
  // syst_name.push_back("hDamp_Down");
  // syst_name.push_back("hDamp_Up");
  // syst_name.push_back("mTop_171p5");
  // syst_name.push_back("mTop_173p5");

  n_syst = syst_name.size();
  cout << "n_syst = " << n_syst << endl;
  index_mc_nominal = find(syst_name.begin(), syst_name.end(), "Nominal") - syst_name.begin();
  cout << "Index_MC_Nominal = " << index_mc_nominal << endl;

  // syst_name_short for Draw_Each()
  for (unsigned int i = 0; i < n_syst; i++)
  {
    TString temp = syst_name[i];
    if (temp.Contains("_Up"))
      temp = temp.ReplaceAll("_Up", "");
    else if (temp.Contains("_Down"))
      temp = temp.ReplaceAll("_Down", "");
    else if (temp.Contains("mtop"))
    {
      temp = temp.ReplaceAll("_173p5", "");
      temp = temp.ReplaceAll("_171p5", "");
    }
    else if (temp.Contains("Scale_Variation"))
    {
      int pos = temp.Last('_');
      temp = temp.Remove(pos, 2);
    }

    if (temp != "Nominal")
      syst_name_short.push_back(temp);
  }
  syst_name_short.erase(unique(syst_name_short.begin(), syst_name_short.end()), syst_name_short.end());
  n_syst_name_short = syst_name_short.size();

  // get sample_name
  TList *list_sample = ((TDirectory *)fin->Get(region_name[0] + "/" + syst_name[0]))->GetListOfKeys();
  Setup_Name(list_sample, sample_name);

  // if (analyser == "Vcb")
  // {
  //   if (chk_qcd_data_driven)
  //     sample_name.erase(find(sample_name.begin(), sample_name.end(), "QCD_bEn"));
  //   else
  //     sample_name.erase(find(sample_name.begin(), sample_name.end(), "QCD_Data_Driven"));
  // }

  for (int i = 0; i < sample_name.size(); i++)
    cout << sample_name[i] << endl;

  n_sample = sample_name.size();
  cout << "n_sample = " << n_sample << endl;

  // get variable_name
  TList *list_variable = ((TDirectory *)fin->Get(region_name[0] + "/" + syst_name[0] + "/" + sample_name[0]))->GetListOfKeys();
  Setup_Name(list_variable, variable_name);
  if (analyser == "Vcb_DL")
    variable_name.erase(find(variable_name.begin(), variable_name.end(), "BvsC_3rd_4th_Jets"));
  n_variable = variable_name.size();

  tl = new TLegend(0.44, 0.69, 0.89, 0.89);
  tl->SetBorderSize(0);
  tl->SetFillStyle(0);
  tl->SetNColumns(3);

  // setup histo_mc
  Setup_Histo_MC();

  // setup histo_data
  Setup_Histo_Data();

  Ordering_Sample_Name();

  Run();
} // Data_MC_Comparison::Data_MC_Comparison(const TString& a_era, const TString& a_channel))

//////////

Data_MC_Comparison::~Data_MC_Comparison()
{
} // Data_MC_Comparison::~Data_MC_Comparison()

//////////

void Data_MC_Comparison::Run()
{
  cout << "Data_MC_Comparison::Run" << endl;

  Stack_MC();
  Envelope();
  Compare();
  Draw();
  //   Save();

  for (int i = 0; i < n_syst_name_short; i++)
  {
    if (analyser == "Vcb")
    {
      // Draw_Each(syst_name_short[i], "Best_MVA_Score");
      Draw_Each(syst_name_short[i], "Template_MVA_Score");
      // Draw_Each(syst_name_short[i], "N_BJets");
      Draw_Each(syst_name_short[i], "Total");
      // Draw_Each(syst_name_short[i], "BvsC_Leading_Jet");
      // Draw_Each(syst_name_short[i], "CvsB_Leading_Jet");
      // Draw_Each(syst_name_short[i], "CvsL_Leading_Jet");
      // Draw_Each(syst_name_short[i], "N_BJets");
    }
    else if (analyser == "Vcb_DL")
    {
      Draw_Each(syst_name_short[i], "BvsC_3rd_4th_Jets_Unrolled");
      // Draw_Each(syst_name_short[i], "Leading_Lepton_Pt");
      // Draw_Each(syst_name_short[i], "Total");
    }
  }

  if (analyser == "Vcb")
    Compare_QCD();

  return;
} // void Data_MC_Comparison::Run()

//////////

void Data_MC_Comparison::Compare()
{
  cout << "[Data_MC_Comparison::Compare]: Init" << endl;

  histo_ratio = new TH1D **[n_region];
  gr_ratio = new TGraphAsymmErrors **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_ratio[i] = new TH1D *[n_variable];
    gr_ratio[i] = new TGraphAsymmErrors *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = "Ratio_" + region_name[i] + "_" + variable_name[j];
      histo_ratio[i][j] = new TH1D(histo_name, histo_name, variable_conf[i][j].vec_bin.size() - 1, variable_conf[i][j].vec_bin.data());

      histo_ratio[i][j]->Divide(histo_data[i][j], (TH1D *)(stack_mc[i][index_mc_nominal][j]->GetStack()->Last()));

      gr_ratio[i][j] = new TGraphAsymmErrors();

      int n_point = gr_variation_merged[i][j]->GetN();
      for (int k = 0; k < n_point; k++)
      {
        float x = gr_variation_merged[i][j]->GetPointX(k);
        float y = gr_variation_merged[i][j]->GetPointY(k);

        float error_h = 0;
        float error_l = 0;
        if (y != 0)
        {
          error_h = gr_variation_merged[i][j]->GetErrorYhigh(k) / y;
          error_l = gr_variation_merged[i][j]->GetErrorYlow(k) / y;
        }

        gr_ratio[i][j]->SetPoint(k, x, 1);
        gr_ratio[i][j]->SetPointError(k, 0, 0, error_l, error_h);
        // gr_ratio[i][j]->SetPointError(k, 0, 0, 1, 1);
      } // loop over n_bin
    } // loop over n_variable
  } // loop over n_region

  cout << "[Data_MC_Comparison::Compare]: Done" << endl;

  return;
} // void Data_MC_Comparison::Compare()

//////////

void Data_MC_Comparison::Compare_QCD()
{
  cout << "[Data_MC_Comparison::Compare_QCD]: Init" << endl;

  TCanvas ***canvas = new TCanvas **[n_region];
  TLegend ***tl = new TLegend **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    canvas[i] = new TCanvas *[n_variable];
    tl[i] = new TLegend *[n_variable];

    for (int j = 0; j < n_variable; j++)
    // for (int j = 0; j < 1; j++)
    {
      TString canvas_name = "QCD_Comp_" + region_name[i] + "_" + era + "_" + channel + "_" + variable_name[j];
      canvas[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas[i][j]->Draw();

      int index_qcd_ben = find(sample_name.begin(), sample_name.end(), "QCD_bEn") - sample_name.begin();
      int index_qcd_dd = find(sample_name.begin(), sample_name.end(), "QCD_Data_Driven") - sample_name.begin();

      float max = histo_mc[i][index_mc_nominal][index_qcd_ben][j]->GetMaximum();

      max = (max < histo_mc[i][index_mc_nominal][index_qcd_dd][j]->GetMaximum()) ? histo_mc[i][index_mc_nominal][index_qcd_dd][j]->GetMaximum() : max;
      max *= 1.5;

      histo_mc[i][index_mc_nominal][index_qcd_ben][j]->GetYaxis()->SetRangeUser(0, max);
      histo_mc[i][index_mc_nominal][index_qcd_ben][j]->SetLineColor(2);
      histo_mc[i][index_mc_nominal][index_qcd_ben][j]->SetMarkerColor(2);
      histo_mc[i][index_mc_nominal][index_qcd_dd][j]->SetLineColor(4);
      histo_mc[i][index_mc_nominal][index_qcd_dd][j]->SetMarkerColor(4);

      histo_mc[i][index_mc_nominal][index_qcd_ben][j]->Draw();
      histo_mc[i][index_mc_nominal][index_qcd_dd][j]->Draw("SAME");

      tl[i][j] = new TLegend(0.7, 0.7, 0.9, 0.9);
      tl[i][j]->AddEntry(histo_mc[i][index_mc_nominal][index_qcd_dd][j], "Data Driven", "lep");
      tl[i][j]->AddEntry(histo_mc[i][index_mc_nominal][index_qcd_ben][j], "QCD bEn.", "lep");
      tl[i][j]->Draw("SAME");

      canvas[i][j]->Print(canvas_name + "." + extension, extension);
    } // for (int j = 0; j < n_variable; j++)
  } //  for(int i=0; i<n_region; i++)

  cout << "[Data_MC_Comparison::Compare_QCD]: Done" << endl;

  return;
} // void Data_MC_Comparison::Compare_QCD()

//////////

void Data_MC_Comparison::Draw()
{
  cout << "[Data_MC_Comparison::Draw] Init" << endl;

  gStyle->SetOptStat(0);

  canvas = new TCanvas **[n_region];
  pad = new TPad ***[n_region];
  for (int i = 0; i < n_region; i++)
  // for (int i = 1; i < n_region; i++)
  {
    canvas[i] = new TCanvas *[n_variable];
    pad[i] = new TPad **[n_variable];

    for (int j = 0; j < n_variable; j++)
    {
      TString canvas_name = region_name[i] + "_" + era + "_" + channel + "_" + variable_name[j];
      cout << canvas_name << endl;

      canvas[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas[i][j]->Draw();

      pad[i][j] = new TPad *[2];

      TString pad_name = region_name[i] + "_" + variable_name[j] + "_";

      pad[i][j][0] = new TPad(pad_name + "Comp", pad_name + "Comp", 0, 0.3, 1, 1);
      canvas[i][j]->cd();
      pad[i][j][0]->Draw();

      pad[i][j][0]->cd();

      stack_mc[i][index_mc_nominal][j]->Draw("BAR");

      stack_mc[i][index_mc_nominal][j]->SetTitle("");
      stack_mc[i][index_mc_nominal][j]->GetXaxis()->SetTitle(variable_conf[i][j].variable_title);

      float max = stack_mc[i][index_mc_nominal][j]->GetMaximum();
      stack_mc[i][index_mc_nominal][j]->SetMaximum(1.6 * max);

      stack_mc[i][index_mc_nominal][j]->SetMinimum(0.1);

      gr_variation_merged[i][j]->SetFillColor(1);
      gr_variation_merged[i][j]->SetFillStyle(3002);
      gr_variation_merged[i][j]->Draw("SAME3");

      // scaled signal
      // stack_mc_signal[i][0][j]->Draw("SAME ");
      TH1D *histo_signal = (TH1D *)stack_mc_signal[i][index_mc_nominal][j]->GetStack()->Last();
      if (analyser == "Vcb")
        histo_signal->SetLineColor(color["TTLJ_45"]);
      else if (analyser == "Vcb_DL")
        histo_signal->SetLineColor(color["TTLJ_JJ_45"]);
      histo_signal->SetLineWidth(3);
      histo_signal->Draw("SAME");

      // syst band for signal line
      if (analyser == "Vcb")
        gr_variation_merged_signal[i][j]->SetFillColor(color["TTLJ_45"]);
      else if (analyser == "Vcb_DL")
        gr_variation_merged_signal[i][j]->SetFillColor(color["TTLJ_JJ_45"]);
      gr_variation_merged_signal[i][j]->SetFillStyle(3002);
      gr_variation_merged_signal[i][j]->Draw("SAME3");

      pad[i][j][0]->cd();
      TLegend *legend = (TLegend *)tl->Clone();
      legend->Draw("SAME");

      if (region_name[i] != "Signal" || variable_name[j] != "Template_MVA_Score")
      {
        histo_data[i][j]->Sumw2();
        histo_data[i][j]->SetMarkerStyle(8);
        histo_data[i][j]->Draw("SAME");
      }

      TLatex *latex = new TLatex();
      latex->SetTextSize(0.04);
      latex->DrawLatexNDC(0.1, 0.91, "CMS #bf{work in progress}");
      latex->DrawLatexNDC(0.785, 0.91, Form("%s, %.2f fb^{-1}", channel.Data(), lumi));

      if (variable_name[j] == "Template_MVA_Score")
      {
        pad[i][j][0]->SetLogy();
      }

      pad[i][j][1] = new TPad(pad_name + "Ratio", pad_name + "Ratio", 0, 0, 1, 0.3);
      canvas[i][j]->cd();
      pad[i][j][1]->Draw();
      pad[i][j][1]->cd();

      histo_ratio[i][j]->SetTitle("");
      // histo_ratio[i][j]->GetXaxis()->SetTitle(variable_conf[j].variable_title);
      histo_ratio[i][j]->GetXaxis()->SetTitle("");
      histo_ratio[i][j]->GetYaxis()->SetTitle("Normalized by Nominal MC");
      histo_ratio[i][j]->GetYaxis()->SetTitleSize(0.07);
      histo_ratio[i][j]->GetYaxis()->SetTitleOffset(0.3);
      histo_ratio[i][j]->GetYaxis()->SetRangeUser(0.5, 1.5);
      histo_ratio[i][j]->SetMarkerStyle(8);
      histo_ratio[i][j]->Draw();

      // if (region_name[i] != "Signal" || variable_name[j] != "Template_MVA_Score")
      // {
      gr_ratio[i][j]->SetFillColor(1);
      gr_ratio[i][j]->SetFillStyle(3001);
      gr_ratio[i][j]->Draw("SAME3");
      // }

      canvas[i][j]->Update();
      canvas[i][j]->Modified();
      canvas[i][j]->Print(canvas_name + "." + extension, extension);
    } // loop over n_variable
  } // loop over_n_region

  cout << "[Data_MC_Comparison::Draw] Done" << endl;

  return;
} // void Data_MC_Comparison::Draw()

//////////

void Data_MC_Comparison::Draw_Each(const TString &a_syst_name, const TString &a_variable_name)
{
  gStyle->SetOptStat(0);

  // canvas_each = new TCanvas **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    // canvas_each[i] = new TCanvas *[n_variable];

    for (int j = 0; j < n_variable; j++)
    // for (int j = 0; j < 1; j++)
    {
      if (a_variable_name != "All" && a_variable_name != variable_name[j])
        continue;

      TString can_name = "Canvas_Each_" + region_name[i] + "_" + era + "_" + channel + "_" + a_syst_name + "_" + variable_name[j];
      // canvas_each[i][j] = new TCanvas(can_name, can_name, 800, 500);
      // canvas_each[i][j]->Draw();
      TCanvas *canvas_each = new TCanvas(can_name, can_name, 1600, 1000);
      canvas_each->Draw();

      TLegend *tl = new TLegend(0.45, 0.6, 0.95, 0.90);
      tl->SetNColumns(2);

      TString pad_name = region_name[i] + "_" + variable_name[j] + "_";
      TPad *pad_each[2];

      pad_each[0] = new TPad(pad_name + "Comp", pad_name + "Comp", 0, 0.4, 1, 1);
      canvas_each->cd();
      pad_each[0]->Draw();
      pad_each[0]->cd();

      // nominal
      TH1D *histo_nominal = (TH1D *)(stack_mc[i][index_mc_nominal][j]->GetStack()->Last());
      TH1D *histo_nominal_signal = (TH1D *)(stack_mc_signal[i][index_mc_nominal][j]->GetStack()->Last());

      histo_nominal->SetLineColor(1);
      // float max = histo_nominal->GetMaximum();
      float max = histo_nominal->GetBinContent(histo_nominal->GetMaximumBin());
      histo_nominal->GetYaxis()->SetRangeUser(0, 1.4 * max);

      histo_nominal_signal->SetLineColor(1);
      histo_nominal_signal->SetLineStyle(2);

      histo_nominal->Draw();
      histo_nominal_signal->Draw("SAME");

      tl->AddEntry(histo_nominal, "MC moninal", "le");

      vector<TH1D *> vec_histo_ratio;
      vector<TH1D *> vec_histo_ratio_signal;

      // to contain data
      // for Signal region, it's only dummy for drawing
      TString histo_name = "Ratio_" + region_name[i] + "_" + variable_name[j] + "_Data";
      TH1D *histo_ratio = new TH1D(histo_name, histo_name, variable_conf[i][j].vec_bin.size() - 1, variable_conf[i][j].vec_bin.data());

      // data
      // histo_data[i][j]->Sumw2();
      if (region_name[i] != "Signal")
      {
        histo_data[i][j]->SetMarkerStyle(8);
        histo_data[i][j]->SetMarkerColor(1);
        histo_data[i][j]->Draw("SAMEP");

        tl->AddEntry(histo_data[i][j], "Data", "p");

        histo_ratio->Divide(histo_data[i][j], histo_nominal);
      }
      else
        tl->AddEntry((TObject *)0, "", "");

      vec_histo_ratio.push_back(histo_ratio);

      // systs
      int n_syst_target = 0; // only to tweak line color
      float range_ratio_max = 0;
      float range_ratio_min = 10;

      for (int k = 0; k < n_syst; k++)
      {
        TString syst_name_temp = syst_name[k];

        // if (syst_name[k].Contains(syst_name_temp))
        if (a_syst_name.Contains("mtop"))
        {
          syst_name_temp.ReplaceAll("mtop_173p5", "");
          syst_name_temp.ReplaceAll("mtop_171p5", "");
        }
        else
        {
          syst_name_temp.ReplaceAll(a_syst_name, "");
          syst_name_temp.ReplaceAll("_", "");
        }

        if (syst_name_temp == "Down" || syst_name_temp == "Up" || syst_name_temp == "")
        {
          cout << a_syst_name << " " << syst_name[k] << " " << k << endl;

          TH1D *histo_syst = (TH1D *)(stack_mc[i][k][j]->GetStack()->Last());
          cout << histo_syst->Integral() << endl;

          histo_syst->Sumw2();
          histo_syst->SetMarkerStyle(n_syst_target + 43);
          histo_syst->SetLineColor(n_syst_target + 2);
          histo_syst->SetMarkerColor(n_syst_target + 2);
          histo_syst->SetMarkerSize(1.5);
          histo_syst->Draw("SAME");

          tl->AddEntry(histo_syst, syst_name[k], "lep");

          TH1D *histo_syst_signal = (TH1D *)(stack_mc_signal[i][k][j]->GetStack()->Last());
          histo_syst_signal->SetLineStyle(2);
          histo_syst_signal->SetMarkerStyle(n_syst_target + 41);
          if (n_syst_target == 0)
          {
            histo_syst_signal->SetLineColor(4);
            histo_syst_signal->SetMarkerColor(4);
          }
          else if (n_syst_target == 1)
          {
            histo_syst_signal->SetLineColor(6);
            histo_syst_signal->SetMarkerColor(6);
          }
          histo_syst_signal->SetMarkerSize(1.5);
          histo_syst_signal->Draw("SAME");

          tl->AddEntry(histo_syst_signal, syst_name[k] + "_Signal#times" + (TString)to_string(signal_scale), "lep");

          TString histo_name = "Ratio_" + region_name[i] + "_" + variable_name[j] + "_" + syst_name[k];
          TH1D *histo_ratio = new TH1D(histo_name, histo_name, variable_conf[i][j].vec_bin.size() - 1, variable_conf[i][j].vec_bin.data());
          histo_ratio->Divide(histo_syst, histo_nominal);

          vec_histo_ratio.push_back(histo_ratio);

          TString histo_name_signal = "Ratio_Signal" + region_name[i] + "_" + variable_name[j] + "_" + syst_name[k];
          TH1D *histo_ratio_signal = new TH1D(histo_name_signal, histo_name_signal, variable_conf[i][j].vec_bin.size() - 1, variable_conf[i][j].vec_bin.data());
          histo_ratio_signal->Divide(histo_syst_signal, histo_nominal_signal);

          vec_histo_ratio_signal.push_back(histo_ratio_signal);

          // min max for ratio
          if (histo_ratio->GetMinimum() < range_ratio_min)
            range_ratio_min = histo_ratio->GetMinimum();
          if (histo_ratio->GetMaximum() > range_ratio_max)
            range_ratio_max = histo_ratio->GetMaximum();

          if (histo_ratio_signal->GetMinimum() < range_ratio_min)
            range_ratio_min = histo_ratio_signal->GetMinimum();
          if (histo_ratio_signal->GetMaximum() > range_ratio_max)
            range_ratio_max = histo_ratio_signal->GetMaximum();

          n_syst_target++;
        }
      } // loop over syst

      range_ratio_min *= 0.99;
      range_ratio_max *= 1.01;

      tl->Draw();

      TLatex *latex = new TLatex();
      latex->SetTextSize(0.04);
      latex->DrawLatexNDC(0.1, 0.91, "CMS #bf{work in progress}");
      latex->DrawLatexNDC(0.785, 0.91, Form("%s, %.2f fb^{-1}", channel.Data(), lumi));

      canvas_each->cd();
      pad_each[1] = new TPad(pad_name + "Ratio", pad_name + "Ratio", 0, 0, 1, 0.4);
      pad_each[1]->Draw();
      pad_each[1]->cd();

      // i==0 is data, the highers are systs
      // for signal region, from i==0 systs
      for (unsigned int k = 0; k < vec_histo_ratio.size(); k++)
      {
        TH1D *histo_ratio = vec_histo_ratio[k];
        histo_ratio->SetLineColor(k + 1);
        histo_ratio->SetMarkerColor(k + 1);

        if (k == 0)
        {
          histo_ratio->GetYaxis()->SetTitle("Divided by MC Nominal");
          histo_ratio->GetYaxis()->SetRangeUser(range_ratio_min, range_ratio_max);
          histo_ratio->SetMarkerStyle(8);
          histo_ratio->Draw();
        }
        else
        {
          histo_ratio->SetMarkerStyle(k + 42);
          histo_ratio->SetMarkerSize(1.5);
          histo_ratio->Draw("SAME");
        }
      } // loop over systs

      // signal
      for (unsigned int k = 0; k < vec_histo_ratio_signal.size(); k++)
      {
        TH1D *histo_ratio_signal = vec_histo_ratio_signal[k];
        histo_ratio_signal->SetLineStyle(2);
        histo_ratio_signal->SetMarkerStyle(k + 41);
        histo_ratio_signal->SetMarkerSize(1.5);
        if (k == 0)
        {
          histo_ratio_signal->SetLineColor(4);
          histo_ratio_signal->SetMarkerColor(4);
        }
        else if (k == 1)
        {
          histo_ratio_signal->SetLineColor(6);
          histo_ratio_signal->SetMarkerColor(6);
        }
        histo_ratio_signal->Draw("SAME");
      }

      canvas_each->Update();
      canvas_each->Print(can_name + "." + extension, extension);

      // clear
      for (unsigned int i = 0; i < vec_histo_ratio.size(); i++)
      {
        delete vec_histo_ratio[i];
      }
      vec_histo_ratio.clear();

      for (unsigned int i = 0; i < vec_histo_ratio_signal.size(); i++)
      {
        delete vec_histo_ratio_signal[i];
      }
      vec_histo_ratio_signal.clear();

      pad_each[0]->Close();
      pad_each[1]->Close();
      canvas_each->Close();

      gSystem->ProcessEvents();

      delete pad_each[0];
      delete pad_each[1];
      delete canvas_each;
      delete tl;
    } // loop over n_variable
  } // loop over n_region

  return;
} // void Data_MC_Comparison::Draw_Each(const TString& variable_name)

//////////

void Data_MC_Comparison::Envelope()
{
  cout << "[Data_MC_Comparison::Envelope]: Init" << endl;

  // allocation
  gr_variation = new TGraphAsymmErrors ***[n_region];
  gr_variation_signal = new TGraphAsymmErrors ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    gr_variation[i] = new TGraphAsymmErrors **[n_syst];
    gr_variation_signal[i] = new TGraphAsymmErrors **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      gr_variation[i][j] = new TGraphAsymmErrors *[n_variable];
      gr_variation_signal[i][j] = new TGraphAsymmErrors *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        gr_variation[i][j][k] = new TGraphAsymmErrors();
        gr_variation_signal[i][j][k] = new TGraphAsymmErrors();
      } // loop over n_variable
    } // loop over n_syst;
  } // loop over n_region

  for (int i = 0; i < n_region; i++)
  {
    for (int j = 0; j < n_variable; j++)
    {
      TH1D *histo_nominal = (TH1D *)(stack_mc[i][index_mc_nominal][j]->GetStack()->Last());
      TH1D *histo_nominal_signal = (TH1D *)(stack_mc_signal[i][index_mc_nominal][j]->GetStack()->Last());

      for (int k = 0; k < n_syst; k++)
      {
        TH1D *histo_syst = (TH1D *)(stack_mc[i][k][j]->GetStack()->Last());
        TH1D *histo_syst_signal = (TH1D *)(stack_mc_signal[i][k][j]->GetStack()->Last());

        // cout << syst_name[k] << endl;

        for (int l = 0; l < variable_conf[i][j].n_bin; l++)
        {
          float bin_center = histo_nominal->GetBinCenter(l + 1);
          float bin_width = histo_nominal->GetBinWidth(l + 1);

          float content_nominal = histo_nominal->GetBinContent(l + 1);
          float content_syst = histo_syst->GetBinContent(l + 1);
          float diff = content_syst - content_nominal;

          float content_nominal_signal = histo_nominal_signal->GetBinContent(l + 1);
          float content_syst_signal = histo_syst_signal->GetBinContent(l + 1);
          float diff_signal = content_syst_signal - content_nominal_signal;

          // if (variable_name[j] == "N_BJets" && content_nominal != 0)
          // {
          //   cout << syst_name[k] << endl;
          //   cout << "l= " << l << ", bin_center= " << bin_center << ", bin_width= " << bin_width << ", nominal= " << content_nominal << ", syst= " << content_syst << ", diff=" << diff << endl;
          // }

          // to mimic histogram for better display
          for (int m = -1; m < 2; m++)
          // for (int m = 0; m < 1; m++)
          {
            int n_point = gr_variation[i][k][j]->GetN();

            // background
            gr_variation[i][k][j]->SetPoint(n_point, bin_center + m * 0.99 * bin_width / 2, content_nominal);
            if (0 < diff)
              gr_variation[i][k][j]->SetPointError(n_point, 0, 0, 0, diff);
            else
              gr_variation[i][k][j]->SetPointError(n_point, 0, 0, TMath::Abs(diff), 0);

            // signal
            gr_variation_signal[i][k][j]->SetPoint(n_point, bin_center + m * 0.99 * bin_width / 2, content_nominal_signal);
            if (0 < diff_signal)
              gr_variation_signal[i][k][j]->SetPointError(n_point, 0, 0, 0, diff_signal);
            else
              gr_variation_signal[i][k][j]->SetPointError(n_point, 0, 0, TMath::Abs(diff_signal), 0);
          } // loop over 3
        } // loop over n_bin
      } // loop over n_syst
    } // loop over n_variable
  } // loop over n_region

  // merge variations via quadratic sum
  gr_variation_merged = new TGraphAsymmErrors **[n_region];
  gr_variation_merged_signal = new TGraphAsymmErrors **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    // cout << region_name[i] << endl;

    gr_variation_merged[i] = new TGraphAsymmErrors *[n_variable];
    gr_variation_merged_signal[i] = new TGraphAsymmErrors *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      // cout << variable_name[j] << endl;

      gr_variation_merged[i][j] = new TGraphAsymmErrors();
      gr_variation_merged_signal[i][j] = new TGraphAsymmErrors();

      TH1D *histo = (TH1D *)stack_mc[i][index_mc_nominal][j]->GetStack()->Last();
      TH1D *histo_signal = (TH1D *)stack_mc_signal[i][index_mc_nominal][j]->GetStack()->Last();

      int n_point = gr_variation[i][index_mc_nominal][j]->GetN();

      for (int k = 0; k < n_point; k++)
      {
        float x = gr_variation[i][index_mc_nominal][j]->GetPointX(k);

        int bin = histo->FindBin(x);

        float stat_error = histo->GetBinError(bin);
        float variation_up = 0;
        float variation_down = 0;

        float stat_error_signal = histo_signal->GetBinError(bin);
        float variation_up_signal = 0;
        float variation_down_signal = 0;

        for (int l = 0; l < n_syst; l++)
        {
          // excluding nominal
          if (syst_name[l] == "Nominal")
            continue;

          float ey_h = gr_variation[i][l][j]->GetErrorYhigh(k);
          float ey_l = gr_variation[i][l][j]->GetErrorYlow(k);

          variation_up = TMath::Sqrt(TMath::Power(variation_up, 2.) + TMath::Power(ey_h, 2.));
          variation_down = TMath::Sqrt(TMath::Power(variation_down, 2.) + TMath::Power(ey_l, 2.));

          float ey_h_signal = gr_variation_signal[i][l][j]->GetErrorYhigh(k);
          float ey_l_signal = gr_variation_signal[i][l][j]->GetErrorYlow(k);

          variation_up_signal = TMath::Sqrt(TMath::Power(variation_up_signal, 2.) + TMath::Power(ey_h_signal, 2.));
          variation_down_signal = TMath::Sqrt(TMath::Power(variation_down_signal, 2.) + TMath::Power(ey_l_signal, 2.));

          // cout << syst_name[l] << endl;
          // cout << "l = " << l << ", x = " << x << ", ey_h = " << ey_h << ", variation_up = " << variation_up << ", ey_l = " << ey_l << ", variation_down = " << variation_down << endl;
          // cout << "l = " << l << ", x = " << x << ", ey_h_signal = " << ey_h_signal << ", variation_up_signal = " << variation_up_signal << ", ey_l_signal = " << ey_l_signal << ", variation_down_signal = " << variation_down_signal << endl;

          // if (variable_conf[j].variable_title.Contains("N_BJets") && k == 7)
          //   cout << syst_name[l] << ", k = " << k << ", x = " << x << ", ey_h = " << ey_h << ", variation_up = " << variation_up << ", ey_l = " << ey_l << ", variation_down = " << variation_down << endl;
        } // loop over n_syst
          // if (variable_conf[j].variable_title.Contains("N_Vertex"))
          // cout << "x = " << x << ", stat = " << stat_error << ", down = " << variation_down << ", up = " << variation_up << endl;

        variation_up = TMath::Sqrt(TMath::Power(stat_error, 2.) + TMath::Power(variation_up, 2.));
        variation_down = TMath::Sqrt(TMath::Power(stat_error, 2.) + TMath::Power(variation_down, 2.));

        // cout << "k= " << k << ", x=" << x << ", content= " << histo->GetBinContent(bin) << ", variation_down=" << variation_down << ", variation_up=" << variation_up << endl;

        gr_variation_merged[i][j]->SetPoint(k, x, histo->GetBinContent(bin));
        gr_variation_merged[i][j]->SetPointError(k, 0, 0, variation_down, variation_up);

        variation_up_signal = TMath::Sqrt(TMath::Power(stat_error_signal, 2.) + TMath::Power(variation_up_signal, 2.));
        variation_down_signal = TMath::Sqrt(TMath::Power(stat_error_signal, 2.) + TMath::Power(variation_down_signal, 2.));

        // cout << "k= " << k << ", x=" << x << ", content= " << histo_signal->GetBinContent(bin) << ", variation_down_signal=" << variation_down_signal << ", variation_up_signal=" << variation_up_signal << endl;
        // cout << "stat = " << stat_error_signal << endl;

        gr_variation_merged_signal[i][j]->SetPoint(k, x, histo_signal->GetBinContent(bin));
        gr_variation_merged_signal[i][j]->SetPointError(k, 0, 0, variation_down_signal, variation_up_signal);
      } // loop over n_bin
    } // loop over n_variable
  } // loop over n_region

  cout << "[Data_MC_Comparison::Envelope] Done" << endl;

  return;
} // void Data_MC_Comparison::Envelope()

//////////

vector<int> Data_MC_Comparison::Get_Histo_Group(const TString &sample_name_merged)
{
  vector<int> vec_histo_group;
  for (unsigned i = 0; i < sample_name.size(); i++)
  {
    if (samples.map_short_short_name[sample_name[i]] == sample_name_merged)
      vec_histo_group.push_back(i);
  }

  if (vec_histo_group.size() == 0)
  {
    int index = find(sample_name.begin(), sample_name.end(), sample_name_merged) - sample_name.begin();
    vec_histo_group.push_back(index);
  }

  return vec_histo_group;
} // vector<int> Data_MC_Comparison::Get_Histo_Group(const TString &sample_name_merged)

//////////

void Data_MC_Comparison::Ordering_Sample_Name()
{

  // for (unsigned int i = 0; i < n_sample; i++)
  //   cout << sample_name[i] << " " << histo_mc[0][0][i][0]->Integral() << endl;

  if (analyser == "Vcb")
  {
    if (chk_simple)
    {
      if (chk_qcd_data_driven)
        sample_name_order[0] = "QCD_Data_Driven";
      else
        sample_name_order[0] = "QCD_bEn";

      sample_name_order[1] = "VV";
      sample_name_order[2] = "ttV";
      sample_name_order[3] = "VJets";
      sample_name_order[4] = "ST";
      sample_name_order[5] = "TTLL";
      sample_name_order[6] = "TTLL_CC";
      sample_name_order[7] = "TTLL_BB";
      sample_name_order[8] = "TTLJ_2";
      sample_name_order[9] = "TTLJ_CC_2";
      sample_name_order[10] = "TTLJ_BB_2";
      sample_name_order[11] = "TTLJ_4";
      sample_name_order[12] = "TTLJ_CC_4";
      sample_name_order[13] = "TTLJ_BB_4";
      sample_name_order[14] = "TTLJ_45";
      sample_name_order[15] = "TTLJ_CC_45";
      sample_name_order[16] = "TTLJ_BB_45";
    }
    else
    {
      sample_name_order[0] = "QCD_bEn";
      sample_name_order[1] = "VV";
      sample_name_order[2] = "ttV";
      sample_name_order[3] = "VJets";
      sample_name_order[4] = "ST";
      sample_name_order[5] = "TTLL";
      sample_name_order[6] = "TTLJ_2";
      sample_name_order[7] = "TTLJ_4";
      sample_name_order[8] = "TTLJ_45";
      sample_name_order[9] = "TTLL_CC";
      sample_name_order[10] = "TTLJ_CC_2";
      sample_name_order[11] = "TTLJ_CC_4";
      sample_name_order[12] = "TTLJ_CC_45";
      sample_name_order[13] = "TTLL_BB";
      sample_name_order[14] = "TTLJ_BB_2";
      sample_name_order[15] = "TTLJ_BB_4";
      sample_name_order[16] = "TTLJ_BB_45";
    }
  } // if (analyser == "Vcb")
  else if (analyser == "Vcb_DL")
  {
    if (chk_simple)
    {
      sample_name_order[0] = "QCD_bEn";
      sample_name_order[1] = "VV";
      sample_name_order[2] = "ttV";
      sample_name_order[3] = "VJets";
      sample_name_order[4] = "ST";
      sample_name_order[5] = "TTLJ_JJ_2";
      sample_name_order[6] = "TTLJ_JJ_4";
      sample_name_order[7] = "TTLL_JJ";
      sample_name_order[8] = "TTLJ_CC_2";
      sample_name_order[9] = "TTLJ_CC_4";
      sample_name_order[10] = "TTLL_CC";
      sample_name_order[11] = "TTLJ_BB_2";
      sample_name_order[12] = "TTLJ_BB_4";
      sample_name_order[13] = "TTLL_BB";
      sample_name_order[14] = "TTLJ_JJ_45";
      sample_name_order[15] = "TTLJ_CC_45";
      sample_name_order[16] = "TTLJ_BB_45";
    }
    else
    {
      sample_name_order[0] = "QCD_bEn";
      sample_name_order[1] = "VV";
      sample_name_order[2] = "ttV";
      sample_name_order[3] = "VJets";
      sample_name_order[4] = "ST";
      sample_name_order[5] = "TTLJ_JJ_4";
      sample_name_order[6] = "TTLJ_JJ_2";
      sample_name_order[7] = "TTLJ_JJ_45";
      sample_name_order[8] = "TTLL_JJ";
      sample_name_order[9] = "TTLJ_CC_4";
      sample_name_order[10] = "TTLJ_CC_2";
      sample_name_order[11] = "TTLJ_CC_45";
      sample_name_order[12] = "TTLL_CC";
      sample_name_order[13] = "TTLJ_CC_4";
      sample_name_order[14] = "TTLJ_BB_2";
      sample_name_order[15] = "TTLJ_BB_45";
      sample_name_order[16] = "TTLL_BB";
    }
  } // else if (analyser == "Vcb_DL")

  for (auto it = sample_name_order.begin(); it != sample_name_order.end(); it++)
    cout << it->first << ", " << it->second << endl;

  return;
} // void Data_MC_Comparison::Ordering_Sample_Name()

//////////

void Data_MC_Comparison::Save()
{
  TString fout_name = "Vcb_Data_MC_Comparison_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "RECREATE");

  dir_variable = new TDirectory **[n_region];
  for (int i = 0; i < n_region; i++)
    dir_variable[i] = new TDirectory *[n_variable];

  for (int i = 0; i < n_region; i++)
  {
    TDirectory *dir_region = fout->mkdir(region_name[i]);

    for (int j = 0; j < n_variable; j++)
    {
      dir_variable[i][j] = dir_region->mkdir(variable_name[j]);

      canvas[i][j]->SetName("Canvas_" + variable_name[j]);
      canvas[i][j]->SetTitle("Canvas_" + variable_name[j]);

      dir_variable[i][j]->cd();
      canvas[i][j]->Write();

      // canvas[i][j]->SaveAs(variable_name[j] + ".cpp");
    } // loop over n_variable
  } // loop over n_region

  for (int i = 0; i < n_region; i++)
  {
    for (int j = 0; j < n_variable; j++)
    {
      dir_variable[i][j]->cd();

      for (int k = 0; k < n_syst - 1; k++)
      {
        TString gr_name = region_name[i] + "_" + variable_name[j] + "_" + syst_name[k + 1];
        gr_variation[i][k][j]->SetTitle(gr_name);
        gr_variation[i][k][j]->SetName(gr_name);

        gr_variation[i][k][j]->Write();
      } // loop over n_syst-1
    } // loop over n_variable
  } // loop over n_region

  fout->Close();

  return;
} // void Data_MC_Comparison::Save()

//////////

void Data_MC_Comparison::Setup_Histo_Data()
{
  // if (find(region_name.begin(), region_name.end(), "ThreeB") != region_name.end())
  //   n_region--;

  histo_data = new TH1D **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_data[i] = new TH1D *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = region_name[i] + "/Data/" + variable_name[j];

      histo_data[i][j] = (TH1D *)fin->Get(histo_name);

      Histo_Conf conf;
      conf.variable_title = histo_data[i][j]->GetName();
      conf.n_bin = histo_data[i][j]->GetNbinsX();

      for (int k = 0; k < conf.n_bin; k++)
        conf.vec_bin.push_back(histo_data[i][j]->GetBinLowEdge(k + 1));

      conf.vec_bin.push_back(histo_data[i][j]->GetBinLowEdge(conf.n_bin) + histo_data[i][j]->GetBinWidth(conf.n_bin));

      // cout << conf.variable_title << " " << conf.n_bin << " " << conf.x_low << " " << conf.x_up << endl;

      variable_conf[i].push_back(conf);
    } // n_variable
  } // n_region

  n_region = region_name.size();

  return;
} // void Data_MC_Comparison::Setup_Histo_Data()

//////////

void Data_MC_Comparison::Setup_Histo_MC()
{
  histo_mc = new TH1D ****[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_mc[i] = new TH1D ***[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      histo_mc[i][j] = new TH1D **[n_sample];
      for (int k = 0; k < n_sample; k++)
      {
        histo_mc[i][j][k] = new TH1D *[n_variable];
        for (int l = 0; l < n_variable; l++)
        {
          TString histo_name = region_name[i] + "/" + syst_name[j] + "/" + sample_name[k] + "/" + variable_name[l];

          // if (histo_name.Contains("Nominal") && histo_name.Contains("QCD"))
          //   cout << histo_name << endl;

          histo_mc[i][j][k][l] = (TH1D *)fin->Get(histo_name);

          if (sample_name[k].Contains("_BB"))
            histo_mc[i][j][k][l]->Scale(TTBB_Scale);
          else if (sample_name[k].Contains("_CC"))
            histo_mc[i][j][k][l]->Scale(TTCC_Scale);
        } // loop over n_variable
      } // loop over sample
    } // loop over n_syst
  } // loop over n_region

  return;
} // void Data_MC_Comparison::Setup_Histo_MC()

//////////

void Data_MC_Comparison::Setup_Name(const TList *list, vector<TString> &vec_name)
{
  int n_entries = list->GetEntries();

  for (int i = 0; i < n_entries; i++)
  {
    TDirectory *dir = (TDirectory *)list->At(i);
    TString name = dir->GetName();

    if (name.Contains("PDF_Error_Set"))
    {
      TString temp = name;

      int first = temp.First('_');
      temp = temp.Remove(0, first + 1);

      int second = temp.First('_');
      temp = temp.Remove(0, second + 1);

      int third = temp.First('_');
      TString index = temp.Remove(0, third + 1);

      if (index.IsDigit())
        continue;

    } // if (name.Contains("PDF_Error_Set"))

    vec_name.push_back(name);
  }

  return;
} // int Data_MC_Comparison::Setup_Name(const TList* list, TString* name)

//////////

void Data_MC_Comparison::Stack_MC()
{
  cout << "[Data_MC_Comparison::Stack_MC] Init" << endl;

  stack_mc = new THStack ***[n_region];
  stack_mc_signal = new THStack ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    // cout << "i = " << i << " " << region_name[i] << endl;

    stack_mc[i] = new THStack **[n_syst];
    stack_mc_signal[i] = new THStack **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      // cout << "j = " << j << " " << syst_name[j] << endl;

      stack_mc[i][j] = new THStack *[n_variable];
      stack_mc_signal[i][j] = new THStack *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        // cout << "k = " << k << " " << variable_name[k] << endl;

        TString stack_name = region_name[i] + "_" + syst_name[j] + "_" + variable_name[k];
        cout << stack_name << endl;

        stack_mc[i][j][k] = new THStack(stack_name, stack_name);
        stack_mc_signal[i][j][k] = new THStack("Signal_" + stack_name, "Signal_" + stack_name);

        for (auto it = sample_name_order.begin(); it != sample_name_order.end(); it++)
        {
          // cout << it->second << " ";

          vector<int> vec_sample_index = Get_Histo_Group(it->second);
          for (unsigned int l = 0; l < vec_sample_index.size(); l++)
          {
            // cout << vec_sample_index[l] << " ";

            histo_mc[i][j][vec_sample_index[l]][k]->SetLineColor(color[it->second]);
            histo_mc[i][j][vec_sample_index[l]][k]->SetLineWidth(2);

            histo_mc[i][j][vec_sample_index[l]][k]->SetFillColor(color[it->second]);

            if (syst_name[j] == "Jet_En_BBEC1_Down" && variable_name[k] == "Total")
            {
              cout << histo_mc[i][j][vec_sample_index[l]][k]->GetBinContent(1) << endl;
            }

            if (it->second.Contains("_45"))
            {
              // scale signal
              histo_mc[i][j][vec_sample_index[l]][k]->Scale(signal_scale);

              stack_mc_signal[i][j][k]->Add(histo_mc[i][j][vec_sample_index[l]][k]);
            }
            else
              stack_mc[i][j][k]->Add(histo_mc[i][j][vec_sample_index[l]][k]);
          } // loop over histo group
          // cout << endl;

          if (i == 0 && j == 0 && k == 0)
          {
            if (analyser == "Vcb")
            {
              if (chk_simple)
              {
                if (!it->second.Contains("_CC") && !it->second.Contains("_BB"))
                {
                  if (it->second.Contains("_45"))
                    tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TTLJ, W#rightarrowcb#times" + (TString)to_string(signal_scale), "l");
                  else
                  {
                    if (it->second.Contains("_2"))
                      tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TTLJ, W#rightarrowud (us)", "f");
                    else if (it->second.Contains("_4"))
                      tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TTLJ, W#rightarrowcs (cd)", "f");
                    else
                      tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], it->second, "f");
                  }
                }
              } // if (chk_simple)
              else
                tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], it->second, "f");
            } // if (analyser == "Vcb")
            else if (analyser == "Vcb_DL")
            {
              if (chk_simple)
              {
                if (it->second.Contains("_JJ"))
                {
                  if (it->second.Contains("_JJ_2"))
                    tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TT+JJ", "f");
                  else if (it->second.Contains("_JJ_45"))
                    tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TTLJ, W#rightarrowcb#times" + (TString)to_string(signal_scale), "l");
                }
                else if (it->second.Contains("_CC"))
                {
                  if (it->second.Contains("_CC_2"))
                    tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TT+C/CC", "f");
                }
                else if (it->second.Contains("_BB"))
                {
                  if (it->second.Contains("_BB_2"))
                    tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], "TT+B/BB", "f");
                }
                else
                  tl->AddEntry(histo_mc[i][j][vec_sample_index[0]][k], it->second, "f");
              } // if(chk_simple)
            } // else if (analyser == "Vcb_DL")
          } // legend

        } // loop over order

      } // loop over n_variable
    } // loop over n_syst
  } // loop over n_region

  cout << "[Data_MC_Comparison::Stack_MC] Done" << endl;

  return;
} // void Data_MC_Comparison::Stack_MC()

//////////
