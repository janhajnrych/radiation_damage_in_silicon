// work deried from: V.Ivanchenko
#include "ElectronCaptureProcess.h"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4Electron.hh"

ElectronCaptureProcess::ElectronCaptureProcess(const G4String& regName, G4double ekinlim):
    G4VDiscreteProcess("eCapture", fElectromagnetic), kinEnergyThreshold(ekinlim),
    regionName(regName), region(nullptr) {
  if(regName == "" || regName == "world") { 
    regionName = "DefaultRegionForTheWorld";
  }
  pParticleChange = &particleChange;
}

ElectronCaptureProcess::~ElectronCaptureProcess() {}

void ElectronCaptureProcess::SetKinEnergyLimit(G4double val) {
  kinEnergyThreshold = val;
  if(verboseLevel > 0) {
    G4cout << "### G4ElectronCapture: Tracking cut E(MeV) = " << kinEnergyThreshold/MeV << G4endl;
  }
}

void ElectronCaptureProcess::BuildPhysicsTable(const G4ParticleDefinition&) {
  region = (G4RegionStore::GetInstance())->GetRegion(regionName);
  if(region && verboseLevel > 0) {
    G4cout << "ElectronCapture: Tracking cut E(MeV) = "
	   << kinEnergyThreshold/MeV << " is assigned to " << regionName 
	   << G4endl;
  }
}

G4bool ElectronCaptureProcess::IsApplicable(const G4ParticleDefinition&) {
  return true;
}

G4double ElectronCaptureProcess::PostStepGetPhysicalInteractionLength(const G4Track& aTrack, G4double, G4ForceCondition* condition) {
  *condition = NotForced;
  G4double limit = DBL_MAX; 
  if(region && aTrack.GetVolume()->GetLogicalVolume()->GetRegion() == region && aTrack.GetKineticEnergy() < kinEnergyThreshold) {
    { limit = 0.0; }
  }
  return limit;
}

G4VParticleChange* ElectronCaptureProcess::PostStepDoIt(const G4Track& aTrack, const G4Step&) {
  pParticleChange->Initialize(aTrack);
  pParticleChange->ProposeTrackStatus(fStopAndKill);
  pParticleChange->ProposeLocalEnergyDeposit(aTrack.GetKineticEnergy());
  particleChange.SetProposedKineticEnergy(0.0);
  return pParticleChange;
}

G4double ElectronCaptureProcess::GetMeanFreePath(const G4Track&,G4double, G4ForceCondition*) {
  return DBL_MAX;
}


