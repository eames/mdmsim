#ifndef MDMTrackerSD_h
#define MDMTrackerSD_h

#include "G4VSensitiveDetector.hh"
#include "MDMTrackerHit.hh"

class MDMTrackerSD : public G4VSensitiveDetector {
public:
    MDMTrackerSD(G4String name);
    virtual ~MDMTrackerSD();
        
    virtual void Initialize(G4HCofThisEvent* HCE);
    virtual G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
	    
private:
    MDMTrackerHitsCollection* fHitsCollection;
    G4int fHCID;
};

#endif
