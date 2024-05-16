#include "Selection.h"

ClassImp(Selection);

//////////

Selection::Selection(const TString &a_era, const TString &a_channel, const TString &a_extension) : samples(a_era, a_channel)
{
  era = a_era;
  channel = a_channel;
  extension = a_extension;

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunResult/Central_Syst/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  cout << "Number of MC = " << samples.map_mc.size() << endl;
  for (unsigned int i = 0; i < samples.map_mc.size(); i++)
  {
    auto it = samples.map_mc.begin();
    std::advance(it, i);

    auto it_short = samples.map_short_name_mc.begin();
    std::advance(it_short, i);

    TString sample_name = it->first;
    TString sample_name_short = it_short->second;

    cout << sample_name << ", " << sample_name_short << endl;

    map_fin[sample_name] = new TFile(path_base + it->second);

    if (sample_name == "TTLJ")
    {
      map_histo["TTLJ_ud"] = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow_21")->Clone();
      map_histo["TTLJ_us"] = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow_23")->Clone();
      map_histo["TTLJ_cd"] = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow_41")->Clone();
      map_histo["TTLJ_cs"] = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow_43")->Clone();
    }
    else if (sample_name == "TTLJ_WtoCB")
      map_histo["TTLJ_cb"] = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow_45")->Clone();
    else
    {
      if (map_histo[sample_name_short] == NULL)
        map_histo[sample_name_short] = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow")->Clone();
      else
      {
        TH1D *histo = (TH1D *)map_fin[sample_name]->Get("Central/Cut_Flow")->Clone();
        map_histo[sample_name_short]->Add(histo);
      }
    }
  }

  color = {{"TTLJ_cb", kRed},
           {"TTLJ_cs", kOrange + 2},
           {"TTLJ_cd", kOrange - 4},
           {"TTLJ_us", kYellow - 10},
           {"TTLJ_ud", kYellow},
           {"TTLL", kSpring},
           {"ST_tw", kMagenta - 0},
           {"ST_tch", kMagenta - 7},
           {"ST_sch", kMagenta - 10},
           {"WJets", kBlue},
           {"DYJets", kBlue - 10},
           {"ttH", kTeal - 9},
           {"ttW", kTeal + 4},
           {"ttZ", kTeal + 0},
           {"VV", kGreen + 4},
           {"QCD_bEn", kGray}};

  Draw();
} // Selection::Selection(const TString &a_era, const TString &a_channel)

//////////

Selection::~Selection()
{
} // Selection::~Selection()

//////////

void Selection::Draw()
{
  gStyle->SetOptStat(0);

  cout << "[Selection::Draw]: Init" << endl;

  vector<TString> bin_label = {"No Cut",
                               "Met Filter",
                               "Trigger",
                               "Exactly one Lepton",
                               "At Least Four Jets",
                               "At Leaset Two B-tagged Jets"};

  TLatex *latex = new TLatex();

  float lumi;
  if (era == "2016preVFP")
    lumi = lumi_2016a;
  else if (era == "2016postVFP")
    lumi = lumi_2016a;
  else if (era == "2017")
    lumi = lumi_2017;
  else if (era == "2018")
    lumi = lumi_2018;

  // draw
  TCanvas *canvas[3];
  for (int i = 0; i < 3; i++)
  {
    TString name = "Cutflow_" + to_string(i);
    canvas[i] = new TCanvas(name, name, 1300, 800);
    canvas[i]->Divide(3, 2);
    canvas[i]->Draw();
  }

  // cosmetic
  for (unsigned int i = 0; i < map_histo.size(); i++)
  {
    auto it = map_histo.begin();
    std::advance(it, i);

    TString mc_name = it->first;
    cout << mc_name << " " << map_histo[mc_name]->GetBinContent(bin_label.size()) << endl;

    // map_histo[it->first]

    int canvas_index;
    int subpad_index;

    if (mc_name.Contains("TTLJ"))
    {
      canvas_index = 0;

      if (mc_name.Contains("ud"))
        subpad_index = 1;
      else if (mc_name.Contains("us"))
        subpad_index = 2;
      else if (mc_name.Contains("cd"))
        subpad_index = 4;
      else if (mc_name.Contains("cs"))
        subpad_index = 5;
      else if (mc_name.Contains("cb"))
        subpad_index = 6;
    }
    else if (mc_name.Contains("TTLL"))
    {
      canvas_index = 1;
      subpad_index = 1;
    }
    else if (mc_name.Contains("ST_sch"))
    {
      canvas_index = 1;
      subpad_index = 2;
    }
    else if (mc_name.Contains("ST_tch"))
    {
      canvas_index = 1;
      subpad_index = 3;
    }
    else if (mc_name.Contains("ST_tw"))
    {
      canvas_index = 1;
      subpad_index = 4;
    }
    else if (mc_name.Contains("WJets"))
    {
      canvas_index = 1;
      subpad_index = 5;
    }
    else if (mc_name.Contains("DYJets"))
    {
      canvas_index = 1;
      subpad_index = 6;
    }
    else if (mc_name.Contains("QCD_bEn"))
    {
      canvas_index = 2;
      subpad_index = 1;
    }
    else if (mc_name.Contains("ttH"))
    {
      canvas_index = 2;
      subpad_index = 2;
    }
    else if (mc_name.Contains("ttW"))
    {
      canvas_index = 2;
      subpad_index = 3;
    }
    else if (mc_name.Contains("ttZ"))
    {
      canvas_index = 2;
      subpad_index = 4;
    }
    else if (mc_name.Contains("VV"))
    {
      canvas_index = 2;
      subpad_index = 5;
    }

    map_histo[mc_name]->SetTitle("");
    map_histo[mc_name]->SetName("Cut Flow");
    map_histo[mc_name]->GetXaxis()->SetRange(1, bin_label.size());
    for (unsigned int j = 0; j < bin_label.size(); j++)
      map_histo[mc_name]->GetXaxis()->SetBinLabel(j + 1, bin_label[j]);
    map_histo[mc_name]->GetYaxis()->SetTitle(mc_name);
    map_histo[mc_name]->GetYaxis()->SetTitleOffset(1.5);

    canvas[canvas_index]->cd(subpad_index);
    map_histo[mc_name]->Draw();

    latex->DrawLatexNDC(0.1, 0.91, "CMS Work in progress");
    latex->DrawLatexNDC(0.66, 0.91, Form("L = %.2f /fb", lumi));

    canvas[canvas_index]->cd(subpad_index)->SetLogy();
  }

  for (int i = 0; i < 3; i++)
  {
    TString name = "Cutflow_" + to_string(i) + "_" + era + "_" + channel + "." + extension;
    canvas[i]->Print(name, extension);
  }

  // stack
  THStack *cutflow_stack = new THStack("Cutflow_Stacked", "");

  TLegend *tl = new TLegend(0.5, 0.7, 0.9, 0.9);
  tl->SetNColumns(4);

  // dirty and quick
  map_histo["ttH"]->SetFillColor(color["ttH"]);
  cutflow_stack->Add(map_histo["ttH"]);
  tl->AddEntry(map_histo["ttH"], "ttH", "f");

  map_histo["ttW"]->SetFillColor(color["ttW"]);
  cutflow_stack->Add(map_histo["ttW"]);
  tl->AddEntry(map_histo["ttW"], "ttW", "f");

  map_histo["ttZ"]->SetFillColor(color["ttZ"]);
  cutflow_stack->Add(map_histo["ttZ"]);
  tl->AddEntry(map_histo["ttZ"], "ttZ", "f");

  map_histo["VV"]->SetFillColor(color["VV"]);
  cutflow_stack->Add(map_histo["VV"]);
  tl->AddEntry(map_histo["VV"], "VV", "f");

  map_histo["QCD_bEn"]->SetFillColor(color["QCD_bEn"]);
  cutflow_stack->Add(map_histo["QCD_bEn"]);
  tl->AddEntry(map_histo["QCD_bEn"], "QCD_bEn", "f");

  map_histo["DYJets"]->SetFillColor(color["DYJets"]);
  cutflow_stack->Add(map_histo["DYJets"]);
  tl->AddEntry(map_histo["DYJets"], "DYJets", "f");

  map_histo["WJets"]->SetFillColor(color["WJets"]);
  cutflow_stack->Add(map_histo["WJets"]);
  tl->AddEntry(map_histo["WJets"], "WJets", "f");

  map_histo["ST_sch"]->SetFillColor(color["ST_sch"]);
  cutflow_stack->Add(map_histo["ST_sch"]);
  tl->AddEntry(map_histo["ST_sch"], "ST_sch", "f");

  map_histo["ST_tch"]->SetFillColor(color["ST_tch"]);
  cutflow_stack->Add(map_histo["ST_tch"]);
  tl->AddEntry(map_histo["ST_tch"], "ST_tch", "f");

  map_histo["ST_tw"]->SetFillColor(color["ST_tw"]);
  cutflow_stack->Add(map_histo["ST_tw"]);
  tl->AddEntry(map_histo["ST_tw"], "ST_tw", "f");

  map_histo["TTLL"]->SetFillColor(color["TTLL"]);
  cutflow_stack->Add(map_histo["TTLL"]);
  tl->AddEntry(map_histo["TTLL"], "TTLL", "f");

  map_histo["TTLJ_ud"]->SetFillColor(color["TTLJ_ud"]);
  cutflow_stack->Add(map_histo["TTLJ_ud"]);
  tl->AddEntry(map_histo["TTLJ_ud"], "TTLJ_ud", "f");

  map_histo["TTLJ_us"]->SetFillColor(color["TTLJ_us"]);
  cutflow_stack->Add(map_histo["TTLJ_us"]);
  tl->AddEntry(map_histo["TTLJ_us"], "TTLJ_us", "f");

  map_histo["TTLJ_cd"]->SetFillColor(color["TTLJ_cd"]);
  cutflow_stack->Add(map_histo["TTLJ_cd"]);
  tl->AddEntry(map_histo["TTLJ_cd"], "TTLJ_cd", "f");

  map_histo["TTLJ_cs"]->SetFillColor(color["TTLJ_cs"]);
  cutflow_stack->Add(map_histo["TTLJ_cs"]);
  tl->AddEntry(map_histo["TTLJ_cs"], "TTLJ_cs", "f");

  map_histo["TTLJ_cb"]->SetLineColor(color["TTLJ_cb"]);
  map_histo["TTLJ_cb"]->SetLineWidth(2);
  tl->AddEntry(map_histo["TTLJ_cb"], "TTLJ_cb", "l");

  TCanvas *canvas_stack = new TCanvas("Cutflow_Stack", "Cutflow_Stack", 1300, 800);
  canvas_stack->Draw();

  canvas_stack->cd();
  cutflow_stack->Draw("BAR");
  canvas_stack->Update();

  map_histo["TTLJ_cb"]->Draw("SAME");

  cutflow_stack->GetXaxis()->SetRange(1, bin_label.size());
  // cutflow_stack->GetYaxis()->SetLimits(1e2, 1e8);
  cutflow_stack->GetYaxis()->SetTitle("Cutflow Stacked");
  cutflow_stack->SetMinimum(1e3);
  cutflow_stack->SetMaximum(1e12);
  canvas_stack->SetLogy();

  tl->Draw("SAME");

  latex->DrawLatexNDC(0.1, 0.91, "CMS Work in progress");
  latex->DrawLatexNDC(0.74, 0.91, Form("L = %.2f /fb", lumi));

  canvas_stack->Update();

  TString name = "Cutflow_Stack_" + era + "_" + channel + "." + extension;
  canvas_stack->Print(name, extension);

  cout << "[Selection::Draw]: Done" << endl;

  return;
} // void Selection::Draw()

//////////