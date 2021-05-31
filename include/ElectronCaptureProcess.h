// work deried from: V.Ivanchenko
#ifndef ElectronCaptureProcess_h
#define ElectronCaptureProcess_h

#include "G4VDiscreteProcess.hh"
#include "globals.hh"
#include "G4ParticleChangeForGamma.hh"

class G4Region;

class ElectronCaptureProcess : public G4VDiscreteProcess
{
public:
  ElectronCaptureProcess(const G4String& regName, G4double ekinlimit);
  virtual ~ElectronCaptureProcess() override;
  void SetKinEnergyLimit(G4double);
  void BuildPhysicsTable(const G4ParticleDefinition&) override;
  G4bool IsApplicable(const G4ParticleDefinition&) override;
  G4double PostStepGetPhysicalInteractionLength(const G4Track& track,
                                                G4double previousStepSize,
                                                G4ForceCondition* condition) override;
  G4VParticleChange* PostStepDoIt(const G4Track&, const G4Step&) override;
protected:
  G4double GetMeanFreePath(const G4Track&, G4double,G4ForceCondition*) override;
private:
  ElectronCaptureProcess(const ElectronCaptureProcess&);
  ElectronCaptureProcess& operator=(const ElectronCaptureProcess &right);

  G4double kinEnergyThreshold;
  G4String regionName;
  G4Region* region;
  G4ParticleChangeForGamma particleChange;
};


#endif

