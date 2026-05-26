#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Incorrect data";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Can`t open file";
    return 1;
  }
  //...
  file.close();
  return 0;
}
