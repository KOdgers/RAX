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
/// \file radioactivedecay/rdecay01/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.cc 78307 2013-12-11 10:55:57Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




//Changed 5000+1000 nm to 1000+200 nm


#include "DetectorConstruction.hh"

#include "G4Tubs.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4RotationMatrix.hh"


//Sensitive Detector Setup
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction()
{
  fWorldSize = 4.*cm;
  WinnerRadius = 0.*cm;
  WouterRadius = 2.*cm;
  Whz = 3.*cm;
  startAngle = 0.*deg;
  spanningAngle = 360.*deg;


  //Photodiode Dimensions
  NumberOfLayers = 500;
  pdLength = .5*cm;
  pdThick = 2000*nm;
  DeadThick = 200*nm;
  pdAThick = .5*mm;
  SeparationLength= 2*mm;  //Half of the separation distance
  WorldBoxLength = pdThick*8*NumberOfLayers;






}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  //
  // define a material
  //   
  G4Material* Air =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"); 

  G4Material* Space =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  G4Material* Steel =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_STAINLESS-STEEL");

  G4Material* Am =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Am");

  G4Material* Si =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

  G4Material* PC =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYCARBONATE");

  G4Material* Cs =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Cs");

  G4Material* Co =
  G4NistManager::Instance()->FindOrBuildMaterial("G4_Co");






  
  //     
  // World
  //
//  G4Box*
//  solidWorld = new G4Box("World",                          //its name
//                   fWorldSize/2,fWorldSize/2,fWorldSize/2);//its size


//World

//////Cylindrical World
//  G4Tubs* solidWorld
//    = new G4Tubs("World",
//                 0.*cm,
//                 fWorldSize,
//                 Whz+2.*cm,
//                 startAngle,
//                 spanningAngle);


  G4Box* solidWorld
    = new G4Box("World",
                 pdLength,
                 WorldBoxLength,
                 pdLength);

  G4LogicalVolume*
  logicWorld = new G4LogicalVolume(solidWorld,             //its solid
                                   Space,                    //its material
                                   "World");               //its name
  G4VPhysicalVolume*
  physiWorld = new G4PVPlacement(0,                      //no rotation
                                 G4ThreeVector(),        //at (0,0,0)
                                 logicWorld,             //its logical volume
                                 "World",                //its name
                                 0,                      //its mother  volume
                                 false,                  //no boolean operation
                                 0);                     //copy number

// Solids


  // Photodiode Si -I
  G4Box* solidTarget
     = new G4Box("Target",
                 pdLength,
                 pdThick,
                 pdLength);

  // Photodiode Si -P
  G4Box* solidDead
     = new G4Box("P-Type",
                 pdLength,
                 DeadThick,
                 pdLength);





  // Photodiode Casing


  // Photodiode Si
//  G4Box* solidCase
//     = new G4Box("Case",
//                 pdLength,
//                 2*pdThick,
//                 pdLength);

// Logics

       //its name
  G4LogicalVolume*
  logicTarget = new G4LogicalVolume(solidTarget,             //its solid
                                   Si,                    //its material
                                   "Target");               //its name

  G4LogicalVolume*
  logicDead = new G4LogicalVolume(solidDead,             //its solid
                                   Si,                    //its material
                                   "Dead");               //its name

/*
  G4LogicalVolume*
  logicMask = new G4LogicalVolume(solidMask,             //its solid
                                   Steel,                    //its material
                                   "Mask");               //its name

  G4LogicalVolume*
  logicMaskHole = new G4LogicalVolume(solidMaskHole,             //its solid
                                   Air,                    //its material
                                   "Mask Hole");               //its name
*/
//  G4LogicalVolume*
//  logicAType = new G4LogicalVolume(solidAType,             //its solid
//                                   Air,                    //its material
//                                   "A-Type");               //its name


//  G4LogicalVolume*
//  logicCase = new G4LogicalVolume(solidCase,
//                                     PC,
//                                     "Case");




  // Physicals




//  G4VPhysicalVolume*
//  physiTarget = new G4PVPlacement(0,
//                                 G4ThreeVector(0*cm,SeparationLength,0),
//                                 logicTarget,
//                                 "Target",
//                                 logicWorld,
//                                 false,
//                                 3);

 G4double spacing = (pdThick+DeadThick);
new G4PVPlacement(0,
                                 G4ThreeVector(0*cm,0,0),
                                 logicTarget,
                                 "Target",
                                 logicWorld,
                                 false,
                                 0);
for(G4int i=1;i<NumberOfLayers;i++){
	new G4PVPlacement(0,
                                 G4ThreeVector(0*cm,2*spacing*i,0),
                                 logicTarget,
                                 "Target",
                                 logicWorld,
                                 false,
                                 i);


new G4PVPlacement(0,
                         G4ThreeVector(0*cm,(2*i-1)*spacing,0),
                         logicDead,
                         "Dead",
                         logicWorld,
                         false,
                         i);
}

/*

  G4VPhysicalVolume*
  physiPType = new G4PVPlacement(0,
                                 G4ThreeVector(0*cm,SeparationLength-pdThick-pdPThick,0),
                                 logicPType,
                                 "P-Type",
                                 logicWorld,
                                 false,
                                 0);
*/
/*
  G4VPhysicalVolume*
  physiMaskHole = new G4PVPlacement(0,
                                 G4ThreeVector(0,0,0),
                                 logicMaskHole,
                                 "MaskHole",
                                 logicMask,
                                 false,
                                 0);

  G4VPhysicalVolume*
  physiMask = new G4PVPlacement(0,
                                 G4ThreeVector(0,SeparationLength-pdThick-2*pdPThick-pdAThick,0),
                                 logicMask,
                                 "Mask",
                                 logicWorld,
                                 false,
                                 0);
*/







//logicSides->SetVisAttributes (G4VisAttributes::Invisible);
//logicEnds->SetVisAttributes (G4VisAttributes::Invisible);
logicWorld->SetVisAttributes (G4VisAttributes::Invisible);
                
  //
  //always return the physical World
  //  
  return physiWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    // declare Photodiode as a MultiFunctionalDetector scorer
    //
    G4MultiFunctionalDetector* PD = new G4MultiFunctionalDetector("Target");
    G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");
    PD->RegisterPrimitive(primitiv1);
    SetSensitiveDetector("Target",PD);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......






























