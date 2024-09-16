#include <cmath>
#include <future>
#include <iostream>
#include <stdexcept>

class SquareRootException
{
  static double square_root(double x)
  {
    if (x < 0)
    {
      throw std::out_of_range("x < 0");
    }
    return sqrt(x);
  }

public:
  static void Main()
  {
    try
    {
      std::future<double> f = std::async(square_root, -1);
      double y = f.get();
      std::cout << y << std::endl;
    }
    catch (const std::exception &ex)
    {
      throw ex;
    }
  }
};