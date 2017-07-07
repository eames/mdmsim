#include "MDMActionInitialization.hh"
#include "MDMPrimaryGeneratorAction.hh"
#include "MDMRunAction.hh"
#include "MDMEventAction.hh"
#include "MDMSteppingAction.hh"
#include "MDMTrackingAction.hh"
#include "MDMDetectorConstruction.hh"

MDMActionInitialization::MDMActionInitialization(MDMDetectorConstruction* detector)
 : G4VUserActionInitialization(), fDetector(detector)
{}

MDMActionInitialization::~MDMActionInitialization()
{}

void MDMActionInitialization::BuildForMaster() const
{
  SetUserAction(new MDMRunAction(fDetector,NULL));
}

void MDMActionInitialization::Build() const
{
  MDMPrimaryGeneratorAction* primary = new MDMPrimaryGeneratorAction();
  primary->SetIsChamber(fDetector->IsChamber());
  if(!fDetector->IsChamber()) primary->SetOxfordInputs(fStageTree);
  SetUserAction(primary);
  
  MDMRunAction* runAction = new MDMRunAction(fDetector,primary);
  runAction->SetIsChamber(fDetector->IsChamber());
  SetUserAction(runAction);
  
  MDMEventAction* eventAction = new MDMEventAction;
  eventAction->ParseParams(fEventActionParams);
  eventAction->SetIsChamber(fDetector->IsChamber());
  SetUserAction(eventAction);
  
  SetUserAction(new MDMTrackingAction);
 
  MDMSteppingAction* steppingAction = new MDMSteppingAction(eventAction);
  steppingAction->SetIsChamber(fDetector->IsChamber());
  SetUserAction(steppingAction);
}
