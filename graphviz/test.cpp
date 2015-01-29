//clang++ -std=c++11 test.cpp && ./a.out && dot test.dot -Tpdf -otest.pdf && open test.pdf

#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_set>
#include <map>
#include <vector>

using namespace std;

typedef struct{
	vector<string> inputs;
	vector<string> outputs;
}comp_t;

int main(){
	map<string,comp_t> comps;
	vector<string> links;
	ifstream halfile("test.hal", fstream::in);
	ofstream dotfile("test.dot", fstream::out);
	string s((std::istreambuf_iterator<char>(halfile)), std::istreambuf_iterator<char>());
	smatch m;
	//m[]     1      2         3      4        5
	regex  e("(\\w+).(\\w+) <= (\\w+).(\\w+) = (.+)");
	while(std::regex_search (s,m,e)){
		//if not connected to itself
		if(m[1].str() != m[3].str() && m[2].str() != m[4].str()){
			comps[m[3].str()].outputs.push_back(m[4].str());
			comps[m[1].str()].inputs.push_back(m[2].str());
			string link;
			link.append(m[3].str());
			link.append(":");
			link.append(m[4].str());
			link.append(" -> ");
			link.append(m[1].str());
			link.append(":");
			link.append(m[2].str());
			link.append("[label=\"");
			link.append(m[5].str());
			link.append("\"];");
			links.push_back(link);
		}
		s = m.suffix().str();
	}
	
	dotfile << "digraph G {" << endl;
	dotfile << "graph [rankdir = LR];" << endl;
	dotfile << "node[shape=record];" << endl;
	
	for(auto &comp:comps){
		dotfile << comp.first << "[label=\"{{";
		
		for(auto &input:comp.second.inputs){
			dotfile << "<" << input << ">" << input << "|";
		}
		dotfile << "}|" << comp.first << "|{";
		for(auto &output:comp.second.outputs){
			dotfile << "<" << output << ">" << output << "|";
		}
		dotfile << "}}\"];" << endl;
	}
	
	for(auto &link:links){
		dotfile << link << endl;
	}

	dotfile << "}" << endl;
	return 0;
}