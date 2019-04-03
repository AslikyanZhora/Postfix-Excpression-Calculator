#pragma once

#include <iostream>
#include <stack>
#include <string>
#include <math.h>
#include <fstream>

class Calculator
{
public:
	int CalculateFromAsembler(const std::string filename);
	int Calculate(const std::string expr);
	std::string GetPostfix();
	void GetAsemblerCode();
	void GetAsemblerCode(const std::string filename);
private:
	int Priority(char c);
	void InfixToPostfix(const std::string expr);
	int Check(const std::string str);
private:
	std::string m_postfix;
	std::string m_str;
};

int Calculator::Priority(char c)
{
	switch (c)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		return -1;
	}
}

void Calculator::InfixToPostfix(const std::string expr)
{
	std::stack<char> mystack;
	for (int i = 0; i < expr.length(); ++i)
	{
		if (expr[i] == ' ')
			continue;

		else if (isdigit(expr[i]))
		{
			while (isdigit(expr[i]))
			{
				m_postfix += expr[i];
				++i;
			}
			--i;
			m_postfix += ' ';
		}
		else if ((expr[i] >= 'a' && expr[i] <= 'z') || (expr[i] >= 'A' && expr[i] <= 'Z'))
		{
			m_postfix += expr[i];
			m_postfix += ' ';
		}
		else if (expr[i] == '(')
		{
			mystack.push(expr[i]);
			continue;
		}
		else if (expr[i] == ')')
		{
			while (!mystack.empty() && mystack.top() != '(')
			{
				char c = mystack.top();
				m_postfix += ' ';
				m_postfix += c;
				m_postfix += ' ';
				mystack.pop();


			}
			if (mystack.top() == '(')
			{
				char c = mystack.top();
				mystack.pop();
			}

		}
		else
		{
			while (!mystack.empty() && Priority(expr[i]) <= Priority(mystack.top()))
			{
				char c = mystack.top();
				mystack.pop();
				m_postfix += ' ';
				m_postfix += c;
				m_postfix += ' ';
			}
			mystack.push(expr[i]);
		}
	}
	while (!mystack.empty())
	{
		char c = mystack.top();
		mystack.pop();
		m_postfix += ' ';
		m_postfix += c;
		m_postfix += ' ';
	}
}

int Calculator::Calculate(const std::string expr)
{
	InfixToPostfix(expr);
	std::stack<int> stack;

	for (int i = 0; i < m_postfix.length(); ++i)
	{
		if (m_postfix[i] == ' ')
			continue;

		if (isdigit(m_postfix[i]))
		{
			m_str += "PUSH ";
			int number = 0;
			while (isdigit(m_postfix[i]))
			{
				number = number * 10 + (int)(m_postfix[i] - '0');
				m_str += m_postfix[i];
				++i;
			}
			--i;
			stack.push(number);
			m_str += "\n";
		}
		else
		{
			int value1 = stack.top();
			stack.pop();
			int value2 = stack.top();
			stack.pop();

			switch (m_postfix[i])
			{
			case '+':
				stack.push(value2 + value1);
				m_str += "ADD\n";
				break;
			case '-':
				stack.push(value2 - value1);
				m_str += "SUB\n";
				break;
			case '*':
				stack.push(value2 * value1);
				m_str += "MUL\n";
				break;
			case '/':
				stack.push(value2 / value1);
				m_str += "DIV\n";
				break;
			case '^':
				stack.push((int)pow(value2, value1));
				m_str += "POW\n";
				break;
			default:
				break;
			}
		}
	}
	return stack.top();
}

std::string Calculator::GetPostfix()
{
	return m_postfix;
}

void Calculator::GetAsemblerCode()
{
	std::cout << m_str << std::endl;
}

void Calculator::GetAsemblerCode(const std::string filename)
{
	std::ofstream  file;
	file.open(filename);
	file << m_str;
	file.close();
}

int Calculator::CalculateFromAsembler(const std::string filename)
{
	std::fstream file;
	file.open(filename.c_str());
	std::string word;
	std::stack<int> stack;
	int value1 = 0;
	int value2 = 0;
	while (file >> word)
	{
		switch (Check(word))
		{
		case -1:
			continue;
		case 0:
			stack.push(stoi(word));
			break;
		case 1:
			value1 = stack.top();
			stack.pop();
			value2 = stack.top();
			stack.pop();
			stack.push(value2 + value1);
			break;
		case 2:
			value1 = stack.top();
			stack.pop();
			value2 = stack.top();
			stack.pop();
			stack.push(value2 - value1);
			break;
		case 3:
			value1 = stack.top();
			stack.pop();
			value2 = stack.top();
			stack.pop();
			stack.push(value2 * value1);
			break;
		case 4:
			value1 = stack.top();
			stack.pop();
			value2 = stack.top();
			stack.pop();
			stack.push(value2 / value1);
			break;
		case 5:
			value1 = stack.top();
			stack.pop();
			value2 = stack.top();
			stack.pop();
			stack.push((int)pow(value2, value1));
			break;
		}
	}
	file.close();
	return stack.top();


}

int Calculator::Check(const std::string str)
{
	if (str == "ADD")
		return 1;
	else if (str == "SUB")
		return 2;
	else if (str == "MUL")
		return 3;
	else if (str == "DIV")
		return 4;
	else if (str == "POW")
		return 5;
	else if (str == "PUSH")
		return -1;
	else
		return 0;
}
