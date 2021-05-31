#ifndef PhysicsList_h
#define PhysicsList_h
#include <unordered_map>
#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

class PhysicsList: public G4VUserPhysicsList
{
public:
  PhysicsList();
  virtual ~PhysicsList();
  void SetGammaCut(G4double);
  void SetElectronCut(G4double);
  void SetPositronCut(G4double);
  void SetProtonCut(G4double);
protected:
  void ConstructGeneral();
  void ConstructElectroMagneticProcess();
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();
private:
  std::unordered_map<std::string, G4double> cuts;
  
};
#endif
