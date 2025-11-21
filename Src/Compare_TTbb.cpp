#include "Compare_TTbb.h"

ClassImp(Compare_TTbb);

//////////

Compare_TTbb::Compare_TTbb(const TString &a_era, const TString &a_channel, const TString &a_tagger, const bool &a_use_spanet)
    : samples(a_era, a_channel, "Vcb"), tagging_rf(a_era)
{
  cout << "[Compare_TTbb::Compare_TTbb]: Init analysis" << endl;

  ROOT::EnableImplicitMT(4);

  reduction = 1;

  era = a_era;
  channel = a_channel;
  tagger = a_tagger;
  use_spanet = a_use_spanet;

  TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  TString path = path_sample_base + era;
  if (use_spanet)
  {
    if (tagger == "B")
      path += "/Vcb_BTag_SPANet/Central_Syst/";
    else if (tagger == "C")
      path += "/Vcb_CTag_SPANet/Central_Syst/";
  }
  else
  {
    if (tagger == "B")
      path += "/Vcb_BTag/Central_Syst/";
    else if (tagger == "C")
      path += "/Vcb_CTag/Central_Syst/";
  }

  fin_5f = new TFile(path + samples.map_mc["TTLJ"]);
  fin_4f = new TFile(path + samples.map_mc["TTLJ_TTbb_4f"]);
  fin_dps = new TFile(path + samples.map_mc["TTLJ_bbDPS"]);

  fin_template_5f = new TFile(path + "7Class_BigModel/" + samples.map_mc["TTLJ"]);
  fin_template_4f = new TFile(path + "7Class_BigModel/" + samples.map_mc["TTLJ_TTbb_4f"]);
  fin_template_dps = new TFile(path + "7Class_BigModel/" + samples.map_mc["TTLJ_bbDPS"]);

  tree_5f = (TTree *)fin_5f->Get(channel + "/Central/Result_Tree");
  tree_4f = (TTree *)fin_4f->Get(channel + "/Central/Result_Tree");
  tree_dps = (TTree *)fin_dps->Get(channel + "/Central/Result_Tree");

  template_tree_5f = (TTree *)fin_template_5f->Get(channel + "/Central/Result_Tree");
  template_tree_4f = (TTree *)fin_template_4f->Get(channel + "/Central/Result_Tree");
  template_tree_dps = (TTree *)fin_template_dps->Get(channel + "/Central/Result_Tree");

  tree_5f->AddFriend(template_tree_5f);
  tree_4f->AddFriend(template_tree_4f);
  tree_dps->AddFriend(template_tree_dps);

  event.Setup_Tree(tree_5f, Syst::Central, false, false);
  event.Setup_Tree(tree_4f, Syst::Central, false, false);
  event.Setup_Tree(tree_dps, Syst::Central, false, false);

  vec_short_name_mc = {"TTLJ_5f_BJ_2", "TTLJ_4f_BJ_2",
                       "TTLJ_5f_BB_2", "TTLJ_4f_BB_2",
                       "TTLJ_5f_BJ_4", "TTLJ_4f_BJ_4",
                       "TTLJ_5f_BB_4", "TTLJ_4f_BB_4"};
  n_sample_merge_mc = vec_short_name_mc.size();

  map_tree["TTLJ"] = tree_5f;
  map_tree["TTLJ_TTbb_4f"] = tree_4f;
  map_tree["TTLJ_bbDPS"] = tree_dps;

  Init_Histo();
  Read_Tree();
  Draw();
} // Compare_TTbb::Compare_TTbb(const TString &a_era, const TString &a_channel, const TString &a_tagger)

//////////

Compare_TTbb::~Compare_TTbb()
{

} // Compare_TTbb::~Compare_TTbb()

//////////

void Compare_TTbb::Draw()
{
  gROOT->SetBatch(kTRUE);

  TCanvas *canvas = new TCanvas("canvas", "canvas", 1600, 1000);
  canvas->Divide(4, 1);

  TPad ***pad = new TPad **[4];
  TH1D **histo_ratio = new TH1D *[4];
  for (int i = 0; i < 4; i++)
  {
    pad[i] = new TPad *[2];
    for (int j = 0; j < 2; j++)
    {
      // Absolute
      canvas->cd(i + 1);
      pad[i][0] = new TPad("Pad_Absolute", "Pad_Absolute", 0, 0.3, 1, 1);
      pad[i][0]->Draw();
      pad[i][0]->cd();

      TString histo_name;
      if (i == 0)
        histo_name = "TT+BJ, W#rightarrowud (us)";
      else if (i == 1)
        histo_name = "TT+BB, W#rightarrowud (us)";
      else if (i == 2)
        histo_name = "TT+BJ, W#rightarrowcs (cd)";
      else if (i == 3)
        histo_name = "TT+BB, W#rightarrowcs (cd)";

      histo[2 * i]->SetTitle(histo_name);
      histo[2 * i]->Draw();

      histo[2 * i + 1]->SetLineColor(2);
      histo[2 * i + 1]->Draw("same");

      if (i == 0)
      {
        TLegend *tl = new TLegend(0.65, 0.75, 0.9, 0.9);
        tl->AddEntry(histo[2 * i], "5f", "lep");
        tl->AddEntry(histo[2 * i + 1], "4f+DPS", "lep");
        tl->Draw("same");
      }

      // Ratio
      canvas->cd(i + 1);
      pad[i][1] = new TPad("Pad_Ratio", "Pad_Ratio", 0, 0.0, 1, 0.3);
      pad[i][1]->Draw();
      pad[i][1]->cd();

      histo_ratio[i] = new TH1D("histo_ratio", "histo_ratio", 90, 0.1, 1);
      histo_ratio[i]->Divide(histo[2 * i + 1], histo[2 * i]);
      histo_ratio[i]->GetYaxis()->SetTitle("4f/5f");
      histo_ratio[i]->Draw();
    }
  }

  if (use_spanet)
    canvas->Print(Form("5f_Vs_4f_%s_tagger_SPANet.png", tagger.Data()), "png");
  else
    canvas->Print(Form("5f_Vs_4f_%s_tagger_XGBoost.png", tagger.Data()), "png");

  return;
} // Compare_TTbb::Draw()

//////////

void Compare_TTbb::Fill_Histo_MC(const TString &sample_name)
{
  int abcd_region_index = Set_ABCD_Region();
  if (abcd_region_index != 3)
    return;

  int histo_index = Histo_Index(sample_name);
  if (histo_index == -999)
    return;

  TString histo_name_rf = Histo_Name_RF(sample_name);

  float weight_ckm = Reweight_CKM(sample_name);
  float weight_tthf = Reweight_TTHF(sample_name);

  event.weight = 1;
  event.weight *= event.weight_lumi;
  event.weight *= event.weight_mc;
  event.weight *= event.weight_hem_veto;
  event.weight *= weight_ckm;
  event.weight *= weight_tthf;
  event.weight *= event.weight_pileup;
  event.weight *= event.weight_prefire;
  event.weight *= event.weight_pujet_veto;
  event.weight *= event.weight_top_pt;
  event.weight *= event.weight_sl_trig;
  event.weight *= event.weight_mu_id;
  event.weight *= event.weight_mu_iso;
  event.weight *= event.weight_el_id;
  event.weight *= event.weight_el_reco;

  if (tagger == "B")
  {
    event.weight *= event.weight_b_tag;
    event.weight *= tagging_rf.Get_Tagging_RF_B_Tag("D", histo_name_rf, "B_Tag_Nominal", event.vec_jet_pt, event.vec_jet_eta, event.vec_jet_flavor);
  }
  else if (tagger == "C")
  {
    event.weight *= event.weight_c_tag;
    event.weight *= tagging_rf.Get_Tagging_RF_C_Tag("D", histo_name_rf, "C_Tag_Nominal", event.vec_jet_pt, event.vec_jet_eta, event.vec_jet_flavor);
  }

  histo[histo_index]->Fill(event.template_score, event.weight);

  return;
} // void Compare_TTbb::Fill_Histo_MC(const TString &sample_name)

//////////

int Compare_TTbb::Histo_Index(const TString &sample_name)
{
  int index = -999;

  TString histo_name;
  if (sample_name == "TTLJ")
    histo_name = "TTLJ_5f";
  else if (sample_name == "TTLJ_TTbb_4f" || sample_name == "TTLJ_bbDPS")
    histo_name = "TTLJ_4f";

  int gen_ttbar_id = event.gen_ttbar_id % 100;
  if (gen_ttbar_id == 51 || gen_ttbar_id == 52)
    histo_name += "_BJ";
  else if (gen_ttbar_id == 53 || gen_ttbar_id == 54 || gen_ttbar_id == 55)
    histo_name += "_BB";
  else
    return -999;

  if (event.decay_mode == 21 || event.decay_mode == 23)
    histo_name += "_2";
  else if (event.decay_mode == 41 || event.decay_mode == 43)
    histo_name += "_4";
  else if (event.decay_mode == 45)
    histo_name += "_45";

  // cout << "Histo_Name = " << histo_name << endl;

  index = find(vec_short_name_mc.begin(), vec_short_name_mc.end(), histo_name) - vec_short_name_mc.begin();

  return index;
} // int Compare_TTbb::Histo_Index(const TString &sample_name)

//////////

TString Compare_TTbb::Histo_Name_RF(const TString &sample_name)
{
  TString histo_name_rf = sample_name;

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

  return histo_name_rf;
} // TString Compare_TTbb::Histo_Name_RF(const TString &sample_name)

//////////

void Compare_TTbb::Init_Histo()
{
  cout << "[Compare_TTbb::Init_Histo]: Init histo" << endl;

  histo = new TH1D *[n_sample_merge_mc];
  for (int i = 0; i < n_sample_merge_mc; i++)
  {
    histo[i] = new TH1D(vec_short_name_mc[i], vec_short_name_mc[i], 90, 0.1, 1);
  }

  cout << "[Compare_TTbb::Init_Histo]: Done" << endl;

  return;
} // void Compare_TTbb::Init_Histo()

//////////

void Compare_TTbb::Read_Tree()
{
  cout << "[Compare_TTbb::Read_Tree]: Init" << endl;

  for (auto it = map_tree.begin(); it != map_tree.end(); it++)
  {
    TString sample_name = it->first;
    cout << "Sample_Name = " << sample_name << endl;

    Long64_t n_entries = it->second->GetEntries();
    n_entries /= reduction;
    cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

    for (Long64_t i = 0; i < n_entries; i++)
    {
      if (i % 500000 == 0)
        cout << "Processing... " << i << "/" << n_entries << endl;

      it->second->GetEntry(i);

      event.template_score = event.Multi_To_One();

      if (event.template_score < 0.1)
        continue;

      Fill_Histo_MC(sample_name);

    } // loop over entries
  } // for (auto it = map_tree->begin(); it != map_tree->end(); it++)

  cout << "[Compare_TTbb::Read_Tree]: Done" << endl;

  return;
} // void Compare_TTbb::Read_Tree()

//////////

float Compare_TTbb::Reweight_CKM(const TString &sample_name)
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
} // float Compare_TTbb::Reweight_CKM(const TString& sample_name)

//////////

float Compare_TTbb::Reweight_TTHF(const TString &sample_name)
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
} // float Compare_TTbb::Reweight_TTHF(const TString &sample_name)

//////////

int Compare_TTbb::Set_ABCD_Region()
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
} // int Compare_TTbb::Set_ABCD_Region()

//////////