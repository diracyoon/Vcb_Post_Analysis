CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(Vcb_Post_Analysis)

MESSAGE(${CMAKE_PROJECT_NAME})

SET(PROJECT_DIR ${PROJECT_SOURCE_DIR}/..)

LIST(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
FIND_PACKAGE(ROOT COMPONENTS RooFitCore RooFit TMVA)

INCLUDE(${ROOT_USE_FILE})

INCLUDE_DIRECTORIES(${PROJECT_DIR}/Include)

SET(INCLUDE_DIR ${PROJECT_DIR}/Include)
SET(SRC_DIR ${PROJECT_DIR}/Src)

ROOT_GENERATE_DICTIONARY(G__Vcb_Post_Analysis Const_Def.h Samples.h W_Event.h Result_Event.h Permutation_TMVA.h Data_MC_Comparison.h Reco_Eval_Event.h Reco_Eval.h Average_Syst_PDF.h Template.h LINKDEF ${INCLUDE_DIR}/Vcb_Post_Analysis_LinkDef.h)

#ADD_COMPILE_OPTIONS(-std=c++14)

ADD_LIBRARY(Vcb_Post_Analysis SHARED ${SRC_DIR}/Samples.cpp ${SRC_DIR}/W_Event.cpp ${SRC_DIR}/Result_Event.cpp ${SRC_DIR}/Permutation_TMVA.cpp ${SRC_DIR}/Data_MC_Comparison.cpp ${SRC_DIR}/Reco_Eval_Event.cpp ${SRC_DIR}/Reco_Eval.cpp ${SRC_DIR}/Average_Syst_PDF.cpp ${SRC_DIR}/Template.cpp G__Vcb_Post_Analysis.cxx)

TARGET_LINK_LIBRARIES(Vcb_Post_Analysis ${ROOT_LIBRARIES})
