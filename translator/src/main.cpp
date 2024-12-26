#include "translator.h"

int main(int argc, char** argv)
{
    Translator translator;
    std::string expression;

    std::cout << "Welcome to the Calculator!" << std::endl << std::endl;

    while (1) {
        std::cout << "Enter a mathematical expression: ";
        std::getline(std::cin, expression);

        try {
            double result = translator.evaluate(expression);
            std::cout << "Result: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error evaluating expression: " << e.what() << std::endl;
        }
    }

    return 0;
}