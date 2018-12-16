#include"grammar_analysis.h"
#include"lexical_analysis.h"
#include"interpreter.h"
#include<iostream>
using namespace std;
vector<pair<int, int> > sym;
vector<string> id;
vector<string> num;
vector<code> codeRepo;
int main(){
    lexical_analysis la(sym,id,num);
    grammar_analysis ga(sym,id,num,codeRepo);
    interpreter interpret(codeRepo);
return 0;
}
