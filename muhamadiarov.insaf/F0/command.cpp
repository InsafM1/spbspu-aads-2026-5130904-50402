#include "command.hpp"

namespace muhamadiarov
{
  const double INF = 1e18;
  static Robot robot; 

  enum class Goal
  {
    PATH,
    TIME,
    GOLD
  };

  Graph& getGraph(GraphTable& graphs, std::string name)
  {
    if (!graphs.has(name))
    {
      throw std::runtime_error("getGraph:Graph not found");
    }
    return graphs.get(name);
  }

  struct DijkstraResult
  {
    List<int> path;
    double totalCost;
    bool found;

    DijkstraResult():
      totalCost(0),
      found(false)
    {}
  };

  DijkstraResult dijkstra(const Graph& graph, int start, int end, Goal goal)
  {
    DijkstraResult result;

    if (!graph.findVertex(start) || !graph.findVertex(end)) {
      return result;
    }

    const List< int >& vertices = graph.getVertices();
    Vector< int > vertList;
    LCIter< int > it = vertices.cbegin();
    for (size_t i = 0; i < vertices.size(); ++i)
    {
      vertList.pushBack(*it);
      ++it;
    }

    Vector< double > dist;
    Vector< bool > visited;
    Vector< int > prev;

    for (size_t i = 0; i < vertList.size(); ++i)
    {
      dist.pushBack(INF);
      visited.pushBack(false);
      prev.pushBack(-1);
    }

    int startIdx = -1;
    int endIdx = -1;
    for (size_t i = 0; i < vertList.size(); ++i)
    {
      if (vertList[i] == start)
      {
        startIdx = i;
      }
      if (vertList[i] == end)
      {
        endIdx = i;
      }
    }

    dist[startIdx] = 0;

    for (size_t count = 0; count < vertList.size(); ++count)
    {
      int current = -1;
      double minDist = INF;

      for (size_t i = 0; i < vertList.size(); ++i)
      {
        if (!visited[i] && dist[i] < minDist)
        {
          minDist = dist[i];
          current = i;
        }
      }

      if (current == -1 || minDist == INF)
      {
        break;
      }
      if (current == endIdx)
      {
        break;
      }

      visited[current] = true;
      int currentVertex = vertList[current];

      List< std::pair< int, Edge > > outgoing = graph.getOutBounds(currentVertex);
      LCIter< std::pair< int, Edge > > edgeIt = outgoing.cbegin();
      for (size_t i = 0; i < outgoing.size(); ++i)
      {
        int neighbor = edgeIt->first;
        const Edge& edge = edgeIt->second;

        int neighborIdx = -1;
        for (size_t j = 0; j < vertList.size(); ++j)
        {
          if (vertList[j] == neighbor)
          {
            neighborIdx = j;
            break;
          }
        }
        double dopCost = 0.0;
        double energyCost = (*edgeIt).second.getEnergy(robot.getSpeed());
        bool enough = true;
        while (!robot.consumeEnergy(energyCost))
        {
          if (robot.getCurrentEnergy() == robot.getMaxEnergy())
          {
            enough = false;
            break;
          }
          robot.charge();
          dopCost += 1.0;
        }
        if (neighborIdx == -1 || visited[neighborIdx] || !enough)
        {
          ++edgeIt;
          continue;
        }

        double cost;
        if (goal == Goal::TIME)
        {
          cost = edge.getTime(robot.getSpeed()) + dopCost;
        }
        else if (goal == Goal::PATH)
        {
          cost = static_cast< double >(edge.distance_);
        }
        else
        {
          int value;
          if (graph.getPointValue(neighbor, value))
          {
            cost = static_cast< double >(value); 
          }
        }

        if (dist[current] + cost < dist[neighborIdx])
        {
          dist[neighborIdx] = dist[current] + cost;
          prev[neighborIdx] = current;
        }
        ++edgeIt;
      }
    }

    if (dist[endIdx] == INF)
    {
      return result;
    }

    Vector< int > pathReversed;
    int currentIdx = endIdx;
    while (currentIdx != -1)
    {
      pathReversed.pushBack(vertList[currentIdx]);
      currentIdx = prev[currentIdx];
    }

    for (int i = static_cast< int >(pathReversed.size()) - 1; i >= 0; --i)
    {
      result.path.pushBack(pathReversed[i]);
    }

    result.totalCost = dist[endIdx];
    result.found = true;

    return result;
  }
}

void muh::makeRobot(std::istream& in, GraphTable&)
{
  double maxEnergy, recoveryRate, speed;
  if (!(in >> maxEnergy >> recoveryRate >> speed))
  {
    throw std::runtime_error("makeRobot: error input");
  }
  robot = Robot(maxEnergy, recoveryRate, speed);
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
    robot.setMaxEnergy(value);
  }
  else if (param == "-recoveryRate")
  {
    robot.setRecoveryRate(value);
  }
  else if (param == "-speed")
  {
    robot.setSpeed(value);
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

void muh::addEdge(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int from, to;
  char type;
  size_t distance;

  if (!(in >> graphName >> from >> to >> type >> distance))
  {
    throw std::runtime_error("addEdge: error input");
  }

  Graph& g = getGraph(graphs, graphName);
  g.addConnection(from, to, type, distance); 
}

void muh::rmEdge(std::istream& in, GraphTable& graphs)
{
  std::string graphName;
  int from, to;
  char type;
  size_t distance;

  if (!(in >> graphName >> from >> to >> type >> distance))
  {
    throw std::runtime_error("rmEdge: error input"); 
  }

  Graph& g = getGraph(graphs, graphName);
  g.removeConnection(from, to, type, distance);
}

void muh::setEdge(std::istream& in, GraphTable& graphs)
{
  std::string mode;
  std::string graphName;
  int from, to;
  char oldType;
  size_t oldDistance;
  
  if (!(in >> mode >> graphName >> from >> to >> oldType >> oldDistance))
  {
    throw std::runtime_error("setEdge: error input"); 
  }

  Graph& g = getGraph(graphs, graphName);
  if (!g.findConnection(from, to, oldType, oldDistance))
  { 
    throw std::invalid_argument("setEdge: not found this connection"); 
  }
  
  g.removeConnection(from, to, oldType, oldDistance);
  if (mode == "-type")
  {
    char newType;
    if (!(in >> newType))
    {
      throw std::runtime_error("setEdge: error input");
    }

    g.addConnection(from, to, newType, oldDistance);
  }
  else if (mode == "-distance")
  {
    size_t newDistance;
    if (!(in >> newDistance))
    {
      throw std::runtime_error("setEdge: error input");
    }

    g.addConnection(from, to, oldType, newDistance);
  }
  else if (mode == "-all")
  {
    char newType;
    size_t newDistance;
    if (!(in >> newType >>newDistance))
    {
      throw std::runtime_error("setEdge: error input");
    }

    g.addConnection(from, to, newType, newDistance);
  }
  else
  { 
    throw std::invalid_argument("setEdge: incorrect type"); 
  }
}
