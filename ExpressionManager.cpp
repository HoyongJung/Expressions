#include "ExpressionManager.h"
const string OPEN = "({[";
const string CLOSE = ")}]";
const string OPERATION = "+-*/%";

bool ExpressionManager::isOpen(string ch) {
    return OPEN.find(ch) != string::npos;     
}
bool ExpressionManager::isClose(string ch) {
    return CLOSE.find(ch) != string::npos;     
}
bool ExpressionManager::isPair(string ch1, string ch2) {
    return OPEN.find(ch1) == CLOSE.find(ch2);
}
bool ExpressionManager::isOperator(string ch) {
    return OPERATION.find(ch) != string::npos;
}
bool ExpressionManager::isNumber(string ch) {
    stringstream ss;
    int num;
    ss << ch;
    if (ss >> num) {
        return true;
    }
    else {
        return false;
    }
}
int ExpressionManager::Precedence(string ch) {
    int precedence;
    if (isClose(ch)) {
        precedence = 3;
    }
    else if (isOperator(ch)) {
        if (ch == "*" || ch == "/" || ch == "%") {
            precedence = 2;
        }
        else if (ch == "+" || ch == "-") {
            precedence = 1;
        }
    }
    else if (isOpen(ch)) {
        precedence = 0;
    }
    return precedence;
}

/*
* Checks whether an expression is balanced on its parentheses
*
* - The given expression will have a space between every number or operator
*
* @return true if expression is balanced
* @return false otherwise
*/
bool ExpressionManager::isBalanced(string expression) {
    stack<char> sc;
    char next_ch;
    char top_ch;
    bool balanced = true;
    for (string::iterator it = expression.begin(); balanced && (it != expression.end()); ++it) {
        next_ch = *it;
        if (isOpen(string(1, next_ch))) {
            sc.push(next_ch);
        }
        else if (isClose(string(1, next_ch))) {
            if (sc.empty()) {
                balanced = false;
            }
            else {
                top_ch = sc.top();
                sc.pop();
                balanced = OPEN.find(top_ch) == CLOSE.find(next_ch);
            }
        }
    }
    return balanced && sc.empty();
}

/**
* Converts a postfix expression into an infix expression
* and returns the infix expression.
*
* - The given postfix expression will have a space between every number or operator.
* - The returned infix expression must have a space between every number or operator.
* - Redundant parentheses are acceptable i.e. ( ( 3 * 4 ) + 5 ).
* - Check lab requirements for what will be considered invalid.
*
* return the string "invalid" if postfixExpression is not a valid postfix expression.
* otherwise, return the correct infix expression as a string.
*/
string ExpressionManager::postfixToInfix(string postfixExpression) {
    stack<string> ss;
    string valueOne;
    string valueTwo;
    istringstream input(postfixExpression);
    string token;
    int numCount = 0;
	int opCount = 0;
    bool invalid = false;
    
    while (input >> token) {
		if (isNumber(token) && (token.find(".") == string::npos)) {
			ss.push(token);
			++numCount;
		}
		else if (isOperator(token) && ss.size() >= 2) {
			valueTwo = ss.top();
			ss.pop();
			valueOne = ss.top();
			ss.pop();
			if (valueOne != "0" && valueTwo != "0") {
    			if (token == "+") {
    			    ss.push("( " + valueOne + " + " + valueTwo + " )");
    			}
    			else if (token == "-") {
    			    ss.push("( " + valueOne + " - " + valueTwo + " )");
    			}
    			else if (token == "*") {
    			    ss.push("( " + valueOne + " * " + valueTwo + " )");
    			}
    			else if (token == "/") {
    			    ss.push("( " + valueOne + " / " + valueTwo + " )");
    			}
    			else if (token == "%") {
    			    ss.push("( " + valueOne + " % " + valueTwo + " )");
    			}
    			++opCount;
			}
		}
		else {
			invalid = true;
			break;
		}
	}
	if (!ss.empty()) {
    	if (!invalid && !isOperator(string(1, ss.top()[0])) && (numCount == (opCount + 1))) {
    		return ss.top();
    	}
    	else {
    		return "invalid";
    	}
	}
	else {
	    return "invalid";
	}
}

/*
* Evaluates a postfix expression returns the result as a string
*
* - The given postfix expression will have a space between every number or operator.
* - Check lab requirements for what will be considered invalid.
*
* return the string "invalid" if postfixExpression is not a valid postfix Expression
* otherwise, return the correct evaluation as a string
*/
string ExpressionManager::postfixEvaluate(string postfixExpression) {
    stack<string> ss;
    string valueOne;
    string valueTwo;
    int result;
    istringstream input(postfixExpression);
    string token;
    
    while (input >> token) {
        if (isNumber(token)) {
            ss.push(token);
        }
        else if (isOperator(token) && ss.size() >= 2) {
            valueTwo = ss.top();
            ss.pop();
            valueOne = ss.top();
            ss.pop();
            if (token == "+") {
			    result = stoi(valueOne) + stoi(valueTwo);
			    ss.push(to_string(result));
			}
			else if (token == "-") {
			    result = stoi(valueOne) - stoi(valueTwo);
			    ss.push(to_string(result));
			}
			else if (token == "*") {
			    result = stoi(valueOne) * stoi(valueTwo);
			    ss.push(to_string(result));
			}
			else if (token == "/") {
			    if (valueTwo != "0") {
			        result = stoi(valueOne) / stoi(valueTwo);
			        ss.push(to_string(result));
			    }
			    else {
			        break;
			    }
			}
			else if (token == "%") {
			    result = stoi(valueOne) % stoi(valueTwo);
			    ss.push(to_string(result));
			}
        }
        else {
            break;
        }
    }
    if (!ss.empty()) {
        return ss.top();
    }
    else {
        return "invalid";
    }
}

/*
* Converts an infix expression into a postfix expression
* and returns the postfix expression
*
* - The given infix expression will have a space between every number or operator.
* - The returned postfix expression must have a space between every number or operator.
* - Check lab requirements for what will be considered invalid.
*
* return the string "invalid" if infixExpression is not a valid infix expression.
* otherwise, return the correct postfix expression as a string.
*/
string ExpressionManager::infixToPostfix(string infixExpression) {
    stack<string> os;
    string postfix;
    istringstream input(infixExpression);
    string token;
    int numCount = 0;
    int opCount = 0;
    bool invalid = false;
    
    if (isBalanced(infixExpression)) {
        while (input >> token) {
    		if (isNumber(token) && (token.find(".") == string::npos)) {
    		    if (postfix[postfix.size() - 1] != ' ' && postfix.size() != 0) {
    		        postfix += (" " + token + " ");
    			    ++numCount;
    		    }
    		    else {
    			    postfix += (token + " ");
    			    ++numCount;
    		    }
    		}
    		else if (isOperator(token)) {
    			if (os.empty() || (Precedence(os.top()) < Precedence(token))) {
    				os.push(token);
    			}
    			else {
    				if (!os.empty() && isOperator(os.top())) {
    					while (!os.empty() && (Precedence(os.top()) >= Precedence(token))) {
    						postfix += (os.top() + " ");
    						os.pop();
    					}
    				}
    				os.push(token);
    			}
    			++opCount;
    		}
    		else if (isOpen(token)) {
    			os.push(token);
    		}
    		else if (isClose(token)) {
    			while (!os.empty() && !isPair(os.top(), token)) {
    				postfix += (os.top());
    				os.pop();
    			}
    			os.pop();
    			if (os.size() > 0) {
    				postfix += " ";
    			}
    		}
    		else {
    			invalid = true;
    			break;
    		}
    	}
    }
    else {
		invalid = true;
	}
	while (!os.empty() && isOperator(os.top())) {
	    if (os.size() > 1) {
		    postfix += (os.top() + " ");
		    os.pop();
	    }
	    else {
	        postfix += (os.top());
		    os.pop();
	    }
	}
	if (!invalid && !isOperator(string(1, postfix[0])) && (numCount == (opCount + 1))) {
		return postfix;
	}
	else {
		return "invalid";
	}
}