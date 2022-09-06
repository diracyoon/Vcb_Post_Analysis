#include "Samples.h"

ClassImp(Samples)

//////////

Samples::Samples(const TString& a_era, const TString& a_channel)
{
  era = a_era;
  channel = a_channel;

  if(era=="2018")
    {
      if(channel=="Mu")
	{
	  map_mc["TTLJ_WtoCB_powheg"] = "Vcb_Mu_TTLJ_WtoCB_powheg.root";
	  map_mc["TTLJ_powheg"] = "Vcb_Mu_TTLJ_powheg.root";
	  map_mc["TTLL_powheg"] = "Vcb_Mu_TTLL_powheg.root";
	  map_mc["SingleTop_tW_top"] = "Vcb_Mu_SingleTop_tW_top_NoFullyHad.root";
	  map_mc["SingleTop_tW_antitop"] = "Vcb_Mu_SingleTop_tW_antitop_NoFullyHad.root";
	  map_mc["WJets"] = "Vcb_Mu_WJets_MG.root";
	  map_mc["DYJets"] = "Vcb_Mu_DYJets_MG.root";
	  map_mc["QCD_bEn_HT100to200"] = "Vcb_Mu_QCD_bEnriched_HT100to200.root";
	  map_mc["QCD_bEn_HT200to300"] = "Vcb_Mu_QCD_bEnriched_HT200to300.root";
	  map_mc["QCD_bEn_HT300to500"] = "Vcb_Mu_QCD_bEnriched_HT300to500.root";
	  map_mc["QCD_bEn_HT500to700"] = "Vcb_Mu_QCD_bEnriched_HT500to700.root";
	  map_mc["QCD_bEn_HT700to1000"] = "Vcb_Mu_QCD_bEnriched_HT700to1000.root";
	  map_mc["QCD_bEn_HT1000to1500"] = "Vcb_Mu_QCD_bEnriched_HT1000to1500.root";
	  map_mc["QCD_bEn_HT1500to2000"] = "Vcb_Mu_QCD_bEnriched_HT1500to2000.root";
	  map_mc["QCD_bEn_HT2000toInf"] = "Vcb_Mu_QCD_bEnriched_HT2000toInf.root";
	  
	  map_short_name_mc["TTLJ_WtoCB_powheg"] = "TTLJ_WtoCB";
	  map_short_name_mc["TTLJ_powheg"] = "TTLJ";
	  map_short_name_mc["TTLL_powheg"] = "TTLL";
	  map_short_name_mc["SingleTop_tW_top"] = "ST";
	  map_short_name_mc["SingleTop_tW_antitop"] = "ST";
	  map_short_name_mc["WJets"] = "WJets";
	  map_short_name_mc["DYJets"] = "DYJets";
          map_short_name_mc["QCD_bEn_HT100to200"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT200to300"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT300to500"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT500to700"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT700to1000"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT1000to1500"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT1500to2000"] = "QCD_bEn";
	  map_short_name_mc["QCD_bEn_HT2000toInf"] = "QCD_bEn";
	  
	  map_data["SingleMuon_A"] = "Vcb_Mu_SingleMuon_A.root";
	  map_data["SingleMuon_B"] = "Vcb_Mu_SingleMuon_B.root";
	  map_data["SingleMuon_C"] = "Vcb_Mu_SingleMuon_C.root";
	  map_data["SingleMuon_D"] = "Vcb_Mu_SingleMuon_D.root";
	  
	  map_short_name_data["SingleMuon_A"] = "SingleMuon";
	  map_short_name_data["SingleMuon_B"] = "SingleMuon";
	  map_short_name_data["SingleMuon_C"] = "SingleMuon";
	  map_short_name_data["SingleMuon_D"] = "SingleMuon";
	}//if(channel=="Mu")
      else if(channel=="El")
	{
	}//else if(channel=="El")
    }//if(era==2018)
}//Samples::Samples(const TString& a_era, const TString& a_channel)

//////////

Samples::~Samples()
{
}

//////////
