#include<iostream>
#include<vector>
#include<fstream>
#include"grammar_analysis.h"
using namespace std;

#define NUMBERID 30 // 数字的内部编码表示
// 保留字
string reserved_word[13] = {"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write"};
// 算符&&界符
string symbol[16] = {"+","-","*","/",":=","=","#","<",">","<=",">=","(",")",";",",","."};

vector<pair<int, int> > sym;
vector<string> sym_output;
vector<string> id;
vector<string> num;
// 回车待处理
void GetSym(string input_str);

int Reserve(string word);
int ReserveSymbol(string word);

int Reserve(string word){
    int code_num = 0;
    int count_num = 0;
    for(int i=0;i<13;i++){
        count_num++;
        if(word == reserved_word[i]){
            code_num = count_num;
            return code_num;
        }
    }
    return code_num;
}

int ReserveSymbol(string word){
    int code_num = 13;
    for(int i=0;i<16;i++){
        code_num++;
        if(word == symbol[i]){
            return code_num;
        }
    }
    return 0;
}

bool IsSymbol(char ch){
    for(int i=0;i<16;i++){
        if(ch==symbol[i][0]){
            return 1;
        }
    }
    return 0;
}

void GetSym(string input_str){
int index = 0;
while(index<input_str.length()){
    int code;
    int value=-1;
    string strToken = "";
    while(index<input_str.length()&&input_str[index]==' '){
        index++;
    }
    if(index>=input_str.length()){
        return;
    }
    if(islower(input_str[index])){
        strToken+=input_str[index];
        index++;
        while(index<input_str.length()&&(islower(input_str[index])||isdigit(input_str[index]))){
            strToken+=input_str[index];
            index++;
        }
        code = Reserve(strToken);
        // value = -1;
        if(code==0){
            value = id.size();
            id.push_back(strToken);
        }
        sym.push_back(make_pair(code,value));
        sym_output.push_back(strToken);
    }
    else if(isdigit(input_str[index])){
        strToken+=input_str[index];
        index++;
        while(index<input_str.length()&&(isdigit(input_str[index]))){
            strToken+=input_str[index];
            index++;
        }
        code = NUMBERID;
        value = num.size();
        num.push_back(strToken);
        sym.push_back(make_pair(code,value));
        sym_output.push_back(strToken);
    }
    else if(IsSymbol(input_str[index])){
        strToken += input_str[index];
        index++;
        if(strToken=="<"||strToken==">"){
            if(index<input_str.length()&&input_str[index]=='='){
                strToken+="=";
                index++;
            }
        }
        if(strToken==":"){
            if(index<input_str.length()&&input_str[index]=='='){
                strToken+="=";
                index++;
            }
            else{
                cout<<"ERROR:"<<input_str<<endl;
                return;
            }
        }
        code = ReserveSymbol(strToken);
        sym.push_back(make_pair(code,value));
        sym_output.push_back(strToken);
    }
    else{
        cout<<"ERROR:"<<input_str<<endl;
        return;
    }
}


}

int get_length(int num){//计算数字占的位数
    if(num==0){
        return 1;
    }
    else{
        int length = 0;
        if(num<0){
            num = -num;
            length++;
        }
        while(num>0){
        num/=10;
        length++;
        }
        return length;
    }

}

int main(){
    ifstream in("test.txt");
    string line;
    while(getline(in,line)){
        GetSym(line);
    }
    in.close();
    for(int i=0;i<sym.size();i++){
        cout<<i+1<<'('<<sym[i].first<<' '<<sym[i].second<<')'<<endl;
    }
    for(int i=0;i<id.size();i++){
        cout<<id[i]<<endl;
    }
    cout<<endl;
    for(int i=0;i<num.size();i++){
        cout<<num[i]<<endl;
    }
    int first_length = 11;
    int second_length =11;
    cout<<"  symbol     class      address"<<endl;
    for(int i =0;i<sym_output.size();i++){
        cout<<i+1<<" ";
        string str1_temp = "";
        string str2_temp = "";
        for(int j=0;j<first_length-sym_output[i].size();j++){str1_temp+=" ";}
        for(int j=0;j<second_length-get_length(sym[i].first);j++){str2_temp+=" ";}
        cout<<sym_output[i]<<str1_temp<<sym[i].first<<str2_temp;
        if(sym[i].first==0){
            cout<<sym[i].second<<endl;
        }
        else if(sym[i].first==30){
            cout<<sym[i].second<<endl;
        }
        else{
            cout<<endl;
            continue;
        }
    }
    //grammar_analysis ga(sym,id,num);
return 0;
}
