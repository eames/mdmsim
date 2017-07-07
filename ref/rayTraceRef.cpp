#include <stdio.h>
#include <iostream>
#include "MDMTrace.h"

int main() {
  //Get Global Instance
  MDMTrace* mdm = MDMTrace::Instance();

  //Set MDM
  double Brho =  727.7; // kG cm
  mdm->SetMDMAngle(              5.00);  //ALWAYS SET
  //mdm->SetMDMDipoleField(Brho*1000.0/160.);  //ALWAYS SET // Brho*1000.0/160. //dipole used*1.034
                                            // (In mdmTrace, ~LISE brho in TM * 1000 * 1000/160) 
  mdm->SetMDMDipoleField(3462.3);//3462.5

  //Set Particles                  
  mdm->SetTargetMass(    6.00);  //KINEMATICS ONLY *
  mdm->SetProjectileMass( 22.);  //KINEMATICS ONLY *
  mdm->SetScatteredMass(   2.014102);    //ALWAYS
  mdm->SetScatteredCharge( 1.);    //ALWAYS

  //Set Energetics
  mdm->SetBeamEnergy(   20.00);  //KINEMATICS ONLY *
  mdm->SetQValue(        9.14);  //KINEMATICS ONLY * //Ground state
  mdm->SetResidualEnergy(11.2);  //KINEMATICS ONLY *
  mdm->SetScatteredEnergy(7.756);  //WITHOUT KINEMATICS ONLY

  //Set Angles
  double angle = 5.00;
  //double angles[7] =  {1.95,3.42,4.185,4.95,5.715,6.48,7.95};
  //double angles[7] = {angle-(2.0*0.765 + 1.47),angle-(2.0*0.765),angle-0.765,angle,angle+0.765,angle+(2.0*0.765),angle + (2.0*0.765 + 1.47)};

  printf("Oxford Wire Positions\n");
  /*for(int i=0;i<7;i++) {
    mdm->SetScatteredAngle(angles[i]);  //ALWAYS
    mdm->SendRay();                     //WITHOUT KINEMATICS ONLY
    //mdm->SendRayWithKinematics();     //KINEMATICS ONLY *
    double x1,x2,x3,x4,a1;
    mdm->GetOxfordWirePositions(a1,x1,x2,x3,x4);
    printf("Initial Angle: %8.2f\t W1Angle: %8.2f\t 1: %5.4f\t 2: %5.4f\t 3: %5.4f\t 4: %5.4f\n",angles[i],a1,x1,x2,x3,x4);
  }*/
    
  mdm->SetScatteredAngle(angle);  //ALWAYS
  mdm->SendRay();                     //WITHOUT KINEMATICS ONLY
  double x1,x2,x3,x4,a1;
  mdm->GetOxfordWirePositions(a1,x1,x2,x3,x4);
  printf("%5.4f\n",x1);
  printf("%5.4f\n",x2);
  printf("%5.4f\n",x3);
  printf("%5.4f\n",x4);
  return 0;
}
