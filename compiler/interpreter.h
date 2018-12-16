#ifndef INTERPRETER_H_INCLUDED
#define INTERPRETER_H_INCLUDED
#include"grammar_analysis.h"
class interpreter{
public:
    interpreter(vector<code> codeRepo);
private:
    void execute(vector<code> codeRepo);
};


#endif // INTERPRETER_H_INCLUDED
