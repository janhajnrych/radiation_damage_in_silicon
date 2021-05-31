#include "SteppingAction.h"
#include "OutputingStrategy.h"

SteppingAction::SteppingAction(OutputingStrategy* outputingStrategy): outputStrategy(outputingStrategy) {}

SteppingAction::~SteppingAction(){}

void SteppingAction::UserSteppingAction(const G4Step* step) {
   outputStrategy->WriteStep(step);
}    
