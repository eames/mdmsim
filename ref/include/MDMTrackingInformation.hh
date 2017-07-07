#ifndef MDMTrackingInformation_h
#define MDMTrackingInformation_h

#include "G4VUserTrackInformation.hh"

class MDMTrackingInformation : public G4VUserTrackInformation {

public:
  MDMTrackingInformation(G4double,G4double,G4double,G4double);
  ~MDMTrackingInformation();

  void Print() const;

  void SetCMEnergy(G4double val) {fCMEnergy =val;};
  void SetCMLightTheta(G4double val) {fCMLightTheta =val;};
  void SetLabLightTheta(G4double val) {fLabLightTheta =val;};
  void SetCMLightPhi(G4double val) {fCMLightPhi =val;};
  
  G4double GetCMEnergy() const {return fCMEnergy;};
  G4double GetCMLightTheta() const {return fCMLightTheta;};
  G4double GetLabLightTheta() const {return fLabLightTheta;};
  G4double GetCMLightPhi() const {return fCMLightPhi;};

private:
  G4double fCMEnergy;
  G4double fCMLightTheta;
  G4double fLabLightTheta;
  G4double fCMLightPhi;
};

#endif
