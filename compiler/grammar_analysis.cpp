#include<iostream>
#include<vector>
#include<assert.h>
#include "grammar_analysis.h"

using namespace std;
grammar_analysis::grammar_analysis(vector<pair<int, int> >sym, vector<string>id, vector<string> num){
        this->sym = sym;
        this->id = id;
        this->num = num;
        root = new node("����");
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
// ���� first={const,var,procedure,[a-z],if,while,call,read,write,begin,��}
int grammar_analysis::program(){
    addNode("�ֳ���");
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
// �ֳ��� first={const,var,procedure,[a-z],if,while,call,read,write,begin,��}
int grammar_analysis::subprogram(){
    int temp;
    if(sym[countor].first==1){
        addNode("����˵������");
        temp = const_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==2){
        addNode("����˵������");
        temp = variable_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==3){
        addNode("����˵������");
        temp = progress_description();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    //if(sym[countor].first==30||sym[countor].first==7||sym[countor].first==10||sym[countor].first==9||sym[countor].first==12||sym[countor].first==13||sym[countor].first==4){
    addNode("���");
    temp = statement();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    return 1;
}
// ����˵������ first={const}
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
    addNode("��������");
    temp = const_define();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("��������");
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
// �������� first={[a-z]}
int grammar_analysis::const_define(){
    int temp;
    if(sym[countor].first == 0){
        addNode("��ʶ��");
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
    addNode("����");
    temp = unsigned_int();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// �޷������� first={[0-9]}
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
// ����˵������ first={var}
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
    addNode("��ʶ��");
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("��ʶ��");
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
// ��ʶ�� first={[a-z]}
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
// ����˵������ first={procedure}
int grammar_analysis::progress_description(){
    int temp;
    addNode("����ͷ��");
    temp = progress_head();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    addNode("�ֳ���");
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
    if(sym[countor].first==3){// ��Դ��ķ������ķ���procedure�����ܳ����������ĵط�
        addNode("���̲���˵��");
        temp = progress_description();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else{
        return 1;
    }
}
// �����ײ� first={procedure}
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
    addNode("��ʶ��");
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
// ��� first={[a-z],if,while,call,read,write,begin,��}
int grammar_analysis::statement(){
    int temp;
    if(sym[countor].first==0){
        addNode("��ֵ���");
        temp = assignment_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==7){
        addNode("�������");
        temp = conditional_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==10){
        addNode("����ѭ�����");
        temp = when_type_loop_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==9){
        addNode("���̵������");
        temp = call_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==12){
        addNode("�����");
        temp = read_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==13){
        addNode("д���");
        temp = write_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else if(sym[countor].first==4){
        addNode("�������");
        temp = compound_statement();
        pos_pointer = pos_pointer->parent;
        return 1;
    }
    else{
        addNode("��");
        pos_pointer = pos_pointer->parent;
        return 1;
    }
}
// ��ֵ��� first={[a-z]}
int grammar_analysis::assignment_statement(){
    if(sym[countor].first==0){
        addNode("��ʶ��");
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
    addNode("���ʽ");
    int temp = expression();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// ������� first={begin}
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
    addNode("���");
    temp = statement();
    if(temp == -1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==27){
        addNode(";");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("���");
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
// ���� first={odd,+,-,[a-z],[0-9],(}
int grammar_analysis::condition(){
    int temp;
    if(sym[countor].first==6){
        addNode("odd");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("���");
        temp = expression();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else{
        addNode("���ʽ");
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        addNode("��ϵ�����");
        temp = relation_operator();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
        addNode("���ʽ");
        temp = expression();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
}
// ���ʽ first={+,-,[a-z],[0-9],(}
int grammar_analysis::expression(){
    if(sym[countor].first==14||sym[countor].first==15){
        addNode("[+|-]");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    int temp;
    addNode("��");
    temp = item();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==14||sym[countor].first==15){
        addNode("�Ӽ������");
        add_sub_operator();
        pos_pointer = pos_pointer->parent;
        addNode("��");
        temp = item();
        if(temp==-1){
        return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    return 1;
}
// �� first={[a-z],[0-9],(}
int grammar_analysis::item(){
    int temp;
    addNode("����");
    temp = factor();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==16||sym[countor].first==17){
        addNode("�˳������");
        mul_div_operator();
        pos_pointer = pos_pointer->parent;
        addNode("����");
        temp = factor();
        if(temp==-1){
        return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    return 1;
}
// ���� first={[a-z],[0-9],(}
int grammar_analysis::factor(){
    int temp;
    if(sym[countor].first==0){
        addNode("��ʶ��");
        temp = identifier();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else if(sym[countor].first==30){
        addNode("�޷�������");
        temp = unsigned_int();
        pos_pointer = pos_pointer->parent;
        return temp;
    }
    else if(sym[countor].first==25){
        addNode("(");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("���ʽ");
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
// �Ӽ������ first={+,-}
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
// �˳������ first={*,/}
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
// ��ϵ����� first={=,#,<,<=,>,>=}
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
// ������� first={if}
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
    addNode("����");
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
    addNode("���");
    temp = statement();
    return temp;
}
// ���̵������ first={call}
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
    addNode("��ʶ��");
    int temp = identifier();
    pos_pointer = pos_pointer->parent;
    return temp;
}
// ����ѭ����� first={while}
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
    addNode("����");
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
// ����� first={read}
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
    addNode("��ʶ��");
    temp = identifier();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("��ʶ��");
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
// д��� first={write}
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
    addNode("���ʽ");
    temp = expression();
    if(temp==-1){
        return -1;
    }
    pos_pointer = pos_pointer->parent;
    while(sym[countor].first==28){
        addNode(",");
        pos_pointer = pos_pointer->parent;
        countor++;
        addNode("���ʽ");
        temp = expression();
        if(temp==-1){
            return -1;
        }
        pos_pointer = pos_pointer->parent;
    }
    if(sym[countor].first==26){
        addNode("��");
        pos_pointer = pos_pointer->parent;
        countor++;
    }
    else{
        cout<<"Error in write statement of"<<sym[countor].first<<endl;
        return -1;
    }
    return 1;
}
// ��ĸ first={[a-z]}
//int grammar_analysis::letter(){}
// ���� first={[0-9]}
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

