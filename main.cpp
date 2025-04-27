#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstring>
#include <functional>
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

bool declaredcommandToEncodingMap = false;
unordered_map<string, int> commandToEncodingMap = {}; 
int commandToEncoding(string command) {
    if(declaredcommandToEncodingMap == false){
        declaredcommandToEncodingMap == true;
        
        commandToEncodingMap["ha"] = 0;
        commandToEncodingMap["no"] = 1;
        commandToEncodingMap["rr"] = 2;
        commandToEncodingMap["cm"] = 2;
        commandToEncodingMap["ir"] = 3;
        commandToEncodingMap["rm"] = 4;
        commandToEncodingMap["mr"] = 5;
        commandToEncodingMap["ad"] = 6;
        commandToEncodingMap["su"] = 6;
        commandToEncodingMap["an"] = 6;
        commandToEncodingMap["xo"] = 6;
        commandToEncodingMap["ca"] = 8;
        commandToEncodingMap["re"] = 9;
        commandToEncodingMap["pu"] = 10;
        commandToEncodingMap["po"] = 11;
    }
    if(command.substr(0,1)=="j"){
        return 7;
    }
    return commandToEncodingMap[command.substr(0,2)];
}

bool declaredEncodingToLenMap = false;
unordered_map<int, int> encodingToLenMap = {};
int encodingToLen(int encoding) {
    if(declaredEncodingToLenMap == false){
        declaredEncodingToLenMap = true;
        
        encodingToLenMap[0] = 1;
        encodingToLenMap[1] = 1;
        encodingToLenMap[2] = 2;
        encodingToLenMap[3] = 10;
        encodingToLenMap[4] = 10;
        encodingToLenMap[5] = 10;
        encodingToLenMap[6] = 2;

        encodingToLenMap[7] = 9;
        encodingToLenMap[8] = 9;
        encodingToLenMap[9] = 1;
        encodingToLenMap[10] = 2;
        encodingToLenMap[11] = 2;
    }

    return encodingToLenMap[encoding];
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

    long long d = 0;
    int exp = 0;

    int stopAt = 0;
    bool isNegative = s.at(0) == '-';
    if(isNegative){
        // negative
        stopAt = 1;
    }

    // cout << s << "\n";

    // cout << (int )isNegative << "\n";

    //remove 0x, if it exists
    std::vector<string> split1 = split(s, "x");
    if(split1.size()>1){
        s=split1.at(split1.size()-1);
    }
    else{
        s = split1.at(0);
    }

    // std::cout << "str" << s << "\n";

    for(int i = s.size()-1; i >= stopAt; i--){
        d += ((long long) hexMap[s.at(i)]) << exp;// raising to 2^exp = 2^(4*hex) which is right
        exp += 4;
    }

    if(isNegative){
        d *= -1;
    }
    
    return d;
}

byte getNthByte(long long d, int n){

    return (byte) ( ( (unsigned long long) d >> (8*n) ) & 0xFF );
    // return d >> ( (double)8 * n);
}

byte joinByte(byte b, byte i){
    // byte b
    // byte i
    // ->
    // b | i in a single byte (last 4 positions of each should be 0)
    return (byte) ((((long) b)<<4) | (long) i);
}

typedef void(*FunctionPointer)();

// void pushArray(int curIndex, std::vector<byte> bigArray){
//     // bigArray[curIndex++] = val;
// }

// half, nop, cmovXX, irmovq, rmmovq, mrmovq, OPq, jXX, call, ret, pushq, popq
void halt(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations) {
    bigArray.push_back((byte) 0);}

void nop(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations) {
    bigArray.push_back((byte) 16);
}

// cmovX rA, rB
// encoding: 2 | fn | rA | rB
// rrmovq
void cmovXX(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    // example: cmovle %rax, %rbx 
    // 2 | fn
    std::vector<string> splitLine = split(line, " ");// [cmovle, rax, rbx]
    std::byte fn = getFnFromSuffix(
        split(splitLine.at(0), "mov").at(1)// le
    );// -> 1 because le corresponds to 1

    // std::cout << (int) joinByte((byte) 2, (byte) 6);

    byte firstByte = joinByte( (byte) 2, fn );// 2 | 1

    bigArray.push_back(firstByte);

    string r1 = splitLine.at(1);
    byte rAind = registerToVal(r1.substr(0,r1.length() - 1));
    byte rBind = registerToVal(splitLine.at(2));
    
    bigArray.push_back(joinByte(rAind, rBind));
}

void irmovq(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
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
    // cout << d << "\n";
    for(int i = 0; i < 8; i++){
        // cout << i << (int) getNthByte(d, i) << "\n";
        bigArray.push_back(
            getNthByte(d, i)
        );
    }
}

void rmmovq(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
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

void mrmovq(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    // mrmovq 0x41c(%rbp), %rcx
    // 5 | 0, rA | rB, [long] D

    // 5 | 0
    bigArray.push_back(joinByte((byte) 5, (byte) 0));

    std::vector<string> splitLine = split(line, " ");
    
    // 0x41c(%rbp) -> [0x41c, %rbp)]
    std::vector<string> split2 = split(splitLine.at(1), "(");
    string uncleanr1 = split2.at(1);// removing the ), at the end
    byte rAind = registerToVal(uncleanr1.substr(0, uncleanr1.size()-2));
    
    byte rBind = registerToVal(splitLine.at(2));
    
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

void opQ(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    std::vector<string> splitLine = split(line, " ");
    byte fnByte = (byte) 0;
    if(splitLine.at(0)=="subq"){
        fnByte = (byte) 1;
    }
    else if(splitLine.at(0)=="andq"){
        fnByte = (byte) 2;
    }
    else if(splitLine.at(0)=="xorq"){
        fnByte = (byte) 3;
    }
    bigArray.push_back(joinByte((byte) 6, fnByte));
    
    string r1 = splitLine.at(1);
    byte rAind = registerToVal(r1.substr(0,r1.length() - 1));
    byte rBind = registerToVal(splitLine.at(2));
    
    bigArray.push_back(joinByte(rAind, rBind));

}

void call(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    std::vector<string> splitLine = split(line, " ");
    bigArray.push_back(joinByte((byte) 8, (byte) 0));
    
    int fnLocation = fnjmpLocations[splitLine.at(1)];
    for(int i = 0; i<8; i++){
        bigArray.push_back(
            getNthByte(fnLocation,i)
        );
    }
}

void jmp(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    // jmp rA
    // 7 | fn, [double] dest
    std::vector<string> splitLine = split(line, " ");
    
    bigArray.push_back(joinByte((byte) 7, getFnFromSuffix(splitLine.at(0).substr(1,splitLine.at(0).length()-1))));

    int jumpLocation = fnjmpLocations[splitLine.at(1)];
    
    for(int i = 0; i<8; i++){
        bigArray.push_back(
            getNthByte(jumpLocation,i)
        );
    }
    
}

void ret(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    bigArray.push_back(joinByte((byte) 9, (byte) 0));
}

void pushq(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    // pushq %rax
    // encoding: A | 0, rA | F
    bigArray.push_back(joinByte((byte) 10, (byte) 0));// 10 = A

    // rA | F
    string rA = split(line, " ").at(1);
    byte rAind = registerToVal(rA);
    bigArray.push_back(joinByte(rAind, (byte) 0b1111) );
}

void popq(string line, std::vector<byte>& bigArray, unordered_map<string,int>& fnjmpLocations){
    bigArray.push_back(joinByte((byte) 11, (byte) 0));// 11 = B
    
    string rA = split(line, " ").at(1);
    byte rAind = registerToVal(rA);
    bigArray.push_back(joinByte(rAind, (byte) 0b1111) );
}


// TO COMPILE AND RUN:
// g++ main.cpp -o compiled ; ./com0piled

int main() {
    // std::string myString = "Hello, World!";
    // std::cout << myString << std::endl;
    // cmovXX("cmovl %rax %r14", bigArray);
    // irmovq("irmovq $0xabcd, %rdx", bigArray);
    // rmmovq("rmmovq %rsi, 0x41c(%rsp)", bigArray);
    
    unordered_map<string, int> fnjmpLocations = {};

    // PASS 1 - getting the indicies and putting them in the function LUT
    int byteIndex = 0;
    std::ifstream assembly_code;
    assembly_code.open("file.s");
    string line;
    if(assembly_code.is_open()){
        while(std::getline(assembly_code, line))
        {
            if(line.length() == 0) {
                continue;
            }
            
            if((int)line.at(line.length()-1)==13){
                line = line.substr(0,line.size()-1); //Remove carridge return
            }
            if(line.length() == 0){
                continue; //Check if string is only newline
            }
            vector<string> splits = split(line,"    ");
            string lineWithoutTabs = splits.at(splits.size() - 1);
            
            if(lineWithoutTabs.substr(lineWithoutTabs.length() - 1, 1) == ":"){
                string withoutTheColon = lineWithoutTabs.substr(0, lineWithoutTabs.length() - 1);
                // cout << "setting fnJMP Locations\n" << withoutTheColon << "\n" << byteIndex << "\n";
                fnjmpLocations[withoutTheColon] = byteIndex;
                continue;
            }

            if(lineWithoutTabs.substr(0, 1) == "."){
                // assembler directive.
                string firstBit = lineWithoutTabs.substr(0, 4);
                vector<string> splitLine = split(lineWithoutTabs, " ");
                if(firstBit == ".pos"){
                    byteIndex = parseHex(splitLine.at(1)); 
                } else if(firstBit == ".qua"){
                    byteIndex += 8;
                } else if(firstBit == ".ali"){
                    int alignTo = parseHex(splitLine.at(1));
                    while(byteIndex % alignTo != 0){
                        byteIndex++;
                    }
                }
                continue;
            }
            
            if(lineWithoutTabs.substr(0,1)=="#"){
                continue;
            }

            byteIndex += encodingToLen(commandToEncoding(lineWithoutTabs));
        }
    }

    std::unordered_map<int, std::function<void(string line, std::vector<byte>&, unordered_map<string,int>&)>> encodingToConversionFn;
    encodingToConversionFn[0] = halt;
    encodingToConversionFn[1] = nop; 
    encodingToConversionFn[2] = cmovXX;
    encodingToConversionFn[3] = irmovq;
    encodingToConversionFn[4] = rmmovq;
    encodingToConversionFn[5] = mrmovq;
    encodingToConversionFn[6] = opQ;
    encodingToConversionFn[7] = jmp;
    encodingToConversionFn[8] = call;
    encodingToConversionFn[9] = ret;
    encodingToConversionFn[10] = pushq;
    encodingToConversionFn[11] = popq;
    

    std::vector<byte> bigArray = {};

    unordered_map<string, int> loc = {};
    
    std::ifstream assembly_code_reopen;
    assembly_code_reopen.open("file.s");
    byteIndex = 0;
    if(assembly_code_reopen.is_open()){
        while(std::getline(assembly_code_reopen, line)){
            if(line.length() == 0) continue;
            
            if((int)line.at(line.length()-1)==13){
                line = line.substr(0,line.size()-1); //Remove carridge return
            }

            if(line.length() == 0) continue; //Check if line is only newline
            // cout << line << '\n';

            // [empty line] or
            // [tab]
            vector<string> splits = split(line,"    ");
            string lineWithoutTabs = splits.at(splits.size() - 1);

            if(lineWithoutTabs.length() == 0){
                continue;
            }

            if(lineWithoutTabs.substr(0, 1) == "."){
                // assembler directive.
                string firstBit = lineWithoutTabs.substr(0, 4);
                vector<string> splitLine = split(lineWithoutTabs, " ");
                if(firstBit == ".pos"){
                    
                    byteIndex = parseHex(splitLine.at(1));
                    for(int i = 0; i < byteIndex; i++){
                        bigArray.push_back((byte) 0);
                    }
                } else if(firstBit == ".qua"){
                    long long data = parseHex(splitLine.at(1));
                    for(int i = 0; i < 8; i++){
                        bigArray.push_back(getNthByte(data, i));
                    }
                    byteIndex += 8;
                } else if(firstBit == ".ali"){
                    int alignTo = parseHex(splitLine.at(1));
                    while(byteIndex % alignTo != 0){
                        bigArray.push_back((byte) 0);
                        byteIndex++;
                    }
                }
                continue;
            }

            if(lineWithoutTabs.substr(lineWithoutTabs.length() - 1, 1) == ":"){
                // string withoutTheColon = lineWithoutTabs.substr(0, lineWithoutTabs.length() - 1);
                // addLabel();// loop:
                continue;
            }
            
            int encoding = commandToEncoding(lineWithoutTabs);
            
            encodingToConversionFn[encoding](line, bigArray, fnjmpLocations);
            byteIndex += encodingToLen(commandToEncoding(lineWithoutTabs));
        }
        assembly_code.close();
    }

    // log bigArray
    cout << "bigArray Contents:\n" << std::hex;
    for (auto& el : bigArray) {
        std::cout << (int) el << " ";
    }
    
    return 0;
}
