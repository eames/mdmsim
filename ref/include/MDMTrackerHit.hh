#ifndef MDMTrackerHit_h
#define MDMTrackerHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class MDMTrackerHit : public G4VHit {
public:
    MDMTrackerHit(G4int,G4ThreeVector,G4ThreeVector,G4double,G4double);
    virtual ~MDMTrackerHit();

    G4int GetID() const {return fID;};
    G4ThreeVector GetPrePosition() const {return fPrePos;};
    G4ThreeVector GetPostPosition() const {return fPostPos;};
    G4double GetTime() const {return fTime;};
    G4double GetEnergy() const {return fEnergy;};

    inline void* operator new(size_t);
    inline void operator delete(void*);

private:
    G4int fID;
    G4ThreeVector fPrePos;
    G4ThreeVector fPostPos;
    G4double fEnergy;
    G4double fTime;
};

typedef G4THitsCollection<MDMTrackerHit> MDMTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<MDMTrackerHit>* MDMTrackerHitAllocator;

inline void* MDMTrackerHit::operator new(size_t)
{
      if (!MDMTrackerHitAllocator) MDMTrackerHitAllocator = new G4Allocator<MDMTrackerHit>;
          return (void*)MDMTrackerHitAllocator->MallocSingle();
}

inline void MDMTrackerHit::operator delete(void* aHit)
{
      MDMTrackerHitAllocator->FreeSingle((MDMTrackerHit*) aHit);
}

#endif
