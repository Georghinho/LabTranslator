#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>
#include <cctype>
#include <sstream>

class Translator {
public:
    double evaluate(const std::string& expression) const {
        std::string cleanExpression = removeSpaces(expression);
        if (!areParenthesesBalanced(cleanExpression)) {
            throw std::invalid_argument("Unmatched parentheses in expression");
        }
        std::string postfix = infixToPostfix(cleanExpression);
        return evaluatePostfix(postfix);
    }

private:
    enum class State {
        START,
        NUMBER,
        OPERATOR,
        LPAREN,
        RPAREN,
        ERROR
    };

    int precedence(char op) const {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    bool isOperator(char c) const {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

    bool areParenthesesBalanced(const std::string& expression) const {
        std::stack<char> stack;
        for (char c : expression) {
            if (c == '(') {
                stack.push(c);
            }
            else if (c == ')') {
                if (stack.empty()) {
                    return false;
                }
                stack.pop();
            }
        }
        return stack.empty();
    }

    std::string removeSpaces(const std::string& str) const {
        std::string result;
        for (char c : str) {
            if (!std::isspace(c)) {
                result += c;
            }
        }
        return result;
    }

    std::string infixToPostfix(const std::string& infix) const {
        std::stack<char> operators;
        std::ostringstream postfix;
        State state = State::START; // Начальное состояние
        bool lastWasOperatorOrLParen = true; // Для обработки унарного минуса

        for (size_t i = 0; i < infix.length(); ++i) {
            char token = infix[i];

            switch (state) {
            case State::START:
                if (std::isdigit(token) || (token == '.' && std::isdigit(infix[i + 1]))) {
                    postfix << token;
                    state = State::NUMBER; // Переход в состояние NUMBER
                    lastWasOperatorOrLParen = false;
                }
                else if (isOperator(token)) {
                    if (token == '-' && lastWasOperatorOrLParen) {
                        postfix << '0'; // Для унарного минуса добавляем 0
                    }
                    postfix << ' ';
                    operators.push(token);
                    state = State::OPERATOR; // Переход в состояние OPERATOR
                    lastWasOperatorOrLParen = true;
                }
                else if (token == '(') {
                    operators.push(token);
                    state = State::LPAREN; // Переход в состояние LPAREN
                    lastWasOperatorOrLParen = true;
                }
                else {
                    throw std::invalid_argument("Invalid character in expression");
                }
                break;

            case State::NUMBER:
                if (std::isdigit(token) || token == '.') {
                    postfix << token;
                }
                else if (isOperator(token)) {
                    postfix << ' ';
                    while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                        postfix << operators.top() << ' ';
                        operators.pop();
                    }
                    operators.push(token);
                    state = State::OPERATOR; // Переход в состояние OPERATOR
                    lastWasOperatorOrLParen = true;
                }
                else if (token == ')') {
                    postfix << ' ';
                    while (!operators.empty() && operators.top() != '(') {
                        postfix << operators.top() << ' ';
                        operators.pop();
                    }
                    if (!operators.empty() && operators.top() == '(') {
                        operators.pop();
                    }
                    state = State::RPAREN; // Переход в состояние RPAREN
                    lastWasOperatorOrLParen = false;
                }
                else {
                    throw std::invalid_argument("Invalid character in expression");
                }
                break;

            case State::OPERATOR:
                if (std::isdigit(token) || (token == '.' && std::isdigit(infix[i + 1]))) {
                    postfix << token;
                    state = State::NUMBER; // Переход в состояние NUMBER
                    lastWasOperatorOrLParen = false;
                }
                else if (token == '(') {
                    operators.push(token);
                    state = State::LPAREN; // Переход в состояние LPAREN
                    lastWasOperatorOrLParen = true;
                }
                else {
                    throw std::invalid_argument("Invalid character in expression");
                }
                break;

            case State::LPAREN:
                if (std::isdigit(token) || (token == '.' && std::isdigit(infix[i + 1]))) {
                    postfix << token;
                    state = State::NUMBER; // Переход в состояние NUMBER
                    lastWasOperatorOrLParen = false;
                }
                else if (isOperator(token)) {
                    if (token == '-' && lastWasOperatorOrLParen) {
                        postfix << '0'; // Для унарного минуса добавляем 0
                    }
                    postfix << ' ';
                    operators.push(token);
                    state = State::OPERATOR; // Переход в состояние OPERATOR
                    lastWasOperatorOrLParen = true;
                }
                else if (token == '(') {
                    operators.push(token);
                    lastWasOperatorOrLParen = true;
                }
                else {
                    throw std::invalid_argument("Invalid character in expression");
                }
                break;

            case State::RPAREN:
                if (isOperator(token)) {
                    postfix << ' ';
                    while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                        postfix << operators.top() << ' ';
                        operators.pop();
                    }
                    operators.push(token);
                    state = State::OPERATOR; // Переход в состояние OPERATOR
                    lastWasOperatorOrLParen = true;
                }
                else if (token == ')') {
                    while (!operators.empty() && operators.top() != '(') {
                        postfix << ' ' << operators.top();
                        operators.pop();
                    }
                    if (!operators.empty() && operators.top() == '(') {
                        operators.pop();
                    }
                    lastWasOperatorOrLParen = false;
                }
                else {
                    throw std::invalid_argument("Invalid character in expression");
                }
                break;

            case State::ERROR:
                throw std::invalid_argument("Invalid state in finite state machine");
            }
        }

        while (!operators.empty()) {
            postfix << ' ' << operators.top();
            operators.pop();
        }

        return postfix.str();
    }

    double evaluatePostfix(const std::string& postfix) const {
        std::stack<double> values;
        std::istringstream tokens(postfix);
        std::string token;

        while (tokens >> token) {
            if (std::isdigit(token[0]) || (token[0] == '-' && token.length() > 1)) {
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
                    if (b == 0) {
                        throw std::runtime_error("Division by zero");
                    }
                    values.push(a / b);
                    break;
                default:
                    throw std::invalid_argument("Unknown operator");
                }
            }
        }

        if (values.size() != 1) {
            throw std::runtime_error("Invalid expression");
        }

        return values.top();
    }
};

