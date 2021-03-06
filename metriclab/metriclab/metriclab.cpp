
#include "stdafx.h"
#include <regex>
#include <map>
#include <fstream>
#include <string.h>
#include <iostream>
#include "table.h"
#include <sstream>
#include <cstring>
using namespace std;

const char *file = "python.txt";
map<string, int> ids;
map<string, int> priors;
map<string, int> i_o;
regex per("\\w+((\\.)?\\w+\\s)?=");

string toStr(int i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

bool checkP(string s) {
	return (s.find("input") != string::npos) || (s.find("print") != string::npos);
}

bool checkC(string s, string per) {
	bool a1 = s.find("for") != string::npos;
	bool a2 = s.find("if") != string::npos;
	bool a3 = s.find("while") != string::npos;
	bool a4 = s.find("switch") != string::npos;
	return a1 | a2 | a3 | a4;
}


int checkM(string s, string pe) {
	if (s.find(pe) < s.find("=")) return 2;
	if (s.find(pe) > s.find("=")) return 1;
}

void vcont(vector<string> v) {
	for (auto i : v) {
		cout << i << endl;
	}
	cout << endl;
}

int main() {

ifstream fin(file);
while (!fin.eof()) {
	string cur;
	getline(fin, cur);
	sregex_iterator next(cur.begin(), cur.end(), per);
	sregex_iterator end;
	while (next != end) {
		smatch a = *next;
		string abc = a.str();
		if (abc[abc.length() -1] == '=') abc.replace(abc.length() - 1, abc.length(), "");
		ids[abc]++;
		next++;
		switch (priors[abc]) {
		case 0: {
			if (checkP(cur)) {
				priors[abc] = 1;
				i_o[abc] = 1;
				break;
			}
			if (checkC(cur, abc)) {
				priors[a.str()] = 3;
				break;
			}
			if (checkM(cur, abc) == 2) priors[abc] = 2;
			priors[abc] = 4;
			break;
		}
		case 1: {
			if (checkC(cur, abc)) {
				priors[abc] = 3;
				break;
			}
			priors[abc] = checkM(cur, abc);

			break;
		}
		case 2: {
			if (checkC(cur, abc)) {
				priors[abc] = 3;
				break;
			}
			break;
		}
		case 3: {
			break;
		}
		case 4: {
			if (checkP(cur)) {
				priors[abc] = 1;
				i_o[abc] = 1;
				break;
			}
			if (checkC(cur, abc)) {
				priors[abc] = 3;
				break;
			}
			if (checkM(cur, abc) == 1) {
				priors[abc] = 4;
			}
			else priors[abc] = 2;
		}
		}
	}
}
ofstream fout("result.txt");
TextTable spen;
spen.add("id");
for (auto i : ids) {
	spen.add(i.first);
}
spen.add("sum");
spen.endOfRow();
spen.add("Spen");
int s = 0;
for (auto i : ids) {
	s += i.second - 1;
	spen.add(toStr(i.second - 1));
}
spen.add(toStr(s));
spen.endOfRow();
fout << spen;
cout << "full Chepin\n\n";
vector<string> P, M, C, T;
for (auto i : priors) {
	switch (i.second) {
	case 1:
		P.push_back(i.first);
		break;
	case 2:
		M.push_back(i.first);
		break;
	case 3:
		C.push_back(i.first);
		break;
	case 4:
		T.push_back(i.first);
		break;
	}
}
cout << "P = " << P.size() << endl;
vcont(P);
cout << "M = " << M.size() << endl;
vcont(M);
cout << "C = " << C.size() << endl;
vcont(C);
cout << "T = " << T.size() << endl;
vcont(T);
cout << "Q = " << P.size() + 2 * M.size() + 3 * C.size() + 0.5 * T.size() << endl;
cout << endl << "i/o Chepin" << endl << endl;
P.clear();
M.clear();
C.clear();
T.clear();
for (auto i : priors) {
	if (i_o[i.first])
		switch (i.second) {
		case 1:
			P.push_back(i.first);
			break;
		case 2:
			M.push_back(i.first);
			break;
		case 3:
			C.push_back(i.first);
			break;
		case 4:
			T.push_back(i.first);
			break;
		}
}
cout << "P = " << P.size() << endl;
vcont(P);
cout << "M = " << M.size() << endl;
vcont(M);
cout << "C = " << C.size() << endl;
vcont(C);
cout << "T = " << T.size() << endl;
vcont(T);
cout << "Q = " << P.size() + 2 * M.size() + 3 * C.size() + 0.5 * T.size() << endl;
fout.close();
	system("pause");
	return 0;
}
 




/*	regex num("-?[[:digit:]]+|-?[[:digit:]]+\\.[[:digit:]]+|#\\w+|#\\w+\\s=\\s\\w+");
	regex perem("([a-z]+|[0-9]+)(\\.|\\_)?([a-z]+|[0-9]+)\\s=|([a-z]+|[0-9]+)\\s=|([a-z]+|[0-9]+)(\\.|\\_)?[a-z]+(\\.|\\_)?([a-z]+|[0-9]+)\\s=");
	regex dops("[><\\*]?=|\\+\\+|;|\\s\\+\\s|\\s\\-\\s|\\.");
	regex fncs("def\\s[a-z]+|\\.[a-z]+\\(|\\s[a-z]+\\(|\\s[a-z]+\\.[a-z]+\\(|\\s[a-z](\\.|\\_)?[A-Z]?[a-z]+(\\.|\\_)?[a-z]+(\\_|\\.)[a-z]+\\(|(\\s|\\.)?\\w+\\(|\\w+\\.?\\w+\\(|\\.format\\(|\\.say\\(");
	regex ted("\\(\\s\\)|\\{\\s\\}|\\)\\s\\{|\\}\\se|e\\s\\{");
	regex usl("for|if\\selse|if|while|elif");
	ifstream pfin("python.txt");
	ofstream fout("edited.txt");
	map<string, int> operators;
	map<string, int> operands;
	while (!pfin.eof()) {
		string t;
		getline(pfin, t);
		string nt = regex_replace(t, num, " ");
		fout << nt << endl;
	}
	fout.close();
	ifstream fin("edited.txt");
	while (!fin.eof()) {
		string t;
		getline(fin,t);
		sregex_iterator next(t.begin(), t.end(), perem);
		sregex_iterator end;
		while (next != end) {
			smatch a = *next;
			string abc = a.str();
			if (abc[abc.length() - 1] == '=') abc.replace(abc.length() - 1, abc.length(), "");
			operands[abc]++;
			next++;
		}
	}
	fin.close();
	ifstream pt("edited.txt");
	ofstream ft("edited2.txt");
	while (!pt.eof()) {
		string t;
		getline(pt, t);
		string nt = regex_replace(t, perem, " ");
		ft << nt << endl;
	}
	pt.close();
	ft.close();
	ifstream pt1("edited2.txt");
	ofstream ft1("edited.txt");
	while (!pt1.eof()) {
		string t;
		getline(pt1, t);
		sregex_iterator next(t.begin(), t.end(), dops);
		sregex_iterator end;
		while (next != end) {
			smatch a = *next;
			operators[a.str()]++;
			next++;
		}
		string nt = regex_replace(t, dops, " ");
		ft1 << nt << endl;
	}

	ft1.close();
	pt1.close();
	ifstream ft2("edited.txt");
	string al = "";
	while (!ft2.eof()) {
		string s;
		getline(ft2,s);
		al += s + " ";
	}
	{
		sregex_iterator next(al.begin(), al.end(), fncs);
		sregex_iterator end;
		string nnn;
		while (next != end) {
			smatch a = *next;
			nnn = a.str();
			if (nnn[nnn.length()-1] == '(') nnn.replace(nnn.length()-1, nnn.length(), "");
			if (nnn[0] == '.') nnn.replace(0, 1, "");
			operators[nnn]++;
			next++;
		}
	}
	al = regex_replace(al, fncs, "");
	al = regex_replace(al, ted, "");
	al = regex_replace(al, regex("\\s\\s"), " ");
	{
		sregex_iterator next(al.begin(), al.end(), usl);
		sregex_iterator end;
		while (next != end) {
			smatch a = *next;
			string nnn = a.str();
			if (nnn == "if else") nnn = "if else";
			operators[nnn]++;
			next++;
		}
	}

	TextTable oprt;
	int sum = 0, n1 = 0, n2 = 0, nn1 = 0, nn2 = 0;
	oprt.add("");
	oprt.add("Operator");
	oprt.add("Amount");
	oprt.endOfRow();
	for (auto i : operators) {
		oprt.add("");
		oprt.add(i.first);
		stringstream ss;
		sum += i.second;
		ss << i.second;
		oprt.add(ss.str());
		oprt.endOfRow();
	}
	{
		oprt.add("Sum");
		stringstream t1, t2;
		t1 << operators.size();
		oprt.add(t1.str());
		n1 = operators.size();
		nn1 = sum;
		t2 << sum;
		oprt.add(t2.str());
		oprt.endOfRow();
	}
	TextTable opnd;
	sum = 0;
	opnd.add("");
	opnd.add("Operand");
	opnd.add("Amount");
	opnd.endOfRow();
	for (auto i : operands) {
		opnd.add("");
		opnd.add(i.first);
		stringstream ss;
		sum += i.second;
		ss << i.second;
		opnd.add(ss.str());
		opnd.endOfRow();
	}
	{
		opnd.add("Sum");
		stringstream t1, t2;
		t1 << operands.size();
		n2 = operands.size();
		nn2 = sum;
		opnd.add(t1.str());
		t2 << sum;
		opnd.add(t2.str());
		opnd.endOfRow();
	}
	ofstream result("result.txt");
	result << oprt << endl << opnd << endl;
	result << "n1 = " << n1 << endl << "n2 = " << n2 << endl << "N1 = " << nn1 << endl << "N2 = " << nn2 << endl << "n = " << n1 + n2 << endl << "N = " << nn1 + nn2 << endl << "V = " << double(nn1 + nn2) * log(double(n1 + n2)) / log(2.);
//	cout << oprt << endl << opnd << endl;
	//cout << "n1 = " << n1 << endl << "n2 = " << n2 << endl << "N1 = " << nn1 << endl << "N2 = " << nn2 << endl << "n = " << n1 + n2 << endl << "N = " << nn1 + nn2 << endl << "V = " << double(nn1 + nn2) * log(double(n1 + n2)) / log(2.)<<endl;
	string txt;
	int count = 1;
	regex len("\\s\\s\\s\\s");
	regex cycl("if\\s|for\\s|elif\\s|else\\s|while\\s");
	ifstream nest("python.txt");
	map<string, int> cycls;
	map<int, int> lines;
	while (getline(nest, txt))
	{
		if (txt.find("if ") != std::string::npos ||
			txt.find("elif ") != std::string::npos ||
			txt.find("for ") != std::string::npos ||
			txt.find("else ") != std::string::npos ||
			txt.find("while ") != std::string::npos)
		{
			ptrdiff_t const match_count(distance(sregex_iterator(txt.begin(), txt.end(), len), sregex_iterator()));
			lines.insert(pair<int, int>(count, match_count));
		}
		count++;
	}
	nest.close();
	ifstream cycles("python.txt");
	while (getline(cycles, txt))
	{
		sregex_iterator next(txt.begin(), txt.end(), cycl);
		sregex_iterator end;
		while (next != end) {
			smatch a = *next;
			string abc = a.str();
			cycls[abc]++;
			next++;
		}
	}
	cycles.close();
	for (auto it = cycls.cbegin(); it != cycls.cend(); ++it)
	{
		cout << it->first << " " << it->second << "\n";
	}
	/*for (auto it = lines.cbegin(); it != lines.cend(); ++it)
	{
		cout << it->first << " " << it->second << "\n";
	}
	int CLsum = 0;
	for (auto i : cycls)
	{
		CLsum += i.second;
	}
	cout << "CL=" << CLsum << endl;

	unsigned currentMax = 0;
	unsigned arg_max = 0;

	for (auto it = lines.cbegin(); it != lines.cend(); ++it) {

		if (it->second > currentMax) {
			arg_max = it->first;
			currentMax = it->second;
		}
	}
	//cout << "On Line " << arg_max << " CLI= " << currentMax << endl;
	cout << "CLI= " << currentMax << endl;
	double cl = (double)CLsum/(double)nn1;
	cout << "cl=" <<  double(cl)<<endl; */