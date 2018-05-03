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
// $Id: LXeTrajectory.hh 72349 2013-07-16 12:13:16Z gcosmo $
//
/// \file optical/LXe/include/LXeTrajectory.hh
/// \brief Definition of the LXeTrajectory class
//
#ifndef D1Trajectory_h
#define D1Trajectory_h 1

#include "G4Trajectory.hh"
#include "G4Allocator.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "G4TrajectoryPoint.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class G4Polyline;                   // Forward declaration.

class D1Trajectory : public G4Trajectory
{
  public:

    D1Trajectory();
    D1Trajectory(const G4Track* aTrack);
    D1Trajectory(D1Trajectory &);
    virtual ~D1Trajectory();
 
    virtual void DrawTrajectory() const;
 
    inline void* operator new(size_t);
    inline void  operator delete(void*);

    void SetDrawTrajectory(G4bool b){fDrawit=b;}
    void Shape1(){fShape1=true;}
    void Shape2(){fShape2=true;}
    void SetForceDrawTrajectory(G4bool b){fForceDraw=b;}
    void SetForceNoDrawTrajectory(G4bool b){fForceNoDraw=b;}

  private:

    G4bool fShape1;
    G4bool fShape2;
    G4bool fDrawit;
    G4bool fForceNoDraw;
    G4bool fForceDraw;
    G4ParticleDefinition* fParticleDefinition;
};

extern G4ThreadLocal G4Allocator<D1Trajectory>* D1TrajectoryAllocator;

inline void* D1Trajectory::operator new(size_t)
{
  if(!D1TrajectoryAllocator)
      D1TrajectoryAllocator = new G4Allocator<D1Trajectory>;
  return (void*)D1TrajectoryAllocator->MallocSingle();
}

inline void D1Trajectory::operator delete(void* aTrajectory)
{
  D1TrajectoryAllocator->FreeSingle((D1Trajectory*)aTrajectory);
}

#endif
