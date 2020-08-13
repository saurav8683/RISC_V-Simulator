# RISC_V-Simulator
                                         CS204 PROJECT : Phase 1
Group members(5):
1.Gyan Prakash Singh(2017csb1077)
2.Utkarsh Kumar(2017csb1116)
3.Soumya (2017csb1114)
4.Aman(2017csb1067)
5.Saurav (2017csb1106)

Phase 1:
Spiliting of the task:

a.Contribution of Aman:I type ,S type,U type,UJ type of instructions were handled by him.
b.Contribution of Gyan:SB type,R type of instructions were handled by him.
c. (.data) and .text and memory handling part were handled by both Gyan and Aman.
d. Contribution of Utkarsh Kumar :separating all type of instructions and their different fields and making different files for the use of instruction code.
e. Soumya and Saurav: Testing of the code and other utility functions for interconversion of Decimal ,binary , and hexadecimal.

How to run the programme:
1.   Check all the files of instruction detail( we are already including Them in the folder  )
2.   Include the programme File ( File with assembly code   assembly_code.txt )
3.   Then Run The command ->g++ phase1.cpp
4.   You will Recieve three files machine_code.mc ( machine code file ) , MEM.txt ( Memory to send in phase 2), a_c_.txt ( an intermediate file) .  


Phase 2:
Spiliting of the task:

a.Contribution of Saurav : Decode function , ALU stage , Control Part , Making Utility file related to instructions 
b.Contribution of Soumya : Memory stage , update stage , Control Part
c.Contribution of Utkarsh : Fetch stage , ALU stage ( some part) ,Control Part 
d.Contribution of Aman : Related file handling 
e.Contribution of Gyan Prakash: ALU Tesing  and Final Code Testing  


How to run the programme:
1.   Check all the files of instruction detail( we are already including Them in the folder  )
2.   Include the programme File ( File with assembly code   assembly_code.txt )
3.   Then Run The command ->g++ phase1.cpp
4.   You will Recieve three files machine_code.mc ( machine code file ) , MEM.txt ( Memory to send in phase 2), a_c_.txt ( an intermediate file) .  
5.   Then Run The command ->g++ phase2.cpp
6.	You will Recieve one file-> MEM1.txt ( Memory updated in phase 2) .
7.   Values of Registers will be printed.  
since the data part of the code will be passed from first part with a file name MEM.txt so first part is also included in this.



Assumptions and Limitations :

1.   We have handled all the instruction having opcode in the reference sheet other than li , lui , sd ,ld ,fence.i , fence .

Memory array size: 2000
0-1000  - instruction memory
1000-1500 - data memory 
2000- lower side -- stack 
Heap - Not available 

2.  if la instruction comes we change it to only auipc because   because only memory size is small so it can be handled only in this . 
   if lw  with variable  comes then it is broken in two instructions auipc and lw.
   since number of instruction is differed so dont try to match machine code exactly with venus .


3.     loop:   add x2,x3,x4 
       will not work 
       write it as 
       loop:
       add x2,x3,x4

4. jalr  x1  0(x2)   is the right formate.

5.












































