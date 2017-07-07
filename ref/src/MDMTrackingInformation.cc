#include "MDMTrackingInformation.hh"

MDMTrackingInformation::MDMTrackingInformation(G4double cmEnergy, G4double cmLightTheta, G4double labLightTheta, G4double cmLightPhi) :
  fCMEnergy(cmEnergy), fCMLightTheta(cmLightTheta), fLabLightTheta(labLightTheta), fCMLightPhi(cmLightPhi) {
  }

MDMTrackingInformation::~MDMTrackingInformation() {

}

void MDMTrackingInformation::Print() const {

}
