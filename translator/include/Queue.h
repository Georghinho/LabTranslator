#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>
#include <cctype>
#include <sstream>

class Translator {
public:
    double evaluate(const std::string& expression) const {
        std::string postfix = infixToPostfix(expression);
        return evaluatePostfix(postfix);
    }
private:
    int precedence(char op) const {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    bool isOperator(char c) const {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    // Польская запись
    std::string infixToPostfix(const std::string& infix) const {
        std::stack<char> operators;
        std::ostringstream postfix;
        std::string numberBuffer; 

        for (size_t i = 0; i < infix.length(); ++i) {
            char c = infix[i];

            if (std::isspace(c)) {
                continue; 
            }

            if (std::isdigit(c) || c == '.') { 
                numberBuffer += c; 
            }
            else {
                if (!numberBuffer.empty()) {
                    postfix << numberBuffer << " "; 
                    numberBuffer.clear(); 
                }

                if (isOperator(c)) { 
                    // Проверка унарного минуса
                    if (c == '-' && (i == 0 || isOperator(infix[i - 1]) || infix[i - 1] == '(')) {
                        numberBuffer += '-'; 
                    }
                    else {
                        while (!operators.empty() && precedence(operators.top()) >= precedence(c)) {
                            postfix << operators.top() << " ";
                            operators.pop();
                        }
                        operators.push(c);
                    }
                }
                else if (c == '(') { 
                    operators.push(c);
                }
                else if (c == ')') { 
                    while (!operators.empty() && operators.top() != '(') {
                        postfix << operators.top() << " ";
                        operators.pop();
                    }
                    if (!operators.empty() && operators.top() == '(') {
                        operators.pop();
                    }
                }
                else { 
                    throw std::invalid_argument("Invalid character in expression");
                }
            }
        }

        
        if (!numberBuffer.empty()) {
            postfix << numberBuffer << " ";
        }

        while (!operators.empty()) {
            postfix << operators.top() << " ";
            operators.pop();
        }

        return postfix.str();
    }

    
    double evaluatePostfix(const std::string& postfix) const {
        std::stack<double> values;
        std::istringstream tokens(postfix);
        std::string token;

        while (tokens >> token) {
            
            if (std::isdigit(token[0]) || (token[0] == '-' && token.size() > 1) || (token[0] == '.' && token.size() > 1)) {
                values.push(std::stod(token));
            }
            else if (isOperator(token[0])) { 
                if (values.size() < 2) {
                    throw std::runtime_error("Invalid expression");
                }
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                switch (token[0]) {
                case '+': values.push(a + b); break;
                case '-': values.push(a - b); break;
                case '*': values.push(a * b); break;
                case '/':
                    if (b == 0) throw std::runtime_error("Division by zero");
                    values.push(a / b);
                    break;
                }
            }
        }

        if (values.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }

        return values.top();
    }
};

