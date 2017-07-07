//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 25 05:39:21 2016 by ROOT version 5.34/24
// from TTree simData/Simulation Data
// found on file: MDM_0.root
//////////////////////////////////////////////////////////

#ifndef OxfordInputs_h
#define OxfordInputs_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class OxfordInputs {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Double_t        angle_X;
   Double_t        angle_Y;
   Double_t        window_position_X;
   Double_t        window_position_Y;
   Double_t        energy;
   Double_t        wire_position_X;
   Double_t        wire_position_Y;
   Int_t           charge;
   Int_t           mass;
   Bool_t          transmitted;
   Bool_t          accepted;
   Double_t        siEnergy;
   Int_t           siQuad;

   // List of branches
   TBranch        *b_angle_X;   //!
   TBranch        *b_angle_Y;   //!
   TBranch        *b_window_position_X;   //!
   TBranch        *b_window_position_Y;   //!
   TBranch        *b_energy;   //!
   TBranch        *b_wire_position_X;   //!
   TBranch        *b_wire_position_Y;   //!
   TBranch        *b_charge;   //!
   TBranch        *b_mass;   //!
   TBranch        *b_transmitted;   //!
   TBranch        *b_accepted;   //!
   TBranch        *b_siEnergy;   //!
   TBranch        *b_siQuad;   //!

   OxfordInputs(TTree *tree=0);
   virtual ~OxfordInputs();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef OxfordInputs_cxx
OxfordInputs::OxfordInputs(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      //TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("MDM_0.root");
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/hdfs/user/hjayatissa/geant_mdm_csi/stage1/MDM_0.root");
      if (!f || !f->IsOpen()) {
         //f = new TFile("MDM_0.root");
         f = new TFile("/hdfs/user/hjayatissa/geant_mdm_csi/stage1/MDM_0.root");
      }
      f->GetObject("simData",tree);

   }
   Init(tree);
}

OxfordInputs::~OxfordInputs()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t OxfordInputs::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t OxfordInputs::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void OxfordInputs::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("angle_X", &angle_X, &b_angle_X);
   fChain->SetBranchAddress("angle_Y", &angle_Y, &b_angle_Y);
   fChain->SetBranchAddress("window_position_X", &window_position_X, &b_window_position_X);
   fChain->SetBranchAddress("window_position_Y", &window_position_Y, &b_window_position_Y);
   fChain->SetBranchAddress("energy", &energy, &b_energy);
   fChain->SetBranchAddress("wire_position_X", &wire_position_X, &b_wire_position_X);
   fChain->SetBranchAddress("wire_position_Y", &wire_position_Y, &b_wire_position_Y);
   fChain->SetBranchAddress("charge", &charge, &b_charge);
   fChain->SetBranchAddress("mass", &mass, &b_mass);
   fChain->SetBranchAddress("transmitted", &transmitted, &b_transmitted);
   fChain->SetBranchAddress("accepted", &accepted, &b_accepted);
   fChain->SetBranchAddress("siEnergy", &siEnergy, &b_siEnergy);
   fChain->SetBranchAddress("siQuad", &siQuad, &b_siQuad);
   Notify();
}

Bool_t OxfordInputs::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void OxfordInputs::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t OxfordInputs::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef OxfordInputs_cxx
