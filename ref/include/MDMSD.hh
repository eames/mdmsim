#ifndef MDMSD_h
#define MDMSD_h

#include "G4VSensitiveDetector.hh"
#include "MDMHit.hh"

class MDMSD : public G4VSensitiveDetector {
public:
  MDMSD(G4String name);
  virtual ~MDMSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
  
private:
  MDMHitsCollection* fHitsCollection;
  G4int fHCID;
};

#endif
