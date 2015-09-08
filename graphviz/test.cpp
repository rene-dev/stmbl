//clang++ -std=c++11 test.cpp && ./a.out && dot test.dot -Tpdf -otest.pdf && open test.pdf

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include <vector>

using namespace std;

typedef struct{
	string name;
	string drv_comp_name;
	string drv_pin_name;
	float value;
}pin_t;

typedef struct{
	string name;
	map<string,pin_t> pins;
}comp_t;

int main(){
	bool include_unlinked_comps = false;
	bool include_unlinked_pins = true;

	map<string, comp_t> comps;

	ifstream halfile("test.hal", fstream::in);
	ofstream dotfile("test.dot", fstream::out);
	string s((std::istreambuf_iterator<char>(halfile)), std::istreambuf_iterator<char>());
	smatch m;
	//m[]     1      2         3      4        5
	regex  e("(\\w+).(\\w+) <= (\\w+).(\\w+) = (.+)");
	while(std::regex_search (s,m,e)){
		comps[m[1].str()].name = m[1].str();
		comps[m[1].str()].pins[m[2].str()].name = m[2].str();
		comps[m[1].str()].pins[m[2].str()].drv_comp_name = m[3].str();
		comps[m[1].str()].pins[m[2].str()].drv_pin_name = m[4].str();
		comps[m[1].str()].pins[m[2].str()].value = stod(m[5].str());

		s = m.suffix().str();
	}

	dotfile << "digraph G {" << endl;
  dotfile << " rankdir = LR;" << endl;
	dotfile << " ranksep = 2.0;" << endl;
	//dotfile << " remincross = true;" << endl;
//	dotfile << " splines = true;" << endl;
dotfile << " splines = spline;" << endl;
 dotfile << " overlap = false;" << endl;
	//dotfile << " concentrate = true;" << endl;
  //dotfile << " start = regular;" << endl;
  //dotfile << " forcelabels = true;" << endl;
  dotfile << " nodesep = 0.5;" << endl;

	for(auto &comp:comps){
	  dotfile << " " << comp.second.name << "[shape = \"none\", label = <<table border=\"2\" cellspacing=\"0\">\n  <tr><td border=\"1\" bgcolor=\"#FFD08E\"> " << comp.second.name << "</td></tr>\n";

		for(auto &pin:comp.second.pins){
			if(include_unlinked_pins || comp.second.name != pin.second.drv_comp_name || pin.second.name != pin.second.drv_pin_name){
				dotfile << "  <tr><td port=\"" << pin.second.name << "\" border = \"1\"> " << pin.second.name << " = " << pin.second.value << " </td></tr>\n";
			}
	  }
	  dotfile << " </table>>];" << endl;
	}

	for(auto &comp:comps){
		for(auto &pin:comp.second.pins){
			if(comp.second.name != pin.second.drv_comp_name || pin.second.name != pin.second.drv_pin_name){
				dotfile << " " << pin.second.drv_comp_name << ":" << pin.second.drv_pin_name << " -> " << comp.second.name << ":" << pin.second.name << " [spines = \"ortho\"];" << endl;
			}
		}
	}

	dotfile << "}" << endl;
	return 0;
}
