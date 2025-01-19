#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>
using namespace std;

std::vector<std::string> split(std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
}

bool completed = false;
unordered_map<string, byte> registerMap = {};
byte registerToVal(string reg) {
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
byte getFnFromSuffix(string suffix) {
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

bool inittedHexMap = false;
unordered_map<char, int> hexMap = {};
long long parseHex(string s){
    if(inittedHexMap == false){
        inittedHexMap = true;
        hexMap['0'] = 0;
        hexMap['1'] = 1;
        hexMap['2'] = 2;
        hexMap['3'] = 3;
        hexMap['4'] = 4;
        hexMap['5'] = 5;
        hexMap['6'] = 6;
        hexMap['7'] = 7;
        hexMap['8'] = 8;
        hexMap['9'] = 9;
        hexMap['a'] = 10;
        hexMap['A'] = 10;
        hexMap['b'] = 11;
        hexMap['B'] = 11;
        hexMap['c'] = 12;
        hexMap['C'] = 12;
        hexMap['d'] = 13;
        hexMap['D'] = 13;
        hexMap['e'] = 14;
        hexMap['E'] = 14;
        hexMap['f'] = 15;
        hexMap['F'] = 15;
    }

    long d = 0;
    int exp = 0;
    for(int i = s.size()-1; i >= 0; i--){
        d += ((long long) hexMap[s.at(i)]) << exp;// raising to 2^exp = 2^(4*hex) which is right
        exp += 4;
    }
    return d;
}

byte getNthByte(long long d, int n){
    return (byte) ( ( d >> (8*n) ) & 0xFF );
    // return d >> ( (double)8 * n);
}

// untested
byte writeRightHalfByte(byte b, int i){
    b = b | ((byte) (i & 0xF));
    return b;
}

byte joinByte(byte b, byte i){
    // byte b
    // byte i
    // ->
    // b | i in a single byte (last 4 positions of each should be 0)
    return (b<<4) | i;
}

// untested
byte writeLeftHalfByte(byte b, int i){
    b = b | ((byte) ((i & 0xF) << 4));
    return b;
}

typedef void(*FunctionPointer)();

// void pushArray(int curIndex, std::vector<byte> bigArray){
//     // bigArray[curIndex++] = val;
// }

// half, nop, cmovXX, irmovq, rmmovq, mrmovq, OPq, jXX, call, ret, pushq, popq
void halt(string& line, std::vector<byte>& bigArray) {
    bigArray.push_back((byte) 0);}

void nop(string& line, std::vector<byte>& bigArray) {
    bigArray.push_back((byte) 16);
}

// cmovX rA, rB
// encoding: 2 | fn | rA | rB
// rrmovq
void cmovXX(string line, std::vector<byte>& bigArray){
    // example: cmovle %rax, %rbx 
    // 2 | fn
    std::vector<string> splitLine = split(line, " ");// [cmovle, rax, rbx]
    std::byte fn = getFnFromSuffix(
        split(splitLine.at(0), "mov").at(1)// le
    );// -> 1 because le corresponds to 1

    // std::cout << (int) joinByte((byte) 2, (byte) 6);

    byte firstByte = joinByte( (byte) 2, fn );// 2 | 1

    bigArray.push_back(firstByte);

    byte rAind = registerToVal(splitLine.at(1));
    byte rBind = registerToVal(splitLine.at(2));
    
    bigArray.push_back(joinByte(rAind, rBind));
}

void irmovq(string line, std::vector<byte>& bigArray){
    // 3 | 0, F | rB, | [long] V (in little endian order)

    // irmovq $0xabcd, %rdx

    // 3 | 0
    bigArray.push_back(joinByte((byte) 3, (byte) 0));

    // F | rB
    std::vector<string> splitLine = split(line, " ");// [cmovle, rax, rbx]
    byte rBind = registerToVal(splitLine.at(2));

    // cout << splitLine.at(2);
    // std::cout << (int) rBind;

    bigArray.push_back(joinByte((byte) 0xF, (byte) rBind));

    // [long] V
    string immediateVal = splitLine.at(1);
    long long d = parseHex(immediateVal.substr(3, immediateVal.length() - 4));// remove $0x from the start and , from the end

    // cout << std::hex << d << "\n";

    // this is already in little endian because the least significant byte is stored first.
    for(int i = 0; i < 8; i++){
        bigArray.push_back(
            getNthByte(d, i)
        );
    }
}

void rmmovq(string line, std::vector<byte>& bigArray){
    // rmmovq %rsp, 0x41c(%rbx)
    // 4 | 0, rA | rB, [long] D

    // 4 | 0
    bigArray.push_back(joinByte((byte) 4, (byte) 0));

    std::vector<string> splitLine = split(line, " ");

    // not clean register 1 - means we have some "garbage" like the comma that we need to clean
    string uncleanr1 = splitLine.at(1); // %rsp,
    byte rAind = registerToVal(uncleanr1.substr(0, uncleanr1.size()-1));

    // 0x41c(%rbx) -> [0x41c, %rbx)]
    std::vector<string> split2 = split(splitLine.at(2), "(");
    string uncleanr2 = split2.at(1);// removing the ) at the end
    byte rBind = registerToVal(uncleanr2.substr(0, uncleanr2.size()-1));
    
    // rA | rB
    bigArray.push_back(joinByte(rAind, rBind));

    long long d = parseHex(split2.at(0));// 0x41c is split2.at(0)

    // same as irmovq, [long] D
    for(int i = 0; i < 8; i++){
        bigArray.push_back(
            getNthByte(d, i)
        );
    }
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

// TO COMPILE AND RUN:
// g++ main.cpp -o compiled ; ./compiled

int main() {
    // std::string myString = "Hello, World!";
    // std::cout << myString << std::endl;
    std::vector<byte> bigArray = {};
    // cmovXX("cmovl %rax %r14", bigArray);
    // irmovq("irmovq $0xabcd, %rdx", bigArray);
    // rmmovq("rmmovq %rsi, 0x41c(%rsp)", bigArray);

    // log bigArray
    cout << "bigArray Contents:\n" << std::hex;
    for (auto& el : bigArray) {
        std::cout << (int) el << std::hex << std::endl;
    }

    // print nth element
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
