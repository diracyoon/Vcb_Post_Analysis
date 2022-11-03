#include "Data_MC_Comparison.h"

ClassImp(Data_MC_Comparison);

//////////

Data_MC_Comparison::Data_MC_Comparison(const TString &a_era, const TString &a_channel, const TString &a_extension)
{
  cout << "[Data_MC_Comparison::Data_MC_Comparison]: Init analysis" << endl;

  era = a_era;
  channel = a_channel;
  extension = a_extension;

  TString path_base = getenv("Vcb_Post_Analysis_WD");

  TString fin_name = path_base + "/Macro/Data_MC_Comparison/Vcb_Histos_" + era + "_" + channel + ".root";

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
  n_pdf_error_set = Setup_Name(list_syst, syst_name, true);
  syst_name.erase(find(syst_name.begin(), syst_name.end(), "Data"));
  // syst_name.push_back("Nominal");
  // syst_name.push_back("Pileup_Down");
  // syst_name.push_back("Pileup_Up");
  // n_pdf_error_set = 100;
  n_syst = syst_name.size();
  // n_pdf_error_set = 0;
  cout << "n_pdf_error_set = " << n_pdf_error_set << endl;
  cout << "n_syst = " << n_syst << endl;

  // get sample_name
  TList *list_sample = ((TDirectory *)fin->Get(region_name[0] + "/" + syst_name[0]))->GetListOfKeys();
  Setup_Name(list_sample, sample_name);
  n_sample = sample_name.size();

  // get variable_name
  TList *list_variable = ((TDirectory *)fin->Get(region_name[0] + "/" + syst_name[0] + "/" + sample_name[0]))->GetListOfKeys();
  Setup_Name(list_variable, variable_name);
  n_variable = variable_name.size();

  // setup histo_mc
  histo_mc = new TH1D ****[n_region];
  histo_pdf_error_set = new TH1D ****[n_region];
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

    // histograms for PDF_Error_Set
    // For easy handle, let's separate histograms for PDF_Error_Set
    histo_pdf_error_set[i] = new TH1D ***[n_pdf_error_set];
    for (int j = 0; j < n_pdf_error_set; j++)
    {
      histo_pdf_error_set[i][j] = new TH1D **[n_sample];
      for (int k = 0; k < n_sample; k++)
      {
        histo_pdf_error_set[i][j][k] = new TH1D *[n_variable];
        for (int l = 0; l < n_variable; l++)
        {
          TString histo_name = region_name[i] + "/PDF_Error_Set_" + to_string(j) + "/" + sample_name[k] + "/" + variable_name[l];
          // cout << histo_name << endl;
          histo_pdf_error_set[i][j][k][l] = (TH1D *)fin->Get(histo_name);
        }
      }
    } // loop over n_pdf_error_set
  }   // loop over n_region

  // setup histo_data
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

  Run();
} // Data_MC_Comparison::Data_MC_Comparison(const TString& a_era, const TString& a_channel))

//////////

Data_MC_Comparison::~Data_MC_Comparison()
{
} // Data_MC_Comparison::~Data_MC_Comparison()

//////////

void Data_MC_Comparison::Run()
{
  cout << "Stack_MC" << endl;
  Stack_MC();
  cout << "Envelope" << endl;
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
      for (int k = 0; k < variable_conf[j].n_bin; k++)
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

      canvas[i][j] = new TCanvas(canvas_name, canvas_name, 800, 500);
      canvas[i][j]->Draw();

      pad[i][j] = new TPad *[2];

      TString pad_name = region_name[i] + "_" + variable_name[j] + "_";

      pad[i][j][0] = new TPad(pad_name + "Comp", pad_name + "Comp", 0, 0.4, 1, 1);
      canvas[i][j]->cd();
      pad[i][j][0]->Draw();

      pad[i][j][0]->cd();

      stack_mc[i][0][j]->Draw("BAR");
      float max = stack_mc[i][0][j]->GetMaximum();
      stack_mc[i][0][j]->SetMaximum(2 * max);

      gr_variation_merged[i][j]->SetFillColor(1);
      gr_variation_merged[i][j]->SetFillStyle(3001);
      gr_variation_merged[i][j]->Draw("SAME3");

      histo_data[i][j]->Sumw2();
      histo_data[i][j]->SetMarkerStyle(8);
      // histo_data[i][j]->SetMarkerSize(8);
      histo_data[i][j]->Draw("SAME");
      // histo_data[i][j]->Draw("");

      pad[i][j][1] = new TPad(pad_name + "Ratio", pad_name + "Ratio", 0, 0, 1, 0.4);
      canvas[i][j]->cd();
      pad[i][j][1]->Draw();
      pad[i][j][1]->cd();
      histo_ratio[i][j]->Draw();
      histo_ratio[i][j]->GetYaxis()->SetRangeUser(0, 2);

      gr_ratio[i][j]->SetFillColor(1);
      gr_ratio[i][j]->SetFillStyle(3001);
      gr_ratio[i][j]->Draw("SAME3");

    } // loop over n_variable
  }   // loop over_n_region

  return;
} // void Data_MC_Comparison::Draw()

//////////

void Data_MC_Comparison::Envelope()
{
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
    cout << region_name[i] << endl;
    for (int j = 0; j < n_variable; j++)
    {
      cout << variable_name[j] << endl;

      TH1D *histo_nominal = (TH1D *)(stack_mc[i][0][j]->GetStack()->Last());
      for (int k = 0; k < n_syst - 1; k++) // excluding nominal, excluding merged_pdf_error_set which is the last
      {
        cout << syst_name[k + 1] << endl;

        TH1D *histo_syst = (TH1D *)(stack_mc[i][k + 1][j]->GetStack()->Last());

        for (int l = 0; l < variable_conf[j].n_bin; l++)
        {
          float bin_center = histo_nominal->GetBinCenter(l + 1);
          float content_nominal = histo_nominal->GetBinContent(l + 1);
          float content_syst = histo_syst->GetBinContent(l + 1);

          float diff = content_syst - content_nominal;

          // if (variable_conf[j].variable_title.Contains("N_Vertex"))
          //   cout << bin_center << " " << content_nominal << " " << content_syst << " " << diff << endl;

          gr_variation[i][k][j]->SetPoint(l, bin_center, content_nominal);
          if (0 < diff)
            gr_variation[i][k][j]->SetPointError(l, 0, 0, 0, diff);
          else
            gr_variation[i][k][j]->SetPointError(l, 0, 0, TMath::Abs(diff), 0);
        } // loop over n_bin
      }   // loop over n_syst
    }     // loop over n_variable
  }       // loop over n_region

  Merge_PDF_Error_Set();

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

      for (int k = 0; k < variable_conf[j].n_bin; k++)
      {
        float stat_error = TMath::Sqrt(histo->GetBinContent(k + 1));

        float variation_up = 0;
        float variation_down = 0;

        float x = gr_variation[i][0][j]->GetPointX(k);

        for (int l = 0; l < n_syst; l++)
        {
          float ey_h = gr_variation[i][l][j]->GetErrorYhigh(k);
          float ey_l = gr_variation[i][l][j]->GetErrorYlow(k);

          variation_up = TMath::Sqrt(TMath::Power(variation_up, 2.) + TMath::Power(ey_h, 2.));
          variation_down = TMath::Sqrt(TMath::Power(variation_down, 2.) + TMath::Power(ey_l, 2.));

          // if (variable_conf[j].variable_title.Contains("N_Vertex"))
          //   cout << l << ", x = " << x << ", ey_h = " << ey_h << ", variation_up = " << variation_up << ", ey_l = " << ey_l << ", variation_down = " << variation_down << endl;
        } // loop over n_syst -1
        // if (variable_conf[j].variable_title.Contains("N_Vertex"))
        //   cout << "x = " << x << ", stat = " << stat_error << " " << variation_up << " " << variation_down << endl;

        variation_up = TMath::Sqrt(TMath::Power(stat_error, 2.) + TMath::Power(variation_up, 2.));
        variation_down = TMath::Sqrt(TMath::Power(stat_error, 2.) + TMath::Power(variation_down, 2.));

        gr_variation_merged[i][j]->SetPoint(k, histo->GetBinCenter(k + 1), histo->GetBinContent(k + 1));
        gr_variation_merged[i][j]->SetPointError(k, 0, 0, variation_down, variation_up);
        // gr_variation_merged[i][j]->SetPointError(k, 0, 0, 100, 100);
      } // loop over n_bin
    }   // loop over n_variable
  }     // loop over n_region

  return;
} // void Data_MC_Comparison::Envelope()

//////////

void Data_MC_Comparison::Merge_PDF_Error_Set()
{
  // Merge histos for 100 pdf_error_sets
  cout << "Merging histograms for 100 pdf_error_sets" << endl;

  stack_pdf_error_set = new THStack ***[n_region];
  for (int i = 0; i < n_region; i++)
  {
    stack_pdf_error_set[i] = new THStack **[n_pdf_error_set];
    for (int j = 0; j < n_pdf_error_set; j++)
    {
      stack_pdf_error_set[i][j] = new THStack *[n_variable];
      for (int k = 0; k < n_variable; k++)
      {
        TString stack_name = region_name[i] + "_PDF_Error_Set_" + to_string(j) + "_" + variable_name[k];
        stack_pdf_error_set[i][j][k] = new THStack(stack_name, stack_name);

        for (int l = 0; l < n_sample; l++)
        {
          stack_pdf_error_set[i][j][k]->Add(histo_pdf_error_set[i][j][l][k]);
        } // loop over n_sample
      }   // loop over n_variable
    }     // loop over n_pdf_error_set
  }       // loop over n_region

  for (int i = 0; i < n_region; i++)
  {
    for (int j = 0; j < n_variable; j++)
    {
      if (gr_variation[i][n_syst - 1][j]->GetN() != 0)
      {
        cout << "gr_variation is already filled. Check Merged_PDF_Error_Set is lastly push_backed. Aborted!" << endl;
        exit(1);
      }

      TH1D *histo_nominal = (TH1D *)(stack_mc[i][0][j]->GetStack()->Last());
      for (int k = 0; k < variable_conf[j].n_bin; k++)
      {
        float x = histo_nominal->GetBinCenter(k + 1);
        float y = histo_nominal->GetBinContent(k + 1);

        float max = -999999999;
        float min = 999999999;

        for (int l = 0; l < n_pdf_error_set; l++)
        {
          TH1D *histo = (TH1D *)(stack_pdf_error_set[i][l][j]->GetStack()->Last());
          float content = histo->GetBinContent(k + 1);

          if (max < content)
            max = content;
          if (content < min)
            min = content;
        } // loop over n_pdf_error_set

        gr_variation[i][n_syst - 1][j]->SetPoint(k, x, y);

        float eyl;

        if (min < y)
          eyl = y - min;
        else
          eyl = 0;

        float eyh;
        if (y < max)
          eyh = max - y;
        else
          eyh = 0;

        gr_variation[i][n_syst - 1][j]->SetPointError(k, 0, 0, eyl, eyh);

        // cout << "bin_index = " << k << ", x = " << x << ", y = " << y << ", eyl = " << y - min << ", eyh = " << max - y << endl;
        cout << "bin_index = " << k << ", x = " << x << ", y = " << y << ", min = " << min << ", max = " << max << ", eyl = " << eyl << ", eyh = " << eyh << endl;
      } // loop over n_bin
    }   // loop n_variable
  }     // loop n_region

  return;
} // void Data_MC_Comparison::Merge_PDF_Error_Set()

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
      canvas[i][j]->SetName(variable_name[j]);
      canvas[i][j]->SetTitle(variable_name[j]);

      dir_region->cd();
      canvas[i][j]->Write();
    } // loop over n_variable
  }   // loop over n_region

  fout->Close();

  return;
} // void Data_MC_Comparison::Save()

//////////

int Data_MC_Comparison::Setup_Name(const TList *list, vector<TString> &vec_name, const bool &chk_excluding_pdf_error_set)
{
  // maybe it's messy, but let's don't waste time

  int n_entries = list->GetEntries();
  int n_pdf_error_set = 0;

  for (int i = 0; i < n_entries; i++)
  {
    TDirectory *dir = (TDirectory *)list->At(i);
    TString name = dir->GetName();
    // cout << name << endl;

    if (!chk_excluding_pdf_error_set)
      vec_name.push_back(name);
    else
    {
      if (name.Contains("PDF_Error_Set_"))
        n_pdf_error_set++;
      else
        vec_name.push_back(name);
    }
  }

  return n_pdf_error_set;
} // int Data_MC_Comparison::Setup_Name(const TList* list, TString* name)

//////////

void Data_MC_Comparison::Stack_MC()
{
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
        // cout << stack_name << endl;

        stack_mc[i][j][k] = new THStack(stack_name, stack_name);

        for (int l = 0; l < n_sample; l++)
        {
          histo_mc[i][j][l][k]->SetFillColorAlpha(color[l], .2);
          stack_mc[i][j][k]->Add(histo_mc[i][j][l][k]);
        } // loop over n_sample
      }   // loop over n_variable
    }     // loop over n_syst
  }       // loop over n_region

  return;
} // void Data_MC_Comparison::Stack_MC()

//////////