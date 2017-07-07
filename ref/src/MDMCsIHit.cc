#include "MDMCsIHit.hh"

G4ThreadLocal G4Allocator<MDMCsIHit>* MDMCsIHitAllocator;

MDMCsIHit::MDMCsIHit(G4int ID, G4ThreeVector pos, G4double energy, G4double time, G4int mass, G4int charge) :
  G4VHit(), fID(ID), fPos(pos), fEnergy(energy), fTime(time), fMass(mass), fCharge(charge) {

}

MDMCsIHit::~MDMCsIHit() {

}
