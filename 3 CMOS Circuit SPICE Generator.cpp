#include <iostream>
#include <vector>
#include <string>
using namespace std;
int counter = 0;
int nodeCount = 0;
int nodeInverterCount = 1;

struct transistor {
	string name;
	string drain;
	string gate;
	string source;
	//	string body;
	string type;
};
bool validated(string input, vector <string>& functions);
vector<transistor> generatePunAndInverters(string input, vector<transistor>& inverters);
int nOfInverters(string input);
vector<transistor> generageNmos(vector<transistor> pmos, string input);
vector<transistor> generateSpice(string input);
int main() {

	string input;
	cout << "Enter the function without any spaces or parentheses\n";
	cin >> input;

	vector <string> functions;
	while (!validated(input, functions)) {
		functions.clear();
		cout << "invalid input\n";
		cout << "Enter the function without any spaces or parentheses\n";
		cin >> input;
	}
	for (int i = 0; i < functions.size(); i++)
		nodeCount = nodeCount + nOfInverters(functions[i]);
	//cout << "inverters: " << nodeCount << endl;
	nodeCount++;
	vector<transistor> test;
	vector< vector<transistor>> transistors;
	for (int i = 0; i < functions.size(); i++)
	{
		test = generateSpice(functions[i]);
		//for (int i = 0; i < test.size(); i++)
		//	cout << test[i].name << "\t" << test[i].drain << "\t" << test[i].gate << "\t" << test[i].source << "\t" << test[i].source << "\t" << test[i].type << "\n";
		//cout << endl;
		transistors.push_back(test);
		test.clear();
	}
	cout << endl;
	cout << "Name\tDrain\tGate\tSource\tBody\tType\n";
	for (int i = 0; i < transistors.size(); i++)
	{
		for (int j = 0; j < transistors[i].size(); j++)
			cout << transistors[i][j].name << "\t" << transistors[i][j].drain << "\t" << transistors[i][j].gate << "\t" << transistors[i][j].source << "\t" << transistors[i][j].source << "\t" << transistors[i][j].type << "\n";
		cout << endl;
	}
	//system("pause");
	return 0;
}

bool validated(string input, vector <string>& functions) {
	if (!((input[0] >= 'A' && input[0] <= 'Z') || (input[0] >= 'a' && input[0] <= 'z')))
		return false;
	if (input[1] != '=')
		return false;

	for (int i = 2; i < input.size(); i++)
		if (!((input[i] >= 'A' && input[i] <= 'Z') || (input[i] >= 'a' && input[i] <= 'z') || input[i] == 39 || input[i] == '|' || input[i] == '&' || input[i] == ';' || input[i] == '='))
			return false;

	int j = 0;
	for (int i = 2; i < input.size(); i++)
		if (input[i] == ';')
		{
			string s = input.substr(j, i - j);
			functions.push_back(s);
			j = i + 1;
		}
	string s = input.substr(j, input.size() - j);
	functions.push_back(s);

	//for (int i = 0; i < functions.size(); i++)
	//	cout << functions[i] << endl;
	return true;
}

//y=a'&b'&c'|d'|e'|f'&g'
vector<transistor> generatePunAndInverters(string input, vector<transistor>& inverters)
{
	vector<transistor> pmos;
	transistor temp;
	int currentDrainNextSource;
	bool afterOR = false;
	//assumed no repertition in the input
	for (int i = 2; i < input.size(); i++)
	{
		if (input[i] != '&' && input[i] != '|')
		{
			if (i + 1 < input.size() && input[i] != 39 && input[i + 1] == 39)
			{
				if (i + 2 < input.size() && input[i + 2] == '&')
				{
					if (pmos.size() == 0 || afterOR)
					{
						temp.source = "Vdd";
						afterOR = 0;
					}
					else
						temp.source = to_string(nodeCount++);

					temp.drain = to_string(nodeCount);
				}
				else if (i + 2 < input.size() && input[i + 2] == '|' || i == input.size() - 2) //i==input.size()-2 
				{
					if (pmos.size() == 0 || afterOR)
						temp.source = "Vdd";
					else
						temp.source = to_string(nodeCount++); //not applicable with the OR (|)
					temp.drain = input[0];
					afterOR = 1;
				}

				temp.gate = input[i];
				string str = to_string(counter++);
				temp.name = "M" + str;
				temp.type = "PMOS";
				pmos.push_back(temp);
			}
			else if (input[i] != 39)
			{
				transistor tempInverterPmos;
				tempInverterPmos.source = "Vdd";
				tempInverterPmos.gate = input[i];
				tempInverterPmos.drain = to_string(nodeInverterCount);
				string str = to_string(counter++);
				tempInverterPmos.name = "M" + str;
				tempInverterPmos.type = "PMOS";

				int inverterOutNode = nodeInverterCount;
				transistor tempInverterNmos;
				tempInverterNmos.source = "0";
				tempInverterNmos.gate = input[i];
				tempInverterNmos.drain = to_string(nodeInverterCount++);
				str = to_string(counter++);
				tempInverterNmos.name = "M" + str;
				tempInverterNmos.type = "NMOS";


				if (i + 1 < input.size() && input[i + 1] == '&')
				{
					if (pmos.size() == 0 || afterOR)
					{
						temp.source = "Vdd";
						afterOR = 0;
					}
					else
						temp.source = to_string(nodeCount++);

					temp.drain = to_string(nodeCount);
				}
				else if (i + 1 < input.size() && input[i + 1] == '|' || i == input.size() - 1) //i==input.size()-1 
				{
					if (pmos.size() == 0 || afterOR)
						temp.source = "Vdd";
					else
						temp.source = to_string(nodeCount++); //not applicable with the OR (|)
					temp.drain = input[0];
					afterOR = 1;
				}
				temp.gate = to_string(inverterOutNode);
				str = to_string(counter++);
				temp.name = "M" + str;
				temp.type = "PMOS";
				inverters.push_back(tempInverterPmos);
				inverters.push_back(tempInverterNmos);
				pmos.push_back(temp);
			}
		}
	}
	return pmos;
}

int nOfInverters(string input) {
	int counter = 0;
	for (int i = 2; i < input.size(); i++)
		if (!(input[i] == 39 || input[i] == '&' || input[i] == '|'))
			if (i + 1 < input.size() && input[i + 1] != 39)
				counter++;
	if (input[input.size() - 1] != 39)
		counter++;
	return counter;
}

vector<transistor> generageNmos(vector<transistor> pmos, string input) {
	vector<transistor> nmos;
	transistor temp;
	//nodeCount++;
	string drain;
	if (input.find('|') != -1)
		drain = to_string(nodeCount++);

	else
		drain = to_string(nodeCount);

	string source;
	bool lowestBranch = true;
	int countFirstBranch = 0;
	int countLastBranch = 0;

	for (int i = 0; i < pmos.size(); i++)
	{
		countFirstBranch++;
		if (pmos[i].drain.size() == 1 && pmos[i].drain[0] == input[0])
			break;
	}
	for (int i = pmos.size()-1; i >= 0; i--)
	{
		countLastBranch++;
		if (pmos[i].source == "Vdd")
			break;
	}
	for (int i = pmos.size() - 1; i >= 0; i--)
	{
		if (lowestBranch)
			temp.source = "0";
		else
			temp.source = source;

		if (pmos[i].source == "Vdd")
		{
			lowestBranch = 0;
			source = drain;
			temp.drain = drain;
			if (pmos.size()-i== countLastBranch)
				drain = to_string(nodeCount);
			else
				drain = to_string(nodeCount++);

		}
		else
			temp.drain = drain;
		if (i < countFirstBranch)
			temp.drain = input[0];

		temp.gate = pmos[i].gate;
		string str = to_string(counter++);
		temp.name = "M" + str;
		temp.type = "NMOS";
		nmos.push_back(temp);
	}

	return nmos;
}

vector<transistor> generateSpice(string input) {
	vector<transistor> transistors;
	vector<transistor> inverters;
	vector<transistor> pmos, nmos;
	pmos = generatePunAndInverters(input, inverters);
	nmos = generageNmos(pmos, input);

	for (int i = 0; i < pmos.size(); i++)
		transistors.push_back(pmos[i]);
	for (int i = 0; i < nmos.size(); i++)
		transistors.push_back(nmos[i]);
	for (int i = 0; i < inverters.size(); i++)
		transistors.push_back(inverters[i]);
	return transistors;
}
//y=a'&b'&c'|d'&e'|f'&g'

//y=a&b&c|d