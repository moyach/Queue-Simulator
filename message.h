#include<string>


class Message {
    public:
        static const std::string invalidOption;

        static const std::string noObjectFound;

        static const std::string red;
        static const std::string normal;
        static const std::string green;
        static const std::string yellow;

        static std::string errorEmpty(std::string input);

        static std::string errorFormat(std::string input, std::string format);

        static std::string error(std::string input);

        static std::string maxCapacity(std::string className);

        static std::string newObjectAdded(std::string className);

        static std::string inlineMessage(std::string message);

        static std::string newlineMessage(std::string message);
};
