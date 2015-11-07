/**
 * @author Emanuel B.
 * Utilitary to count sources and headers lines for statistics
 *
**/


#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <cstdio>
#include <dirent.h>
#include <sys/types.h>
using namespace std;

vector<string> read_directory(const string &);
pair<int,int> lineCount(const string);
ostream &operator<<(ostream &,const map<string,pair<int,int> > &);
pair<int,int> operator+(const pair<int,int> &,const pair<int,int> &);
pair<int,int> operator+=(pair<int,int> &,const pair<int,int> &);
double perc(const pair<int,int>);

int main() {
	char cstr[81];
	map<string,pair<int,int> > cppLines;
	map<string,pair<int,int> > hLines;
	pair<int,int> auxLine;
	pair<int,int> sumcpp = make_pair(0,0);
	pair<int,int> sumh = make_pair(0,0);
	pair<int,int> sumLine = make_pair(0,0);

	vector<string> possibleFiles = read_directory(string());
	if (!possibleFiles.empty()) {
		for (const auto &s : possibleFiles) {
			auxLine = lineCount(s);
			if (s.find(".cpp")!=string::npos && s.find(".cpp~")==string::npos) {
				cppLines.emplace(s,auxLine);
				sumcpp+=auxLine;				
			}
			else if (s.find(".h")!=string::npos && s.find(".h~")==string::npos) {
				hLines.emplace(s,auxLine);
				sumh+=auxLine;
			}
		}
		sumLine = sumh + sumcpp;
		sprintf(cstr,"%.2lf %%",perc(sumcpp));
		cout	<< "### RESULT:" << endl;
		cout	<< left << setw(25) << ".cpp File Name" << left << setw(10) << "Total"
				<< left << setw(10) << "Effe." << left << setw(10) << "Perc." << endl << endl << cppLines
				<< left << setw(25) << "@@@@@ TOTAL(CPP):" << left << setw(10) << sumcpp.first
				<< left << setw(10) << sumcpp.second
				<< left << setw(10) << string(cstr) << endl << endl;
		sprintf(cstr,"%.2lf %%",perc(sumh));
		cout	<< left << setw(25) << ".h File Name" << left << setw(10) << "Total"
				<< left << setw(10) << "Effe." << endl << endl << hLines
				<< left << setw(25) << "@@@@@ TOTAL(H):" << left << setw(10) << sumh.first
				<< left << setw(10) << sumh.second
				<< left << setw(10) << string(cstr) << endl << endl;
		sprintf(cstr,"%.2lf %%",perc(sumLine));
		cout	<< left << setw(25) << "@@@@@ TOTAL(GENERAL):" << left << setw(10) << sumLine.first
				<< left << setw(10) << sumLine.second
				<< left << setw(10) << string(cstr) << endl << endl;
	}
	return 0;
}


vector<string> read_directory(const string &path = string()) {
	vector<string> result;
	dirent *de;
	DIR *dp;
	errno = 0;
	dp = opendir(path.empty() ? "." : path.c_str());
	if (dp) {
		while (true) {
			errno = 0;
			de = readdir(dp);
			if (de == NULL) break;
			result.push_back(string(de->d_name));
		}
		closedir(dp);
		sort(result.begin(),result.end());
	}
	return result;
}

pair<int,int> lineCount(const string fname) {
	ifstream iFile(fname.c_str(),ios::in);
	string lin;
	if (!iFile) return make_pair(0,0);
	pair<int,int> result = make_pair(0,0);
	while (getline(iFile,lin)) {
		(result.first)++;
		if (lin.length()) {
			(result.second)++;
		}
	}
	return result;
}

ostream &operator<<(ostream &output,const map<string,pair<int,int> > &mapLine) {
	char cstr[81];
	for (const auto &i : mapLine) {
		sprintf(cstr,"%.2lf %%",perc(i.second));
		output	<< left << setw(25) << "@ " + i.first + ":" << left << setw(10) << (i.second).first
				<< left << setw(10) << (i.second).second
				<< left << setw(10) << string(cstr) << endl;
	}
	return output;
}

pair<int,int> operator+(const pair<int,int> &left,const pair<int,int> &right) {
	return make_pair(left.first+right.first,left.second+right.second);
}

pair<int,int> operator+=(pair<int,int> &left,const pair<int,int> &right) {
	left = left + right;
	return left;
}

double perc(const pair<int,int> p) {
	double i = static_cast<double>(p.first);
	double j = static_cast<double>(p.second);
	return (100.0*j/i);
}
