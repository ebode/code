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
// $Id: LXeEventMessenger.cc 70256 2013-05-28 07:29:30Z gcosmo $
//
/// \file optical/LXe/src/LXeEventMessenger.cc
/// \brief Implementation of the LXeEventMessenger class
//
//
#include "D1EventMessenger.hh"
#include "D1EventAction.hh"

#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1EventMessenger::D1EventMessenger(D1EventAction* event)
 : fD1Event(event)
{
  fSaveThresholdCmd = new G4UIcmdWithAnInteger("/D1/saveThreshold",this);
  fSaveThresholdCmd->
SetGuidance("Set the photon count threshold for saving the random number seed");
  fSaveThresholdCmd->SetParameterName("photons",true);
  fSaveThresholdCmd->SetDefaultValue(4500);
  fSaveThresholdCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fVerboseCmd = new G4UIcmdWithAnInteger("/D1/eventVerbose",this);
  fVerboseCmd->SetGuidance("Set the verbosity of event data.");
  fVerboseCmd->SetParameterName("verbose",true);
  fVerboseCmd->SetDefaultValue(1);

  fShape1ThresholdCmd = new G4UIcmdWithAnInteger("/D1/Shape1Threshold",this);
  fShape1ThresholdCmd->SetGuidance("Set the Shape1Threshold (in # of photons)");

  fShape2ThresholdCmd = new G4UIcmdWithAnInteger("/D1/Shape2Threshold",this);
  fShape2ThresholdCmd->SetGuidance("Set the Shape2Threshold (in # of photons)");

  fForceDrawPhotonsCmd=new G4UIcmdWithABool("/D1/forceDrawPhotons",this);
  fForceDrawPhotonsCmd->SetGuidance("Force drawing of photons.");
  fForceDrawPhotonsCmd
    ->SetGuidance("(Higher priority than /D1/forceDrawNoPhotons)");

  fForceDrawNoPhotonsCmd=new G4UIcmdWithABool("/D1/forceDrawNoPhotons",this);
  fForceDrawNoPhotonsCmd->SetGuidance("Force no drawing of photons.");
  fForceDrawNoPhotonsCmd
    ->SetGuidance("(Lower priority than /D1/forceDrawPhotons)");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1EventMessenger::~D1EventMessenger(){
  delete fSaveThresholdCmd;
  delete fVerboseCmd;
  delete fShape1ThresholdCmd;
  delete fShape2ThresholdCmd;
  delete fForceDrawPhotonsCmd;
  delete fForceDrawNoPhotonsCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1EventMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
  if( command == fSaveThresholdCmd ){
    fD1Event->SetSaveThreshold(fSaveThresholdCmd->GetNewIntValue(newValue));
  }
  else if( command == fVerboseCmd ){
    fD1Event->SetEventVerbose(fVerboseCmd->GetNewIntValue(newValue));
  }
  else if( command == fShape1ThresholdCmd ){
    fD1Event->SetShape1Threshold(fShape1ThresholdCmd->GetNewIntValue(newValue));
  }
else if( command == fShape2ThresholdCmd ){
    fD1Event->SetShape2Threshold(fShape2ThresholdCmd->GetNewIntValue(newValue));
  }
  else if(command == fForceDrawPhotonsCmd){
    fD1Event->SetForceDrawPhotons(fForceDrawPhotonsCmd
                                  ->GetNewBoolValue(newValue));
  }
  else if(command == fForceDrawNoPhotonsCmd){
    fD1Event->SetForceDrawNoPhotons(fForceDrawNoPhotonsCmd
                                  ->GetNewBoolValue(newValue));
    G4cout<<"TEST"<<G4endl;
  }
}
