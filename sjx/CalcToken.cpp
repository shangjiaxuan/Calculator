#include "Header.h"

#include "CalcToken.h"

using namespace std;

namespace Calc {

	void Token_stream::init(std::istream& ist) {
		current = &ist;
	}

	void Token_stream::putback(Token t) {
		if (full) {
			throw runtime_error("putback() into a full buffer");
		}
		buffer = t;
		full = true;
	}
	Token Token_stream::get() {
		if (full) {
			full = false;
			return buffer;
		}
		while (current->good()) {
			char ch = current->get();
			while (ch == ' ') {
				current->get(ch);
			}
			switch (ch) {
			case quit: if (current->peek() == space || current->peek() == end || current->peek() == print) {
				return Token{ quit };
			}goto CalcAlpha;
			case print:
			case end:
			case '(': case ')':
			case '+': case '-':
			case '*': case '/': case '%':
			case '=':
				return Token{ ch };
			case '.':
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
			{
				current->putback(ch);
				double val;
				*current >> val;
				return Token{ number,val };
			}
			default:
			CalcAlpha:
				if (isalpha(ch)) {
					string s{ ch };
					while (current->get(ch) && (isalpha(ch) > 0 || isdigit(ch) > 0 || ch == '_')) {
						s += ch;
						if (s == squareroot) {
							return Token{ root2 };
						}
						if (s == declkey) {
							return Token{ let };
						}
					}
					current->putback(ch);
					if (isdecl) {
						return Token{ name,s };
					}
					char test{ 0 };
					current->get(test);
					//ȥ�ո�
					if (test == ' ') {
						do {
							current->get(test);
						} while (test == ' ');
						current->putback(test);
						break;
					}
					//ȥ�ո�
					//������ֵ����
					if (test == '=') {
						return Token{ assign,s };
					}
					//������ֵ����
					//������ȡ����
					current->putback(test);
					return Token{ access,s };
					//������ȡ����
				}
				throw runtime_error("Bad token");
			}
		}
	}

	Token Token_stream::peek() {
		Token t = get();
		putback(t);
		return t;
	}

	void Token_stream::ignore(const char c)
	{
		// ������������ľ��ǽ���������ջ���������
		if (full && (c == buffer.kind)) {
			full = false;
			return;
		}

		full = false; // ��ջ�����

		// ����������
		char ch{ 0 }; // 0������ַ����ǲ������ü��������
		while (current->get(ch)) {
			if (ch == c || ch == '\n') {
				return;
			}
		}
	}
}
