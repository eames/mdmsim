#include "MDMInelasticProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "MDMDetectorConstruction.hh"
#include "MDMTrackingInformation.hh"
#include "G4IonTable.hh"
#include "G4Box.hh"

//#include "MDMTrace.h"

MDMInelasticProcess::MDMInelasticProcess(const G4String& processName)
  : G4VDiscreteProcess(processName,fHadronic), fScatteringLength(1e6) {
  SetProcessSubType(111);
  fQValue = 0.;
}

MDMInelasticProcess::~MDMInelasticProcess() {
}

G4double MDMInelasticProcess::GetMeanFreePath(const G4Track& aTrack,
					    G4double, G4ForceCondition* condition) {

  G4double length = aTrack.GetPosition()[2];
  //std::cout << "The length is   "<< length/mm << std::endl;
  
  G4double mfp = (length<fScatteringLength||aTrack.GetTrackID()>1) ? DBL_MAX : 0.;

  *condition = NotForced;
  return mfp;
}

G4VParticleChange* MDMInelasticProcess::PostStepDoIt( const G4Track& aTrack,
						    const G4Step& aStep) {
  
  G4StepPoint* postStepPoint = aStep.GetPostStepPoint();
  if (postStepPoint->GetStepStatus()==fGeomBoundary) {
    return G4VDiscreteProcess::PostStepDoIt(aTrack,aStep);
  }

  aParticleChange.Initialize(aTrack);

  G4double energy = aTrack.GetKineticEnergy();

  //G4double rand_num = G4UniformRand()*1.0;
  G4double state1 = 11.2;
  G4double tQValue = 0.0;
/*
  if(rand_num>0.0 && rand_num <=1.0)
    tQValue = fQValue - state1;
     
  else
    tQValue = fQValue;
*/

  tQValue = fQValue - state1;

  G4double totalEnergy = energy+tQValue;
  G4ParticleDefinition* projectile = aTrack.GetDefinition();
  G4double projectileMass = projectile->GetAtomicMass();
  
  G4double B = projectileMass*fLightProductMass/(projectileMass+fTargetMass)/(fLightProductMass+fHeavyProductMass)*(energy/totalEnergy);
  G4double D = fTargetMass*fHeavyProductMass/(projectileMass+fTargetMass)/(fLightProductMass+fHeavyProductMass)*(1.+projectileMass/fTargetMass*tQValue/totalEnergy);

  G4double maxAngle = (B<D) ? 3.14159 : asin(sqrt(D/B));

  G4double arg = (1.-cos(maxAngle))*G4UniformRand()+cos(maxAngle);
  G4double pAngleLightLab = acos(arg);
  G4double aAngleLightLab = 2.*3.14159*G4UniformRand();

  //printf("B: %f D: %f\n",B,D);

  G4double lightEnergyLab = (B<=D) ? totalEnergy*B*pow(cos(pAngleLightLab)+sqrt(D/B-sin(pAngleLightLab)*sin(pAngleLightLab)),2.) :
    totalEnergy*B*pow(cos(pAngleLightLab)-sqrt(D/B-sin(pAngleLightLab)*sin(pAngleLightLab)),2.);
  G4double lightEnergyLab2 = (B<=D) ? totalEnergy*B*pow(cos(pAngleLightLab+0.001)+sqrt(D/B-sin(pAngleLightLab+0.001)*sin(pAngleLightLab+0.001)),2.) :
    totalEnergy*B*pow(cos(pAngleLightLab+0.001)-sqrt(D/B-sin(pAngleLightLab+0.001)*sin(pAngleLightLab+0.001)),2.);
  G4double heavyEnergyLab = totalEnergy-lightEnergyLab;

  G4double val = sqrt(lightEnergyLab/totalEnergy/D)*sin(pAngleLightLab);
  G4double val2 = sqrt(lightEnergyLab2/totalEnergy/D)*sin(pAngleLightLab+0.001);
  G4double pAngleLightCM = (val2>val) ? asin(val) : 3.14159-asin(val); 
  G4double aAngleLightCM = aAngleLightLab; 

  G4double pAngleHeavyLab = asin(sqrt(fLightProductMass/(fHeavyProductMass)*lightEnergyLab/heavyEnergyLab)*sin(pAngleLightLab));

  G4ThreeVector momentumDirection = aTrack.GetMomentumDirection();
  G4ThreeVector v = G4ThreeVector(0.,0.,1.).cross(momentumDirection);
  G4double rotAngle = acos(momentumDirection.z());

  G4ThreeVector lightLab(sin(pAngleLightLab)*cos(aAngleLightLab),
			 sin(pAngleLightLab)*sin(aAngleLightLab),
			 cos(pAngleLightLab));
  lightLab = lightLab.rotate(v,rotAngle);

  G4ThreeVector heavyLab(-1.*sin(pAngleHeavyLab)*cos(aAngleLightLab),
			 -1.*sin(pAngleHeavyLab)*sin(aAngleLightLab),
			 cos(pAngleHeavyLab));
  heavyLab = heavyLab.rotate(v,rotAngle);

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* light;
  if(particleTable->GetIonTable()->FindIon(fLightProductCharge,fLightProductMass,0.0)) 
    light = particleTable->GetIonTable()->FindIon(fLightProductCharge,fLightProductMass,0.0);
  else light = particleTable->GetIonTable()->GetIon(fLightProductCharge,fLightProductMass,0.0);

  G4ParticleDefinition* heavy;
  if(particleTable->GetIonTable()->FindIon(fHeavyProductCharge,fHeavyProductMass,0.0)) 
    heavy = particleTable->GetIonTable()->FindIon(fHeavyProductCharge,fHeavyProductMass,0.0);
  else heavy = particleTable->GetIonTable()->GetIon(fHeavyProductCharge,fHeavyProductMass,0.0);


  /*printf("IncomingZ: %d incomingM: %d recoilZ: %d recoilM: %d \n Lab Energy Incoming: %f Light Product Energy: %f Heavy Product Energy: %f \n Light Product Angle: %f Heavy Product Angle %f\n",projectile->GetAtomicNumber(),projectile->GetAtomicMass(),light->GetAtomicNumber(),light->GetAtomicMass(),energy,lightEnergyLab,heavyEnergyLab,pAngleLightLab,pAngleHeavyLab);*/

  //printf("%f\n",lightEnergyLab);

  G4Track* sec1 = new G4Track(new G4DynamicParticle(light,lightLab.unit(), lightEnergyLab*MeV),
			      aTrack.GetGlobalTime(),
			      aTrack.GetPosition());
 
  sec1->SetUserInformation(new MDMTrackingInformation(energy*fTargetMass/(projectileMass+fTargetMass),pAngleLightCM,pAngleLightLab, aAngleLightCM));
 
  G4Track* sec2 = new G4Track(new G4DynamicParticle(heavy, heavyLab.unit(), heavyEnergyLab*MeV),
			      aTrack.GetGlobalTime(),aTrack.GetPosition());

  aParticleChange.SetNumberOfSecondaries(2);
  aParticleChange.AddSecondary(sec1);
  aParticleChange.AddSecondary(sec2);

  aParticleChange.ProposeEnergy(0.);
  aParticleChange.ProposeTrackStatus(fStopAndKill);

  return &aParticleChange;
}

void MDMInelasticProcess::StartTracking(G4Track* track) {
  G4VProcess::StartTracking(track);	// Apply base class actions

  //Get boundary for inelastic scattering process
  const MDMDetectorConstruction* detectorConstruction
    = static_cast<const MDMDetectorConstruction*>
    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4LogicalVolume* Target = detectorConstruction->GetTargetVolume();
  G4double fTargetLength = (dynamic_cast<G4Box*>(Target->GetSolid()))->GetZHalfLength()*2.*detectorConstruction->GetNumLayers();

  fScatteringLength = fTargetLength*G4UniformRand(); //
  //std::cout << "Read Scattering Length   " << fTargetLength/mm << std::endl;
}

void MDMInelasticProcess::ParseParams(std::map<std::string,double> &params) {
    double lightProductMass = -1;
    double lightProductCharge = -1;
    double heavyProductMass = -1;
    double heavyProductCharge = -1;
    double targetMass = -1;
    double targetCharge = -1;

    for(std::map<std::string,double>::const_iterator it = params.begin();
	it!= params.end();it++) {
      if(it->first == "qValue"  ) {
	  SetQValue(it->second);
      } else if(it->first == "lightProductMass" ) {
	lightProductMass = it->second;
      } else if(it->first == "lightProductCharge" ) {
	lightProductCharge = it->second;
      } else if(it->first == "heavyProductMass" ) {
	heavyProductMass = it->second;
      } else if(it->first == "heavyProductCharge" ) {
	heavyProductCharge = it->second;
      } else if(it->first == "targetMass" ) {
	targetMass = it->second;
      } else if(it->first == "targetCharge" ) {
	targetCharge = it->second;
      } 
    }
    if(lightProductCharge>0 && lightProductMass>0) {
      SetLightProduct(lightProductCharge,lightProductMass);
    }
    if(heavyProductCharge>0 && heavyProductMass>0) {
      SetHeavyProduct(heavyProductCharge,heavyProductMass);
    }
    if(targetCharge>0 && targetMass>0) {
      SetTarget(targetCharge,targetMass);
    }
}


