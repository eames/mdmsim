#include "MDMDetectorConstruction.hh"
#include "MDMActionInitialization.hh"
#include "QGSP_BERT.hh"
#include "json/json.h"
#include "MDMInelasticPhysics.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "Randomize.hh"
#include "MDMAnalysis.hh"
#include "G4LogicalVolume.hh"

#include "Randomize.hh"
#include "time.h"
#include <fstream>
#include "../include/OxfordInputs.hh"

#include <TTree.h>
#include <TFile.h>
//#include <regex>

int main(int argc,char** argv)
{
  //CREATE AND READ JSON CONFIG
  Json::Value config;
  std::string configFileName = argv[1];
  std::ifstream configStream(configFileName.c_str());
  configStream >> config;
  configStream.close();

  //PARSE JSON
  G4int processNumber = config["processNumber"].asInt();
  G4String macroName = config["macroName"].asString();
  G4bool isInteractive = config["interactive"].asBool();
  std::map<std::string,double> reactionParams;
  reactionParams["qValue"] = config["qValue"].asDouble();
  reactionParams["lightProductCharge"] = config["lightProduct"][0].asDouble();
  reactionParams["lightProductMass"] = config["lightProduct"][1].asDouble();
  reactionParams["heavyProductCharge"] = config["heavyProduct"][0].asDouble();
  reactionParams["heavyProductMass"] = config["heavyProduct"][1].asDouble();
  reactionParams["targetCharge"] = config["target"][0].asDouble();
  reactionParams["targetMass"] = config["target"][1].asDouble();
  
  std::map<std::string,double> eventActionParams;
  eventActionParams["driftVelocity1"] = config["driftVelocity1"].asDouble();
  eventActionParams["driftTimeRes1"] = config["driftTimeRes1"].asDouble();
  eventActionParams["driftPosRes1"] = config["driftPosRes1"].asDouble();
  eventActionParams["driftVelocity2"] = config["driftVelocity2"].asDouble();
  eventActionParams["driftTimeRes2"] = config["driftTimeRes2"].asDouble();
  eventActionParams["driftPosRes2"] = config["driftPosRes2"].asDouble();
  eventActionParams["workFunction"] = config["workFunction"].asDouble();
  eventActionParams["fanoFactor"] = config["fanoFactor"].asDouble();

  bool IsChamber = config["IsChamber"].asBool();
  std::string stageFile = config["stageFile"].asString();

  // Choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  //set random seed with system time
  G4long seed = time(NULL);
  seed+=473879*processNumber;
  CLHEP::HepRandom::setTheSeed(seed);

  char filename[25];
  MDMAnalysis* analysis = MDMAnalysis::Instance();

  if(IsChamber) sprintf(filename,"MDM_%d.root",processNumber);
  else {
    //std::string runNum = stageFile.substr(stageFile.find_last_of('_')+1,stageFile.find_last_of('.')-stageFile.find_last_of('_')-1);
    //sprintf(filename,"MDM_%s.root",runNum.c_str());
    sprintf(filename,"MDM_%d.root",processNumber);
  }

  analysis->SetFilename(filename);

  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
#else
  G4RunManager* runManager = new G4RunManager;
#endif

  // Set mandatory initialization classes
  // Detector construction
  MDMDetectorConstruction* detector = new MDMDetectorConstruction();
  detector->SetIsChamber(IsChamber);
  runManager->SetUserInitialization(detector);

  // Physics list
  G4VModularPhysicsList* physicsList = new QGSP_BERT;
  if(IsChamber)
    {
      MDMInelasticPhysics* reactionPhysics = new MDMInelasticPhysics; 
      reactionPhysics->SetReactionParams(reactionParams);
      physicsList->RegisterPhysics(reactionPhysics);
    }
  runManager->SetUserInitialization(physicsList);
    
  // User action initialization
  MDMActionInitialization* actionInitialization = new MDMActionInitialization(detector);
  int numEntries = 0;
  if(!IsChamber) {
    TFile* f = new TFile(stageFile.c_str(),"read");
    TTree* tree = (TTree*) f-> Get("simData");
    numEntries = tree->GetEntries();
    OxfordInputs* stageTree = new OxfordInputs(tree);
    actionInitialization->SetStageTree(stageTree);
  
  }
  actionInitialization->SetEventActionParams(eventActionParams);
  runManager->SetUserInitialization(actionInitialization);

  // Initialize G4 kernel
  runManager->Initialize(); 

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (!isInteractive) {
     // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = macroName;
    UImanager->ApplyCommand(command+fileName);
    if(!IsChamber)
      runManager->BeamOn(numEntries);
  }
  else {
    // interactive mode : define UI session
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac"); 
#else
    UImanager->ApplyCommand("/control/execute init.mac"); 
#endif
    ui->SessionStart();
    delete ui;
#endif
  }
  
#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}
