#include "MDMRunAction.hh"
#include "MDMAnalysis.hh"
//#include "MDMPrimaryGeneratorAction.hh"
//#include "MDMDetectorConstruction.hh"
//#include "MDMRun.hh"

#include "G4RunManager.hh"

MDMRunAction::MDMRunAction(MDMDetectorConstruction* detector, MDMPrimaryGeneratorAction* primary)
: G4UserRunAction(), fDetector(detector), fPrimary(primary) {
MDMAnalysis::Instance();
}

MDMRunAction::~MDMRunAction()
{
  delete MDMAnalysis::Instance();
}

void MDMRunAction::BeginOfRunAction(const G4Run*)
{ 
  MDMAnalysis::Instance()->OpenFile(fIsChamber);
}

void MDMRunAction::EndOfRunAction(const G4Run*)
{
  MDMAnalysis::Instance()->CloseFile();
}
