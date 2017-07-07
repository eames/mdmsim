#include "MDMSteppingAction.hh"
#include "MDMEventAction.hh"
#include "MDMDetectorConstruction.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

MDMSteppingAction::MDMSteppingAction(MDMEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  W1(0),W2(0),W3(0),W4(0)
{}

MDMSteppingAction::~MDMSteppingAction()
{}

void MDMSteppingAction::UserSteppingAction(const G4Step* step)
{
  if(!fIsChamber) {
      const MDMDetectorConstruction* detectorConstruction
	= static_cast<const MDMDetectorConstruction*>
	(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

      //Get Scoring Volumes 
      W1 = detectorConstruction->GetW1();
      W2 = detectorConstruction->GetW2();
      W3 = detectorConstruction->GetW3();
      W4 = detectorConstruction->GetW4();   

    if(step->GetTrack()->GetDefinition()->GetAtomicMass() == 2 && step->GetTrack()->GetDefinition()->GetAtomicNumber() == 1) {

    }
    
    if(!step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()) return;

    G4LogicalVolume* preVolume 
      = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
    G4LogicalVolume* postVolume 
      = step->GetPostStepPoint()->GetTouchableHandle()
       ->GetVolume()->GetLogicalVolume();   

    ///ORIGINAL PRE-STEPPOINT
    G4ThreeVector world_pos = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector local_pos = step->GetPreStepPoint()->GetTouchableHandle()->GetHistory()->GetTopTransform().TransformPoint(world_pos);

    if(step->GetTrack()->GetDefinition()->GetAtomicMass() == 2 && step->GetTrack()->GetDefinition()->GetAtomicNumber() == 1) {
      if (preVolume==W1 && postVolume!=W1) {
   	fEventAction->SetW1Position(local_pos);}
      if (preVolume==W2 && postVolume!=W2) {
    	fEventAction->SetW2Position(local_pos);}
      if (preVolume==W3 && postVolume!=W3) {
    	fEventAction->SetW3Position(local_pos);}
      if (preVolume==W4 && postVolume!=W4) {
   	fEventAction->SetW4Position(local_pos);}
    }
  }
}




