#include<iostream>
#include<vector>
#include<assert.h>
#include "grammar_analysis.h"

using namespace std;
grammar_analysis::grammar_analysis(vector<pair<int, int> >sym, vector<string>id, vector<string> num){
        this->sym = sym;
        this->id = id;
        this->num = num;
        root = new node("程序");
        pos_pointer = root;
        root->parent = NULL;
        program();
        vector<node*> nodes;
        vector<int> nodeNum;
        nodes.push_back(root);
        nodeNum.push_back(root->children.size());
        drawTree(nodes, nodeNum);
}
void grammar_analysis::addNode(const string& key){
    //cout<<key<<endl;
    node* childnode = new node(key);
    childnode->parent = pos_pointer;
    pos_pointer->children.push_back(childnode);
    pos_pointer = childnode;
}
// 程序 first={const,var,procedure,[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::program(){
    addNode("分程序");
    int temp = subprogram();
    if(temp==-1){
        return  -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[countor].first==29){
        addNode(".");
        pos_pointer = pos_pointer->parent;
        countor++;
        return 1;
    }
    else{
        cout<<"Error in program description of "<<sym[countor].first<<endl;
        return -1;
    }

}
// 分程序 first={const,var,procedure,[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::subprogram(){
    int temp;
    if(sym[countor].first==1){
        addNode("常量说明部分");
        temp = const_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==2){
        addNode("变量说明部分");
        temp = variable_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==3){
        addNode("过程说明部分");
        temp = progress_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    //if(sym[countor].first==30||sym[countor].first==7||sym[countor].first==10||sym[countor].first==9||sym[countor].first==12||sym[countor].first==13||sym[countor].first==4){
    addNode("语句");
    temp = statement();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    return 1;
}
// 常量说明部分 first={const}
int grammar_analysis::const_description(){
    int temp;
    if(sym[countor].first==1){
        addNode("const");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in const description of "<<sym[countor].first<<endl;
        return -1;
    }
    addNode("常量定义");
    temp = const_define();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("常量定义");
        temp=const_define();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        countor++;
        return 1;
    }
    else{
        cout<<"Error in const description of "<<sym[countor].first<<endl;
        return -1;
    }
}
// 常量定义 first={[a-z]}
int grammar_analysis::const_define(){
    int temp;
    if(sym[countor].first == 0){
        addNode("标识符");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in const define of "<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first == 19){
        addNode("=");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in const define of "<<sym[countor].first<<endl;
        return -1;
    }
    addNode("数字");
    temp = unsigned_int();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 无符号整数 first={[0-9]}
int grammar_analysis::unsigned_int(){
    if(sym[countor].first==30){
        countor++;
        return 1;
    }
    else{
        cout<<"Error in defining unsigned int "<<sym[countor].first<<endl;
        return -1;
    }
}
// 变量说明部分 first={var}
int grammar_analysis::variable_description(){
    int temp;
    if(sym[countor].first==2){
        addNode("var");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in variable description of "<<sym[countor].first<<endl;
        return -1;
    }
    addNode("标识符");
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("标识符");
        temp=identifier();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        countor++;
        return 1;
    }
    else{
        cout<<"Error in variavle description of "<<sym[countor].first<<endl;
        return -1;
    }
}
// 标识符 first={[a-z]}
int grammar_analysis::identifier(){
    if(sym[countor].first==0){
        countor++;
        return 1;
    }
    else{
        cout<<"Error in defining identifier "<<sym[countor].first<<endl;
        return -1;
    }
}
// 过程说明部分 first={procedure}
int grammar_analysis::progress_description(){
    int temp;
    addNode("过程头部");
    temp = progress_head();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    addNode("分程序");
    temp = subprogram();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[countor].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        countor++;
        temp = 1;
    }
    else{
        //cout<<"Error in progress description of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==3){// 针对此文法，此文法中procedure不可能出现在其他的地方
        addNode("过程部分说明");
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
    if(sym[countor].first==3){
        addNode("procedure");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in progress head of"<<sym[countor].first<<endl;
    }
    addNode("标识符");
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[countor].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        countor++;
        return 1;
    }
    else{
        cout<<"Error in progress head of "<<sym[countor].first<<endl;
        return -1;
    }
}
// 语句 first={[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::statement(){
    int temp;
    if(sym[countor].first==0){
        addNode("赋值语句");
        temp = assignment_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==7){
        addNode("条件语句");
        temp = conditional_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==10){
        addNode("当型循环语句");
        temp = when_type_loop_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==9){
        addNode("过程调试语句");
        temp = call_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==12){
        addNode("读语句");
        temp = read_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==13){
        addNode("写语句");
        temp = write_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==4){
        addNode("复合语句");
        temp = compound_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else{
        addNode("空");
        pos_pointer = pos_pointer->parent;
        return 1;
    }
}
// 赋值语句 first={[a-z]}
int grammar_analysis::assignment_statement(){
    if(sym[countor].first==0){
        addNode("标识符");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in assignment statement of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==18){
        addNode(":=");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in assignment statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("表达式");
    int temp = expression();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 复合语句 first={begin}
int grammar_analysis::compound_statement(){
    int temp;
    if(sym[countor].first==4){
        addNode("begin");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in compound statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("语句");
    temp = statement();
    if(temp == -1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("语句");
        temp=statement();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==5){
        addNode("end");
        pos_pointer = pos_pointer->parent;
        countor++;
        return 1;
    }
    else{
        cout<<"Error in compound statement of"<<sym[countor].first<<endl;
        return -1;
    }
}
// 条件 first={odd,+,-,[a-z],[0-9],(}
int grammar_analysis::condition(){
    int temp;
    if(sym[countor].first==6){
        addNode("odd");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("语句");
        temp = expression();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else{
        addNode("表达式");
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        addNode("关系运算符");
        temp = relation_operator();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        addNode("表达式");
        temp = expression();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
}
// 表达式 first={+,-,[a-z],[0-9],(}
int grammar_analysis::expression(){
    if(sym[countor].first==14||sym[countor].first==15){
        addNode("[+|-]");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    int temp;
    addNode("项");
    temp = item();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==14||sym[countor].first==15){
        addNode("加减运算符");
        add_sub_operator();
        pos_pointer = pos_pointer->parent;
        addNode("项");
        temp = item();
        if(temp==-1){
        return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    return 1;
}
// 项 first={[a-z],[0-9],(}
int grammar_analysis::item(){
    int temp;
    addNode("因子");
    temp = factor();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==16||sym[countor].first==17){
        addNode("乘除运算符");
        mul_div_operator();
        pos_pointer = pos_pointer->parent;
        addNode("因子");
        temp = factor();
        if(temp==-1){
        return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    return 1;
}
// 因子 first={[a-z],[0-9],(}
int grammar_analysis::factor(){
    int temp;
    if(sym[countor].first==0){
        addNode("标识符");
        temp = identifier();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else if(sym[countor].first==30){
        addNode("无符号整数");
        temp = unsigned_int();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else if(sym[countor].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("表达式");
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        if(sym[countor].first==26){
            addNode(")");
            pos_pointer = pos_pointer->parent;
            countor++;
            return -1;
        }
    }
    else{
        return -1;
    }

}
// 加减运算符 first={+,-}
int grammar_analysis::add_sub_operator(){
    if(sym[countor].first==14||sym[countor].first==15){
        countor++;
        return 1;
    }
    else{
        cout<<"Error in addition and subtract operator of "<<sym[countor].first<<endl;
        return -1;
    }
}
// 乘除运算符 first={*,/}
int grammar_analysis::mul_div_operator(){
    if(sym[countor].first==16||sym[countor].first==17){
        countor++;
        return 1;
    }
    else{
        cout<<"Error in multiple and division operator of "<<sym[countor].first<<endl;
        return -1;
    }
}
// 关系运算符 first={=,#,<,<=,>,>=}
int grammar_analysis::relation_operator(){
    if(sym[countor].first==19||sym[countor].first==20||sym[countor].first==21||sym[countor].first==22||sym[countor].first==23||sym[countor].first==24){
        countor++;
        return 1;
    }
    else{
        cout<<"Error in relation operator of "<<sym[countor].first<<endl;
        return -1;
    }
}
// 条件语句 first={if}
int grammar_analysis::conditional_statement(){
    int temp;
    if(sym[countor].first==7){
        addNode("if");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in conditional statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("条件");
    temp = condition();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[countor].first==8){
        addNode("then");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in conditional statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("语句");
    temp = statement();
    return temp;
}
// 过程调用语句 first={call}
int grammar_analysis::call_statement(){
    if(sym[countor].first==9){
        addNode("call");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in call statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("标识符");
    int temp = identifier();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 当型循环语句 first={while}
int grammar_analysis::when_type_loop_statement(){
    int temp;
    if(sym[countor].first==10){
        addNode("while");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in while type loop statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("条件");
    temp = condition();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    if(sym[countor].first==11){
        addNode("then");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in while type loop statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("");
    temp = statement();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// 读语句 first={read}
int grammar_analysis::read_statement(){
    int temp;
    if(sym[countor].first==12){
        addNode("read");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("标识符");
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("标识符");
        temp = identifier();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==26){
        addNode(")");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return 1;
}
// 写语句 first={write}
int grammar_analysis::write_statement(){
    int temp;
    if(sym[countor].first==13){
        addNode("write");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in write statement of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in write statement of"<<sym[countor].first<<endl;
        return -1;
    }
    addNode("表达式");
    temp = expression();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("表达式");
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==26){
        addNode("）");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in write statement of"<<sym[countor].first<<endl;
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
    vector<node*> childnodes;
    vector<int> childNum;
    int countnum = 0;
    for(int i=0;i<nodes.size();i++){
        cout<<nodes[i]->element<<" ";
        if(nodeNum[countnum]==i+1){
            cout<<"  ";
            countnum++;
        }
        childNum.push_back(nodes[i]->children.size());
        for(int j=0;j<nodes[i]->children.size();j++){
            childnodes.push_back(nodes[i]->children[j]);
        }
    }
    cout<<endl;
    drawTree(childnodes,childNum);
}

