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
    // cout << it->first << " " << it->second << endl;
    vec_short_name_mc.push_back(it->second);
  }
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  // flavor separation
  vector<TString> vec_flavor_separation;
  for (int i = 0; i < vec_short_name_mc.size(); i++)
  {
    TString name = vec_short_name_mc[i];
    if (name.Contains("TTLJ") || name.Contains("TTLL"))
    {
      vec_flavor_separation.push_back(name + "_CC");
      vec_flavor_separation.push_back(name + "_BB");
    }
  }
  vec_short_name_mc.insert(vec_short_name_mc.end(), vec_flavor_separation.begin(), vec_flavor_separation.end());

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());

  n_sample_merge_mc = vec_short_name_mc.size();
  cout << "n_sample_merge_mc = " << n_sample_merge_mc << endl;

  if (mode == "Analysis")
  {
    vec_eras = {"2016preVFP", "2016postVFP", "2017", "2018"};
    //vec_eras = {"2016preVFP"};

    Init_Histo();
    Read_Histo();
    Ratio();
  }
  else if (mode == "Application")
  {
    Read_Ratio();
    // cout << "test " << Get_Modelling_Patch("TTLL_mtop173p5", "Baseline") << endl;
  }
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

    delete fout;
  } // if (mode == "Analysis")

  cout << "[Modelling_Patch::~Modelling_Patch]: Done" << endl;

} // Modelling_Patch::~Modelling_Patch()

//////////

float Modelling_Patch::Get_Modelling_Patch(const TString &sample, const TString &variation) const
{
  auto it_sample = patch_all.find(sample);
  if (it_sample != patch_all.end())
  {
    TString variation_may_swap = variation;

    if (variation_may_swap.Contains("Scale_Variation"))
    {
      if (sample.Contains("TTLJ_WtoCB") || sample.Contains("TTLJ") || sample.Contains("TTLL") ||
          sample.Contains("TTLJ_TTbb_4f") || sample.Contains("TTLJ_bbDPS") || sample.Contains("TTLL_TTbb_4f") || sample.Contains("TTLL_bbDPS") ||
          sample == "ST_tch" || sample == "ST_tw" ||
          sample == "ttHTobb" || sample == "ttHToNonbb")
      {
        // index is OK
        // do nothing
      }
      else if (sample == "ST_sch" ||
               sample == "WJets" || sample == "DYJets" ||
               sample == "QCD_bEn" ||
               sample == "ttWToLNu" || sample == "ttWToQQ" ||
               sample == "ttZToLLNuNu" || sample == "ttZToQQ" || sample == "ttZToQQ")
      {
        if (variation_may_swap.Contains("_1"))
          variation_may_swap.ReplaceAll("_1", "_3");
        else if (variation_may_swap.Contains("_3"))
          variation_may_swap.ReplaceAll("_3", "_1");
        else if (variation_may_swap.Contains("_2"))
          variation_may_swap.ReplaceAll("_2", "_6");
        else if (variation_may_swap.Contains("_6"))
          variation_may_swap.ReplaceAll("_6", "_2");
        else if (variation_may_swap.Contains("_5"))
          variation_may_swap.ReplaceAll("_5", "_7");
        else if (variation_may_swap.Contains("_7"))
          variation_may_swap.ReplaceAll("_7", "_5");
      }
      else if (sample == "WW" || sample == "WZ" || sample == "ZZ" ||
               sample.Contains("CP5") || sample.Contains("mtop") || sample.Contains("hdamp"))
      {
        // irrelevant
        // do nothing
      }
      else
        cerr << "[Modelling_Patch::Get_Modelling_Patch]: Unknown sample name: " << sample << endl;
    } // if (varation_may_swap.Contains("Scale_Variation"))

    auto it_variation = it_sample->second.find(variation_may_swap);
    if (it_variation != it_sample->second.end())
      return it_variation->second;
    else
    {
      // cout << "[Modelling_Patch::Get_Modelling_Patch]: No variation found for " << sample << " " << variation << endl;
      return 1.;
    }
  }
  else
  {
    // cout << "[Modelling_Patch::Get_Modelling_Patch]: No sample found" << endl;
    return 1.;
  }

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

  histo_pdf_error_set.resize(n_sample_merge_mc);
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    TString histo_name = vec_short_name_mc[i] + "_Baseline";
    histo_baseline.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_PDF_Alternative";
    histo_pdf_alternative.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    for (int j = 0; j < 100; j++)
    {
      histo_name = vec_short_name_mc[i] + "_PDF_Error_Set_" + to_string(j);
      histo_pdf_error_set[i].push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));
    }

    histo_name = vec_short_name_mc[i] + "_PDF_As_Down";
    histo_pdf_as_down.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_PDF_As_Up";
    histo_pdf_as_up.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Top_Pt_Reweight";
    histo_top_pt_reweight.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_1";
    histo_scale_variation_1.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_2";
    histo_scale_variation_2.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_3";
    histo_scale_variation_3.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_4";
    histo_scale_variation_4.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_6";
    histo_scale_variation_6.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_Scale_Variation_8";
    histo_scale_variation_8.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_PS_0";
    histo_ps_0.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_PS_1";
    histo_ps_1.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_PS_2";
    histo_ps_2.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_PS_3";
    histo_ps_3.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_CP5_Down";
    histo_cp5_down.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_CP5_Up";
    histo_cp5_up.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_hdamp_Down";
    histo_hdamp_down.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_hdamp_Up";
    histo_hdamp_up.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_mtop_171p5";
    histo_mtop_down.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));

    histo_name = vec_short_name_mc[i] + "_mtop_173p5";
    histo_mtop_up.push_back(make_unique<TH1D>(histo_name, histo_name, 1, 0, 1));
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
      // cout << sample_name << endl;

      if (sample_name.Contains("_CC") || sample_name.Contains("_BB"))
      {
        int last_underscore = sample_name.Last('_');

        TString temp = sample_name(0, last_underscore);
        int second_to_last_underscore = temp.Last('_');

        sample_name.Remove(second_to_last_underscore, last_underscore - second_to_last_underscore);
      }
      else
      {
        Ssiz_t underscore_pos = sample_name.Last('_');

        if (underscore_pos != kNPOS)
          sample_name.Remove(underscore_pos);
      }

      cout << "nominal sample name: " << sample_name << endl;

      int index_nominal = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), sample_name) - vec_short_name_mc.begin();

      // this part is wrong
      // But let's leave it since it is not used in the analysis
      cout << "histo_baseline: " << histo_baseline[index_nominal]->GetBinContent(1) << ", " << histo_baseline[i]->GetBinContent(1);
      histo_baseline[i]->Divide(histo_baseline[index_nominal].get(), histo_baseline[i].get());
      cout << ", after histo_baseline: " << histo_baseline[i]->GetBinContent(1) << " +- " << histo_baseline[i]->GetBinError(1) <<  endl;
    }
    else
    {
      histo_pdf_alternative[i]->Divide(histo_baseline[i].get(), histo_pdf_alternative[i].get());
      for (int j = 0; j < 100; j++)
        histo_pdf_error_set[i][j]->Divide(histo_baseline[i].get(), histo_pdf_error_set[i][j].get());
      histo_pdf_as_down[i]->Divide(histo_baseline[i].get(), histo_pdf_as_down[i].get());
      histo_pdf_as_up[i]->Divide(histo_baseline[i].get(), histo_pdf_as_up[i].get());
      histo_top_pt_reweight[i]->Divide(histo_baseline[i].get(), histo_top_pt_reweight[i].get());
      histo_scale_variation_1[i]->Divide(histo_baseline[i].get(), histo_scale_variation_1[i].get());
      histo_scale_variation_2[i]->Divide(histo_baseline[i].get(), histo_scale_variation_2[i].get());
      histo_scale_variation_3[i]->Divide(histo_baseline[i].get(), histo_scale_variation_3[i].get());
      histo_scale_variation_4[i]->Divide(histo_baseline[i].get(), histo_scale_variation_4[i].get());
      histo_scale_variation_6[i]->Divide(histo_baseline[i].get(), histo_scale_variation_6[i].get());
      histo_scale_variation_8[i]->Divide(histo_baseline[i].get(), histo_scale_variation_8[i].get());
      histo_ps_0[i]->Divide(histo_baseline[i].get(), histo_ps_0[i].get());
      histo_ps_1[i]->Divide(histo_baseline[i].get(), histo_ps_1[i].get());
      histo_ps_2[i]->Divide(histo_baseline[i].get(), histo_ps_2[i].get());
      histo_ps_3[i]->Divide(histo_baseline[i].get(), histo_ps_3[i].get());
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
    const auto &sample_name_short = samples.map_short_name_mc[sample_name];
    cout << sample_name << ", " << sample_name_short << endl;

    vector<TString> vec_histo_name = Short_Name_Group(sample_name_short);
    // for (const auto &histo_name : vec_histo_name)
    //   cout << "   " << histo_name << endl;

    for (const auto &era : vec_eras)
    {
      cout << "Era: " << era << endl;

      TString path = path_sample_base + era + "/Modelling_Patch/" + samples.map_mc[sample_name];
      TFile *fin = new TFile(path);

      TString full_name = samples.map_mc[sample_name];
      full_name.ReplaceAll("Vcb_Modelling_Patch_", "");
      full_name.ReplaceAll(".root", "");

      for (int i = 0; i < vec_histo_name.size(); i++)
      {
        cout << vec_histo_name[i] << endl;

        int histo_index = Histo_Index(vec_histo_name[i]);

        TString flavor;
        if (vec_histo_name[i].Contains("_BB"))
          flavor = "_BB";
        else if (vec_histo_name[i].Contains("_CC"))
          flavor = "_CC";
        else
          flavor = "";

        TH1D *histo_to_add_baseline = (TH1D *)fin->Get(full_name + flavor + "_Baseline");
        histo_baseline[histo_index]->Add(histo_to_add_baseline);

        if (sample_name.Contains("CP5") || sample_name.Contains("hdamp") || sample_name.Contains("mtop"))
          continue;

        TH1D *histo_to_add_pdf_alternative = (TH1D *)fin->Get(full_name + flavor + "_PDF_Alternative");
        histo_pdf_alternative[histo_index]->Add(histo_to_add_pdf_alternative);

        for (int i = 0; i < 100; i++)
        {
          TH1D *histo_to_add_pdf_error_set = (TH1D *)fin->Get(full_name + flavor + ("_PDF_Error_Set_" + to_string(i + 1)).c_str());
          histo_pdf_error_set[histo_index][i]->Add(histo_to_add_pdf_error_set);
        }

        TH1D *histo_to_add_pdf_as_down = (TH1D *)fin->Get(full_name + flavor + "_PDF_As_Down");
        histo_pdf_as_down[histo_index]->Add(histo_to_add_pdf_as_down);

        TH1D *histo_to_add_pdf_as_up = (TH1D *)fin->Get(full_name + flavor + "_PDF_As_Up");
        histo_pdf_as_up[histo_index]->Add(histo_to_add_pdf_as_up);

        TH1D *histo_to_add_top_pt_reweight = (TH1D *)fin->Get(full_name + flavor + "_Top_Pt_Reweight");
        histo_top_pt_reweight[histo_index]->Add(histo_to_add_top_pt_reweight);

        TH1D *histo_to_add_scale_variation_1 = (TH1D *)fin->Get(full_name + flavor + "_Scale_Variation_1");
        histo_scale_variation_1[histo_index]->Add(histo_to_add_scale_variation_1);

        TH1D *histo_to_add_scale_variation_2 = (TH1D *)fin->Get(full_name + flavor + "_Scale_Variation_2");
        histo_scale_variation_2[histo_index]->Add(histo_to_add_scale_variation_2);

        TH1D *histo_to_add_scale_variation_3 = (TH1D *)fin->Get(full_name + flavor + "_Scale_Variation_3");
        histo_scale_variation_3[histo_index]->Add(histo_to_add_scale_variation_3);

        TH1D *histo_to_add_scale_variation_4 = (TH1D *)fin->Get(full_name + flavor + "_Scale_Variation_4");
        histo_scale_variation_4[histo_index]->Add(histo_to_add_scale_variation_4);

        TH1D *histo_to_add_scale_variation_6 = (TH1D *)fin->Get(full_name + flavor + "_Scale_Variation_6");
        histo_scale_variation_6[histo_index]->Add(histo_to_add_scale_variation_6);

        TH1D *histo_to_add_scale_variation_8 = (TH1D *)fin->Get(full_name + flavor + "_Scale_Variation_8");
        histo_scale_variation_8[histo_index]->Add(histo_to_add_scale_variation_8);

        TH1D *histo_to_add_ps_0 = (TH1D *)fin->Get(full_name + flavor + "_PS_0");
        histo_ps_0[histo_index]->Add(histo_to_add_ps_0);

        TH1D *histo_to_add_ps_1 = (TH1D *)fin->Get(full_name + flavor + "_PS_1");
        histo_ps_1[histo_index]->Add(histo_to_add_ps_1);

        TH1D *histo_to_add_ps_2 = (TH1D *)fin->Get(full_name + flavor + "_PS_2");
        histo_ps_2[histo_index]->Add(histo_to_add_ps_2);

        TH1D *histo_to_add_ps_3 = (TH1D *)fin->Get(full_name + flavor + "_PS_3");
        histo_ps_3[histo_index]->Add(histo_to_add_ps_3);
      } // loop over histo_name

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
    // cout << sample_name << endl;

    TDirectory *dir = (TDirectory *)fin->Get(sample_name);
    TList *list = dir->GetListOfKeys();
    vector<TString> vec_variation_name;
    Setup_Name(list, vec_variation_name);

    map<TString, float> patch;
    for (int j = 0; j < vec_variation_name.size(); j++)
    {
      TString variation_name = vec_variation_name[j];
      TH1D *ratio = (TH1D *)fin->Get(sample_name + "/" + variation_name);

      variation_name = variation_name.Remove(0, sample_name.Length() + 1);

      // cout << variation_name << " " << ratio->GetBinContent(1) << endl;

      patch.insert({variation_name, ratio->GetBinContent(1)});
    }

    patch_all.insert({sample_name, patch});
  }

  fin->Close();
  delete fin;

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

vector<TString> Modelling_Patch::Short_Name_Group(const TString &group_name)
{
  vector<TString> vec_name_group;

  if (group_name.Contains("TTLJ") || group_name.Contains("TTLL"))
  {
    vec_name_group.push_back(group_name);
    vec_name_group.push_back(group_name + "_CC");
    vec_name_group.push_back(group_name + "_BB");
  }
  else
    vec_name_group.push_back(group_name);

  return vec_name_group;
} // vector<TString> Modelling_Patch::Short_Name_Group(const TString &short_name)

//////////