#include "HEM_Veto.h"

ClassImp(HEM_Veto);

//////////

HEM_Veto::HEM_Veto(const TString &a_tagger) : samples_mu("2018", "Mu", "Vcb"), samples_el("2018", "El", "Vcb")
{
  gROOT->SetBatch(kTRUE);

  reduction = 1;

  tagger = a_tagger;

  Register_Sample();
  Init_Histo();
  Read_Tree();
}

//////////

HEM_Veto::~HEM_Veto()
{
  gStyle->SetOptStat(0);

  TCanvas *canvas = new TCanvas("canvas", "canvas", 1400, 1000);

  for (unsigned int i = 0; i < samples_mu.vec_period.size(); i++)
  {
    canvas->cd();
    jet_eta_phi[i]->SetTitle("");
    jet_eta_phi[i]->GetXaxis()->SetTitle("Jet #eta");
    jet_eta_phi[i]->GetYaxis()->SetTitle("Jet #phi");
    jet_eta_phi[i]->Draw("COLZ");

    TLatex *latex = new TLatex();
    latex->SetTextSize(0.04);
    latex->DrawLatexNDC(0.1, 0.91, "CMS #bf{work in progress}");
    latex->DrawLatexNDC(0.53, 0.91, Form("#bf{2018 Single Mu + Single El Period %s}", samples_mu.vec_period[i].Data()));

    canvas->Print("HEM_Veto_Jet_Eta_Phi_2018_" + samples_mu.vec_period[i] + ".png", "png");
  } // loop over periods
}

//////////

void HEM_Veto::Fill_Histo_Data(const TString &fin_name)
{
  Ssiz_t pos = fin_name.Last('_');
  TString period = fin_name(pos + 1, fin_name.Length());

  int index_period = find(samples_mu.vec_period.begin(), samples_mu.vec_period.end(), period) - samples_mu.vec_period.begin();

  event.weight = 1;
  event.weight *= event.weight_hem_veto;

  for (unsigned int i = 0; i < event.vec_jet_pt->size(); i++)
  {
    // cout << event.vec_jet_pt->at(i) << ", " << event.vec_jet_phi->at(i) << endl;
    jet_eta_phi[index_period]->Fill(event.vec_jet_eta->at(i), event.vec_jet_phi->at(i), event.weight);
  }

  return;
} // void HEM_Veto::Fill_Histo_Data()

//////////

void HEM_Veto::Init_Histo()
{
  cout << "[HEM_Veto::Init_Histo]: Init" << endl;

  jet_eta_phi = new TH2D *[samples_mu.vec_period.size()];
  for (unsigned int i = 0; i < samples_mu.vec_period.size(); i++)
  {
    jet_eta_phi[i] = new TH2D("Jet_Eta_Phi_" + samples_mu.vec_period[i], "Jet_Eta_Phi_" + samples_mu.vec_period[i], 50, -2.5, 2.5, 64, -3.2, 3.2);
  } // loop over periods

  cout << "[HEM_Veto::Init_Histo]: Done" << endl;

  return;
} // void HEM_Veto::Init_Histo()

//////////

void HEM_Veto::Read_Tree()
{
  cout << "[HEM_Veto::Read_Tree]: Start to read trees" << endl;

  for (auto it = map_tree_data.begin(); it != map_tree_data.end(); it++)
  {
    TString fin_name = it->first;
    cout << fin_name << endl;

    Long64_t n_entries = it->second->GetEntries();
    n_entries /= reduction;
    cout << "N_Entries = " << it->second->GetEntries() << ", Reduction = " << reduction << ", N_Entries/Reduction = " << n_entries << endl;

    for (Long64_t i = 0; i < n_entries; i++)
    {
      if (i % 500000 == 0)
        cout << "Processing... " << i << "/" << n_entries << "." << endl;

      event.Clear();
      it->second->GetEntry(i);

      Fill_Histo_Data(fin_name);
    } // loop over n_entries

    // map_fin_data[fin_name]->Close();
  } // loop over data period

  cout << "[HEM_Veto::Read_Tree]: Done" << endl;
  return;
} // void HEM_Veto::Read_Tree()

//////////

void HEM_Veto::Register_Sample()
{
  cout << "[HEM_Veto::Register_Sample]: Init" << endl;

  TString path_sample_base = getenv("Vcb_Post_Analysis_Sample_Dir");
  TString result_path;

  if (tagger == "B")
    result_path = path_sample_base + "/2018/Vcb_BTag/DATA/";
  else if (tagger == "C")
    result_path = path_sample_base + "/2018/Vcb_CTag/DATA/";

  for (auto it = samples_mu.map_data.begin(); it != samples_mu.map_data.end(); it++)
  {
    cout << it->second << endl;

    map_fin_data[it->first] = new TFile(result_path + it->second);
    map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get("Mu/Central/Result_Tree");

    event.Setup_Tree(map_tree_data[it->first], Syst::Central, false);
  }

  for (auto it = samples_el.map_data.begin(); it != samples_el.map_data.end(); it++)
  {
    cout << it->second << endl;

    map_fin_data[it->first] = new TFile(result_path + it->second);
    map_tree_data[it->first] = (TTree *)map_fin_data[it->first]->Get("El/Central/Result_Tree");

    event.Setup_Tree(map_tree_data[it->first], Syst::Central, false);
  }

  cout << "[HEM_Veto::Register_Sample]: Done" << endl;

  return;
} // void HEM_Veto::Register_Sample()

//////////