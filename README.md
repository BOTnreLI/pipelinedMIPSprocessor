# Pipelined MIPS processor, C++
## UMass Amherst 2020 FA ECE510 project 3
### Project Goal
1. Understand 5 stage pipelined machine and multiple events that occur in parallel. 
2. Build C++ program capability
#### By the assignment requirement

*This assignment requires a simple 5 stage pipelined machine to be simulated. The simulator should be capable of implementing the MIPS architecture on a cycle by cycle basis. The simulator must be cycle accurate with respect to contents of the registers, but need not be faithful to other hardware details such as control signals. The output of the simulator, in addition to the register contents and latch values should include the utilization factor of each functional unit and the total time in cycles to execute a set of instructions. Implement the simulator according to the specifications described below in C++. Submit the code, simulation results and a project description write-up.*

##### Instructions to be simulated
The simulator should implement the following instructions: add, sub, addi, mul, lw, sw, beq, lui, and, andi, or, ori, sll, srl, slti, and sltiu. Note that these instructions operate integer instructions only. The MIPS instruction format can be used for all instructions except mul. 
:::
Assume the syntax for mul is mul $a,$b,$c, meaning that we multiply the contents of $b and $c, the least significant 32 bits of results are placed in register $a and the most significant 32-bits of the result will be stored in register $(a+1). For example, mul $t0, $t8, $t9 will store lower 32-bits of the product of $t8 * $t9 in register $t0 and the upper 32-bits of the product in register $t1 (Hint: See MIPS green sheet instructions summary for registers numbering). This is different from the mult instruction in MIPS. Assume the opcode and function code for mul to be same as that of mult.
:::

#### Inputs to the simulator
1. MIPS machine code as a text file
2. A query to the user to select between instruction or cycle mode: Instruction mode and Cycle mode
3. A query to the user to select the number of instructions or cycles (depending on the choice made in the previous query)
4. After executing the number of instructions or cycles entered initially by the user, a third query to the user to choose to continue execution or not.
    -If yes, repeats from step 3.
    -If no, exits the execution and demonstrate the result.
    
#### Other requirements
##### Memory, Registers and PC
The memory is one word wide and 2K bytes in size. There are physically separate instruction and data memories for the instruction and data. Data memory is initialized to 0 at the beginning of each simulation run. There is no cache in this machine.
There are 32 registers; register 0 is hardwired to 0. In addition, there is a Program Counter (PC). PC should start execution by fetching the instruction stored in the location to which it is initialized.

##### CPU
The pipelined MIPS processor has 5 stages: IF, ID, EX, MEM, WB. There are pipeline registers between the stages: IF/ID, ID/EX, EX/MEM, MEM/WB. Assume the pipeline registers to contain following latches:
• IF/ID : IR, NPC
• ID/EX:IR,NPC,A,B,Imm
• EX/MEM : IR, B, ALUOutput , cond
• MEM/WB : IR, ALUOutput, LMD

##### Output of the simulator
In addition to displaying the register contents and latch values after the execution of each cycle/instruction, it should output the following statistics
• Utilization of each stage. Utilization is the fraction of cycles for which the stage is doing useful work. Just waiting for a structural, control, or data hazard to clear in front of it does not constitute useful work.
• Total time (in CPU cycles) taken to execute the MIPS program on the simulated machine. (This is NOT the time taken to execute the simulation; it is the time taken by the machine being simulated.)

### Final report

#### Improvement list
> 1. hazard handle, include 3 different types(Structure, Data, Control hazard).
> 2. Utilization of memory calculation
> 3. Better Interface
