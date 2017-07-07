#include "MDMAnalysis.hh"
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include "G4SystemOfUnits.hh"

MDMAnalysis* MDMAnalysis::fInstance = NULL;

MDMAnalysis::MDMAnalysis() {  
}

MDMAnalysis* MDMAnalysis::Instance() {
  if(!fInstance) {
    fInstance =  new MDMAnalysis(); 
  } 
    return fInstance;
}

void MDMAnalysis::SetFilename(const char* filename) {
  fFilename=filename;
}

void MDMAnalysis::OpenFile(bool isChamber) {
  fFile = (isChamber) ? new TFile(fFilename.c_str(),"recreate") : 
    new TFile(Form("Final_%s",fFilename.c_str()),"recreate");

  fTree = new TTree("simData","Simulation Data");

  if(isChamber) {
    fTree->Branch("angle_X",&fangle[0],"angle_X/D");
    fTree->Branch("angle_Y",&fangle[1],"angle_Y/D");
    fTree->Branch("window_position_X",&fwposition[0],"window_position_X/D");
    fTree->Branch("window_position_Y",&fwposition[1],"window_position_Y/D");
    fTree->Branch("energy",&fenergy,"energy/D");
    fTree->Branch("wire_position_X",&fwirepos[0],"wire_position_X/D");
    fTree->Branch("wire_position_Y",&fwirepos[1],"wire_position_Y/D");
    fTree->Branch("charge",&fcharge,"charge/I");
    fTree->Branch("mass",&fmass,"mass/I");
    fTree->Branch("transmitted",&fTransmitted,"transmitted/O");
    fTree->Branch("accepted",&fAccepted,"accepted/O");
    fTree->Branch("siEnergy",&fSiEnergy,"siEnergy/D");
    fTree->Branch("siQuad",&fSiQuad,"siQuad/I");
  }
 
  else {
    /*fTree->Branch("Energy",&fEScint,"Energy/D");
    fTree->Branch("Scint_Position_X",&fPosScint[0],"Position_X/D");
    fTree->Branch("Scint_Position_Y",&fPosScint[1],"Position_Y/D");
    fTree->Branch("Scint_Position_Z",&fPosScint[2],"Position_Z/D");

    fTree->Branch("Edeposited",&fEdep,"Edeposited/D");*/

    fTree->Branch("W1_X",&fW1P[0],"W1_X/D");
    fTree->Branch("W1_Y",&fW1P[1],"W1_Y/D");
    fTree->Branch("W2_X",&fW2P[0],"W2_X/D");
    fTree->Branch("W3_X",&fW3P[0],"W3_X/D");
    fTree->Branch("W4_X",&fW4P[0],"W4_X/D");

    /*fTree->Branch("muo_mul",&muo_mul,"muo_mul/I");
    fTree->Branch("Row",fRow,"Row[muo_mul]/I");
    fTree->Branch("Column",fColumn,"Column[muo_mul]/I");
    fTree->Branch("MuOTime",fMuOTime,"MuOTime[muo_mul]/I");
    fTree->Branch("MuOEnergy",fMuOEnergy,"MuOEnergy[muo_mul]/D");*/

    fTree->Branch("CsIHitX",&fCsIHitX);
    fTree->Branch("CsIHitY",&fCsIHitY);
    fTree->Branch("CsIHitZ",&fCsIHitZ);
    fTree->Branch("CsIHitE",&fCsIHitE);
    fTree->Branch("CsIHitT",&fCsIHitT);
    fTree->Branch("CsIHitDet",&fCsIHitDet);
    fTree->Branch("CsIHitMass",&fCsIHitMass);
    fTree->Branch("CsIHitCharge",&fCsIHitCharge);
    
    fTree->Branch("WireHitX",&fWireHitX);
    fTree->Branch("WireHitE",&fWireHitE);
    fTree->Branch("WireHitNo",&fWireHitNo);
    fTree->Branch("WireHitMass",&fWireHitMass);
    fTree->Branch("WireHitCharge",&fWireHitCharge);

  }
}

void MDMAnalysis::CloseFile() {
  fTree->Write();
  fFile->Close();
}

void MDMAnalysis::SetAngle2(double angleX, double angleY) {
  fangle[0] = angleX;
  fangle[1] = angleY;
}

void MDMAnalysis::SetWindowDist(double windowX, double windowY) {
  fwposition[0] = windowX;
  fwposition[1] = windowY;
}

void MDMAnalysis::SetEnergy2(G4double eng) {
  fenergy = eng;
}

void MDMAnalysis::SetWirePos(double distX, double distY) {
  fwirepos[0] = distX;
  fwirepos[1] = distY;
}

void MDMAnalysis::SetCharge2(int charge) {
  fcharge = charge;
}

void MDMAnalysis::SetMass2(int mass) {
  fmass = mass;
}

void MDMAnalysis::SetAccepted(bool accepted) {
  fAccepted = accepted;
}

void MDMAnalysis::SetTransmitted(bool trans) {
  fTransmitted = trans;
}

void MDMAnalysis::SetSiEnergy(double energy) {
  fSiEnergy = energy;
}

void MDMAnalysis::SetSiQuad(int quad) {
  fSiQuad = quad;
}

void MDMAnalysis::Fill() {
  fTree->Fill();
}
///////////////////////////////////////////////////
/*void MDMAnalysis::SetEdep(double v){
  fEdep = v;
}*/

void MDMAnalysis::SetW1P(G4ThreeVector w1p) {
  fW1P[0] = w1p.x()/cm;
  fW1P[1] = w1p.y()/cm;
}

void MDMAnalysis::SetW2P(G4ThreeVector w2p) {
  fW2P[0] = w2p.x()/cm;
}

void MDMAnalysis::SetW3P(G4ThreeVector w3p) {
  fW3P[0] = w3p.x()/cm;
}

void MDMAnalysis::SetW4P(G4ThreeVector w4p) {
  fW4P[0] = w4p.x()/cm;
}

/*
void MDMAnalysis::SetPosScint(G4ThreeVector pos) {
  fPosScint[0] = pos.x()/cm;
  fPosScint[1] = pos.y()/cm;
  fPosScint[2] = pos.z()/cm;
}

void MDMAnalysis::SetEScint(double En) {
  fEScint = En;
}
*/
/*void MDMAnalysis::SetMuO(int row, int col,double time, double energy) {
  fRow = row;
  fColumn = col;
  fMuOTime = time;
  fMuOEnergy = energy;
}*/

void MDMAnalysis::SetCsIHitX(std::vector<double>& v) {
    fCsIHitX=v;
}

void MDMAnalysis::SetCsIHitY(std::vector<double>& v) {
    fCsIHitY=v;
}

void MDMAnalysis::SetCsIHitZ(std::vector<double>& v) {
    fCsIHitZ=v;
}

void MDMAnalysis::SetCsIHitE(std::vector<double>& v) {
    fCsIHitE=v;
}

void MDMAnalysis::SetCsIHitT(std::vector<double>& v) {
    fCsIHitT=v;
}
void MDMAnalysis::SetCsIHitDet(std::vector<int>& v) {
    fCsIHitDet=v;
}
void MDMAnalysis::SetCsIHitMass(std::vector<int>& v) {
    fCsIHitMass=v;
}
void MDMAnalysis::SetCsIHitCharge(std::vector<int>& v) {
    fCsIHitCharge=v;
}

void MDMAnalysis::SetWireHitX(std::vector<double>& v) {
    fWireHitX=v;
}

void MDMAnalysis::SetWireHitE(std::vector<double>& v) {
    fWireHitE=v;
}

void MDMAnalysis::SetWireHitNo(std::vector<int>& v) {
    fWireHitNo=v;
}
void MDMAnalysis::SetWireHitMass(std::vector<int>& v) {
    fWireHitMass=v;
}
void MDMAnalysis::SetWireHitCharge(std::vector<int>& v) {
    fWireHitCharge=v;
}

