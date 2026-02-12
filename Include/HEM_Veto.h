#ifndef __HEM_Veto_H__
#define __HEM_Veto_H__

#include <iostream>
#include <map>

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TLatex.h>
#include <TStyle.h>

#include <Samples.h>
#include <Result_Event.h>

class HEM_Veto : public TObject
{
public:
  HEM_Veto(const TString &a_tagger = "B");
  ~HEM_Veto();

private:
  TString tagger;

  int reduction;

  Samples samples_mu;
  Samples samples_el;

  map<TString, TFile *> map_fin_data;
  map<TString, TTree *> map_tree_data;

  Result_Event event;

  TH2D **jet_eta_phi;// period
  TLatex *latex;

  void Fill_Histo_Data(const TString &fin_name);
  void Init_Histo();
  void Read_Tree();
  void Register_Sample();

  ClassDef(HEM_Veto, 1);
};

#endif /* __HEM_Veto_H__ */