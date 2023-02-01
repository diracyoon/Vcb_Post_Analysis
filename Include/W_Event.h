#ifndef __W_Event_H__
#define __W_Event_H__

#include <iostream>

#include <TObject.h>
#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>

#include <RooRealVar.h>
#include <RooDataHist.h> 
#include <RooHistPdf.h>

using namespace std;
using namespace RooFit;

class W_Event : public TObject
{
 public:
  W_Event(const TString& a_era, const TString& a_channel, const TString& a_mode);
  virtual ~W_Event();
    
 protected:
  TString era;
  TString channel;
  TString mode;

  float bvsc_w_u;
  float cvsb_w_u;
  float cvsl_w_u;

  float bvsc_w_d;
  float cvsb_w_d;
  float cvsl_w_d;

  float pt_w_u;
  float pt_w_d;

  float eta_w_u;
  float eta_w_d;

  float m_w_u;
  float m_w_d;

  int swapped_mva;
  
  //For MC
  bool pu_conta_w_u;
  bool pu_conta_w_d;

  int swapped_truth;
  
  RooRealVar x;
  RooDataHist* data_hist[6];
  RooHistPdf* pdf[6];

  TH1D* histo[6];
  TTree* tree;
  TFile* fin;
  TFile* fout;
  
  float Cal_Likelihood(const float score_w_u[3], const float score_w_d[3]);
  bool Swap();

  ClassDef(W_Event, 1);
};

#endif /*__W_Event_H__ */
