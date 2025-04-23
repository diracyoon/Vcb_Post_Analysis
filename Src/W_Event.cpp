#include "W_Event.h"
#include <Data_MC_Comparison.h>

ClassImp(W_Event);

//////////

W_Event::W_Event(const TString &a_era, const TString &a_channel, const TString &a_mode) : x("Tagging_Score", "Tagging_Score", 0, 1)
{
  era = a_era;
  channel = a_channel;
  mode = a_mode;

  cout << mode << endl;

  // Obsolted
  // TString path_base = getenv("Vcb_Post_Analysis_WD");
  // path_base += "/Sample/" + era + "/RunTemplateTruth/";

  // for(int i=0; i<6; i++)
  //   {
  //     TString histo_name;
  //     if(i==0) histo_name = "bvsc_w_u";
  //     else if(i==1) histo_name = "cvsb_w_u";
  //     else if(i==2) histo_name = "cvsl_w_u";
  //     else if(i==3) histo_name = "bvsc_w_d";
  //     else if(i==4) histo_name = "cvsb_w_d";
  //     else if(i==5) histo_name = "cvsl_w_d";

  //     histo[i] = new TH1D(histo_name, histo_name, 100, 0, 1);
  //   }

  // fin = new TFile(path_base+"Vcb_TTLJ_WtoCB_powheg.root");
  // tree = (TTree*)fin->Get("Template_Truth_45");

  // tree->SetBranchAddress("bvsc_w_u", &bvsc_w_u);
  // tree->SetBranchAddress("cvsb_w_u", &cvsb_w_u);
  // tree->SetBranchAddress("cvsl_w_u", &cvsl_w_u);
  // tree->SetBranchAddress("bvsc_w_d", &bvsc_w_d);
  // tree->SetBranchAddress("cvsb_w_d", &cvsb_w_d);
  // tree->SetBranchAddress("cvsl_w_d", &cvsl_w_d);

  // Long64_t n_entries = tree->GetEntries();
  // for(Long64_t i=0; i<n_entries; i++)
  //   {
  //     tree->GetEntry(i);

  //     histo[0]->Fill(bvsc_w_u);
  //     histo[1]->Fill(cvsb_w_u);
  //     histo[2]->Fill(cvsl_w_u);
  //     histo[3]->Fill(bvsc_w_d);
  //     histo[4]->Fill(cvsb_w_d);
  //     histo[5]->Fill(cvsl_w_d);
  //   }

  // for(int i=0; i<6; i++)
  //   {
  //     TString name = "data_hist_";
  //     if(i==0) name += "bvsc_w_u";
  //     else if(i==1) name += "cvsb_w_u";
  //     else if(i==2) name += "cvsl_w_u";
  //     else if(i==3) name += "bvsc_w_d";
  //     else if(i==4) name += "cvsb_w_d";
  //     else if(i==5) name += "cvsl_w_d";

  //     data_hist[i] = new RooDataHist(name, name, x, histo[i]);

  //     TString name_pdf = "pdf_";
  //     if(i==0) name_pdf += "bvsc_w_u";
  //     else if(i==1) name_pdf += "cvsb_w_u";
  //     else if(i==2) name_pdf += "cvsl_w_u";
  //     else if(i==3) name_pdf += "bvsc_w_d";
  //     else if(i==4) name_pdf += "cvsb_w_d";
  //     else if(i==5) name_pdf += "cvsl_w_d";

  //     pdf[i] = new RooHistPdf(name_pdf, name_pdf, x, *data_hist[i], 2);
  //   }
} // W_Event::W_Event(const TString& a_mode)

//////////

W_Event::~W_Event()
{
  // Obsolted
  // fin->Close();

  // fout = new TFile("W_Event.root", "RECREATE");
  // fout->cd();
  // for (int i = 0; i < 6; i++)
  //   histo[i]->Write();
  // fout->Close();

  // for (int i = 0; i < 6; i++)
  // {
  //   delete histo[i];
  //   delete data_hist[i];
  //   delete pdf[i];
  // }
} // W_Event::~W_Event()

//////////

bool W_Event::Swap()
{
  // cout << "void W_Event::Swap()" << endl;
  // cout <<  swapped << " " << bvsc_w_u << " " << bvsc_w_d << endl;

  bool chk_swap = false;
  if (mode == "BvsC_Only")
  {
    if (bvsc_w_d < bvsc_w_u)
      chk_swap = true;
  } // BvsC_Only
  else if (mode == "PDF")
  {
    float score_w_u[3] = {bvsc_w_u, cvsb_w_u, cvsl_w_u};
    float score_w_d[3] = {bvsc_w_d, cvsb_w_d, cvsl_w_d};

    float likelihood = Cal_Likelihood(score_w_u, score_w_d);

    float score_w_u_swapped[3] = {bvsc_w_d, cvsb_w_d, cvsl_w_d};
    float score_w_d_swapped[3] = {bvsc_w_u, cvsb_w_u, cvsl_w_u};

    float likelihood_swapped = Cal_Likelihood(score_w_u_swapped, score_w_d_swapped);

    if (likelihood < likelihood_swapped)
      chk_swap = true;
  }
  else if (mode == "Permutation_MVA")
  {
    if (swapped_mva)
      chk_swap = true;
  }
  else
  {
    cout << "Can't find swap option. Check it first." << endl;
    exit(EXIT_FAILURE);
  }

  if (chk_swap)
  {
    float bvsc_temp = bvsc_w_u;
    float cvsb_temp = cvsb_w_u;
    float cvsl_temp = cvsl_w_u;
    float pt_temp = pt_w_u;
    float eta_temp = eta_w_u;
    float m_temp = m_w_u;
    bool pu_conta_temp = pu_conta_w_u;

    bvsc_w_u = bvsc_w_d;
    cvsb_w_u = cvsb_w_d;
    cvsl_w_u = cvsl_w_d;
    pt_w_u = pt_w_d;
    eta_w_u = eta_w_d;
    m_w_u = m_w_d;
    pu_conta_w_u = pu_conta_w_d;

    bvsc_w_d = bvsc_temp;
    cvsb_w_d = cvsb_temp;
    cvsl_w_d = cvsl_temp;
    pt_w_d = pt_temp;
    eta_w_d = eta_temp;
    m_w_d = m_temp;
    pu_conta_w_d = pu_conta_temp;
  }

  return chk_swap;
} // bool W_Event::Swap()

//////////

float W_Event::Cal_Likelihood(const float score_w_u[3], const float score_w_d[3])
{
  float prob = 1;
  for (int i = 0; i < 3; i++)
  {
    x.setVal(score_w_u[i]);
    prob *= pdf[i]->getVal();
  }
  for (int i = 0; i < 3; i++)
  {
    x.setVal(score_w_d[i]);
    prob *= pdf[i + 3]->getVal();
  }

  return prob;
} // float W_Event::Cal_Likelihood(const float score_w_u[3], const float score_w_d[3])

//////////
