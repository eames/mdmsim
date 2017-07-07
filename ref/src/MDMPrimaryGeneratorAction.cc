#include "MDMPrimaryGeneratorAction.hh"
#include "MDMAnalysis.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include <TLorentzVector.h>
#include "Randomize.hh"
#include "OxfordInputs.hh"

MDMPrimaryGeneratorAction::MDMPrimaryGeneratorAction(): 
    G4VUserPrimaryGeneratorAction() {

  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(22.0*MeV);

  fNumGenerated = 0;
}

MDMPrimaryGeneratorAction::~MDMPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void MDMPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(!fIsChamber) {
    fStageTree->LoadTree(fNumGenerated);
    fStageTree->fChain->GetEntry(fNumGenerated++);
    bool transmitted = fStageTree->transmitted;
    double energy = (transmitted) ? fStageTree->energy : 0;
    double posX = fStageTree->window_position_X;
    double posY = fStageTree->window_position_Y;
    double angleX = fStageTree->angle_X;
    double angleY = fStageTree->angle_Y;
    int charge = (transmitted) ? fStageTree->charge : 12;
    int mass = (transmitted) ? fStageTree->mass : 26;

    //if (charge==0 || mass==0) return;

    G4ParticleDefinition* ion;
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    if(particleTable->GetIonTable()->FindIon(charge,mass,0.0))
      ion = particleTable->GetIonTable()->FindIon(charge,mass,0.0);
    else ion = particleTable->GetIonTable()->GetIon(charge,mass,0.0);

    fParticleGun->SetParticleDefinition(ion);

  fParticleGun->SetParticlePosition(G4ThreeVector(posX*cm,posY*cm,0.));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(tan(angleX*3.14159/180.),tan(angleY*3.14159/180.),1).unit());
  fParticleGun->SetParticleEnergy(energy*MeV);

  }
  fParticleGun->GeneratePrimaryVertex(anEvent);
}


