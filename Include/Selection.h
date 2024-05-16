#ifndef __Selection_H__
#define __Selection_H__

#include <iostream>

#include <TObject.h>
#include <TString.h>
#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TStyle.h>
#include <THStack.h>
#include <TLegend.h>

#include <Const_Def.h>
#include <Samples.h>

using namespace std;

class Selection : public TObject
{
public:
  Selection(const TString &a_era = "2018", const TString &a_channel = "Mu", const TString &a_extension = "png");
  ~Selection();

protected:
  TString era;
  TString channel;
  TString extension;

  Samples samples;

  map<TString, TFile *> map_fin;
  map<TString, TH1D *> map_histo;
  map<TString, int> color;

  void Draw();

private:
  ClassDef(Selection, 1);
};

#endif /* __Selection_H__ */