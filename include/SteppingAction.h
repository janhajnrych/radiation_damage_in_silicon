#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4UserSteppingAction.hh"

class RunAction;
class DetectorConstruction;
class PrimaryGeneratorAction;
class OutputingStrategy;

class SteppingAction : public G4UserSteppingAction
{
public:

  SteppingAction(OutputingStrategy* outputingStrategy);
  ~SteppingAction();
  void UserSteppingAction(const G4Step*);
private:
    OutputingStrategy* outputStrategy;
};
#endif
