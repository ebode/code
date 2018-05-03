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
// $Id: B1EventAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file B1EventAction.hh
/// \brief Definition of the B1EventAction class

#ifndef D1EventAction_h
#define D1EventAction_h 1

#include "G4UserEventAction.hh"
#include "D1EventMessenger.hh"
#include "G4ThreeVector.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

class G4Event;
class D1RecorderBase;

class D1EventAction : public G4UserEventAction
{
  public:
    D1EventAction(D1RecorderBase*);
    virtual ~D1EventAction();

  public:
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
  
     void SetSaveThreshold(G4int );

    void SetEventVerbose(G4int v){fVerbose=v;}

    void SetShape1Threshold(G4int t){fShape1Threshold=t;}
    
    void SetShape2Threshold(G4int t){fShape2Threshold=t;}

    void SetForceDrawPhotons(G4bool b){fForcedrawphotons=b;}
    void SetForceDrawNoPhotons(G4bool b){fForcenophotons=b;}

    
  private:

  // methods
  G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
                                          const G4Event* anEvent) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
  void PrintEventStatistics(G4double sha1Edep, G4double sha1TrackLength,  
                            G4double sha2Edep, G4double sha2TrackLength) const; 
   //G4int calorimeterCollID;
   // G4int muonCollID;

// data members                   
  
   D1RecorderBase* fRecorder; 

  G4int  fSha1EdepHCID;
  G4int  fSha2EdepHCID;
  G4int  fSha1TrackLengthHCID;
  G4int  fSha2TrackLengthHCID;

  
  
   D1EventMessenger* fEventMessenger;

    G4int              fSaveThreshold;

    G4int              fShape1CollID;
    G4int              fShape2CollID;

    G4int              fVerbose;

    G4int              fShape1Threshold;

    G4int              fShape2Threshold;

    G4bool fForcedrawphotons;
    G4bool fForcenophotons;
 };

#endif
    
