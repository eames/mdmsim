#include "MDMEventAction.hh"
#include "MDMAnalysis.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

#include <gsl/gsl_randist.h>
#include "Randomize.hh"
#include <TRandom3.h>
#include <map>
#include <assert.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF2.h>
#include "G4Event.hh"

#include "MDMTrackerHit.hh"
#include "MDMHit.hh"
#include "MDMCsIHit.hh"
#include "MDMWireHit.hh"
#include "MDMTrace.h"

MDMEventAction::MDMEventAction()
: G4UserEventAction(),
  fSiHCID(-1),
  fEntranceHCID(-1),
 
  /*fEScint(0.),
  fPosScint(G4ThreeVector(0.,0.,0.)),*/
  fPW1(G4ThreeVector(0.,0.,0.)),
  fPW2(G4ThreeVector(0.,0.,0.)),
  fPW3(G4ThreeVector(0.,0.,0.)),
  fPW4(G4ThreeVector(0.,0.,0.))

{
  for(int i=0; i<4; i++) {
    for (int j=0; j<7; j++) {
      fTrackerHCID[i][j]=-1;
    }
  }

  for(int k=0; k<8; k++) fCsIHCID[k] = -1;
  for(int k=0; k<4; k++) fWireHCID[k] = -1;

  r = gsl_rng_alloc(gsl_rng_taus);
  G4long seed = floor(2015123456*G4UniformRand());
  fRandom3 = new TRandom3(seed);
  gsl_rng_set(r,seed);
    
  MDMTrace* mdm = MDMTrace::Instance();
    
  //Set MDM
  mdm->SetMDMAngle(            5.0);
  mdm->SetMDMDipoleField(   3384.8); //3384.8 for 11.22 excited state

  //Set Particles
  mdm->SetScatteredMass(        2.);
  mdm->SetScatteredCharge(      1.);
} 

MDMEventAction::~MDMEventAction()
{
  gsl_rng_free (r);
  delete fRandom3;
}

void MDMEventAction::BeginOfEventAction(const G4Event*)
{     
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    G4String SDname;
      
    if(fIsChamber) {
      fSiHCID = sdManager->GetCollectionID("si/Collection");
      fEntranceHCID = sdManager->GetCollectionID("entrance/Collection");
    }

    else {
      /*fEScint = 0.;
      fPosScint = G4ThreeVector(0.,0.,0.);*/
      fPW1 = G4ThreeVector(0.,0.,0.);
      fPW2 = G4ThreeVector(0.,0.,0.);
      fPW3 = G4ThreeVector(0.,0.,0.);
      fPW4 = G4ThreeVector(0.,0.,0.);

      for(int i=0; i<4; i++) {
	for (int j=0; j<7; j++) {
      	  char name[256];
      	  sprintf(name,"MuO_%d_%d/MuOCollection",i+1,j+1);
	  fTrackerHCID[i][j] = sdManager->GetCollectionID(SDname=name);
	}
      }

      for (int j=0; j<8; j++) {
	char name[256];
	sprintf(name,"CsIDetector%d/CsICollection",j+1);
	fCsIHCID[j] = sdManager->GetCollectionID(SDname=name);
      }
      
      for (int j=0; j<4; j++) {
	char name[256];
	sprintf(name,"Wire%d/WireCollection",j+1);
	fWireHCID[j] = sdManager->GetCollectionID(SDname=name);
      }
    }
}

void MDMEventAction::EndOfEventAction(const G4Event* event)
{  
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if(!hce)
    {
      std::cout << "No hits collecction of this event found" << std::endl;
      return;
    }
  
  MDMAnalysis* analysis = MDMAnalysis::Instance();

  if(fIsChamber) {
    
    MDMHitsCollection* hSiHC;
    MDMHitsCollection* hEntranceHC;
    hSiHC = static_cast<MDMHitsCollection*>(hce->GetHC(fSiHCID));
    hEntranceHC = static_cast<MDMHitsCollection*>(hce->GetHC(fEntranceHCID));
    /*
    if(notFound1) {
      std::cout << "Some of the hits collection might not be found" << std::endl;
      return;
    }*/

    G4double totalSiEnergyDep = 0.;
    G4ThreeVector avgPosition(0.,0.,0.);
    for(int i=0; i<hSiHC->entries();i++) {
      totalSiEnergyDep += (*hSiHC)[i]->GetEDep();
      avgPosition += (*hSiHC)[i]->GetLocalPosition();
    }

    avgPosition = (hSiHC->entries()>0) ? avgPosition/hSiHC->entries() : G4ThreeVector(0.,0.,0.);
    int siQuad;
    if(avgPosition[0]>0. && avgPosition[1]>0.) siQuad = 1;
    else if(avgPosition[0]>0. && avgPosition[1]<0.) siQuad = 2;
    else if(avgPosition[0]<0. && avgPosition[1]<0.) siQuad = 3;
    else if(avgPosition[0]<0. && avgPosition[1]>0.) siQuad = 4;

    analysis->SetSiEnergy(totalSiEnergyDep);
    analysis->SetSiQuad(siQuad);
 
    bool notFound1 = false;
    for(int i = hEntranceHC->entries()-1; i>=0; i--) {
      notFound1 = true;
      analysis->SetAccepted(true);
      analysis->SetEnergy2((*hEntranceHC)[i]->GetEnergy());
      analysis->SetCharge2((*hEntranceHC)[i]->GetHitCharge());
      analysis->SetMass2((*hEntranceHC)[i]->GetHitMass());

      MDMTrace* mdm = MDMTrace::Instance();

      mdm->SetScatteredCharge((*hEntranceHC)[i]->GetHitCharge());
      mdm->SetScatteredMass((*hEntranceHC)[i]->GetHitMass());
      mdm->SetScatteredEnergy((*hEntranceHC)[i]->GetEnergy());
      //std::cout << (*hEntranceHC)[i]->GetEnergy() << std::endl;
      mdm->SetScatteredAngle((*hEntranceHC)[i]->GetXAngle(),(*hEntranceHC)[i]->GetYAngle());
      mdm->SendRay();

      double mdmPosX, mdmPosY, mdmAngleX, mdmAngleY;
      //mdm->GetPositionAngleFirstWire(mdmPosX,mdmAngleX);
      mdm->GetPositionAngleFirstWire(mdmPosX,mdmPosY,mdmAngleX,mdmAngleY);

      if(mdmPosX<100000) {
        //printf("%f \t%f\t%f\t%f\n",mdmPosX,mdmAngleX,mdmPosY,mdmAngleY);
	
	analysis->SetTransmitted(true);
	analysis->SetAngle2(mdmAngleX,mdmAngleY);
	analysis->SetWirePos(mdmPosX,mdmPosY);

	double window_dist[2];
	window_dist[0] = (mdmPosX-(25.025*0.1*tan(fAngleX*3.14159/180.)));
	window_dist[1] = (mdmPosY-(25.025*0.1*tan(fAngleY*3.14159/180.)));
	analysis->SetWindowDist(window_dist[0],window_dist[1]);
      }
      else {
	analysis->SetTransmitted(false);
	analysis->SetAngle2(0.,0.);
	analysis->SetWirePos(0.,0.);
	analysis->SetWindowDist(0.,0.);
      }
      break;
    }
    if(!notFound1) {
      analysis->SetAccepted(false);
      analysis->SetEnergy2(0.);
      analysis->SetCharge2(0.);
      analysis->SetMass2(0.);
      analysis->SetTransmitted(false);
      analysis->SetAngle2(0.,0.);
      analysis->SetWirePos(0.,0.);
      analysis->SetWindowDist(0.,0.);
    }
  }
  else {
  
    MDMTrackerHitsCollection* hTrackerHC[4][7];
    bool notFoundTracker = false;
    for(int i=0; i<4; i++) {
      for (int j=0; j<7; j++) {
	hTrackerHC[i][j] = static_cast<MDMTrackerHitsCollection*>(hce->GetHC(fTrackerHCID[i][j]));
	if(!hTrackerHC[i][j]) notFoundTracker = true;
      }
    }
    
    MDMCsIHitsCollection* hCsIHC[8];
    bool notFoundCsI = false;
    for (int j=0; j<8; j++) {
      hCsIHC[j] = static_cast<MDMCsIHitsCollection*>(hce->GetHC(fCsIHCID[j]));
      if(!hCsIHC[j]) notFoundCsI = true;
    }
  
    MDMWireHitsCollection* hWireHC[4];
    bool notFoundWire = false;
    for (int j=0; j<4; j++) {
      hWireHC[j] = static_cast<MDMWireHitsCollection*>(hce->GetHC(fWireHCID[j]));
      if(!hWireHC[j]) notFoundWire = true;
    }

    if(notFoundTracker||notFoundCsI||notFoundWire) {
	std::cout << "Some of the hits collection might not be found" << std::endl;
	return;
      }

    std::vector<double> CsIHitX;
    std::vector<double> CsIHitY;
    std::vector<double> CsIHitZ;
    std::vector<int> CsIHitDet;
    std::vector<double> CsIHitE;
    std::vector<double> CsIHitT;
    std::vector<int> CsIHitMass;
    std::vector<int> CsIHitCharge;

    for(int i = 0;i<8;i++) {

      G4double totalCsIEnergyDep = 0.;
      G4ThreeVector avgCsIPosition(0.,0.,0.);
      G4int massCsI=0; G4int chargeCsI=0;
      G4double timeCsI;

      for(int j = 0;j<hCsIHC[i]->entries();j++) {
	CsIHitX.push_back((*hCsIHC[i])[j]->GetPosition().x());
	CsIHitY.push_back((*hCsIHC[i])[j]->GetPosition().y());
    	CsIHitZ.push_back((*hCsIHC[i])[j]->GetPosition().z());
	CsIHitE.push_back((*hCsIHC[i])[j]->GetEnergy());
 
     	/*totalCsIEnergyDep += (*hCsIHC[i])[j]->GetEnergy();
      	avgCsIPosition += (*hCsIHC[i])[j]->GetPosition();*/

	CsIHitT.push_back((*hCsIHC[i])[j]->GetTime());
    	CsIHitMass.push_back((*hCsIHC[i])[j]->GetMass());
    	CsIHitCharge.push_back((*hCsIHC[i])[j]->GetCharge());
	CsIHitDet.push_back(i+1);

      /*massCsI += (*hCsIHC[i])[j]->GetMass();
      	chargeCsI += (*hCsIHC[i])[j]->GetCharge();
      	timeCsI += (*hCsIHC[i])[j]->GetTime();*/
      }

      //avgCsIPosition = (hCsIHC[i]->entries()>0) ? avgCsIPosition/hCsIHC[i]->entries() : G4ThreeVector(0.,0.,0.);
      
      /*massCsI = (hCsIHC[i]->entries()>0) ? massCsI/hCsIHC[i]->entries() : 0;
      chargeCsI = (hCsIHC[i]->entries()>0) ? chargeCsI/hCsIHC[i]->entries() : 0;
      timeCsI = (hCsIHC[i]->entries()>0) ? timeCsI/hCsIHC[i]->entries() : 0.;*/

      /*CsIHitX.push_back(avgCsIPosition.x());
      CsIHitY.push_back(avgCsIPosition.y());
      CsIHitZ.push_back(avgCsIPosition.z());
      CsIHitE.push_back(totalCsIEnergyDep);*/

      /*CsIHitT.push_back(timeCsI);
      CsIHitMass.push_back(massCsI);
      CsIHitCharge.push_back(chargeCsI);*/
    }

    analysis->SetCsIHitX(CsIHitX);
    analysis->SetCsIHitY(CsIHitY);
    analysis->SetCsIHitZ(CsIHitZ);
    analysis->SetCsIHitE(CsIHitE);
    analysis->SetCsIHitT(CsIHitT);
    analysis->SetCsIHitMass(CsIHitMass);
    analysis->SetCsIHitCharge(CsIHitCharge);
    analysis->SetCsIHitDet(CsIHitDet);

    std::vector<double> WireHitX;
    std::vector<int> WireHitNo;
    std::vector<double> WireHitE;
    std::vector<int> WireHitMass;
    std::vector<int> WireHitCharge;

    for(int i = 0;i<4;i++) {
      for(int j = 0;j<hWireHC[i]->entries();j++) {
	WireHitX.push_back((*hWireHC[i])[j]->GetLocalPosition().x());
	WireHitE.push_back((*hWireHC[i])[j]->GetEnergy());
	WireHitMass.push_back((*hWireHC[i])[j]->GetHitMass());
	WireHitCharge.push_back((*hWireHC[i])[j]->GetHitCharge());
	WireHitNo.push_back(i+1);
      }
    }

    analysis->SetWireHitX(WireHitX);
    analysis->SetWireHitE(WireHitE);
    analysis->SetWireHitNo(WireHitNo);
    analysis->SetWireHitMass(WireHitMass);
    analysis->SetWireHitCharge(WireHitCharge);

    /*double clusterX[4][7];
    double clusterY[4][7];
    double clusterZ[4][7];
    double clusterE[4][7];
    double clusterT[4][7];
    int clusterSize[4][7];

    //assert we have enough indices in the analysis arrays
    for(int i=0; i<4; i++) {
      for (int j=0; j<7; j++) {
	printf("%d clusters fired\n",hTrackerHC[i][j]->entries());
      }
    }*/
/*
    //set cluster number and loop over it
    analysis->muo_mul=0;
    for(int i=0; i<4; i++) {
      for (int j=0; j<7; j++) {

	G4double totalEnergy = 0.;
	G4long totalElectrons = 0;

	//if(hTrackerHC[i][j]->entries()==0) return;
	//std::cout << " Here" <<std::endl;
	
	for(G4int k=0; k<hTrackerHC[i][j]->entries();++k) {
	  //Extract energy, derive electrons, get starting values
	  G4double energyDep = (*hTrackerHC[i][j])[k]->GetEnergy(); //MeV
	  G4double meanPairs = energyDep/fWorkFunction*1e6;
	  G4int thisPairs = floor(gsl_ran_gamma(r,meanPairs/fFanoFactor,fFanoFactor));
	  totalElectrons += thisPairs;
	  totalEnergy += energyDep;

	  G4double preX = (*hTrackerHC[i][j])[k]->GetPrePosition().x(); //cm
	  G4double preY = (*hTrackerHC[i][j])[k]->GetPrePosition().y();
	  G4double preZ = (*hTrackerHC[i][j])[k]->GetPrePosition().z();
	  G4double postX = (*hTrackerHC[i][j])[k]->GetPostPosition().x();
	  G4double postY = (*hTrackerHC[i][j])[k]->GetPostPosition().y();
	  G4double postZ = (*hTrackerHC[i][j])[k]->GetPostPosition().z();
	  G4double t = (*hTrackerHC[i][j])[k]->GetTime(); //ns

      	  //Push cluster values to analysis
      	  clusterX[i][j]=preX;
      	  clusterY[i][j]=preY;
      	  clusterZ[i][j]=preZ;
     	  clusterE[i][j]=energyDep;
     	  clusterT[i][j]=t;
     	  clusterSize[i][j]=thisPairs;
    
     	  //if(i%10==0) G4cout << preX <<"  "<<preY<<"  "<<preZ <<std::endl;}}}
     	  if(k%10==0) G4cout << "Tracked " << k << " of " << hTrackerHC[i][j]->entries() << " interactions (" 
                               << totalElectrons << " total electrons)" << std::endl;
    
     	  //Drift every electron  
       	  for(G4int l = 0;l<thisPairs;++l) {
    	    G4double x,y,z;
    	    if(postX==0.&&postY==0.&&postZ==0.) {
    	      x = preX;
    	      y = preY;
      	      z = preZ;
    	    } 
    	    else {
    	      G4double xSlope = (postX-preX)/(postZ-preZ);
    	      G4double ySlope = (postY-preY)/(postZ-preZ);
    	      G4double xInter = postX-xSlope*postZ;
    	      G4double yInter = postY-ySlope*postZ;
    	      G4double rand = G4UniformRand();
      	      z = rand*(postZ-preZ)+preZ;
      	      x = xSlope*z+xInter;
      	      y = ySlope*z+yInter;
    	    }
      
	    double x1,z1,t1;
	    //Get endpoint of electron drift
	    G4double height = 4.5-y; //cm
	    G4double driftTime1 = 1e-2*height/fDriftVelocity1; //ns
	    t1 = t+fRandom3->Gaus(driftTime1,driftTime1*fDriftTimeRes1/sqrt(height));
	    x1 = fRandom3->Gaus(x,height*fDriftPosRes1/sqrt(height));
	    z1 = fRandom3->Gaus(4.5-z,height*fDriftPosRes1/sqrt(height));
	    
	    //timingHists[i][j]->Fill(t1);
	    }
	 }
	 //energyHists[i][j]->Fill(totalEnergy);
       }
    }
*/
    analysis->SetW1P(fPW1);
    analysis->SetW2P(fPW2);
    analysis->SetW3P(fPW3);
    analysis->SetW4P(fPW4);
  }
  analysis->Fill();
}

void MDMEventAction::ParseParams(const std::map<std::string,double> &params) {
  for(std::map<std::string,double>::const_iterator it = params.begin(); it!= params.end(); it++) {
    if(it->first == "driftVelocity1") {
      fDriftVelocity1 = it->second;
      std::cout << "SET: Drift Velocity 1 -- " << fDriftVelocity1 <<std::endl;
    }else if(it->first == "driftTimeRes1") {
      fDriftTimeRes1 = it->second;
      std::cout << "SET: Drift Time Res 1 -- " << fDriftTimeRes1 <<std::endl;
    }else if(it->first == "driftPosRes1") {
      fDriftPosRes1 = it->second;
      std::cout << "SET: Drift Pos Res 1 -- " << fDriftPosRes1 <<std::endl;
    }else if(it->first == "driftVelocity2") {
      fDriftVelocity2 = it->second;
      std::cout << "SET: Drift Velocity 2 -- " << fDriftVelocity2 <<std::endl;
    }else if(it->first == "driftTimeRes2") {
      fDriftTimeRes2 = it->second;
      std::cout << "SET: Drift Time Res 2 -- " << fDriftTimeRes2 <<std::endl;
    }else if(it->first == "driftPosRes2") {
      fDriftPosRes2 = it->second;
      std::cout << "SET: Drift Pos Res 2 -- " << fDriftPosRes2 <<std::endl;
    }else if(it->first == "workFunction") {
      fWorkFunction = it->second;
      std::cout << "SET: Work Function -- " << fWorkFunction <<std::endl;
    }else if(it->first == "fanoFactor") {
      fFanoFactor = it->second;
      std::cout << "SET: Fano Factor -- " << fFanoFactor <<std::endl;
    }
  }
}

void MDMEventAction::SetAngle1(double angleX, double angleY)
{
  fAngleX = angleX;
  fAngleY = angleY;
}

void MDMEventAction::SetEnergy1(G4double energy)
{
  fEnergy = energy;
}

void MDMEventAction::SetInEnergy1(G4double inenergy)
{
  fInEnergy = inenergy;
}

void MDMEventAction::SetCharge1(G4int charge)
{
  fCharge = charge;
}

void MDMEventAction::SetMass1(G4int mass)
{
  fMass = mass;
}

/*void MDMEventAction::SetEScint(G4double scE)
{
  fEScint = scE;
}

void MDMEventAction::SetPosScint(G4ThreeVector posi)
{
  fPosScint = posi;
}*/

void MDMEventAction::SetW1Position(G4ThreeVector w1pos)
{ fPW1 = w1pos;
}

void MDMEventAction::SetW2Position(G4ThreeVector w2pos)
{ fPW2 = w2pos;
}
void MDMEventAction::SetW3Position(G4ThreeVector w3pos)
{ fPW3 = w3pos;
}

void MDMEventAction::SetW4Position(G4ThreeVector w4pos)
{ fPW4 = w4pos;
}





