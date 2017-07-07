#ifndef MDMActionInitialization_h
#define MDMActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"
#include <string>
#include <map>

class OxfordInputs;
class MDMDetectorConstruction;
class MDMEventAction;

/// Action initialization class

class MDMActionInitialization : public G4VUserActionInitialization
{
  public:
    MDMActionInitialization(MDMDetectorConstruction*);

    virtual ~MDMActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;
    
    void SetEventActionParams(std::map<std::string,double> params) {
      fEventActionParams = params;
    };

    void SetStageTree(OxfordInputs* tree) {
      fStageTree = tree;
    }

  private:
    MDMDetectorConstruction* fDetector;
    std::map<std::string,double> fEventActionParams;
    OxfordInputs* fStageTree;

};

#endif

    
