#include <iostream>
#include <vector>
#include <cmath>
#include "TFile.h"
#include "TTree.h"
#include "HepMC3/Print.h"
#include "HepMC3/GenEvent.h"
#include "HepMC3/Reader.h"
#include "HepMC3/ReaderAsciiHepMC2.h"
#include "HepMC3/WriterAsciiHepMC2.h"
#include "HepMC3/ReaderAscii.h"
#include "HepMC3/WriterAscii.h"
#include "HepMC3/WriterHEPEVT.h"
#include "HepMC3/WriterPlugin.h"
#include "HepMC3/ReaderHEPEVT.h"
#include "HepMC3/ReaderLHEF.h"
#include "HepMC3/ReaderPlugin.h"
#include "HepMC3/ReaderFactory.h"

#ifdef HEPMC3_ROOTIO
#include "HepMC3/ReaderRoot.h"
#include "HepMC3/WriterRoot.h"
#include "HepMC3/ReaderRootTree.h"
#include "HepMC3/WriterRootTree.h"
#endif

using namespace std;
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input.hepmc> <output.root>" << std::endl;
        return 1;
    }

    const char* hepmc_filename = argv[1];
    const char* root_filename = argv[2];

    // Create ROOT file and tree
    TFile * file = new TFile(root_filename,"RECREATE");
    TTree * tree = new TTree("events","events");
   
    // Variables to be stored in the tree
    int event_number;
    vector<int> particle_id;
    vector<float> particle_px;
    vector<float> particle_py;
    vector<float> particle_pz;
    vector<float> particle_pt;
    //vector<float> particle_mass;
    vector<float> particle_eta;
    vector<float> particle_phi;

    tree->Branch("event_number", &event_number);
    tree->Branch("particle_id", &particle_id);
    tree->Branch("particle_px", &particle_px);
    tree->Branch("particle_py", &particle_py);
    tree->Branch("particle_pz", &particle_pz);
    tree->Branch("particle_pt", &particle_pt);
    //tree->Branch("particle_mass", &particle_mass);
    tree->Branch("particle_eta", &particle_eta);
    tree->Branch("particle_phi", &particle_phi);

    // Open HepMC3 file
    HepMC3::ReaderAsciiHepMC2 hepmc_reader(hepmc_filename);

    HepMC3::GenEvent event;
    
    while (hepmc_reader.read_event(event)) {
        // Clear vectors for the new event
	if (hepmc_reader.failed()) {
		break; 
	}
        particle_id.clear();
	particle_px.clear();
	particle_py.clear();
	particle_pz.clear();
        particle_pt.clear();
	//particle_mass.clear();
        particle_eta.clear();
        particle_phi.clear();

        event_number = event.event_number();
	//cout << event_number <<endl;
        // Process particles
        for (const auto& particle : event.particles()){
            particle_id.push_back(particle->pid());
	    particle_px.push_back(particle->momentum().px());
	    particle_py.push_back(particle->momentum().py());
	    particle_pz.push_back(particle->momentum().pz());

            float pt = sqrt(pow(particle->momentum().px(), 2) + pow(particle->momentum().py(), 2));
           
	    particle_pt.push_back(pt);
	    //particle_mass.push_back(particle->mass());
            particle_eta.push_back(particle->momentum().eta());
            particle_phi.push_back(particle->momentum().phi());
        }

        // Fill tree
        tree->Fill();
    }

    // Save and close ROOT file
    file -> cd();

    tree -> Write();

    file -> Close();

    return 0;
}

