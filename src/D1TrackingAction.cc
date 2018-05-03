//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: LXeTrackingAction.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical/LXe/src/LXeTrackingAction.cc
/// \brief Implementation of the LXeTrackingAction class
//
//
#include "D1Trajectory.hh"
#include "D1TrackingAction.hh"
#include "D1UserTrackInformation.hh"
#include "D1DetectorConstruction.hh"
#include "D1RecorderBase.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ParticleTypes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 D1TrackingAction::D1TrackingAction(D1RecorderBase* r)
  : fRecorder(r) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  

void D1TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  //Let this be up to the user via vis.mac
  //  fpTrackingManager->SetStoreTrajectory(true);

  //Use custom trajectory class
  fpTrackingManager->SetTrajectory(new D1Trajectory(aTrack));

  //This user track information is only relevant to the photons
  fpTrackingManager->SetUserTrackInformation(new D1UserTrackInformation);

  /*  const G4VProcess* creator = aTrack->GetCreatorProcess();
  if(creator)
    G4cout<<creator->GetProcessName()<<G4endl;
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1TrackingAction::PostUserTrackingAction(const G4Track* aTrack){
  D1Trajectory* trajectory=(D1Trajectory*)fpTrackingManager->GimmeTrajectory();
  D1UserTrackInformation*
    trackInformation=(D1UserTrackInformation*)aTrack->GetUserInformation();

  //Lets choose to draw only the photons that hit the sphere and a pmt
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition()){

    const G4VProcess* creator=aTrack->GetCreatorProcess();
    if(creator && creator->GetProcessName()=="OpShape1"){
      trajectory->Shape1();
      trajectory->SetDrawTrajectory(true);
    }

   // if(D1DetectorConstruction::GetShape1On()){
      if((trackInformation->GetTrackStatus()&hitShape1)&&
         (trackInformation->GetTrackStatus()&hitShape2)){
        trajectory->SetDrawTrajectory(true);
      //}
    }
    else{
      if(trackInformation->GetTrackStatus()&hitShape1)
        trajectory->SetDrawTrajectory(true);
    }
  }
  else //draw all other trajectories
    trajectory->SetDrawTrajectory(true);

  if(trackInformation->GetForceDrawTrajectory())
    trajectory->SetDrawTrajectory(true);

  if(fRecorder)fRecorder->RecordTrack(aTrack);
}
