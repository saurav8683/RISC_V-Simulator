#include<iostream>
#include<bits/stdc++.h>
using namespace std;

bool memory[2000][32],reg[32][32],IR[32],mux_INC=0;
int RA,RB,PC=0,RZ,RY,RM,immediate,branch_offset,ALU_op=0,c,PC_temp,clock_cycle_count=0,q,INST_COUNT=0,MIS_PREDICTION=0,CONTROL_STMT=0,CNT_HAZARD=0,DATA_HZD=0;
int DATA_STALLS=0,CONTROL_STALLS=0,STALLS=0,DATA_TRANSFER=0,ALU_INST=0;   //declaring memory and intermediate registers.
bool rs1[5],rs2[5],rd[5],imm[13],immu[21],ti[32];
	int aluresult,rs1_i,rs2_i,rd_i,imm_i,imm_u,t=1999*4,type;


int Reg[5][18];

int bin2dec(bool data[],int n,int sign=0)
{
	int temp=1,sum=0,flag=0;
	bool temp_data[n];
	for(int i=0;i<n;i++)
		temp_data[i]=data[i];
	if(sign!=0&&temp_data[0]==1)
	{
		for(int i=0;i<n;i++)
		{
			if(temp_data[i]==0)
				temp_data[i]=1;
			else
				temp_data[i]=0;
		}
		for(int i=n-1;i>=0;i--)
		{
			if(temp_data[i]==0)
			{
				temp_data[i]=1;
				break;
			}
			else
				temp_data[i]=0;
		}
		flag=1;
	}
	while(n>0)
	{
		sum+=temp*temp_data[n-1];
		temp*=2;
		n--;
	}
	if(flag==1)
		return sum*-1;
	return sum;
}

void dec2bin(bool value[32],int n,int size=32)
{

	for(int j=0;j<size;j++)
		value[j]=0;
	int i=1,flag=0;
	if(n<0)
	{
		flag=1;
		n*=-1;
	}
	while(n!=0)
	{
		value[size-i]=n%2;
		n/=2;
		i++;
	}
	if(flag==1)
	{
		for(i=0;i<size;i++)
		{
			if(value[i]==0)
				value[i]=1;
			else
				value[i]=0;
		}
		for(i=size-1;i>=0;i--)
		{
			if(value[i]==0)
			{
				value[i]=1;
				break;
			}
			else
				value[i]=0;
		}
	}

}

void xyz(char abc[],bool b[32])
{
	for(int i=0;i<8;i++)
	{
		if(abc[i]=='0')
		{
			b[4*i]=0;
			b[4*i+1]=0;
			b[4*i+2]=0;
			b[4*i+3]=0;
		}
		else if(abc[i]=='1')
		{
			b[4*i]=0;
			b[4*i+1]=0;
			b[4*i+2]=0;
			b[4*i+3]=1;
		}
		else if(abc[i]=='2')
		{
			b[4*i]=0;
			b[4*i+1]=0;
			b[4*i+2]=1;
			b[4*i+3]=0;
		}
		else if(abc[i]=='3')
		{
			b[4*i]=0;
			b[4*i+1]=0;
			b[4*i+2]=1;
			b[4*i+3]=1;
		}
		else if(abc[i]=='4')
		{
			b[4*i]=0;
			b[4*i+1]=1;
			b[4*i+2]=0;
			b[4*i+3]=0;
		}
		else if(abc[i]=='5')
		{
			b[4*i]=0;
			b[4*i+1]=1;
			b[4*i+2]=0;
			b[4*i+3]=1;
		}
		else if(abc[i]=='6')
		{
			b[4*i]=0;
			b[4*i+1]=1;
			b[4*i+2]=1;
			b[4*i+3]=0;
		}
		else if(abc[i]=='7')
		{
			b[4*i]=0;
			b[4*i+1]=1;
			b[4*i+2]=1;
			b[4*i+3]=1;
		}
		else if(abc[i]=='8')
		{
			b[4*i]=1;
			b[4*i+1]=0;
			b[4*i+2]=0;
			b[4*i+3]=0;
		}
		else if(abc[i]=='9')
		{
			b[4*i]=1;
			b[4*i+1]=0;
			b[4*i+2]=0;
			b[4*i+3]=1;
		}
		else if(abc[i]=='A'||abc[i]=='a')
		{
			b[4*i]=1;
			b[4*i+1]=0;
			b[4*i+2]=1;
			b[4*i+3]=0;
		}
		else if(abc[i]=='B'||abc[i]=='b')
		{
			b[4*i]=1;
			b[4*i+1]=0;
			b[4*i+2]=1;
			b[4*i+3]=1;
		}
		else if(abc[i]=='C'||abc[i]=='c')
		{
			b[4*i]=1;
			b[4*i+1]=1;
			b[4*i+2]=0;
			b[4*i+3]=0;
		}
		else if(abc[i]=='D'||abc[i]=='d')
		{
			b[4*i]=1;
			b[4*i+1]=1;
			b[4*i+2]=0;
			b[4*i+3]=1;
		}
		else if(abc[i]=='E'||abc[i]=='e')
		{
			b[4*i]=1;
			b[4*i+1]=1;
			b[4*i+2]=1;
			b[4*i+3]=0;
		}
		else if(abc[i]=='F'||abc[i]=='f')
		{
			b[4*i]=1;
			b[4*i+1]=1;
			b[4*i+2]=1;
			b[4*i+3]=1;
		}
	}
}

int assign(int instype)
{
	if(instype==28||instype==10||(instype>=24&&instype<=26)||instype==51||instype==50||(instype>=1&&instype<=7))
		return 1;
	else if(instype==29)
		return 2;
	else if(instype==37||instype==18)
		return 3;
	else if(instype==36||instype==17)
		return 4;
	else if(instype==29)
		return 5;
	else if(instype==33||instype==14)
		return 6;
	else if(instype==29)
		return 7;
	else if(instype==30||instype==11)
		return 8;
	else if(instype==34||instype==15||instype==35)
		return 9;
	else if(instype==46||instype==48||instype==31)
		return 10;
	else if(instype==44)
		return 11;
	else if(instype==49)
		return 17;
	else if(instype==45)
		return 16;
	else if(instype==16)
		return 14;
	else if(instype==32)
		return 15;
	else if(instype==47)
		return 17;
	else if(instype==19)
		return 18;
}

void read()
{
	ifstream f;
	f.open("machine_code.mc");
	char arr[1000][8];
	c=0;
	string line;
	getline(f,line);
	while(f)
	{
		cout<<line<<" ";
		for(int i=0;i<8;i++)
		arr[c][i]=line[i];
		c++;
	getline(f,line);
	}
	f.close();
	for(int i=0;i<c;i++)
	{
		xyz(arr[i],memory[i]);
	}
	ifstream fin;
	fin.open("MEM.txt");
	string line1;
	int c1=1000;
	getline(fin,line1);
	while(fin){
	for(int j=0;j<32;j++)
    {
       memory[c1][j]=(line1[j]-48);
    }
    getline(fin,line1);
    c1++;
	}
	fin.close();
}



void fetch1()
{
	for(int i=0;i<32;i++)
		IR[i]=memory[PC][i];

	PC+=1;
}

void decode21(int instype)
{
	if((instype>=28&&instype<=37)||(instype>=39&&instype<=43))					//R type
	{
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rs2[i]=IR[7+i];
			rd[i]=IR[20+i];
		}

		rs1_i=bin2dec(rs1,5);
		rs2_i=bin2dec(rs2,5);
		RA=bin2dec(reg[rs1_i],32,1);
		RB=bin2dec(reg[rs2_i],32,1);
		rd_i=bin2dec(rd,5);
		ALU_op=assign(instype);
		cout<<"ALU_op="<<ALU_op<<" ";
		type=1;
	}
	else if((instype>=1&&instype<=18)||(instype>=20&&instype<=23)||instype==27||instype==50)		//I TYpe
	{
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rd[i]=IR[20+i];
		}
		if(instype==16||instype==23)
		{
			for(int i=0;i<5;i++)
			{
			imm[i]=IR[7+i];
			}
			RB=bin2dec(imm,5);
		}
		else
		{
		for(int i=0;i<12;i++)
		{
			imm[i]=IR[i];
		}
			RB=bin2dec(imm,12,1);
		}
		cout<<"\n";
		rs1_i=bin2dec(rs1,5);
		rd_i=bin2dec(rd,5);
		RA=bin2dec(reg[rs1_i],32,1);
		ALU_op=assign(instype);
		cout<<"ALU_op="<<ALU_op<<"\n";
		if(instype==50)
		{
			PC=RZ/4;
			RZ=PC_temp*4;
		}
	}
	else if((instype>=24&&instype<=26))				//S Type
	{
	    type=3;
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rs2[i]=IR[7+i];
			imm[7+i]=IR[20+i];
		}
		for(int i=0;i<7;i++)
		{
			imm[i]=IR[i];
		}
		rs1_i=bin2dec(rs1,5);
		rs2_i=bin2dec(rs2,5);
		RA=bin2dec(reg[rs1_i],32,1);
		RB=bin2dec(imm,12,1);
		ALU_op=assign(instype);
		cout<<"ALU_op="<<ALU_op<<"\n";
		q=bin2dec(reg[rs2_i],32,1);
		type=2;
	}
	else if((instype>=44&&instype<=49))		//SB Type
	{
	    type=4;
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rs2[i]=IR[7+i];
			imm[2+i]=IR[1+i];
		}
		imm[0]=IR[0];
		imm[1]=IR[24];
		imm[7]=IR[6];
		imm[12]=0;
		for(int i=0;i<4;i++)
		{
			imm[8+i]=IR[20+i];
		}
		rs1_i=bin2dec(rs1,5);
		rs2_i=bin2dec(rs2,5);
		RA=bin2dec(reg[rs1_i],32,1);
		RB=bin2dec(reg[rs2_i],32,1);
		if(instype==48||instype==49)
		{
			RA=abs(RA);
			RB=abs(RB);
		}
		cout<<"\n";
		branch_offset=bin2dec(imm,13,1)/4;
		ALU_op=assign(instype);
		cout<<"ALU_op="<<ALU_op<<endl;
	}
	else if(instype==51)				//UJ type
	{
	    type=5;
		for(int i=0;i<5;i++)
		{
			rd[i]=IR[20+i];
		}
		immu[0]=IR[0];
		for(int i=0;i<8;i++)
			immu[i+1]=IR[12+i];
		immu[9]=IR[11];
		for(int i=0;i<10;i++)
			immu[10+i]=IR[1+i];
		immu[20]=0;
		for(int i=0;i<5;i++)
		{
			rd[i]=IR[20+i];
		}
		rd_i=bin2dec(rd,5);
		RA=PC-1;


		RB=bin2dec(immu,21,1)/4;
		ALU_op=assign(instype);
		PC_temp=PC;
		cout<<"ALU_op="<<ALU_op<<"\n";
		RZ=PC_temp*4;
	}
	else if(instype==19)
	{
	    type=6;
		ALU_op=assign(instype);
		for(int i=0;i<20;i++)
			imm[i]=IR[i];
		RZ=bin2dec(imm,20);
		for(int i=0;i<5;i++)
			rd[i]=IR[20+i];
		rd_i=bin2dec(rd,5);
	}
}

int decode1(bool data[])         //Takes the word as input and gives an integer
{
    int instype;                          //decode
	bool opcode[7],fn3[3],fn7[7];
	int op,fun3,fun7,i;
	for(i=0;i<7;i++)
	{
		fn7[i]=data[i];
		opcode[i]=data[25+i];
	}
	for(i=0;i<3;i++)
		fn3[i]=data[17+i];
	op=bin2dec(opcode,7);
	fun3=bin2dec(fn3,3);
	fun7=bin2dec(fn7,7);
	if(op==3)
		{
			if(fun3==0)
				instype= 1;
			else if(fun3==1)
				instype= 2;
			else if(fun3==2)
				instype= 3;
			else if(fun3==3)
				instype= 4;
			else if(fun3==4)
				instype= 5;
			else if(fun3==5)
				instype= 6;
			else if(fun3==6)
				instype= 7;
		}
	else if(op==19)
	{
			if(fun3==0)
				instype= 10;
			else if(fun3==1&&fun7==0)
				instype= 11;
			else if(fun3==2)
				instype= 12;
			else if(fun3==3)
				instype= 13;
			else if(fun3==4)
				instype= 14;
			else if(fun3==5&&fun7==0)
				instype= 15;
			else if(fun3==5&&fun7==32)
				instype= 16;
			else if(fun3==6)
				instype= 17;
			else if(fun3==7)
				instype= 18;
	}
	else if(op==23)
		instype= 19;
	else if(op==27)
	{
			if(fun3==0)
				instype= 20;
			else if(fun3==1)
				instype= 21;
			else if(fun3==5&&fun7==0)
				instype= 22;
			else if(fun3==5&&fun7==32)
				instype= 23;
	}
	else if(op==35)
	{
			if(fun3==0)
				instype= 24;
			else if(fun3==1)
				instype= 25;
			else if(fun3==2)
				instype= 26;
			else if(fun3==3)
				instype= 27;
	}
	else if(op==51)
	{
			if(fun3==0&&fun7==0)
				instype= 28;
			else if(fun3==0&&fun7==32)
				instype= 29;
			else if(fun3==1)
				instype= 30;
			else if(fun3==2&&fun7==0)
				instype= 31;
			else if(fun3==3&&fun7==0)
				instype= 32;
			else if(fun3==4&&fun7==0)
				instype= 33;
			else if(fun3==5&&fun7==0)
				instype= 34;
			else if(fun3==5&&fun7==32)
				instype= 35;
			else if(fun3==6&&fun7==0)
				instype= 36;
			else if(fun3==7&&fun7==0)
				instype= 37;
	}
	else if(op==55)
	{
		if(fun3==0&&fun7==0)
				instype= 39;
		else if(fun3==0&&fun7==32)
				instype= 40;
		else if(fun3==1&&fun7==0)
				instype= 41;
		else if(fun3==5&&fun7==0)
				instype= 42;
		else if(fun3==5&&fun7==32)
				instype= 43;
		else
			instype= 38;
	}
	else if(op==99)
	{
		if(fun3==0)
				instype= 44;
		else if(fun3==1)
				instype= 45;
		else if(fun3==4)
				instype= 46;
		else if(fun3==5)
				instype= 47;
		else if(fun3==6)
				instype= 48;
		else if(fun3==7)
				instype= 49;
	}
	else if(op==103)
		instype= 50;
	else if(op==111)
		instype= 51;


	decode21(instype);
	return instype;
}



void alu1(int instype)                     //alu
{
	int x;
	if(ALU_op==18)
	{
		return;
	}
	else if(ALU_op==1)
		x= RA+RB;
	else if(ALU_op==2)
		x= RA-RB;
	else if(ALU_op==3)
	{
		bool RA_binary[32],RB_binary[32],RZ_binary[32];
		dec2bin(RA_binary,RA);
		dec2bin(RB_binary,RB);
		for(int i=0;i<32;i++)
			if(RA_binary[i]==1&&RB_binary[i]==1)
				RZ_binary[i]=1;
			else
				RZ_binary[i]=0;
		RZ=bin2dec(RZ_binary,32);
		x= RZ;
	}
	else if(ALU_op==4)
	{
		bool RA_binary[32],RB_binary[32],RZ_binary[32];
		dec2bin(RA_binary,RA);
		dec2bin(RB_binary,RB);
		for(int i=0;i<32;i++)
			if(RA_binary[i]==1||RB_binary[i]==1)
				RZ_binary[i]=1;
			else
				RZ_binary[i]=0;
		RZ=bin2dec(RZ_binary,32);
		x= RZ;
	}
	else if(ALU_op==5)
		x= RA*RZ;
	else if(ALU_op==6)
	{

		x= (RA^RB);
	}
	else if(ALU_op==7)
		x= RA/RZ;
	else if(ALU_op==8)
	{
		int k=pow(2,RB);

		RZ=k*RA;
		x= RZ;
	}
	else if(ALU_op==9)
	{
		int k=pow(2,RB);

		RZ=RA/k;
		x= RZ;
	}
	else if(ALU_op==10)
	{
		mux_INC=0;
		if(RA<RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-1);
	}
		x= mux_INC;
	}
	else if(ALU_op==12)
	{
		x=   RA<RB?1:0;
	}
	else if(ALU_op==11)
	{
		mux_INC=0;
		if(RA==RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-1);
	}
		x= 0;
	}
	else if(ALU_op==13)
	{
		x=   RA<abs(RB)?1:0;
	}
	else if(ALU_op==14)
	{
		int k=pow(2,RB);
		RZ=RA/k;
		x= RZ;
	}
	else if(ALU_op==15)
	{
		x= abs(RA)<abs(RB)?1:0;
	}
	else if(ALU_op==16)
	{
		mux_INC=0;
		if(RA!=RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-1);
	}
		x= 0;
	}
	else if(ALU_op==17)
	{
		mux_INC=0;
		if(RA>=RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-1);
	}
		x= mux_INC;
	}
	if(instype==51)
	{
		PC=x;
	}
	else{
		RZ=x;
	}
	if(instype==50)
		{
			PC=RZ/4;
			RZ=PC_temp*4;
		}
}

void memaccess1(int instype)
{
		int tempo=0,modtemp=0,j=0,choice=0,z=0;
		bool temporary[32];
		if(instype>=1&&instype<=7)
			choice=1;
		else if(instype>=24&&instype<=26)
		{
			choice=2;
			z=q;
		}
		if(choice==0)  // introduced this option
        {
            RY=RZ; return;
        }
		if(choice==1)
		{
			if(instype==1)//byte
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<8;i++)
					temporary[i]=memory[tempo][(24-modtemp*8)+i];
				RY=bin2dec(temporary,8,1);
			}
			else if(instype==2)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<16;i++)
				{
					if(modtemp<3)
						temporary[i]=memory[tempo][(16-modtemp*8)+i];
					else
					{
						if(i<8)
							temporary[i]=memory[tempo+1][24+i];
						else
							temporary[i]=memory[tempo][i-8];
					}
				}
				RY=bin2dec(temporary,16,1);
			}
			else if(instype==3)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				if(modtemp==0)
					RY=bin2dec(memory[tempo],32,1);
				else
				{
					for(int i=0;i<32;i++)
					{
						if(i<modtemp*8)
							temporary[i]=memory[tempo+1][(32-modtemp*8)+i];
						else
							temporary[i]=memory[tempo][i-modtemp*8];
					}
					RY=bin2dec(temporary,32,1);
				}
			}
			else if(instype==5)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<8;i++)
					temporary[i]=memory[tempo][(24-modtemp*8)+i];
				RY=bin2dec(temporary,8);
			}
			else if(instype==6)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<16;i++)
				{
					if(modtemp<3)
						temporary[i]=memory[tempo][(16-modtemp*8)+i];
					else
					{
						if(i<8)
							temporary[i]=memory[tempo+1][24+i];
						else
							temporary[i]=memory[tempo][i-8];
					}
				}
				RY=bin2dec(temporary,16);
			}
			else if(instype==7)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				if(modtemp==0)
					RY=bin2dec(memory[tempo],32,1);
				else
				{
					for(int i=0;i<32;i++)
					{
						if(i<modtemp*8)
							temporary[i]=memory[tempo+1][(32-modtemp*8)+i];
						else
							temporary[i]=memory[tempo][i-modtemp*8];
					}
					RY=bin2dec(temporary,32);
				}
			}

		}
		if(choice==2)
		{
			if(instype==24)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				dec2bin(temporary,z,8);
				for(int i=0;i<8;i++)
					memory[tempo][(24-modtemp*8)+i]=temporary[i];

			}
			else if(instype==25)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				dec2bin(temporary,z,16);
				for(int i=0;i<16;i++)
				{
					if(modtemp<3)
						memory[tempo][(16-modtemp*8)+i]=temporary[i];
					else
					{
						if(i<8)
							memory[tempo+1][24+i]=temporary[i];
						else
							memory[tempo][i-8]=temporary[i];
					}
				}

			}
			else if(instype==26)
			{
				tempo=RZ/4;
				modtemp=RZ%4;

				if(modtemp==0)
				{
					dec2bin(temporary,z,32);
					for(int i=0;i<32;i++)
					memory[tempo][i]=temporary[i];
				}
				else
				{
					dec2bin(temporary,z,32);
					for(int i=0;i<32;i++)
					{
						if(i<modtemp*8)
							memory[tempo+1][(32-modtemp*8)+i]=temporary[i];
						else
							memory[tempo][i-modtemp*8]=temporary[i];
					}
				}
			}
		}
}

void update1(int rd_i,int instype)
{
	bool bin_value[32];
	if((instype>=24&&instype<=26)||(instype>=44&&instype<=49))
	return ;
	if(instype>=1&&instype<=7)
    {
        dec2bin(bin_value,RY,32);
        for(int i=0;i<32;i++)
            reg[rd_i][i]=bin_value[i];
        return;
    }
	dec2bin(bin_value,RY,32);  //RZ changed to RY
	for(int i=0;i<32;i++)
	{
		reg[rd_i][i]=bin_value[i];
	}
	cout<<"\n";
	for(int i=0;i<32;i++)
	{
			reg[0][i]=0;
	}
}









void fetch()
{
	for(int i=0;i<32;i++)
		IR[i]=memory[PC][i];

	PC+=1;
}

void decode2(int instype)
{
	if((instype>=28&&instype<=37)||(instype>=39&&instype<=43))					//R type
	{
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rs2[i]=IR[7+i];
			rd[i]=IR[20+i];
		}

		rs1_i=bin2dec(rs1,5);
		rs2_i=bin2dec(rs2,5);
		RA=bin2dec(reg[rs1_i],32,1);
		RB=bin2dec(reg[rs2_i],32,1);
		rd_i=bin2dec(rd,5);
		ALU_op=assign(instype);
		//cout<<"ALU_op="<<ALU_op<<" ";
		type=1;
	}
	else if((instype>=1&&instype<=18)||(instype>=20&&instype<=23)||instype==27||instype==50)		//I TYpe
	{
	    type=2;
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rd[i]=IR[20+i];
		}
		if(instype==16||instype==23)
		{
			for(int i=0;i<5;i++)
			{
			imm[i]=IR[7+i];
			}
			RB=bin2dec(imm,5);
		}
		else
		{
		for(int i=0;i<12;i++)
		{
			imm[i]=IR[i];
		}
			RB=bin2dec(imm,12,1);
		}
		cout<<"\n";
		rs1_i=bin2dec(rs1,5);
		rs2_i=-2;                             //This i have introduced
		rd_i=bin2dec(rd,5);
		RA=bin2dec(reg[rs1_i],32,1);
		ALU_op=assign(instype);
		//cout<<"ALU_op="<<ALU_op<<"\n";
		/*if(instype==50)
		{
			PC=RZ/4;
			RZ=PC_temp*4;
		}*/
	}
	else if((instype>=24&&instype<=26))				//S Type
	{
	    type=3;
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rs2[i]=IR[7+i];
			imm[7+i]=IR[20+i];
		}
		for(int i=0;i<7;i++)
		{
			imm[i]=IR[i];
		}
		rs1_i=bin2dec(rs1,5);
		rs2_i=bin2dec(rs2,5);
		rd_i=-3;
		RA=bin2dec(reg[rs1_i],32,1);
		RB=bin2dec(imm,12,1);
		ALU_op=assign(instype);
		//cout<<"ALU_op="<<ALU_op<<"\n";
		//q=bin2dec(reg[rs2_i],32,1);
		type=bin2dec(reg[rs2_i],32,1);

	}
	else if((instype>=44&&instype<=49))		//SB Type
	{
	    type=4;
		for(int i=0;i<5;i++)
		{
			rs1[i]=IR[12+i];
			rs2[i]=IR[7+i];
			imm[2+i]=IR[1+i];
		}
		imm[0]=IR[0];
		imm[1]=IR[24];
		imm[7]=IR[6];
		imm[12]=0;
		for(int i=0;i<4;i++)
		{
			imm[8+i]=IR[20+i];
		}
		rs1_i=bin2dec(rs1,5);
		rs2_i=bin2dec(rs2,5);
		rd_i=-20;
		RA=bin2dec(reg[rs1_i],32,1);
		RB=bin2dec(reg[rs2_i],32,1);
		if(instype==48||instype==49)
		{
			RA=abs(RA);
			RB=abs(RB);
		}
		cout<<"\n";
		branch_offset=bin2dec(imm,13,1)/4;
		ALU_op=assign(instype);
		//cout<<"ALU_op="<<ALU_op<<endl;
	}
	else if(instype==51)				//UJ type
	{
	    type=5;
		for(int i=0;i<5;i++)
		{
			rd[i]=IR[20+i];
		}
		immu[0]=IR[0];
		for(int i=0;i<8;i++)
			immu[i+1]=IR[12+i];
		immu[9]=IR[11];
		for(int i=0;i<10;i++)
			immu[10+i]=IR[1+i];
		immu[20]=0;
		for(int i=0;i<5;i++)
		{
			rd[i]=IR[20+i];
		}
		rd_i=bin2dec(rd,5);
		RA=PC-1;
		//cout<<"aaaaaaaaaammam"<<endl;
		rs1_i=-4;
        rs2_i=-5;

		RB=bin2dec(immu,21,1)/4;
		ALU_op=assign(instype);
		PC_temp=PC;                cout<<" IN case of jal "<<RB<<"  "<<RA<<" "<<PC_temp<<endl;
		//cout<<"ALU_op="<<ALU_op<<"\n";
		//RZ=PC_temp*4;
	}
	else if(instype==19)
	{
	    type=6;
		ALU_op=assign(instype);
		for(int i=0;i<20;i++)
			imm[i]=IR[i];
		//RZ=bin2dec(imm,20);
		cout<<bin2dec(imm,20)<<endl;
		rs1_i=-50;
		rs2_i=-51;
		for(int i=0;i<5;i++)
			rd[i]=IR[20+i];
		rd_i=bin2dec(rd,5);
	}
}

int decode(bool data[])         //Takes the word as input and gives an integer
{
    int instype;                          //decode
	bool opcode[7],fn3[3],fn7[7];
	int op,fun3,fun7,i;
	for(i=0;i<7;i++)
	{
		fn7[i]=data[i];
		opcode[i]=data[25+i];
	}
	for(i=0;i<3;i++)
		fn3[i]=data[17+i];
	op=bin2dec(opcode,7);
	fun3=bin2dec(fn3,3);
	fun7=bin2dec(fn7,7);
	if(op==3)
		{
			if(fun3==0)
				instype= 1;
			else if(fun3==1)
				instype= 2;
			else if(fun3==2)
				instype= 3;
			else if(fun3==3)
				instype= 4;
			else if(fun3==4)
				instype= 5;
			else if(fun3==5)
				instype= 6;
			else if(fun3==6)
				instype= 7;
		}
	else if(op==19)
	{
			if(fun3==0)
				instype= 10;
			else if(fun3==1&&fun7==0)
				instype= 11;
			else if(fun3==2)
				instype= 12;
			else if(fun3==3)
				instype= 13;
			else if(fun3==4)
				instype= 14;
			else if(fun3==5&&fun7==0)
				instype= 15;
			else if(fun3==5&&fun7==32)
				instype= 16;
			else if(fun3==6)
				instype= 17;
			else if(fun3==7)
				instype= 18;
	}
	else if(op==23)
		instype= 19;
	else if(op==27)
	{
			if(fun3==0)
				instype= 20;
			else if(fun3==1)
				instype= 21;
			else if(fun3==5&&fun7==0)
				instype= 22;
			else if(fun3==5&&fun7==32)
				instype= 23;
	}
	else if(op==35)
	{
			if(fun3==0)
				instype= 24;
			else if(fun3==1)
				instype= 25;
			else if(fun3==2)
				instype= 26;
			else if(fun3==3)
				instype= 27;
	}
	else if(op==51)
	{
			if(fun3==0&&fun7==0)
				instype= 28;
			else if(fun3==0&&fun7==32)
				instype= 29;
			else if(fun3==1)
				instype= 30;
			else if(fun3==2&&fun7==0)
				instype= 31;
			else if(fun3==3&&fun7==0)
				instype= 32;
			else if(fun3==4&&fun7==0)
				instype= 33;
			else if(fun3==5&&fun7==0)
				instype= 34;
			else if(fun3==5&&fun7==32)
				instype= 35;
			else if(fun3==6&&fun7==0)
				instype= 36;
			else if(fun3==7&&fun7==0)
				instype= 37;
	}
	else if(op==55)
	{
		if(fun3==0&&fun7==0)
				instype= 39;
		else if(fun3==0&&fun7==32)
				instype= 40;
		else if(fun3==1&&fun7==0)
				instype= 41;
		else if(fun3==5&&fun7==0)
				instype= 42;
		else if(fun3==5&&fun7==32)
				instype= 43;
		else
			instype= 38;
	}
	else if(op==99)
	{
		if(fun3==0)
				instype= 44;
		else if(fun3==1)
				instype= 45;
		else if(fun3==4)
				instype= 46;
		else if(fun3==5)
				instype= 47;
		else if(fun3==6)
				instype= 48;
		else if(fun3==7)
				instype= 49;
	}
	else if(op==103)
		instype= 50;
	else if(op==111)
		instype= 51;


	decode2(instype);
	return instype;
}



void alu(int instype)                     //alu
{
	int x;
	if(instype==19)
        {RZ=bin2dec(imm,20);
        //cout<<"FCUK "<<RZ<<endl;
        }
	if(ALU_op==18)
	{
		return;
	}
	else if(ALU_op==1)
    {
		x= RA+RB; //cout<<PC<<" JAL "<<x<<" "<<RA<<" "<<RB<<endl;
    if(instype==51)
	{
	    RZ=PC_temp*4;
		PC=x;
		cout<<PC<<" JAL "<<x<<" "<<RA<<" "<<RB<<" PC_TEMP-> "<<PC_temp<<endl;
	}
	else{
		RZ=x;
	    }
	if(instype==50)
		{
			//PC=RZ/4;
			PC=x/4;
			RZ=PC_temp*4;  cout<<PC_temp<<"  "<<"PC -- x "<<PC<<endl;
		}
    }
	else if(ALU_op==2)
		x= RA-RB;
	else if(ALU_op==3)
	{
		bool RA_binary[32],RB_binary[32],RZ_binary[32];
		dec2bin(RA_binary,RA);
		dec2bin(RB_binary,RB);
		for(int i=0;i<32;i++)
			if(RA_binary[i]==1&&RB_binary[i]==1)
				RZ_binary[i]=1;
			else
				RZ_binary[i]=0;
		RZ=bin2dec(RZ_binary,32);
		x= RZ;
	}
	else if(ALU_op==4)
	{
		bool RA_binary[32],RB_binary[32],RZ_binary[32];
		dec2bin(RA_binary,RA);
		dec2bin(RB_binary,RB);
		for(int i=0;i<32;i++)
			if(RA_binary[i]==1||RB_binary[i]==1)
				RZ_binary[i]=1;
			else
				RZ_binary[i]=0;
		RZ=bin2dec(RZ_binary,32);
		x= RZ;
	}
	else if(ALU_op==5)
		x= RA*RZ;
	else if(ALU_op==6)
	{

		x= (RA^RB);
	}
	else if(ALU_op==7)
		x= RA/RZ;
	else if(ALU_op==8)
	{
		int k=pow(2,RB);

		RZ=k*RA;
		x= RZ;
	}
	else if(ALU_op==9)
	{
		int k=pow(2,RB);

		RZ=RA/k;
		x= RZ;
	}
	else if(ALU_op==10)
	{
		mux_INC=0;
		if(RA<RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-2);

	}
		x= mux_INC;
	}
	else if(ALU_op==12)
	{
		x=   RA<RB?1:0;
	}
	else if(ALU_op==11)
	{
		mux_INC=0;  cout<<" BEQ "<<RA<<"  "<<RB<<endl;
		if(RA==RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-2);
	}
		x= 0;
	}
	else if(ALU_op==13)
	{
		x=   RA<abs(RB)?1:0;
	}
	else if(ALU_op==14)
	{
		int k=pow(2,RB);
		RZ=RA/k;
		x= RZ;
	}
	else if(ALU_op==15)
	{
		x= abs(RA)<abs(RB)?1:0;
	}
	else if(ALU_op==16)
	{
		mux_INC=0;
		if(RA!=RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-2);
	}
		x= 0;
	}
	else if(ALU_op==17)
	{
		mux_INC=0;
		if(RA>=RB)
			mux_INC=1;
		else
			mux_INC=0;
		if(mux_INC==1)
	{
		PC+=(branch_offset-2);
	}
		x= mux_INC;
	}

}

void memaccess(int instype)
{
		int tempo=0,modtemp=0,j=0,choice=0,z=0;
		cout<<RZ<<"amanamanamanamanmanamnamanamnamanam    "<<instype<<"   "<<q<<endl;
		bool temporary[32];
		if(instype>=1&&instype<=7)
			choice=1;
		else if(instype>=24&&instype<=26)
		{
			choice=2;
			z=q;
		}
		if(choice==0)  // introduced this option
        {
            RY=RZ; return;
        }
		if(choice==1)
		{
			if(instype==1)//byte
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<8;i++)
					temporary[i]=memory[tempo][(24-modtemp*8)+i];
				RY=bin2dec(temporary,8,1);
			}
			else if(instype==2)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<16;i++)
				{
					if(modtemp<3)
						temporary[i]=memory[tempo][(16-modtemp*8)+i];
					else
					{
						if(i<8)
							temporary[i]=memory[tempo+1][24+i];
						else
							temporary[i]=memory[tempo][i-8];
					}
				}
				RY=bin2dec(temporary,16,1);
			}
			else if(instype==3)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				if(modtemp==0)
					RY=bin2dec(memory[tempo],32,1);
				else
				{
					for(int i=0;i<32;i++)
					{
						if(i<modtemp*8)
							temporary[i]=memory[tempo+1][(32-modtemp*8)+i];
						else
							temporary[i]=memory[tempo][i-modtemp*8];
					}
					RY=bin2dec(temporary,32,1);
				}
			}
			else if(instype==5)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<8;i++)
					temporary[i]=memory[tempo][(24-modtemp*8)+i];
				RY=bin2dec(temporary,8);
			}
			else if(instype==6)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				for(int i=0;i<16;i++)
				{
					if(modtemp<3)
						temporary[i]=memory[tempo][(16-modtemp*8)+i];
					else
					{
						if(i<8)
							temporary[i]=memory[tempo+1][24+i];
						else
							temporary[i]=memory[tempo][i-8];
					}
				}
				RY=bin2dec(temporary,16);
			}
			else if(instype==7)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				if(modtemp==0)
					RY=bin2dec(memory[tempo],32,1);
				else
				{
					for(int i=0;i<32;i++)
					{
						if(i<modtemp*8)
							temporary[i]=memory[tempo+1][(32-modtemp*8)+i];
						else
							temporary[i]=memory[tempo][i-modtemp*8];
					}
					RY=bin2dec(temporary,32);
				}
			}

		}
		if(choice==2)
		{
			if(instype==24)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				dec2bin(temporary,z,8);
				for(int i=0;i<8;i++)
					memory[tempo][(24-modtemp*8)+i]=temporary[i];

			}
			else if(instype==25)
			{
				tempo=RZ/4;
				modtemp=RZ%4;
				dec2bin(temporary,z,16);
				for(int i=0;i<16;i++)
				{
					if(modtemp<3)
						memory[tempo][(16-modtemp*8)+i]=temporary[i];
					else
					{
						if(i<8)
							memory[tempo+1][24+i]=temporary[i];
						else
							memory[tempo][i-8]=temporary[i];
					}
				}

			}
			else if(instype==26)
			{
				tempo=RZ/4;
				modtemp=RZ%4;

				if(modtemp==0)
				{
					dec2bin(temporary,z,32);
					for(int i=0;i<32;i++)
					memory[tempo][i]=temporary[i];
				}
				else
				{
					dec2bin(temporary,z,32);
					for(int i=0;i<32;i++)
					{
						if(i<modtemp*8)
							memory[tempo+1][(32-modtemp*8)+i]=temporary[i];
						else
							memory[tempo][i-modtemp*8]=temporary[i];
					}
				}
			}
		}
}

void update(int rd_i,int instype)
{
    INST_COUNT++;
	bool bin_value[32];
	if((instype>=24&&instype<=26)||(instype>=44&&instype<=49))
	return ;
	if(instype>=1&&instype<=7)
    {
        dec2bin(bin_value,RY,32);
        for(int i=0;i<32;i++)
            reg[rd_i][i]=bin_value[i];
        return;
    }
	dec2bin(bin_value,RY,32);  //RZ changed to RY
	for(int i=0;i<32;i++)
	{
		reg[rd_i][i]=bin_value[i];
	}
	cout<<"\n";
	for(int i=0;i<32;i++)
	{
			reg[0][i]=0;
	}
}





int main()
{
    cout<<" FOR UNPIPELINE PRESS 1 ";
    int unpipeline;
    cin>>unpipeline; int signal;
    if(unpipeline==0)
    {cout<<" ENTER 1 TO CLOSE DATA FORWARDING ";
    cin>>signal;
    for(int j=0;j<5;j++)
    {
        for(int k=0;k<18;k++)
            Reg[j][k]=-1;
    }
    }
    int DEC=0,DEC1=0;
    int D,F,E,M,U,F1=0,D1=0,E1=0,M1=0,U1=0;
    int ENABLE; int step=0; int knob5;
	dec2bin(ti,t);
    int instype;
    int K=0;
    if(unpipeline==0){
    cout<<" ENABLE BY PRESSING 1 PRINTING REGISTER FILE AT EACH INSTRUCTION ";
    cin>>ENABLE;
    cout<<" printing BUFFER REGISTER VALUE AT EACH CYCLE BY PRESSING 1  ";
    cin>>F;
    cout<<" INSTRUCTION NUMBER WHOSE DATA YOU WANT TO PRINT -1 IF YOU DONT WANT ";
    cin>>knob5;}
	read();
	cout<<"\n";
	for(int j=0;j<c;j++)
	{
		for(int k=0;k<32;k++)
			cout<<memory[j][k];
		cout<<endl;
	}
	for(int j=0;j<32;j++)
	{
		for(int k=0;k<32;k++)
			reg[j][k]=0;
	}
		for(int i=0;i<32;i++)
		reg[2][i]=ti[i];

   if(unpipeline==0){
    int chala[4],Nc[4];
    for(int j=0;j<4;j++)
    {
        chala[j]=0; Nc[j]=1;
    }
    int S=0;
    int INST=0;
	while(1)
	{  S++;
	    cout<<"\n\n\n"<<"  START  "<<endl;
		clock_cycle_count++;
		if(chala[3]==1&&Nc[3]==1)
        {
            if(U==1)
                U1=1;
            for(int i=0;i<32;i++)
	       {
			reg[0][i]=0;
           }

            for(int j=0;j<18;j++)
               Reg[4][j]=Reg[3][j];

             cout<<"  UPDATE  "<<Reg[4][0]<<endl;
             if(Reg[3][12]==-10)
                CNT_HAZARD++;
		if(Reg[3][11]==1)
		  {
		     if(Reg[3][12]!=-10)
		      {
		          INST++;
		          if(ENABLE==1)
                    step=1;
		          update(Reg[3][10],Reg[3][12]);
		        if(Reg[3][12]==51||Reg[3][12]==50||(instype>=44&&instype<=49))
                          CONTROL_STMT++;
                if(Reg[3][12]==51||Reg[3][12]==50)
                    CONTROL_STALLS++;
                if((Reg[3][12]>=1&&Reg[3][12]<=7)||(Reg[3][12]>=24&&Reg[3][12]<=26))
                            DATA_TRANSFER++;
                if(INST==knob5)
                {
                    cout<<"\n\n"<<" DATA FOR INSTRUCTION NO. "<<knob5<<endl;
                    cout<<" PC->"<<Reg[3][0]<<endl;
                    cout<<" RA->"<<Reg[3][2]<<endl;
                    cout<<" RB->"<<Reg[3][4]<<endl;
                    cout<<" RZ->"<<Reg[3][13]<<endl;
                    cout<<" RY->"<<Reg[3][15]<<endl;
                    cout<<" \n\n";
                }
		      }
		  }
		}
		if(Nc[3]==0)
		 { break;}

        if(chala[2]==1&&Nc[2]==1)
        {  if(M==1)
            M1=1;
            for(int i=0;i<32;i++)
	        {
			reg[0][i]=0;
	        }
            cout<<"MEMORY"<<endl;
        for(int j=0;j<18;j++)
           Reg[3][j]=Reg[2][j];
        cout<<"  MEMORY  "<<Reg[3][0]<<endl;
        if(Reg[3][8]==Reg[4][10]&&Reg[3][8]!=-10)
        {   DATA_HZD++; DATA_STALLS++;
            q=Reg[4][15]; cout<<"rGH  "<<Reg[3][8]<<endl;
            if(signal==1)
                clock_cycle_count++;
         }
        else
            q=Reg[3][17];
        if(Reg[3][12]!=-10)
         memaccess(Reg[3][12]);
        if(Reg[3][17]!=-10){
        Reg[3][15]=RY; Reg[3][16]=1; RZ=RY; Reg[3][13]=RY;
        }
        chala[3]=1;


        }

        if(Nc[2]==0){
            Nc[3]=0;
            if(PC<c)
                Nc[3]=1;
        }

             // EXECUTE
        if(chala[1]==1&&Nc[1]==1)
        {  cout<<"  EXECUTE  "<<Reg[1][0]<<endl;
        if(E==1)
            E1=1;
            for(int i=0;i<32;i++)
	        {
			reg[0][i]=0;
	        }
            int a=0,b=0,c=0,d=0,e=0,f=0; DEC=0; K=0; DEC1=0;
        //cout<<"EXECUTE"<<endl;
        if(Reg[1][6]==Reg[3][10]&&Reg[1][6]!=0&&Reg[1][6]!=-10)
           {RA=RY; Reg[1][2]=RY; a=1; b=1; c++; cout<<"aman1  RY->"<<RY<<" "<<Reg[1][6]<<endl;
             if((Reg[3][12]>=1&&Reg[3][12]<=7)&&signal==0)
                {clock_cycle_count++; d=1; DATA_STALLS++;}
             if(signal==1)
             {   d=1;
                 clock_cycle_count+=2;
                 DATA_STALLS+=2;
                 e=1;
             }
             DATA_HZD++;
           }
        if(Reg[1][8]==Reg[3][10]&&Reg[1][8]!=0&&Reg[1][8]!=-10)
           {RB=RY; Reg[1][4]=RY; a=1; b=0; c++; cout<<"aman2 RY->  "<<RY<<"  "<<Reg[1][8]<<endl;
             if((Reg[3][12]>=1&&Reg[3][12]<=7)&&signal==0)
                {
                    if(d!=1)
                     {clock_cycle_count++; DATA_STALLS++;}
                 }
            if(signal==1)
             {
                 if(d!=1)
                  {clock_cycle_count+=2; DATA_STALLS+=2;}
                 e=1;
             }
             DATA_HZD++;
           }

        if(Reg[1][6]==Reg[4][10]&&b==0&&c<2&&Reg[1][6]!=0&&Reg[1][6]!=-10)
           {RA=Reg[4][15]; Reg[1][2]=Reg[4][15]; cout<<"aman3 "<<Reg[4][15]<<" ->  "<<Reg[1][6]<<endl;
             if(signal==1)
             {
                 if(e!=1)
                 {clock_cycle_count+=1; f=1; DATA_STALLS++;}
             }
             DATA_HZD++;
           }
        if(Reg[1][8]==Reg[4][10]&&b==1&&c<2&&Reg[1][8]!=0&&Reg[1][8]!=-10)
           {RB=Reg[4][15]; Reg[1][4]=Reg[4][15]; cout<<"aman4 "<<Reg[4][15]<<" ->  "<<Reg[1][8]<<endl;
            if(signal==1)
             {
                 if(e!=1&&f!=1)
                  {clock_cycle_count+=1; DATA_STALLS++;}
             }
             DATA_HZD++;
           }

        if(Reg[1][6]==Reg[2][10]&&a==0&&Reg[1][6]!=0&&Reg[1][6]!=-10)
            {RA=RZ; Reg[1][2]=RZ; cout<<"aman5"<<endl;
             if(a==1)
              clock_cycle_count--;
            }
        else if(Reg[1][8]==Reg[2][10]&&a==0&&Reg[1][8]!=0&&Reg[1][8]!=-10)
             {RB=RZ; Reg[1][4]=RZ; cout<<"aman6"<<endl;
             if(a==1)
              clock_cycle_count--;
             }
        if(Reg[1][17]!=-10)
           {alu(Reg[1][12]); cout<<"  ALU exexuted "<<endl;}

        if(Reg[1][12]>=44&&Reg[1][12]<=49)
        {
            if(mux_INC==1)
                {DEC=1; //clock_cycle_count+=2;
                 K=1;
                 clock_cycle_count++;
                 MIS_PREDICTION++;
                 CONTROL_STALLS+=2;
                }
            cout<<endl;
            cout<<"GOes"<<endl;
        }
        if(Reg[1][17]!=-10)
         {
             for(int j=0;j<18;j++)
              Reg[2][j]=Reg[1][j];
         }
        else
        {
          for(int j=0;j<18;j++)
              Reg[2][j]=-10;
        }

        if(Reg[1][17]!=-10)
        {
            Reg[2][13]=RZ; Reg[2][14]=1; //Reg[2][0]=PC; Reg[2][1]=1;
        }
        chala[2]=1;


        }
        if(Nc[1]==0){
            Nc[2]=0;
            if(PC<c)
                Nc[2]=1;
        }


          // DECODE
        if(chala[0]==1&&Nc[0]==1)
        { DEC1=0;
         if(D==1)
            D1=1;
        cout<<"  DECODE  "<<Reg[0][0]<<endl;
          for(int i=0;i<32;i++)
	        {
			reg[0][i]=0;
	        }

         for(int j=0;j<18;j++)
           Reg[1][j]=Reg[0][j];
        if(1)
         instype=decode(IR);
        if((instype==51||instype==50)&&K==0&&Reg[1][17]!=-10)
            DEC1=1;
        if(DEC==0&&Reg[1][17]!=-10){
                cout<<PC<<" "<<"DECODE EXECuted "<<endl;
        Reg[1][2]=RA; Reg[1][3]=1; Reg[1][4]=RB; Reg[1][5]=1; Reg[1][6]=rs1_i; Reg[1][7]=1; Reg[1][8]=rs2_i; Reg[1][9]=1; Reg[1][10]=rd_i; Reg[1][11]=1; Reg[1][17]=type;
        Reg[1][12]=instype;}
        else
        {
           for(int j=0;j<18;j++)
             Reg[1][j]=-10;
        }
        chala[1]=1;

        //cout<<"DECODE"<<endl;
        }

        if(Nc[0]==0){
            Nc[1]=0;
                if(PC<c)
                 {
                    Nc[1]=1;
                 }
         for(int j=0;j<18;j++)
            Reg[1][j]=-10;
        }

        if(PC<c){
                if(F==1)
                  F1=1;

                Nc[0]=1;
                for(int i=0;i<32;i++)
	            {
			     reg[0][i]=0;
	            }
		fetch();
		Reg[0][0]=PC-1; Reg[0][1]=1;
		chala[0]=1;
		cout<<"  FETCH  "<<Reg[0][0]<<endl;
		for(int j=2;j<18;j++)
            Reg[0][j]=-1;
		  if(DEC1==1)
          {  cout<<"NOT"<<endl;
            for(int j=0;j<18;j++)
             Reg[0][j]=-10;
          }
        }
        else{
                for(int j=0;j<18;j++)
                   Reg[0][j]=-10;
         Nc[0]=0;
        }

      cout<<PC<<" "<<c<<endl;

      if(step==1)
      {
          step=0;ENABLE=0;
          for(int i=0;i<32;i++)
	   {
		cout<<"reg"<<i<<"=";
		for(int j=0;j<32;j++)
		{
			cout<<reg[i][j];
		}
		cout<<"\n";
	  }
	  cout<<" RUN BY 0 AND STEP BY 1  ";
	  cin>>ENABLE;

      }
      if(F1==1)
      {
          F=0; F1=0;
          cout<<"\n\n";
          cout<<" PC -> "<<PC-1<<endl;
          cout<<" FOR VALUE OF ANOTHER CYCLE 1 ELSE RUN WITH 0 ";
          cin>>D;
      }
      if(D1==1)
      {
          D=0; D1=0;
          cout<<"\n\n";
          cout<<" PC -> "<<PC-1<<endl;
          cout<<" RA -> "<<RA<<" RB -> "<<RB<<endl;
          cout<<" FOR VALUE OF ANOTHER CYCLE 1 ELSE RUN WITH 0 ";
          cin>>E;
      }
      if(E1==1)
      {
          E=0; E1=0;
          cout<<"\n\n";
          cout<<" PC -> "<<PC-1<<endl;
          cout<<" RA -> "<<RA<<" RB -> "<<RB<<endl;
          cout<<" RZ -> "<<RZ<<endl;
          cout<<" FOR VALUE OF ANOTHER CYCLE 1 ELSE RUN WITH 0 ";
          cin>>M;
      }
      if(M1==1)
      {
          M=0; M1=0;
          cout<<"\n\n";
          cout<<" PC -> "<<PC-1<<endl;
          cout<<" RA -> "<<RA<<" RB -> "<<RB<<endl;
          cout<<" RZ -> "<<RZ<<endl;
          cout<<" RY -> "<<RY<<endl;
          cout<<" FOR VALUE OF ANOTHER CYCLE 1 ELSE RUN WITH 0 ";
          cin>>U;
      }
      if(U1==1)
      {
          U=0; U1=0;
          cout<<"\n\n";
          cout<<" PC -> "<<PC-1<<endl;
          cout<<" RA -> "<<RA<<" RB -> "<<RB<<endl;
          cout<<" RZ -> "<<RZ<<endl;
          cout<<" RY -> "<<RY<<endl;
          cout<<" FOR VALUE OF ANOTHER CYCLE 1 ELSE RUN WITH 0 ";
          cin>>U;
      }

     }
   }
   else
   {
       while(PC<c)
	{
		clock_cycle_count++;
		fetch1();
		instype=decode1(IR);
		alu1(instype);
		memaccess1(instype);
		update1(rd_i,instype);
     }
   }

     /*cout<<"instype="<<instype<<"\n";
		cout<<"ALU_op="<<ALU_op<<"\n";*/
   for(int i=0;i<32;i++)
	{
		cout<<"reg"<<i<<"=";
		for(int j=0;j<32;j++)
		{
			cout<<reg[i][j];
		}
		cout<<"\n";
	}

	 ofstream fout1;
  fout1.open("MEM1.txt");
  for(int i=1000;i<1500;i++)
  {
      for(int j=0;j<32;j++)
        fout1<<memory[i][j];
      fout1<<endl;
  }
  fout1.close();
   ofstream fout2;
  fout2.open("MEM2.txt");
  for(int i=1999;i>=1500;i--)
  {
      for(int j=0;j<32;j++)
        fout2<<memory[i][j];
      fout2<<endl;
  }
  fout2.close();
  cout<<"Cycle_count="<<clock_cycle_count-1<<"\n";
  cout<<" INSTRUCTION COUNT "<<INST_COUNT<<endl;
  cout<<" MIS PREDICTION "<<MIS_PREDICTION<<endl;
  cout<<" CONTROL INSTRUCTION "<<CONTROL_STMT<<endl;
  cout<<" DATA HAZARD "<<DATA_HZD<<endl;
  cout<<" CONTROL HAZARD "<<CNT_HAZARD<<endl;
  cout<<" DATA STALLS "<<DATA_STALLS<<endl;
  cout<<" CONTROL STALLS "<<CONTROL_STALLS<<endl;
  cout<<" DATA TRANSFER INSTRUCTION "<<DATA_TRANSFER<<endl;
}
