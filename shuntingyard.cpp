#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <cmath>

//using namespace std;

// Token class to represent numbers, operators, and parentheses
class Token {
public:
    enum Type {
        Number,
        Operator,
        LeftParen,
        RightParen
    };
    Token(Type type) : type(type) {}
    Type type;
    double value;  
    char op;  
};

// Function to createToken an arithmetic  / Scientific / deciaml expresion
std::vector<Token> createToken(std::string expr) 
{
    std::vector<Token> tokens;
    std::istringstream iss(expr);
    double num;
    char op;
    std::string token;
    while (iss >> token) 
    {
    std::istringstream token_iss(token);
    if (token_iss >> num) 
    {
        tokens.push_back(Token(Token::Number));
        tokens.back().value = num;
    } else if (token.size() == 1) 
    {
      op = token[0];
      if  (op == '+' || op == '-' || op == '*' || op == '/' || op == '%')
      {
     // Operator
      tokens.push_back(Token(Token::Operator));
      tokens.back().op = op;
      } else if (op == '(') 
      {
      // Left parenthesis
            tokens.push_back(Token(Token::LeftParen));
       } else if (op == ')') 
       {
      // Right parenthesis
            tokens.push_back(Token(Token::RightParen));
        }
     }
    }
        return tokens;
}

// Function to convert an expression to RPN
std::vector<Token> to_rpn(std::vector<Token> tokens) {
    std::vector<Token> rpn;
    std::stack<Token> op_stack;
    for (int i = 0; i < tokens.size(); i++) {
        Token& token = tokens[i];
        switch (token.type) {
            case Token::Number:
                rpn.push_back(token);
                break;
            case Token::Operator:
                while (!op_stack.empty() && op_stack.top().type == Token::Operator &&
                       ((op_stack.top().op == '*' || op_stack.top().op == '/' || op_stack.top().op == '%') ||
                        (token.op == '+' || token.op == '-'))) {
                    rpn.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.push(token);
                break;
            case Token::LeftParen:
                op_stack.push(token);
                break;
            case Token::RightParen:
                while (!op_stack.empty() && op_stack.top().type != Token::LeftParen) {
                    rpn.push_back(op_stack.top());
                    op_stack.pop();
                }
                op_stack.pop();
                break;
        }
    }
    while (!op_stack.empty()) 
    {
        rpn.push_back(op_stack.top());
        op_stack.pop();
    }
    return rpn;
}


bool isValidExpression(const std::string& InputExpression) 
{
    std::istringstream iss(InputExpression);
    std::string token;

    while (iss >> token) 
    {
        if (token == "(" || token == ")" || token == "+" || token == "-" || token == "*" || token == "/" || token == "%") 
        {
            continue;
        }
    }
    return true;
}


// Recursive function to evaluate an RPN expression
double evaluateRPNExp(std::vector<Token>& rpn) 
{
    if (rpn.empty()) 
    {
        return 0.0;
    } else {
        Token token = rpn.back();
        rpn.pop_back();

        switch (token.type) {
            case Token::Operator: {
                double op2 = evaluateRPNExp(rpn);
                double op1 = evaluateRPNExp(rpn);
                switch ((char)token.op) {
                    case '+':
                        return op1 + op2;
                    case '-':
                        return op1 - op2;
                    case '*':
                        return op1 * op2;
                    case '/':
                        return op1 / op2;
                    case '%':
                        return int(fmod(op1, op2));
                    default:
                        return 0.0;
                }
            }
            case Token::Number:
                 return token.value;
            default:
                return 0.0;
        }
    }
}

void displayRPNExp(const std::vector<Token>& rpn, int index) {
    if (index >= rpn.size()) 
    {
        return;
    }

    Token token = rpn[index];

    if (token.type == Token::Number) 
    {
        std::cout << token.value << " ";
        displayRPNExp(rpn, index + 1);
    } else  if (token.type == Token::Operator) 
    {
        std::cout << token.op << " ";
        displayRPNExp(rpn, index + 1);
    } 
}


int main (  ) 
 {
   std::string InputExpression;
   while( getline(std::cin, InputExpression) )
   {
    if (InputExpression.empty() )
    {
        break;
    }    
    else if (isValidExpression(InputExpression)) 
    {
        std::vector<Token> tokens = createToken(InputExpression);
        std::vector<Token> myVector  = to_rpn(tokens);
        displayRPNExp(myVector, 0);
        
        double result = evaluateRPNExp(myVector);
        std::cout << "= " << result << std::endl;
    } 
    else 
    {
        break;
    }
   }
}