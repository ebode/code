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
// $Id: LXePMTSD.cc 73915 2013-09-17 07:32:26Z gcosmo $
//
/// \file optical/LXe/src/LXePMTSD.cc
/// \brief Implementation of the LXePMTSD class
//
//
#include "D1Shape1SD.hh"
#include "D1Shape1Hit.hh"
#include "D1DetectorConstruction.hh"
#include "D1UserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1Shape1SD::D1Shape1SD(G4String name)
  : G4VSensitiveDetector(name),fShape1HitCollection(0),fShape1PositionsX(0)
  ,fShape1PositionsY(0),fShape1PositionsZ(0)
{
  collectionName.insert("Shape1HitCollection");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1Shape1SD::~D1Shape1SD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1Shape1SD::SetShape1Positions(const std::vector<G4ThreeVector>& positions)
{
  for (G4int i=0; i<G4int(positions.size()); ++i) {
    if(fShape1PositionsX)fShape1PositionsX->push_back(positions[i].x());
    if(fShape1PositionsY)fShape1PositionsY->push_back(positions[i].y());
    if(fShape1PositionsZ)fShape1PositionsZ->push_back(positions[i].z());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1Shape1SD::Initialize(G4HCofThisEvent* hitsCE){
  fShape1HitCollection = new D1Shape1HitsCollection
                      (SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, fShape1HitCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool D1Shape1SD::ProcessHits(G4Step* ,G4TouchableHistory* ){
  return false;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool D1Shape1SD::ProcessHits_constStep(const G4Step* aStep,
                                       G4TouchableHistory* ){

  //need to know if this is an optical photon
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;
 
  //User replica number 1 since photocathode is a daughter volume
  //to the pmt which was replicated
  G4int Shape1Number=
    aStep->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);
  G4VPhysicalVolume* physVol=
    aStep->GetPostStepPoint()->GetTouchable()->GetVolume(1);

  //Find the correct hit collection
  G4int n=fShape1HitCollection->entries();
  D1Shape1Hit* hit=NULL;
  for(G4int i=0;i<n;i++){
    if((*fShape1HitCollection)[i]->GetShape1Number()==Shape1Number){
      hit=(*fShape1HitCollection)[i];
      break;
    }
  }
 
  if(hit==NULL){//this pmt wasnt previously hit in this event
    hit = new D1Shape1Hit(); //so create new hit
    hit->SetShape1Number(Shape1Number);
    hit->SetShape1PhysVol(physVol);
    fShape1HitCollection->insert(hit);
    hit->SetShape1Pos((*fShape1PositionsX)[Shape1Number],(*fShape1PositionsY)[Shape1Number],
                   (*fShape1PositionsZ)[Shape1Number]);
  }

  hit->IncPhotonCount(); //increment hit for the selected pmt
 
  if(!D1DetectorConstruction::GetShape1On()){
    hit->SetDrawit(true);
    //If the sphere is disabled then this hit is automaticaly drawn
  }
  else{//sphere enabled
    D1UserTrackInformation* trackInfo=
      (D1UserTrackInformation*)aStep->GetTrack()->GetUserInformation();
    if(trackInfo->GetTrackStatus()&hitShape1)
      //only draw this hit if the photon has hit the sphere first
      hit->SetDrawit(true);
  }

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1Shape1SD::EndOfEvent(G4HCofThisEvent* ) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1Shape1SD::clear() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1Shape1SD::DrawAll() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1Shape1SD::PrintAll() {}
