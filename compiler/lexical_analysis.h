#ifndef LEXICAL_ANALYSIS_H_INCLUDED
#define LEXICAL_ANALYSIS_H_INCLUDED
using namespace std;
class lexical_analysis{
public:
    lexical_analysis(vector<pair<int, int> >& sym, vector<string>& id, vector<string>& num);
private:
    string reserved_word[13] = {"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write"};
    // Ëã·û&&½ç·û
    string symbol[16] = {"+","-","*","/",":=","=","#","<",">","<=",">=","(",")",";",",","."};
    vector<pair<int, int> > sym;
    vector<string> sym_output;
    vector<string> id;
    vector<string> num;
    void GetSym(string input_str);
    int Reserve(string word);
    int ReserveSymbol(string word);
    int get_length(int num);
    bool IsSymbol(char ch);
    void display();

};

#endif // LEXICAL_ANALYSIS_H_INCLUDED
