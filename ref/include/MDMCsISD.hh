#ifndef MDMCsISD_h
#define MDMCsISD_h

#include "G4VSensitiveDetector.hh"
#include "MDMCsIHit.hh"

class MDMCsISD : public G4VSensitiveDetector {
public:
  MDMCsISD(G4String name);
  virtual ~MDMCsISD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  
private:
  MDMCsIHitsCollection* fHitsCollection;
  G4int fHCID;
};

#endif
