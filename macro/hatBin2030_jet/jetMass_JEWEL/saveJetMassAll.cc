#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/Selector.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"

#include "TF1.h"
#include "TH1.h"
#include "TMath.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TClonesArray.h"
#include "TParticle.h"
#include "TDatabasePDG.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>

using namespace fastjet;
using namespace std;

int main(int argc,char** argv)
{
  if (argc != 3){
  	std::cerr << "Usage: " << argv[0] << "<input.root> <output.root>" <<std::endl;
	return 1;
  }
  
  const char * input_rootfilename = argv[1];
  const char * output_rootfilename = argv[2];
  
  TH1F* jetPT = new TH1F("jetPT","jetPT",400,0.0,200.0);
  TH1F* recojetPT = new TH1F("recojetPT","recojetPT",400,0.0,200.0);

  TH1F* jetMass510 = new TH1F("jetMass510","jetMass510",400,0.0,200.0);
  TH1F* jetMass1020 = new TH1F("jetMass1020","jetMass1020",400,0.0,200.0);
  TH1F* jetMass2040 = new TH1F("jetMass2040","jetMass2040",400,0.0,200.0);
  TH1F* jetMass4060 = new TH1F("jetMass4060","jetMass4060",400,0.0,200.0);
  TH1F* jetMass60100 = new TH1F("jetMass60100","jetMass60100",400,0.0,200.0);
  TH1F* jetMass100150 = new TH1F("jetMass100150","jetMass100150",400,0.0,200.0);

  Int_t jetNumber[6] ={0,0,0,0,0,0};

  jetPT->Sumw2();
  recojetPT->Sumw2();
  jetMass510->Sumw2();
  jetMass1020->Sumw2();
  jetMass2040->Sumw2();
  jetMass4060->Sumw2();
  jetMass60100->Sumw2();
  jetMass100150->Sumw2();

  // Input file and tree
  TFile* fin = TFile::Open(input_rootfilename);
  TTree* tin = (TTree*)fin->Get("events");

  TClonesArray *particles = new TClonesArray("TParticle");
  //tin->GetBranch("carray")->SetAutoDelete(kFALSE);
  //tin->SetBranchAddress("carray",&particles);

  Int_t nentries = tin->GetEntries();

  Int_t jetn = 0;
  Int_t recojetn = 0;

  // Event loop
  for(Int_t iev = 0; iev < nentries; iev++)
  {
    //pseudojet extract
    vector <PseudoJet> jetvec;

    particles->Clear();
    tin->GetEntry(iev); //마법의 코드 , particles에도 정보가 알아서 들어가짐.
    
    for(Int_t i=0; i<particles->GetEntriesFast(); i++)
    {
        TParticle* part = (TParticle*)particles->At(i);

        //statuscode 뭐냐? TMath::Abs(part->Eta()) < 2.0 && part->Pt() > 0.2
        //statuscode => The status code includes information on how a particle was produced
        Double_t pt = TMath::Sqrt(TMath::Power(part->Px() ,2) + TMath::Power(part->Py() ,2));

        if (part->GetFirstDaughter() == 0 && TMath::Abs(part->Eta()) < 2.4 && pt > 0.15){
          jetvec.push_back(PseudoJet(part->Px(),part->Py(),part->Pz(),part->Energy())); 
        }
    }

    //embedding bkg
    double_t stdDev = 5.0;
    double_t mean = 3200;

    TF1 *gaussianFunc = new TF1("gaussian", "gaus", mean - 5 * stdDev, mean + 5 * stdDev);
    gaussianFunc->SetParameters(1.0 / (stdDev * sqrt(2 * TMath::Pi())), mean, stdDev);

    TRandom random;
    Double_t Bg_pT, Bg_Px, Bg_Py, Bg_Pz, Bg_E, Bg_Eta, Bg_Phi, Bg_P;
    Double_t pion_mass = 0.13975; 

    Int_t multiplicity = gaussianFunc->GetRandom();

    TF1 *f1 = new TF1("f1", "x*exp(-2*x)", 0.15, 10);

    Double_t TrackEtaMax = 2.4;
    for(int i = 0; i < multiplicity; i++){  //Generate Thermal BackGround
      Bg_pT = f1->GetRandom();
      //Bg_Phi = random.Uniform(-TMath::Pi()*2, 2*TMath::Pi());
      Bg_Phi = random.Uniform(0, 2*TMath::Pi());
      Bg_Eta = random.Uniform(-TrackEtaMax, TrackEtaMax);

      Bg_Px = Bg_pT*TMath::Cos(Bg_Phi);
      Bg_Py = Bg_pT*TMath::Sin(Bg_Phi);
      Bg_P = Bg_pT*TMath::CosH(Bg_Eta);

      //Bg_Pz = TMath::Sqrt(Bg_P*Bg_P - Bg_Px*Bg_Px - Bg_Py*Bg_Py);
      Bg_Pz = Bg_pT*TMath::SinH(Bg_Eta);
      Bg_E = TMath::Sqrt(Bg_Px*Bg_Px + Bg_Py*Bg_Py + Bg_Pz*Bg_Pz + pion_mass*pion_mass);
      Double_t E_test = TMath::Sqrt(Bg_P*Bg_P + pion_mass*pion_mass); 

      jetvec.push_back(PseudoJet(Bg_Px, Bg_Py, Bg_Pz, Bg_E));
    }

    //making jet
    Double_t R = 0.4;
    JetDefinition jet_def(antikt_algorithm, R); //jet란 이런 것이다! 

    ClusterSequence cs(jetvec, jet_def); //jet를 clustering
    vector <PseudoJet> jets = sorted_by_pt(cs.inclusive_jets()); //허용된 최소한의 pt보다 큰 jet들만 //괄호에 minimum 값을 적으면 돼.

    for(Int_t i=0; i< jets.size(); i++){
	if(TMath::Abs(jets[i].eta()) < R + 2.4){
	   jetPT->Fill(jets[i].pt());
	   if(5 < jets[i].pt() && jets[i].pt() < 10){
		jetNumber[0]++;
                jetMass510->Fill(jets[i].m());

           }
	   if(10 < jets[i].pt() && jets[i].pt() < 20){
                jetNumber[1]++;
		jetMass1020->Fill(jets[i].m());

           }
	   if(20 < jets[i].pt() && jets[i].pt() < 40){
		jetNumber[2]++;
		jetMass2040->Fill(jets[i].m());

           }
	   if(40 < jets[i].pt() && jets[i].pt() < 60){
                jetNumber[3]++;
		jetMass4060->Fill(jets[i].m());

           }
	   if(60 < jets[i].pt() && jets[i].pt() < 100){
                jetNumber[4]++;
		jetMass60100->Fill(jets[i].m());

           }
	   if(100 < jets[i].pt() && jets[i].pt() < 150){
		jetNumber[5]++;
		jetMass100150->Fill(jets[i].m());
	
	   }
	}
    }
    //jet number
    jetn += jets.size();
    
    //Area-based subtraction
    Double_t ghost_maxrap = 6.0;
    GhostedAreaSpec area_spec(ghost_maxrap);
    AreaDefinition area_def(active_area, area_spec);

    ClusterSequenceArea clust_seq_full(jets, jet_def, area_def);

    Double_t ptmin = 7.0;
    vector <PseudoJet> full_jets = sorted_by_pt(clust_seq_full.inclusive_jets(ptmin));

    //estimate the background
    Double_t R0 = 0.4;
    JetDefinition jet_def_bkgd(kt_algorithm, R0);
    AreaDefinition area_def_bkgd(active_area_explicit_ghosts , GhostedAreaSpec(ghost_maxrap));
    Selector selector = SelectorAbsRapMax(4.5) * (!SelectorNHardest(2));
    JetMedianBackgroundEstimator bkgd_estimator(selector, jet_def_bkgd, area_def_bkgd);

    bkgd_estimator.set_particles(jetvec);

    BackgroundEstimate bkgd_estimate = bkgd_estimator.estimate();
    
    for (Int_t i=0; i<full_jets.size(); i++){
        recojetPT-> Fill(full_jets[i].pt() - bkgd_estimate.rho() * full_jets[i].area());
    }

    //recojet number
    recojetn += full_jets.size();

  }
  
  TFile* fout = TFile::Open(output_rootfilename, "RECREATE");

  jetPT->Scale(1.0/jetn);
 
  recojetPT->Scale(1.0/recojetn);
  
  jetMass510->Scale(1.0/jetNumber[0]);
  
  jetMass1020->Scale(1.0/jetNumber[1]);
  
  jetMass2040->Scale(1.0/jetNumber[2]);
 
  jetMass4060->Scale(1.0/jetNumber[3]);
  
  jetMass60100->Scale(1.0/jetNumber[4]);
    
  jetMass100150->Scale(1.0/jetNumber[5]);
 
  fout->cd();
  
  jetPT->Write();
  recojetPT->Write();
  jetMass510->Write();
  jetMass1020->Write();
  jetMass2040->Write();
  jetMass4060->Write();
  jetMass60100->Write();
  jetMass100150->Write();

  fout->Close();  
}