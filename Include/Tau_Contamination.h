#ifndef __Tau_Contamination__
#define __Tau_Contamination__

#include <iostream>

#include <TTree.h>
#include <TH1D.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TCanvas.h>

#include <Result_Event.h>

using namespace std;

class Tau_Contamination : public TObject
{
public:
    Tau_Contamination(const TString &a_era = "2018", const TString &a_channel="Mu");
    ~Tau_Contamination();

protected:
    int reduction;

    TString channel;
    TString era;

    Result_Event event;

    ClassDef(Tau_Contamination, 1);
};

#endif /*__Tau_Contamination__*/