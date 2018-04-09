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
/// \file radioactivedecay/rdecay01/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
// $Id: EventAction.cc 93256 2015-10-14 08:15:10Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"
#include "HistoManager.hh"
#include "Run.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
ofstream histfile("Energy.txt");
//ofstream airfile("EnergyAir.txt");


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
:G4UserEventAction(),
 fDecayChain(),fEvisTot(0.)
{
  // Set default print level 
  G4RunManager::GetRunManager()->SetPrintProgress(10000);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
 fDecayChain = " ";
 fEvisTot = 0.;
 Energy = 0.;
 Energy2 = 0.;
Last =0;
First=0;
Count=0;
EnergyArray ="";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{

 G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
 if(!HCE) return;
 G4SDManager* SDMan = G4SDManager::GetSDMpointer();
 PDCollisionID   = SDMan->GetCollectionID("Target/edep");



 G4THitsMap<G4double>* evtMap =
                    (G4THitsMap<G4double>*)(HCE->GetHC(PDCollisionID));


 std::map<G4int,G4double*>::iterator itr;
G4int copyNb =0;
Energy2 = 0;
Count = 0;
 for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++) {
   copyNb  = (itr->first);
   edep = *(itr->second);
	if (Count==0){
         First= copyNb;
	 EnergyArray.append(std::to_string(edep/keV));
         Count=Count+1;
       }else if(copyNb==First+1){
        EnergyArray.append(" , ");
        EnergyArray.append(std::to_string(edep/keV));
        First = copyNb;
       }else if(copyNb!=First+1){
            for (G4int i=0; i<(copyNb-First-1);i++){  // Modified to fix the insertion of zero deposit layers
                   EnergyArray.append(" , ");
                   EnergyArray.append(std::to_string(0));}
        EnergyArray.append(" , ");
        EnergyArray.append(std::to_string(edep/keV));
        First = copyNb;
}


 }

histfile<<EnergyArray<<endl;

// if (Energy > 1) histfile<<copyNb<<','<<Energy<<endl;




 G4int evtNb = evt->GetEventID(); 
 G4int printProgress = G4RunManager::GetRunManager()->GetPrintProgress();
 //printing survey
 //
 if (evtNb%printProgress == 0) 
   G4cout << "    End of event. Decay chain:" << fDecayChain 
          << G4endl << G4endl;
 
 //total visible energy
 G4AnalysisManager::Instance()->FillH1(9, fEvisTot);
 Run* run 
  = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
 run->EvisEvent(fEvisTot);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


