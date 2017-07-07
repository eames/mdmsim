#ifndef MDMSteppingAction_h
#define MDMSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class MDMEventAction;
class G4LogicalVolume;
class MDMTrace;

class MDMSteppingAction : public G4UserSteppingAction
{
  public:
    MDMSteppingAction(MDMEventAction* eventAction);
    virtual ~MDMSteppingAction();

    // method from the base class
    virtual void UserSteppingAction(const G4Step*);

    void SetIsChamber(bool isChamber) {
      fIsChamber = isChamber;
    };

  private:
    MDMEventAction*  fEventAction;

    bool fIsChamber;

    G4LogicalVolume* W1;
    G4LogicalVolume* W2;
    G4LogicalVolume* W3;
    G4LogicalVolume* W4;

    char fname[256];
};

#endif
