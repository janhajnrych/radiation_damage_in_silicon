#include "OutputingStrategy.h"
#include "G4Step.hh"
#include "G4CsvAnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"

namespace  {

G4String mapName(const G4String& inputName, const std::unordered_map<std::string, std::string>& nameMap) {
    if (nameMap.count(inputName) != 0)
        return nameMap.at(inputName);
    return inputName;
}

}

using AnalysisManager = G4CsvAnalysisManager;

OutputingStrategy::OutputingStrategy() {
    particleShorthandMap["proton"] = "H+";
    processShorthandMap["p_G4MicroElecInelastic"]= "inelastic";
    processShorthandMap["e-_G4MicroElecInelastic"]= "inelastic";
    processShorthandMap["p_G4MicroElecElastic"]= "elastic";
    processShorthandMap["e-_G4MicroElecElastic"]= "elastic";
    processShorthandMap["eCapture"]= "capture";
}

void OutputingStrategy::WriteStep(const G4Step* step) {
    G4double x,y,z,xp,yp,zp;
    G4String particleName = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    if (processName !="Transportation")  {
      x=step->GetPreStepPoint()->GetPosition().x()/nanometer;
      y=step->GetPreStepPoint()->GetPosition().y()/nanometer;
      z=step->GetPreStepPoint()->GetPosition().z()/nanometer;
      xp=step->GetPostStepPoint()->GetPosition().x()/nanometer;
      yp=step->GetPostStepPoint()->GetPosition().y()/nanometer;
      zp=step->GetPostStepPoint()->GetPosition().z()/nanometer;
      auto dx = x-xp;
      auto dy = y-yp;
      auto dz = z-zp;
      auto* analysisManager = AnalysisManager::Instance();
      analysisManager->FillNtupleSColumn(0, mapName(particleName, particleShorthandMap));
      analysisManager->FillNtupleSColumn(1, mapName(processName, processShorthandMap));
      analysisManager->FillNtupleDColumn(2, x);
      analysisManager->FillNtupleDColumn(3, y);
      analysisManager->FillNtupleDColumn(4, z);
      auto stepLen = std::sqrt(dx*dx+dy*dy+dz*dz);
      analysisManager->FillNtupleDColumn(5, stepLen);
      analysisManager->FillNtupleDColumn(6, step->GetPreStepPoint()->GetKineticEnergy()/eV);
      analysisManager->FillNtupleDColumn(7, (step->GetPreStepPoint()->GetKineticEnergy() - step->GetPostStepPoint()->GetKineticEnergy())/eV );
      analysisManager->FillNtupleDColumn(8, step->GetTotalEnergyDeposit()/eV);
      analysisManager->AddNtupleRow();
    }
}

void OutputingStrategy::WriteHeader() {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;
    analysisManager->SetVerboseLevel(1);
    analysisManager->OpenFile("output");
    analysisManager->SetIsCommentedHeader(true);
    analysisManager->SetIsHippoHeader(false);
    analysisManager->CreateNtuple("collisions", "physics");
    analysisManager->CreateNtupleSColumn("particle");
    analysisManager->CreateNtupleSColumn("process");
    analysisManager->CreateNtupleDColumn("x [nm]");
    analysisManager->CreateNtupleDColumn("y [nm]");
    analysisManager->CreateNtupleDColumn("z [nm]");
    analysisManager->CreateNtupleDColumn("stepLength [nm]");
    analysisManager->CreateNtupleDColumn("initialKinEnergy [eV]");
    analysisManager->CreateNtupleDColumn("kinEnergyDiff [eV]");
    analysisManager->CreateNtupleDColumn("totEnergyDeposit [eV]");
    analysisManager->FinishNtuple();
}

void OutputingStrategy::Close() {
    AnalysisManager* analysisManager = AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
