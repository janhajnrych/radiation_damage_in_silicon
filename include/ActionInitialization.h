#ifndef ActionInitialization_h
#define ActionInitialization_h
#include "G4VUserActionInitialization.hh"

class DetectorConstruction;
class OutputingStrategy;

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(DetectorConstruction*);
    virtual ~ActionInitialization();
    virtual void BuildForMaster() const;
    virtual void Build() const;
  private:
    DetectorConstruction* detectorConstruction;
    OutputingStrategy* outputingStrategy;
};

#endif

    
