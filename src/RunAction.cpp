#include "RunAction.h"
#include "G4Run.hh"
#include "TrackingAction.h"
#include "G4ParticleDefinition.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"
#include "G4CsvAnalysisManager.hh"
#include "OutputingStrategy.h"

using AnalysisManager = G4CsvAnalysisManager;

namespace  {

    void PrintParticles(std::map<const G4ParticleDefinition*, int>& container) {
        std::map<const G4ParticleDefinition*, int>::iterator it;
        for(it = container.begin(); it != container.end(); it ++) {
            G4cout << "N " << it->first->GetParticleName() << " : " << it->second << G4endl;
        }
    }

}

RunAction::RunAction(OutputingStrategy* outputingStrategy): outputStrategy(outputingStrategy) {
    trackingAction = nullptr;
    alreadyInitialized = false;
}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run* run) {  
  if(isMaster) // WARNING : in sequential mode, isMaster == true    
    BeginMaster(run);    
  else 
    BeginWorker(run);
}

void RunAction::EndOfRunAction(const G4Run* run) {
  if(isMaster)   
    EndMaster(run);
  else	
    EndWorker(run);
}

void RunAction::BeginMaster(const G4Run* run) {
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);
  if(sequential) {
    if(!alreadyInitialized){
        InitializeWorker(run);
    }
    outputStrategy->WriteHeader();
  }

}

void RunAction::BeginWorker(const G4Run* run) {
  if(!alreadyInitialized){
      InitializeWorker(run);
  }
  outputStrategy->WriteHeader();
}

void RunAction::EndMaster(const G4Run* run) {
  bool sequential = (G4RunManager::GetRunManager()->GetRunManagerType() == G4RunManager::sequentialRM);
  if(sequential)    
    EndWorker(run);    
}

void RunAction::EndWorker(const G4Run* run) {
  G4int nofEvents = run->GetNumberOfEvent();
  if ( nofEvents == 0 ) {
    G4cout << "NO EVENTS TREATED IN THIS RUN .... LEAVING"<< G4endl;
    return;
  }
  outputStrategy->Close();
  delete AnalysisManager::Instance();
  std::map<const G4ParticleDefinition*, int>& particlesCreatedInWorld = trackingAction->GetNParticlesCreatedInWorld();
  G4cout << "Number and type of particles created outside region \"Target\" :" << G4endl;
  PrintParticles(particlesCreatedInWorld);
  G4cout << "-----------------------------" << G4endl;
  std::map<const G4ParticleDefinition*, int>& particlesCreatedInTarget = trackingAction->GetNParticlesCreatedInTarget();
  G4cout << "Number and type of particles created in region \"Target\" :" << G4endl;
  PrintParticles(particlesCreatedInTarget);
}

void RunAction::InitializeWorker(const G4Run*) {
    if (trackingAction == 0) {
        auto* ptr = dynamic_cast<const TrackingAction*>(G4RunManager::GetRunManager()->GetUserTrackingAction());
        if(ptr == nullptr && isMaster == false) {
			G4ExceptionDescription exDescrption ;
            exDescrption << "TrackingAction is a null pointer. Has it been correctly initialized ?";
            G4Exception("RunAction::BeginOfRunAction", "RunAction", FatalException, exDescrption);
        } else {
           trackingAction = const_cast<TrackingAction*>(ptr);
        }
	}
    alreadyInitialized = true;
}

void RunAction::PrintRunInfo(const G4Run* run) {
    G4cout << "Run ID = " << run->GetRunID() << G4endl;
    G4cout << "Run type is = " << G4RunManager::GetRunManager()->GetRunManagerType() << G4endl;
    G4cout << "Event processed = " << run->GetNumberOfEventToBeProcessed() << G4endl;
    G4cout << "Number of events = " << run->GetNumberOfEvent() << G4endl;
}
