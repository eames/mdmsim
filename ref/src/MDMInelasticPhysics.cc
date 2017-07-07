#include "MDMInelasticPhysics.hh"

#include "MDMInelasticProcess.hh"
#include "G4GenericIon.hh"
#include "globals.hh"
#include "G4PhysicsListHelper.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(MDMInelasticPhysics);//BinaryReactionPhysics

MDMInelasticPhysics::MDMInelasticPhysics(G4int)
:  G4VPhysicsConstructor("BinaryReactionPhysics") {
}

MDMInelasticPhysics::MDMInelasticPhysics(const G4String& name)
:  G4VPhysicsConstructor(name) {
}

MDMInelasticPhysics::~MDMInelasticPhysics()
{
}

void MDMInelasticPhysics::ConstructParticle()
{
  G4GenericIon::GenericIon();
}

void MDMInelasticPhysics::ConstructProcess()
{
  MDMInelasticProcess* reactionProcess = new MDMInelasticProcess();
  reactionProcess->ParseParams(fReactionParams);
  G4PhysicsListHelper::GetPhysicsListHelper()->
    RegisterProcess(reactionProcess, G4GenericIon::GenericIon());
}

