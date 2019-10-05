#include<string>

class Validator {
    public:
        static bool isEmpty(std::string str);
        static bool isNumber(std::string str);
        static bool isAlpha(std::string str);
        static bool isFloat(std::string str);

        static std::string toLower(std::string str);
};
