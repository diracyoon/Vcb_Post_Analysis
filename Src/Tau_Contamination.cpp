#include "Tau_Contamination.h"

ClassImp(Tau_Contamination);

//////////

Tau_Contamination::Tau_Contamination(const TString& a_era, const TString& a_channel)
{
    cout << "[Tau_Contamination::Tau_Contamination]: Init analysis" << endl;

    reduction = 100;

    era = a_era;
    channel = a_channel;
}//Tau_Contamination::Tau_Contamination(const TString& a_era, const TString& a_channel)

//////////

Tau_Contamination::~Tau_Contamination()
{

}//Tau_Contamination::~Tau_Contamination()

//////////
