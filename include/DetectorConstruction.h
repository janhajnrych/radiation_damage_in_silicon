#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4VisAttributes.hh"

class G4Region;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  DetectorConstruction();
  ~DetectorConstruction();
  G4VPhysicalVolume* Construct();
  G4Region* GetTargetRegion();                       
private:
  G4VPhysicalVolume* worldPhysVol;
  G4LogicalVolume*   worldLogicVol;
  G4Box*             worldBox;

  G4Material*        SiMat;
  G4Material*        gasMat;
  G4Region*          solidRegion;

  void DefineMaterials();

  G4VPhysicalVolume* ConstructDetector();     
};


#endif
