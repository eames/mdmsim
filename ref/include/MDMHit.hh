#ifndef MDMHit_h
#define MDMHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

class MDMHit : public G4VHit {
public:
  MDMHit(G4int,G4ThreeVector,G4ThreeVector,G4double,G4double,G4double,G4int,G4int,G4double,G4double);
  virtual ~MDMHit();

  G4int GetID() const {return fID;};
  G4double GetTime() const {return fTime;};
  G4double GetEnergy() const {return fEnergy;};
  G4double GetEDep() const {return fEDep;};
  G4ThreeVector GetPosition() const {return fPos;};
  G4ThreeVector GetLocalPosition() const {return fPosLocal;};
  G4int GetHitCharge() const {return fHitCharge;};
  G4int GetHitMass() const {return fHitMass;};
  G4double GetXAngle() const {return fXAngle;};
  G4double GetYAngle() const {return fYAngle;};
  
  inline void* operator new(size_t);
  inline void operator delete(void*);

private:
  G4int fID;
  G4ThreeVector fPos;
  G4ThreeVector fPosLocal;
  G4double fEDep;
  G4double fEnergy;
  G4double fTime;
  G4int fHitMass;
  G4int fHitCharge;
  G4double fXAngle;
  G4double fYAngle;

};

typedef G4THitsCollection<MDMHit> MDMHitsCollection;

extern G4ThreadLocal G4Allocator<MDMHit>* MDMHitAllocator;

inline void* MDMHit::operator new(size_t)
{
    if (!MDMHitAllocator) MDMHitAllocator = new G4Allocator<MDMHit>;
    return (void*)MDMHitAllocator->MallocSingle();
}

inline void MDMHit::operator delete(void* aHit)
{
    MDMHitAllocator->FreeSingle((MDMHit*) aHit);
}

#endif
