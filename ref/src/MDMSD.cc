#include "MDMSD.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

MDMSD::MDMSD(G4String name) :
  G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1) {
  G4String HCname = "Collection";
  collectionName.insert(HCname);
}

MDMSD::~MDMSD() {

}

void MDMSD::Initialize(G4HCofThisEvent* hce) {
    fHitsCollection = new MDMHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (fHCID<0) { 
      fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); 
    }
    hce->AddHitsCollection(fHCID,fHitsCollection);
}

G4bool MDMSD::ProcessHits(G4Step* step, G4TouchableHistory*) {

  G4String type = step->GetTrack()->GetDefinition()->GetParticleType();
  if(type!="nucleus"&&type!="baryon") return true;

  G4ThreeVector momentumDirection = step->GetTrack()->GetMomentumDirection();
  G4double xAngle = atan(momentumDirection[0]/momentumDirection[2])*180./3.14159;
  G4double yAngle = atan(momentumDirection[1]/momentumDirection[2])*180./3.14159;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();

  G4double edep = step->GetTotalEnergyDeposit();
  //if (edep==0.) return true;

  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(preStepPoint->GetTouchable());
  G4int copyNo = touchable->GetVolume()->GetCopyNo();
  G4double hitTime = preStepPoint->GetGlobalTime();
  
  G4ThreeVector prePosition = preStepPoint->GetPosition();
  G4ThreeVector translatedPosition = prePosition-preStepPoint->GetPhysicalVolume()->GetObjectTranslation();
  G4ThreeVector rotatedPosition = preStepPoint->GetPhysicalVolume()->GetObjectRotation()->inverse().operator()(translatedPosition);

  G4double kineticEnergy = step->GetTrack()->GetKineticEnergy();
  G4int hitMass = step->GetTrack()->GetDefinition()->GetAtomicMass();
  G4int hitCharge = step->GetTrack()->GetDefinition()->GetAtomicNumber();

  //std::cout <<xAngle<< "  "<<yAngle<<"    "<<kineticEnergy <<std::endl;
  
  MDMHit* hit = new MDMHit(copyNo,prePosition/cm,rotatedPosition/cm,kineticEnergy/MeV,hitTime/ns,edep/MeV,hitMass,hitCharge,xAngle,yAngle);
  fHitsCollection->insert(hit);

  return true;
}
