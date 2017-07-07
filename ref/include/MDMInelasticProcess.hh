#ifndef MDMInelasticProcess_h
#define MDMInelasticProcess_h

#include "G4VDiscreteProcess.hh"
//#include "MDMTrace.h"

//class MDMTrace;

class MDMInelasticProcess : public G4VDiscreteProcess {
public:
  MDMInelasticProcess(const G4String& name = "BinaryReaction");
  ~MDMInelasticProcess();

  G4double GetMeanFreePath(const G4Track&,G4double,
			   G4ForceCondition*);
  G4VParticleChange* PostStepDoIt(const G4Track&,const G4Step&);

  void StartTracking(G4Track*);
  void SetQValue(G4double qValue) {
    fQValue = qValue; 
    std::cout << "SET: Q Value -- " << fQValue << std::endl;
  };
  void SetTarget(G4double charge, G4double mass) {
    fTargetMass = mass;
    fTargetCharge = charge;
    std::cout << "SET: Target -- " << fTargetCharge << ' ' << fTargetMass << std::endl;
  };
  void SetLightProduct(G4double charge, G4double mass) {
    fLightProductMass = mass;
    fLightProductCharge = charge;
    std::cout << "SET: Light Product -- " << fLightProductCharge << ' ' << fLightProductMass << std::endl;
  };
  void SetHeavyProduct(G4double charge, G4double mass) {
    fHeavyProductMass = mass;
    fHeavyProductCharge = charge;
    std::cout << "SET: Heavy Product -- " << fHeavyProductCharge << ' ' << fHeavyProductMass << std::endl;
  };
  G4double GetLightProductMass() {
    return fLightProductMass;
  }
  G4double GetLightProductCharge() {
    return fLightProductCharge;
  }
  void ParseParams(std::map<std::string,double>&);

private:
  G4double fScatteringLength;
  G4double fQValue;
  G4double fTargetMass;
  G4double fTargetCharge;
  G4double fLightProductMass;
  G4double fLightProductCharge;
  G4double fHeavyProductMass;
  G4double fHeavyProductCharge;
};

#endif
