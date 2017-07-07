#include "MDMWireHit.hh"

G4ThreadLocal G4Allocator<MDMWireHit>* MDMWireHitAllocator;

MDMWireHit::MDMWireHit(G4int ID, G4ThreeVector pos, G4ThreeVector posLocal, G4double energy, G4double time, G4double eDep, G4int hitMass, 
		   G4int hitCharge, G4double xAngle, G4double yAngle) :
  G4VHit(), fID(ID), fPos(pos), fPosLocal(posLocal), fEnergy(energy), fTime(time), fEDep(eDep),
  fHitMass(hitMass), fHitCharge(hitCharge), fXAngle(xAngle), fYAngle(yAngle) {

}

MDMWireHit::~MDMWireHit() {

}
