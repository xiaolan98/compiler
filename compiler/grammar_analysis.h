#ifndef GRAMMAR_ANALYSIS_H_INCLUDED
#define GRAMMAR_ANALYSIS_H_INCLUDED
#include<vector>
using namespace std;
struct node{
    string element;
    vector<node*> children;
    node* parent;
    node(string key){
        element = key;
    }
};
class grammar_analysis{
public:
    grammar_analysis(vector<pair<int, int> >sym, vector<string>id, vector<string> num);
private:
    node* root;
    node* pos_pointer;
    vector<pair<int, int> > sym;
    vector<string> id;
    vector<string> num;
    int countor=0;
    int program();// first={const,var,procedure,[a-z],if,while,call,read,write,begin,Пе}
    int subprogram();// first={const,var,procedure,[a-z],if,while,call,read,write,begin,Пе}
    int const_description();// first={const}
    int const_define();// first={[a-z]}
	int unsigned_int();// first={[0-9]}
	int variable_description();// first={var}
	int identifier();// first={[a-z]}
	int progress_description();// first={procedure}
	int progress_head();// first={procedure}
	int statement();// first={[a-z],if,while,call,read,write,begin,Пе}
	int assignment_statement();// first={[a-z]}
	int compound_statement();// first={begin}
	int condition();// first={odd,+,-,[a-z],[0-9],(}
	int expression();// first={+,-,[a-z],[0-9],(}
	int item();// first={[a-z],[0-9],(}
	int factor();// first={[a-z],[0-9],(}
	int add_sub_operator();// first={+,-}
	int mul_div_operator();// first={*,/}
	int relation_operator();// first={=,#,<,<=,>,>=}
	int conditional_statement();// first={if}
	int call_statement();// first={call}
	int when_type_loop_statement();// first={while}
	int read_statement();// first={read}
	int write_statement();// first={write}
	//int letter();// first={[a-z]}
	//int digit();// first={[0,9]}
	void drawTree(vector<node*> nodes, vector<int> nodeNum);
	void addNode(const string& key);

};

#endif // GRAMMAR_ANALYSIS_H_INCLUDED
