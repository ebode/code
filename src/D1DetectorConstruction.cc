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
// $Id: D1DetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file D1DetectorConstruction.cc
/// \brief Implementation of the A1DetectorConstruction class

#include "D1DetectorConstruction.hh"
//#include "G4VSensitiveDetector.hh"
#include "D1Shape1SD.hh"
#include "D1Shape2SD.hh"
#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"



#include "G4Material.hh"
#include "G4Element.hh"
//#include "D1ShapeSD.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
//#include "G4Cons.hh"
// #include "G4Orb.hh"
// #include "G4Sphere.hh"
// #include "G4Trd.hh"
#include "G4Tubs.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SDChargedFilter.hh"
#include "G4PSTrackLength.hh"
#include "G4VSDFilter.hh"
#include "G4PSNofSecondary.hh"
#include "G4SDParticleFilter.hh"
//#include "G4Timer.hh"
//#include "G4OpticalPhoton.hh"


#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"

#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"


G4bool D1DetectorConstruction::fShape1On = true;

G4bool D1DetectorConstruction::fShape2On = true;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1DetectorConstruction::D1DetectorConstruction()
: G4VUserDetectorConstruction(),logicShape1(0),logicShape2(0),
 fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

D1DetectorConstruction::~D1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* D1DetectorConstruction::Construct()
{ 
 G4double a, z, density;
 G4String name, symbol;
 G4int nel;

 
 //  ------------- Elements -------------
  a = 1.01*g/mole;
  G4Element* elH  = new G4Element(name="Hydrogen", symbol="H", z=1., a);
  a = 12.01*g/mole;
  G4Element* elC  = new G4Element(name="Carbon",   symbol="C", z=6., a);
  a = 16.00*g/mole;
  G4Element* elO  = new G4Element(name="Oxygen",   symbol="O", z=8., a);

// PMMA C5H8O2 ( Acrylic )
// -------------
    density = 1.19*g/cm3;
    G4Material* Acrylic = new G4Material(name="Acrylic", density, nel=3);
    Acrylic->AddElement(elC, 5);
    Acrylic->AddElement(elH, 8);
    Acrylic->AddElement(elO, 2);
  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 4*cm, env_sizeZ = 45*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_GLASS_LEAD");
   
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_GLASS_LEAD");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
 

   


                    
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
 
  
  //     
  // Shape1 
  //
  G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_GLASS_LEAD");
  G4ThreeVector pos1 = G4ThreeVector(0, 0.5*cm, -7*cm);
  // parallelepiped shape       
  //G4double shape_dxa = 12*cm, shape2_dxb = 12*cm;
  G4double shape1_dx = 4*cm, shape1_dy = 4*cm;
  G4double shape1_dz  = 45*cm;      
  
  G4Box* solidShape1 =    
    new G4Box("Shape1",                      //its name 
              0.5*shape1_dx, 0.5*shape1_dy, 0.5*shape1_dz); //its size
                
 logicShape1 =                         
    new G4LogicalVolume(solidShape1,         //its solid
                        shape1_mat,          //its material
                        "Shape1");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos1,                    //at position
                    logicShape1,             //its logical volume
                    "Shape",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                
//     
  // Shape 2
  //  
 G4Material* shape2_mat = G4Material::GetMaterial("Acrylic");
 G4ThreeVector pos2 = G4ThreeVector(0, 0, 16*cm);
        
// parallelepiped shape       
  //G4double shape_dxa = 12*cm, shape2_dxb = 12*cm;
 // G4double shape2_dx = 15*cm, shape2_dy = 15*cm;
//  G4double shape2_dz  = 1*cm; 
   
//G4Box* solidShape2 =    
//    new G4Box("Shape2",                      //its name 
 //             0.5*shape2_dx, 0.5*shape2_dy, 0.5*shape2_dz); //its size                   
 
G4double OutRadius = 1.8*cm;
G4double InRadius = 0.0*mm;
G4double Length = 1.0*cm;
G4double StartAngle = 0.;
G4double SpanAngle = 360.*deg;


G4Tubs* solidShape2 = new G4Tubs("Shape2", InRadius, OutRadius, Length/2., StartAngle, SpanAngle);
 
 logicShape2 =                         
    new G4LogicalVolume(solidShape2,         //its solid
                       shape2_mat,          //its material
                       "Shape2");           //its name
               
  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2",                //its name
                    logicEnv,                //its mother  volume
                    false,                   //no boolean operation
                   0,                       //copy number
                   checkOverlaps);          //overlaps checking
 

//Surface properties for the WLS slab
G4double ephoton[] = {7.0*eV, 7.14*eV};
  const G4int num = sizeof(ephoton)/sizeof(G4double);    

  //**Sphere surface properties
  G4double Shape1Reflectivity[] = {1.0, 1.0};
  assert(sizeof(Shape1Reflectivity) == sizeof(ephoton));
  G4double Shape1Efficiency[] = {0.0, 0.0};
  assert(sizeof(Shape1Efficiency) == sizeof(ephoton));
  G4MaterialPropertiesTable* Shape1PT = new G4MaterialPropertiesTable();
  Shape1PT->AddProperty("REFLECTIVITY", ephoton, Shape1Reflectivity, num);
  Shape1PT->AddProperty("EFFICIENCY", ephoton, Shape1Efficiency, num);
  G4OpticalSurface* OpShape1Surface =
    new G4OpticalSurface("Shape1Surface",unified,polished,dielectric_metal);
  OpShape1Surface->SetMaterialPropertiesTable(Shape1PT);

//**Sphere surface properties
  G4double Shape2Reflectivity[] = {1.0, 1.0};
  assert(sizeof(Shape2Reflectivity) == sizeof(ephoton));
  G4double Shape2Efficiency[] = {0.0, 0.0};
  assert(sizeof(Shape2Efficiency) == sizeof(ephoton));
  G4MaterialPropertiesTable* Shape2PT = new G4MaterialPropertiesTable();
  Shape2PT->AddProperty("REFLECTIVITY", ephoton, Shape2Reflectivity, num);
  Shape2PT->AddProperty("EFFICIENCY", ephoton, Shape2Efficiency, num);
  G4OpticalSurface* OpShape2Surface =
    new G4OpticalSurface("Shape1Surface",unified,polished,dielectric_metal);
  OpShape2Surface->SetMaterialPropertiesTable(Shape2PT);



  //always return the physical World
// Set Shape2 as scoring volume
  //
  fScoringVolume = logicShape2;

  //
  //always return the physical World
  //
  return physWorld;
}


//}  




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 void D1DetectorConstruction::ConstructSDandField()
{
 //G4String ShapeSDname = "/mydet/Shape";
 // D1SD * ShapeSD = new D1SD(ShapeSDname);
//  SetSensitiveDetector("Shape",ShapeSD);

G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  // 
  // Scorers
  //

  // declare Shape1 as a MultiFunctionalDetector scorer
  //  
  G4MultiFunctionalDetector* absDetector 
    = new G4MultiFunctionalDetector("Shape1");

  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");
  absDetector->RegisterPrimitive(primitive);
  
   primitive = new G4PSTrackLength("TrackLength");
  G4SDChargedFilter* charged = new G4SDChargedFilter("chargedFilter");
  primitive ->SetFilter(charged);
  absDetector->RegisterPrimitive(primitive);  

 //  primitive = new G4Timer("Time");
//  absDetector->RegisterPrimitive(primitive); 

  G4SDParticleFilter* photonFilter =
  new G4SDParticleFilter ("photonFilter", "opticalphoton" );
 

      
  // register number of secondary particles from optical photon
  primitive = new G4PSNofSecondary ( "nOpticalphoton" );
  primitive->SetFilter ( photonFilter );
  absDetector->RegisterPrimitive ( primitive );
  
  SetSensitiveDetector("Shape1",absDetector);


// declare Shape1 as a MultiFunctionalDetector scorer
  //  
  G4MultiFunctionalDetector* gapDetector 
    = new G4MultiFunctionalDetector("Shape2");

 
  primitive = new G4PSEnergyDeposit("Edep");
  gapDetector->RegisterPrimitive(primitive);

  primitive = new G4PSTrackLength("TrackLength");
  primitive ->SetFilter(charged);
  gapDetector->RegisterPrimitive(primitive);  
/*
 // primitive = new G4Timer("Time");
 // gapDetector->RegisterPrimitive(primitive);
*/
   primitive = new G4PSNofSecondary ( "nOpticalphoton" );
   primitive->SetFilter ( photonFilter );
   gapDetector->RegisterPrimitive ( primitive );


  
SetSensitiveDetector("Shape2",gapDetector);


 // sensitive detectors -----------------------------------------------------
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    G4String SDname;
    
    G4VSensitiveDetector* Shape1 
      = new D1Shape1SD(SDname="/Shape1");
    SDman->AddNewDetector(Shape1);
    logicShape1->SetSensitiveDetector(Shape1);

    G4VSensitiveDetector* Shape2 
      = new D1Shape2SD(SDname="/Shape2");
    SDman->AddNewDetector(Shape2);
    logicShape2->SetSensitiveDetector(Shape2);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1DetectorConstruction::SetDefaults() {
 
  fShape1On = true;
  
  fShape2On = true;  

 // G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1DetectorConstruction::SetShape1On(G4bool b) {
  fShape1On=b;
 // G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void D1DetectorConstruction::SetShape2On(G4bool b) {
  fShape2On=b;
 // G4RunManager::GetRunManager()->ReinitializeGeometry();
}

