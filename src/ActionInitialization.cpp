#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.h"
#include "SteppingAction.h"
#include "DetectorConstruction.h"
#include "TrackingAction.h"
#include "G4RunManager.hh"
#include "OutputingStrategy.h"

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction)
    : G4VUserActionInitialization(), detectorConstruction(detConstruction) {
    outputingStrategy = new OutputingStrategy();
}

ActionInitialization::~ActionInitialization(){
    delete outputingStrategy;
}

void ActionInitialization::BuildForMaster() const {
	// In MT mode, to be clearer, the RunAction class for the master thread might be
	// different than the one used for the workers.
	// This RunAction will be called before and after starting the
	// workers.
	// For more details, please refer to :
	// https://twiki.cern.ch/twiki/bin/view/Geant4/Geant4MTForApplicationDevelopers
	//
     RunAction* runAction= new RunAction(outputingStrategy);
     SetUserAction(runAction);
}

void ActionInitialization::Build() const {
	SetUserAction(new PrimaryGeneratorAction);
    TrackingAction* trackingAction = new TrackingAction(detectorConstruction);
	SetUserAction(trackingAction);
    RunAction* runAction= new RunAction(outputingStrategy);
	SetUserAction(runAction);
    SetUserAction(new SteppingAction(outputingStrategy));
}  
