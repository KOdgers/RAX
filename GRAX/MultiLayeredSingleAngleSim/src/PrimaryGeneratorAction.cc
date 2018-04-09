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
/// \file radioactivedecay/rdecay01/src/PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class
//
//
// $Id: PrimaryGeneratorAction.cc 78307 2013-12-11 10:55:57Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "DetectorConstruction.hh"

#include <iomanip>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
ofstream Angfile("Angles.txt");

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

//  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
//  G4ParticleDefinition* particle
//                    = particleTable->FindParticle("geantino");
//  fParticleGun->SetParticleDefinition(particle);
//  fParticleGun->SetParticleEnergy(0*eV);
//  fParticleGun->SetParticlePosition(G4ThreeVector(0.,-0.25*cm,0.));
//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));

//  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
//  G4ParticleDefinition* particle
//                    = particleTable->FindParticle("gamma");
//  fParticleGun->SetParticleDefinition(particle);
//  fParticleGun->SetParticleEnergy(661*keV);
//  fParticleGun->SetParticlePosition(G4ThreeVector(0.,-25.*cm,0.));
//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));


  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
//  G4ParticleDefinition* particle
//                    = particleTable->FindParticle("e-");
//  fParticleGun->SetParticleDefinition(particle);
//  fParticleGun->SetParticleEnergy(1*MeV);
fParticleGun->SetParticlePosition(G4ThreeVector(-0*cm,-.05*mm,0));




//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(.3,.7,0));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

//  G4double     pi  = 3.14159265358979323846;
//  G4double cosTheta =.2+.8*G4UniformRand(), phi = 2*pi*G4UniformRand();
//  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
//  G4double ux = sinTheta*std::cos(phi),
//           uz = sinTheta*std::sin(phi),
//           uy = cosTheta;
//  Angfile<<cosTheta<<endl;
//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

  G4double Energy = 950*G4UniformRand();
  Energy = (50+Energy);
  G4int IntEnergy = static_cast<int>(Energy);
//  G4double Energy = 200;
  fParticleGun->SetParticleEnergy(IntEnergy*keV) ;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,1,0));
  Angfile<<IntEnergy<<endl;
  fParticleGun->GeneratePrimaryVertex(anEvent);





//  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
//  fParticleGun->GeneratePrimaryVertex(anEvent);

//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(.3,.7,0));

//fParticleGun->SetParticleMomentumDirection(G4ThreeVector(.3,.7,0));
//fParticleGun->GeneratePrimaryVertex(anEvent);




}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
