#ifndef __Const_Def_H__
#define __Const_Def_H__

#include <iostream>

#include <TString.h>

using namespace std;

const bool chk_include_pseudo_additional = true;

// 2016preVFP
const float bvsc_wp_l_2016preVFP = 0.0508;
const float bvsc_wp_m_2016preVFP = 0.2598;
const float bvsc_wp_t_2016preVFP = 0.6502;

const float cvsl_wp_l_2016preVFP = 0.039;
const float cvsl_wp_m_2016preVFP = 0.098;
const float cvsl_wp_t_2016preVFP = 0.270;

const float cvsb_wp_l_2016preVFP = 0.327;
const float cvsb_wp_m_2016preVFP = 0.370;
const float cvsb_wp_t_2016preVFP = 0.256;

// 2016postVFP
const float bvsc_wp_l_2016postVFP = 0.0480;
const float bvsc_wp_m_2016postVFP = 0.2489;
const float bvsc_wp_t_2016postVFP = 0.6377;

const float cvsl_wp_l_2016postVFP = 0.039;
const float cvsl_wp_m_2016postVFP = 0.099;
const float cvsl_wp_t_2016postVFP = 0.269;

const float cvsb_wp_l_2016postVFP = 0.305;
const float cvsb_wp_m_2016postVFP = 0.353;
const float cvsb_wp_t_2016postVFP = 0.247;

// 2017
const float bvsc_wp_l_2017 = 0.0532;
const float bvsc_wp_m_2017 = 0.3040;
const float bvsc_wp_t_2017 = 0.7476;

const float cvsl_wp_l_2017 = 0.030;
const float cvsl_wp_m_2017 = 0.085;
const float cvsl_wp_y_2017 = 0.520;

const float cvsb_wp_l_2017 = 0.400;
const float cvsb_wp_m_2017 = 0.340;
const float cvsb_wp_y_2017 = 0.050;

// 2018
const float bvsc_wp_l_2018 = 0.0490;
const float bvsc_wp_m_2018 = 0.2783;
const float bvsc_wp_t_2018 = 0.7100;

const float cvsl_wp_l_2018 = 0.038;
const float cvsl_wp_m_2018 = 0.099;
const float cvsl_wp_t_2018 = 0.282;

const float cvsb_wp_l_2018 = 0.246;
const float cvsb_wp_m_2018 = 0.325;
const float cvsb_wp_t_2018 = 0.267;

const float Z_MASS = 91.1876;

// ckm pdg 2023
const float Vud_pdg = 0.97373;
const float Vus_pdg = 0.2243;
const float Vub_pdg = 0.00382;

const float Vcd_pdg = 0.221;
const float Vcs_pdg = 0.975;
const float Vcb_pdg = 0.0408;

// ckm powheg
const float Vud_squared_powheg = 0.95;
const float Vus_squared_powheg = 0.05;
const float Vub_squared_powheg = 0;

const float Vcd_squared_powheg = 0.05;
const float Vcs_squared_powheg = 0.95;
const float Vcb_squared_powheg = 0.0;

const float lumi_2016a = 19.52;
const float lumi_2016b = 16.81;
const float lumi_2017 = 41.53;
const float lumi_2018 = 59.74;

const float MUON_ETA = 2.4;
const float ELECTRON_ETA = 2.5;

const float REL_ISO_MUON = 0.15;

// medium
const float REL_ISO_ELECTRON_BARREL_A = 0.0478;
const float REL_ISO_ELECTRON_BARREL_B = 0.506;
const float REL_ISO_ELECTRON_ENDCAP_A = 0.0658;
const float REL_ISO_ELECTRON_ENDCAP_B = 0.963;

const float MET_PT = 20;

const float TTBB_Scale = 1.36;
const float TTCC_Scale = 1.11;

const float Cross_Section_Total_TT = 453.63;
const float Cross_Seciton_Total_TT_JJ = 408.0950984; // 447.7876174;
const float Cross_Section_Total_TTCC = 35.08476781;  // 4.060488667;
const float Cross_Section_Total_TTBB = 10.45013377;  // 1.781893947;

typedef enum Syst_
{
    Central,
    JetResDown,
    JetResUp,
    JetEnDown,
    JetEnUp,
    JetEnAbsoluteDown,
    JetEnAbsoluteUp,
    JetEnBBEC1Down,
    JetEnBBEC1Up,
    JetEnEC2Down,
    JetEnEC2Up,
    JetEnFlavorQCDDown,
    JetEnFlavorQCDUp,
    JetEnHFDown,
    JetEnHFUp,
    JetEnRelativeBalDown,
    JetEnRelativeBalUp,
    JetEnAbsolute2016Down,
    JetEnAbsolute2016Up,
    JetEnBBEC12016Down,
    JetEnBBEC12016Up,
    JetEnEC22016Down,
    JetEnEC22016Up,
    JetEnHF2016Down,
    JetEnHF2016Up,
    JetEnRelativeSample2016Down,
    JetEnRelativeSample2016Up,
    JetEnAbsolute2017Down,
    JetEnAbsolute2017Up,
    JetEnBBEC12017Down,
    JetEnBBEC12017Up,
    JetEnEC22017Down,
    JetEnEC22017Up,
    JetEnHF2017Down,
    JetEnHF2017Up,
    JetEnRelativeSample2017Down,
    JetEnRelativeSample2017Up,
    JetEnAbsolute2018Down,
    JetEnAbsolute2018Up,
    JetEnBBEC12018Down,
    JetEnBBEC12018Up,
    JetEnEC22018Down,
    JetEnEC22018Up,
    JetEnHF2018Down,
    JetEnHF2018Up,
    JetEnRelativeSample2018Down,
    JetEnRelativeSample2018Up,
    /* MuonEnUp, MuonEnDown,  */
    ElectronResDown,
    ElectronResUp,
    ElectronEnDown,
    ElectronEnUp,
    UEDown,
    UEUp
} Syst;

static Syst StringToSyst(const TString &syst_str)
{
    if (syst_str == "Central")
        return Syst::Central;
    else if (syst_str == "JetResDown")
        return Syst::JetResDown;
    else if (syst_str == "JetResUp")
        return Syst::JetResUp;
    else if (syst_str == "JetEnDown")
        return Syst::JetEnDown;
    else if (syst_str == "JetEnUp")
        return Syst::JetEnUp;
    else if (syst_str == "JetEnAbsoluteDown")
        return Syst::JetEnAbsoluteDown;
    else if (syst_str == "JetEnAbsoluteUp")
        return Syst::JetEnAbsoluteUp;
    else if (syst_str == "JetEnBBEC1Down")
        return Syst::JetEnBBEC1Down;
    else if (syst_str == "JetEnBBEC1Up")
        return Syst::JetEnBBEC1Up;
    else if (syst_str == "JetEnEC2Down")
        return Syst::JetEnEC2Down;
    else if (syst_str == "JetEnEC2Up")
        return Syst::JetEnEC2Up;
    else if (syst_str == "JetEnFlavorQCDDown")
        return Syst::JetEnFlavorQCDDown;
    else if (syst_str == "JetEnFlavorQCDUp")
        return Syst::JetEnFlavorQCDUp;
    else if (syst_str == "JetEnHFDown")
        return Syst::JetEnHFDown;
    else if (syst_str == "JetEnHFUp")
        return Syst::JetEnHFUp;
    else if (syst_str == "JetEnRelativeBalDown")
        return Syst::JetEnRelativeBalDown;
    else if (syst_str == "JetEnRelativeBalUp")
        return Syst::JetEnRelativeBalUp;
    else if (syst_str == "JetEnAbsolute2016Down")
        return Syst::JetEnAbsolute2016Down;
    else if (syst_str == "JetEnAbsolute2016Up")
        return Syst::JetEnAbsolute2016Up;
    else if (syst_str == "JetEnBBEC12016Down")
        return Syst::JetEnBBEC12016Down;
    else if (syst_str == "JetEnBBEC12016Up")
        return Syst::JetEnBBEC12016Up;
    else if (syst_str == "JetEnEC22016Down")
        return Syst::JetEnEC22016Down;
    else if (syst_str == "JetEnEC22016Up")
        return Syst::JetEnEC22016Up;
    else if (syst_str == "JetEnHF2016Down")
        return Syst::JetEnHF2016Down;
    else if (syst_str == "JetEnHF2016Up")
        return Syst::JetEnHF2016Up;
    else if (syst_str == "JetEnRelativeSample2016Down")
        return Syst::JetEnRelativeSample2016Down;
    else if (syst_str == "JetEnRelativeSample2016Up")
        return Syst::JetEnRelativeSample2016Up;
    else if (syst_str == "JetEnAbsolute2017Down")
        return Syst::JetEnAbsolute2017Down;
    else if (syst_str == "JetEnAbsolute2017Up")
        return Syst::JetEnAbsolute2017Up;
    else if (syst_str == "JetEnBBEC12017Down")
        return Syst::JetEnBBEC12017Down;
    else if (syst_str == "JetEnBBEC12017Up")
        return Syst::JetEnBBEC12017Up;
    else if (syst_str == "JetEnEC22017Down")
        return Syst::JetEnEC22017Down;
    else if (syst_str == "JetEnEC22017Up")
        return Syst::JetEnEC22017Up;
    else if (syst_str == "JetEnHF2017Down")
        return Syst::JetEnHF2017Down;
    else if (syst_str == "JetEnHF2017Up")
        return Syst::JetEnHF2017Up;
    else if (syst_str == "JetEnRelativeSample2017Down")
        return Syst::JetEnRelativeSample2017Down;
    else if (syst_str == "JetEnRelativeSample2017Up")
        return Syst::JetEnRelativeSample2017Up;
    else if (syst_str == "JetEnAbsolute2018Down")
        return Syst::JetEnAbsolute2018Down;
    else if (syst_str == "JetEnAbsolute2018Up")
        return Syst::JetEnAbsolute2018Up;
    else if (syst_str == "JetEnBBEC12018Down")
        return Syst::JetEnBBEC12018Down;
    else if (syst_str == "JetEnBBEC12018Up")
        return Syst::JetEnBBEC12018Up;
    else if (syst_str == "JetEnEC22018Down")
        return Syst::JetEnEC22018Down;
    else if (syst_str == "JetEnEC22018Up")
        return Syst::JetEnEC22018Up;
    else if (syst_str == "JetEnHF2018Down")
        return Syst::JetEnHF2018Down;
    else if (syst_str == "JetEnHF2018Up")
        return Syst::JetEnHF2018Up;
    else if (syst_str == "JetEnRelativeSample2018Down")
        return Syst::JetEnRelativeSample2018Down;
    else if (syst_str == "JetEnRelativeSample2018Up")
        return Syst::JetEnRelativeSample2017Up;
    else if (syst_str == "ElectronResDown")
        return Syst::ElectronResDown;
    else if (syst_str == "ElectronResUp")
        return Syst::ElectronResUp;
    else if (syst_str == "ElectronEnDown")
        return Syst::ElectronEnDown;
    else if (syst_str == "ElectronEnUp")
        return Syst::ElectronEnUp;
    else if (syst_str == "UEDown")
        return Syst::UEDown;
    else if (syst_str == "UEUp")
        return Syst::UEUp;
    else
    {
        cerr << "Unkonwn Syst." << endl;
        exit(1);
    }
}

const float threeb_cr_cut = 0.65;

#endif /* __Const_Def_H__ */
