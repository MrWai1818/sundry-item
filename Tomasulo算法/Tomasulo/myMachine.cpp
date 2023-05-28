#include "myMachine.h"
//初始化
myMachine::myMachine()
{
    Add.resize(3);  //设置3个Add
    Mult.resize(2);  //设置2个Mult
    Load.resize(3);  //设置3个Load
    Store.resize(3);  //设置3个Store
    Ins.resize(0);     //初始化指令集
    //初始化环境
    spare.push_back(3); 
    spare.push_back(3); 
    spare.push_back(3);
    spare.push_back(2);
    filname_output.resize(5);
    finish = 0;
    cycle = 0;
    Reg.resize(8);    //设置12个寄存器
    for (int i = 0; i < Reg.size(); i++)
    {   
        string temp0 = "R(F", temp1 = ")";   //初始化寄存器值
        int t = i * 2;  
        if (t > 9)  //标号大于9
        {
            Reg[i].value = temp0 + char((t / 8) + '0') + char((t % 8) + '0') +temp1; 
        }else{
            Reg[i].value = temp0 + char(t + '0') +temp1; 
        }
    }
    
}
//客制化部件数
myMachine::myMachine(int na, int nm, int nl, int ns, int nr)
{
    Add.resize(na);  //设置na个Add
    Mult.resize(nm);  //设置nm个Mult
    Load.resize(nl);  //设置nl个Load
    Store.resize(ns);  //设置ns个Store
    Ins.resize(0);     //初始化指令集
    //初始化环境
    spare.push_back(na); 
    spare.push_back(nm); 
    spare.push_back(nl);
    spare.push_back(ns);
    finish = 0;
    cycle = 0;
    Reg.resize(nr);    //设置12个寄存器
    for (int i = 0; i < Reg.size(); i++)
    {   
        string temp0 = "R(F", temp1 = ")";   //初始化寄存器值
        int t = i * 2;  
        if (t > 9)  //标号大于9
        {
            Reg[i].value = temp0 + char((t / nr) + '0') + char((t % nr) + '0') +temp1; 
        }else{
            Reg[i].value = temp0 + char(t + '0') +temp1; 
        }
    }
}
//从指令获取寄存器标号
vector<int> myMachine::get_index_of_reg_from_ins(int pc){
    string snum0 = Ins[pc].ins[2], snum1 = Ins[pc].ins[3];
    snum0.erase(0, 1), snum1.erase(0, 1);
    int first_opnum = 0, second_opnum = 0;
    vector<int> res(2);
    for (int i = 0; i < snum0.size(); i ++)
    {
        first_opnum += int(snum0[i] - '0') * pow(10, snum0.size() - 1 - i); 
    }
    for (int i = 0; i < snum1.size(); i ++)
    {
        second_opnum += int(snum1[i] - '0') * pow(10, snum1.size() - 1 - i); 
    }
    res[0] = first_opnum / 2, res[1] = second_opnum / 2;
    return res;
}
//从CBD取结果
string myMachine::get_from_CBD(char f, char b){
    for (int i = 0; i < CBD.size(); i++)//从CBD找数据
        if ((CBD[i].first[0] == f) && (CBD[i].first.back() == b))
            return CBD[i].second;
    return "";
}
//获取指令集
void myMachine::getInstuction(string put){
    put += ".txt";
    ifstream input(put);	//打开文件
	if(!input.is_open())
		std::cerr << "cannot open the file" <<endl;	//排错
	while (!input.eof())		//读完文件
	{
		Instuction temp;
        temp.ins.resize(4); //分段读取指令
		input >> temp.ins[0] >> temp.ins[1] >> temp.ins[2] >> temp.ins[3];
        if (temp.ins[0] != "")    //防止读空
        {
            Ins.push_back(temp);    //加入指令集
        }
	}
	input.close();		//读毕
}
//指令流出
void myMachine::Issue(int PC){  
    Instuction process_ins = Ins[PC];
    int no_reg = 0;   //store的来源寄存器标号，其余的目的寄存器标号
    string snum = Ins[PC].ins[1];
    snum.erase(0, 1);
    for (int i = 0; i < snum.size(); i ++)
    {
        no_reg += int(snum[i] - '0') * pow(10, snum.size() - 1 - i); 
    }
    no_reg /= 2;
    //指令判断  
    if ((Ins[PC].ins[0] == "LD") && (spare[0] != 0))    //load运算且缓冲器有位置
    {
        int i_for_L;    //缓冲器单元号
        //装入load缓冲器
        for (int i = 0; i < Load.size(); i++)
        {//更改缓冲器值
            if (Load[i].busy != 1)
            {
                Load[i].busy = 1;   //更改状态
                Load[i].no_of_ins = PC; //当前指令号
                Load[i].Fu = no_reg;    //目的寄存器    
                if (Ins[PC].ins[2] == "0")  
                {
                    Load[i].src = Ins[PC].ins[2] + "+" + Ins[PC].ins[3];  //数据来源
                }else{
                    Load[i].src = Ins[PC].ins[2] + Ins[PC].ins[3];  //数据来源
                }  
                Load[i].cycles_ins = cycle + LD;    //开始计算时延
                i_for_L = i;     //缓冲器单元号
                spare[0] --;       //占用一个单元
                break;
            }
        }
        //更新目的寄存器
        Reg[no_reg].wb_done = 0;    //寄存器未准备就绪
        Reg[no_reg].value = "";
        string temp = "Load";
        Reg[no_reg].source = temp + char((i_for_L + 1) + '0'); //寄存器数据来源
        for (int i = 0; i < CBD.size(); i++)    //删除总线已有的保留站的值
        {
            if (CBD[i].first == temp + char((i_for_L + 1) + '0')) //有新的数据进保留站就要替换旧值
            {
                CBD.erase(CBD.begin() + i);
                break;
            }
        }
        Ins[PC].issue = cycle;        //指令的IS周期
    }else if ((Ins[PC].ins[0] == "SD") && (spare[1] != 0))   //store运算且缓冲器有位置
    {
        int i_for_S;    //缓冲器单元号
        for (int i = 0; i < Store.size(); i++)
        {//更改缓冲器值
            if (Store[i].busy != 1)
            {
                Store[i].busy = 1;   //更改状态
                Store[i].no_of_ins = PC; //当前指令号
                Store[i].Qk = Ins[PC].ins[1];  //操作数来源
                if (Ins[PC].ins[2] == "0")  
                {
                    Store[i].addr = Ins[PC].ins[2] + "+" + Ins[PC].ins[3];  //数据来源
                }else{
                    Store[i].addr = Ins[PC].ins[2] + Ins[PC].ins[3];  //数据目的地
                }  
                i_for_S = i;     //缓冲器单元号
                spare[1] --;       //占用一个单元
                break;
            }
        }
        //判断是否获取操作数
        if (Reg[no_reg].wb_done)
        {   //准备完毕，VK填入值，Qk清空
            Store[i_for_S].Vk = Reg[no_reg].value;
            Store[i_for_S].Qk = "";
        }else{  //没准备好，替换操作数来源寄存器为对应缓冲器的标号
            Store[i_for_S].Qk = Reg[no_reg].source;
            Store[i_for_S].Vk = "";
        }  
        // 如果指令载入时，数据已经准备好，直接开始计算延时
        if ((Store[i_for_S].Vk != "") && (Store[i_for_S].issue_done == 0))  //准备就绪，更新时延
        {
           Store[i_for_S].cycles_ins = cycle+ SD;
           Store[i_for_S].issue_done = 1;
        }
        Ins[PC].issue = cycle;        //指令的IS周期 
    }else if (((Ins[PC].ins[0] == "ADDD") || (Ins[PC].ins[0] == "SUBD")) && (spare[2] != 0))     //+-运算
    {
        //第一和第二操作数的寄存器
        vector<int> tempi= get_index_of_reg_from_ins(PC);
        int first_opnum = tempi[0], second_opnum = tempi[1];
        int i_for_A;    //保留站标号
        for (int i = 0; i < Add.size(); i++)
        {//更改保留站值
            if (Add[i].busy != 1)
            {
                Add[i].busy = 1;   //更改状态
                Add[i].no_of_ins = PC; //当前指令号
                Add[i].Qj = Ins[PC].ins[2]; //第一个操作数来源
                Add[i].Qk = Ins[PC].ins[3]; //第二个操作数来源
                Add[i].op = Ins[PC].ins[0]; //操作
                Add[i].des = no_reg;    //目的地
                i_for_A = i;     //缓冲器单元号
                spare[2] --;       //占用一个单元
                break;
            }
        }
        //判断第一个操作数是否准备完毕
        if (Reg[first_opnum].wb_done)
        {   //准备完毕，V填入值，Q清空
            Add[i_for_A].Vj = Reg[first_opnum].value;
            Add[i_for_A].Qj = "";
        }else{  //没准备好，替换寄存器为对应保留站的标号
            Add[i_for_A].Qj = Reg[first_opnum].source;
        }
        //判断第二个操作数是否准备完毕
        if (Reg[second_opnum].wb_done)
        {   //准备完毕，V填入值，Q清空
            Add[i_for_A].Vk = Reg[second_opnum].value;
            Add[i_for_A].Qk = "";
        }else{  //没准备好，替换寄存器为对应保留站的标号
            Add[i_for_A].Qk = Reg[second_opnum].source;
        }
        //更新目的寄存器
        Reg[no_reg].wb_done = 0;    //寄存器未准备就绪
        Reg[no_reg].value = "";
        string temp ="Add";
        Reg[no_reg].source = temp + char((i_for_A + 1) + '0'); //寄存器数据来源
        for (int i = 0; i < CBD.size(); i++)    //删除总线已有的保留站的值
        {
            if (CBD[i].first == temp + char((i_for_A + 1) + '0')) //有新的数据进保留站就要替换旧值
            {
                CBD.erase(CBD.begin() + i);
                break;
            }
        }
        // 如果指令载入时，数据已经准备好，直接开始计算延时
        if ((Add[i_for_A].issue_done == 0) && (Add[i_for_A].Vj != "") && (Add[i_for_A].Vk != "")) //准备就绪，开始计算时延
        {
            if (Add[i_for_A].op == "ADDD")
            {
                Add[i_for_A].cycles_ins = cycle + fadd;    //开始计算时延
            }else{
                Add[i_for_A].cycles_ins = cycle + fsub;    //开始计算时延
            }
            Add[i_for_A].issue_done = 1;//代表准备完毕，可以进入执行阶段
        }
        Ins[PC].issue = cycle;        //指令的IS周期
    }else if (((Ins[PC].ins[0] == "MULTD") || (Ins[PC].ins[0] == "DIVD")) && (spare[3] != 0))    //*/运算
    {
        //第一和第二操作数的寄存器标号
        vector<int> tempi= get_index_of_reg_from_ins(PC);
        int first_opnum = tempi[0], second_opnum = tempi[1];
        int i_for_M;    //保留站标号
        for (int i = 0; i < Mult.size(); i++)
        {//更改保留站值
            if (Mult[i].busy != 1)
            {
                Mult[i].busy = 1;   //更改状态
                Mult[i].no_of_ins = PC; //当前指令号
                Mult[i].Qj = Ins[PC].ins[2]; //第一个操作数来源
                Mult[i].Qk = Ins[PC].ins[3]; //第二个操作数来源
                Mult[i].op = Ins[PC].ins[0]; //操作
                Mult[i].des = no_reg;    //目的地
                i_for_M = i;     //缓冲器单元号
                spare[3] --;       //占用一个单元
                break;
            }
        }
        //判断第一个操作数是否准备完毕
        if (Reg[first_opnum].wb_done)
        {   //准备完毕，V填入值，Q清空
            Mult[i_for_M].Vj = Reg[first_opnum].value;
            Mult[i_for_M].Qj = "";
        }else{  //没准备好，替换寄存器为对应保留站的标号
            Mult[i_for_M].Qj = Reg[first_opnum].source;
        }
        //判断第二个操作数是否准备完毕
        if (Reg[second_opnum].wb_done)
        {   //准备完毕，V填入值，Q清空
            Mult[i_for_M].Vk = Reg[second_opnum].value;
            Mult[i_for_M].Qk = "";
        }else{  //没准备好，替换寄存器为对应保留站的标号
            Mult[i_for_M].Qk = Reg[second_opnum].source;
        }
        //更新目的寄存器
        Reg[no_reg].wb_done = 0;    //寄存器未准备就绪
        Reg[no_reg].value = "";
        string temp = "Mult";
        Reg[no_reg].source = temp + char((i_for_M + 1) + '0'); //寄存器数据来源
        for (int i = 0; i < CBD.size(); i++)    //删除总线已有的保留站的值
        {
            if (CBD[i].first == temp + char((i_for_M + 1) + '0')) //有新的数据进保留站就要替换旧值
            {
                CBD.erase(CBD.begin() + i);
                break;
            }
        }
        // 如果指令载入时，数据已经准备好，直接开始计算延时
        if ((Mult[i_for_M].issue_done == 0) && (Mult[i_for_M].Vj != "") && (Mult[i_for_M].Vk != "")) //准备就绪，开始计算时延
        {
            if (Mult[i_for_M].op == "MULTD")
            {
                Mult[i_for_M].cycles_ins = cycle + fmult;    //开始计算时延
            }else{
                Mult[i_for_M].cycles_ins = cycle + fdiv;    //开始计算时延
            }
            Mult[i_for_M].issue_done = 1;
        }
        Ins[PC].issue = cycle;        //指令的IS周期
    }
}
//执行指令
void myMachine::Exec(int PC){
    //遍历所有保留站和缓冲器,执行对应指令
    //遍历load缓冲器
    for (int i = 0; i < Load.size(); i++)
    {
        if ((Load[i].busy == 1) && (Load[i].cycles_ins <= cycle))
        {
            Load[i].exe_done = 1;   //计算完毕可以写回
            Load[i].cycles_ins += WB;   //加上写回的周期
            Ins[Load[i].no_of_ins].exec = cycle;    //指令执行周期
            break;      //首个busy，且消耗完延迟的单元即为缓冲器首部
        }
    }
    //遍历store缓冲器
     for (int i = 0; i < Store.size(); i++)
    {   //store还需要来操作数准备完毕
        if ((Store[i].busy == 1) && (Store[i].cycles_ins == cycle) && (Store[i].Qk == ""))
        {
            Store[i].exe_done = 1;
            Store[i].cycles_ins += WB;   //加上写回的周期
            string temp0 = "M(", temp1 = ")";
            Store[i].addr = temp0 + Store[i].addr + temp1;//存数据到Mem
            Ins[Store[i].no_of_ins].exec = cycle;   //指令执行周期
            break;      //首个busy，且消耗完延迟的单元即为缓冲器首部
        }
    }
    //遍历ADD保留站
    for (int i = 0; i < Add.size(); i++)
    {   //延迟已过且两个操作数均已准备好才可执行
        if ((Add[i].busy == 1) && (Add[i].cycles_ins == cycle) && (Add[i].Qj == "") && (Add[i].Qk == ""))
        {
            //计算结果
            if (Add[i].op == "ADDD")
            {   
                string temp = "+";
                Add[i].res = Add[i].Vj + temp + Add[i].Vk;   //加法运算
            }else{
                string temp = "-";
                Add[i].res = Add[i].Vj + temp + Add[i].Vk;   //减法运算
            }
            Add[i].exe_done = 1;    //计算完毕，可以写回
            Add[i].cycles_ins += WB;   //加上写回的周期
            Ins[Add[i].no_of_ins].exec = cycle;   //指令执行周期
        }
    }
    //遍历Mult保留站
    for (int i = 0; i < Mult.size(); i++)
    {   //延迟已过且两个操作数均已准备好才可执行
        if ((Mult[i].busy == 1) && (Mult[i].cycles_ins == cycle) && (Mult[i].Qj == "") && (Mult[i].Qk == ""))
        {
            //计算结果
            string temp0 = Mult[i].Vj;
            string temp1 = Mult[i].Vk;
            if (temp0.size() > 8)    //加括号，防止乱序
                temp0 = "(" +temp0 + ")";
            if (temp1.size() > 8)    //加括号，防止乱序
                temp1 = "(" +temp1 + ")";
            if (Mult[i].op == "MULTD")
            {
                string temp = "*";
                Mult[i].res = temp0 + temp + temp1;   //加法运算
            }else{
                string temp = "/";
                Mult[i].res = temp0 + temp + temp1;   //减法运算
            }
            Mult[i].exe_done = 1;    //计算完毕，可以写回
            Mult[i].cycles_ins += WB;   //加上写回的周期
            Ins[Mult[i].no_of_ins].exec = cycle;   //指令执行周期
        }
    }
}
//写回
void myMachine::WriteBack(int PC){
    //保留站执行完毕，即CDB准备就绪，操作完成
    //将等待结果的寄存器填上结果
    //通过遍历所有缓冲器和保留站可写回单元实现
    //load缓冲器
    for (int i = 0; i < Load.size(); i++)
    {//代表数据准备写回
        if ((Load[i].exe_done == 1) && (Load[i].busy == 1) && (Load[i].cycles_ins == cycle))  
        {
            string temp0 = "M(", temp1 = ")";
            if (Reg[Load[i].Fu].wb_done == 0)
            {
                Reg[Load[i].Fu].value = temp0 + Load[i].src + temp1;    //值写回寄存器
                Reg[Load[i].Fu].source = "";
                Reg[Load[i].Fu].wb_done = 1;    //寄存器准备就绪
            }
            Ins[Load[i].no_of_ins].wb = cycle;  //指令的写回周期
            string temp = "Load";   
            CBD.push_back(make_pair(temp + char((i + 1) + '0'), temp0 + Load[i].src + temp1));  //结果存入CBD
            Load[i].ini();  //重置缓冲器 
            spare[0] ++;
            finish ++;  //写回后，代表指令完成
        }
    } 
    //store缓冲器
    for (int i = 0; i < Store.size(); i++)
    {//代表数据准备写回
        if ((Store[i].exe_done == 1) && (Store[i].busy == 1) && (Store[i].cycles_ins == cycle))  
        {
            //之后写回存储器了
            Ins[Store[i].no_of_ins].wb = cycle;  //指令的写回周期
            Store[i].ini();  //重置缓冲器 
            spare[1] ++;
            finish ++;  //写回后，代表指令完成
        }
        //数据不能写回，但是准备好操作要更新
        if ((Store[i].busy == 1) && (Store[i].exe_done == 0))
        {   
            if (Store[i].Vk == "")
            {
                char ri = Store[i].Qk.back();     //来源缓冲器或保留站编号
                Store[i].Vk = get_from_CBD(Store[i].Qk[0], ri); //填上需要数据,从CBD取
                if (Store[i].Vk != "")  Store[i].Qk = "";   //Qk置空
            }
        }
        if ((Store[i].Vk != "") && (Store[i].issue_done == 0))  //准备就绪，更新时延
        {
           Store[i].cycles_ins = cycle+ SD;
           Store[i].issue_done = 1;
        }
    }
    //Add保留站
    for (int i = 0; i < Add.size(); i++)
    {
        //代表数据准备写回
        if ((Add[i].exe_done == 1) && (Add[i].busy == 1) && (Add[i].cycles_ins == cycle))  
        {
            //若有更新的指令的值占用了该寄存器，则不更新寄存器，只写CBD
            if (Reg[Add[i].des].wb_done == 0)
            {
                Reg[Add[i].des].value = Add[i].res;//写结果到寄存器
                Reg[Add[i].des].source = "";
                Reg[Add[i].des].wb_done = 1;    //寄存器准备就绪
            }
            Ins[Add[i].no_of_ins].wb = cycle;  //指令的写回周期
            string temp = "Add";
            CBD.push_back(make_pair(temp + char((i + 1) + '0'), Add[i].res));  //结果存入CBD
            Add[i].ini();  //重置保留站
            spare[2] ++;
            finish ++;  //写回后，代表指令完成
        }
        //数据不能写回，但是准备好操作数操作要更新
        if ((Add[i].busy == 1) && (Add[i].exe_done == 0))
        {   
            //第一操作数更新
            if (Add[i].Vj == "")
            {
                char ri1 = Add[i].Qj.back();     //来源缓冲器或保留站编号
                Add[i].Vj = get_from_CBD(Add[i].Qj[0], ri1); //填上需要数据,从CBD取
                if (Add[i].Vj != "")  Add[i].Qj = "";   //Qk置空
            }
            //第二操作数更新
            if (Add[i].Vk == "")
            {
                char ri2 = Add[i].Qk.back();     //来源缓冲器或保留站编号
                Add[i].Vk = get_from_CBD(Add[i].Qk[0], ri2); //填上需要数据,从CBD取
                if (Add[i].Vk != "")  Add[i].Qk = "";   //Qk置空
            }
        }
        if ((Add[i].issue_done == 0) && (Add[i].Vj != "") && (Add[i].Vk != "")) //准备就绪，开始计算时延
        {
            if (Add[i].op == "ADDD")
            {
                Add[i].cycles_ins = cycle + fadd;    //开始计算时延
            }else{
                Add[i].cycles_ins = cycle + fsub;    //开始计算时延
            }
            Add[i].issue_done = 1;
        }
    }
    //Mult保留站
    for (int i = 0; i < Mult.size(); i++)
    {
        //代表数据准备写回
        if ((Mult[i].exe_done == 1) && (Mult[i].busy == 1) && (Mult[i].cycles_ins == cycle))  
        {
            //若有更新的指令的值占用了该寄存器，则不更新寄存器，只写CBD
            if (Reg[Mult[i].des].wb_done == 0)
            {
                Reg[Mult[i].des].value = Mult[i].res;//写结果到寄存器
                Reg[Mult[i].des].source = "";
                Reg[Mult[i].des].wb_done = 1;    //寄存器准备就绪
            }
            Ins[Mult[i].no_of_ins].wb = cycle;  //指令的写回周期
            string temp = "Mult";
            CBD.push_back(make_pair(temp + char((i + 1) + '0'), Mult[i].res));  //结果存入CBD
            Mult[i].ini();  //重置保留站
            spare[3] ++;
            finish ++;  //写回后，代表指令完成
        }
        //数据不能写回，但是准备好操作数操作要更新
        if ((Mult[i].busy == 1) && (Mult[i].exe_done == 0))
        {      
            //第一操作数更新
            if (Mult[i].Vj == "")
            {
                char ri1 = Mult[i].Qj.back();     //来源缓冲器或保留站编号
                Mult[i].Vj = get_from_CBD(Mult[i].Qj[0], ri1); //填上需要数据,从CBD取
                if (Mult[i].Vj != "")  Mult[i].Qj = "";   //Qk置空
            }
            //第二操作数更新
            if (Mult[i].Vk == "")
            {
                char ri2 = Mult[i].Qk.back();     //来源缓冲器或保留站编号
                Mult[i].Vk = get_from_CBD(Mult[i].Qk[0], ri2); //填上需要数据,从CBD取
                if (Mult[i].Vk != "")  Mult[i].Qk = "";   //Qk置空
            }
        }
        if ((Mult[i].issue_done == 0) && (Mult[i].Vj != "") && (Mult[i].Vk != "")) //准备就绪，开始计算时延
        {
            if (Mult[i].op == "MULTD")
            {
                Mult[i].cycles_ins = cycle + fmult;    //开始计算时延
            }else{
                Mult[i].cycles_ins = cycle + fdiv;    //开始计算时延
            }
            Mult[i].issue_done = 1;
        }
    }
}
//Tomasulo，将以上三个插件合并
void myMachine::Tomasulo(string put){
    char n = put.back();
    getInstuction(put);
    string ofin = "output-";
    ofin += n;
    filname_output[int(n - '0') - 1] = ofin;  //准备分文件输出
    ofin += ".txt";
    ofstream output(ofin, ios::out);  //新的指令集，新的结果
    output.close();
    int PC = 0; //当前运行指令
    //遍历指令集来执行所有指令
    while (finish != Ins.size())
    {
        cycle ++;   //下一个周期
        for (int i = 0; i < Ins.size(); i++)
        {   
            if (Ins[i].wb == 0)     //结果写回
            {
                WriteBack(i);	//写回
            }
            if ((i == 0) && (Ins[0].issue == 0))	//防止i-1数组越界
            {
                Issue(0);
                break;
            }else if ((Ins[i].issue == 0) && (Ins[i - 1].issue != 0))  //指令必须按顺序读取
            {
                Issue(i);   //指令流入
                break;      //一个周期只流入一条指令
            } 
            if (Ins[i].exec == 0)   //指令运行
            {
                Exec(i);	//指令执行
            }   
        }    
        exportResult_cycle(filname_output[int(n - '0') - 1]);
    }
    exportResult_final(filname_output[int(n - '0') - 1]);
}
//以文本形式输出每个周期状态
void myMachine::exportResult_cycle(string put){
    put += ".txt"; 
    ofstream output(put, ios::app);  //创建文件，每个周期新的内容写后面
	if(!output.is_open())
		std::cerr << "cannot open the file" << endl;
    output << "Cycle_" << cycle << ";" <<endl;
    //输出Load缓冲器的状态
	for (int i = 0; i < Load.size(); i++)
	{
        output << " Load" << i + 1 << ":";
        if (Load[i].busy == 0)  //为缓冲器写状态
        {
            output << "No" << "," << Load[i].src << ";";
        }else{
            output << "Yes" << "," << Load[i].src << ";";
        }
		output << endl; //换行
	}
    //输出Store缓冲器状态
    for (int i = 0; i < Store.size(); i++)
    {
        output << " Store" << i + 1 << ":";
        if (Store[i].busy == 0)  //为缓冲器写状态
        {
            output << "No" << "," << Store[i].addr << ",";
        }else{
            output << "Yes" << "," << Store[i].addr << ",";
        }
        if (Store[i].Qk != "")   //防止写空
        {
            output << Store[i].Qk << ";";
        }else{
            output << Store[i].Vk << ";";
        }
		output << endl; //换行
    }
    //输出Add保留站状态
    for (int i = 0; i < Add.size(); i++)
    {
        output << " Add" << i + 1 << ":";
        if (Add[i].busy == 0)  //为缓冲器写状态
        {
            output << "No" << "," << Add[i].op << ",";      //写入操作和状态
        }else{
            output << "Yes" << "," << Add[i].op << ",";
        }
        output << Add[i].Vj << "," << Add[i].Vk << ","; //写入Vj和Vk
        output << Add[i].Qj << "," << Add[i].Qk << ";"; //写入Qj和Qk
		output << endl; //换行
    }
    //输出Mult保留站状态
    for (int i = 0; i < Mult.size(); i++)
    {
        output << " Mult" << i + 1 << ":";
        if (Mult[i].busy == 0)  //为缓冲器写状态
        {
            output << "No" << "," << Mult[i].op << ",";      //写入操作和状态
        }else{
            output << "Yes" << "," << Mult[i].op << ",";
        }
        output << Mult[i].Vj << "," << Mult[i].Vk << ","; //写入Vj和Vk
        output << Mult[i].Qj << "," << Mult[i].Qk << ";"; //写入Qj和Qk
		output << endl; //换行
    }
    //最后写寄存器状态
    for (int i = 0; i < Reg.size(); i++)
    {
        output << "F" << 2 * i << ":";  //标号
        if (Reg[i].value != "")   //有值写值，没值写来源
        {
            if (Reg[i].value[0] == 'R')
            {
                output << ";";
            }else{
               output << Reg[i].value << ";"; 
            } 
        }else{
            output << Reg[i].source << ";";
        }
    }
    
    output << endl << "------------------------------------" << endl; //分割线
	output.close();		//结束
	return;
}
//以文本形式输出最终结果
void myMachine::exportResult_final(string put){
    put += ".txt";
	ofstream output(put, ios::app);  //创建文件，写最终结果
	if(!output.is_open())
		std::cerr << "cannot open the file" << endl;
    output << endl;
	for (int i = 0; i < Ins.size(); i++)
	{
		for (int j = 0; j < Ins[i].ins.size(); j++)
		{
			output << Ins[i].ins[j]<<" ";       //写入指令
		}
        output << " : " << Ins[i].issue << ","; //写入issue周期
        output << Ins[i].exec << ",";//写入exec周期
        output << Ins[i].wb << ";";//写入wb周期
		output << endl; //换行
	}
	output.close();		//结束
	return;
}