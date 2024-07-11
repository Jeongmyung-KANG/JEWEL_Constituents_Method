#include "TParticle.h"
#include "Pythia8/Pythia.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH1F.h"
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"

using namespace std;
using namespace Pythia8;

int main(){
    
    TFile * file = new TFile("Particle20_30.root","RECREATE");
    TTree * tree = new TTree("Particle","particle");

    Pythia pythia;
    pythia.readString("Beams:idA = 2212"); 
    pythia.readString("Beams:idB = 2212");
    pythia.readString("Beams:eCM = 5020."); 
    pythia.readString("HardQCD:all = on"); 
    pythia.readString("PhaseSpace:pTHatMin = 20"); 
    pythia.readString("PhaseSpace:pTHatMax = 30"); 

    pythia.init(); //특정 정보를 읽는다. 인수가 없을 때는 모든 정보

    Int_t nev = 10000;
    TClonesArray* carray = new TClonesArray("TParticle"); 

    tree -> Branch("carray", &carray);

    Double_t mass;
    //Int_t mother;
    //Int_t pdg;

    Double_t px;
    Double_t py;
    Double_t pz;
    Double_t e;

    Int_t Mother;
    Int_t Daughter;
    Int_t Pdg;
    Int_t Statuscode;
    
    for (Int_t iEvent = 0; iEvent < nev; iEvent++) {
        if (!pythia.next()){
            continue;
        }
        for (Int_t itrack = 0 ; itrack < pythia.event.size(); itrack++){
            TParticle* track = new((*carray)[itrack]) TParticle();
            mass = pythia.event[itrack].m();
            px = pythia.event[itrack].px();
            py = pythia.event[itrack].py(); 
            pz = pythia.event[itrack].pz();
            e = pythia.event[itrack].e();
	        //Double_t pt = pythia.event[itrack].pT();
            Mother = pythia.event[itrack].mother1(); 
            Daughter = pythia.event[itrack].daughter1();
	        Pdg = pythia.event[itrack].id();
            Statuscode = pythia.event[itrack].status();

            /*
            if(Daughter == 0){
                cout <<"Evemt : " << iEvent << " index " << itrack << " pdg code : " << Pdg << " status code : " << Statuscode << 
                " pt : " << pt << " px " << px << " py : " << py << " pz : " << pz << 
                " daughter : "<< Daughter << " mother : " << Mother << endl; 
            }
            */
	        
           
            track->SetCalcMass(mass);
            track->SetMomentum(px,py,pz,e);
            track->SetMother(0,Mother);
            track->SetDaughter(0,Daughter);
            track->SetPdgCode(Pdg);
            track->SetStatusCode(Statuscode);
            //track->SetProductionVertex(vx,vy,vz,vt);
	        //track->SetPolarisation(PolarTheta,PolarPhi);
        }
        tree ->Fill(); 
    }
    
    file -> cd();
    tree -> Write();
    file -> Close();

    return 0;
}
///home/hyejilee/5jet/makeParticle.cc
//g++ makeParticle.cc -o makeparticle.exe `root-config --cflags` -I/home/hyejilee/pythia8310/include -O2 -std=c++17 -pedantic -W -Wall -Wshadow -fPIC -pthread  -L/home/hyejilee/pythia8310/lib -Wl,-rpath,/home/hyejilee/pythia8310/lib -lpythia8 -ldl `root-config --evelibs`