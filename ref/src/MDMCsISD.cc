#include "MDMCsISD.hh"
#include "G4SDManager.hh"

MDMCsISD::MDMCsISD(G4String name) :
  G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1) {
  G4String HCname = "CsICollection";
  collectionName.insert(HCname);
}

MDMCsISD::~MDMCsISD() {

}

void MDMCsISD::Initialize(G4HCofThisEvent* hce) {
  fHitsCollection = new MDMCsIHitsCollection(SensitiveDetectorName, collectionName[0]);
  if (fHCID<0) {
    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);
  }
  hce->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool MDMCsISD::ProcessHits(G4Step* step, G4TouchableHistory*) {
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  if(preStepPoint->GetStepStatus()==fGeomBoundary) {

  G4TouchableHistory* touchable = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = touchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();
  G4ThreeVector position = preStepPoint->GetPosition();
  G4int mass = step->GetTrack()->GetDefinition()->GetAtomicMass();
  G4int charge = step->GetTrack()->GetDefinition()->GetAtomicNumber();

  MDMCsIHit* hit = new MDMCsIHit(copyNo,position,edep,hitTime,mass,charge);
  fHitsCollection->insert(hit);
  }
  return true;
}
