#include "Samples.h"

ClassImp(Samples);

//////////

Samples::Samples(const TString &a_era, const TString &a_channel, const TString &a_analyser)
{
  era = a_era;
  channel = a_channel;
  analyser = a_analyser;

  map_mc["TTLJ_WtoCB"] = "Vcb_TTLJ_WtoCB_powheg.root";
  map_mc["TTLJ"] = "Vcb_TTLJ_powheg.root";
  map_mc["TTLL"] = "Vcb_TTLL_powheg.root";
  map_mc["SingleTop_sch_Lep"] = "Vcb_SingleTop_sch_Lep.root";
  map_mc["SingleTop_tch_top_Incl"] = "Vcb_SingleTop_tch_top_Incl.root";
  map_mc["SingleTop_tch_antitop_Incl"] = "Vcb_SingleTop_tch_antitop_Incl.root";
  map_mc["SingleTop_tW_top"] = "Vcb_SingleTop_tW_top_NoFullyHad.root";
  map_mc["SingleTop_tW_antitop"] = "Vcb_SingleTop_tW_antitop_NoFullyHad.root";
  map_mc["WJets"] = "Vcb_WJets_MG.root";
  map_mc["DYJets"] = "Vcb_DYJets_MG.root";
  map_mc["QCD_bEn_HT100to200"] = "Vcb_QCD_bEnriched_HT100to200.root";
  map_mc["QCD_bEn_HT200to300"] = "Vcb_QCD_bEnriched_HT200to300.root";
  map_mc["QCD_bEn_HT300to500"] = "Vcb_QCD_bEnriched_HT300to500.root";
  map_mc["QCD_bEn_HT500to700"] = "Vcb_QCD_bEnriched_HT500to700.root";
  map_mc["QCD_bEn_HT700to1000"] = "Vcb_QCD_bEnriched_HT700to1000.root";
  map_mc["QCD_bEn_HT1000to1500"] = "Vcb_QCD_bEnriched_HT1000to1500.root";
  map_mc["QCD_bEn_HT1500to2000"] = "Vcb_QCD_bEnriched_HT1500to2000.root";
  map_mc["QCD_bEn_HT2000toInf"] = "Vcb_QCD_bEnriched_HT2000toInf.root";
  map_mc["ttHTobb"] = "Vcb_ttHTobb.root";
  map_mc["ttHToNonbb"] = "Vcb_ttHToNonbb.root";
  map_mc["ttWToLNu"] = "Vcb_ttWToLNu.root";
  map_mc["ttWToQQ"] = "Vcb_ttWToQQ.root";
  map_mc["ttZToLLNuNu"] = "Vcb_ttZToLLNuNu.root";
  map_mc["ttZToQQ"] = "Vcb_ttZToQQ.root";
  map_mc["WW"] = "Vcb_WW_pythia.root";
  map_mc["WZ"] = "Vcb_WZ_pythia.root";
  map_mc["ZZ"] = "Vcb_ZZ_pythia.root";

  map_short_name_mc["TTLJ_WtoCB"] = "TTLJ_WtoCB";
  map_short_name_mc["TTLJ"] = "TTLJ";
  map_short_name_mc["TTLL"] = "TTLL";
  map_short_name_mc["SingleTop_sch_Lep"] = "ST";
  map_short_name_mc["SingleTop_tch_top_Incl"] = "ST";
  map_short_name_mc["SingleTop_tch_antitop_Incl"] = "ST";
  map_short_name_mc["SingleTop_tW_top"] = "ST";
  map_short_name_mc["SingleTop_tW_antitop"] = "ST";
  map_short_name_mc["WJets"] = "VJets";
  map_short_name_mc["DYJets"] = "VJets";
  map_short_name_mc["QCD_bEn_HT100to200"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT200to300"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT300to500"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT500to700"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT700to1000"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT1000to1500"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT1500to2000"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT2000toInf"] = "QCD_bEn";
  map_short_name_mc["ttHTobb"] = "ttV";
  map_short_name_mc["ttHToNonbb"] = "ttV";
  map_short_name_mc["ttWToLNu"] = "ttV";
  map_short_name_mc["ttWToQQ"] = "ttV";
  map_short_name_mc["ttZToLLNuNu"] = "ttV";
  map_short_name_mc["ttZToQQ"] = "ttV";
  map_short_name_mc["WW"] = "VV";
  map_short_name_mc["WZ"] = "VV";
  map_short_name_mc["ZZ"] = "VV";

  map_top_syst["TTLJ_CP5down"] = "Vcb_TTLJ_powheg_CP5Down.root";
  map_top_syst["TTLJ_CP5up"] = "Vcb_TTLJ_powheg_CP5Up.root";
  map_top_syst["TTLJ_hdampdown"] = "Vcb_TTLJ_powheg_hdampDown.root";
  map_top_syst["TTLJ_hdampup"] = "Vcb_TTLJ_powheg_hdampUp.root";
  map_top_syst["TTLJ_mtop_171p5"] = "Vcb_TTLJ_powheg_mtop171p5.root";
  map_top_syst["TTLJ_mtop_173p5"] = "Vcb_TTLJ_powheg_mtop173p5.root";

  map_top_syst["TTLL_CP5down"] = "Vcb_TTLL_powheg_CP5Down.root";
  map_top_syst["TTLL_CP5up"] = "Vcb_TTLL_powheg_CP5Up.root";
  map_top_syst["TTLL_hdampdown"] = "Vcb_TTLL_powheg_hdampDown.root";
  map_top_syst["TTLL_hdampup"] = "Vcb_TTLL_powheg_hdampUp.root";
  map_top_syst["TTLL_mtop_171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
  map_top_syst["TTLL_mtop_173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";

  if (era == "2016preVFP")
    vec_period = {"B_ver2", "C", "D", "E", "F"};
  else if (era == "2016postVFP")
    vec_period = {"F", "G", "H"};
  if (era == "2017")
    vec_period = {"B", "C", "D", "E", "F"};
  else if (era == "2018")
    vec_period = {"A", "B", "C", "D"};

  if (channel == "Mu" || channel == "MM")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["SingleMuon_" + vec_period[i]] = "Vcb_SingleMuon_" + vec_period[i] + ".root";
      map_short_name_data["SingleMuon_" + vec_period[i]] = "SingleMuon";
    }
  } // if(channel=="Mu")
  if (channel == "El" || channel == "EE")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["EGamma_" + vec_period[i]] = "Vcb_EGamma_" + vec_period[i] + ".root";
      map_short_name_data["EGamma_" + vec_period[i]] = "EGamma";
    }
  } // if(channel=="El")
  if (channel == "ME")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["SingleMuon_" + vec_period[i]] = "Vcb_SingleMuon_" + vec_period[i] + ".root";
      map_short_name_data["SingleMuon_" + vec_period[i]] = "SingleMuon";
    }

    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["EGamma_" + vec_period[i]] = "Vcb_EGamma_" + vec_period[i] + ".root";
      map_short_name_data["EGamma_" + vec_period[i]] = "EGamma";
    }
  }

  if (era != "2018")
  {
    for (auto it = map_data.begin(); it != map_data.end(); it++)
    {
      if (it->second.Contains("EGamma"))
        it->second.ReplaceAll("EGamma", "SingleElectron");
    }
  }

  if (analyser == "Vcb_Tagging_RF")
  {
    map_mc["TTLJ_hdampDown"] = "Vcb_TTLJ_powheg_hdampDown.root";
    map_mc["TTLJ_hdampUp"] = "Vcb_TTLJ_powheg_hdampUp.root";
    map_mc["TTLJ_CP5Down"] = "Vcb_TTLJ_powheg_CP5Down.root";
    map_mc["TTLJ_CP5Up"] = "Vcb_TTLJ_powheg_CP5Up.root";
    map_mc["TTLJ_mtop171p5"] = "Vcb_TTLJ_powheg_mtop171p5.root";
    map_mc["TTLJ_mtop173p5"] = "Vcb_TTLJ_powheg_mtop173p5.root";

    map_short_name_mc["TTLJ_hdampDown"] = "TTLJ_hdampDown";
    map_short_name_mc["TTLJ_hdampUp"] = "TTLJ_hdampUp";
    map_short_name_mc["TTLJ_CP5Down"] = "TTLJ_CP5Down";
    map_short_name_mc["TTLJ_CP5Up"] = "TTLJ_CP5Up";
    map_short_name_mc["TTLJ_mtop171p5"] = "TTLJ_mtop171p5";
    map_short_name_mc["TTLJ_mtop173p5"] = "TTLJ_mtop173p5";

    map_mc["TTLL_hdampDown"] = "Vcb_TTLL_powheg_hdampDown.root";
    map_mc["TTLL_hdampUp"] = "Vcb_TTLL_powheg_hdampUp.root";
    map_mc["TTLL_CP5Down"] = "Vcb_TTLL_powheg_CP5Down.root";
    map_mc["TTLL_CP5Up"] = "Vcb_TTLL_powheg_CP5Up.root";
    map_mc["TTLL_mtop171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
    map_mc["TTLL_mtop173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";

    map_short_name_mc["TTLL_hdampDown"] = "TTLL_hdampDown";
    map_short_name_mc["TTLL_hdampUp"] = "TTLL_hdampUp";
    map_short_name_mc["TTLL_CP5Down"] = "TTLL_CP5Down";
    map_short_name_mc["TTLL_CP5Up"] = "TTLL_CP5Up";
    map_short_name_mc["TTLL_mtop171p5"] = "TTLL_mtop171p5";
    map_short_name_mc["TTLL_mtop173p5"] = "TTLL_mtop173p5";
  
    for (auto it = map_mc.begin(); it != map_mc.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_Tagging_RF");
    /*
        map_mc.clear();
        map_short_name_mc.clear();

        map_mc["SingleTop_sch_Lep"] = "Vcb_SingleTop_sch_Lep.root";
        map_mc["SingleTop_tch_top_Incl"] = "Vcb_SingleTop_tch_top_Incl.root";
        map_mc["SingleTop_tch_antitop_Incl"] = "Vcb_SingleTop_tch_antitop_Incl.root";
        map_mc["SingleTop_tW_top"] = "Vcb_SingleTop_tW_top_NoFullyHad.root";
        map_mc["SingleTop_tW_antitop"] = "Vcb_SingleTop_tW_antitop_NoFullyHad.root";

        map_short_name_mc["SingleTop_sch_Lep"] = "ST";
        map_short_name_mc["SingleTop_tch_top_Incl"] = "ST";
        map_short_name_mc["SingleTop_tch_antitop_Incl"] = "ST";
        map_short_name_mc["SingleTop_tW_top"] = "ST";
        map_short_name_mc["SingleTop_tW_antitop"] = "ST";

        for (auto it = map_mc.begin(); it != map_mc.end(); it++)
          it->second.ReplaceAll("Vcb", "Vcb_Tagging_RF");
          */
  } // if (analyser == "Vcb_Tagging_RF")
  else if (analyser == "Vcb_DL")
  {
    for (auto it = map_mc.begin(); it != map_mc.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_DL");

    for (auto it = map_top_syst.begin(); it != map_top_syst.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_DL");

    for (auto it = map_data.begin(); it != map_data.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_DL");
  } // if (analyser == "Vcb_DL")
  else if (analyser == "Vcb_Tagging_RF_DL")
  {
    map_mc["TTLJ_hdampDown"] = "Vcb_TTLJ_powheg_hdampDown.root";
    map_mc["TTLJ_hdampUp"] = "Vcb_TTLJ_powheg_hdampUp.root";
    map_mc["TTLJ_CP5Down"] = "Vcb_TTLJ_powheg_CP5Down.root";
    map_mc["TTLJ_CP5Up"] = "Vcb_TTLJ_powheg_CP5Up.root";
    map_mc["TTLJ_mtop171p5"] = "Vcb_TTLJ_powheg_mtop171p5.root";
    map_mc["TTLJ_mtop173p5"] = "Vcb_TTLJ_powheg_mtop173p5.root";

    map_short_name_mc["TTLJ_hdampDown"] = "TTLJ_hdampDown";
    map_short_name_mc["TTLJ_hdampUp"] = "TTLJ_hdampUp";
    map_short_name_mc["TTLJ_CP5Down"] = "TTLJ_CP5Down";
    map_short_name_mc["TTLJ_CP5Up"] = "TTLJ_CP5Up";
    map_short_name_mc["TTLJ_mtop171p5"] = "TTLJ_mtop171p5";
    map_short_name_mc["TTLJ_mtop173p5"] = "TTLJ_mtop173p5";

    map_mc["TTLL_hdampDown"] = "Vcb_TTLL_powheg_hdampDown.root";
    map_mc["TTLL_hdampUp"] = "Vcb_TTLL_powheg_hdampUp.root";
    map_mc["TTLL_CP5Down"] = "Vcb_TTLL_powheg_CP5Down.root";
    map_mc["TTLL_CP5Up"] = "Vcb_TTLL_powheg_CP5Up.root";
    map_mc["TTLL_mtop171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
    map_mc["TTLL_mtop173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";

    map_short_name_mc["TTLL_hdampDown"] = "TTLL_hdampDown";
    map_short_name_mc["TTLL_hdampUp"] = "TTLL_hdampUp";
    map_short_name_mc["TTLL_CP5Down"] = "TTLL_CP5Down";
    map_short_name_mc["TTLL_CP5Up"] = "TTLL_CP5Up";
    map_short_name_mc["TTLL_mtop171p5"] = "TTLL_mtop171p5";
    map_short_name_mc["TTLL_mtop173p5"] = "TTLL_mtop173p5";

    for (auto it = map_mc.begin(); it != map_mc.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_Tagging_RF_DL");
  } // else if (analyzer == "Vcb_Tagging_RF_DL")
} // Samples::Samples(const TString& a_era, const TString& a_channel)

//////////

Samples::~Samples()
{
}

//////////
