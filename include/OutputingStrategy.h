#ifndef OutputingStrategy_h
#define OutputingStrategy_h
#include "G4String.hh"
#include <unordered_map>

class G4Step;

class OutputingStrategy
{
public:
  OutputingStrategy();
  ~OutputingStrategy() = default;
  void WriteStep(const G4Step* step);
  void Close();
  void WriteHeader();
private:
    std::unordered_map<std::string, std::string> particleShorthandMap;
    std::unordered_map<std::string, std::string> processShorthandMap;
};

#endif
