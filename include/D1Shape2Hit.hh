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
// $Id: LXePMTHit.hh 72250 2013-07-12 08:59:26Z gcosmo $
//
/// \file optical/LXe/include/LXePMTHit.hh
/// \brief Definition of the LXePMTHit class
//
//
#ifndef D1Shape2Hit_h
#define D1Shape2Hit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

#include "tls.hh"

class G4VTouchable;

class D1Shape2Hit : public G4VHit
{
  public:
 
    D1Shape2Hit();
    virtual ~D1Shape2Hit();
    D1Shape2Hit(const D1Shape2Hit &right);

    const D1Shape2Hit& operator=(const D1Shape2Hit &right);
    G4int operator==(const D1Shape2Hit &right) const;

    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
 
    virtual void Draw();
    virtual void Print();

    inline void SetDrawit(G4bool b){fDrawit=b;}
    inline G4bool GetDrawit(){return fDrawit;}

    inline void IncPhotonCount(){fPhotons++;}
    inline G4int GetPhotonCount(){return fPhotons;}

    inline void SetShape2Number(G4int n) { fShape2Number = n; }
    inline G4int GetShape2Number() { return fShape2Number; }

    inline void SetShape2PhysVol(G4VPhysicalVolume* physVol){this->fPhysVol=physVol;}
    inline G4VPhysicalVolume* GetShape2PhysVol(){return fPhysVol;}

    inline void SetShape2Pos(G4double x,G4double y,G4double z){
      fPos=G4ThreeVector(x,y,z);
    }
 
    inline G4ThreeVector GetShape2Pos(){return fPos;}

  private:

    G4int fShape2Number;
    G4int fPhotons;
    G4ThreeVector fPos;
    G4VPhysicalVolume* fPhysVol;
    G4bool fDrawit;

};

typedef G4THitsCollection<D1Shape2Hit> D1Shape2HitsCollection;

extern G4ThreadLocal G4Allocator<D1Shape2Hit>* D1Shape2HitAllocator;

inline void* D1Shape2Hit::operator new(size_t){
  if(!D1Shape2HitAllocator)
      D1Shape2HitAllocator = new G4Allocator<D1Shape2Hit>;
  return (void *) D1Shape2HitAllocator->MallocSingle();
}

inline void D1Shape2Hit::operator delete(void *aHit){
  D1Shape2HitAllocator->FreeSingle((D1Shape2Hit*) aHit);
}

#endif
