#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

// enum для всех видов токенов, поддерживаемых программой
enum TokenType {
    VARIABLE,  //
    TRUE,      //
    FALSE,     //
    AND,       //
    OR,        //
    NOT,       //
    LPAREN,    //
    RPAREN,    //
    END        //
};

// стурктура токена представляет собой строку (чем он был в строке и что это за
// токен по типу)
struct Token {
    TokenType type;
    std::string value;
};

// класс парсера логических выражений
class Parser {
   public:
    // метод для обновления значения переменной
    void setVariable(const std::string& name, bool value) {
        variables[name] = value;
    }

    // функция для вычисления значения выражения
    bool evaluate(const std::string& expression) {
        // проверяем правильное ли выражение
        if (!balance_check(expression)) {
            throw std::runtime_error("Invalid parenthesis");
        }

        // считываем первый токен и запускаем процесс рекурсивной обработки
        std::istringstream stream(expression);
        Token token = getNextToken(stream);
        return parseExpression(stream, token);
    }

    // функция для получения имен и значений всех переменных
    std::vector<std::pair<std::string, bool>> getVariables() {
        std::vector<std::pair<std::string, bool>> result;
        for (auto var : variables) {
            result.push_back(std::make_pair(var.first, var.second));
        }

        return result;
    }

    // получим количество сохраненных переменных
    size_t getVariablesCount() { return variables.size(); }

   private:
    // хэш таблица для хранения перменных
    std::unordered_map<std::string, bool> variables;

    // функция для парсинга текущего токена
    Token getNextToken(std::istringstream& stream) {
        char ch;

        // пока можем вводть из выражения
        while (stream >> ch) {
            // пропускаем пробелы
            if (std::isspace(ch)) continue;

            // если первый символ токена букува
            if (std::isalpha(ch)) {
                std::string var(1, ch);

                // пока токен не закончится продолжаем его читать
                while (stream.peek() != EOF && std::isalnum(stream.peek())) {
                    stream >> ch;
                    var += ch;
                }

                // проверка на то, какой токен перед нами
                if (var == "AND") {
                    return {TokenType::AND, "&"};
                } else if (var == "OR") {
                    return {TokenType::OR, "|"};
                } else if (var == "NOT") {
                    return {TokenType::NOT, "!"};
                } else if (var == "TRUE") {
                    return {TokenType::TRUE, "T"};
                } else if (var == "FALSE") {
                    return {TokenType::FALSE, "F"};
                } else if (var == "(") {
                    return {TokenType::LPAREN, "("};
                } else if (var == ")") {
                    return {TokenType::RPAREN, ")"};
                } else {
                    return {TokenType::VARIABLE, var};
                }
            }
        }

        // если файл закончился - возвращаем токен END
        return {TokenType::END, ""};
    }

    // функция для парсинга логического выражения
    bool parseExpression(std::istringstream& stream, Token& token) {
        // вычисляем первое значение в выражении
        bool result = parseTerm(stream, token);

        while (token.type == TokenType::OR) {  // обрабатываем операторы OR
            token = getNextToken(stream);
            result = result || parseTerm(stream, token);
        }

        return result;
    }

    //
    bool parseTerm(std::istringstream& stream, Token& token) {
        bool result = parseFactor(stream, token);  // обрабатываем первый фактор

        while (token.type == TokenType::AND) {  // обрабаытываем оператор END
            token = getNextToken(stream);
            result = result && parseFactor(stream, token);
        }

        return result;
    }

    // функция для обработки токена с логическим действием (не перменной)
    bool parseFactor(std::istringstream& stream, Token& token) {
        if (token.type == TokenType::NOT) {
            token = getNextToken(stream);
            return !parseFactor(stream, token);
        } else if (token.type == TokenType::LPAREN) {
            token = getNextToken(stream);
            bool result = parseExpression(stream, token);
            if (token.type != TokenType::RPAREN) {
                throw std::runtime_error("Mismatched parentheses");
            }
            token = getNextToken(stream);
            return result;
        } else if (token.type == TokenType::VARIABLE) {
            if (variables.count(token.value) == 0) {
                throw std::runtime_error("Invalid variable " + token.value);
            }
            bool value = variables[token.value];
            token = getNextToken(stream);
            return value;
        } else if (token.type == TokenType::TRUE) {
            token = getNextToken(stream);
            return true;
        } else if (token.type == TokenType::FALSE) {
            token = getNextToken(stream);
            return false;
        }
        throw std::runtime_error("Unexpected token");
    }

    // функция для проверки на правильную скобочную последовательность с помощью
    // метода баланса
    bool balance_check(const std::string& expression) {
        int balance = 0;
        for (size_t i = 0; i < expression.size(); ++i) {
            if (expression[i] == '(')
                balance++;
            else if (expression[i] == ')')
                balance--;

            if (balance < 0) return false;
        }

        return true;
    }
};

int main() {
    Parser parser;
    std::string command;

    // сообщение со всеми командами
    std::string help_message =
        "Commands:\n"                                                  //
        "\tset <var_name> <exp> -"                                     //
        "set variable \"var_name\" to result of expression \"exp\"\n"  //
        "\tprint <var_name> - print value of variable \"var_name\"\n"  //
        "\tprint_vars - print all variables with their values\n"       //
        "\texit - shutdown the program\n";

    // выводим сообщение при старте
    std::cout << help_message << std::endl;

    bool flag = true;
    while (flag) {
        std::cout << "Enter command: ";

        std::getline(std::cin, command);

        // проверяем выполнение разных команд
        if (command.substr(0, 3) == "set") {
            std::istringstream iss(command.substr(4));

            std::string variable;
            std::string expression;

            iss >> variable;
            expression = iss.str().substr(iss.tellg());

            try {
                bool result = parser.evaluate(expression);
                parser.setVariable(variable, result);
            } catch (const std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        } else if (command.substr(0, 10) == "print_vars") {
            if (parser.getVariablesCount() != 0) {
                auto result = parser.getVariables();

                for (auto var : result) {
                    std::cout << var.first << " "
                              << (var.second ? "True" : "False") << std::endl;
                }
            } else {
                std::cout << "No variables yet" << std::endl;
            }
        } else if (command.substr(0, 5) == "print") {
            std::string expression = command.substr(5);
            try {
                bool result = parser.evaluate(expression);
                std::cout << "result: ";
                std::cout << (result ? "True" : "False") << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "error: " << e.what() << std::endl;
            }
        } else if (command.substr(0, 4) == "exit") {
            flag = false;
        } else if (command.substr(0, 4) == "help") {
            std::cout << help_message << std::endl;
        } else {
            std::cout << "unknown command" << std::endl;
        }
    }

    return 0;
}