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
// $Id: D1ActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file D1ActionInitialization.cc
/// \brief Implementation of the B1ActionInitialization class

#include "D1ActionInitialization.hh"
#include "D1PrimaryGeneratorAction.hh"
#include "D1RunAction.hh"
#include "D1EventAction.hh"
#include "D1TrackingAction.hh"
#include "D1SteppingAction.hh"
#include "D1StackingAction.hh"
#include "D1SteppingVerbose.hh"

#include "D1RecorderBase.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1ActionInitialization::D1ActionInitialization(D1RecorderBase* recorder)
 :G4VUserActionInitialization(), fRecorder(recorder) 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1ActionInitialization::~D1ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1ActionInitialization::BuildForMaster() const
{
   SetUserAction(new D1RunAction(fRecorder));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1ActionInitialization::Build() const
{
  SetUserAction(new D1PrimaryGeneratorAction);
/*
  D1RunAction* runAction = new D1RunAction;
  SetUserAction(runAction);
  
  D1EventAction* eventAction = new D1EventAction();
  SetUserAction(eventAction);
 
*/
  SetUserAction(new D1StackingAction());

  SetUserAction(new D1RunAction(fRecorder));
  SetUserAction(new D1EventAction(fRecorder));
  SetUserAction(new D1TrackingAction(fRecorder));
  SetUserAction(new D1SteppingAction(fRecorder)); 
  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



G4VSteppingVerbose* D1ActionInitialization::InitializeSteppingVerbose() const
{
  return new D1SteppingVerbose();
}


