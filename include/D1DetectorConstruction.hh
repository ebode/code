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
// $Id: D1DetectorConstruction.hh 69565 2013-05-08 12:35:31Z gcosmo $
//
/// \file D1DetectorConstruction.hh
/// \brief Definition of the B1DetectorConstruction class

#ifndef D1DetectorConstruction_h
#define D1DetectorConstruction_h 1


#include "D1Shape1SD.hh"
#include "D1Shape2SD.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
 #include "G4Cache.hh"

class G4VSensitiveDetector;
class G4VPhysicalVolume;
class G4LogicalVolume;
//class D1ShapeSD;

/// Detector construction class to define materials and geometry.

class D1DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    D1DetectorConstruction();
    virtual ~D1DetectorConstruction();
    
   virtual void ConstructSDandField();
    virtual G4VPhysicalVolume* Construct();
    
   void SetDefaults();
 

public:

 // G4VPhysicalVolume* Getshape()      {return fshape;};    

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

     void SetShape1On(G4bool );
    static G4bool GetShape1On(){return fShape1On;}

      void SetShape2On(G4bool );
    static G4bool GetShape2On(){return fShape2On;}


private:


    G4LogicalVolume* logicShape1;
    G4LogicalVolume* logicShape2;
  //   G4VPhysicalVolume*  fshape;
  //   G4Cache<D1ShapeSD*> fD1ShapeSD;      // pointer -> sensitive detector
   static G4bool fShape1On;

   static G4bool fShape2On;

 protected:
    G4LogicalVolume*  fScoringVolume;
  //  G4LogicalVolume* logicalShape;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

