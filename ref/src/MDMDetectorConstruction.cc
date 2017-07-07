#include "MDMDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4UserLimits.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"

#include "G4RunManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"

#include "MDMTrackerSD.hh"
#include "G4SDManager.hh"
#include "MDMSD.hh"
#include "MDMWireSD.hh"
#include "MDMCsISD.hh"

MDMDetectorConstruction::MDMDetectorConstruction() :
  G4VUserDetectorConstruction(),
  fPressureinTorr(100.0),fTemperature(293.15)
{}

MDMDetectorConstruction::~MDMDetectorConstruction(){
}

G4VPhysicalVolume* MDMDetectorConstruction::Construct() {

  ConstructMaterials();
  
  G4Material* vacuum =
      new G4Material("Vacuum",      //Name as String
         1,             //Atomic Number,  in this case we use 1 for hydrogen  
         1.008*g/mole,  //Mass per Mole "Atomic Weight"  1.008*g/mole for Hydoren 
         1.e-25*g/cm3,  //Density of Vaccuum  *Cant be Zero, Must be small insted 
         kStateGas,     //kStateGas for Gas
         2.73*kelvin,   //Temperature for gas 
         1.e-25*g/cm3); //Pressure for Vaccum

  //Overlaps flag
  G4bool checkOverlaps = true;

  //Create vacuum filled world
  G4VSolid* solidWorld = new G4Box("World",5.*m,5.*m,5.*m);
  logicWorld = new G4LogicalVolume(solidWorld,vacuum,"WorldLogical");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"WorldPhys",0,false,0,checkOverlaps);
  //logicWorld->SetUserLimits(new G4UserLimits(0.1e-8*m));

  if(fIsChamber) {
    //Materials
    //G4Material* li = G4Material::GetMaterial("G4_Li");
    G4Material* c = G4Material::GetMaterial("G4_C");
    G4Material* si = G4Material::GetMaterial("G4_Si");
    
    G4Element* elLi = new G4Element("Lithium","L" , 3, 6.015*g/mole);
    G4Element* elF  = new G4Element("Fluorine"  ,"F" , 9, 18.998*g/mole);
    G4Material* LiF = new G4Material("LiF",2.635*g/cm3,2);
    LiF->AddElement(elLi,1);
    LiF->AddElement(elF,1);
  
    //dimensions
    G4double targetXY = 2.0*cm; 
    numLayers = 500;
    G4double targetthickness = 30.e-6*g/cm2; //0.000565*mm 7.2e-6 for Li only
    G4double backingThickness = 10.e-6*g/cm2;

    std::cout<< "thickness is    "<< targetthickness/(g/cm2) <<"  Density is "<<LiF->GetDensity()/(g/cm3)<<std::endl;
  
    //____________________
    //Create target
    //____________________
    G4VSolid* solidTarget = new G4Box("Target",0.5*targetXY,0.5*targetXY,0.5*targetthickness/numLayers/LiF->GetDensity());
    logicTarget = new G4LogicalVolume(solidTarget,LiF,"TargetLogical");
    logicTarget->SetUserLimits(new G4UserLimits(0.1e-8*m));
    for(int x=0; x<numLayers; x++) {
      new G4PVPlacement(0,G4ThreeVector(0.0,0.0,targetthickness/numLayers/LiF->GetDensity()*(0.5+x)),logicTarget,"TargetPhys",logicWorld,false,x,checkOverlaps);}

    //____________________
    //Target Backing
    //____________________
    G4VSolid* solidBacking = new G4Box("Backing",0.5*targetXY, 0.5*targetXY, backingThickness/2./c->GetDensity());
    logicBacking = new G4LogicalVolume(solidBacking,c,"BackingLogical");
    new G4PVPlacement(0,G4ThreeVector(0.,0.,backingThickness/2./c->GetDensity()+targetthickness/LiF->GetDensity()),logicBacking,"BackingPhys",logicWorld,false,0,checkOverlaps);

    //____________________
    // Slit Box
    // ___________________
    G4double slitBoxDistance = 65.25*cm;
    G4double mdmAngle = 5.0*deg;
    G4VSolid* entranceSlitBox = new G4Box("entranceSlitBox",slitBoxDistance*tan(2.*3.14159/180.),slitBoxDistance*tan(1*3.14159/180.),1.*mm);//4x1 for wide, 0.1x1 for single
    fEntranceSlitLogical = new G4LogicalVolume(entranceSlitBox,vacuum,"entranceSlitLogical");
    G4RotationMatrix rotm = G4RotationMatrix(); //rotation of daughter frame
    rotm.rotateY(mdmAngle); 
    G4ThreeVector translate = G4ThreeVector(sin(mdmAngle)*slitBoxDistance,0.,cos(mdmAngle)*slitBoxDistance); //position in mother frame
    G4Transform3D transform = G4Transform3D(rotm,translate);
    new G4PVPlacement(transform,fEntranceSlitLogical,"entranceSlitPhysical",logicWorld,false,0,checkOverlaps);

    //____________________
    //Si detector
    //____________________
    G4double quadSize        = 2.5*cm;
    G4double siThickness     = 1.0*mm;
    G4double flangeWidth     = 5.75*mm;
    G4double siRotationAngle = -30.*deg;
    G4double siDistance      = 15*cm;
    
    G4VSolid* solidSi = new G4Box("siDetector",quadSize/2.,quadSize/2.,siThickness/2.);
    logicSi = new G4LogicalVolume(solidSi,si,"SiLogical");
    G4RotationMatrix rotm1 = G4RotationMatrix();
    rotm1.rotateY(siRotationAngle);
    new G4PVPlacement(G4Transform3D(rotm1,G4ThreeVector(sin(siRotationAngle)*siDistance + cos(siRotationAngle)*quadSize/2.,quadSize/2.,cos(siRotationAngle)*siDistance - sin(siRotationAngle)*quadSize/2.)),logicSi,"siPhys",logicWorld,false,0,checkOverlaps);
    new G4PVPlacement(G4Transform3D(rotm1,G4ThreeVector(sin(siRotationAngle)*siDistance + cos(siRotationAngle)*quadSize/2.,-quadSize/2.,cos(siRotationAngle)*siDistance - sin(siRotationAngle)*quadSize/2.)),logicSi,"siPhys",logicWorld,false,1,checkOverlaps);
    new G4PVPlacement(G4Transform3D(rotm1,G4ThreeVector(sin(siRotationAngle)*siDistance - cos(siRotationAngle)*quadSize/2.,-quadSize/2.,cos(siRotationAngle)*siDistance + sin(siRotationAngle)*quadSize/2.)),logicSi,"siPhys",logicWorld,false,2,checkOverlaps);
    new G4PVPlacement(G4Transform3D(rotm1,G4ThreeVector(sin(siRotationAngle)*siDistance - cos(siRotationAngle)*quadSize/2.,quadSize/2.,cos(siRotationAngle)*siDistance + sin(siRotationAngle)*quadSize/2.)),logicSi,"siPhys",logicWorld,false,3,checkOverlaps);
  }
  else {
    //Materials
    G4Material* kapton = G4Material::GetMaterial("G4_KAPTON");
    G4Material* mylar = G4Material::GetMaterial("G4_MYLAR");
    G4Material* csi = G4Material::GetMaterial("G4_CESIUM_IODIDE");

    G4Element* elH = new G4Element("Hydrogen","H" , 1, 1.008*g/mole);
    G4Element* elC  = new G4Element("Carbon"  ,"C" , 6, 12.011*g/mole);

    //Gas for Oxford
    double molarmass = 58.12; //g per mole //58.12 for isobutane //16.04 for methane
    double density = 1.603e-05*fPressureinTorr*molarmass/fTemperature;

    G4Material* isobutane= new G4Material("Isobutane",density*g/cm3, 2, kStateGas); //C4H10
    isobutane->AddElement(elC,4);
    isobutane->AddElement(elH,10);

    G4Material* methane= new G4Material("Methane", density*g/cm3, 2, kStateGas); //C1H4
    methane->AddElement(elC,1);
    methane->AddElement(elH,4);

    fGas = isobutane;

    //dimensions
    G4double oxfordX = 30.8*cm;
    G4double oxfordY = 12.*cm;
    G4double volumeZ = 650.*mm;
    G4double FWZ = 0.025*mm;
    G4double FoilZ = 0.002*mm;
    G4double csiZ = 4.0*cm;
    G4double wire_thick = 14.*mm;

    //G4double vol1 = 25*mm ; G4double vol2 = 151*mm; G4double vol3 = 163*mm; G4double vol4 = 145*mm; G4double vol5 = 110*mm;
    G4double vol1 = 25.*mm ; G4double vol2 = 151.*mm; G4double vol3 = 163.*mm; G4double vol4 = 7.*mm; G4double vol5 = 7.*mm; G4double vol6=110.*mm;

    //_____________________________
    //Create Front Window of Oxford
    //_____________________________
    G4Box* solidFW = new G4Box("Front_Window",0.5*oxfordX,0.5*oxfordY,0.5*FWZ); 
    //G4Box* solidFW = new G4Box("Front_Window",0.5*250.0*cm,0.5*oxfordY,0.5*FWZ);
    logicFW = new G4LogicalVolume(solidFW,kapton,"FWLogical");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,0.5*FWZ),logicFW,"FWPhys",logicWorld,false,0,checkOverlaps);  
    

    //____________________________
    //Create mylar foil cover of CsI
    //____________________________
    G4VSolid* solidFoil = new G4Box("mylarFoil",0.5*5.0*cm,0.5*5.0*cm,0.5*FoilZ);
    logicFoil = new G4LogicalVolume(solidFoil,mylar,"FoilLogical");
    for(int i=0; i<8; i++) {
      new G4PVPlacement(0,G4ThreeVector(3.5*5.0*cm-5.0*cm*i,0.0,FWZ+volumeZ+0.5*FoilZ),logicFoil,"FoilPhys",logicWorld,false,0,checkOverlaps); 
    } 

    //____________________________
    //Create CsI delectors
    //____________________________
    for(int j=0; j<8; j++) {
      char name[256];
      sprintf(name,"CsIBox%d",j+1);
      G4VSolid* solidCsI = new G4Box("CsI",0.5*5.0*cm,0.5*5.0*cm,0.5*csiZ);
      sprintf(name,"CsILogical%d",j+1);
      logicCsI[j] = new G4LogicalVolume(solidCsI,csi,name);
      new G4PVPlacement(0,G4ThreeVector(3.5*5.0*cm-5.0*cm*j,0.0,FWZ+volumeZ+FoilZ+0.5*csiZ),logicCsI[j],name,logicWorld,false,0,checkOverlaps);  
    }

    //Tracking volume for MuO
    /*
    G4VSolid* solidTracker = new G4Box("VolumeTracker",0.5*oxfordX,0.5*10.5*cm,0.5*13.0*cm); 
    logicTracker = new G4LogicalVolume(solidTracker,fGas,"TrackerLogical");
    new G4PVPlacement(0,G4ThreeVector(0.0,-0.75*cm,FWZ+vol1+vol2+vol3+vol4+3.0*wire_thick+0.5*13.0*cm),logicTracker,"TrackerPhys",logicWorld,false,0,checkOverlaps);   
    */

    for(int i=0; i<4; i++) {
      for(int j=0; j<7; j++) {
	char name[256];
	sprintf(name,"solidMuO_%d_%d",i+1,j+1);
	G4VSolid* solidMuO = new G4Box(name,0.5*44.0*mm,0.5*oxfordY,0.5*32.5*mm); 
	sprintf(name,"logicMuO_%d_%d",i+1,j+1);
	logicMuO[i][j] = new G4LogicalVolume(solidMuO,fGas,name); 
	sprintf(name,"phyMuO_%d_%d",i+1,j+1);

	G4double muo_center[3];

	muo_center[0]=-3.0*44.0*mm + j*44.0*mm;
	muo_center[1]=0*cm;
	muo_center[2]=FWZ+vol1+vol2+vol3+vol4+3.0*wire_thick+0.5*32.5*mm + 32.5*mm*i;

	new G4PVPlacement(0,G4ThreeVector(muo_center[0],muo_center[1],muo_center[2]),logicMuO[i][j],"MuOPhy",logicWorld,false,0,checkOverlaps);
      }
    }

    //____________________
    //WIRES
    //____________________
    
    for(int k=0; k<4; k++) {
      char name[256];
      sprintf(name,"WireBox%d",k+1);
      G4Box* solidW = new G4Box(name,0.5*oxfordX,0.5*oxfordY,0.5*wire_thick); 
      sprintf(name,"WireLogical%d",k+1);
      logicW[k] = new G4LogicalVolume(solidW,fGas,name); 
    }
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ + vol1 +0.5*wire_thick),logicW[0],"W1Phys",logicWorld,false,0,checkOverlaps);  
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ + vol1 + vol2 + wire_thick + 0.5*wire_thick),logicW[1],"W2Phys",logicWorld,false,0,checkOverlaps);  
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ + vol1 + vol2 + vol3 + 2.0*wire_thick +0.5*wire_thick),logicW[2],"W3Phys",logicWorld,false,0,checkOverlaps);
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ + vol1 + vol2 + vol3 + vol4 + vol5 + 13.0*cm + 3.0*wire_thick + 0.5*wire_thick),logicW[3],"W4Phys",logicWorld,false,0,checkOverlaps);

    ///////Volume parts///////

    G4Box* solidVolume1 = new G4Box("Volume1",0.5*oxfordX,0.5*oxfordY,0.5*vol1); 
    logicVolume1 = new G4LogicalVolume(solidVolume1,fGas,"VolumeLogical1");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ+0.5*vol1),logicVolume1,"VolumePhys1",logicWorld,false,0,checkOverlaps);   

    G4Box* solidVolume2 = new G4Box("Volume2",0.5*oxfordX,0.5*oxfordY,0.5*vol2); 
    logicVolume2 = new G4LogicalVolume(solidVolume2,fGas,"VolumeLogical2");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ+vol1+wire_thick+0.5*vol2),logicVolume2,"VolumePhys2",logicWorld,false,0,checkOverlaps);   

    G4Box* solidVolume3 = new G4Box("Volume3",0.5*oxfordX,0.5*oxfordY,0.5*vol3); 
    logicVolume3 = new G4LogicalVolume(solidVolume3,fGas,"VolumeLogical3");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ+vol1+vol2+2.0*wire_thick+0.5*vol3),logicVolume3,"VolumePhys3",logicWorld,false,0,checkOverlaps);   
    
    G4Box* solidVolume4 = new G4Box("Volume4",0.5*oxfordX,0.5*oxfordY,0.5*vol4); 
    logicVolume4 = new G4LogicalVolume(solidVolume4,fGas,"VolumeLogical4");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ+vol1+vol2+vol3+3.0*wire_thick+0.5*vol4),logicVolume4,"VolumePhys4",logicWorld,false,0,checkOverlaps);   
    
    G4Box* solidVolume5 = new G4Box("Volume5",0.5*oxfordX,0.5*oxfordY,0.5*vol5); 
    logicVolume5 = new G4LogicalVolume(solidVolume5,fGas,"VolumeLogical5");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ+vol1+vol2+vol3+vol4+3.0*wire_thick+130*mm+0.5*vol5),logicVolume5,"VolumePhys5",logicWorld,false,0,checkOverlaps); 

    G4Box* solidVolume6 = new G4Box("Volume5",0.5*oxfordX,0.5*oxfordY,0.5*vol6); 
    logicVolume6 = new G4LogicalVolume(solidVolume6,fGas,"VolumeLogical6");
    new G4PVPlacement(0,G4ThreeVector(0.0,0.0,FWZ+vol1+vol2+vol3+vol4+vol5+130*mm+4.0*wire_thick+0.5*vol6),logicVolume6,"VolumePhys6",logicWorld,false,0,checkOverlaps);   
  }

  SetAttributes();

  return physWorld;
}

void MDMDetectorConstruction::ConstructMaterials() {
  G4NistManager* man = G4NistManager::Instance();
  //man->FindOrBuildMaterial("G4_Li");
  man->FindOrBuildMaterial("G4_KAPTON");
  man->FindOrBuildMaterial("G4_MYLAR");
  man->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  man->FindOrBuildMaterial("G4_C");
  man->FindOrBuildMaterial("G4_Si");
}

void MDMDetectorConstruction::ConstructSDandField()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;

  if(fIsChamber) {
    G4VSensitiveDetector* siDetector = new MDMSD("si");
    G4VSensitiveDetector* entranceDetector = new MDMSD("entrance");
    SDman->AddNewDetector(siDetector);
    SDman->AddNewDetector(entranceDetector);
    logicSi->SetSensitiveDetector(siDetector);
    fEntranceSlitLogical->SetSensitiveDetector(entranceDetector);
  }
  else {
    
    //MuO
    for(int i=0; i<4; i++) {
      for(int j=0; j<7; j++) {
	char name[256];
	sprintf(name,"MuO_%d_%d",i+1,j+1);
	G4VSensitiveDetector* MuO = new MDMTrackerSD(SDname=name);
	SDman->AddNewDetector(MuO);
	logicMuO[i][j]->SetSensitiveDetector(MuO);
      }
    }

    //CsI detectors
    for(int i=0; i<8; i++) {
      char name[256];
      sprintf(name,"CsIDetector%d",i+1);
      G4VSensitiveDetector* CsIDetector = new MDMCsISD(SDname=name);
      SDman->AddNewDetector(CsIDetector);
      logicCsI[i]->SetSensitiveDetector(CsIDetector);
    }

    //Wires
    for(int i=0; i<4; i++) {
      char name[256];
      sprintf(name,"Wire%d",i+1);
      G4VSensitiveDetector* Wire = new MDMWireSD(SDname=name);
      SDman->AddNewDetector(Wire);
      logicW[i]->SetSensitiveDetector(Wire);
    }
/*
    //Build Field
    G4UniformElectricField* field = new G4UniformElectricField(G4ThreeVector(0.0,1000./fDriftGap*volt/cm,0.0));
    G4EqMagElectricField* equation = new G4EqMagElectricField(field); 
    G4ClassicalRK4* stepper = new G4ClassicalRK4( equation, 8 );   
    G4FieldManager* fieldMgr = new G4FieldManager();
    fieldMgr->SetDetectorField(field);
    G4double minStep = 0.010*mm ;
    G4MagInt_Driver* intgrDriver = new G4MagInt_Driver(minStep,stepper,stepper->GetNumberOfVariables() );
    G4ChordFinder* chordFinder = new G4ChordFinder(intgrDriver);
    fieldMgr->SetChordFinder( chordFinder );
    fTrackingLogical->SetFieldManager(fieldMgr,true);  */
  }
}

void MDMDetectorConstruction::SetAttributes() {
  G4VisAttributes * worldAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  worldAttr->SetVisibility(false);
  logicWorld->SetVisAttributes(worldAttr);
  
  if(fIsChamber) {
    G4VisAttributes * targetAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    targetAttr->SetVisibility(true);
    targetAttr->SetForceWireframe(true);
    logicTarget->SetVisAttributes(targetAttr);
  
    G4VisAttributes * scoreAttr = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    scoreAttr->SetVisibility(true);
    scoreAttr->SetForceWireframe(true);
    fEntranceSlitLogical->SetVisAttributes(scoreAttr);

    G4VisAttributes * siAttr = new G4VisAttributes(G4Colour(0.5, 0., 0.5));
    siAttr->SetVisibility(true);
    siAttr->SetForceWireframe(true);
    logicSi->SetVisAttributes(siAttr);

  }
  else {

    G4VisAttributes * FWAttr = new G4VisAttributes(G4Colour(1.5, 0.5, 0.5));
    FWAttr->SetVisibility(true);
    FWAttr->SetForceWireframe(true);
    logicFW->SetVisAttributes(FWAttr);

    G4VisAttributes * FoilAttr = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    FoilAttr->SetVisibility(true);
    FoilAttr->SetForceWireframe(true);
    logicFoil->SetVisAttributes(FoilAttr);

    //CsI detectors
    G4VisAttributes * CsIAttr = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    CsIAttr->SetVisibility(true);
    CsIAttr->SetForceWireframe(true);
    for(int i=0; i<8; i++) logicCsI[i]->SetVisAttributes(CsIAttr);
   

    //Wires
    G4VisAttributes * WAttr = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    WAttr->SetVisibility(true);
    WAttr->SetForceWireframe(true);
    for(int k=0; k<4; k++) logicW[k]->SetVisAttributes(WAttr);

    //Tracking Volume
    G4VisAttributes * MuOAttr = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    MuOAttr->SetVisibility(true);
    MuOAttr->SetForceWireframe(true);
    //logicTracker->SetVisAttributes(MuOAttr);
    for(int i=0; i<4; i++) {
      for(int j=0; j<7; j++) {
	logicMuO[i][j]->SetVisAttributes(MuOAttr);
      }
    }
   

    //Scints 5

    G4VisAttributes * VolAttr1 = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    VolAttr1->SetVisibility(false);
    VolAttr1->SetForceWireframe(true);
    logicVolume1->SetVisAttributes(VolAttr1);
    G4VisAttributes * VolAttr2 = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    VolAttr2->SetVisibility(false);
    VolAttr2->SetForceWireframe(true);
    logicVolume2->SetVisAttributes(VolAttr2);
    G4VisAttributes * VolAttr3 = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    VolAttr3->SetVisibility(false);
    VolAttr3->SetForceWireframe(true);
    logicVolume3->SetVisAttributes(VolAttr3);
    G4VisAttributes * VolAttr4 = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    VolAttr4->SetVisibility(false);
    VolAttr4->SetForceWireframe(true);
    logicVolume4->SetVisAttributes(VolAttr4);
    G4VisAttributes * VolAttr5 = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    VolAttr5->SetVisibility(false);
    VolAttr5->SetForceWireframe(true);
    logicVolume5->SetVisAttributes(VolAttr5);
    G4VisAttributes * VolAttr6 = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    VolAttr6->SetVisibility(false);
    VolAttr6->SetForceWireframe(true);
    logicVolume6->SetVisAttributes(VolAttr6);
  }
}

