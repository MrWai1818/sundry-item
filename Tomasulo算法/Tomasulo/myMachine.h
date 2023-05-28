#ifndef MYMACHINE_H
#define MYMACHINE_H
#include <string.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

//寄存器
struct Register
{
    string value = "";  //寄存器值
    string source = "";     //值的来源
    int wb_done = 1;    //1代表已被写回到CBD，即数据准备完毕
    void ini(){     //初始化寄存器
        string value = "";  
        string source = "";    
        int wb_done = 1;   
    }
};

//Load组件
struct Load
{
    int busy = 0;   //是否忙
    int Fu = -1;    //load目的寄存器
    string  src = "";   //load数据来源，如34+R3
    int no_of_ins = 0;  //当前执行指令编号
    int cycles_ins = 0;  //当前指令需要执行周期数
    int exe_done = 0;    //指令执行完毕，可以写回到
    void ini(){ //初始化缓冲器
        busy = 0;  
        Fu = -1;    
        src = "";   
        no_of_ins = 0;  
        cycles_ins = 0;  
        exe_done = 0;    
    }
};

//Store组件
struct Store
{
    int busy = 0;   //是否忙
    string addr = "";    //store目的地,如0+R1
    string Vk = "";     //store操作数值
    string Qk = "";    //store操作数来源
    int no_of_ins = 0;  //当前执行指令编号
    int cycles_ins = 0;  //当前指令需要执行周期数
    int issue_done = 0 ; //数据准备完毕，可以执行，开始计算时延
    int exe_done = 0;    //执行结束，可以写回
    void ini(){     //初始化缓冲器
        busy = 0;   //是否忙
        addr = "";    
        Vk = "";     
        Qk = "";    
        issue_done = 0;
        no_of_ins = 0; 
        cycles_ins = 0; 
        exe_done = 0;   
    }
};

//Add和Mult组件
struct AoM
{
    int busy = 0;   //是否忙
    string op = "";     //操作
    string Vj = "";     //第一个操作数值
    string Vk = "";     //第二个操作数值
    string Qj = "";     //第一个操作数值来源
    string Qk = "";     //第二个操作数值来源
    string res = "";     //运算结果
    int des = -1;   //结果写入寄存器号
    int no_of_ins = 0;  //当前执行指令编号
    int cycles_ins = 0;  //当前指令需要执行周期数
    int issue_done = 0 ; //数据准备完毕，可以执行，开始计算时延
    int exe_done = 0;    //执行结束，可以写回
    void ini(){     //初始化保留站
        busy = 0;  
        op = "";    
        Vj = "";    
        Vk = "";    
        Qj = "";    
        Qk = "";   
        res = "";
        des = -1;  
        no_of_ins = 0;  
        issue_done = 0;
        cycles_ins = 0;  
        exe_done = 0;   
    }
};

//指令
struct Instuction
{
    vector<string> ins;  //指令内容
    int issue = 0;  //指令发射周期
    int exec = 0;   //指令执行周期
    int wb = 0;     //指令写回周期
};

//机器类
class myMachine
{
private:
    vector<Load> Load;   //Load缓冲器
    vector<Store> Store;  //Store缓冲器
    vector<AoM> Add;    //Add保留站
    vector<AoM> Mult;   //Mult保留站 
    vector<Instuction> Ins; //指令集
    vector<Register> Reg;   //寄存器组
    vector<pair<string, string> > CBD;   //公共数据总线，第一个string为数据来源，第二个为数据
    int cycle;  //时钟周期
    int finish; //完成指令数
    //各个组件空闲状态,0:Load 1:Store 2:Add 3:Mult 
    vector<int> spare;   
    vector<string> filname_output;  //输出文件名,用于分文件输出
    enum cycles{        //各项操作耗费周期数
        IS = 1, //issue
        WB = 1, //write back
        LD = 2, //load 
        SD = 2, //store
        fadd = 2,   //float add
        fsub = 2,   //float sub
        fmult = 10, //float mult
        fdiv = 20   //float div
    };   
public:
    myMachine(); 
    //机器客制化，na:Add数量，nm:Mult数量，nl:Load数量，ns:Store数量，nr:Reg数量
    myMachine(int na, int nm, int nl, int ns, int nr);
    void getInstuction(string put);   //获取指令集
    void exportResult_cycle(string put);      //文本输出每个周期状态
    void exportResult_final(string put);      //文本输出最终结果
    vector<int> get_index_of_reg_from_ins(int pc);   //从指令获取寄存器标号
    string get_from_CBD(char f, char b);  //从CBD取结果
    //Tomasulo算法
    void Issue(int PC);   //指令流出
    void Exec(int PC);    //指令执行
    void WriteBack(int PC);      //结果写回
    void Tomasulo(string put);  //算法执行,put输入指令集文件 
};
#endif