#include "DetectorConstruction.h"
#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"


namespace  {

    G4Material* makeAir() {
         G4NistManager * man = G4NistManager::Instance();
        G4Element* elN = man->FindOrBuildElement(7);
        G4Element* elO = man->FindOrBuildElement(8);
        auto density = 1.204*mg/cm3; // 20 deg C
        G4Material* Air = new G4Material("Air", density, 2);
        Air->AddElement(elN, 70.0*perCent);
        Air->AddElement(elO, 30.0*perCent);
        return Air;
    }
}


DetectorConstruction::DetectorConstruction():worldPhysVol(nullptr), worldLogicVol(nullptr), worldBox(nullptr){}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct(){
  DefineMaterials();
  return ConstructDetector();
}

void DetectorConstruction::DefineMaterials() {
  G4NistManager * man = G4NistManager::Instance();
  G4Material * Si = man->FindOrBuildMaterial("G4_Si");
  gasMat = makeAir();
  SiMat = Si;
}

G4Region* DetectorConstruction::GetTargetRegion()  {
    return solidRegion;
}

G4VPhysicalVolume* DetectorConstruction::ConstructDetector() {
    G4ThreeVector targetSize(0.5*um, 0.5*um, 0.5*um);
    G4ThreeVector worldSize(1*um, 1*um, 1*um);
    worldBox = new G4Box("World", worldSize.x()/2, worldSize.y()/2, worldSize.z()/2);
    worldLogicVol = new G4LogicalVolume(worldBox, gasMat, "World");
    worldPhysVol = new G4PVPlacement(nullptr, G4ThreeVector(), "World", worldLogicVol, nullptr, false, 0);
    G4Box* targetSolid = new G4Box("Target", targetSize.x()/2, targetSize.y()/2, targetSize.z()/2);
    G4LogicalVolume* logicTarget = new G4LogicalVolume(targetSolid, SiMat, "Target");
    new G4PVPlacement(nullptr, G4ThreeVector(), "Target", logicTarget, worldPhysVol, false, 0);
    G4VisAttributes* worldVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    worldVisAtt->SetVisibility(true);
    worldLogicVol->SetVisAttributes(worldVisAtt);
    G4VisAttributes* worldVisAtt1 = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
    worldVisAtt1->SetVisibility(true);
    logicTarget->SetVisAttributes(worldVisAtt1);
    solidRegion = new G4Region("Target");
    G4ProductionCuts* cuts = new G4ProductionCuts();
    const G4double defCut = nm;
    cuts->SetProductionCut(defCut, "gamma");
    cuts->SetProductionCut(defCut, "e-");
    cuts->SetProductionCut(defCut, "e+");
    cuts->SetProductionCut(defCut, "proton");
    solidRegion->SetProductionCuts(cuts);
    solidRegion->AddRootLogicalVolume(logicTarget);
    return worldPhysVol;
}
