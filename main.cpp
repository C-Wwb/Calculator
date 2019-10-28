#include <iostream>
#include<stack>
#include <sstream>
#include<stdlib.h>
using namespace std;

int Priority(char operate)//定义栈中优先级
{
    switch(operate)
    {
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 3;
        case '(':
        case ')':
            return 1;
        default:
            return 0;
    }
}
string Infix_postfix(string infix)//中缀转为后缀（逆波兰算法）
{
    stack<char> operate;//定义一个操作数组
    string Suffix = "";//初始化后缀表达式
    char currentOp;//当前栈顶
    int negative;
    int has_left=0;//判断是否有左括号
    int i = 0,j = 0;//i为中缀当前指向 j为后缀当前指向
    while(infix[i]!='=')
    {
        if(i+1!='\0')
            negative = 0;
        if(infix[i]>='0' && infix[i]<='9') //判断数字
        {
            Suffix[j++] = '|';//j是后缀表达索引
            Suffix[j++] =infix[i];//存储当前数字并指向下一个
            while(infix[++i]>='0' && infix[i]<='9') //整数部分，后缀直接取中缀
            {
                Suffix[j++] =infix[i];
            }
            if(infix[i]=='.') //小数部分
            {
                Suffix[j++]='.';
                i+=1;//中缀索引往后移一位，后缀再取
                while(infix[i]>='0' && infix[i]<='9')
                {
                    Suffix[j++] =infix[i];
                    i+=1;
                }
            }
        }
        else if(infix[i]=='(')//如果读入(，因为左括号优先级最高，因此放入栈中，但是注意，当左括号放入栈中后，则优先级最低。
        {
            operate.push(infix[i++]);
            has_left++;
        }
        else if(infix[i]==')')//如果读入），则将栈中运算符取出放入输出字符串，直到取出（为止，注意：（）不输出到输出字符串。
        {

            if(has_left==0 || operate.empty()){//没有左括号
                cout<<"error.."<<endl;
                exit(1);//跳出循环
            }else
            {
                currentOp = operate.top();//取出栈顶
                while(currentOp!='(')
                {
                    Suffix[j++]='|';
                    Suffix[j++]=currentOp;
                    operate.pop();//赋值之后扔出来
                    if(operate.empty())
                    {
                        cout<<"error..."<<endl;
                        break;
                    }
                    currentOp = operate.top();
                }
                operate.pop();//删除栈中(
                has_left--;
                i++;
            }
        }

        else if(infix[i]=='+'||infix[i]=='-'||infix[i]=='/'||infix[i]=='*')
        {
            //判断负数
            if(infix[i]=='-')
            {

                if(i==0)//第一个为‘-’时为负号
                    negative = 1;
                else if(infix[i-1]=='+'||infix[i-1]=='-'||infix[i-1]=='/'||infix[i-1]=='*')//如果前面有操作符则为负号
                    negative = 1;
                if(negative==1)
                {
                    Suffix[j++] = '|';//负号
                    Suffix[j++] = '-';
                    i+=1;
                    if(infix[i]>=48 && infix[i]<=57) //判断数字
                    {
                        Suffix[j++] =infix[i];
                        while(infix[++i]>='0' && infix[i]<='9') //整数部分
                        {
                            Suffix[j++] =infix[i];
                        }
                        if(infix[i]=='.') //是小数
                        {
                            Suffix[j++]='.';
                            i+=1;
                            while(infix[i]>='0' && infix[i]<='9') //小数部分
                            {
                                Suffix[j++] =infix[i];
                                i+=1;
                            }
                        }
                    }
                    continue;
                }
            }

            //如果读入一般运算符如+-*/，则放入堆栈，但是放入堆栈之前必须要检查栈顶
            if(operate.empty())
            {
                operate.push(infix[i++]);
            }
            else
            {
                char top = operate.top();//栈顶
                if(Priority(top)<Priority(infix[i])) //放入的符号优先级低于栈顶
                {
                    operate.push(infix[i++]);//放入栈顶并指向下一个
                }
                else//如果放入的优先级较低，则需要将栈顶的运算符放入输出字符串。
                {
                    while(Priority(top)>=Priority(infix[i]))//如果栈顶的优先级大于放入的优先级
                    {
                        Suffix[j++]='|';//依旧是后缀索引
                        Suffix[j++]=top;
                        operate.pop();//让索引等于后从数组中移除
                        if(!operate.empty())
                        {
                            top = operate.top();//数组栈顶等于栈顶
                        }
                        else
                            break;
                    }
                    operate.push(infix[i++]);//放入栈顶并指向下一个
                }
            }

        }
        else
        {
            i+=1;
        }
    }

    //顺序读完表达式，如果栈中还有操作符，则弹出，并放入输出字符串。
    while(!operate.empty())
    {
        char to = operate.top();
        Suffix[j++]='|';
        Suffix[j++]=to;
        operate.pop();
    }
    Suffix[j] = '#';//结束符
    return Suffix;
}

double CalSuffix(string suffix) {

    double result;
    stack<double> number;//操作数栈
    int i = 0,j;
    int n=0;//判断是否为数字
    string current;//当前
    while(suffix[i]!='#')
    {
        int isNum=0;
        if(suffix[i]=='|')
        {
            for(j=i+1;; j++)
            {
                if(suffix[j]=='|')
                    break;
                if(suffix[j]=='#')
                    break;
            }

            //获取后缀表达式之间元素
            for(int k=i+1; k<j; k++)
            {
                current+=suffix[k];
            }

            //判断是否为数值
            for(int k=0; k<current.size(); k++)
            {
                if(current[k]>=48 && current[k]<=57)//数字
                {
                    //数字转double
                    istringstream iss(current);
                    double num;
                    iss >> num;
                    number.push(num);
                    isNum = 1;
                    break;
                }
            }
            if(isNum!=1){//如果不是数字，而是符号的话，定义计算法则
                if(number.size()<2){//避免类似6*的形式，需要取两个数来进行操作，如果栈内为偶数项，取为null就会报错
                    cout<<"error...."<<endl;
                    exit(-1);
                }
                double n2 = number.top();
                number.pop();
                double n1 = number.top();
                number.pop();
                if(current=="+"){
                    number.push(n1+n2);
                }
                else if(current=="-"){
                    number.push(n1-n2);
                }
                else if(current=="*"){
                    number.push(n1*n2);
                }
                else if(current=="/"){
                    number.push(n1/n2);
                }
            }
            current="";//清空当前字符串;
        }

        i++;
    }
    if(number.size()!=1)//判断number的长短
        cout<<"error."<<endl,exit(-1);
    return number.top();
}
int main()
{
    string a;
    cout<<"Please input formula:\n";
    cin>>a;
    istringstream iss(a);
    double num;
    iss >> num;
    cout<<CalSuffix(Infix_postfix(a));//中缀转后缀之后计算
    return 0;
}
/*
一、主体思想（eg：0.3/(5*2+1)#——>0.3 5 2 * 1 + /#）
1.遇到操作数：直接输出（添加到后缀表达式中）
2.栈为空时，遇到运算符，直接入栈
3.遇到左括号：将其入栈
4.遇到右括号：执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出。
5.遇到其他运算符：加减乘除：弹出所有优先级大于或者等于该运算符的栈顶元素，然后将该运算符入栈
6.最终将栈中的元素依次出栈，输出。
 */