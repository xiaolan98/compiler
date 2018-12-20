#include<iostream>
#include<vector>
#include<assert.h>
#include<fstream>
#include<stack>
#include<string>
#include "grammar_analysis.h"
using namespace std;
#define PROGRAM "program"
#define SUBPROGRAM "subprogram"
#define CONST_DESCRIPTION "const description"
#define CONST_DEFINE "const define"
#define UNSIGNED_INT "unsigned int"
#define VARIABLE_DESCRIPTION "variable description"
#define IDENTIFIER "identifier"
#define PROGRESS_DESCRIPTION "progress description"
#define PROGRESS_HEAD "progress head"
#define STATEMENT "statement"
#define ASSIGNMENT_STATEMENT "assignment statement"
#define COMPOUND_STATEMENT "compound statement"
#define CONDITION "condition"
#define EXPRESSION "expression"
#define ITEM "item"
#define FACTOR "factor"
#define ADD_SUB_OPERATOR "add sub operator"
#define MUL_DIV_OPERATOR "mul div operator"
#define RELATION_OPERATOR "relation operator"
#define CONDITIONAL_STATEMENT "conditional statement"
#define CALL_STATEMENT "call statement"
#define WHEN_TYPE_LOOP_STATEMENT "while type loop statement"
#define READ_STATEMENT "read statement"
#define WRITE_STATEMENT "write statement"
grammar_analysis::grammar_analysis(vector<pair<int, int> >sym, vector<string>id, vector<string> num, vector<code>& codeRepository){
        this->sym = sym;
        this->id = id;
        for(int i=0;i<num.size();i++){
            this->num.push_back(convertStringToInt(num[i]));
        }
        dx = 3;
        lev = 0;
        nodeIdCounter = 0;
        root = new node(PROGRAM,nodeIdCounter);
        nodeIdCounter++;
        pos_pointer = root;
        root->parent = NULL;
        allNode.push_back(root);
        program();
        codeRepository = this->codeRepo;
        vector<node*> nodes;
        vector<int> nodeNum;
        nodes.push_back(root);
        nodeNum.push_back(root->children.size());
        //drawTree(nodes, nodeNum);
        generateDot();
        //show_object_code_and_table();
}
void grammar_analysis::addNode(const string& key){
    //cout<<key<<endl;
    node* childnode = new node(key,nodeIdCounter);
    nodeIdCounter++;
    childnode->parent = pos_pointer;
    pos_pointer->children.push_back(childnode);
    pos_pointer = childnode;
    allNode.push_back(childnode);
}
// 程序 first={const,var,procedure,[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::program(){
    addNode(SUBPROGRAM);
    codeRepo.push_back(code("INT",0,-1));
    codeRepo.push_back(code("JMP",0,-1));
    int temp = subprogram(-1);
    if(temp==-1){
        return  -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[counter].first==29){
        addNode(".");
        pos_pointer = pos_pointer->parent;
        counter++;
        return 1;
    }
    else{
        cout<<"Error in program description of "<<sym[counter].first<<endl;
        return -1;
    }

}
// 分程序 first={const,var,procedure,[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::subprogram(int procedureAddr){
    lev++;
    dx = 3;
    int temp;
    if(sym[counter].first==1){
        addNode(CONST_DESCRIPTION);
        temp = const_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==2){
        addNode(VARIABLE_DESCRIPTION);
        temp = variable_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==3){
        addNode(PROGRESS_DESCRIPTION);
        temp = progress_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    addNode(STATEMENT);
    if(procedureAddr!=-1){
        table[procedureAddr].adr_or_value = codeRepo.size();
    }
    else{
        codeRepo[1].a = codeRepo.size();
        int intnum=0;
        for(int i =0;i<table.size();i++){
            if(table[i].kind=="PROCEDURE"){
                codeRepo[0].a = intnum;
                break;
            }
            if(table[i].kind=="VARIABLE"){
                intnum++;
            }
        }
    }
    temp = statement();
    if(temp==-1){
        return -1;
    }
    codeRepo.push_back(code("OPR",0,0));
    pos_pointer = pos_pointer->parent;
    lev--;
    return 1;
}
// 常量说明部分 first={const}
int grammar_analysis::const_description(){
    int temp;
    if(sym[counter].first==1){
        addNode("const");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in const description of "<<sym[counter].first<<endl;
        return -1;
    }
    addNode(CONST_DEFINE);
    temp = const_define();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(CONST_DEFINE);
        temp=const_define();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        counter++;
        return 1;
    }
    else{
        cout<<"Error in const description of "<<sym[counter].first<<endl;
        return -1;
    }
}
// 常量定义 first={[a-z]}
int grammar_analysis::const_define(){
    int temp;
    string tempname;
    int tempnum;
    if(sym[counter].first == 0){
        tempname = id[sym[counter].second];
        addNode(IDENTIFIER);
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in const define of "<<sym[counter].first<<endl;
        return -1;
    }
    if(sym[counter].first == 19){
        addNode("=");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in const define of "<<sym[counter].first<<endl;
        return -1;
    }
    addNode(UNSIGNED_INT);
    temp = unsigned_int();
    pos_pointer = pos_pointer->parent;
    tempnum = num[sym[counter-1].second];
    table.push_back(tableElement(tempname,"CONSTANT",lev,tempnum));
    return temp;
}
// 无符号整数 first={[0-9]}
int grammar_analysis::unsigned_int(){
    if(sym[counter].first==30){
        counter++;
        return 1;
    }
    else{
        cout<<"Error in defining unsigned int "<<sym[counter].first<<endl;
        return -1;
    }
}
// 变量说明部分 first={var}
int grammar_analysis::variable_description(){
    int temp;
    if(sym[counter].first==2){
        addNode("var");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in variable description of "<<sym[counter].first<<endl;
        return -1;
    }
    addNode(IDENTIFIER);
    temp = identifier();
    table.push_back(tableElement(id[sym[counter-1].second],"VARIABLE",lev,dx));
    dx++;
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(IDENTIFIER);
        temp=identifier();
        table.push_back(tableElement(id[sym[counter-1].second],"VARIABLE",lev,dx));
        dx++;
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        counter++;
        return 1;
    }
    else{
        cout<<"Error in variavle description of "<<sym[counter].first<<endl;
        return -1;
    }
}
// 标识符 first={[a-z]}
int grammar_analysis::identifier(){
    if(sym[counter].first==0){
        counter++;
        return 1;
    }
    else{
        cout<<"Error in defining identifier "<<sym[counter].first<<endl;
        return -1;
    }
}
// 过程说明部分 first={procedure}
int grammar_analysis::progress_description(){
    int temp;
    int procedureIndex;
    addNode(PROGRESS_HEAD);
    procedureIndex = table.size();
    table.push_back(tableElement(id[sym[counter+1].second],"PROCEDURE",lev,-1));
    temp = progress_head();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    addNode(SUBPROGRAM);
    temp = subprogram(procedureIndex);
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[counter].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        counter++;
        temp = 1;
    }
    else{
        //cout<<"Error in progress description of"<<sym[counter].first<<endl;
        return -1;
    }
    if(sym[counter].first==3){// 针对此文法，此文法中procedure不可能出现在其他的地方
        addNode(PROGRESS_DESCRIPTION);
        temp = progress_description();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else{
        return 1;
    }
}
// 过程首部 first={procedure}
int grammar_analysis::progress_head(){
    int temp;
    if(sym[counter].first==3){
        addNode("procedure");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in progress head of"<<sym[counter].first<<endl;
    }
    addNode(IDENTIFIER);
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[counter].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        counter++;
        return 1;
    }
    else{
        cout<<"Error in progress head of "<<sym[counter].first<<endl;
        return -1;
    }
}
// 语句 first={[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::statement(){
    int temp;
    if(sym[counter].first==0){
        addNode(ASSIGNMENT_STATEMENT);
        temp = assignment_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[counter].first==7){
        addNode(CONDITIONAL_STATEMENT);
        temp = conditional_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[counter].first==10){
        addNode(WHEN_TYPE_LOOP_STATEMENT);
        temp = when_type_loop_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[counter].first==9){
        addNode(CALL_STATEMENT);
        temp = call_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[counter].first==12){
        addNode(READ_STATEMENT);
        temp = read_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[counter].first==13){
        addNode(WRITE_STATEMENT);
        temp = write_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[counter].first==4){
        addNode(COMPOUND_STATEMENT);
        temp = compound_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else{
        addNode("empty");
        pos_pointer = pos_pointer->parent;
        return 1;
    }
}
// 赋值语句 first={[a-z]}
int grammar_analysis::assignment_statement(){
    int indexOfVriable;
    if(sym[counter].first==0){
        indexOfVriable = IsInTable(id[sym[counter].second]);
        if(indexOfVriable==-1){
            cout<<id[sym[counter].second]<<"is used before statement"<<endl;
            return -1;
        }
        addNode(IDENTIFIER);
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in assignment statement of"<<sym[counter].first<<endl;
        return -1;
    }
    if(sym[counter].first==18){
        addNode(":=");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in assignment statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(EXPRESSION);
    int temp = expression();
    codeRepo.push_back(code("STO",lev-table[indexOfVriable].level,table[indexOfVriable].adr_or_value));
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 复合语句 first={begin}
int grammar_analysis::compound_statement(){
    int temp;
    if(sym[counter].first==4){
        addNode("begin");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in compound statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(STATEMENT);
    temp = statement();
    if(temp == -1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(STATEMENT);
        temp=statement();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==5){
        addNode("end");
        pos_pointer = pos_pointer->parent;
        counter++;
        return 1;
    }
    else{
        cout<<"Error in compound statement of"<<sym[counter].first<<endl;
        return -1;
    }
}
// 条件 first={odd,+,-,[a-z],[0-9],(}
int grammar_analysis::condition(){
    int temp;
    int tempsym;
    if(sym[counter].first==6){
        tempsym = 6;
        addNode("odd");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(STATEMENT);
        temp = expression();
        codeRepo.push_back(code("OPR",0,11));
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else{
        addNode(EXPRESSION);
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        addNode(RELATION_OPERATOR);
        tempsym = sym[counter].first;
        temp = relation_operator();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        addNode(EXPRESSION);
        temp = expression();
        pos_pointer = pos_pointer->parent;
        if(tempsym==19){
            codeRepo.push_back(code("OPR",0,5));
        }
        else if(tempsym==20){
            codeRepo.push_back(code("OPR",0,6));
        }
        else if(tempsym==21){
            codeRepo.push_back(code("OPR",0,7));
        }
        else if(tempsym==22){
            codeRepo.push_back(code("OPR",0,8));
        }
        else if(tempsym==23){
            codeRepo.push_back(code("OPR",0,9));
        }
        else if(tempsym==24){
            codeRepo.push_back(code("OPR",0,10));
        }
        return temp;
    }
}
// 表达式 first={+,-,[a-z],[0-9],(}
int grammar_analysis::expression(){
    int sign = 0;
    if(sym[counter].first==14||sym[counter].first==15){
        if(sym[counter].first==15){
            codeRepo.push_back(code("LIT",0,0));
            sign = 1;
        }
        addNode("[+|-]");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    int temp;
    addNode(ITEM);
    temp = item();
    if(temp==-1){
        return -1;
    }
    if(sign==1){
        codeRepo.push_back(code("OPR",0,2));
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==14||sym[counter].first==15){
        int tempsym = sym[counter].first;
        addNode(ADD_SUB_OPERATOR);
        add_sub_operator();
        pos_pointer = pos_pointer->parent;
        addNode(ITEM);
        temp = item();
        if(temp==-1){
        return -1;
        }
        if(tempsym==14){
            codeRepo.push_back(code("OPR",0,1));
        }
        else if(tempsym==15){
            codeRepo.push_back(code("OPR",0,2));
        }
        pos_pointer = pos_pointer->parent;
    }
    return 1;
}
// 项 first={[a-z],[0-9],(}
int grammar_analysis::item(){
    int temp;
    addNode(FACTOR);
    temp = factor();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==16||sym[counter].first==17){
        int tempsym = sym[counter].first;
        addNode(MUL_DIV_OPERATOR);
        mul_div_operator();
        pos_pointer = pos_pointer->parent;
        addNode(FACTOR);
        temp = factor();
        if(temp==-1){
        return -1;
        }
        if(tempsym==16){
            codeRepo.push_back(code("OPR",0,3));
        }
        else if(tempsym==17){
            codeRepo.push_back(code("OPR",0,4));
        }
        pos_pointer = pos_pointer->parent;
    }
    return 1;
}
// 因子 first={[a-z],[0-9],(}
int grammar_analysis::factor(){
    int temp;
    if(sym[counter].first==0){
        addNode(IDENTIFIER);
        int tempindex = IsInTable(id[sym[counter].second]);
        //cout<<id[sym[counter].second]<<"  "<<tempindex<<endl;
        if(tempindex==-1){
            cout<<id[sym[counter].second]<<"is used before statement"<<endl;
            return -1;
        }
        else if(table[tempindex].kind=="CONSTANT"){
            codeRepo.push_back(code("LIT",0,table[tempindex].adr_or_value));
        }
        else if(table[tempindex].kind=="VARIABLE"){
            codeRepo.push_back(code("LOD",lev-table[tempindex].level,table[tempindex].adr_or_value));
        }
        temp = identifier();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else if(sym[counter].first==30){
        addNode(UNSIGNED_INT);
        codeRepo.push_back(code("LIT",0,num[sym[counter].second]));
        temp = unsigned_int();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else if(sym[counter].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(EXPRESSION);
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        if(sym[counter].first==26){
            addNode(")");
            pos_pointer = pos_pointer->parent;
            counter++;
            return -1;
        }
    }
    else{
        return -1;
    }

}
// 加减运算符 first={+,-}
int grammar_analysis::add_sub_operator(){
    if(sym[counter].first==14||sym[counter].first==15){
        counter++;
        return 1;
    }
    else{
        cout<<"Error in addition and subtract operator of "<<sym[counter].first<<endl;
        return -1;
    }
}
// 乘除运算符 first={*,/}
int grammar_analysis::mul_div_operator(){
    if(sym[counter].first==16||sym[counter].first==17){
        counter++;
        return 1;
    }
    else{
        cout<<"Error in multiple and division operator of "<<sym[counter].first<<endl;
        return -1;
    }
}
// 关系运算符 first={=,#,<,<=,>,>=}
int grammar_analysis::relation_operator(){
    if(sym[counter].first==19||sym[counter].first==20||sym[counter].first==21||sym[counter].first==22||sym[counter].first==23||sym[counter].first==24){
        counter++;
        return 1;
    }
    else{
        cout<<"Error in relation operator of "<<sym[counter].first<<endl;
        return -1;
    }
}
// 条件语句 first={if}
int grammar_analysis::conditional_statement(){
    int temp;
    if(sym[counter].first==7){
        addNode("if");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in conditional statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(CONDITION);
    temp = condition();
    int jpcindex = codeRepo.size();
    codeRepo.push_back(code("JPC",0,-1));
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[counter].first==8){
        addNode("then");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in conditional statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(STATEMENT);
    temp = statement();
    codeRepo[jpcindex].a = codeRepo.size();
    return temp;
}
// 过程调用语句 first={call}
int grammar_analysis::call_statement(){
    if(sym[counter].first==9){
        addNode("call");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in call statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(IDENTIFIER);
    int stackcount=0;
    int procedureIndex;//没有考虑引用没有声明的过程
    for(int i=0;i<table.size();i++){
        if(table[i].name==id[sym[counter].second]){
            procedureIndex = i;
            i++;
            while(i<table.size()&&table[i].kind!="PROCEDURE"){
                if(table[i].kind=="VARIABLE"){
                    stackcount++;
                }
                i++;
            }
            break;
        }
    }
    int temp = identifier();
    codeRepo.push_back(code("INT",0,stackcount));
    codeRepo.push_back(code("CAL",table[procedureIndex].level+1-lev,table[procedureIndex].adr_or_value));
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 当型循环语句 first={while}
int grammar_analysis::when_type_loop_statement(){
    int temp;
    if(sym[counter].first==10){
        addNode("while");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in while type loop statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(CONDITION);
    int jmpAddr = codeRepo.size();
    temp = condition();
    int jpcindex = codeRepo.size();
    codeRepo.push_back(code("JPC",0,-1));
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[counter].first==11){
        addNode("do");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in while type loop statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(STATEMENT);
    temp = statement();
    codeRepo.push_back(code("JMP",0,jmpAddr));
    codeRepo[jpcindex].a = codeRepo.size();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 读语句 first={read}
int grammar_analysis::read_statement(){
    int temp;
    if(sym[counter].first==12){
        addNode("read");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in read statement of"<<sym[counter].first<<endl;
        return -1;
    }
    if(sym[counter].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in read statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(IDENTIFIER);
    codeRepo.push_back(code("OPR",0,12));
    int tempindex = IsInTable(id[sym[counter].second]);
    if(tempindex==-1){
        cout<<id[sym[counter].second]<<"is used before statement"<<endl;
        return -1;
    }
    else if(table[tempindex].kind=="CONSTANT"){
        cout<<"Constant can not be reassigned"<<endl;
    }
    else if(table[tempindex].kind=="VARIABLE"){
        codeRepo.push_back(code("STO",lev-table[tempindex].level,table[tempindex].adr_or_value));
    }
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(IDENTIFIER);
        codeRepo.push_back(code("OPR",0,12));
        int tempindex = IsInTable(id[sym[counter].second]);
        if(tempindex==-1){
            cout<<id[sym[counter].second]<<"is used before statement"<<endl;
            return -1;
        }
        else if(table[tempindex].kind=="CONSTANT"){
            cout<<"Constant can not be reassigned"<<endl;
        }
        else if(table[tempindex].kind=="VARIABLE"){
            codeRepo.push_back(code("STO",lev-table[tempindex].level,table[tempindex].adr_or_value));
        }
        temp = identifier();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==26){
        addNode(")");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in read statement of"<<sym[counter].first<<endl;
        return -1;
    }
    return 1;
}
// 写语句 first={write}
int grammar_analysis::write_statement(){
    int temp;
    if(sym[counter].first==13){
        addNode("write");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in write statement of"<<sym[counter].first<<endl;
        return -1;
    }
    if(sym[counter].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in write statement of"<<sym[counter].first<<endl;
        return -1;
    }
    addNode(EXPRESSION);
    temp = expression();
    codeRepo.push_back(code("OPR",0,13));
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[counter].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        counter++;
        addNode(EXPRESSION);
        temp = expression();
        codeRepo.push_back(code("OPR",0,13));
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[counter].first==26){
        addNode(")");
        pos_pointer = pos_pointer->parent;
        counter++;
    }
    else{
        cout<<"Error in write statement of"<<sym[counter].first<<endl;
        return -1;
    }
    return 1;
}
// 字母 first={[a-z]}
//int grammar_analysis::letter(){}
// 数字 first={[0-9]}
//int grammar_analysis::digit(){}
void grammar_analysis::drawTree(vector<node*> nodes, vector<int> nodeNum){
    if(nodes.size()==0){
        return;
    }
    for(int i=0;i<nodeNum.size();i++){
        cout<<nodeNum[i];
    }
    cout<<endl;
    vector<node*> childnodes;
    vector<int> childNum;
    int countnum = 0;
    int countchild = 0;
    for(int i=0;i<nodes.size();i++){
        cout<<nodes[i]->element;
        countchild++;
        while(nodeNum[countnum]==0){
            countnum++;
        }
        if(nodeNum[countnum]==countchild){
            cout<<"  ";
            countnum++;
            countchild = 0;
        }
        childNum.push_back(nodes[i]->children.size());
        for(int j=0;j<nodes[i]->children.size();j++){
            childnodes.push_back(nodes[i]->children[j]);
        }
    }
    cout<<endl;
    drawTree(childnodes,childNum);
}
void grammar_analysis::generateDot(){
    ofstream out("image.gv");
    out<<"digraph tree{"<<endl;
    for(int i=0;i<allNode.size();i++){
        out<<"element"<<allNode[i]->nodeId<<"[label="<<"\""<<allNode[i]->element<<"\"]"<<endl;
    }
    stack<node*> nodeStack;
    nodeStack.push(root);
    node* ptr;
    while(!nodeStack.empty()){
        ptr = nodeStack.top();
        if(ptr->parent){
            out<<"element"<<ptr->parent->nodeId<<" -> "<<"element"<<ptr->nodeId<<endl;
        }
        nodeStack.pop();
        for(int i=0;i<ptr->children.size();i++){
            nodeStack.push(ptr->children[i]);
        }
    }
    out<<"}";
    out.close();
}
int grammar_analysis::convertStringToInt(const string &s){
	int val;
	strstream ss;
	ss << s;
	ss >> val;
	return val;
}
int grammar_analysis::IsInTable(string entity){
    int nextLevel = lev;
    for(int i=table.size()-1;i>=0;i--){
        if(table[i].name=="PROCEDURE"){
            nextLevel = table[i].level;
        }
        if(table[i].level>nextLevel){
            continue;
        }
        if(table[i].name == entity){
            return i;
        }
    }
    return -1;
}
void grammar_analysis::show_object_code_and_table(){
    for(int i=0;i<table.size();i++){
            cout<<i<<"   "<<table[i].name<<"    "<<table[i].kind<<"    "<<table[i].level<<"    "<<table[i].adr_or_value<<endl;
    }
    for(int i=0;i<codeRepo.size();i++){
        cout<<i<<"   "<<codeRepo[i].f<<"   "<<codeRepo[i].l<<"   "<<codeRepo[i].a<<endl;
    }
}
