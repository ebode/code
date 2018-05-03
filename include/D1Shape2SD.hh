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
// $Id: LXePMTSD.hh 73915 2013-09-17 07:32:26Z gcosmo $
//
/// \file optical/LXe/include/LXePMTSD.hh
/// \brief Definition of the LXePMTSD class
//
//
#ifndef D1Shape2SD_h
#define D1Shape2SD_h 1

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "D1Shape2Hit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class D1Shape2SD : public G4VSensitiveDetector
{

  public:

    D1Shape2SD(G4String name);
    virtual ~D1Shape2SD();
 
    virtual void Initialize(G4HCofThisEvent* );
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* );
 
    //A version of processHits that keeps aStep constant
    G4bool ProcessHits_constStep(const G4Step* ,
                                 G4TouchableHistory* );
    virtual void EndOfEvent(G4HCofThisEvent* );
    virtual void clear();
    void DrawAll();
    void PrintAll();
 
    //Initialize the arrays to store pmt possitions
    inline void InitShape2s(G4int nShape2s){
      if(fShape2PositionsX)delete fShape2PositionsX;
      if(fShape2PositionsY)delete fShape2PositionsY;
      if(fShape2PositionsZ)delete fShape2PositionsZ;
      fShape2PositionsX=new G4DataVector(nShape2s);
      fShape2PositionsY=new G4DataVector(nShape2s);
      fShape2PositionsZ=new G4DataVector(nShape2s);
    }

    //Store a pmt position
    void SetShape2Positions(const std::vector<G4ThreeVector>& positions);

  private:

    D1Shape2HitsCollection* fShape2HitCollection;

    G4DataVector* fShape2PositionsX;
    G4DataVector* fShape2PositionsY;
    G4DataVector* fShape2PositionsZ;
};

#endif
