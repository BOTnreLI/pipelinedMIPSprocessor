#include <iostream>
#include <string>
#include <map>
#include <fstream>


using namespace std;

#define maximum 2048
#define regNum 32

#define maximumMEM 2048 // using 2k memory for instruction and data memory


class latch{
    public:
        string instruction;
        string rd;
        string rs;
        string rt;
        string Imm;
        string opcode;
        string func;
        string shamt;
        int ALUout;
        int cond;
        int LMD;
        int PC; //program counter;
        int finishedCounter;
        latch();
        void signExtendtion();

};

void latch::signExtendtion(){
    if(this->Imm[0] == '0'){
        this->Imm = "0000000000000000" + this->Imm;
    }
    else if(this->Imm[0] == '1'){
        this->Imm = "1111111111111111" + this->Imm;
    }
}

latch::latch(){  // INIT()
    instruction = "0";
    rd = "0";
    rs = "0";
    rt = "0";
    Imm = "0";
    opcode = "0";
    ALUout = 0;
    cond = 0;
    LMD = 0;
    PC = 0;
    finishedCounter = 0;
}

class mainProcessor{
    public:
        latch IF_ID;
        latch ID_EXE;
        latch EXE_MEM;
        latch MEM_WB;
        string instruction[maximumMEM];
        string data[maximumMEM];
        void fetch();
        void decode();
        void execute();
        void memory();
        void writeBack();
        int reg[regNum];
        int programCounter;
        int stallCounter;
        void INIT();
        int stupid;
        void NOPoperation();
};

void mainProcessor::INIT(){ // mainProcessor INIT()
    latch();
    stupid = 0;

    reg[0] = 0; // "$zero", constant value zero;
    reg[1] = 0; // "$at"

    reg[2] = 0; // "$v0"
    reg[3] = 0; // "$v1"

    reg[4] = 0; // "$a0"
    reg[5] = 0; // "$a1"
    reg[6] = 0; // "$a2"
    reg[7] = 0; // "$a3"

    reg[8] = 0; // "$t0"
    reg[9] = 0; // "$t1"
    reg[10] = 0; // "$t2"
    reg[11] = 0; // "$t3"
    reg[12] = 0; // "$t4"
    reg[13] = 0; // "$t5"
    reg[14] = 0; // "$t6"
    reg[15] = 0; // "$t7"

    reg[16] = 0; // "$s0"
    reg[17] = 0; // "$s1"
    reg[18] = 0; // "$s2"
    reg[19] = 0; // "$s3"
    reg[20] = 0; // "$s4"
    reg[21] = 0; // "$s5"
    reg[22] = 0; // "$s6"
    reg[23] = 0; // "$s7"

    reg[24] = 0; // "$t8"
    reg[25] = 0; // "$t9"
 
    reg[26] = 0; // "$k0"
    reg[27] = 0; // "$k1"

    reg[28] = 0; // "$gp"
    reg[29] = 0; // "$sp"
    reg[30] = 0; // "$fp"
    reg[31] = 0; // "$ra"

    programCounter = 0;
    stallCounter = 0;
}

void mainProcessor::NOPoperation(){
    //insert the NOP and stall when hazard might happen
    //
}

void mainProcessor::fetch(){
   IF_ID.instruction = instruction[IF_ID.PC];
   if(IF_ID.instruction != "0"){
       if(EXE_MEM.cond && EXE_MEM.opcode == "000100"){
           IF_ID.PC = EXE_MEM.ALUout;
       }else{
           IF_ID.PC += 1;
       }
   }else{
       stallCounter += 1;
   }
}

void mainProcessor::decode(){
    ID_EXE.instruction = IF_ID.instruction;
    EXE_MEM.instruction = ID_EXE.instruction;
    if(ID_EXE.instruction != ""  && ID_EXE.instruction != "0"){
        //slicing the string.
        EXE_MEM.opcode = ID_EXE.instruction.substr(0, 6); //0-6
    
        if(ID_EXE.opcode == "000000"){ // R- type
            EXE_MEM.rt = (ID_EXE.instruction.substr(12, 5));
            EXE_MEM.rs = (ID_EXE.instruction.substr(7, 5));
            EXE_MEM.rd = (ID_EXE.instruction.substr(17, 5));
            EXE_MEM.func = (ID_EXE.instruction.substr(27, 6));
            EXE_MEM.shamt = (ID_EXE.instruction.substr(22, 5));
        }else{ // I- type
            EXE_MEM.rs = (ID_EXE.instruction.substr(6, 5)); // 6 - 10
            EXE_MEM.rt = (ID_EXE.instruction.substr(11, 5)); //11 - 15
            EXE_MEM.Imm = (ID_EXE.instruction.substr(16, 16)); // 16 - 31
            if(EXE_MEM.instruction != ""){
                EXE_MEM.signExtendtion();
            }
        }
    }else{
        stallCounter += 1;
    }
}
void mainProcessor::execute(){
    // ID_EXE -> EXE_MEM
    /*
    string rs, rd, rt, opcode, func, Imm, shamt
    */
    
    EXE_MEM.instruction = ID_EXE.instruction;
    EXE_MEM.rs = ID_EXE.rs;
    EXE_MEM.rt = ID_EXE.rt;
    EXE_MEM.rd = ID_EXE.rd;
    EXE_MEM.Imm = ID_EXE.Imm;
    EXE_MEM.opcode = ID_EXE.opcode;
    EXE_MEM.func = ID_EXE.func;
    EXE_MEM.shamt = ID_EXE.shamt;
    
    // string to interger
    string Instruction = ID_EXE.instruction;

    

    if(EXE_MEM.opcode == "000000"){ // R - type
        if(EXE_MEM.func == "100000"){ // add => rd = rs + rt
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] + reg[stoi(EXE_MEM.rt, nullptr, 2)];
        }else if(EXE_MEM.func == "100010"){ // sub => rd = rs - rt
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] - reg[stoi(EXE_MEM.rt, nullptr, 2)];
        }else if(EXE_MEM.func == "100100"){ // and => rd = rs & rt
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] & reg[stoi(EXE_MEM.rt, nullptr, 2)];
        }else if(EXE_MEM.func == "100101"){ // or => rd = rs | rt
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] | reg[stoi(EXE_MEM.rt, nullptr, 2)];
        }else if(EXE_MEM.func == "000000"){ // sll => rd = rt << shamt
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rt, nullptr, 2)] << stoi(EXE_MEM.shamt, nullptr, 2);
        }else if(EXE_MEM.func == "000010"){ // srl => rd = rt >> shamt
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rt, nullptr, 2)] >> stoi(EXE_MEM.shamt, nullptr, 2);
        }else if(EXE_MEM.func == "011000"){ //mul => {Hi, Lo} = rs * rt
            EXE_MEM.ALUout = stoi(EXE_MEM.rs, nullptr, 2) * stoi(EXE_MEM.rt, nullptr, 2);
        }
        else{
            stallCounter += 1;
        }
    }else{ // I - type
        if(EXE_MEM.opcode == "000100"){ // op = 4, beq, if rs == rt, pc + 4
            if(reg[stoi(EXE_MEM.rs, nullptr, 2)] == reg[stoi(EXE_MEM.rt, nullptr, 2)]){
                EXE_MEM.PC = EXE_MEM.PC + stoi(EXE_MEM.Imm, nullptr, 2) +1;
                EXE_MEM.cond = 1;
            }
        }
        else if(EXE_MEM.opcode == "001000"){ //op = 8, addi, rt = rs + Imm;
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] + stoi(EXE_MEM.Imm, nullptr, 2);
        }else if(EXE_MEM.opcode == "001100"){ // op = 12, andi rt = rs & Imm;
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] & stoi(EXE_MEM.Imm, nullptr, 2);
        }else if(EXE_MEM.opcode == "001101"){ // op = 13, ori, rt = rs | Imm;
            EXE_MEM.ALUout = reg[stoi(EXE_MEM.rs, nullptr, 2)] | stoi(EXE_MEM.Imm, nullptr, 2);
        }else if(EXE_MEM.opcode == "001010"){ // op = 10, slti, rt = (rs < Imm) ? 1 ; 0
            EXE_MEM.ALUout = (reg[stoi(EXE_MEM.rs, nullptr, 2)] < stoi(EXE_MEM.Imm, nullptr, 2)) ? 1 : 0;
        }else if(EXE_MEM.opcode == "001011"){ // op = 11, sltiu, rt = (rs < Imm) ? 1 : 0
            EXE_MEM.ALUout = (reg[stoi(EXE_MEM.rs, nullptr, 2)] < stoi(EXE_MEM.Imm, nullptr, 2)) ? 1 : 0;
        }else if(EXE_MEM.opcode == "001111"){ // op = 15, lui, rt = upper bit of Imm
            EXE_MEM.ALUout = stoi(EXE_MEM.Imm.substr(0,15), nullptr, 2);//type unsigned long;
        }else if(EXE_MEM.opcode == "110101"){ // op = 35, lw from memory to register
            EXE_MEM.ALUout = stoi(EXE_MEM.rs, nullptr, 2) + stoi(EXE_MEM.Imm, nullptr, 2);
            EXE_MEM.instruction = Instruction;
            EXE_MEM.rt = stoi(EXE_MEM.rt, nullptr, 2);
        }else if(EXE_MEM.opcode == "101011"){ // op = 43, sw register to memory
            EXE_MEM.ALUout = stoi(EXE_MEM.rs, nullptr, 2) + stoi(EXE_MEM.Imm, nullptr, 2);
            EXE_MEM.instruction = Instruction;
            EXE_MEM.rt = stoi(EXE_MEM.rt, nullptr, 2);
        }
        else{
            stallCounter += 1;
        }
    }
}

void mainProcessor::memory(){
    //on do lw, sw here
    //others parameter pass to next stage
    //exe_mem -> mem_wb
    MEM_WB.instruction = EXE_MEM.instruction;
    MEM_WB.rs = EXE_MEM.rs;
    MEM_WB.rt = EXE_MEM.rt;
    MEM_WB.rd = EXE_MEM.rd;
    MEM_WB.Imm = EXE_MEM.Imm;
    MEM_WB.opcode = EXE_MEM.opcode;
    MEM_WB.func = EXE_MEM.func;
    MEM_WB.shamt = EXE_MEM.shamt;
    MEM_WB.ALUout = EXE_MEM.ALUout;

    if(EXE_MEM.instruction != ""){
        MEM_WB.ALUout = EXE_MEM.ALUout;
    MEM_WB.instruction = EXE_MEM.instruction;
    if(EXE_MEM.instruction.substr(0, 6) == "100011"){ //lw
        MEM_WB.LMD = stoi(data[EXE_MEM.ALUout]);
    }else if(EXE_MEM.instruction.substr(0, 6) == "101111"){ // sw
        data[EXE_MEM.ALUout] = EXE_MEM.rt;
    }
    }else{
        stallCounter += 1;
    }
}

void mainProcessor::writeBack(){
    if(MEM_WB.instruction != ""){
        if(MEM_WB.opcode == "100011"){   //op = 35,  lw
        reg[stoi(MEM_WB.rt, nullptr, 2)] = MEM_WB.LMD;
        }
        else{
        reg[stoi(MEM_WB.rt, nullptr, 2)] = MEM_WB.ALUout;
        stupid +=1;
    }
    }else{
        stallCounter+=1;
    }

 }


int main(){
    int answer=0;
    bool done = false;

    string fileName;
    string inputString;

    mainProcessor Processor;

    do {
    int counter = 0;
    ifstream fin;
    string fileName;
    string inputString;
    int cycle = 0;
    int i =0;
    cout << "Welcom, here's the BOT_NRE's MIPS simulator\n\n";
    cout << "In this simulator, I'm only implement some instructions\n\n";
    cout << "Mode 1. Instruction Mode \n";
    cout << "Mode 2. Cycle Mode\n";
    cout << "LEAVE 3. leave program \n";
    cin >> answer;

    switch(answer){
        case 1: // instruction mode
            cout << "enter the .txt file name\n";
            cin >> fileName;
            if(!fileName.empty()){
                cout << "receiving and executing..." << endl;
                fin.open(fileName);
                if(!fin){
                    cout << "I/O failaure, go check the permission of it" << endl;
                }else{
                    cout << "file opened" << endl;
                }
                while(getline(fin, inputString)){// start from here
                    Processor.instruction[counter] = inputString;
                    counter++;
                }// read instructions <- end
                Processor.INIT();
                while(Processor.stupid != counter){
                    Processor.writeBack();
                    Processor.memory();
                    Processor.execute();
                    Processor.decode();
                    Processor.fetch();
                }
            }else{
            
                cout << "invaild input name" << endl;
            }
            break;
        case 2: // cycle mode
            cout << "enter the cycle that you wanna execute.\n";
            cin >> cycle;
            cout << "wanna execute" << cycle << "cycles" << endl;
            cout << "input the fileName" << endl;
            cin >> fileName;
            if(!fileName.empty()){
                cout << "receiving and executing..." << endl;
                fin.open(fileName);
                if(!fin){
                    cout << "I/O failaure, go check the permission of it" << endl;
                }else{
                    cout << "file opened" << endl;
                }
                while(getline(fin, inputString)){
                    Processor.instruction[counter] = inputString;
                    counter++;
                }
                Processor.INIT();
                while(i < counter){
                    Processor.writeBack();
                    Processor.memory();
                    Processor.execute();
                    Processor.decode();
                    Processor.fetch();
                    i += 1;
                }
            }else{
            
                cout << "invaild input name" << endl;
            }
            break;
        case 3:
            cout << "leaving the program...\n";
            done = true;
            break;
        default:
            cout << "You should choose between \"Mode 1\" and \"Mode 2\" \n";
            break;
        }
    } while(!done);

    return 0;
}
