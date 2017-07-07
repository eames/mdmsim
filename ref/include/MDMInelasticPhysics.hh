#ifndef MDMInelasticPhysics_h
#define MDMInelasticPhysics_h

#include "G4VPhysicsConstructor.hh"
#include <string>
#include <map>

class MDMInelasticPhysics : public G4VPhysicsConstructor {
public:
  MDMInelasticPhysics(G4int verbose =1);
  MDMInelasticPhysics(const G4String& name);
  virtual ~MDMInelasticPhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

  void SetReactionParams(std::map<std::string,double> params) {
    fReactionParams=params;
  };

private:
  std::map<std::string,double> fReactionParams;
};

#endif
