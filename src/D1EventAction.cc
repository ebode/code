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
// $Id: D1EventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file D1EventAction.cc
/// \brief Implementation of the B1EventAction class
#include "D1EventAction.hh"
#include "D1Analysis.hh"
#include "D1UserEventInformation.hh"
#include "D1Trajectory.hh"
#include "D1Shape1Hit.hh"
#include "D1Shape2Hit.hh"
#include "D1RecorderBase.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"


#include "Randomize.hh"
#include <iomanip>

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
D1EventAction::D1EventAction(D1RecorderBase* r)
: G4UserEventAction(),fRecorder(r),
   fSha1EdepHCID(-1),
   fSha2EdepHCID(-1),
   fSha1TrackLengthHCID(-1),
   fSha2TrackLengthHCID(-1), 
   fSaveThreshold(0),fShape1CollID(-1),fShape2CollID(-1),fVerbose(0), fShape1Threshold(1),
   fShape2Threshold(1),fForcedrawphotons(false),fForcenophotons(false)
{
fEventMessenger = new D1EventMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
D1EventAction::~D1EventAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4THitsMap<G4double>* 
D1EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* anEvent) const
{
  G4THitsMap<G4double>* hitsCollection 
    = static_cast<G4THitsMap<G4double>*>(
        anEvent->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("D1EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double D1EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0;
  std::map<G4int, G4double*>::iterator it;
  for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
    sumValue += *(it->second);
  }
  return sumValue;  
}  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void D1EventAction::PrintEventStatistics(
                            G4double sha1Edep, G4double sha1TrackLength, 
                          G4double sha2Edep, G4double sha2TrackLength) const
{
  // Print event statistics
  //
  G4cout
     << "   Shape1: total energy: " 
     << std::setw(7) << G4BestUnit(sha1Edep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(sha1TrackLength, "Length")
    << G4endl
     << "   Shape2: total energy: " 
     << std::setw(7) << G4BestUnit(sha2Edep, "Energy")
     << "       total track length: " 
     << std::setw(7) << G4BestUnit(sha2TrackLength, "Length")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void D1EventAction::BeginOfEventAction(const G4Event* anEvent)
{
//New event, add the user information object
  G4EventManager::
    GetEventManager()->SetUserInformation(new D1UserEventInformation);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  if(fShape1CollID<0)
    fShape1CollID=SDman->GetCollectionID("Shape1Collection");
  if(fShape2CollID<0)
    fShape2CollID=SDman->GetCollectionID("Shape2HitCollection");
  
  if(fRecorder)fRecorder->RecordBeginOfEvent(anEvent);


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void D1EventAction::EndOfEventAction(const G4Event* anEvent)
{
// Get hist collections IDs
  if ( fSha1EdepHCID == -1 ) {
    fSha1EdepHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Shape1/Edep");
    fSha2EdepHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Shape2/Edep");
     fSha1TrackLengthHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Shape1/TrackLength");
    fSha2TrackLengthHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("Shape2/TrackLength");
   

 D1UserEventInformation* eventInformation
    =(D1UserEventInformation*)anEvent->GetUserInformation();
 
  G4TrajectoryContainer* trajectoryContainer=anEvent->GetTrajectoryContainer();
 
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();

  // extract the trajectories and draw them
  if (G4VVisManager::GetConcreteInstance()){
    for (G4int i=0; i<n_trajectories; i++){
      D1Trajectory* trj = (D1Trajectory*)
        ((*(anEvent->GetTrajectoryContainer()))[i]);
      if(trj->GetParticleName()=="opticalphoton"){
        trj->SetForceDrawTrajectory(fForcedrawphotons);
        trj->SetForceNoDrawTrajectory(fForcenophotons);
      }
      trj->DrawTrajectory();
    }
  }
 
  D1Shape1HitsCollection* Shape1HC = 0;
  D1Shape2HitsCollection* Shape2HC = 0;
  G4HCofThisEvent* hitsCE = anEvent->GetHCofThisEvent();
 
  //Get the hit collections
  if(hitsCE){
    if(fShape1CollID>=0)Shape1HC = (D1Shape1HitsCollection*)(hitsCE->GetHC(fShape1CollID));
    if(fShape2CollID>=0)Shape2HC = (D1Shape2HitsCollection*)(hitsCE->GetHC(fShape2CollID));
  }

if(Shape1HC){
    G4ThreeVector reconPos(0.,0.,0.);
    G4int Shape1s=Shape1HC->entries();
    //Gather info from all PMTs
    for(G4int i=0;i<Shape1s;i++){
      eventInformation->IncHitCount((*Shape1HC)[i]->GetPhotonCount());
      reconPos+=(*Shape1HC)[i]->GetShape1Pos()*(*Shape1HC)[i]->GetPhotonCount();
      if((*Shape1HC)[i]->GetPhotonCount()>=fShape1Threshold){
        eventInformation->IncShape1SAboveThreshold();
      }
      else{//wasnt above the threshold, turn it back off
        (*Shape1HC)[i]->SetDrawit(false);
      }
    }
 
    if(eventInformation->GetHitCount()>0){//dont bother unless there were hits
      reconPos/=eventInformation->GetHitCount();
      if(fVerbose>0){
        G4cout << "\tReconstructed position of hits in D1: "
               << reconPos/mm << G4endl;
      }
      eventInformation->SetReconPos(reconPos);
    }
    Shape1HC->DrawAllHits();
  }

  if(fVerbose>0){
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit Shape1s in this event : "
           << eventInformation->GetHitCount() << G4endl;
    G4cout << "\tNumber of Shape1s above threshold("<<fShape1Threshold<<") : "
           << eventInformation->GetShape1SAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
           << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
           << eventInformation->GetPhotonCount_Ceren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
           << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
           << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
           << G4endl;
    G4cout << "Unacounted for photons in this event : "
           << (eventInformation->GetPhotonCount_Scint() +
               eventInformation->GetPhotonCount_Ceren() -
               eventInformation->GetAbsorptionCount() -
               eventInformation->GetHitCount() -
               eventInformation->GetBoundaryAbsorptionCount())
           << G4endl;
  }

if(Shape2HC){
    G4ThreeVector reconPos(0.,0.,0.);
    G4int Shape2s=Shape2HC->entries();
    //Gather info from all PMTs
    for(G4int i=0;i<Shape2s;i++){
      eventInformation->IncHitCount((*Shape2HC)[i]->GetPhotonCount());
      reconPos+=(*Shape2HC)[i]->GetShape2Pos()*(*Shape2HC)[i]->GetPhotonCount();
      if((*Shape2HC)[i]->GetPhotonCount()>=fShape2Threshold){
        eventInformation->IncShape2SAboveThreshold();
      }
      else{//wasnt above the threshold, turn it back off
        (*Shape2HC)[i]->SetDrawit(false);
      }
    }
 
    if(eventInformation->GetHitCount()>0){//dont bother unless there were hits
      reconPos/=eventInformation->GetHitCount();
      if(fVerbose>0){
        G4cout << "\tReconstructed position of hits in D1: "
               << reconPos/mm << G4endl;
      }
      eventInformation->SetReconPos(reconPos);
    }
    Shape2HC->DrawAllHits();
  }

  if(fVerbose>0){
    //End of event output. later to be controlled by a verbose level
    G4cout << "\tNumber of photons that hit Shape2s in this event : "
           << eventInformation->GetHitCount() << G4endl;
    G4cout << "\tNumber of Shape2s above threshold("<<fShape2Threshold<<") : "
           << eventInformation->GetShape2SAboveThreshold() << G4endl;
    G4cout << "\tNumber of photons produced by scintillation in this event : "
           << eventInformation->GetPhotonCount_Scint() << G4endl;
    G4cout << "\tNumber of photons produced by cerenkov in this event : "
           << eventInformation->GetPhotonCount_Ceren() << G4endl;
    G4cout << "\tNumber of photons absorbed (OpAbsorption) in this event : "
           << eventInformation->GetAbsorptionCount() << G4endl;
    G4cout << "\tNumber of photons absorbed at boundaries (OpBoundary) in "
           << "this event : " << eventInformation->GetBoundaryAbsorptionCount()
           << G4endl;
    G4cout << "Unacounted for photons in this event : "
           << (eventInformation->GetPhotonCount_Scint() +
               eventInformation->GetPhotonCount_Ceren() -
               eventInformation->GetAbsorptionCount() -
               eventInformation->GetHitCount() -
               eventInformation->GetBoundaryAbsorptionCount())
           << G4endl;
  }




//If we have set the flag to save 'special' events, save here
  if(fSaveThreshold&&eventInformation->GetPhotonCount() <= fSaveThreshold)
    G4RunManager::GetRunManager()->rndmSaveThisEvent();
 
 if(fRecorder)fRecorder->RecordEndOfEvent(anEvent);
}

//}

// Get sum values from hits collections
  //
  G4double sha1Edep = GetSum(GetHitsCollection(fSha1EdepHCID, anEvent));
  G4double sha2Edep = GetSum(GetHitsCollection(fSha2EdepHCID, anEvent));

  G4double sha1TrackLength 
    = GetSum(GetHitsCollection(fSha1TrackLengthHCID, anEvent));
  G4double sha2TrackLength 
    = GetSum(GetHitsCollection(fSha2TrackLengthHCID, anEvent));
  



// get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

// fill histograms
  //  
  analysisManager->FillH1(1, sha1Edep);
  analysisManager->FillH1(2, sha2Edep);
  analysisManager->FillH1(3, sha1TrackLength);
  analysisManager->FillH1(4, sha2TrackLength);
  analysisManager->FillH1(5, fShape1Threshold);
  analysisManager->FillH1(6, fShape2Threshold);
 
// fill ntuple
  //
  analysisManager->FillNtupleDColumn(0, sha1Edep);
  analysisManager->FillNtupleDColumn(1, sha2Edep);
  analysisManager->FillNtupleDColumn(2, sha1TrackLength);
  analysisManager->FillNtupleDColumn(3, sha2TrackLength);
  analysisManager->FillNtupleDColumn(4, fShape1Threshold);
  analysisManager->FillNtupleDColumn(5, fShape2Threshold);
  analysisManager->AddNtupleRow();  



 //print per event (modulo n)
  //
  G4int eventID = anEvent->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
    PrintEventStatistics(sha1Edep, sha2Edep, sha1TrackLength,  sha2TrackLength);
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1EventAction::SetSaveThreshold(G4int save){
/*Sets the save threshold for the random number seed. If the number of photons
generated in an event is lower than this, then save the seed for this event
in a file called run###evt###.rndm
*/
  fSaveThreshold=save;
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");
  //  G4UImanager::GetUIpointer()->ApplyCommand("/random/setSavingFlag 1");
}

