#include "Modelling_Patch.h"

ClassImp(Modelling_Patch);

//////////

Modelling_Patch::Modelling_Patch(const TString &a_mode)
    : samples("2018", "Mu", "Vcb_Modelling_Patch") // The leading two arguments are jsut place holder
{
  cout << "[Modelling_Patch::Modelling_Patch]: Init analysis" << endl;

  mode = a_mode;

  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    cout << it->first << " " << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  if (mode == "Analysis")
  {
    vec_eras = {"2016preVFP", "2016postVFP", "2017", "2018"};
    // vec_eras = {"2016preVFP"};

    Init_Histo();
    Read_Histo();
    Ratio();
  }
  else if (mode == "Application")
    Read_Ratio();

} // Modelling_Patch::Modelling_Patch()

//////////

Modelling_Patch::~Modelling_Patch()
{
  cout << "[Modelling_Patch::~Modelling_Patch]: Init" << endl;

  if (mode == "Analysis")
  {
    TFile *fout = new TFile("Vcb_Modelling_Patch.root", "RECREATE");

    for (int i = 0; i < n_sample_merge_mc; i++)
    {
      TDirectory *dir = fout->mkdir(vec_short_name_mc[i]);
      dir->cd();

      if (vec_short_name_mc[i].Contains("CP5") || vec_short_name_mc[i].Contains("hdamp") || vec_short_name_mc[i].Contains("mtop"))
      {
        histo_baseline[i]->Write();
      }
      else
      {
        histo_pdf_alternative[i]->Write();
        for (int j = 0; j < 100; j++)
          histo_pdf_error_set[i][j]->Write();
        histo_pdf_as_down[i]->Write();
        histo_pdf_as_up[i]->Write();
        histo_top_pt_reweight[i]->Write();
        histo_scale_variation_1[i]->Write();
        histo_scale_variation_2[i]->Write();
        histo_scale_variation_3[i]->Write();
        histo_scale_variation_4[i]->Write();
        histo_scale_variation_6[i]->Write();
        histo_scale_variation_8[i]->Write();
        histo_ps_0[i]->Write();
        histo_ps_1[i]->Write();
        histo_ps_2[i]->Write();
        histo_ps_3[i]->Write();
      }
    }

    fout->Close();
  } // if (mode == "Analysis")

  cout << "[Modelling_Patch::~Modelling_Patch]: Done" << endl;

} // Modelling_Patch::~Modelling_Patch()

//////////

float Modelling_Patch::Get_Modelling_Patch(const TString &sample, const TString &variation)
{
  auto it_sample = patch_all.find(sample);
  if (it_sample != patch_all.end())
  {
    auto it_variation = it_sample->second.find(sample + "_" + variation);
    if (it_variation != it_sample->second.end())
      return it_variation->second;
    else
      return 1.;
  }
  else
    return 1.;

  return -999;
} // float Modelling_Patch::Get_Modelling_Patch(const TString &sample, const TString &variation)

//////////

int Modelling_Patch::Histo_Index(const TString &sample_name)
{
  int index = -999;

  index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();

  return index;
} // int Modelling_Patch::Histo_Index(const TString &sample_name)

//////////

void Modelling_Patch::Init_Histo()
{
  cout << "[Modelling_Patch::Init_Histo]: Init" << endl;

  histo_baseline = new TH1D *[n_sample_merge_mc];
  histo_pdf_alternative = new TH1D *[n_sample_merge_mc];
  histo_pdf_error_set = new TH1D **[n_sample_merge_mc];
  histo_pdf_as_down = new TH1D *[n_sample_merge_mc];
  histo_pdf_as_up = new TH1D *[n_sample_merge_mc];
  histo_top_pt_reweight = new TH1D *[n_sample_merge_mc];
  histo_scale_variation_1 = new TH1D *[n_sample_merge_mc];
  histo_scale_variation_2 = new TH1D *[n_sample_merge_mc];
  histo_scale_variation_3 = new TH1D *[n_sample_merge_mc];
  histo_scale_variation_4 = new TH1D *[n_sample_merge_mc];
  histo_scale_variation_6 = new TH1D *[n_sample_merge_mc];
  histo_scale_variation_8 = new TH1D *[n_sample_merge_mc];
  histo_ps_0 = new TH1D *[n_sample_merge_mc];
  histo_ps_1 = new TH1D *[n_sample_merge_mc];
  histo_ps_2 = new TH1D *[n_sample_merge_mc];
  histo_ps_3 = new TH1D *[n_sample_merge_mc];
  histo_cp5_down = new TH1D *[n_sample_merge_mc];
  histo_cp5_up = new TH1D *[n_sample_merge_mc];
  histo_hdamp_down = new TH1D *[n_sample_merge_mc];
  histo_hdamp_up = new TH1D *[n_sample_merge_mc];
  histo_mtop_down = new TH1D *[n_sample_merge_mc];
  histo_mtop_up = new TH1D *[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString histo_name = vec_short_name_mc[i] + "_Baseline";
    histo_baseline[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_PDF_Alternative";
    histo_pdf_alternative[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_pdf_error_set[i] = new TH1D *[100];
    for (int j = 0; j < 100; j++)
    {
      histo_name = vec_short_name_mc[i] + "_PDF_Error_Set_" + to_string(j);
      histo_pdf_error_set[i][j] = new TH1D(histo_name, histo_name, 1, 0, 1);
    }

    histo_name = vec_short_name_mc[i] + "_PDF_As_Down";
    histo_pdf_as_down[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_PDF_As_Up";
    histo_pdf_as_up[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Top_Pt_Reweight";
    histo_top_pt_reweight[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_1";
    histo_scale_variation_1[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_2";
    histo_scale_variation_2[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_3";
    histo_scale_variation_3[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_4";
    histo_scale_variation_4[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_6";
    histo_scale_variation_6[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_8";
    histo_scale_variation_8[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_PS_0";
    histo_ps_0[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_PS_1";
    histo_ps_1[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_PS_2";
    histo_ps_2[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_PS_3";
    histo_ps_3[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_CP5_Down";
    histo_cp5_down[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_CP5_Up";
    histo_cp5_up[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_hdamp_Down";
    histo_hdamp_down[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_hdamp_Up";
    histo_hdamp_up[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_mtop_171p5";
    histo_mtop_down[i] = new TH1D(histo_name, histo_name, 1, 0, 1);

    histo_name = vec_short_name_mc[i] + "_mtop_173p5";
    histo_mtop_up[i] = new TH1D(histo_name, histo_name, 1, 0, 1);
  }

  cout << "[Modelling_Patch::Init_Histo]: Done" << endl;

  return;
} // void Modelling_Patch::Init_Histo()

//////////

void Modelling_Patch::Ratio()
{
  cout << "[Modelling_Patch::Ratio]: Init" << endl;

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    if (vec_short_name_mc[i].Contains("CP5") || vec_short_name_mc[i].Contains("hdamp") || vec_short_name_mc[i].Contains("mtop"))
    {
      TString sample_name = vec_short_name_mc[i].Copy();
      cout << sample_name << endl;

      Ssiz_t underscore_pos = sample_name.Last('_');

      if (underscore_pos != kNPOS)
        sample_name.Remove(underscore_pos);

      int index_nominal = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();

      histo_baseline[i]->Divide(histo_baseline[index_nominal], histo_baseline[i]);
      // histo_baseline[i]->Divide();
    }
    else
    {
      histo_pdf_alternative[i]->Divide(histo_baseline[i], histo_pdf_alternative[i]);
      for (int j = 0; j < 100; j++)
        histo_pdf_error_set[i][j]->Divide(histo_baseline[i], histo_pdf_error_set[i][j]);
      histo_pdf_as_down[i]->Divide(histo_baseline[i], histo_pdf_as_down[i]);
      histo_pdf_as_up[i]->Divide(histo_baseline[i], histo_pdf_as_up[i]);
      histo_top_pt_reweight[i]->Divide(histo_baseline[i], histo_top_pt_reweight[i]);
      histo_scale_variation_1[i]->Divide(histo_baseline[i], histo_scale_variation_1[i]);
      histo_scale_variation_2[i]->Divide(histo_baseline[i], histo_scale_variation_2[i]);
      histo_scale_variation_3[i]->Divide(histo_baseline[i], histo_scale_variation_3[i]);
      histo_scale_variation_4[i]->Divide(histo_baseline[i], histo_scale_variation_4[i]);
      histo_scale_variation_6[i]->Divide(histo_baseline[i], histo_scale_variation_6[i]);
      histo_scale_variation_8[i]->Divide(histo_baseline[i], histo_scale_variation_8[i]);
      histo_ps_0[i]->Divide(histo_baseline[i], histo_ps_0[i]);
      histo_ps_1[i]->Divide(histo_baseline[i], histo_ps_1[i]);
      histo_ps_2[i]->Divide(histo_baseline[i], histo_ps_2[i]);
      histo_ps_3[i]->Divide(histo_baseline[i], histo_ps_3[i]);
    }
  }

  cout << "[Modelling_Patch::Ratio]: Done" << endl;

  return;
} // void Modelling_Patch::Ratio()

//////////

void Modelling_Patch::Read_Histo()
{
  cout << "[Modelling_Patch::Read_Histo]: Init" << endl;

  TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");

  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    const auto &sample_name = it->first;
    cout << sample_name << endl;

    int histo_index = Histo_Index(samples.map_short_name_mc[sample_name]);

    for (const auto &era : vec_eras)
    {
      cout << "Era: " << era << endl;

      TString path = path_sample_base + era + "/Modelling_Patch/" + samples.map_mc[sample_name];
      TFile *fin = new TFile(path);

      TH1D *histo_to_add_baseline = (TH1D *)fin->Get("Baseline");
      histo_baseline[histo_index]->Add(histo_to_add_baseline);

      if (sample_name.Contains("CP5") || sample_name.Contains("hdamp") || sample_name.Contains("mtop"))
        continue;

      TH1D *histo_to_add_pdf_alternative = (TH1D *)fin->Get("PDF_Alternative");
      histo_pdf_alternative[histo_index]->Add(histo_to_add_pdf_alternative);

      for (int i = 0; i < 100; i++)
      {
        TH1D *histo_to_add_pdf_error_set = (TH1D *)fin->Get(("PDF_Error_Set_" + to_string(i + 1)).c_str());
        histo_pdf_error_set[histo_index][i]->Add(histo_to_add_pdf_error_set);
      }

      TH1D *histo_to_add_pdf_as_down = (TH1D *)fin->Get("PDF_As_Down");
      histo_pdf_as_down[histo_index]->Add(histo_to_add_pdf_as_down);

      TH1D *histo_to_add_pdf_as_up = (TH1D *)fin->Get("PDF_As_Up");
      histo_pdf_as_up[histo_index]->Add(histo_to_add_pdf_as_up);

      TH1D *histo_to_add_top_pt_reweight = (TH1D *)fin->Get("Top_Pt_Reweight");
      histo_top_pt_reweight[histo_index]->Add(histo_to_add_top_pt_reweight);

      TH1D *histo_to_add_scale_variation_1 = (TH1D *)fin->Get("Scale_Variation_1");
      histo_scale_variation_1[histo_index]->Add(histo_to_add_scale_variation_1);

      TH1D *histo_to_add_scale_variation_2 = (TH1D *)fin->Get("Scale_Variation_2");
      histo_scale_variation_2[histo_index]->Add(histo_to_add_scale_variation_2);

      TH1D *histo_to_add_scale_variation_3 = (TH1D *)fin->Get("Scale_Variation_3");
      histo_scale_variation_3[histo_index]->Add(histo_to_add_scale_variation_3);

      TH1D *histo_to_add_scale_variation_4 = (TH1D *)fin->Get("Scale_Variation_4");
      histo_scale_variation_4[histo_index]->Add(histo_to_add_scale_variation_4);

      TH1D *histo_to_add_scale_variation_6 = (TH1D *)fin->Get("Scale_Variation_6");
      histo_scale_variation_6[histo_index]->Add(histo_to_add_scale_variation_6);

      TH1D *histo_to_add_scale_variation_8 = (TH1D *)fin->Get("Scale_Variation_8");
      histo_scale_variation_8[histo_index]->Add(histo_to_add_scale_variation_8);

      TH1D *histo_to_add_ps_0 = (TH1D *)fin->Get("PS_0");
      histo_ps_0[histo_index]->Add(histo_to_add_ps_0);

      TH1D *histo_to_add_ps_1 = (TH1D *)fin->Get("PS_1");
      histo_ps_1[histo_index]->Add(histo_to_add_ps_1);

      TH1D *histo_to_add_ps_2 = (TH1D *)fin->Get("PS_2");
      histo_ps_2[histo_index]->Add(histo_to_add_ps_2);

      TH1D *histo_to_add_ps_3 = (TH1D *)fin->Get("PS_3");
      histo_ps_3[histo_index]->Add(histo_to_add_ps_3);

      fin->Close();
    } // for (const auto &era : vec_eras)
  } // loop over n_samples

  cout << "[Modelling_Patch::Read_Histo]: Done" << endl;

} // void Modelling_Patch::Read_Histo()

//////////

void Modelling_Patch::Read_Ratio()
{
  cout << "[Modelling_Patch::Read_Ratio]: Init" << endl;

  TString path = getenv("Vcb_Post_Analysis_WD");
  path += "/Corrections/Vcb_Modelling_Patch.root";
  TFile *fin = new TFile(path);

  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString sample_name = vec_short_name_mc[i];
    cout << sample_name << endl;

    TDirectory *dir = (TDirectory *)fin->Get(sample_name);
    TList *list = dir->GetListOfKeys();
    vector<TString> vec_variation_name;
    Setup_Name(list, vec_variation_name);

    map<TString, float> patch;
    for (int j = 0; j < vec_variation_name.size(); j++)
    {
      TString variation_name = vec_variation_name[j];
      TH1D *ratio = (TH1D *)fin->Get(sample_name + "/" + variation_name);

      patch.insert({variation_name, ratio->GetBinContent(1)});
    }

    patch_all.insert({sample_name, patch});
  }

  fin->Close();

  cout << "[Modelling_Patch::Read_Ratio]: Done" << endl;

  return;
} // void Modelling_Patch::Read_Ratio()

//////////

void Modelling_Patch::Setup_Name(const TList *list, vector<TString> &vec_name)
{
  int n_entries = list->GetEntries();

  for (int i = 0; i < n_entries; i++)
  {
    TDirectory *dir = (TDirectory *)list->At(i);
    TString name = dir->GetName();

    vec_name.push_back(name);
  }

  return;
} // void Modelling_Patch::Setup_Name(const TList *list, vector<TString> &vec_name)

//////////