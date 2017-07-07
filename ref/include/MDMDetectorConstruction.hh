#ifndef MDMDetectorConstruction_h
#define MDMDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4FieldManager.hh"
#include "G4NistManager.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class MDMDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MDMDetectorConstruction();
    virtual ~MDMDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    void ConstructMaterials();
    virtual void ConstructSDandField();

    void SetIsChamber(bool isChamber) {
      fIsChamber = isChamber;
      std::cout <<"SET: Is Chamber -- " << isChamber << std::endl;
    };

    bool IsChamber() const {return fIsChamber;};
    G4int GetNumLayers() const {return numLayers;};
    
    G4LogicalVolume* GetTargetVolume() const {return logicTarget; }
    //G4LogicalVolume* GetScintVolume() const {return logicTarget;}
    //G4LogicalVolume* GetMDMOpening() const {return fEntranceSlitLogical;}
    //G4LogicalVolume* GetSiVolume() const {return logicSi;};
    
    G4LogicalVolume* GetW1() const {return logicW[0]; }
    G4LogicalVolume* GetW2() const {return logicW[1]; }
    G4LogicalVolume* GetW3() const {return logicW[2]; }
    G4LogicalVolume* GetW4() const {return logicW[3]; }

  protected: 
    G4LogicalVolume* logicBacking;
    G4LogicalVolume* logicSi;

    int numLayers;
    
    G4LogicalVolume*  fEntranceSlitLogical;
    G4LogicalVolume*  logicWorld;
    G4LogicalVolume*  logicTarget;

    bool fIsChamber;

    G4LogicalVolume*  logicFW;
    G4LogicalVolume*  logicVolume;
    G4LogicalVolume*  logicFoil;
    G4LogicalVolume*  logicCsI[8];
    G4LogicalVolume* logicMuO[4][7];

    G4LogicalVolume* logicW[4];

    G4LogicalVolume* logicVolume1;
    G4LogicalVolume* logicVolume2;
    G4LogicalVolume* logicVolume3;
    G4LogicalVolume* logicVolume4;
    G4LogicalVolume* logicVolume5;
    G4LogicalVolume* logicVolume6;

    G4double fPressureinTorr;
    G4double fTemperature;

    G4Material* fGas;
  
  void SetAttributes();
 };

#endif
