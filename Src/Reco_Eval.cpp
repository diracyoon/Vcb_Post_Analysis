#include "Reco_Eval.h"

ClassImp(Reco_Eval);

//////////

Reco_Eval::Reco_Eval(const TString &a_era, const TString &a_channel, const TString &a_swap_mode, const TString &a_draw_extension) : samples(a_era, a_channel), event(a_era, a_channel, a_swap_mode), tagging_rf(a_era)
{
  ROOT::EnableImplicitMT(4);

  cout << "[Reco_Eval::Reco_Eval]: Init analysis" << endl;

  reduction = 1;

  era = a_era;
  channel = a_channel;
  draw_extension = a_draw_extension;

  if (era == "2018")
  {
    bvsc_wp_m = bvsc_wp_m_2018;
    cvsb_wp_m = cvsb_wp_m_2018;
    cvsl_wp_m = cvsl_wp_m_2018;
  }

  TString path_base = getenv("Vcb_Post_Analysis_WD");
  path_base += "/Sample/" + era + "/" + channel + "/RunResult/Central_Syst/";

  if (samples.map_mc.size() != samples.map_short_name_mc.size())
  {
    cout << "size of samples::maps is not matched. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  cout << "Number of MC = " << samples.map_mc.size() << endl;
  for (auto it = samples.map_mc.begin(); it != samples.map_mc.end(); it++)
  {
    cout << it->first << endl;
    // if (it->first != "TTLJ_WtoCB_powheg" && it->first != "TTLJ_powheg")
    //  continue;
    
    map_fin[it->first] = new TFile(path_base + it->second);
  
    TString key;
    if (channel == "Mu")
      key = "POGTightWithTightIso_Central/Result_Tree";
    else if (channel == "El")
      key = "passMVAID_iso_WP80_Central/Result_Tree";

    map_tree[it->first] = (TTree *)map_fin[it->first]->Get(key);
    event.Setup_Tree(map_tree[it->first], false);
  }

  for (auto it = samples.map_short_name_mc.begin(); it != samples.map_short_name_mc.end(); it++)
  {
    cout << it->first << endl;
    vec_short_name_mc.push_back(it->second);
  }

  // remove redundancy
  sort(vec_short_name_mc.begin(), vec_short_name_mc.end(), Comparing_TString);
  vec_short_name_mc.erase(unique(vec_short_name_mc.begin(), vec_short_name_mc.end()), vec_short_name_mc.end());
  n_sample_merge_mc = vec_short_name_mc.size();

  for (auto it = vec_short_name_mc.begin(); it != vec_short_name_mc.end(); it++)
  {
    if (*it != "TTLJ" && *it != "TTLJ_WtoCB")
      vec_histo_sample.push_back(*it);
  }
  vec_histo_sample.push_back("TTLJ_ud");
  vec_histo_sample.push_back("TTLJ_us");
  vec_histo_sample.push_back("TTLJ_cd");
  vec_histo_sample.push_back("TTLJ_cs");
  vec_histo_sample.push_back("TTLJ_cb");
  n_histo_sample = vec_histo_sample.size();

  cout << "n_sample_merge_mc = " << n_sample_merge_mc << ", n_histo_sample = " << n_histo_sample << endl;

  n_fail_reason = sizeof(fail_reason) / sizeof(TString);
  n_histo_type = n_fail_reason + 1;

  histo_mva_pre = new TH1D **[n_histo_sample];
  histo_mva = new TH1D **[n_histo_sample];
  histo_cutflow = new TH1D **[n_histo_sample];
  histo_had_w = new TH1D **[n_histo_sample];
  histo_had_t = new TH1D **[n_histo_sample];
  histo_lep_w = new TH1D **[n_histo_sample];
  histo_lep_t = new TH1D **[n_histo_sample];
  histo_template = new TH1D **[n_histo_sample];
  histo_count = new TH2D **[n_histo_sample];
  histo_prob = new TH2D *[n_histo_sample];
  histo_swap = new TH1D *[n_histo_sample];
  histo_swap_fix = new TH1D *[n_histo_sample];

  for (int i = 0; i < n_histo_sample; i++)
  {
    histo_mva_pre[i] = new TH1D *[n_histo_type];
    histo_mva[i] = new TH1D *[n_histo_type];
    histo_cutflow[i] = new TH1D *[n_histo_type];
    histo_had_w[i] = new TH1D *[n_histo_type];
    histo_had_t[i] = new TH1D *[n_histo_type];
    histo_lep_w[i] = new TH1D *[n_histo_type];
    histo_lep_t[i] = new TH1D *[n_histo_type];
    histo_template[i] = new TH1D *[n_histo_type];

    histo_count[i] = new TH2D *[n_histo_type];

    TString name_base = vec_histo_sample[i];
    TString name = name_base + "_Prob";
    histo_prob[i] = new TH2D(name, name, 12, 0, 12, 8, 0, 8);

    name = name_base + "_Swap";
    histo_swap[i] = new TH1D(name, name, 2, 0, 2);

    name = name_base + "_Swap_Fixed";
    histo_swap_fix[i] = new TH1D(name, name, 2, 0, 2);

    for (int j = 0; j < n_histo_type; j++)
    {
      if (j == 0)
        name_base += "_Correct";
      else
        name_base += "_Wrong_" + fail_reason[j - 1];

      name = name_base + "_MVA_Pre";
      histo_mva_pre[i][j] = new TH1D(name, name, 100, -1, 1);

      name = name_base + "_MVA";
      histo_mva[i][j] = new TH1D(name, name, 100, -1, 1);

      name = name_base + "_Cutflow";
      histo_cutflow[i][j] = new TH1D(name, name, 10, 0, 10);

      name = name_base + "_Had_W";
      histo_had_w[i][j] = new TH1D(name, name, 60, 20, 140);

      name = name_base + "_Had_T";
      histo_had_t[i][j] = new TH1D(name, name, 100, 70, 270);

      name = name_base + "_Lep_W";
      histo_lep_w[i][j] = new TH1D(name, name, 60, 20, 140);

      name = name_base + "_Lep_T";
      histo_lep_t[i][j] = new TH1D(name, name, 100, 70, 270);

      name = name_base + "_Template";
      histo_template[i][j] = new TH1D(name, name, 100, 0, 1);

      name = name_base + "_Count";
      histo_count[i][j] = new TH2D(name, name, 12, 0, 12, 8, 0, 8);
    }
  }

  dv_histo_conf = {{"MVA_Scores", 100, -1, 1},
                   //{"N_Jets", 20, 0, 20},
                   {"N_BJets", 10, 0, 10},
                   {"N_CJets", 10, 0, 10},
                   {"BvsC_w_u", 100, 0, 1},
                   {"CvsB_w_u", 100, 0, 1},
                   {"CvsL_w_u", 100, 0, 1},
                   {"BvsC_w_d", 100, 0, 1},
                   {"CvsB_w_d", 100, 0, 1},
                   {"CvsL_w_d", 100, 0, 1}};
  n_discriminators = dv_histo_conf.size();

  histo_discriminator = new TH1D ***[n_histo_sample];
  for (int i = 0; i < n_histo_sample; i++)
  {
    histo_discriminator[i] = new TH1D **[n_discriminators];

    for (int j = 0; j < n_discriminators; j++)
    {
      histo_discriminator[i][j] = new TH1D *[n_histo_type];

      for (int k = 0; k < n_histo_type; k++)
      {
        TString name = vec_histo_sample[i] + "_" + dv_histo_conf[j].discriminator;
        if (k == 0)
          name += "_Correct";
        else
          name += "_" + fail_reason[k - 1];

        histo_discriminator[i][j][k] = new TH1D(name, name, dv_histo_conf[j].n_bin, dv_histo_conf[j].x_low, dv_histo_conf[j].x_up);
      }
    }
  }

  gr_significance = new TGraphErrors();

  // output tree for template maker training
  TString fout_name = "Vcb_" + era + "_" + channel + "_Reco_Tree.root";
  fout_tree = new TFile(fout_name, "RECREATE");

  for (int i = 0; i < 5; i++)
  {
    TString name = "Reco_";
    name += to_string(decay_modes[i]);
    out_tree[i] = new TTree(name, name);

    out_tree[i]->Branch("weight", &event.weight);
    out_tree[i]->Branch("n_jets", &event.n_jets);
    out_tree[i]->Branch("n_bjets", &event.n_bjets);
    out_tree[i]->Branch("n_cjets", &event.n_cjets);
    out_tree[i]->Branch("best_mva_score", &event.best_mva_score);

    out_tree[i]->Branch("pt_had_t_b", &event.pt_had_t_b);
    out_tree[i]->Branch("pt_w_u", &event.pt_w_u);
    out_tree[i]->Branch("pt_w_d", &event.pt_w_d);
    out_tree[i]->Branch("pt_lep_t_b", &event.pt_lep_t_b);

    out_tree[i]->Branch("eta_had_t_b", &event.eta_had_t_b);
    out_tree[i]->Branch("eta_w_u", &event.eta_w_u);
    out_tree[i]->Branch("eta_w_d", &event.eta_w_d);
    out_tree[i]->Branch("eta_lep_t_b", &event.eta_lep_t_b);

    out_tree[i]->Branch("bvsc_had_t_b", &event.bvsc_had_t_b);

    out_tree[i]->Branch("bvsc_w_u", &event.bvsc_w_u);
    out_tree[i]->Branch("cvsb_w_u", &event.cvsb_w_u);
    out_tree[i]->Branch("cvsl_w_u", &event.cvsl_w_u);

    out_tree[i]->Branch("bvsc_w_d", &event.bvsc_w_d);
    out_tree[i]->Branch("cvsb_w_d", &event.cvsb_w_d);
    out_tree[i]->Branch("cvsl_w_d", &event.cvsl_w_d);

    out_tree[i]->Branch("bvsc_lep_t_b", &event.bvsc_lep_t_b);

    out_tree[i]->Branch("m_w_u", &event.m_w_u);
    out_tree[i]->Branch("m_w_d", &event.m_w_d);
  }
  /*
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      TString name = "Reco_";
      name += to_string(decay_modes[i]);
      name += "_";

      if (j == 0)
        name += "Correct";
      else
        name += "Fail_" + fail_reason[j - 1];

      cout << name << endl;

      out_tree[i][j] = new TTree(name, name);

      out_tree[i][j]->Branch("weight", &event.weight);
      out_tree[i][j]->Branch("n_jets", &event.n_jets);
      out_tree[i][j]->Branch("n_bjets", &event.n_bjets);
      out_tree[i][j]->Branch("n_cjets", &event.n_cjets);
      out_tree[i][j]->Branch("best_mva_score", &event.best_mva_score);

      out_tree[i][j]->Branch("pt_had_t_b", &event.pt_had_t_b);
      out_tree[i][j]->Branch("pt_w_u", &event.pt_w_u);
      out_tree[i][j]->Branch("pt_w_d", &event.pt_w_d);
      out_tree[i][j]->Branch("pt_lep_t_b", &event.pt_lep_t_b);

      out_tree[i][j]->Branch("eta_had_t_b", &event.eta_had_t_b);
      out_tree[i][j]->Branch("eta_w_u", &event.eta_w_u);
      out_tree[i][j]->Branch("eta_w_d", &event.eta_w_d);
      out_tree[i][j]->Branch("eta_lep_t_b", &event.eta_lep_t_b);

      out_tree[i][j]->Branch("bvsc_had_t_b", &event.bvsc_had_t_b);

      out_tree[i][j]->Branch("bvsc_w_u", &event.bvsc_w_u);
      out_tree[i][j]->Branch("cvsb_w_u", &event.cvsb_w_u);
      out_tree[i][j]->Branch("cvsl_w_u", &event.cvsl_w_u);

      out_tree[i][j]->Branch("bvsc_w_d", &event.bvsc_w_d);
      out_tree[i][j]->Branch("cvsb_w_d", &event.cvsb_w_d);
      out_tree[i][j]->Branch("cvsl_w_d", &event.cvsl_w_d);

      out_tree[i][j]->Branch("bvsc_lep_t_b", &event.bvsc_lep_t_b);

      out_tree[i][j]->Branch("m_w_u", &event.m_w_u);
      out_tree[i][j]->Branch("m_w_d", &event.m_w_d);
    } // loop over reco category
  }   // loop over decay modes
  */
} // Reco_Eval::Reco_Eval(const TString& a_era="2018", const TString& a_channel="Mu")

//////////

Reco_Eval::~Reco_Eval()
{
  fout_tree->cd();
  for (int i = 0; i < n_histo_sample; i++)
  {
    for (int j = 0; j < n_discriminators; j++)
    {
      // stack_dv[i][j]->Write();
    }
  }

  fout_tree->cd();
  for (int i = 0; i < 5; i++)
  {
    out_tree[i]->Write();
    /*
    for (int j = 0; j < 5; j++)
    {
      out_tree[i][j]->Write();
    }
    */
  }

  fout_tree->Close();
} // Reco_Eval::~Reco_Eval()

//////////

void Reco_Eval::Run()
{
  Read_Tree();
  Calculate_Significance();
  Calculate_Prob();
  Draw_Raw();
  //  Draw_Sample_By_Sample();
  //  Draw_Significance();
  Draw_Swap();
  Draw_DV();
} // void Reco_Eval::Run()

//////////

void Reco_Eval::Calculate_Significance()
{
  cout << "[Reco_Eval::Calculate_Significance]: start calculation" << endl;

  int n_cut_bin = histo_cutflow[0][0]->GetNbinsX();
  for (int i = 0; i < n_cut_bin; i++)
  {
    float count_signal = 0;
    float count_background = 0;

    for (int j = 0; j < n_histo_sample; j++)
    {
      cout << vec_histo_sample[j] << endl;
      for (int k = 0; k < 5; k++)
      {
        float count = histo_cutflow[j][k]->GetBinContent(i + 1);

        if (vec_histo_sample[j] == "TTLJ_cb" && k == 0)
          count_signal += count;
        else
          count_background += count;
      } // correct or wrong
    }   // loop over samples

    float count_total = count_signal + count_background;

    float significance = count_signal / TMath::Sqrt(count_total);

    float significance_error = count_signal / count_total;
    significance_error += 1 / 4 * TMath::Power(significance_error, 2);
    significance_error = TMath::Sqrt(significance_error);

    cout << i << " " << count_signal << " " << count_background << " " << count_total << " " << significance << " " << significance_error << endl;

    if (1 < count_total)
    {
      gr_significance->SetPoint(i, i, significance);
      gr_significance->SetPointError(i, 0, significance_error);
    }
  }

  return;
} // void Reco_Eval::Calculate_Significant()

//////////

void Reco_Eval::Calculate_Prob()
{
  for (int i = 0; i < n_histo_sample; i++)
  {
    cout << vec_histo_sample[i] << endl;

    // histo_count[i][0];
    THStack stack;
    for (int j = 0; j < 5; j++)
      stack.Add(histo_count[i][j]);

    histo_prob[i]->Divide(histo_count[i][0], (TH1 *)stack.GetStack()->Last());
  }

  return;
} // void Reco_Eval::Calculate_Prob()

//////////

void Reco_Eval::Draw_DV()
{
  cout << "[Reco_Eval::Draw]: start drawing DV" << endl;

  /* discriminating variables */
  TCanvas *canvas_dv[n_histo_sample];
  stack_dv = new THStack **[n_histo_sample];
  for (int i = 0; i < n_histo_sample; i++)
  {
    TString name = vec_histo_sample[i] + "_DV";

    canvas_dv[i] = new TCanvas(name, name, 1300, 800);
    canvas_dv[i]->Divide(3, 3);
    canvas_dv[i]->Draw();

    stack_dv[i] = new THStack *[n_discriminators];
    for (int j = 0; j < n_discriminators; j++)
    {
      name = vec_histo_sample[i] + "_" + dv_histo_conf[j].discriminator;
      stack_dv[i][j] = new THStack(name, name);

      for (int k = 0; k < n_histo_type; k++)
      {
        histo_discriminator[i][j][k]->SetFillColorAlpha(color[k], .2);

        stack_dv[i][j]->Add(histo_discriminator[i][j][k]);
      } // loop over correct or wrong

      canvas_dv[i]->cd(j + 1);
      stack_dv[i][j]->Draw("BAR");
    } // loop over # of disriminating variables

    canvas_dv[i]->Print(vec_histo_sample[i] + "_DV_" + era + "_" + channel + "." + draw_extension, draw_extension);

    // canvas_dv[i]->SaveAs(vec_histo_sample[i] + "_DV.cpp");
  } // loop over samples

  return;
} // void Reco_Eval::Draw_DV()

//////////

void Reco_Eval::Draw_Raw()
{
  cout << "[Reco_Eval::Draw]: start drawing raw distributions " << endl;

  /* Raw distributions */
  TCanvas *canvas_mva_pre[2];
  TCanvas *canvas_mva[2];
  TCanvas *canvas_had_w[2];
  TCanvas *canvas_had_t[2];
  TCanvas *canvas_lep_w[2];
  TCanvas *canvas_lep_t[2];
  TCanvas *canvas_template[2];
  for (int i = 0; i < 2; i++)
  {
    TString name = "Permutation_MVA_Pre_" + to_string(i);
    canvas_mva_pre[i] = new TCanvas(name, name, 1300, 800);
    canvas_mva_pre[i]->Divide(3, 2);
    canvas_mva_pre[i]->Draw();

    name = "Permutation_MVA_" + to_string(i);
    canvas_mva[i] = new TCanvas(name, name, 1300, 800);
    canvas_mva[i]->Divide(3, 2);
    canvas_mva[i]->Draw();

    name = "Had_W_" + to_string(i);
    canvas_had_w[i] = new TCanvas(name, name, 1300, 800);
    canvas_had_w[i]->Divide(3, 2);
    canvas_had_w[i]->Draw();

    name = "Had_T_" + to_string(i);
    canvas_had_t[i] = new TCanvas(name, name, 1300, 800);
    canvas_had_t[i]->Divide(3, 2);
    canvas_had_t[i]->Draw();

    name = "Lep_W_" + to_string(i);
    canvas_lep_w[i] = new TCanvas(name, name, 1300, 800);
    canvas_lep_w[i]->Divide(3, 2);
    canvas_lep_w[i]->Draw();

    name = "Lep_T_" + to_string(i);
    canvas_lep_t[i] = new TCanvas(name, name, 1300, 800);
    canvas_lep_t[i]->Divide(3, 2);
    canvas_lep_t[i]->Draw();

    name = "Template_" + to_string(i);
    canvas_template[i] = new TCanvas(name, name, 1300, 800);
    canvas_template[i]->Divide(3, 2);
    canvas_template[i]->Draw();
  }

  float ttlj_correct = 0;
  float ttlj_wrong = 0;
  float ttlj_wrong_not_sel = 0;
  float ttlj_cb_correct = 0;
  float ttlj_cb_wrong = 0;
  float ttlj_cb_wrong_not_sel = 0;

  THStack *stack_mva_pre[n_histo_sample];
  THStack *stack_mva[n_histo_sample];
  THStack *stack_had_w[n_histo_sample];
  THStack *stack_had_t[n_histo_sample];
  THStack *stack_lep_w[n_histo_sample];
  THStack *stack_lep_t[n_histo_sample];
  THStack *stack_template[n_histo_sample];
  for (unsigned int i = 0; i < n_histo_sample; i++)
  {
    TString name = vec_histo_sample[i];
    cout << name << endl;

    stack_mva_pre[i] = new THStack(name, name);
    stack_mva[i] = new THStack(name, name);
    stack_had_w[i] = new THStack(name, name);
    stack_had_t[i] = new THStack(name, name);
    stack_lep_w[i] = new THStack(name, name);
    stack_lep_t[i] = new THStack(name, name);
    stack_template[i] = new THStack(name, name);

    for (int j = 0; j < 5; j++)
    {
      histo_mva_pre[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_mva_pre[i]->Add(histo_mva_pre[i][j]);

      histo_mva[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_mva[i]->Add(histo_mva[i][j]);

      histo_had_w[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_had_w[i]->Add(histo_had_w[i][j]);

      histo_had_t[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_had_t[i]->Add(histo_had_t[i][j]);

      histo_lep_w[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_lep_w[i]->Add(histo_lep_w[i][j]);

      histo_lep_t[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_lep_t[i]->Add(histo_lep_t[i][j]);

      histo_template[i][j]->SetFillColorAlpha(color[j], 0.2);
      stack_template[i]->Add(histo_template[i][j]);

      // cout << histo_mva[i][j]->Integral() << endl;
      if (name.Contains("TTLJ") && !name.Contains("cb"))
      {
        if (j == 0)
          ttlj_correct += histo_mva[i][j]->Integral();
        else if (j == 1 || j == 3)
          ttlj_wrong += histo_mva[i][j]->Integral();
        else if (j == 2 || j == 4)
          ttlj_wrong_not_sel += histo_mva[i][j]->Integral();
      }
      else if (name == "TTLJ_cb")
      {
        if (j == 0)
          ttlj_cb_correct += histo_mva[i][j]->Integral();
        else if (j == 1 || j == 3)
          ttlj_cb_wrong += histo_mva[i][j]->Integral();
        else if (j == 2 || j == 4)
          ttlj_cb_wrong_not_sel += histo_mva[i][j]->Integral();
      }
    } // loop over reco type

    // indexing for position
    int canvas_index;
    int subpad_index;
    if (name.Contains("TTLJ"))
    {
      canvas_index = 0;
      if (name.Contains("ud"))
        subpad_index = 1;
      else if (name.Contains("us"))
        subpad_index = 2;
      else if (name.Contains("cd"))
        subpad_index = 4;
      else if (name.Contains("cs"))
        subpad_index = 5;
      else if (name.Contains("cb"))
        subpad_index = 6;
    }
    else
    {
      canvas_index = 1;
      if (name.Contains("TTLL"))
        subpad_index = 1;
      else if (name.Contains("ST"))
        subpad_index = 2;
      else if (name.Contains("VJets"))
        subpad_index = 3;
      else if (name.Contains("ttV"))
        subpad_index = 4;
      else if (name.Contains("VV"))
        subpad_index = 5;
      else if (name.Contains("QCD"))
        subpad_index = 6;
    }

    canvas_mva_pre[canvas_index]->cd(subpad_index);
    stack_mva_pre[i]->Draw("BAR");

    canvas_mva[canvas_index]->cd(subpad_index);
    stack_mva[i]->Draw("BAR");

    canvas_had_w[canvas_index]->cd(subpad_index);
    stack_had_w[i]->Draw("BAR");

    canvas_had_t[canvas_index]->cd(subpad_index);
    stack_had_t[i]->Draw("BAR");

    canvas_lep_w[canvas_index]->cd(subpad_index);
    stack_lep_w[i]->Draw("BAR");

    canvas_lep_t[canvas_index]->cd(subpad_index);
    stack_lep_t[i]->Draw("BAR");

    canvas_template[canvas_index]->cd(subpad_index);
    stack_template[i]->Draw("BAR");
  }

  legend = new TLegend(0.2, 0.2, 0.8, 0.8);
  legend->SetBorderSize(0);

  legend->AddEntry(histo_mva[0][0], "Succeeded", "f");
  legend->AddEntry(histo_mva[0][1], "F., Selected, !HF Conta.", "f");
  legend->AddEntry(histo_mva[0][2], "F., !Selected, !HF Conta.", "f");
  legend->AddEntry(histo_mva[0][4], "F., !Selected, HF Conta.", "f");

  TLatex *latex = new TLatex();

  float lumi = lumi_2018;
  if (era == "2017")
    lumi = lumi_2017;

  for (int i = 0; i < 1; i++)
  {
    canvas_mva_pre[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");

    canvas_mva[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");

    canvas_had_w[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");

    canvas_had_t[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");

    canvas_lep_w[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");

    canvas_lep_t[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");

    canvas_template[i]->cd(3);
    latex->DrawLatex(0.2, 0.9, "CMS Work in progress");
    latex->DrawLatex(0.2, 0.85, Form("L = %.2f /fb", lumi));
    legend->Draw("SAME");
  }

  for (int i = 0; i < 2; i++)
  {
    canvas_mva_pre[i]->Update();
    canvas_mva[i]->Update();
    canvas_had_w[i]->Update();
    canvas_had_t[i]->Update();
    canvas_lep_w[i]->Update();
    canvas_lep_t[i]->Update();
    canvas_template[i]->Update();
  }

  for (int i = 0; i < 2; i++)
  {
    TString name = "Permutation_MVA_Pre_" + to_string(i) + "_" + era + "_" + channel + "." + draw_extension;
    canvas_mva_pre[i]->Print(name, draw_extension);

    name = "Permutation_MVA_" + to_string(i) + "_" + era + "_" + channel + "." + draw_extension;
    canvas_mva[i]->Print(name, draw_extension);

    name = "Had_W_" + to_string(i) + "_" + era + "_" + channel + "." + draw_extension;
    canvas_had_w[i]->Print(name, draw_extension);

    name = "Had_T_" + to_string(i) + "_" + era + "_" + channel + "." + draw_extension;
    canvas_had_t[i]->Print(name, draw_extension);

    name = "Lep_W_" + to_string(i) + "_" + era + "_" + channel + "." + draw_extension;
    canvas_lep_w[i]->Print(name, draw_extension);

    name = "Lep_T_" + to_string(i) + "_" + era + "_" + channel + "." + draw_extension;
    canvas_lep_t[i]->Print(name, draw_extension);

    name = "Template_" + to_string(i) + "_" + era + "_" + channel + "_" + era + "_" + channel + "." + draw_extension;
    canvas_template[i]->Print(name, draw_extension);
  }

  cout << ttlj_correct << " " << ttlj_wrong << " " << ttlj_wrong_not_sel << " " << ttlj_cb_correct << " " << ttlj_cb_wrong << " " << ttlj_cb_wrong_not_sel << endl;

  return;
} // void Reco_Eval::Draw_Raw

//////////

void Reco_Eval::Draw_Sample_By_Sample()
{
  cout << "[Reco_Eval::Draw]: start drawing sample by sample distributions " << endl;

  /* sample by sample */
  THStack *stack_count_wrong[n_histo_sample];
  THStack *stack_cutflow[n_histo_sample];
  for (int i = 0; i < n_histo_sample; i++)
  {
    TString name_base = vec_histo_sample[i];

    TString name = name_base + "_Wrong";
    stack_count_wrong[i] = new THStack(name, name);

    name = name_base + "_Cutflow";
    stack_cutflow[i] = new THStack(name, name);

    for (int j = 0; j < 5; j++)
    {
      if (j != 0)
        stack_count_wrong[i]->Add(histo_count[i][j]);

      histo_cutflow[i][j]->SetFillColorAlpha(color[j], 0.20);
      stack_cutflow[i]->Add(histo_cutflow[i][j]);
    }
  }

  TCanvas *canvas_sample[n_histo_sample];
  for (int i = 0; i < n_histo_sample; i++)
  {
    TString name = vec_histo_sample[i];

    canvas_sample[i] = new TCanvas(name, name, 1300, 800);
    canvas_sample[i]->Divide(2, 2);
    canvas_sample[i]->Draw();

    canvas_sample[i]->cd(1);
    histo_count[i][0]->Draw("COLZ TEXTE");

    canvas_sample[i]->cd(2);
    stack_count_wrong[i]->Draw("COLZ TEXTE");

    canvas_sample[i]->cd(3);
    stack_cutflow[i]->Draw("BAR");

    canvas_sample[i]->cd(4);
    histo_prob[i]->Draw("COLZ TEXTE");

    canvas_sample[i]->Print(name + "." + draw_extension, draw_extension);
  }

  return;
} // void Reco_Eval::Draw_Sample_By_Sample()

//////////

void Reco_Eval::Draw_Significance()
{
  cout << "[Reco_Eval::Draw]: start drawing significance " << endl;

  /* significance */
  TCanvas *canvas_significance = new TCanvas("Significance", "Significance", 1300, 800);
  canvas_significance->Draw();

  gr_significance->SetLineColor(2);
  gr_significance->SetMarkerColor(2);
  gr_significance->Draw("AP*");

  canvas_significance->Print("Significance." + draw_extension, draw_extension);
} // void Reco_Eval::Draw_Significance

//////////

void Reco_Eval::Draw_Swap()
{
  cout << "[Reco_Eval::Draw]: start drawing swap " << endl;

  /* swap */
  TCanvas *canvas_swap = new TCanvas("Swap", "Swap", 1300, 800);
  canvas_swap->Divide(3, 2);
  canvas_swap->Draw();

  int index = 0;
  for (int i = 0; i < n_histo_sample; i++)
  {
    if (vec_histo_sample[i].Contains("TTLJ"))
    {
      if (index == 0 || index == 1)
        canvas_swap->cd(index + 1);
      else
        canvas_swap->cd(index + 2);

      histo_swap[i]->Draw();

      index++;
    }
  }

  canvas_swap->Print("Swap_" + era + "_" + channel + "." + draw_extension, draw_extension);

  /* swap_fixed */
  TCanvas *canvas_swap_fixed = new TCanvas("Swap_Fixed", "Swap_Fixed", 1300, 800);
  canvas_swap_fixed->Divide(3, 2);
  canvas_swap_fixed->Draw();

  index = 0;
  for (int i = 0; i < n_histo_sample; i++)
  {
    if (vec_histo_sample[i].Contains("TTLJ"))
    {
      if (index == 0 || index == 1)
        canvas_swap_fixed->cd(index + 1);
      else
        canvas_swap_fixed->cd(index + 2);

      histo_swap_fix[i]->Draw();

      index++;
    }
  }

  canvas_swap_fixed->Print("Swap_Fixed_" + era + "_" + channel + "." + draw_extension, draw_extension);

  return;
} // void Reco_Eval::Draw_Swap()

//////////

void Reco_Eval::Fill_Cutflow(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason, const int &n_cut)
{

  int index = Get_Index(short_name, index_decay_mode);

  histo_cutflow[index][index_fail_reason]->AddBinContent(n_cut + 1, event.weight);

  return;
} // void Reco_Eval::Fill_Cutflow(const TString& short_name, const int& index_decay_mode, const int& index_fail_reason, const int& n_cut)

//////////

void Reco_Eval::Fill_Histo(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason)
{
  int index = Get_Index(short_name, index_decay_mode);

  histo_mva_pre[index][index_fail_reason]->Fill(event.best_mva_score_pre, event.weight);
  histo_mva[index][index_fail_reason]->Fill(event.best_mva_score, event.weight);

  histo_had_w[index][index_fail_reason]->Fill(event.m_had_w, event.weight);
  histo_had_t[index][index_fail_reason]->Fill(event.m_had_t, event.weight);

  histo_lep_w[index][index_fail_reason]->Fill(event.m_lep_w, event.weight);
  histo_lep_t[index][index_fail_reason]->Fill(event.m_lep_t, event.weight);

  histo_template[index][index_fail_reason]->Fill(event.template_mva_score, event.weight);

  // histo_mva_pre[index][index_fail_reason]->Fill(event.best_mva_score_pre, 1);
  // histo_mva[index][index_fail_reason]->Fill(event.best_mva_score, 1);

  return;
} // void Reco_Eval::Fill_Histo()

//////////

void Reco_Eval::Fill_Histo_Count(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason)
{
  int index = Get_Index(short_name, index_decay_mode);

  histo_count[index][index_fail_reason]->Fill(event.n_jets, event.n_bjets, event.weight);

  return;
} // void Reco_Eval::Fill_Histo_Count(const TString& short_name, const int& index_decay_mode, const int& index_fail_reason)

//////////

void Reco_Eval::Fill_Histo_Discriminators(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason)
{
  int index = Get_Index(short_name, index_decay_mode);

  histo_discriminator[index][0][index_fail_reason]->Fill(event.best_mva_score, event.weight);
  // histo_discriminator[index][1][index_fail_reason]->Fill(event.n_jets, event.weight);
  histo_discriminator[index][1][index_fail_reason]->Fill(event.n_bjets, event.weight);
  histo_discriminator[index][2][index_fail_reason]->Fill(event.n_cjets, event.weight);
  histo_discriminator[index][3][index_fail_reason]->Fill(event.bvsc_w_u, event.weight);
  histo_discriminator[index][4][index_fail_reason]->Fill(event.cvsb_w_u, event.weight);
  histo_discriminator[index][5][index_fail_reason]->Fill(event.cvsl_w_u, event.weight);
  histo_discriminator[index][6][index_fail_reason]->Fill(event.bvsc_w_d, event.weight);
  histo_discriminator[index][7][index_fail_reason]->Fill(event.cvsb_w_d, event.weight);
  histo_discriminator[index][8][index_fail_reason]->Fill(event.cvsl_w_d, event.weight);

  return;
} // void Reco_Eval::Fill_Histo_Discriminators(const TString& short_name, const int& index_decay_mode, const int& index_fail_reason)

//////////

void Reco_Eval::Fill_Histo_Swap(const TString &short_name, const int &index_decay_mode)
{
  int index = Get_Index(short_name, index_decay_mode);

  int unmatched_swapped = 0;
  if (event.swapped_truth == chk_swap)
    unmatched_swapped = 0;
  else
    unmatched_swapped = 1;

  histo_swap[index]->Fill(event.swapped_truth, event.weight);
  histo_swap_fix[index]->Fill(unmatched_swapped, event.weight);

  return;
} // void Reco_Eval::Fill_Histo_Swap(const TString& short_name, const int& index_decay_mode)

//////////

void Reco_Eval::Fill_Output_Tree(const TString &short_name, const int &index_decay_mode, const int &index_fail_reason)
{
  for (int i = 0; i < 5; i++)
  {
    if (index_decay_mode == decay_modes[i])
    {
      out_tree[i]->Fill();
      // out_tree[i][index_fail_reason]->Fill();

      break;
    }
  }

  return;
} // void Reco_Eval::Fill_Output_Tree(const TString& short_name, const int& index_decay_mode)

//////////

int Reco_Eval::Get_Index(const TString &name)
{
  int index;
  for (auto it = vec_histo_sample.begin(); it != vec_histo_sample.end(); it++)
  {
    if (*it == name)
    {
      index = it - vec_histo_sample.begin();
      break;
    }
  }

  return index;
} // int Reco_Eval::Get_Index(const TString& name)

//////////

int Reco_Eval::Get_Index(const TString &short_name, const int &index_decay_mode)
{
  TString name;
  if (short_name == "TTLJ" || short_name == "TTLJ_WtoCB")
  {
    if (index_decay_mode == 21)
      name = "TTLJ_ud";
    else if (index_decay_mode == 23)
      name = "TTLJ_us";
    else if (index_decay_mode == 41)
      name = "TTLJ_cd";
    else if (index_decay_mode == 43)
      name = "TTLJ_cs";
    else if (index_decay_mode == 45)
      name = "TTLJ_cb";
  }
  else
    name = short_name;

  int index = Get_Index(name);

  return index;
} // int Reco_Eval::Get_Index(const TString& short_name, const int& index_decay_mode)

//////////

void Reco_Eval::Read_Tree()
{
  cout << "[Reco_Eval::Read_Tree]: Start to read trees " << endl;

  for (auto it = map_fin.begin(); it != map_fin.end(); it++)
  {
    TString short_name = samples.map_short_name_mc[it->first];

    cout << it->first << " " << short_name << endl;

    TTree *tree = map_tree[it->first];

    Long64_t n_entries = tree->GetEntries();
    n_entries /= reduction;

    for (Long64_t j = 0; j < n_entries; j++)
    {
      if (j % 5000000 == 0)
        cout << "Processing... " << j << "/" << n_entries << endl;

      tree->GetEntry(j);

      // if (20 < event.met_pt)
      //   continue;

      chk_swap = event.Swap();

      int index_fail_reason = -1;
      if (event.chk_reco_correct)
        index_fail_reason = 0; // correct
      else
      {
        if (event.chk_included == true && event.chk_hf_contamination == false)
          index_fail_reason = 1;
        else if (event.chk_included == false && event.chk_hf_contamination == false)
          index_fail_reason = 2;
        else if (event.chk_included == true && event.chk_hf_contamination == true)
          index_fail_reason = 3;
        else if (event.chk_included == false && event.chk_hf_contamination == true)
          index_fail_reason = 4;
      }

      event.weight = 1;
      event.weight *= event.weight_lumi;
      event.weight *= event.weight_mc;
      event.weight *= event.weight_pileup;
      event.weight *= event.weight_prefire;
      event.weight *= event.weight_top_pt;
      event.weight *= event.weight_sl_trig;

      if (channel == "Mu")
      {
        event.weight *= event.weight_mu_id;
        event.weight *= event.weight_mu_iso;
      }
      else if (channel == "El")
      {
        event.weight *= event.weight_el_id;
        event.weight *= event.weight_el_reco;
      }

      event.weight *= event.weight_c_tag;

      // event.weight *= tagging_rf.Get_Tagging_RF("B_Tag_Nominal", event.n_jets);
      event.weight *= tagging_rf.Get_Tagging_RF_C_Tag(short_name, "C_Tag_Nominal", event.n_pv, event.ht);

      // if (event.chk_gentau_conta == true)
      //   continue;

      //if (event.n_bjets == 2)
      //  continue;

      Fill_Output_Tree(short_name, event.decay_mode, index_fail_reason);

      Fill_Histo(short_name, event.decay_mode, index_fail_reason);

      if (index_fail_reason == 0)
        Fill_Histo_Swap(short_name, event.decay_mode);

      Fill_Histo_Count(short_name, event.decay_mode, index_fail_reason);
      Fill_Histo_Discriminators(short_name, event.decay_mode, index_fail_reason);

      /* Cut */
      // no cut
      int n_cut = 0;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // //ttbar pt cut
      // if(event.pt_had_t_b<25 || event.pt_lep_t_b<25) continue;
      // n_cut++;
      // Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // b-tagging on ttbar
      if (event.bvsc_had_t_b < bvsc_wp_m || event.bvsc_lep_t_b < bvsc_wp_m)
        continue;
      n_cut++;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // mva score cut
      if (event.best_mva_score < 0.9)
        continue;
      n_cut++;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // b-tagging on w_d
      if (event.bvsc_w_d < bvsc_wp_m)
        continue;
      n_cut++;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // c-tagging on w_u
      if (event.cvsb_w_u < cvsb_wp_m || event.cvsl_w_u < cvsl_wp_m)
        continue;
      n_cut++;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // no additional c-tagging
      if (event.n_cjets != 1)
        continue;
      n_cut++;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);

      // Fill_Output_Tree(short_name, event.decay_mode);

      // no additional b-tagging
      if (event.n_bjets != 3)
        continue;
      n_cut++;
      Fill_Cutflow(short_name, event.decay_mode, index_fail_reason, n_cut);
      /* Cut */

      // if(1000<j) break;
      // histo_mva_pre
    } // loop over entries
  }   // loop over map_fin

  return;
} // void Reco_Eval::Read_Tree()

//////////
