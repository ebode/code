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
// $Id: D1RunAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file D1RunAction.cc
/// \brief Implementation of the B1RunAction class

       
#include "D1RunAction.hh"
#include "D1Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "D1RecorderBase.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1RunAction::D1RunAction(D1RecorderBase* r)
: G4UserRunAction(), fRecorder(r) 
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in B4Analysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Create directories 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);

  // Book histograms, ntuple
  //
  
  // Creating histograms
  analysisManager->CreateH1("1","Edep in Shape1", 100, 0., 2000*MeV, "MeV");
  analysisManager->CreateH1("2","Energy deposited in the light guide", 100, 0., 10*MeV, "MeV");
  analysisManager->CreateH1("3","trackL in Shape1", 100, 0., 75*cm, "cm" );
  analysisManager->CreateH1("4","trackL in the light guide", 100, 0., 50*cm, "cm");  
  analysisManager->CreateH1("5","Nofsecondary  in Shape1", 100, 0., 100 );
  analysisManager->CreateH1("6","NoSecondary in the light guide", 100, 0., 50);  
 


  // Creating ntuple
  //
  analysisManager->CreateNtuple("D1", "Edep and TrackL");
  analysisManager->CreateNtupleDColumn("ESha1");
  analysisManager->CreateNtupleDColumn("ESha2");
  analysisManager->CreateNtupleDColumn("LSha1");
  analysisManager->CreateNtupleDColumn("LSha2");
  analysisManager->CreateNtupleDColumn("NSha1");
  analysisManager->CreateNtupleDColumn("NSha2");
  analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1RunAction::~D1RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1RunAction::BeginOfRunAction(const G4Run* aRun)
{
 
     if(fRecorder)fRecorder->RecordBeginOfRun(aRun);

//  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
//inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  G4String fileName = "D1";
  analysisManager->OpenFile(fileName);
}

void D1RunAction::EndOfRunAction(const G4Run* aRun)
{
  

if(fRecorder)fRecorder->RecordEndOfRun(aRun);


 // G4int nbEvents = aRun->GetNumberOfEvent();


//G4double Opticalphoton = (G4double)nOpticalphoton/nbEvents;

   // print histogram statistics
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
 


 if ( analysisManager->GetH1(1) ) {
    G4cout << G4endl << " ----> print histograms statistic ";
    if(isMaster) {
      G4cout << "for the entire run " << G4endl << G4endl; 
    }
    else {
      G4cout << "for the local thread " << G4endl << G4endl; 
    }
    
    G4cout << " ESha1 : mean = " 
       << G4BestUnit(analysisManager->GetH1(1)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(1)->rms(),  "Energy") << G4endl;
  G4cout << " ESha2 : mean = " 
       << G4BestUnit(analysisManager->GetH1(2)->mean(), "Energy") 
       << " rms = " 
       << G4BestUnit(analysisManager->GetH1(2)->rms(),  "Energy") << G4endl; 
 
   G4cout << " LSha1 : mean = " 
      << G4BestUnit(analysisManager->GetH1(3)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(3)->rms(),  "Length") << G4endl;

    G4cout << " LSha2 : mean = " 
      << G4BestUnit(analysisManager->GetH1(4)->mean(), "Length") 
      << " rms = " 
      << G4BestUnit(analysisManager->GetH1(4)->rms(),  "Length") << G4endl;
     

}


  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();

}

