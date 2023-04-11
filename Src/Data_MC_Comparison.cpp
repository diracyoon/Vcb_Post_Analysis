#include "Data_MC_Comparison.h"

ClassImp(Data_MC_Comparison);

//////////

Data_MC_Comparison::Data_MC_Comparison(const TString &a_era, const TString &a_channel, const TString &a_analyser, const TString &a_extension)
{
  cout << "[Data_MC_Comparison::Data_MC_Comparison]: Init analysis" << endl;

  era = a_era;
  channel = a_channel;
  analyser = a_analyser;
  extension = a_extension;

  TString path_base = getenv("Vcb_Post_Analysis_WD");

  TString fin_name = path_base;
  if (analyser == "Vcb")
  {
    fin_name = "Vcb_Histos_" + era + "_" + channel + "All.root";
    color = {2, 3, 4, 5, 6, 7, 8};
  }
  else if (analyser == "Vcb_DL")
  {
    fin_name = "Vcb_DL_Histos_" + era + "_" + channel + ".root";
    color = {9, 3, 8, 5, 6, 7, 4, 2};
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
  n_region = region_name.size();

  // get syst_name
  TList *list_syst = ((TDirectory *)fin->Get(region_name[0]))->GetListOfKeys();
  Setup_Name(list_syst, syst_name);
  syst_name.erase(find(syst_name.begin(), syst_name.end(), "Data"));

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

  // get sample_name
  TList *list_sample = ((TDirectory *)fin->Get(region_name[0] + "/" + syst_name[0]))->GetListOfKeys();
  Setup_Name(list_sample, sample_name);
  n_sample = sample_name.size();
  cout << "n_sample = " << n_sample << endl;

  // get variable_name
  TList *list_variable = ((TDirectory *)fin->Get(region_name[0] + "/" + syst_name[0] + "/" + sample_name[0]))->GetListOfKeys();
  Setup_Name(list_variable, variable_name);
  n_variable = variable_name.size();

  tl = new TLegend(0.7, 0.4, 0.9, 0.8);
  tl->SetBorderSize(0);

  // setup histo_mc
  Setup_Histo_MC();

  // setup histo_data
  Setup_Histo_Data();

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
  Save();

  return;
} // void Data_MC_Comparison::Run()

//////////

void Data_MC_Comparison::Compare()
{
  histo_ratio = new TH1D **[n_region];
  gr_ratio = new TGraphAsymmErrors **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_ratio[i] = new TH1D *[n_variable];
    gr_ratio[i] = new TGraphAsymmErrors *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = "Ratio_" + region_name[i] + "_" + variable_name[j];
      histo_ratio[i][j] = new TH1D(histo_name, histo_name, variable_conf[j].n_bin, variable_conf[j].x_low, variable_conf[j].x_up);
      histo_ratio[i][j]->Divide(histo_data[i][j], (TH1D *)(stack_mc[i][0][j]->GetStack()->Last()));

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
    }   // loop over n_variable
  }     // loop over n_region

  return;
} // void Data_MC_Comparison::Compare()

//////////

void Data_MC_Comparison::Draw()
{
  canvas = new TCanvas **[n_region];
  pad = new TPad ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    canvas[i] = new TCanvas *[n_variable];
    pad[i] = new TPad **[n_variable];

    for (int j = 0; j < n_variable; j++)
    {
      TString canvas_name = region_name[i] + "_" + variable_name[j];
      // cout << canvas_name << endl;

      canvas[i][j] = new TCanvas(canvas_name, canvas_name, 1400, 1000);
      canvas[i][j]->Draw();

      pad[i][j] = new TPad *[2];

      TString pad_name = region_name[i] + "_" + variable_name[j] + "_";

      pad[i][j][0] = new TPad(pad_name + "Comp", pad_name + "Comp", 0, 0.4, 1, 1);
      canvas[i][j]->cd();
      pad[i][j][0]->Draw();

      pad[i][j][0]->cd();

      stack_mc[i][0][j]->SetTitle(variable_conf[j].variable_title);

      stack_mc[i][0][j]->Draw("BAR");
      float max = stack_mc[i][0][j]->GetMaximum();
      stack_mc[i][0][j]->SetMaximum(1.7 * max);

      pad[i][j][0]->cd();
      tl->Draw("SAME");

      gr_variation_merged[i][j]->SetFillColor(1);
      gr_variation_merged[i][j]->SetFillStyle(3001);
      gr_variation_merged[i][j]->Draw("SAME3");

      histo_data[i][j]->Sumw2();
      histo_data[i][j]->SetMarkerStyle(8);
      histo_data[i][j]->Draw("SAME");

      pad[i][j][1] = new TPad(pad_name + "Ratio", pad_name + "Ratio", 0, 0, 1, 0.4);
      canvas[i][j]->cd();
      pad[i][j][1]->Draw();
      pad[i][j][1]->cd();
      histo_ratio[i][j]->Draw();

      histo_ratio[i][j]->GetYaxis()->SetTitle("Data/MC");
      histo_ratio[i][j]->GetYaxis()->SetRangeUser(0, 2);

      gr_ratio[i][j]->SetFillColor(1);
      gr_ratio[i][j]->SetFillStyle(3001);
      gr_ratio[i][j]->Draw("SAME3");

      canvas[i][j]->Print(canvas_name + "." + extension, extension);
    } // loop over n_variable
  }   // loop over_n_region

  return;
} // void Data_MC_Comparison::Draw()

//////////

void Data_MC_Comparison::Draw_Each()
{
  /*
  canvas_each = new TCanvas ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    canvas_each[i] = new TCanvas **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      cout << syst_name[j] << endl;

      canvas_each[i][j] = new TCanvas *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString can_name = region_name[i] + "_" + syst_name[j] + "_" + variable_name[k];

        canvas_each[i][j][k] = new TCanvas(can_name, can_name, 800, 500);

        canvas_each[i][j][k]->Draw();
        stack_mc[i][j][k]->Draw("BAR");
      } // loop over n_syst
    }   // loop over n_variable
  }     // loop over n_region
*/
  return;
} // void Data_MC_Comparison::Draw_Each()

//////////

void Data_MC_Comparison::Envelope()
{
  cout << "Data_MC_Comparison::Envelope" << endl;

  // allocation
  gr_variation = new TGraphAsymmErrors ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    gr_variation[i] = new TGraphAsymmErrors **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      gr_variation[i][j] = new TGraphAsymmErrors *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        gr_variation[i][j][k] = new TGraphAsymmErrors();
      } // loop over n_variable
    }   // loop over n_syst;
  }     // loop over n_region

  for (int i = 0; i < n_region; i++)
  {
    // cout << region_name[i] << endl;
    for (int j = 0; j < n_variable; j++)
    {
      // cout << variable_name[j] << endl;
      float bin_width = (variable_conf[j].x_up - variable_conf[j].x_low) / variable_conf[j].n_bin;

      TH1D *histo_nominal = (TH1D *)(stack_mc[i][0][j]->GetStack()->Last());
      for (int k = 0; k < n_syst - 1; k++) // excluding nominal
      {
        // cout << syst_name[k + 1] << endl;

        TH1D *histo_syst = (TH1D *)(stack_mc[i][k + 1][j]->GetStack()->Last());

        for (int l = 0; l < variable_conf[j].n_bin; l++)
        {
          float bin_center = histo_nominal->GetBinCenter(l + 1);
          float content_nominal = histo_nominal->GetBinContent(l + 1);
          float content_syst = histo_syst->GetBinContent(l + 1);

          float diff = content_syst - content_nominal;

          // if (variable_conf[j].variable_title.Contains("N_Vertex"))
          //   cout << bin_center << " " << content_nominal << " " << content_syst << " " << diff << endl;

          // to mimic histogram for better display
          for (int m = -1; m < 2; m++)
          {
            int n_point = gr_variation[i][k][j]->GetN();

            gr_variation[i][k][j]->SetPoint(n_point, bin_center + m * 0.99 * bin_width / 2, content_nominal);
            if (0 < diff)
              gr_variation[i][k][j]->SetPointError(n_point, 0, 0, 0, diff);
            else
              gr_variation[i][k][j]->SetPointError(n_point, 0, 0, TMath::Abs(diff), 0);
          } // loop over 3
        }   // loop over n_bin
      }     // loop over n_syst
    }       // loop over n_variable
  }         // loop over n_region

  // merge variations via quadratic sum
  gr_variation_merged = new TGraphAsymmErrors **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    cout << region_name[i] << endl;
    gr_variation_merged[i] = new TGraphAsymmErrors *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      cout << variable_name[j] << endl;

      gr_variation_merged[i][j] = new TGraphAsymmErrors();

      TH1D *histo = (TH1D *)stack_mc[i][0][j]->GetStack()->Last();

      int n_point = gr_variation[i][0][j]->GetN();
      for (int k = 0; k < n_point; k++)
      {
        float x = gr_variation[i][0][j]->GetPointX(k);

        int bin = histo->FindBin(x);
        float stat_error = TMath::Sqrt(histo->GetBinContent(bin));

        float variation_up = 0;
        float variation_down = 0;

        for (int l = 0; l < n_syst; l++)
        {
          // cout << syst_name[l + 1] << endl;

          float ey_h = gr_variation[i][l][j]->GetErrorYhigh(k);
          float ey_l = gr_variation[i][l][j]->GetErrorYlow(k);

          variation_up = TMath::Sqrt(TMath::Power(variation_up, 2.) + TMath::Power(ey_h, 2.));
          variation_down = TMath::Sqrt(TMath::Power(variation_down, 2.) + TMath::Power(ey_l, 2.));

          // if (variable_conf[j].variable_title.Contains("N_Vertex"))
          //   cout << l << ", x = " << x << ", ey_h = " << ey_h << ", variation_up = " << variation_up << ", ey_l = " << ey_l << ", variation_down = " << variation_down << endl;
        } // loop over n_syst -1
          // if (variable_conf[j].variable_title.Contains("N_Vertex"))
        // cout << "x = " << x << ", stat = " << stat_error << ", down = " << variation_down << ", up = " << variation_up << endl;

        variation_up = TMath::Sqrt(TMath::Power(stat_error, 2.) + TMath::Power(variation_up, 2.));
        variation_down = TMath::Sqrt(TMath::Power(stat_error, 2.) + TMath::Power(variation_down, 2.));

        gr_variation_merged[i][j]->SetPoint(k, x, histo->GetBinContent(bin));
        gr_variation_merged[i][j]->SetPointError(k, 0, 0, variation_down, variation_up);
        // gr_variation_merged[i][j]->SetPointError(k, 0, 0, 100, 100);
      } // loop over n_bin
    }   // loop over n_variable
  }     // loop over n_region

  return;
} // void Data_MC_Comparison::Envelope()

//////////

void Data_MC_Comparison::Save()
{
  TString fout_name = "Vcb_Data_MC_Comparison_" + era + "_" + channel + ".root";
  fout = new TFile(fout_name, "RECREATE");

  for (int i = 0; i < n_region; i++)
  {
    TDirectory *dir_region = fout->mkdir(region_name[i]);

    for (int j = 0; j < n_variable; j++)
    {
      TDirectory *dir_variable = dir_region->mkdir(variable_name[j]);

      canvas[i][j]->SetName(variable_name[j]);
      canvas[i][j]->SetTitle(variable_name[j]);

      dir_variable->cd();
      canvas[i][j]->Write();

      canvas[i][j]->SaveAs(variable_name[j] + ".cpp");
    } // loop over n_variable
  }   // loop over n_region

  fout->Close();

  return;
} // void Data_MC_Comparison::Save()

//////////

void Data_MC_Comparison::Setup_Histo_Data()
{
  histo_data = new TH1D **[n_region];
  for (int i = 0; i < n_region; i++)
  {
    histo_data[i] = new TH1D *[n_variable];
    for (int j = 0; j < n_variable; j++)
    {
      TString histo_name = region_name[i] + "/Data/" + variable_name[j];
      // cout << histo_name << endl;
      histo_data[i][j] = (TH1D *)fin->Get(histo_name);

      if (i == 0)
      {
        Histo_Conf conf;
        conf.variable_title = histo_data[i][j]->GetName();
        conf.n_bin = histo_data[i][j]->GetNbinsX();
        conf.x_low = histo_data[i][j]->GetBinLowEdge(1);
        conf.x_up = histo_data[i][j]->GetBinLowEdge(conf.n_bin) + histo_data[i][j]->GetBinWidth(conf.n_bin);
        // cout << conf.variable_title << " " << conf.n_bin << " " << conf.x_low << " " << conf.x_up << endl;

        variable_conf.push_back(conf);
      } // if
    }   // n_variable
  }     // n_region

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
          // cout << histo_name << endl;
          histo_mc[i][j][k][l] = (TH1D *)fin->Get(histo_name);
        } // loop over n_variable
      }   // loop over sample
    }     // loop over n_syst
  }       // loop over n_region

  return;
} // void Data_MC_Comparison::Setup_Histo_MC()

//////////

void Data_MC_Comparison::Setup_Name(const TList *list, vector<TString> &vec_name)
{
  // maybe it's messy, but let's don't waste time

  int n_entries = list->GetEntries();

  for (int i = 0; i < n_entries; i++)
  {
    TDirectory *dir = (TDirectory *)list->At(i);
    TString name = dir->GetName();
    // cout << name << endl;

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
  for (int i = 0; i < n_region; i++)
  {
    stack_mc[i] = new THStack **[n_syst];
    for (int j = 0; j < n_syst; j++)
    {
      stack_mc[i][j] = new THStack *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString stack_name = region_name[i] + "_" + syst_name[j] + "_" + variable_name[k];
        cout << stack_name << endl;

        stack_mc[i][j][k] = new THStack(stack_name, stack_name);

        for (int l = 0; l < n_sample; l++)
        {
          cout << sample_name[l] << endl;

          histo_mc[i][j][l][k]->SetFillColorAlpha(color[l], .2);
          stack_mc[i][j][k]->Add(histo_mc[i][j][l][k]);

          if (i == 0 && j == 0 && k == 0)
            tl->AddEntry(histo_mc[i][j][l][k], sample_name[l], "f");
        } // loop over n_sample
      }   // loop over n_variable
    }     // loop over n_syst
  }       // loop over n_region

  cout << "[Data_MC_Comparison::Stack_MC] Done" << endl;

  return;
} // void Data_MC_Comparison::Stack_MC()

//////////