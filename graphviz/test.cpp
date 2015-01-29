//clang++ -std=c++11 test.cpp && ./a.out && dot test.dot -Tpdf -otest.pdf && open test.pdf

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>

using namespace std;

typedef struct{
	vector<string> pins;
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
		comps[m[1].str()].pins.push_back(m[2].str());
		if(m[1].str() == m[3].str() && m[2].str() == m[4].str()){//if connected to itself
			//wert foo
		}else{
			string link;
			link.append(m[3].str());
			link.append("_");
			link.append(m[4].str());
			link.append(" -> ");
			link.append(m[1].str());
			link.append("_");
			link.append(m[2].str());
			link.append("[label=\"");
			link.append(m[5].str());
			link.append("\"];");
			links.push_back(link);
		}
		s = m.suffix().str();
	}

	dotfile << "digraph G {" << endl;
  dotfile << "rankdir = LR;" << endl;
  dotfile << "splines = spline;" << endl;
  //dotfile << "ratio = 1;" << endl;
  dotfile << "overlap = false;" << endl;
  dotfile << "start = regular;" << endl;
  dotfile << "forcelabels = true;" << endl;


	for(auto &comp:comps){
	  dotfile << "subgraph cluster_" << comp.first << "{" << endl;
	  dotfile << "  style = rounded;" << endl;
	  dotfile << "  label = \"" << comp.first << "\";" << endl;
		for(auto &pin:comp.second.pins){
			dotfile << "  " << comp.first << "_" << pin << " [shape = box, style = filled, color = lightgrey, label = \"" << pin << "\"];" << endl;
	  }
		dotfile << "}" << endl;
	}

	for(auto &link:links){
		dotfile << link << endl;
	}

	dotfile << "}" << endl;
	return 0;
}
