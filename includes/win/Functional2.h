#include <iterator>
#include <vector>
#include <algorithm>

class Functional2
{
  private:
  /// @brief not modifying x
  /// @param x 
  /// @return x*x
  static int square(int x)
  {
    return x*x;
  }
  public:
  static void main()
  {
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<int> squaredNumbers(numbers.size());

    // Apply the square function to each element in the numbers vector
    std::transform(numbers.begin(), numbers.end(), squaredNumbers.begin(), square);
  }
};