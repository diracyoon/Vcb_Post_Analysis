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
  map_mc["TTLJ_TTbb_4f"] = "Vcb_TTbb_4f_TTLJ.root";
  map_mc["TTLJ_bbDPS"] = "Vcb_TTLJ_bbDPS.root";
  map_mc["TTLL"] = "Vcb_TTLL_powheg.root";
  map_mc["TTLL_TTbb_4f"] = "Vcb_TTbb_4f_TTLL.root";
  map_mc["TTLL_bbDPS"] = "Vcb_TTLL_bbDPS.root";
  map_mc["SingleTop_sch_Lep"] = "Vcb_SingleTop_sch_Lep.root";
  map_mc["SingleTop_tch_top_Incl"] = "Vcb_SingleTop_tch_top_Incl.root";
  map_mc["SingleTop_tch_antitop_Incl"] = "Vcb_SingleTop_tch_antitop_Incl.root";
  map_mc["SingleTop_tW_top"] = "Vcb_SingleTop_tW_top_NoFullyHad.root";
  map_mc["SingleTop_tW_antitop"] = "Vcb_SingleTop_tW_antitop_NoFullyHad.root";
  // map_mc["WJets"] = "Vcb_WJets_MG.root";
  map_mc["WJets_HT100to200"] = "Vcb_WJets_HT100to200.root";
  map_mc["WJets_HT200to400"] = "Vcb_WJets_HT200to400.root";
  map_mc["WJets_HT400to600"] = "Vcb_WJets_HT400to600.root";
  map_mc["WJets_HT600to800"] = "Vcb_WJets_HT600to800.root";
  map_mc["WJets_HT800to1200"] = "Vcb_WJets_HT800to1200.root";
  map_mc["WJets_HT1200to2500"] = "Vcb_WJets_HT1200to2500.root";
  map_mc["WJets_HT2500toInf"] = "Vcb_WJets_HT2500toInf.root";
  // map_mc["WJets"] = "Vcb_WJets_Sherpa.root";
  map_mc["DYJets"] = "Vcb_DYJets_MG.root";
  // map_mc["DYJets"] = "Vcb_DYJets.root";
  map_mc["QCD_bEn_HT100to200"] = "Vcb_QCD_bEnriched_HT100to200.root";
  map_mc["QCD_bEn_HT200to300"] = "Vcb_QCD_bEnriched_HT200to300.root";
  map_mc["QCD_bEn_HT300to500"] = "Vcb_QCD_bEnriched_HT300to500.root";
  map_mc["QCD_bEn_HT500to700"] = "Vcb_QCD_bEnriched_HT500to700.root";
  map_mc["QCD_bEn_HT700to1000"] = "Vcb_QCD_bEnriched_HT700to1000.root";
  map_mc["QCD_bEn_HT1000to1500"] = "Vcb_QCD_bEnriched_HT1000to1500.root";
  map_mc["QCD_bEn_HT1500to2000"] = "Vcb_QCD_bEnriched_HT1500to2000.root";
  map_mc["QCD_bEn_HT2000toInf"] = "Vcb_QCD_bEnriched_HT2000toInf.root";
  // map_mc["QCD_Pt_15to30"] = "Vcb_QCD_Pt_15to30.root";
  // map_mc["QCD_Pt_30to50"] = "Vcb_QCD_Pt_30to50.root";
  // map_mc["QCD_Pt_50to80"] = "Vcb_QCD_Pt_50to80.root";
  // map_mc["QCD_Pt_80to120"] = "Vcb_QCD_Pt_80to120.root";
  // map_mc["QCD_Pt_120to170"] = "Vcb_QCD_Pt_120to170.root";
  // map_mc["QCD_Pt_170to300"] = "Vcb_QCD_Pt_170to300.root";
  // map_mc["QCD_Pt_300to470"] = "Vcb_QCD_Pt_300to470.root";
  // map_mc["QCD_Pt_470to600"] = "Vcb_QCD_Pt_470to600.root";
  // map_mc["QCD_Pt_600to800"] = "Vcb_QCD_Pt_600to800.root";
  // map_mc["QCD_Pt_800to1000"] = "Vcb_QCD_Pt_800to1000.root";
  // map_mc["QCD_Pt_1000to1400"] = "Vcb_QCD_Pt_1000to1400.root";
  // map_mc["QCD_Pt_1400to1800"] = "Vcb_QCD_Pt_1400to1800.root";
  // map_mc["QCD_Pt_1800to2400"] = "Vcb_QCD_Pt_1800to2400.root";
  // map_mc["QCD_Pt_2400to3200"] = "Vcb_QCD_Pt_2400to3200.root";
  // map_mc["QCD_Pt_3200toInf"] = "Vcb_QCD_Pt_3200toInf.root";
  map_mc["ttHTobb"] = "Vcb_ttHTobb.root";
  map_mc["ttHToNonbb"] = "Vcb_ttHToNonbb.root";
  map_mc["ttWToLNu"] = "Vcb_ttWToLNu.root";
  map_mc["ttWToQQ"] = "Vcb_ttWToQQ.root";
  map_mc["ttZToLLNuNu"] = "Vcb_ttZToLLNuNu.root";
  map_mc["ttZToQQ_ll"] = "Vcb_ttZToQQ_ll.root";
  map_mc["ttZToQQ"] = "Vcb_ttZToQQ.root";
  map_mc["WW"] = "Vcb_WW_pythia.root";
  map_mc["WZ"] = "Vcb_WZ_pythia.root";
  map_mc["ZZ"] = "Vcb_ZZ_pythia.root";

  map_short_name_mc["TTLJ_WtoCB"] = "TTLJ_WtoCB";
  map_short_name_mc["TTLJ"] = "TTLJ";
  map_short_name_mc["TTLL"] = "TTLL";
  // map_short_name_mc["TTJJ"] = "TTJJ";
  map_short_name_mc["TTLJ_TTbb_4f"] = "TTLJ_TTbb_4f";
  map_short_name_mc["TTLJ_bbDPS"] = "TTLJ_bbDPS";
  map_short_name_mc["TTLL_TTbb_4f"] = "TTLL_TTbb_4f";
  map_short_name_mc["TTLL_bbDPS"] = "TTLL_bbDPS";
  map_short_name_mc["SingleTop_sch_Lep"] = "ST_sch";
  map_short_name_mc["SingleTop_tch_top_Incl"] = "ST_tch";
  map_short_name_mc["SingleTop_tch_antitop_Incl"] = "ST_tch";
  map_short_name_mc["SingleTop_tW_top"] = "ST_tw";
  map_short_name_mc["SingleTop_tW_antitop"] = "ST_tw";
  // map_short_name_mc["WJets"] = "WJets";
  map_short_name_mc["WJets_HT100to200"] = "WJets";
  map_short_name_mc["WJets_HT200to400"] = "WJets";
  map_short_name_mc["WJets_HT400to600"] = "WJets";
  map_short_name_mc["WJets_HT600to800"] = "WJets";
  map_short_name_mc["WJets_HT800to1200"] = "WJets";
  map_short_name_mc["WJets_HT1200to2500"] = "WJets";
  map_short_name_mc["WJets_HT2500toInf"] = "WJets";
  map_short_name_mc["DYJets"] = "DYJets";
  map_short_name_mc["QCD_bEn_HT100to200"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT200to300"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT300to500"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT500to700"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT700to1000"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT1000to1500"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT1500to2000"] = "QCD_bEn";
  map_short_name_mc["QCD_bEn_HT2000toInf"] = "QCD_bEn";
  // map_short_name_mc["QCD_Pt_15to30"] = "QCD";
  // map_short_name_mc["QCD_Pt_30to50"] = "QCD";
  // map_short_name_mc["QCD_Pt_50to80"] = "QCD";
  // map_short_name_mc["QCD_Pt_80to120"] = "QCD";
  // map_short_name_mc["QCD_Pt_120to170"] = "QCD";
  // map_short_name_mc["QCD_Pt_170to300"] = "QCD";
  // map_short_name_mc["QCD_Pt_300to470"] = "QCD";
  // map_short_name_mc["QCD_Pt_470to600"] = "QCD";
  // map_short_name_mc["QCD_Pt_600to800"] = "QCD";
  // map_short_name_mc["QCD_Pt_800to1000"] = "QCD";
  // map_short_name_mc["QCD_Pt_1000to1400"] = "QCD";
  // map_short_name_mc["QCD_Pt_1400to1800"] = "QCD";
  // map_short_name_mc["QCD_Pt_1800to2400"] = "QCD";
  // map_short_name_mc["QCD_Pt_2400to3200"] = "QCD";
  // map_short_name_mc["QCD_Pt_3200toInf"] = "QCD";
  map_short_name_mc["ttHTobb"] = "ttHTobb";
  map_short_name_mc["ttHToNonbb"] = "ttHToNonbb";
  map_short_name_mc["ttWToLNu"] = "ttWToLNu";
  map_short_name_mc["ttWToQQ"] = "ttWToQQ";
  map_short_name_mc["ttZToLLNuNu"] = "ttZToLLNuNu";
  map_short_name_mc["ttZToQQ_ll"] = "ttZToQQ";
  map_short_name_mc["ttZToQQ"] = "ttZToQQ";
  map_short_name_mc["WW"] = "WW";
  map_short_name_mc["WZ"] = "WZ";
  map_short_name_mc["ZZ"] = "ZZ";

  map_short_short_name["TTLJ_WtoCB"] = "TTLJ_WtoCB";
  map_short_short_name["TTLJ"] = "TTLJ";
  map_short_short_name["TTLL"] = "TTLL";
  // map_short_short_name["TTJJ"] = "TTJJ";
  map_short_short_name["TTbb_4f_TTLJ"] = "TTLJ_TTbb";
  map_short_short_name["TTLJ_bbDPS"] = "TTLJ_TTbb";
  map_short_short_name["TTbb_4f_TTLL"] = "TTLL_TTbb";
  map_short_short_name["TTLL_bbDPS"] = "TTLL_TTbb";
  map_short_short_name["ST_sch"] = "ST";
  map_short_short_name["ST_tch"] = "ST";
  map_short_short_name["ST_tw"] = "ST";
  map_short_short_name["WJets"] = "VJets";
  map_short_short_name["DYJets"] = "VJets";
  map_short_short_name["QCD_bEn"] = "QCD_bEn";
  // map_short_short_name["QCD"] = "QCD";
  map_short_short_name["ttHTobb"] = "ttV";
  map_short_short_name["ttHToNonbb"] = "ttV";
  map_short_short_name["ttWToLNu"] = "ttV";
  map_short_short_name["ttWToQQ"] = "ttV";
  map_short_short_name["ttZToLLNuNu"] = "ttV";
  map_short_short_name["ttZToQQ_ll"] = "ttV";
  map_short_short_name["ttZToQQ"] = "ttV";
  map_short_short_name["WW"] = "VV";
  map_short_short_name["WZ"] = "VV";
  map_short_short_name["ZZ"] = "VV";

  map_top_syst["TTLJ_CP5Down"] = "Vcb_TTLJ_powheg_CP5Down.root";
  map_top_syst["TTLJ_CP5Up"] = "Vcb_TTLJ_powheg_CP5Up.root";
  map_top_syst["TTLJ_hdampDown"] = "Vcb_TTLJ_powheg_hdampDown.root";
  map_top_syst["TTLJ_hdampUp"] = "Vcb_TTLJ_powheg_hdampUp.root";
  map_top_syst["TTLJ_mtop171p5"] = "Vcb_TTLJ_powheg_mtop171p5.root";
  map_top_syst["TTLJ_mtop173p5"] = "Vcb_TTLJ_powheg_mtop173p5.root";
  // map_top_syst["TTLJ_TTbb_4f"] = "Vcb_TTbb_4f_TTLJ.root";
  // map_top_syst["TTLJ_bbDPS"] = "Vcb_TTLJ_bbDPS.root";

  map_top_syst["TTLL_CP5Down"] = "Vcb_TTLL_powheg_CP5Down.root";
  map_top_syst["TTLL_CP5Up"] = "Vcb_TTLL_powheg_CP5Up.root";
  map_top_syst["TTLL_hdampDown"] = "Vcb_TTLL_powheg_hdampDown.root";
  map_top_syst["TTLL_hdampUp"] = "Vcb_TTLL_powheg_hdampUp.root";
  map_top_syst["TTLL_mtop171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
  map_top_syst["TTLL_mtop173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";
  // map_top_syst["TTLL_TTbb_4f"] = "Vcb_TTbb_4f_TTLL.root";
  // map_top_syst["TTLL_bbDPS"] = "Vcb_TTLL_bbDPS.root";

  map_top_syst["TTLJ_WtoCB_CP5Down"] = "Vcb_TTLJ_WtoCB_powheg_CP5Down.root";
  map_top_syst["TTLJ_WtoCB_CP5Up"] = "Vcb_TTLJ_WtoCB_powheg_CP5Up.root";
  map_top_syst["TTLJ_WtoCB_hdampDown"] = "Vcb_TTLJ_WtoCB_powheg_hdampDown.root";
  map_top_syst["TTLJ_WtoCB_hdampUp"] = "Vcb_TTLJ_WtoCB_powheg_hdampUp.root";
  map_top_syst["TTLJ_WtoCB_mtop171p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop171p5.root";
  map_top_syst["TTLJ_WtoCB_mtop173p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop173p5.root";

  if (era == "2016preVFP")
    vec_period = {"B_ver2", "C", "D", "E", "F"};
  else if (era == "2016postVFP")
    vec_period = {"F", "G", "H"};
  if (era == "2017")
    vec_period = {"B", "C", "D", "E", "F"};
  else if (era == "2018")
    vec_period = {"A", "B", "C", "D"};

  if (channel == "Mu")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["SingleMuon_" + vec_period[i]] = "Vcb_SingleMuon_" + vec_period[i] + ".root";
      map_short_name_data["SingleMuon_" + vec_period[i]] = "SingleMuon";
    }
  } // if(channel=="Mu")
  else if (channel == "El")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["EGamma_" + vec_period[i]] = "Vcb_EGamma_" + vec_period[i] + ".root";
      map_short_name_data["EGamma_" + vec_period[i]] = "EGamma";
    }
  } // if(channel=="El")
  else if (channel == "MM")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["SingleMuon_" + vec_period[i]] = "Vcb_SingleMuon_" + vec_period[i] + ".root";
      map_short_name_data["SingleMuon_" + vec_period[i]] = "SingleMuon";
    }
  } // else if (channel == "MM")
  else if (channel == "ME")
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
  } // else if (channel == "ME")
  else if (channel == "EE")
  {
    for (unsigned int i = 0; i < vec_period.size(); i++)
    {
      map_data["EGamma_" + vec_period[i]] = "Vcb_EGamma_" + vec_period[i] + ".root";
      map_short_name_data["EGamma_" + vec_period[i]] = "EGamma";
    }
  } // else if(channel == "EE")

  if (era != "2018")
  {
    for (auto it = map_data.begin(); it != map_data.end(); it++)
    {
      if (it->second.Contains("EGamma"))
        it->second.ReplaceAll("EGamma", "SingleElectron");
    }
  }

  if (analyser == "Vcb_Cal_TF")
  {
    for (auto it = map_mc.begin(); it != map_mc.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_Cal_TF");

    for (auto it = map_data.begin(); it != map_data.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_Cal_TF");
  }
  else if (analyser == "Vcb_Modelling_Patch")
  {
    map_mc["TTLJ_hdampDown"] = "Vcb_TTLJ_powheg_hdampDown.root";
    map_mc["TTLJ_hdampUp"] = "Vcb_TTLJ_powheg_hdampUp.root";
    map_mc["TTLJ_CP5Down"] = "Vcb_TTLJ_powheg_CP5Down.root";
    map_mc["TTLJ_CP5Up"] = "Vcb_TTLJ_powheg_CP5Up.root";
    map_mc["TTLJ_mtop171p5"] = "Vcb_TTLJ_powheg_mtop171p5.root";
    map_mc["TTLJ_mtop173p5"] = "Vcb_TTLJ_powheg_mtop173p5.root";
    // map_mc["TTLJ_TTbb_4f"] = "Vcb_TTbb_4f_TTLJ.root";
    // map_mc["TTLJ_bbDPS"] = "Vcb_TTLJ_bbDPS.root";

    map_short_name_mc["TTLJ_hdampDown"] = "TTLJ_hdampDown";
    map_short_name_mc["TTLJ_hdampUp"] = "TTLJ_hdampUp";
    map_short_name_mc["TTLJ_CP5Down"] = "TTLJ_CP5Down";
    map_short_name_mc["TTLJ_CP5Up"] = "TTLJ_CP5Up";
    map_short_name_mc["TTLJ_mtop171p5"] = "TTLJ_mtop171p5";
    map_short_name_mc["TTLJ_mtop173p5"] = "TTLJ_mtop173p5";
    // map_short_name_mc["TTLJ_TTbb_4f"] = "TTLJ_TTbb_4f";
    // map_short_name_mc["TTLJ_bbDPS"] = "TTLJ_bbDPS";

    map_mc["TTLL_hdampDown"] = "Vcb_TTLL_powheg_hdampDown.root";
    map_mc["TTLL_hdampUp"] = "Vcb_TTLL_powheg_hdampUp.root";
    map_mc["TTLL_CP5Down"] = "Vcb_TTLL_powheg_CP5Down.root";
    map_mc["TTLL_CP5Up"] = "Vcb_TTLL_powheg_CP5Up.root";
    map_mc["TTLL_mtop171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
    map_mc["TTLL_mtop173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";
    // map_mc["TTLL_TTbb_4f"] = "Vcb_TTbb_4f_TTLL.root";
    // map_mc["TTLL_bbDPS"] = "Vcb_TTLL_bbDPS.root";

    map_short_name_mc["TTLL_hdampDown"] = "TTLL_hdampDown";
    map_short_name_mc["TTLL_hdampUp"] = "TTLL_hdampUp";
    map_short_name_mc["TTLL_CP5Down"] = "TTLL_CP5Down";
    map_short_name_mc["TTLL_CP5Up"] = "TTLL_CP5Up";
    map_short_name_mc["TTLL_mtop171p5"] = "TTLL_mtop171p5";
    map_short_name_mc["TTLL_mtop173p5"] = "TTLL_mtop173p5";
    // map_short_name_mc["TTLL_TTbb_4f"] = "TTLL_TTbb_4f";
    // map_short_name_mc["TTLL_bbDPS"] = "TTLL_bbDPS";

    map_mc["TTLJ_WtoCB_CP5Down"] = "Vcb_TTLJ_WtoCB_powheg_CP5Down.root";
    map_mc["TTLJ_WtoCB_CP5Up"] = "Vcb_TTLJ_WtoCB_powheg_CP5Up.root";
    map_mc["TTLJ_WtoCB_hdampDown"] = "Vcb_TTLJ_WtoCB_powheg_hdampDown.root";
    map_mc["TTLJ_WtoCB_hdampUp"] = "Vcb_TTLJ_WtoCB_powheg_hdampUp.root";
    map_mc["TTLJ_WtoCB_mtop171p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop171p5.root";
    map_mc["TTLJ_WtoCB_mtop173p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop173p5.root";

    map_short_name_mc["TTLJ_WtoCB_CP5Down"] = "TTLJ_WtoCB_CP5Down";
    map_short_name_mc["TTLJ_WtoCB_CP5Up"] = "TTLJ_WtoCB_CP5Up";
    map_short_name_mc["TTLJ_WtoCB_hdampDown"] = "TTLJ_WtoCB_hdampDown";
    map_short_name_mc["TTLJ_WtoCB_hdampUp"] = "TTLJ_WtoCB_hdampUp";
    map_short_name_mc["TTLJ_WtoCB_mtop171p5"] = "TTLJ_WtoCB_mtop171p5";
    map_short_name_mc["TTLJ_WtoCB_mtop173p5"] = "TTLJ_WtoCB_mtop173p5";

    for (auto it = map_mc.begin(); it != map_mc.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_Modelling_Patch");
  }
  else if (analyser == "Vcb_Tagging_RF")
  {
    map_mc["TTLJ_hdampDown"] = "Vcb_TTLJ_powheg_hdampDown.root";
    map_mc["TTLJ_hdampUp"] = "Vcb_TTLJ_powheg_hdampUp.root";
    map_mc["TTLJ_CP5Down"] = "Vcb_TTLJ_powheg_CP5Down.root";
    map_mc["TTLJ_CP5Up"] = "Vcb_TTLJ_powheg_CP5Up.root";
    map_mc["TTLJ_mtop171p5"] = "Vcb_TTLJ_powheg_mtop171p5.root";
    map_mc["TTLJ_mtop173p5"] = "Vcb_TTLJ_powheg_mtop173p5.root";
    // map_mc["TTLJ_TTbb_4f"] = "Vcb_TTbb_4f_TTLJ.root";
    // map_mc["TTLJ_bbDPS"] = "Vcb_TTLJ_bbDPS.root";

    map_short_name_mc["TTLJ_hdampDown"] = "TTLJ_hdampDown";
    map_short_name_mc["TTLJ_hdampUp"] = "TTLJ_hdampUp";
    map_short_name_mc["TTLJ_CP5Down"] = "TTLJ_CP5Down";
    map_short_name_mc["TTLJ_CP5Up"] = "TTLJ_CP5Up";
    map_short_name_mc["TTLJ_mtop171p5"] = "TTLJ_mtop171p5";
    map_short_name_mc["TTLJ_mtop173p5"] = "TTLJ_mtop173p5";
    // map_short_name_mc["TTLJ_TTbb_4f"] = "TTLJ_TTbb_4f";
    // map_short_name_mc["TTLJ_bbDPS"] = "TTLJ_bbDPS";

    map_mc["TTLL_hdampDown"] = "Vcb_TTLL_powheg_hdampDown.root";
    map_mc["TTLL_hdampUp"] = "Vcb_TTLL_powheg_hdampUp.root";
    map_mc["TTLL_CP5Down"] = "Vcb_TTLL_powheg_CP5Down.root";
    map_mc["TTLL_CP5Up"] = "Vcb_TTLL_powheg_CP5Up.root";
    map_mc["TTLL_mtop171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
    map_mc["TTLL_mtop173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";
    // map_mc["TTLL_TTbb_4f"] = "Vcb_TTbb_4f_TTLL.root";
    // map_mc["TTLL_bbDPS"] = "Vcb_TTLL_bbDPS.root";

    map_short_name_mc["TTLL_hdampDown"] = "TTLL_hdampDown";
    map_short_name_mc["TTLL_hdampUp"] = "TTLL_hdampUp";
    map_short_name_mc["TTLL_CP5Down"] = "TTLL_CP5Down";
    map_short_name_mc["TTLL_CP5Up"] = "TTLL_CP5Up";
    map_short_name_mc["TTLL_mtop171p5"] = "TTLL_mtop171p5";
    map_short_name_mc["TTLL_mtop173p5"] = "TTLL_mtop173p5";
    // map_short_name_mc["TTLL_TTbb_4f"] = "TTLL_TTbb_4f";
    // map_short_name_mc["TTLL_bbDPS"] = "TTLL_bbDPS";

    map_mc["TTLJ_WtoCB_CP5Down"] = "Vcb_TTLJ_WtoCB_powheg_CP5Down.root";
    map_mc["TTLJ_WtoCB_CP5Up"] = "Vcb_TTLJ_WtoCB_powheg_CP5Up.root";
    map_mc["TTLJ_WtoCB_hdampDown"] = "Vcb_TTLJ_WtoCB_powheg_hdampDown.root";
    map_mc["TTLJ_WtoCB_hdampUp"] = "Vcb_TTLJ_WtoCB_powheg_hdampUp.root";
    map_mc["TTLJ_WtoCB_mtop171p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop171p5.root";
    map_mc["TTLJ_WtoCB_mtop173p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop173p5.root";

    map_short_name_mc["TTLJ_WtoCB_CP5Down"] = "TTLJ_WtoCB_CP5Down";
    map_short_name_mc["TTLJ_WtoCB_CP5Up"] = "TTLJ_WtoCB_CP5Up";
    map_short_name_mc["TTLJ_WtoCB_hdampDown"] = "TTLJ_WtoCB_hdampDown";
    map_short_name_mc["TTLJ_WtoCB_hdampUp"] = "TTLJ_WtoCB_hdampUp";
    map_short_name_mc["TTLJ_WtoCB_mtop171p5"] = "TTLJ_WtoCB_mtop171p5";
    map_short_name_mc["TTLJ_WtoCB_mtop173p5"] = "TTLJ_WtoCB_mtop173p5";

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
    // map_mc["TTLJ_TTbb_4f"] = "Vcb_TTbb_4f_TTLJ.root";
    // map_mc["TTLJ_bbDPS"] = "Vcb_TTLJ_bbDPS.root";

    map_short_name_mc["TTLJ_hdampDown"] = "TTLJ_hdampDown";
    map_short_name_mc["TTLJ_hdampUp"] = "TTLJ_hdampUp";
    map_short_name_mc["TTLJ_CP5Down"] = "TTLJ_CP5Down";
    map_short_name_mc["TTLJ_CP5Up"] = "TTLJ_CP5Up";
    map_short_name_mc["TTLJ_mtop171p5"] = "TTLJ_mtop171p5";
    map_short_name_mc["TTLJ_mtop173p5"] = "TTLJ_mtop173p5";
    // map_short_name_mc["TTLJ_TTbb_4f"] = "TTLJ_TTbb_4f";
    // map_short_name_mc["TTLJ_bbDPS"] = "TTLJ_bbDPS";

    map_mc["TTLL_hdampDown"] = "Vcb_TTLL_powheg_hdampDown.root";
    map_mc["TTLL_hdampUp"] = "Vcb_TTLL_powheg_hdampUp.root";
    map_mc["TTLL_CP5Down"] = "Vcb_TTLL_powheg_CP5Down.root";
    map_mc["TTLL_CP5Up"] = "Vcb_TTLL_powheg_CP5Up.root";
    map_mc["TTLL_mtop171p5"] = "Vcb_TTLL_powheg_mtop171p5.root";
    map_mc["TTLL_mtop173p5"] = "Vcb_TTLL_powheg_mtop173p5.root";
    // map_mc["TTLL_TTbb_4f"] = "Vcb_TTbb_4f_TTLL.root";
    // map_mc["TTLL_bbDPS"] = "Vcb_TTLL_bbDPS.root";

    map_short_name_mc["TTLL_hdampDown"] = "TTLL_hdampDown";
    map_short_name_mc["TTLL_hdampUp"] = "TTLL_hdampUp";
    map_short_name_mc["TTLL_CP5Down"] = "TTLL_CP5Down";
    map_short_name_mc["TTLL_CP5Up"] = "TTLL_CP5Up";
    map_short_name_mc["TTLL_mtop171p5"] = "TTLL_mtop171p5";
    map_short_name_mc["TTLL_mtop173p5"] = "TTLL_mtop173p5";
    // map_short_name_mc["TTLL_TTbb_4f"] = "TTLL_TTbb_4f";
    // map_short_name_mc["TTLL_bbDPS"] = "TTLL_bbDPS";

    map_mc["TTLJ_WtoCB_CP5Down"] = "Vcb_TTLJ_WtoCB_powheg_CP5Down.root";
    map_mc["TTLJ_WtoCB_CP5Up"] = "Vcb_TTLJ_WtoCB_powheg_CP5Up.root";
    map_mc["TTLJ_WtoCB_hdampDown"] = "Vcb_TTLJ_WtoCB_powheg_hdampDown.root";
    map_mc["TTLJ_WtoCB_hdampUp"] = "Vcb_TTLJ_WtoCB_powheg_hdampUp.root";
    map_mc["TTLJ_WtoCB_mtop171p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop171p5.root";
    map_mc["TTLJ_WtoCB_mtop173p5"] = "Vcb_TTLJ_WtoCB_powheg_mtop173p5.root";

    map_short_name_mc["TTLJ_WtoCB_CP5Down"] = "TTLJ_WtoCB_CP5Down";
    map_short_name_mc["TTLJ_WtoCB_CP5Up"] = "TTLJ_WtoCB_CP5Up";
    map_short_name_mc["TTLJ_WtoCB_hdampDown"] = "TTLJ_WtoCB_hdampDown";
    map_short_name_mc["TTLJ_WtoCB_hdampUp"] = "TTLJ_WtoCB_hdampUp";
    map_short_name_mc["TTLJ_WtoCB_mtop171p5"] = "TTLJ_WtoCB_mtop171p5";
    map_short_name_mc["TTLJ_WtoCB_mtop173p5"] = "TTLJ_WtoCB_mtop173p5";

    for (auto it = map_mc.begin(); it != map_mc.end(); it++)
      it->second.ReplaceAll("Vcb", "Vcb_Tagging_RF_DL");
  } // else if (analyzer == "Vcb_Tagging_RF_DL")
} // Samples::Samples(const TString& a_era, const TString& a_channel)

//////////

Samples::~Samples()
{
  // cout << "[Samples::~Samples]: Done" << endl;
}

//////////
