#include "command.hpp"

namespace muhamadiarov
{
  static Robot globalRobot; 

  Graph& getGraph(GraphTable& graphs, std::string name)
  {
    if (!graphs.has(name))
    {
      throw std::runtime_error("getGraph:Graph not found");
    }
    return graphs.get(name);
  }
}

void muh::makeRobot(std::istream& in, GraphTable&)
{
  double maxEnergy, recoveryRate, speed;
  if (!(in >> maxEnergy >> recoveryRate >> speed))
  {
    throw std::runtime_error("makeRobot: error input");
  }
  globalRobot = Robot(maxEnergy, recoveryRate, speed);
}

void muh::setRobot(std::istream& in, GraphTable&)
{
  std::string param;
  double value;
  if (!(in >> param >> value))
  {
    throw std::runtime_error("setRobot: error input");
  }
  
  if (param == "-maxEnergy")
  {
    globalRobot.setMaxEnergy(value);
  }
  else if (param == "-recoveryRate")
  {
    globalRobot.setRecoveryRate(value);
  }
  else if (param == "-speed")
  {
    globalRobot.setSpeed(value);
  }
  else
  {
    throw std::invalid_argument("setRobot:invalid argument <t1>");
  }
}
