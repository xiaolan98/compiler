#include"interpreter.h"
#include<iostream>
using namespace std;
interpreter::interpreter(vector<code> codeRepo){
    execute(codeRepo);
}
void interpreter::execute(vector<code> codeRepo){
    int I=0;//指令寄存器
    int P;//程序地址寄存器
    int T=-1;//栈顶寄存器
    int B=0;//基地址寄存器
    int S[1024];//数据栈
    for(int i=0;i<1024;i++){S[i]=0;}
    while(I<codeRepo.size()){
        //cout<<I<<"  "<<codeRepo[I].f<<"  "<<codeRepo[I].l<<"  "<<codeRepo[I].a<<"  "<<endl;
        //cout<<S[0]<<" "<<S[1]<<" "<<S[2]<<" "<<S[3]<<" "<<S[4]<<endl;
        if(codeRepo[I].f=="LIT"){
            T++;
            S[T] = codeRepo[I].a;
            I++;
        }
        else if(codeRepo[I].f=="LOD"){
            T++;
            //getBaseAddr() is aimed to get the address where the variable states.
            if(codeRepo[I].l==0){
                S[T] = S[B+codeRepo[I].a];
            }
            else{
                int index = S[B];
               // cout<<B<<"  ";
                for(int j=0;j<codeRepo[I].l-1;j++){
                    index = S[index];
                }
                S[T] = S[index+codeRepo[I].a];
                //cout<<index+codeRepo[I].a;
            }
            //cout<<"  "<<S[T]<<endl;
            I++;
        }
        else if(codeRepo[I].f=="STO"){
            if(codeRepo[I].l==0){
                S[B+codeRepo[I].a] = S[T];
            }
            else{
                int index = S[B];
                for(int j=0;j<codeRepo[I].l-1;j++){
                    index = S[index];
                }
                S[index+codeRepo[I].a] = S[T];
            }
            I++;
            T--;
        }
        else if(codeRepo[I].f=="CAL"){
            if(codeRepo[I].l==1){
                S[B] = S[B+1];
            }
            S[B+2] = I;//注意因为opr 0 0 之后还要I++
            //cout<<"RETURN ADDRESS  "<<B<<"  "<<S[B+2]<<"  "<<T<<endl;
            I = codeRepo[I].a;
        }
        else if(codeRepo[I].f=="INT"){
            S[T+2] = B;//DL Dynamic link
            S[T+1] = S[B];
            B = T+1;
            T += codeRepo[I].a + 3;
            //cout<<"B "<<B<<" T "<<T<<endl;
            I++;
        }
        else if(codeRepo[I].f=="JMP"){
            I = codeRepo[I].a;
        }
        else if(codeRepo[I].f=="JPC"){
            if(S[T]==0){
                I = codeRepo[I].a;
            }
            else{
                I++;
            }
        }
        else if(codeRepo[I].f=="OPR"){
            switch(codeRepo[I].a)
            {
                case 0:{
                    //cout<<B<<" "<<S[B]<<" "<<S[B+1]<<" "<<S[B+2]<<" "<<endl;
                    I = S[B+2];//要cout一下 可能有问题
                    //cout<<"I:  "<<I<<endl;
                    int tempB = S[B+1];
                    T = B-1;
                    B = tempB;
                    if(I==0){
                        return;
                    }
                    break;
                }
                case 1:{
                    S[T-1]=(S[T-1]+S[T]);
                    T--;
                    break;
                }
                case 2:{
                    S[T-1]=(S[T-1]-S[T]);
                    T--;
                    break;
                }
                case 3:{
                    S[T-1]=(S[T-1]*S[T]);
                    T--;
                    break;
                }
                case 4:{
                    S[T-1]=(S[T-1]/S[T]);
                    T--;
                    break;
                }
                case 5:{
                    S[T-1] = S[T-1]==S[T]?1:0;
                    T--;
                    break;
                }
                case 6:{
                    S[T-1] = S[T-1]!=S[T]?1:0;
                    T--;
                    break;
                }
                case 7:{
                    S[T-1] = S[T-1]<S[T]?1:0;
                    T--;
                    break;
                }
                case 8:{
                    S[T-1] = S[T-1]>S[T]?1:0;
                    T--;
                    break;
                }
                case 9:{
                    S[T-1] = S[T-1]<=S[T]?1:0;
                    T--;
                    break;
                }
                case 10:{
                    S[T-1] = S[T-1]>=S[T]?1:0;
                    T--;
                    break;
                }
                case 11:{
                    S[T] = S[T]%2==1?1:0;
                    break;
                }
                case 12:{
                    T++;
                    cin>>S[T];
                    break;
                }
                case 13:{
                    cout<<S[T]<<endl;
                    T--;
                    break;
                }
            }
            I++;
        }
    }
}
