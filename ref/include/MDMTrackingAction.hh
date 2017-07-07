#ifndef MDMTrackingAction_h
#define MDMTrackingAction_h

#include "G4String.hh"
#include "G4UserTrackingAction.hh"

class MDMTrackingAction : public G4UserTrackingAction {
public:
  MDMTrackingAction();
  ~MDMTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  
private:
  G4String fName;
};

#endif
