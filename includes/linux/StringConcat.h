#include <cstring>
#include <iostream>
class StringConcat {
   public:
    static void PrintConcat(const char* s1, const char* s2) {
        size_t s1_len = 0;
        size_t s2_len = 0;
        while (s1[s1_len] != '\0') s1_len++;
        while (s2[s2_len] != '\0') s2_len++;
        char* s3 = new char[s1_len + s2_len + 1 + 1];
        size_t i = 0;
        for (; i < s1_len; i++) {
            s3[i] = s1[i];
        }
        s3[i] = '+';
        i++;
        for (size_t j = 0; j < s2_len; j++, i++) {
            s3[i] = s2[j];
        }
        s3[i] = '\0';
        std::cout << s3 << std::endl;
    }
};