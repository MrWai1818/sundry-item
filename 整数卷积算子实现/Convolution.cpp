#include <iostream>
#include <cstring>
#include <cmath>
#include<string>
#include<vector>
using namespace std;
int main(){
    int const map = 7;//定义被卷积的矩阵
    float  A[map*map];
    int const coremap = 3;//定义卷积核
    float B[coremap*coremap];
    int const outmap = map - coremap + 1;  //计算卷积结果维数
    int const convAw = coremap*coremap; //计算卷积过程中的被卷积矩阵的宽和高
    int const convAh = map*map;
    cout<<"请输入待卷积的图"<<endl;
    int put;
    for(int i=0;i<convAh;i++){
        cin>>put;
        A[i]=put;
    }
    cout<<"请输入卷积核"<<endl;
    int cput;
    for(int i=0;i<convAw;i++){
        cin>>cput;
        B[i]=cput;
    }
    float A_convert[convAh*convAw] = { 0 };//定义一个卷积过程中的矩阵
    for (int i = 0; i < outmap; i++){
        for (int j = 0; j < outmap; j++){
            int wh = i * outmap * convAw + j * convAw;
            int col1 = i * map + j;         //进行卷积核进行的第一行运算
            A_convert[wh] = A[col1];
            A_convert[wh + 1] = A[col1 + 1];
            A_convert[wh + 2] = A[col1 + 2];
            int col2 = (i + 1) * map + j;       //进行卷积核进行的第二行运算
            A_convert[wh + 3] = A[col2];
            A_convert[wh + 4] = A[col2 + 1];
            A_convert[wh + 5] = A[col2 + 2];
            int col3 = (i + 2) * map + j;   //进行卷积核进行的第三行运算
            A_convert[wh + 6] = A[col3];
            A_convert[wh + 7] = A[col3 + 1];
            A_convert[wh + 8] = A[col3 + 2];
        }
    }
    vector<int> RES;
    for (int i = 0; i < outmap; i++){
        for (int j = 0; j < outmap; j++){
            int temp = 0;
            int wh = i * outmap * convAw + j * convAw;
            for (int m =0; m < convAw; m++){
                temp += A_convert[wh + m] * B[m] ;
            }
            RES.push_back(temp); //在RES中添加数据a
        }
    }
    cout<<"结果为"<<endl;
    for (int i = 0; i < outmap; i++){
        for (int j = 0; j < outmap; j++){
            cout << RES[i*outmap + j] << " ";
        }
        cout << endl;
    }
    //max pooling
    int stride=outmap-coremap-1;
    int max=-100000;
    for(auto temp:RES){
        if(temp>max){
            max=temp;
        }
    }
    cout<<max;
    system("pause");
    return 0;
}
