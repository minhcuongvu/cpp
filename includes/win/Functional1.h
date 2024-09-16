#include <iostream>
#include <functional>

class Functional1
{
private:
    // Function demonstrating immutability
    static int addOne(int x)
    {
        return x + 1;
    }

    static int applyFunction(int x, std::function<int(int)> func)
    {
        return func(x);
    }

public:
    static int main()
    {
        int value = 10;
        int newValue1 = applyFunction(value, [=](int val)
                                      { return val + 1; });

        std::cout << "Original value: " << value << std::endl;
        std::cout << "New value: " << newValue1 << std::endl;
        int* newValue1Ptr = &newValue1;
        std::cout << "Pointer: " << newValue1Ptr << std::endl;

        int newValue2 = applyFunction(value, addOne);

        std::cout << "Original value: " << value << std::endl;
        std::cout << "New value: " << newValue2 << std::endl;
        return 0;
    }
};
