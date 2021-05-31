#include "G4Types.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "ActionInitialization.h"
#include "DetectorConstruction.h"
#include "PhysicsList.h"

int main(int argc,char** argv) {
  G4UIExecutive* session = nullptr;
  if (argc==1) {
      session = new G4UIExecutive(argc, argv);
  }

  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  auto* runManager = G4RunManagerFactory::CreateRunManager();

  G4int nThreads = 4;
  runManager->SetNumberOfThreads(nThreads);
  
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(new PhysicsList);
  runManager->SetUserInitialization(new ActionInitialization(detector));
  runManager->Initialize();
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc==1) {
    UImanager->ApplyCommand("/control/execute run.mac");
    session->SessionStart();
    delete session;
  } else {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  delete visManager;
  delete runManager;
  return 0;
}

