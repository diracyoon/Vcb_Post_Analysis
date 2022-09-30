#ifndef __Average_Syst_PDF_H__
#define __Average_Syst_PDF_H__

#include <iostream>
#include <map>

#include <TObject.h>
#include <TString.h>
#include <TFile.h>
#include <TDirectory.h>
#include <TH1D.h>
#include <TList.h>

#include <Samples.h>

using namespace std;

class Average_Syst_PDF : public TObject
{
 public:
  Average_Syst_PDF(const TString& a_era, const TString& a_channel);
  ~Average_Syst_PDF();

  void Average(); 
    
 protected:
  Samples samples;

  TString era;
  TString channel;

  map<TString,TFile*> map_fin_mc;
  TFile* fout;

  TH1D* histo_average;
  ClassDef(Average_Syst_PDF, 1);
};

#endif /* __Average_Syst_PDF_H__ */
