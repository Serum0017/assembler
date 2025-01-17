#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>
using namespace std;

vector<string> split(std::string s, string delim/*must have length == 1*/){
    vector<string> split = {""};
    size_t pos;
    std::string token;
    
    while((pos = s.find(delim)) != std::string::npos){
        token = s.substr(pos, 1);
        split.push_back(token);
        s.erase(0,pos+delim.length());
        split.push_back(token);
    }
    
    return split;
}

bool completed = false;
unordered_map<string, byte> registerMap = {};
byte registerToByte(string reg) {
    if(completed == false){
        completed = true;
        // declare map
        registerMap["%rax"] = (byte) 0;
        registerMap["%rcx"] = (byte) 1;
        registerMap["%rdx"] = (byte) 2; 
        registerMap["%rbx"] = (byte) 3;

        registerMap["%rsp"] = (byte) 4;
        registerMap["%rbp"] = (byte) 5;
        registerMap["%rsi"] = (byte) 6;
        registerMap["%rdi"] = (byte) 7;
        
        registerMap["%r8"] = (byte) 8;
        registerMap["%r9"] = (byte) 9;
        registerMap["%r10"] = (byte) 10;
        registerMap["%r11"] = (byte) 11;


        registerMap["%r12"] = (byte) 12;
        registerMap["%r13"] = (byte) 13;
        registerMap["%r14"] = (byte) 14;
    }
    // lookup map
    return registerMap[reg];
}

bool completedFn2B = false;
unordered_map<string, byte> suffixMap = {};
byte movAndJmpFn(string suffix) {
    if(completedFn2B == false){
        completedFn2B = true;
        // declare map
        suffixMap["mp"] = (byte) 0;
        suffixMap["q"] = (byte) 0;
        suffixMap["le"] = (byte) 1; 
        suffixMap["l"] = (byte) 2;
        suffixMap["e"] = (byte) 3;
        suffixMap["ne"] = (byte) 4;
        suffixMap["ge"] = (byte) 5;
        suffixMap["g"] = (byte) 6;
    }
    // lookup map
    return suffixMap[suffix];
}

byte writeRightHalfByte(byte b, int i){
    b = b & ((byte) (i & 0xF));
    return b;
}

byte joinByte(byte b, byte i){
    b = (b<<4) & ((i & ((byte) 0xF)));
    return b;// 0x0[d], 0x0[d2] => 0x[d][d2] 
}

byte writeLeftHalfByte(byte b, int i){
    b = b & ((byte) ((i & 0xF) << 4));
    return b;
}

typedef void(*FunctionPointer)();

// void pushArray(int curIndex, std::vector<byte> bigArray){
//     // bigArray[curIndex++] = val;
// }

// half, nop, cmovXX, irmovq, rmmovq, mrmovq, OPq, jXX, call, ret, pushq, popq
void halt(string line, std::vector<byte> bigArray) {
    bigArray.push_back((byte) 0);}

void nop(string line, std::vector<byte> bigArray) {
    bigArray.push_back((byte) 16);
}

// cmovX rA, rB
// encoding: 2 | fn | rA | rB
// rrmovq
void cmovXX(string line, std::vector<byte> bigArray){
    cout << "calling!";
    std::vector<string> splitLine = split(line, " ");
    std::byte fnByte = movAndJmpFn(
        split(splitLine.at(0), "mov").at(1)
    );
    bigArray.push_back(joinByte((byte) 2, fnByte));
    //determine fn bit to add
    bigArray.push_back(
        joinByte(registerToByte(splitLine.at(1)),
        registerToByte(splitLine.at(2)
    )));
}


/** 
void call(string line, std::vector<byte> bigArray, dynamicHashMap replaceMap) {
    bigArray.push_back((byte) 8);
    bigArray.push_back((byte) 0);
    // int -> string
    // location -> name

    // myFunction -> [0x1234AB]
    // supports [name] -> [vectorLocation] 
    string[] splitLine = line.
    replaceMap[line.]
}

void ret(string line, std::vector<byte> bigArray) {
    bigArray.push_back((byte) 9);
    bigArray.push_back((byte) 0);
}

void pushq(string line, std::vector<byte> bigArray) {
    bigArray.push_back((byte) 0xA);
    bigArray.push_back((byte) 0);
    // raA, F
}

//https://spcs.instructure.com/courses/8654/files/1593217?module_item_id=212606

void movrm(string line, std::vector<byte> bigArray){

}

void call(string line){
    byte* arr = new byte[2];
    arr[0] = (byte) 0;
    arr[1] = (byte) 0;
    return arr;
}
*/


int main() {
    // std::string myString = "Hello, World!";
    // std::cout << myString << std::endl;
    std::vector<byte> bigArray;
    cmovXX("cmovl %rax %r14", bigArray);
    // std::cout << (int) bigArray.at(0);

    // vector<int> replaceIndexes 
    // unordered_map<std::string, std::function<byte[](string)>> funcMap;
    // //ccmovq
    // //rrmovq
    // //cmovle 

    // funcMap["halt"] = halt;
    // funcMap["nop"] = nop;
    // funcMap[""]

    // std::ifstream assembly_code;
    // assemblyCode.open("file.s");
    // string line;
    // if(assemblyCode.is_open()){
    //     while(std::getline(assemblyCode, line)){
    //         cout << line << '\n';
    //     }
    //     assemblyCode.close();
    // }



    // FunctionPointer fps[3] = {};

    // movrm rA rB whatever
    // encoding: [typeByte] [either fn | 0] [rA] [rB] 

    // read the instruction
    // convert it to an array index
    // fps[]

    // const file = fs.readdrSync('./file.s');

    // for(let i = 0; i < file.readLine(); i++){
    //     const split = file.split(' ');

    //     // O(n)
    //     // if(type == 0){
    //     //     // do something
    //     // } else if(type === 1){
    //     // }

    //     // movq rax
    //     const type = split[0];// movq
    //     map[type](...split.slice(1));
    // }

    // const map = [
    //     () => {
    //         // do one thing
    //     },
    //     () => {
    //         // do another thing
    //         subMap[]();
    //     }
    // ]
    
    return 0;
}
