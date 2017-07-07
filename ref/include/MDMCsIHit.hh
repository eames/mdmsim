#ifndef MDMCsIHit_h
#define MDMCsIHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

class MDMCsIHit : public G4VHit {
public:
  MDMCsIHit(G4int, G4ThreeVector, G4double, G4double, G4int, G4int);
  virtual ~MDMCsIHit();

  G4int GetID() const {return fID;};
  G4double GetTime() const {return fTime;};
  G4double GetEnergy() const {return fEnergy;};
  G4ThreeVector GetPosition() const {return fPos;};
  G4int GetMass() const {return fMass;};
  G4int GetCharge() const {return fCharge;};

  inline void* operator new(size_t);
  inline void operator delete(void*);

private:
  G4int fID;
  G4ThreeVector fPos;
  G4double fEnergy;
  G4double fTime;
  G4int fMass;
  G4int fCharge;

};

typedef G4THitsCollection<MDMCsIHit> MDMCsIHitsCollection;

extern G4ThreadLocal G4Allocator<MDMCsIHit>* MDMCsIHitAllocator;

inline void* MDMCsIHit::operator new(size_t)
{
  if (!MDMCsIHitAllocator) MDMCsIHitAllocator = new G4Allocator<MDMCsIHit>;
  return (void*)MDMCsIHitAllocator->MallocSingle();
}

inline void MDMCsIHit::operator delete(void* aHit)
{
  MDMCsIHitAllocator->FreeSingle((MDMCsIHit*) aHit);
}

#endif
