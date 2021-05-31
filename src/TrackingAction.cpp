#include "TrackingAction.h"
#include "G4Track.hh"
#include "G4VSolid.hh"
#include "G4Region.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "DetectorConstruction.h"

using namespace std;

TrackingAction::TrackingAction(DetectorConstruction* detector)
{
    fDetector = detector;
    fTargetRegion = 0;
}

TrackingAction::~TrackingAction()
{
    fDetector = 0;
    fTargetRegion = 0;
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    const G4ParticleDefinition* particleDefinition = track->GetParticleDefinition();

    if(particleDefinition == G4Electron::Definition() || particleDefinition == G4Gamma::Definition())
    {
        if(fTargetRegion == 0) // target region is initialized after detector construction instantiation
        {
            fTargetRegion = fDetector->GetTargetRegion();
        }

        const G4ThreeVector& position = track->GetPosition();

        int N =  fTargetRegion->GetNumberOfRootVolumes();
        std::vector<G4LogicalVolume*>::iterator it_logicalVolumeInRegion =
                fTargetRegion->GetRootLogicalVolumeIterator();

        bool inside_target = false;

        for(int i = 0; i < N ; i++, it_logicalVolumeInRegion++)
        {
            EInside test_status = (*it_logicalVolumeInRegion)->GetSolid()->Inside(position) ;
            if(test_status == kInside)
            {
                inside_target = true;
                break;
            }
            /*
            else if (test_status == kSurface)
            {
            }
            */
        }

        if(inside_target == true)
        {
            fNParticleInTarget[particleDefinition]++;
        }
        else
        {
            fNParticleInWorld[particleDefinition]++;
        }
    }
}

