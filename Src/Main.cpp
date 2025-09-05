#include <iostream>
#include "Histo_Syst.h"

int main(int argc, char* argv[]) {
    if (argc < 7) {
        std::cout << "Usage: " << argv[0] << " <era> <channel> <mode> <index_tree_type> <last_index_tree_type> <tagger>" << std::endl;
        return 1;
    }

    TString era = argv[1];
    TString channel = argv[2];
    TString mode = argv[3];
    int index_tree_type = TString(argv[4]).Atoi();
    int last_index_tree_type = TString(argv[5]).Atoi();
    TString tagger = argv[6];

    std::cout << "Profiler executable started. Creating Histo_Syst object..." << std::endl;
   
    Histo_Syst* histo_syst = new Histo_Syst(era, channel, mode, index_tree_type, last_index_tree_type, tagger);

    delete histo_syst; 

    std::cout << "Profiler executable finished." << std::endl;

    return 0;
}
