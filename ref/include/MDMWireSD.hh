#ifndef MDMWireSD_h
#define MDMWireSD_h

#include "G4VSensitiveDetector.hh"
#include "MDMWireHit.hh"

class MDMWireSD : public G4VSensitiveDetector {
public:
  MDMWireSD(G4String name);
  virtual ~MDMWireSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  
private:
  MDMWireHitsCollection* fHitsCollection;
  G4int fHCID;
};

#endif
