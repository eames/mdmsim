#include <stdio.h>
#include <iostream>
#include <fstream>
#include "MDMTrace.h"
#include "json/json.h"

int main(int argc, char* argv[]) {

  if(argc<2) {
    std::cout << "Usage: mdmTrace config-file" << std::endl;
    return 0;
  }

  Json::Value config;
  std::string configFileName = argv[1];
  std::ifstream configStream(configFileName.c_str());
  configStream >> config;
  configStream.close();

  //Get Global Instance
  MDMTrace* mdm = MDMTrace::Instance();

  std::vector<double> scatteredAngles;
  bool useKinematics=false;
  for(Json::Value::iterator it = config.begin();it!=config.end();it++) {
    if(it.key().asString() == "mdmAngle") {
      mdm->SetMDMAngle(it->asDouble());
      printf("SET: %20s -- %.3f\n","MDM Angle",mdm->GetMDMAngle());
    } else if(it.key().asString() == "mdmDipoleField") {
      mdm->SetMDMDipoleField(it->asDouble());
      printf("SET: %20s -- %.3f\n","MDM Dipole Field",mdm->GetMDMDipoleField());
    } else if(it.key().asString() == "targetMass") {
      mdm->SetTargetMass(it->asDouble());
      printf("SET: %20s -- %.3f\n","Target Mass",mdm->GetTargetMass());
    } else if(it.key().asString() == "projectileMass") {
      mdm->SetProjectileMass(it->asDouble());
      printf("SET: %20s -- %.3f\n","Projectile Mass",mdm->GetProjectileMass());
    } else if(it.key().asString() == "scatteredMass") {
      mdm->SetScatteredMass(it->asDouble());
      printf("SET: %20s -- %.3f\n","Scattered Mass",mdm->GetScatteredMass());
    } else if(it.key().asString() == "scatteredCharge") {
      mdm->SetScatteredCharge(it->asDouble());
      printf("SET: %20s -- %.3f\n","Scattered Charge",mdm->GetScatteredCharge());
    } else if(it.key().asString() == "beamEnergy") {
      mdm->SetBeamEnergy(it->asDouble());
      printf("SET: %20s -- %.3f\n","Beam Energy",mdm->GetBeamEnergy());
    } else if(it.key().asString() == "qValue") {
      mdm->SetQValue(it->asDouble());
      printf("SET: %20s -- %.3f\n","Q Value",mdm->GetQValue());
    } else if(it.key().asString() == "excitationEnergy") {
      mdm->SetResidualEnergy(it->asDouble());
      printf("SET: %20s -- %.3f\n","Excitation Energy",mdm->GetResidualEnergy());
    } else if(it.key().asString() == "scatteredAngles") {
      for(unsigned int i = 0;i<it->size();i++) {
	scatteredAngles.push_back((*it)[i].asDouble());
      }
      printf("SET: %20s -- ","Scattered Angles");
      for(unsigned int i = 0;i<scatteredAngles.size();i++) {
	printf("%.3f ",scatteredAngles[i]);
      }
      printf("\n");
    } else if(it.key().asString() == "scatteredEnergy") {
      mdm->SetScatteredEnergy(it->asDouble());
      printf("SET: %20s -- %.3f\n","Scattered Energy",mdm->GetScatteredEnergy());
    } else if(it.key().asString() == "useKinematics") {
      useKinematics = it->asBool();
      if(useKinematics) {
	printf("Calling MDMTrace with kinematics...\n");
      } else {
	printf("Calling MDMTrace without kinematics...\n");
      }
    }
  } 

  printf("\nOxford Wire Positions\n");
  for(unsigned int i=0;i<scatteredAngles.size();i++) {
    mdm->SetScatteredAngle(scatteredAngles[i]);  //ALWAYS
    if(!useKinematics) mdm->SendRay();                            //WITHOUT KINEMATICS ONLY
    else mdm->SendRayWithKinematics();                //KINEMATICS ONLY
    double x1,x2,x3,x4;
    mdm->GetOxfordWirePositions(x1,x2,x3,x4);
    double energy = (useKinematics) ? mdm->GetEnergyAfterKinematics() : mdm->GetScatteredEnergy();
    printf("\tInitial Angle: %8.2f Energy: %8.2f 1: %8.3f 2: %8.3f 3: %8.3f 4: %8.3f\n",scatteredAngles[i],energy,x1,x2,x3,x4);
  }
  return 0;
}
