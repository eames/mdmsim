#ifndef MDMEventAction_h
#define MDMEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <gsl/gsl_rng.h>
#include <string>
#include <map>

/// Event action class

class TRandom3;

class MDMEventAction : public G4UserEventAction
{
  public:
    MDMEventAction();
    virtual ~MDMEventAction();
    
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
    void ParseParams(const std::map<std::string,double>&);

    void SetAngle1(double,double);
    void SetEnergy1(G4double energy);
    void SetInEnergy1(G4double inenergy);
    void SetCharge1(G4int);
    void SetMass1(G4int);

    void SetIsChamber(bool isChamber) {
      fIsChamber = isChamber;
    };

    //void SetEScint(G4double);
    //void SetPosScint(G4ThreeVector);

    void SetW1Position(G4ThreeVector);
    void SetW2Position(G4ThreeVector);
    void SetW3Position(G4ThreeVector);
    void SetW4Position(G4ThreeVector);

  private:
    double fAngleX,fAngleY;
    G4double fEnergy;
    G4double fInEnergy;
    G4int fCharge;
    G4int fMass;

    bool fIsChamber;

    G4double fDriftVelocity1;
    G4double fDriftTimeRes1;
    G4double fDriftPosRes1;
    G4double fDriftVelocity2;
    G4double fDriftTimeRes2;
    G4double fDriftPosRes2;
    G4double fWorkFunction;
    G4double fFanoFactor;

    gsl_rng * r;
    TRandom3* fRandom3;

    G4double fEScint;
    G4double fdepEnergy;
    G4ThreeVector fPosScint;

    G4ThreeVector fPW1;
    G4ThreeVector fPW2;
    G4ThreeVector fPW3;
    G4ThreeVector fPW4;

    G4int fTrackerHCID[4][7];
    G4int fCsIHCID[8];
    G4int fWireHCID[4];
    G4int fSiHCID;
    G4int fEntranceHCID;
};

#endif

    
