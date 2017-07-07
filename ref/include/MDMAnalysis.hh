#ifndef MDMAnalysis_h
#define MDMAnalysis_h

#include <vector>
#include <string>
#include "globals.hh"
#include "G4ThreeVector.hh"

class TFile;
class TTree;
class TH1F;

class MDMAnalysis {
public:
  MDMAnalysis();
  static MDMAnalysis* Instance();
  void SetFilename(const char*);

  void OpenFile(bool);
  void CloseFile();

  void SetAngle2(double,double);
  void SetWindowDist(double, double);
  void SetEnergy2(G4double);
  void SetWirePos(double,double);
  void SetCharge2(int);
  void SetMass2(int);
  void SetAccepted(bool);
  void SetTransmitted(bool);
  void SetSiEnergy(double);
  void SetSiQuad(int);

  //void SetEScint(double);
  //void SetPosScint(G4ThreeVector);

  //void SetEdep(double);

  void SetW1P(G4ThreeVector);
  void SetW2P(G4ThreeVector);
  void SetW3P(G4ThreeVector);
  void SetW4P(G4ThreeVector);

  void SetCsIHitX(std::vector<double>& v);
  void SetCsIHitY(std::vector<double>& v);
  void SetCsIHitZ(std::vector<double>& v);
  void SetCsIHitE(std::vector<double>& v);
  void SetCsIHitT(std::vector<double>& v);
  void SetCsIHitDet(std::vector<int>& v);  
  void SetCsIHitMass(std::vector<int>& v);
  void SetCsIHitCharge(std::vector<int>& v);

  void SetWireHitX(std::vector<double>& v);
  void SetWireHitE(std::vector<double>& v);
  void SetWireHitNo(std::vector<int>& v);  
  void SetWireHitMass(std::vector<int>& v);
  void SetWireHitCharge(std::vector<int>& v);  

  //void SetMuO(int,int, double,double);
  int muo_mul;
  int fRow[4];
  int fColumn[7];
  double fMuOTime[28];
  double fMuOEnergy[28];

  void Fill();

private:
  static MDMAnalysis* fInstance;
  std::string fFilename;
  TFile* fFile;
  TTree* fTree;

  double fangle[2];
  double fwposition[2];
  double fenergy;
  double fwirepos[2];
  int fcharge;
  int fmass;
  bool fAccepted;
  bool fTransmitted;
  double fSiEnergy;
  int fSiQuad;

  /*double fEScint;
  double fEdep;
  G4double fPosScint[3];*/
  G4double fW1P[3];
  G4double fW2P[3];
  G4double fW3P[3];
  G4double fW4P[3];

  std::vector<double> fCsIHitX;
  std::vector<double> fCsIHitY;
  std::vector<double> fCsIHitZ;
  std::vector<double> fCsIHitE;
  std::vector<double> fCsIHitT;
  std::vector<int> fCsIHitDet;
  std::vector<int> fCsIHitMass;
  std::vector<int> fCsIHitCharge;

  std::vector<double> fWireHitX;
  std::vector<double> fWireHitE;
  std::vector<int> fWireHitNo;
  std::vector<int> fWireHitMass;
  std::vector<int> fWireHitCharge;
};

#endif
