#include "MDMTrackingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "MDMTrackingInformation.hh"
#include "MDMAnalysis.hh"
#include "MDMInelasticProcess.hh"

MDMTrackingAction::MDMTrackingAction() {
  fName = "BinaryReaction";
}

MDMTrackingAction::~MDMTrackingAction() {
}

void MDMTrackingAction::PreUserTrackingAction(const G4Track* track) {
  const G4VProcess* creatorProcess = track->GetCreatorProcess();
  if(!creatorProcess) return;

  if(creatorProcess->GetProcessName()!=fName) return;
  
  MDMInelasticProcess* reactionProcess = (MDMInelasticProcess*) creatorProcess;

  if(track->GetParticleDefinition()->GetAtomicMass() != 2 ||
     track->GetParticleDefinition()->GetAtomicNumber() != 1) return;
}
