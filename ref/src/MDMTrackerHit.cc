#include "MDMTrackerHit.hh"

G4ThreadLocal G4Allocator<MDMTrackerHit>* MDMTrackerHitAllocator;

MDMTrackerHit::MDMTrackerHit(G4int ID, G4ThreeVector prePos, G4ThreeVector postPos,G4double energy, G4double time) :
    G4VHit(), fID(ID), fPrePos(prePos), fPostPos(postPos), fEnergy(energy), fTime(time)
{}

MDMTrackerHit::~MDMTrackerHit() {
}
