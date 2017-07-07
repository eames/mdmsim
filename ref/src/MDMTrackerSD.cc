#include "MDMTrackerSD.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

MDMTrackerSD::MDMTrackerSD(G4String name) :
      G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1) {
      G4String HCname = "MuOCollection";
      collectionName.insert(HCname);
      }

MDMTrackerSD::~MDMTrackerSD() {
}

void MDMTrackerSD::Initialize(G4HCofThisEvent* hce) {
      fHitsCollection = new MDMTrackerHitsCollection(SensitiveDetectorName,collectionName[0]);

      if (fHCID<0) { 
	fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); 
      }
      hce->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool MDMTrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*) {

  G4double edep = step->GetTotalEnergyDeposit();
  if (edep/eV<1.) return true;

  G4String type = step->GetTrack()->GetDefinition()->GetParticleType();
  if(type!="nucleus" && type!="baryon") return true;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4StepPoint* postStepPoint = step->GetPostStepPoint();

  G4int copyNo = preStepPoint->GetTouchableHandle()->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();

  G4ThreeVector prePosition = preStepPoint->GetPosition();
  G4ThreeVector postPosition = (postStepPoint) ? postStepPoint->GetPosition() : G4ThreeVector(0.,0.,0.);

  MDMTrackerHit* hit = new MDMTrackerHit(copyNo,prePosition/cm,postPosition/cm,edep/MeV,hitTime/ns);
  fHitsCollection->insert(hit);
 
  return true;

}
