#ifndef RunAction_h
#define RunAction_h
#include "G4UserRunAction.hh"
#include "globals.hh"
#include <iostream>

class G4Run;
class TrackingAction;
class OutputingStrategy;

class RunAction : public G4UserRunAction
{
public:
  RunAction(OutputingStrategy* outputingStrategy);
  ~RunAction();
  void BeginOfRunAction(const G4Run*);
  void EndOfRunAction(const G4Run*);
private:
  void BeginMaster(const G4Run*);
  void EndMaster(const G4Run*);
  void InitializeWorker(const G4Run*);
  void BeginWorker(const G4Run*);
  void EndWorker(const G4Run*);
  void PrintRunInfo(const G4Run* run);
  TrackingAction* trackingAction;
  G4String fileName;
  bool alreadyInitialized;
  OutputingStrategy* outputStrategy;
};

#endif
