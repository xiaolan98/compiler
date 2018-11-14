#include<iostream>
#include<assert.h>
#include "grammar_analysis.h"
using namespace std;
// 程序 first={const,var,procedure,[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::program(){
    if(subprogram()==1){
        return 1;
    }
    else{
        return -1;
    }
}
// 分程序 first={const,var,procedure,[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::subprogram(){
    int temp;
    if(sym[countor].first==1){
        temp = const_description();
        if(temp==-1){
            return -1;
        }
    }
    if(sym[countor].first==2){
        temp = variable_description();
        if(temp==-1){
            return -1;
        }
    }
    if(sym[countor].first==3){
        temp = progress_description();
        if(temp==-1){
            return -1;
        }
    }
    if(sym[countor].first==30||sym[countor].first==7||sym[countor].first==10||sym[countor].first==9||sym[countor].first==12||sym[countor].first==13||sym[countor].first==4){
        temp = statement();
        if(temp==-1){
            return -1;
        }
    }
    return 1;
}
// 常量说明部分 first={const}
int grammar_analysis::const_description(){
    int temp;
    if(sym[countor].first==1){
        countor++;
    }
    else{
        cout<<"Error in const description of "<<sym[countor].first<<endl;
        return -1;
    }
    temp = const_define();
    if(temp==-1){
        return -1;
    }
    while(sym[countor].first==28){
        countor++;
        temp=const_define();
        if(temp==-1){
            return -1;
        }
    }
    assert(temp==1);
    return temp;//应该temp==1
}
// 常量定义 first={[a-z]}
int grammar_analysis::const_define(){
    int temp;
    if(sym[countor].first == 0){
        countor++;
    }
    else{
        cout<<"Error in const define of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first == 18){
        countor++;
    }
    else{
        cout<<"Error in const define of"<<sym[countor].first<<endl;
        return -1;
    }
    temp = unsigned_int();
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
        countor++;
    }
    else{
        cout<<"Error in variable description of "<<sym[countor].first<<endl;
        return -1;
    }
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    while(sym[countor].first==28){
        countor++;
        temp=identifier();
        if(temp==-1){
            return -1;
        }
    }
    assert(temp==1);
    return temp;
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
    temp = progress_head();
    if(temp==-1){
        return -1;
    }
    temp = subprogram();
    if(temp==-1){
        return -1;
    }
    if(sym[countor].first==27){
        countor++;
        temp = 1;
    }
    else{
        //cout<<"Error in progress description of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==3){// 针对此文法，此文法中procedure不可能出现在其他的地方
        return progress_description();
    }
    else{
        return 1;
    }
}
// 过程首部 first={procedure}
int grammar_analysis::progress_head(){
    if(sym[countor].first==3){
        countor++;
    }
    else{
        cout<<"Error in progress head of"<<sym[countor].first<<endl;
    }
    return identifier();
}
// 语句 first={[a-z],if,while,call,read,write,begin,空}
int grammar_analysis::statement(){
    if(sym[countor].first==0){
        return assignment_statement();
    }
    else if(sym[countor].first==7){
        return conditional_statement();
    }
    else if(sym[countor].first==10){
        return when_type_loop_statement();
    }
    else if(sym[countor].first==9){
        return call_statement();
    }
    else if(sym[countor].first==12){
        return read_statement();
    }
    else if(sym[countor].first==13){
        return write_statement();
    }
    else if(sym[countor].first==4){
        return compound_statement();
    }
    else{
        return 1;
    }
}
// 赋值语句 first={[a-z]}
int grammar_analysis::assignment_statement(){
    if(sym[countor].first==0){
        countor++;
    }
    else{
        cout<<"Error in assignment statement of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==18){
        countor++;
    }
    else{
        cout<<"Error in assignment statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return expression();
}
// 复合语句 first={begin}
int grammar_analysis::compound_statement(){
    int temp;
    if(sym[countor].first==4){
        countor++;
    }
    else{
        cout<<"Error in compound statement of"<<sym[countor].first<<endl;
        return -1;
    }
    temp = statement();
    if(temp == -1){
        return -1;
    }
    while(sym[countor].first==27){
        countor++;
        temp=statement();
        if(temp==-1){
            return -1;
        }
    }
    if(sym[countor].first==5){
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
        countor++;
        return expression();
    }
    else{
        temp = expression();
        if(temp==-1){
            return -1;
        }
        temp = relation_operator();
        if(temp==-1){
            return -1;
        }
        return expression();
    }
}
// 表达式 first={+,-,[a-z],[0-9],(}
int grammar_analysis::expression(){
    if(sym[countor].first==14||sym[countor].first==15){
        countor++;
    }
    int temp;
    temp = item();
    if(temp==-1){
        return -1;
    }
    while(sym[countor].first==14||sym[countor].first==15){
        add_sub_operator();
        temp = item();
        if(temp==-1){
        return -1;
        }
    }
    return 1;
}
// 项 first={[a-z],[0-9],(}
int grammar_analysis::item(){
    int temp;
    temp = factor();
    if(temp==-1){
        return -1;
    }
    while(sym[countor].first==16||sym[countor].first==17){
        mul_div_operator();
        temp = factor();
        if(temp==-1){
        return -1;
        }
    }
    return 1;
}
// 因子 first={[a-z],[0-9],(}
int grammar_analysis::factor(){
    int temp;
    if(sym[countor].first==0){
        return identifier();
    }
    else if(sym[countor].first==30){
        return unsigned_int();
    }
    else if(sym[countor].first==25){s
        temp = expression();
        if(temp==-1){
            return -1;
        }
        if(sym[countor].first==26){
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
        countor++;
    }
    else{
        cout<<"Error in conditional statement of"<<sym[countor].first<<endl;
        return -1;
    }
    temp = condition();
    if(temp==-1){
        return -1;
    }
    if(sym[countor].first==8){
        countor++;
    }
    else{
        cout<<"Error in conditional statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return statement();
}
// 过程调用语句 first={call}
int grammar_analysis::call_statement(){
    if(sym[countor].first==9){
        countor++;
    }
    else{
        cout<<"Error in call statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return identifier();
}
// 当型循环语句 first={while}
int grammar_analysis::when_type_loop_statement(){
    int temp;
    if(sym[countor].first==10){
        countor++;
    }
    else{
        cout<<"Error in while type loop statement of"<<sym[countor].first<<endl;
        return -1;
    }
    temp = condition();
    if(temp==-1){
        return -1;
    }
    if(sym[countor].first==11){
        countor++;
    }
    else{
        cout<<"Error in while type loop statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return statement();
}
// 读语句 first={read}
int grammar_analysis::read_statement(){
    int temp;
    if(sym[countor].first==12){
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==25){
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    while(sym[countor].first==28){
        countor++;
        temp = identifier();
        if(temp==-1){
            return -1;
        }
    }
    if(sym[countor].first==26){
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
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    if(sym[countor].first==25){
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    while(sym[countor].first==28){
        countor++;
        temp = identifier();
        if(temp==-1){
            return -1;
        }
    }
    if(sym[countor].first==26){
        countor++;
    }
    else{
        cout<<"Error in read statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return 1;
}
// 字母 first={[a-z]}
//int grammar_analysis::letter(){}
// 数字 first={[0-9]}
//int grammar_analysis::digit(){}


