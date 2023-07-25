#ifndef __Const_Def_H__
#define __Const_Def_H__

//2016preVFP
const float bvsc_wp_l_2016preVFP = 0.0508;
const float bvsc_wp_m_2016preVFP = 0.2598;
const float bvsc_wp_t_2016preVFP = 0.6502;

const float cvsl_wp_l_2016preVFP = 0.039;
const float cvsl_wp_m_2016preVFP = 0.098;
const float cvsl_wp_t_2016preVFP = 0.270;

const float cvsb_wp_l_2016preVFP = 0.327;
const float cvsb_wp_m_2016preVFP = 0.370;
const float cvsb_wp_t_2016preVFP = 0.256;

//2016postVFP
const float bvsc_wp_l_2016postVFP = 0.0480;
const float bvsc_wp_m_2016postVFP = 0.2489;
const float bvsc_wp_t_2016postVFP = 0.6377;

const float cvsl_wp_l_2016postVFP = 0.039;
const float cvsl_wp_m_2016postVFP = 0.099;
const float cvsl_wp_t_2016postVFP = 0.269;

const float cvsb_wp_l_2016postVFP = 0.305;
const float cvsb_wp_m_2016postVFP = 0.353;
const float cvsb_wp_t_2016postVFP = 0.247;

//2017
const float bvsc_wp_l_2017 = 0.0532;
const float bvsc_wp_m_2017 = 0.3040;
const float bvsc_wp_t_2017 = 0.7476;

const float cvsl_wp_l_2017 = 0.030;
const float cvsl_wp_m_2017 = 0.085;
const float cvsl_wp_y_2017 = 0.520;

const float cvsb_wp_l_2017 = 0.400;
const float cvsb_wp_m_2017 = 0.340;
const float cvsb_wp_y_2017 = 0.050;

//2018
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

const float lumi_2016a = 1;
const float lumi_2016b = 1;
const float lumi_2017 = 41.53;
const float lumi_2018 = 59.74;

typedef enum Syst_
{
    Central,
    JetResUp,
    JetResDown,
    JetEnUp,
    JetEnDown,
    /* MuonEnUp, MuonEnDown,  */
    ElectronResUp,
    ElectronResDown,
    ElectronEnUp,
    ElectronEnDown,
    UnclusteredEnergyUp,
    UnclusteredEnergyDown,
    NSyst
} Syst;

#endif /* __Const_Def_H__ */ 
