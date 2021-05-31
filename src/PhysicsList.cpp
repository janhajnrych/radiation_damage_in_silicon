// work deried from: V.Ivanchenko
#include "PhysicsList.h"
#include "G4SystemOfUnits.hh"
#include "G4MicroElecElastic.hh"
#include "G4MicroElecElasticModel.hh"
#include "G4MicroElecInelastic.hh"
#include "G4MicroElecInelasticModel.hh"
#include "G4LossTableManager.hh"
#include "G4EmConfigurator.hh"
#include "G4VEmModel.hh"
#include "G4DummyModel.hh"
#include "G4eIonisation.hh"
#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"
#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4BraggModel.hh"
#include "G4BraggIonModel.hh"
#include "G4BetheBlochModel.hh"
#include "G4UrbanMscModel.hh"
#include "G4MollerBhabhaModel.hh"
#include "G4IonFluctuations.hh"
#include "G4UniversalFluctuation.hh"
#include "ElectronCaptureProcess.h"
#include "G4UAtomicDeexcitation.hh"

namespace  {

void addElecronProc(G4ProcessManager* procManager){

    // standard multi scttering is active in the world
    G4eMultipleScattering* msc = new G4eMultipleScattering();
    msc->AddEmModel(1, new G4UrbanMscModel());
    procManager->AddProcess(msc, -1, 1, -1);

    // standard ionisation is active in the world
    G4eIonisation* eion = new G4eIonisation();
    procManager->AddProcess(eion, -1, 2, 2);

    // MicroElec elastic is not active in the world
    G4MicroElecElastic* theMicroElecElasticProcess = new G4MicroElecElastic("e-_G4MicroElecElastic");
    theMicroElecElasticProcess->SetEmModel(new G4DummyModel(),1);
    procManager->AddDiscreteProcess(theMicroElecElasticProcess);

    // MicroElec ionisation is not active in the world
    G4MicroElecInelastic* microelecioni = new G4MicroElecInelastic("e-_G4MicroElecInelastic");
    microelecioni->SetEmModel(new G4DummyModel(), 1);
    procManager->AddDiscreteProcess(microelecioni);

    // kill all electrons below threshold
    // Capture of low-energy e-
    ElectronCaptureProcess* ecap = new ElectronCaptureProcess("Target", 16.7*eV);
    procManager->AddDiscreteProcess(ecap);
}

void addProtonProc(G4ProcessManager* procManager){

    // standard multiscattering is active in the world
    G4hMultipleScattering* msc = new G4hMultipleScattering();
    msc->AddEmModel(1, new G4UrbanMscModel());
    procManager->AddProcess(msc, -1, 1, -1);

    // standard ionisation is active in the world
    G4hIonisation* hion = new G4hIonisation();
    procManager->AddProcess(hion, -1, 2, 2);

    // MicroElec ionisation is not active in the world
    G4MicroElecInelastic* microelecioni = new G4MicroElecInelastic("p_G4MicroElecInelastic");
    microelecioni->SetEmModel(new G4DummyModel(),1);
    microelecioni->SetEmModel(new G4DummyModel(),2);
    procManager->AddDiscreteProcess(microelecioni);
}

void addIonProc(G4ProcessManager* procManager){
    // standard multiscattering is active in the world
    G4hMultipleScattering* msc = new G4hMultipleScattering();
    msc->AddEmModel(1, new G4UrbanMscModel());
    procManager->AddProcess(new G4hMultipleScattering, -1, 1, -1);

    // standard ionisation is active in the world
    G4ionIonisation* hion = new G4ionIonisation();
    procManager->AddProcess(hion, -1, 2, 2);

    // MicroElec ionisation is not active in the world
    G4MicroElecInelastic* microelecioni = new G4MicroElecInelastic("ion_G4MicroElecInelastic");
    microelecioni->SetEmModel(new G4DummyModel(),1);
    microelecioni->SetEmModel(new G4DummyModel(),2);
    procManager->AddDiscreteProcess(microelecioni);
}

void defineExtraElectroMagElectronProcess(G4EmConfigurator* emConfig) {
    G4UrbanMscModel* msc =  new G4UrbanMscModel();
    msc->SetActivationLowEnergyLimit(100*MeV);
    emConfig->SetExtraEmModel("e-", "msc" , msc, "Target");
    G4VEmModel* mod;
    mod = new G4MollerBhabhaModel();
    mod->SetActivationLowEnergyLimit(100*MeV);
    emConfig->SetExtraEmModel("e-",  "eIoni",  mod, "Target", 0.0, 10*TeV, new G4UniversalFluctuation());

    // activate MicroElec processes
    mod = new G4MicroElecElasticModel();
    emConfig->SetExtraEmModel("e-", "e-_G4MicroElecElastic", mod, "Target", 0.0, 100*MeV);

    mod = new G4MicroElecInelasticModel();
    emConfig->SetExtraEmModel("e-", "e-_G4MicroElecInelastic", mod, "Target", 16.7*eV, 100*MeV);
}

void defineExtraElectroMagProtonProcess(G4EmConfigurator* emConfig) {
    G4VEmModel* mod;
    mod = new G4BraggModel();
    mod->SetActivationHighEnergyLimit(50*keV);
    emConfig->SetExtraEmModel("proton", "hIoni", mod, "Target", 0.0, 2*MeV, new G4IonFluctuations());

    mod = new G4BetheBlochModel();
    mod->SetActivationLowEnergyLimit(10*GeV);
    emConfig->SetExtraEmModel("proton", "hIoni", mod, "Target", 2*MeV, 10*TeV, new G4IonFluctuations());

    mod = new G4MicroElecInelasticModel();
    mod->SetActivationLowEnergyLimit(50*keV);
    emConfig->SetExtraEmModel("proton","p_G4MicroElecInelastic", mod, "Target", 0.0, 10*GeV);
    emConfig->SetExtraEmModel("proton","p_G4MicroElecInelastic", new G4DummyModel, "Target", 10*GeV, 10*TeV);
}

void defineExtraElectroMagIonProcess(G4EmConfigurator* emConfig){
    G4VEmModel* mod;
    mod = new G4BraggIonModel();
    mod->SetActivationHighEnergyLimit(50*keV);
    emConfig->SetExtraEmModel("GenericIon", "ionIoni", mod, "Target", 0.0, 2*MeV, new G4IonFluctuations());

    mod = new G4BetheBlochModel();
    mod->SetActivationLowEnergyLimit(10*GeV);
    emConfig->SetExtraEmModel("GenericIon", "ionIoni", mod, "Target", 2*MeV, 10*TeV, new G4IonFluctuations());

    mod = new G4MicroElecInelasticModel();
    mod->SetActivationLowEnergyLimit(50*keV);
    emConfig->SetExtraEmModel("GenericIon", "ion_G4MicroElecInelastic", mod, "Target", 0.0, 10*GeV);
    emConfig->SetExtraEmModel("GenericIon", "ion_G4MicroElecInelastic", new G4DummyModel, "Target", 10*GeV, 10*TeV);
}

G4VAtomDeexcitation* createDeexcitationProcess() {
    G4VAtomDeexcitation* deexcitaion = new G4UAtomicDeexcitation();
    deexcitaion->SetFluo(true);
    deexcitaion->SetAuger(true);
    deexcitaion->SetPIXE(true);
    deexcitaion->InitialiseForNewRun();
    return deexcitaion;
}

}

PhysicsList::PhysicsList():  G4VUserPhysicsList(){
  defaultCutValue = micrometer;
  cuts["gamma"] = defaultCutValue;
  cuts["e-"] = defaultCutValue;
  cuts["e+"] = defaultCutValue;
  cuts["proton"] = defaultCutValue;
  SetVerboseLevel(1);
}

PhysicsList::~PhysicsList() {}

void PhysicsList::ConstructParticle(){
  G4Gamma::GammaDefinition();
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4Proton::ProtonDefinition();
  G4GenericIon::GenericIonDefinition();
}

void PhysicsList::ConstructProcess(){
  AddTransportation();
  ConstructElectroMagneticProcess();
  ConstructGeneral();
}

void PhysicsList::ConstructElectroMagneticProcess() {
  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  
  while((*particleIterator)()) {
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "e-") {
      addElecronProc(pmanager);
    } else if ( particleName == "proton" ) {
        addProtonProc(pmanager);
    } else if (particleName == "GenericIon") { 
        addIonProc(pmanager);
    } 
  }

  G4EmConfigurator* emConfig = G4LossTableManager::Instance()->EmConfigurator();

  defineExtraElectroMagElectronProcess(emConfig);
  defineExtraElectroMagProtonProcess(emConfig);
  defineExtraElectroMagIonProcess(emConfig);

  G4LossTableManager::Instance()->SetAtomDeexcitation(createDeexcitationProcess());
}

void PhysicsList::ConstructGeneral() { }

void PhysicsList::SetCuts() {
  if (verboseLevel >0) {
    G4cout << "PhysicsList::SetCuts:";
    G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }  
  for (const auto& pair: cuts){
      SetCutValue(pair.second, pair.first);
  }
  if (verboseLevel>0) {
      DumpCutValuesTable();
  }
}
