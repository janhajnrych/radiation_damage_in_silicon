#include "PrimaryGeneratorAction.h"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  // default gun parameters
  fParticleGun->SetParticleEnergy(10.*keV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,-0.4*um));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
