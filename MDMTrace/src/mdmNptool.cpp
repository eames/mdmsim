//code from mdm Example
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "json/json.h"
#include "MDMTrace.h"
#include "Rayin.h"


//code from the nptool example
#include <string>
#include <TTree.h>
#include <TFile.h>
#include <TInitialConditions.h>
#include <TTiaraHyballData.h>


//code for the root file
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TRandom.h"

//other
#include "progress_bar.h"
#include "elements.h"

// Example program to propagate a defined ray through
// the MDM.
//
// Reads inputs from a json config file. See the example
// config-mdmExample.json 
//
int main(int argc, char* argv[]) 
{

  // set simulation file name (use JSON config in real program!!)

  // Check for correct arguments
  if(argc< 3) 
    {
      std::cout << "Usage: mdmNptool <config-file> <simulation ROOT file> [-o <output ROOT file>]"
		<< std::endl;
      return 0;
    }
  Json::Value config;
  std::string configFileName = argv[1];
  std::ifstream configStream(configFileName.c_str());
  configStream >> config;
  configStream.close();

  std::string nptoolFile = argv[2];
  std::string outputFile = argc > 3 ? argv[4] : "mdmResults.root";
	
  // Create link to rayin.dat file
  // NOTE:: Must come before the first call to
  // MDMTrace::Instance()
  Json::Value::iterator itRay = config.begin();
  while(itRay != config.end()) 
    {
      if (itRay.key().asString() == "rayinFile")
	{ // RAYTRACE FILE
	  break;
	}
      ++itRay;
    }
  if(itRay == config.end()) 
    {
      std::cerr << "ERROR: \"rayinFile\" not set in \"" << argv[1] << "\"\n";
      exit(1);
    }
  Rayin rayin_(itRay->asString());

	
  //Get Global MDM Instance
  MDMTrace* mdm = MDMTrace::Instance();

  //Read from config file
  for(Json::Value::iterator it = config.begin();it!=config.end();it++) 
    {
      if (false) 
	{ // DUMMY - AESTHETICS ONLY
	}
      else if (it.key().asString() == "mdmAngle") 
	{       // MDM ANGLE [mrad]
	  mdm->SetMDMAngle(it->asDouble());
	  printf("SET: %20s -- %.3f\n","MDM Angle",mdm->GetMDMAngle());
	}
      else if (it.key().asString() == "mdmDipoleField") 
	{ // MDM FIELD [kG cm]
	  mdm->SetMDMDipoleField(it->asDouble());
	  printf("SET: %20s -- %.3f\n","MDM Dipole Field",mdm->GetMDMDipoleField());
	}
      else if (it.key().asString() == "scatteredMass") 
	{  // ION MASS [AMU]
	  mdm->SetScatteredMass(it->asDouble());
	  printf("SET: %20s -- %.3f\n","Scattered Mass",mdm->GetScatteredMass());
	}
      else if(it.key().asString() == "scatteredCharge") 
	{ // ION CHARGE STATE [e+ charge]
	  mdm->SetScatteredCharge(it->asDouble());
	  printf("SET: %20s -- %.3f\n","Scattered Charge",mdm->GetScatteredCharge());
	}
      else if(it.key().asString() == "scatteredEnergy") 
	{ // ION ENERGY [MeV]
	  mdm->SetScatteredEnergy(it->asDouble());
	  printf("SET: %20s -- %.3f\n","Scattered Energy",mdm->GetScatteredEnergy());
	}
    }

  // figure out name of scattered particle //
  std::string scatteredParticle;
  {
    std::stringstream ss;
    try {
      ss << get_element_vector().at(mdm->GetScatteredCharge()) << round(mdm->GetScatteredMass());
    } catch(std::exception& e) {
      std::cerr << "ERROR: Invalid charge " << mdm->GetScatteredCharge() << "\n";
      exit(1);
    }
    scatteredParticle = ss.str();
    printf("SET: %20s -- %s\n","Scattered Particle", scatteredParticle.c_str());
  }	

  TFile* fileSim = TFile::Open(nptoolFile.c_str()); // Open simulation ROOT file
  if(!fileSim)
    { // Check that simulation file is valid
      std::cerr << "ERROR:: Bad nptool filename: \"" << nptoolFile << "\"\n";
      exit(1); // quit the program
    }
  TTree* SimulatedTree = dynamic_cast<TTree*>(fileSim->Get("SimulatedTree")); // Get pointer to simulation TTree
  if(!SimulatedTree)
    { // Check that the Simulation TTree exists in the file
      std::cerr << "ERROR:: No \"SimulatedTree\" in file.\n";
      exit(1); // quit the program
    }

  // Create TInitialConditions class object, to access the simulated Heavy-Ion data
  TInitialConditions* ic = new TInitialConditions();
  TTiaraHyballData* hyball = new TTiaraHyballData();
  // Associate the TInitialConditions instance with the branch in SimulatedTree
  SimulatedTree->SetBranchAddress("InitialConditions", &ic);
  SimulatedTree->SetBranchAddress("TiaraHyball", &hyball);
  // The heavy recoil for your simulation (set from JSON config)
  //	std::string scatteredParticle = "F20";

  //creating a file where we save our results
  //ofstream myfile("results.txt");
  //myfile << "K. Energy(MeV)\t In. Angle(rad)\t W1Angle(mrad)\t 1 (cm)\t 2 (cm)\t 3 (cm)\t 4 (cm)\n";

  //Create a new Tfile, TTree and TBranches
  TFile *file1 = new TFile(outputFile.c_str(), "RECREATE");
  TTree *tree = new TTree("mdm_results","Results");
	
  double wire1_xPos;
  double wire2_xPos;
  double wire3_xPos;
  double wire4_xPos;
  double energy;
  double incoming_angle_x;
  double incoming_angle_y;
  double outcoming_angle;

  double wire1_yPos;
  double wire2_yPos;
  double wire3_yPos;
  double wire4_yPos;
  double outcoming_angle_x;
  double outcoming_angle_y;

  double posx,posy,posz;
	
  vector<float> wire_xPos;
  vector<float> wire_zPos = {2,17.1,33.4,49.7};


  tree->Branch("wire1_xPos", &wire1_xPos,"wire1_xPos/D");
  tree->Branch("wire2_xPos", &wire2_xPos,"wire2_xPos/D");
  tree->Branch("wire3_xPos", &wire3_xPos,"wire3_xPos/D");
  tree->Branch("wire4_xPos", &wire4_xPos,"wire4_xPos/D");
  tree->Branch("outcoming_angle_x", &outcoming_angle_x,"outcoming_angle_x/D");

  tree->Branch("wire1_yPos", &wire1_yPos,"wire1_yPos/D");
  tree->Branch("wire2_yPos", &wire2_yPos,"wire2_yPos/D");
  tree->Branch("wire3_yPos", &wire3_yPos,"wire3_yPos/D");
  tree->Branch("wire4_yPos", &wire4_yPos,"wire4_yPos/D");
  tree->Branch("outcoming_angle_y", &outcoming_angle_y,"outcoming_angle_y/D");

  tree->Branch("energy", &energy,"energy/D");
  tree->Branch("incoming_angle_x", &incoming_angle_x,"incoming_angle_x/D");
  tree->Branch("incoming_angle_y", &incoming_angle_y,"incoming_angle_y/D");
  tree->Branch("outcoming_angle", &outcoming_angle,"outcoming_angle/D");

  tree->Branch("incoming_pos_x", &posx, "incoming_pos_x/D");
  tree->Branch("incoming_pos_y", &posy, "incoming_pos_y/D");
  tree->Branch("incoming_pos_z", &posz, "incoming_pos_z/D");
	
  tree->Branch("wire_xPos", &wire_xPos);
  tree->Branch("wire_zPos", &wire_zPos);

  // Loop Over the Input Tree
  for(Long64_t i=0; i< SimulatedTree->GetEntries(); ++i)
    {
      SimulatedTree->GetEntry(i); // Retreive the data from the 'ith' event

      // Now use the member functions of the TInitialConditions class to get
      // the heavy-ion data for this event
      //
      int mult = ic->GetParticleMultiplicity(); // number of emitted particles in the simulation

      // Search for the particle index matching the heavy-ion particle name
      int which = 0;
      while(1)
	{
	  std::string name = ic->GetParticleName(which); // will be something like: "20F[1000]"
	  name = name.substr(0, name.find("[")); // remove excitation energy part

	  if(name == scatteredParticle) break; // success - we found it
	  if(++which == mult) break; // failure - searched all particles & not found
	}
      if(which == mult)
	{  // didn't find the particle in this event, skip
	  std::cerr << "Skipping event " << i << "\n"; continue;
	}

      // if(hyball->GetRingEMult() != 1)
      // {
      // 	continue;
      // }
 
      // Found particle okay, now get the data you want (kinetic energy, angle)
      // using member functions of TInitialConditions
      //
      // FOR EXAMPLE:
      double ekin = ic->GetKineticEnergy(which);
      TVector3 direction  = ic-> GetParticleDirection(which);
      //atan gives the arctan expressed in radiants
      double angle_x = atan( direction.X() / direction.Z()); 
      double angle_y = atan( direction.Y() / direction.Z());
      double raytrace_angle_x = angle_x*180/3.14159; //degrees
      double raytrace_angle_y = angle_y*180/3.14159; //degrees
      posx = ic->GetIncidentPositionX();
      posy = ic->GetIncidentPositionY();
      posz = ic->GetIncidentPositionZ();
		
		
      //cout << raytrace_angle <<"\t";
      //double raytrace_angle = angle/1000; //(mrad)
		
      mdm->SetScatteredAngle(raytrace_angle_x,raytrace_angle_y); //angle must be in degrees!
      //		mdm->SetScatteredAngle(raytrace_angle_x); //angle must be in degrees!
      mdm->SetScatteredEnergy(ekin);
      mdm->SetBeamPosition(posx, posy, posz);
      mdm->SendRay();
      double x1,x2,x3,x4,a1;
      double y1,y2,y3,y4,b1;
      mdm->GetOxfordWirePositions(a1,x1,x2,x3,x4,b1,y1,y2,y3,y4);
  
      wire1_xPos = x1;
      wire2_xPos = x2;
      wire3_xPos = x3;
      wire4_xPos = x4;
      outcoming_angle = a1;
      outcoming_angle_x = a1;

      wire1_yPos = y1;
      wire2_yPos = y2;
      wire3_yPos = y3;
      wire4_yPos = y4;
      outcoming_angle_y = b1;


      incoming_angle_x = raytrace_angle_x;
      incoming_angle_y = raytrace_angle_y;
      energy = ekin;
		
      wire_xPos.clear();
      wire_xPos.push_back(x1);
      wire_xPos.push_back(x2);
      wire_xPos.push_back(x3);
      wire_xPos.push_back(x4);

      tree-> Fill();
	
      //myfile << ekin << "\t\t" << angle <<"\t"  << a1 <<"\t\t" << x1 << "\t" << x2 <<"\t" << x3 <<"\t" << x4 << "\n";
		

      //
      //		cout << "Found \"" << scatteredParticle << "\" at index " << which << ", kinetic energy is " << ekin << " MeV and angle theta is " << angle <<" rad.\n";
 
      //   		printf("Initial Angle: %8.2f\n W1Angle: %8.2f\t 1: %5.4f\t 2: %5.4f\t 3: %5.4f\t 4: %5.4f\n",angle,a1,x1,x2,x3,x4);

      progress_bar(i, SimulatedTree->GetEntries());
    }
  //		myfile.close();
  tree->Write();
  return 0;
  fileSim->Close(); // close input nptool file
  delete fileSim; // restore memory allocated to TFile
}
