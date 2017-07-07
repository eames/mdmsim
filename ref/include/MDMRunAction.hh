#ifndef MDMRunAction_h
#define MDMRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class MDMPrimaryGeneratorAction;
class MDMDetectorConstruction;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume 
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class MDMRunAction : public G4UserRunAction
{
  public:
    MDMRunAction(MDMDetectorConstruction*, MDMPrimaryGeneratorAction*);
    virtual ~MDMRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void SetIsChamber(bool isChamber) {
      fIsChamber = isChamber;
    };

  private:
    bool fIsChamber;
    MDMDetectorConstruction* fDetector;
    MDMPrimaryGeneratorAction* fPrimary;
};

#endif

