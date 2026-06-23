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

void muh::addVertex(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int vertex, value;
  
  if (!(in >> graphName >> vertex >> value))
  {
    throw std::runtime_error("addVertex: error input");
  }
  
  Graph& g = getGraph(graphs, graphName);
  g.addVertex(vertex);
  g.setPointValue(vertex, value);
}

void muh::rmVertex(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int vertex;
  
  if (!(in >> graphName >> vertex))
  {
    throw std::runtime_error("rmVertex: error input");
  }
  
  Graph& g = getGraph(graphs, graphName);
  g.removeVertex(vertex);
}

void muh::setVertex(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int vertex;
  size_t value;
  
  if (!(in >> graphName >> vertex >> value))
  {
    throw std::runtime_error("rmVertex: error input");
  }
  
  Graph& g = getGraph(graphs, graphName);
  g.setPointValue(vertex, value);
}

