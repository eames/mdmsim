#ifndef MDMPrimaryGeneratorAction_h
#define MDMPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class ParticleDefinition;
class OxfordInputs;

class MDMPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MDMPrimaryGeneratorAction();    
    virtual ~MDMPrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);         
  
    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    void SetIsChamber(bool isChamber) {
      fIsChamber = isChamber;
    };

    void SetOxfordInputs(OxfordInputs* tree) {
      fStageTree = tree;
    }
  
  private:
    G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
    bool fIsChamber;

    OxfordInputs* fStageTree;
    int fNumGenerated;
};

#endif


