#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include<bits/stdc++.h>
#include <math.h>

using namespace std;
bool MEMORY[2000][32]={0};

struct Data
{
string name;
int address;
int size;

} ;



fun1()
{
     Data   var_store[100];
     int order=0;
     string  line ;
     ofstream fout;
  fout.open("a_c_.txt");
    ifstream  code ;
    code.open("assmebly_code.txt") ;
    int index=4000;
    int data_part =0;
    int text_part=0;
  while(code)
  {
     getline(code, line);
     if(!code)
       break;
     if(line.length()==0)
        continue;

            int l=line.length();
            int k=0;
            for(int j=0;j<l;j++)
                if(line[j]==' ')
                    k++;
            if(k==l)
                continue;
            for(int i=l;i>=0;i--)
               if(line[i]=='#')
                  line.resize (i);
            //cout<<l<<"  "<<line<<"  ";
            l=line.length();
            for(int i=0;i<l;i++)
             if(line[i]==',')
             line[i]=' ';
    //cout<<data_part<<endl;
     string word;
     stringstream  iss(line) ;
     iss>>word ;
     if(word == ".data")
     {
         data_part =1;
     }
   else if (word == ".text")
   {
        text_part=1;
        data_part=0;
   }
   else if(data_part==1)
   {
       int l=word.length();
    if(word[l-1]==':')
      word.resize(word.size()-1);
    var_store[order].name= word;  //cout<<word<<endl;//name of variable is stored

    string word2;
    iss>>word2;
    if(word2==".word")
     {var_store[order].size=4;
      var_store[order].address=index;
      string word3;
      while(iss>>word3){
        int row=index/4; int rem=index%4;  //cout<<row<<"  "<<rem<<endl;
         index=index+4;

         int value;

         stringstream geek(word3);
         geek>>value;                       //cout<<value<<endl;

         bitset<32> bset2(value);  //cout<<bset2<<endl;
         if(rem==0)
         {
             for(int j=0;j<32;j++)
                MEMORY[row][31-j]=bset2[j];
         }
         else if(rem==1)
         {
             for(int i=0;i<24;i++)
                MEMORY[row][23-i]=bset2[i];
             int j=0;
             for(int i=24;i<32;i++)
             {MEMORY[row+1][31-j]=bset2[i]; j++;}
         }
         else if(rem==2)
         {
             for(int i=0;i<16;i++)
                MEMORY[row][15-i]=bset2[i];
             int j=0;
             for(int i=16;i<32;i++)
             {MEMORY[row+1][31-j]=bset2[i]; j++;}
         }
         else if(rem==3)
         {
             for(int i=0;i<8;i++)
                MEMORY[row][7-i]=bset2[i];
             int j=0;
             for(int i=8;i<32;i++)
             {MEMORY[row+1][31-j]=bset2[i]; j++;}
         }
      }
     }
    else if(word2==".byte")
    {
       var_store[order].size=1;


         string word3;
         var_store[order].address=index;
         int value;
         while(iss>>word3){
         int row=index/4; int rem=index%4;
         //iss>>word3;
         stringstream geek(word3);
         geek>>value;             //cout<<value<<endl;
         bitset<8> bset2(value);  //cout<<bset2<<endl;
          int k=4-rem;
          k=k*8; k=k-1;
            for(int i=0;i<8;i++)
              MEMORY[row][k-i]=bset2[i];
         index=index+1;
         }

         //index=index+1;
    }

    order++;

   }
   else if (text_part==1 || data_part==0)
   {
        if(word=="lw" || word=="lb" )
        {
            string word3; string word4;
            iss>>word3;
            iss>>word4;
            int i;
            for(i=0;i<order;i++)
            {
                if(word4==var_store[i].name)
                {
                    fout<<"auipc"<<" "<<word3<<" "<<var_store[i].address<<endl;
                    fout<<word<<" "<<word3<<" "<<"0("<<word3<<")"<<endl;
                    break;
                }
            }
            if(i==order)
            {
                int l=line.length();
              for(int i=l;i>0;i--)
               if(line[i]=='#')
                  line.resize (i);
            cout<<l<<"  "<<line<<"  ";
            l=line.length();
            for(int i=0;i<l;i++)
             if(line[i]==',')
             line[i]=' ';
          cout<<l<<"  "<<line<<endl;
          fout<<line<<endl;
            }

        }
        else if(word=="la")
        {
            string word3; string word4;
            iss>>word3;
            iss>>word4;
            int i;
            for(i=0;i<order;i++)
            {
                if(word4==var_store[i].name)
                {
                    fout<<"auipc"<<" "<<word3<<" "<<var_store[i].address<<endl;
                    break;
                }
            }
        }
        else if(line.length()!=0)
        {
            int l=line.length();
        for(int i=l;i>0;i--)
            if(line[i]=='#')
             line.resize (i);
            cout<<l<<"  "<<line<<"  ";
        l=line.length();
        for(int i=0;i<l;i++)
            if(line[i]==',')
             line[i]=' ';
          cout<<l<<"  "<<line<<endl;
            fout<<line<<endl;
        }

   }
  }

  ofstream fout1;
  fout1.open("MEM.txt");
  for(int i=1000;i<1500;i++)
  {
      for(int j=0;j<32;j++)
        fout1<<MEMORY[i][j];
      fout1<<endl;
  }
  fout1.close();

}

string   bin_2_hex( bool   input[32] )

{
 //cout<<" in function "<<endl;
string hex;
int count=0;

while(count<8)
{
int temp=0;
int  mul=8;
for(int i=4*count ;i<(4*count+4);i++)
{
   temp+=input[i]*mul;
   mul/=2;
}

if(temp<10)
{
    // cout<<temp<<endl;
     char c=temp+48;
    // cout<<c<<endl;

       hex.push_back(c) ;
}
else
{
  //cout<<temp<<endl;
     char c=temp+55;
    // cout<<c<<endl;

       hex.push_back(c) ;
}
//cout<<" in function a "<<endl;
count++;

}


return    hex ;
}


int main()
{
  fun1();
  vector<string> v;
  vector<string> v1;
  bool Mcode[5000][32]={0};
  string  line ;
  ifstream  code ;
  code.open("a_c_.txt") ;
  //vector<string> v1;
  while(code)    // this while loop contains two vector one conatin all first word and other conatain all word without ':' ;
  {
     getline(code, line);
     if(!code)
       break;
     if(line.length()==0)
        continue;
     string word;
     stringstream  iss(line) ;
     iss>>word ;
     v1.push_back(word);
     int l=word.length();
     if(word[l-1]==':') //here i am checking whether after : some statement is not
       {
         //word.pop_back();
         word.resize (word.size () - 1);
       }
     v.push_back(word);
  }
  code.close();
  code.open("a_c_.txt") ;
  ofstream fout;
  fout.open("machine_code.mc");
  int line_no=0;
  int jump_line=0;
  while(code)   // this code does all main work
  {
     getline(code, line);
     if(!code)
       break;
    string parent=line;
    if(parent.length()==0)
        continue;
    string word;
    stringstream  iss(line) ;
    iss>>word ;  // extracting first word that is operation
    cout<<word<<endl;
    int l=word.length();
    if(word[l-1]==':')
       {jump_line++;continue;}

         /* now writing code for R type instruction */
    if(word=="auipc")
    {
        string word2; string word3;
        iss>>word2;
        int l=word2.length();
        int reg=0;
        for(int j=0;j<l;j++)
        {
            if(word2[j]>=48&&word2[j]<=57)
              reg=reg*10+(word2[j]-48);
        }
        cout<<reg<<" ";
        bitset<5> bset2(reg);
        for(int i=0;i<5;i++)
            Mcode[line_no][24-i]=bset2[i];
        iss>>word3;
        bool opcode[7]={0,0,1,0,1,1,1};
        int value;
        stringstream geek(word3);
         geek>>value;
         cout<<value<<endl;
        bitset<20> bset(value);
        int z=25;
        for(int i=0;i<7;i++)
            {Mcode[line_no][z]=opcode[i]; z++;}
        for(int i=0;i<20;i++)
            Mcode[line_no][19-i]=bset[i];

              // fout<<line_no<<"   ";
               for(int j=0;j<32;j++)
                 {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                 }
                 string    hex;
                 hex= bin_2_hex( Mcode[line_no] );
                 fout<<hex;
                 fout<<"\n";
                cout<<endl;

    }

    if(word =="add" || word =="sub" || word =="sll" || word =="slt" || word =="sltu" || word =="xor" || word =="srl" || word =="sra" ||word =="or" ||word =="and" )
    {  int   rd,rs1,rs2,count=0 , charcount=0;
                                                                                        //contains "R type instruction found"<<endl;
       for(int i=0;i<parent.length() ;i++)
       {
           int   character=parent[i] ;
           //cout<<character<<endl;
           int  charnext ;
            if ( character >=48 && character<=57 )
                 {       charcount++;
                        // cout<<character<<endl;

                         if(i+1<parent.length())
                         {
                              charnext =parent[i+1];

                               if( (charnext >=48 && charnext<=57) )
                                    charcount++;
                            i++;
                         }
                          count++;
                 }

                 character=character-48;
                 charnext=charnext-48;

                   // cout<<charcount<<" "<<count<<" "<<character<<" "<<charnext<<endl;


                              if(    charcount==2 && count==1 )
                                    rd=charnext+10*character ;
                              else  if(  charcount==2  && count==2 )
                                    rs1=charnext+10*character ;
                              else   if( charcount==2 && count==3 )
                                    rs2=charnext+10*character ;
                               else  if (charcount==1 && count==1 )
                                    rd=character ;
                               else if ( charcount==1 && count==2 )
                                    rs1=character ;
                               else if ( charcount==1 && count==3 )
                                    rs2=character ;



                      charcount=0;



      }

       ifstream fin;
       fin.open("R.txt");
       string line1;
       while(fin)             //from here i have started making machine code
       {
         getline(fin, line1);
           if(!fin)
             break;
       // cout<<line1<<endl;
           istringstream ss(line1);
           int k=0,count=0;

              string label;
                do {
                        ss >> label;
                        //cout<<label<<" ";
                        if(label==word)
                        {
                          k=1;count++; //cout<<"A"<<endl;
                        }
                        else if(k==1&&count==2)
                        {  int y=25;
                        //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==3)
                        {
                          int y=17;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==4)
                        {
                          int y=0;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else
                          break;
                    count++;

                    } while (ss);


       } fin.close();
       cout<<rd<<" "<<rs1<<" "<<rs2<<" "<<endl;

                int y=20;
                bool bin[5]={0};
                int m = 4;
                while (rd > 0) {
                    bin[m] = (rd % 2);
                      rd = rd / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=12;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs1 > 0) {
                    bin[m] = (rs1 % 2);
                      rs1 = rs1 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=7;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs2 > 0) {
                    bin[m] = (rs2 % 2);
                      rs2 = rs2 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }

                  // cout<<parent[i]<<endl;
         //cout<<dec2bin(rs1)<<endl;

         //fout<<line_no<<"   ";
         for(int j=0;j<32;j++)
            {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
            }
        string    hex;
        hex= bin_2_hex( Mcode[line_no] );
        fout<<hex;
            fout<<"\n";
         cout<<endl;
       }   // upto here R type work has been done



           if(word=="li")  // for I type instructions
           {
               int rd ,immediate=0,count=0 , charcount=0;
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    charcount++;
                                 i++;
                              }
                                count++;
                           }

                              character=character-48;
                              charnext=charnext-48;

                              if(    charcount==2 && count==1 )
                                    rd=charnext+10*character ;
                              else  if( charcount==2&&count>=2 )
                                    immediate=immediate*100+charnext+10*character ;
                              else  if (charcount==1 && count==1 )
                                    rd=character ;
                               else if ( charcount==1 && count>=2 )
                                    immediate=immediate*10+character ;

                        charcount=0;
                }
                cout<<rd<<" "<<immediate<<endl;
                if(immediate>=2048)
                {
                    //divide it into two parts.   lui and addi
                }

           }

           if(word=="addi"||word=="xori"||word=="ori"||word=="slti"||word=="sltiu"||word=="andi")
           {
               int rd ,rs1,immediate=0,count=0 , charcount=0;
               istringstream ss(line);
              string labelnew;
              string second;
              int x=0;
                do {
                        x++;
                        ss >> labelnew;
                        if(x==2)
                         second=labelnew;

                   }while(ss);
                   cout<<second<<endl;
                   stringstream geek(labelnew);
                    geek>>immediate;
                if(second=="sp")
                {
                    rd=2; rs1=2;

                }
                else{
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    charcount++;
                                 i++;
                              }
                                count++;
                           }

                              character=character-48;
                              charnext=charnext-48;

                              if(    charcount==2 && count==1 )
                                    rd=charnext+10*character ;
                              else  if(  charcount==2  && count==2 )
                                    rs1=charnext+10*character ;
                              /*else  if( charcount==2&&count>=3 )
                                    immediate=immediate*100+charnext+10*character ;*/
                              else  if (charcount==1 && count==1 )
                                    rd=character ;
                              else if ( charcount==1 && count==2 )
                                    rs1=character ;
                               /*else if ( charcount==1 && count>=3 )
                                    immediate=immediate*10+character ;*/

                        charcount=0;
                }
                }
                cout<<rd<<" "<<rs1<<" "<<immediate<<endl;



           ifstream fin;
       fin.open("I.txt");
       string line1;
       while(fin)
       {
         getline(fin, line1);
           if(!fin)
             break;
       // cout<<line1<<endl;
           istringstream ss(line1);
           int k=0,count=0;

              string label;
                do {
                        ss >> label;
                        //cout<<label<<" ";
                        if(label==word)
                        {
                          k=1;count++; //cout<<"A"<<endl;
                        }
                        else if(k==1&&count==2)
                        {  int y=25;
                        //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==3)
                        {
                          int y=17;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else
                          break;
                    count++;

                    } while (ss);


       } fin.close();

                int y=20;
                bool bin[5]={0};
                int m = 4;
                while (rd > 0) {
                    bin[m] = (rd % 2);
                      rd = rd / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=12;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs1 > 0) {
                    bin[m] = (rs1 % 2);
                      rs1 = rs1 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=0;
                bool binImm[12]={0};
                bitset<12> bset2(immediate);
                for(int i=0;i<12;i++)
                   binImm[11-i]=bset2[i];
                /*m = 11;
                while (immediate > 0) {
                    binImm[m] = (immediate % 2);
                      immediate = immediate / 2;
                       m--;
                   }*/
                for(int i=0;i<12;i++)
                {
                    Mcode[line_no][y]=binImm[i];
                    y++;
                }

                 //fout<<line_no<<"  ";
                    for(int j=0;j<32;j++)
                      {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                      }
                      string    hex;
                      hex= bin_2_hex( Mcode[line_no] );
                      fout<<hex;
                      fout<<"\n";
         cout<<endl;
           }

           if(word=="slli"||word=="srli"||word=="srai"||word=="slliw"||word=="srliw"||word=="sraiw")
           {
             int rd ,rs1,immediate=0,count=0 , charcount=0;
               istringstream ss(line);
              string labelnew;
              string second;
              int x=0;
                do {
                        x++;
                        ss >> labelnew;
                        if(x==2)
                         second=labelnew;

                   }while(ss);
                   cout<<second<<endl;
                   stringstream geek(labelnew);
                    geek>>immediate;
                if(second=="sp")
                {
                    rd=2; rs1=2;

                }
                else{
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    charcount++;
                                 i++;
                              }
                                count++;
                           }

                              character=character-48;
                              charnext=charnext-48;

                              if(    charcount==2 && count==1 )
                                    rd=charnext+10*character ;
                              else  if(  charcount==2  && count==2 )
                                    rs1=charnext+10*character ;
                              else  if (charcount==1 && count==1 )
                                    rd=character ;
                              else if ( charcount==1 && count==2 )
                                    rs1=character ;

                        charcount=0;
                }
                }
                cout<<rd<<" "<<rs1<<" "<<immediate<<endl;

                ifstream fin;
       fin.open("I.txt");
       string line1;
       while(fin)             //from here i have started making machine code
       {
         getline(fin, line1);
           if(!fin)
             break;
       // cout<<line1<<endl;
           istringstream ss(line1);
           int k=0,count=0;

              string label;
                do {
                        ss >> label;
                        //cout<<label<<" ";
                        if(label==word)
                        {
                          k=1;count++; //cout<<"A"<<endl;
                        }
                        else if(k==1&&count==2)
                        {  int y=25;
                        //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==3)
                        {
                          int y=17;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==4)
                        {
                          int y=0;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else
                          break;
                    count++;

                    } while (ss);


                    } fin.close();

                    int y=20;
                bool bin[5]={0};
                int m = 4;
                while (rd > 0) {
                    bin[m] = (rd % 2);
                      rd = rd / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=12;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs1 > 0) {
                    bin[m] = (rs1 % 2);
                      rs1 = rs1 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=7;
                bool binImm[5]={0};
                bitset<5> bset2(immediate);
                for(int i=0;i<5;i++)
                   binImm[4-i]=bset2[i];

                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=binImm[i];
                    y++;
                }

                 //fout<<line_no<<"  ";
                    for(int j=0;j<32;j++)
                      {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                      }
                      string    hex;
                      hex= bin_2_hex( Mcode[line_no] );
                      fout<<hex;
                      fout<<"\n";
         cout<<endl;
           }

           if(word=="lw"||word=="jalr"||word=="lb"||word=="lh"||word=="ld"||word=="lbu"||word=="lhu"||word=="lwu"||word=="sd")
           {
               int rd ,rs1,immediate=0,count=0 , charcount=0,a=0;
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    {charcount++;}
                                 i++;
                              }
                                count++;
                           }



                              character=character-48;
                              charnext=charnext-48;

                              if(    charcount==2 && count==1 )
                                    rd=charnext+10*character ;
                              else  if(  charcount==2  && count>=3  &&a==2)
                                    rs1=charnext+10*character ;
                              else  if( charcount==2&&count>=2 &&a==0 )
                                    immediate=immediate*100+charnext+10*character ;
                              else  if (charcount==1 && count==1 )
                                    rd=character ;
                              else if ( charcount==1 && count>=3 &&a==2)
                                    rs1=character ;
                               else if ( charcount==1 && count>=2 &&a==0 )
                                    immediate=immediate*10+character ;

                        charcount=0;
                        charnext=charnext+48;
                        character=character+48;
                        if(charnext=='('||character=='(')
                                    {
                                      a=2;
                                    }

                }
              istringstream ss(line);
              string labelnew;
                do {
                        ss >> labelnew;

                   }while(ss);
                   cout<<labelnew<<endl;
                   int k=0;
                   immediate=0;
                   int neg=0;
                   while(labelnew[k]!='(')
                           {
                            if(labelnew[k]=='-')
                            {neg=1; k++; continue;}
                            immediate=immediate*10+(labelnew[k]-48);
                            k++;
                           }
                    if(labelnew[k+1]=='s'&&labelnew[k+2]=='p')
                        rs1=2;
                    if(neg==1)
                        immediate=-immediate;

                cout<<rd<<" "<<rs1<<" "<<immediate<<endl;

           ifstream fin;
       fin.open("I.txt");
       string line1;
       while(fin)
       {
         getline(fin, line1);
           if(!fin)
             break;
       // cout<<line1<<endl;
           istringstream ss(line1);
           int k=0,count=0;

              string label;
                do {
                        ss >> label;
                        //cout<<label<<" ";
                        if(label==word)
                        {
                          k=1;count++; //cout<<"A"<<endl;
                        }
                        else if(k==1&&count==2)
                        {  int y=25;
                        //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==3)
                        {
                          int y=17;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else
                          break;
                    count++;

                    } while (ss);


       } fin.close();

                int y=20;
                bool bin[5]={0};
                int m = 4;
                while (rd > 0) {
                    bin[m] = (rd % 2);
                      rd = rd / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=12;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs1 > 0) {
                    bin[m] = (rs1 % 2);
                      rs1 = rs1 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=0;
                bitset<12> bset2(immediate);
                bool binImm[12]={0};
                for(int i=0;i<12;i++)
                 binImm[11-i]=bset2[i];
                /*m = 11;
                while (immediate > 0) {
                    binImm[m] = (immediate % 2);
                      immediate = immediate / 2;
                       m--;
                   }*/
                for(int i=0;i<12;i++)
                {
                    Mcode[line_no][y]=binImm[i];
                    y++;
                }

                  //fout<<line_no<<"  ";
                    for(int j=0;j<32;j++)
                      {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                      }
                      string    hex;
                      hex= bin_2_hex( Mcode[line_no] );
                      fout<<hex;
                      fout<<"\n";
         cout<<endl;
           }




       if(word=="jal")
       {
           int rd ,immediate=0,count=0 , charcount=0;
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    charcount++;
                                 i++;
                              }
                                count++;
                           }

                              character=character-48;
                              charnext=charnext-48;

                         if(  charcount==2 && count==1 )
                               {
                                   rd=charnext+10*character ;
                                   break;
                               }
                         else  if (charcount==1 && count==1 )
                                   {rd=character ; break;}

                    charcount=0;
                }
                istringstream ss(line);
                string label;
                do {
                        ss >> label;

                    } while (ss);
                //cout<<rd<<" "<<label<<endl;
            vector<string>::iterator ptr;
            for(ptr=v.begin();ptr<v.end();ptr++)
            {
                if((*ptr)==label)
                    break;
                   // cout<<*ptr<<"  "<<label<<endl;
                immediate++;
            }
           cout<<immediate<<" "<<line_no<<" "<<jump_line<<endl;
           int yz=0;
           if(immediate>jump_line)
           {
                  for(int k=jump_line+1;k<immediate;k++)
                   {
                       int l=v1[k].length();
                       int c=v1[k][l-1];
                        if(c==':')
                            yz++;
                   }
                immediate=(immediate-jump_line-yz)*4;
           }
           else
           {
               for(int k=immediate+1;k<jump_line;k++)
               {
                   int l=v1[k].length();
                       int c=v1[k][l-1];
                        if(c==':')
                            yz++;
               }
               immediate=(jump_line-yz-immediate-1)*4;
               immediate=-immediate;

           }

           cout<<rd<<" "<<immediate<<endl;
           //immediate=(immediate-jump_line)*4;
           bool opcode[7]={1,1,0,1,1,1,1};
           int y=25;
           for(int i=0;i<7;i++)
           {
               Mcode[line_no][y]=opcode[i];
               y++;
           }
                y=20;
                bool bin[5]={0};
                int m = 4;
                while (rd > 0) {
                    bin[m] = (rd % 2);
                      rd = rd / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                bool binImm[21]={0};
                bitset<21> bset2(immediate);
                for(int i=0;i<21;i++)
                  binImm[20-i]=bset2[i];
                /*m = 20;
                while (immediate > 0) {
                    binImm[m] = (immediate % 2);
                      immediate = immediate / 2;
                       m--;
                   }*/
                   y=19;
             for(int i=8;i>=1;i--)
             {
               Mcode[line_no][y]=binImm[i];
                    y--;
             }
             Mcode[line_no][11]=binImm[9];
             y=10;
             for(int i=19;i>=10;i--)
             {
               Mcode[line_no][y]=binImm[i];
                    y--;
             }
             Mcode[line_no][0]=binImm[0];

              //fout<<line_no<<"  ";
             for(int j=0;j<32;j++)
                {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                }
                string    hex;
                hex= bin_2_hex( Mcode[line_no] );
                fout<<hex;
                fout<<"\n";
             cout<<endl;

       }

       if(word=="beq"||word=="bge"||word=="bne"||word=="blt"||word=="bltu"||word=="bgeu")
       {
          int rs2 ,rs1,immediate=0,count=0 , charcount=0;
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    charcount++;
                                 i++;
                              }
                                count++;
                           }

                              character=character-48;
                              charnext=charnext-48;

                         if(  charcount==2 && count==1 )
                               {
                                   rs1=charnext+10*character ;
                               }
                        else  if(  charcount==2  && count==2 )
                                    rs2=charnext+10*character ;
                         else  if (charcount==1 && count==1 )
                                   {rs1=character ; }
                        else if ( charcount==1 && count==2 )
                                    rs2=character ;

                    charcount=0;
                }
                istringstream ss(line);
                string label;
                do {
                        ss >> label;

                    } while (ss);
                //cout<<rs1<<" "<<rs2<<" "<<label<<endl;
                vector<string>::iterator ptr1;
            for(ptr1=v.begin();ptr1<v.end();ptr1++)
            {
                if((*ptr1)==label)
                    break;
                    //cout<<*ptr1<<"  "<<label<<endl;
                immediate++;
            }
           cout<<immediate<<" "<<line_no<<" "<<jump_line<<endl;
           int yz=0;
           if(immediate>jump_line)
           {
                  for(int k=jump_line+1;k<immediate;k++)
                   {
                       int l=v1[k].length();
                       int c=v1[k][l-1];
                        if(c==':')
                            yz++;
                   }
                immediate=(immediate-jump_line-yz)*4;
           }
           else
           {
               for(int k=immediate+1;k<jump_line;k++)
               {
                   int l=v1[k].length();
                       int c=v1[k][l-1];
                        if(c==':')
                            yz++;
               }
               immediate=(jump_line-yz-immediate-1)*4;
               immediate=-immediate;

           }
           cout<<rs1<<" "<<rs2<<" "<<immediate<<endl;
           //immediate=(immediate-jump_line)*4;
           ifstream fin;
       fin.open("SB.txt");
       string line1;
       while(fin)
       {
         getline(fin, line1);
           if(!fin)
             break;
       // cout<<line1<<endl;
           istringstream ss(line1);
           int k=0,count=0;

              string label;
                do {
                        ss >> label;
                        //cout<<label<<" ";
                        if(label==word)
                        {
                          k=1;count++; //cout<<"A"<<endl;
                        }
                        else if(k==1&&count==2)
                        {  int y=25;
                        //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==3)
                        {
                          int y=17;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else
                          break;
                    count++;

                    } while (ss);


       }

                int y=12;
                bool bin[5]={0};
                int m = 4;
                while (rs1 > 0) {
                    bin[m] = (rs1 % 2);
                      rs1 = rs1 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }

                y=7;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs2 > 0) {
                    bin[m] = (rs2 % 2);
                      rs2 = rs2 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }

                bitset<13> bset2(immediate);
                bool binImm[13]={0};
                for(int i=0;i<13;i++)
                    binImm[12-i]=bset2[i];
                /*m = 12;
                while (immediate > 0) {
                    binImm[m] = (immediate % 2);
                      immediate = immediate / 2;
                       m--;
                   }*/


                 Mcode[line_no][24]=binImm[1];
                 Mcode[line_no][0]=binImm[0];
                 y=23;
                 for(int i=11;i>=8;i--)
                 {
                      Mcode[line_no][y]=binImm[i];
                      y--;
                 }
                 y=6;
                 for(int i=7;i>=2;i--)
                 {
                      Mcode[line_no][y]=binImm[i];
                      y--;
                 }

                //fout<<line_no<<"  ";
                for(int j=0;j<32;j++)
                    {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                    }
                    string    hex;
                    hex= bin_2_hex( Mcode[line_no] );
                    fout<<hex<<endl;
                    //fout<<"\n";
                cout<<endl;

       }


       if(word=="sw"||word=="sb"||word=="sh")
           {
               int rs1 ,rs2=0,immediate=0,count=0 , charcount=0,a=0;
               for(int i=0;i<parent.length() ;i++)
                {
                         int   character=parent[i] ;
                         int  charnext=0 ;
                         if ( character >=48 && character<=57 )
                           {       charcount++;
                        // cout<<character<<endl;

                              if(i+1<parent.length())
                              {
                                 charnext =parent[i+1];

                                if( (charnext >=48 && charnext<=57) )
                                    {charcount++;}
                                 i++;
                              }
                                count++;
                           }



                              character=character-48;
                              charnext=charnext-48;

                              if(    charcount==2 && count==1 )
                                    rs2=charnext+10*character ;
                              else  if(  charcount==2  && count>=3  &&a==2)
                                    rs1=charnext+10*character ;
                              else  if( charcount==2&&count>=2 &&a==0 )
                                    immediate=immediate*100+charnext+10*character ;
                              else  if (charcount==1 && count==1 )
                                    rs2=character ;
                              else if ( charcount==1 && count>=3 &&a==2)
                                    rs1=character ;
                               else if ( charcount==1 && count>=2 &&a==0 )
                                    immediate=immediate*10+character ;

                        charcount=0;
                        charnext=charnext+48;
                        character=character+48;
                        if(charnext=='('||character=='(')
                                    {
                                      a=2;
                                    }

                }
                istringstream ss(line);
                string labelnew;
                do {
                        ss >> labelnew;

                   }while(ss);
                   cout<<labelnew<<endl;
                   int k=0;
                   int neg=0;
                   immediate=0;
                   while(labelnew[k]!='(')
                           {
                            if(labelnew[k]=='-')
                            {neg=1; k++; continue;}
                            immediate=immediate*10+(labelnew[k]-48);
                            k++;
                           }
                    if(labelnew[k+1]=='s'&&labelnew[k+2]=='p')
                        rs1=2;
                    if(neg==1)
                        immediate=-immediate;

                cout<<rs1<<" "<<rs2<<" "<<immediate<<endl;

           ifstream fin;
       fin.open("S.txt");
       string line1;
       while(fin)
       {
         getline(fin, line1);
           if(!fin)
             break;
       // cout<<line1<<endl;
           istringstream ss(line1);
           int k=0,count=0;

              string label;
                do {
                        ss >> label;
                        //cout<<label<<" ";
                        if(label==word)
                        {
                          k=1;count++; //cout<<"A"<<endl;
                        }
                        else if(k==1&&count==2)
                        {  int y=25;
                        //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else if(k==1&&count==3)
                        {
                          int y=17;
                          //cout<<label<<endl;
                          int l=label.length();
                          for(int j=0;j<l;j++)
                          {
                            bool x=label[j]-48;
                            Mcode[line_no][y]=x;
                            y++;
                          }
                        }
                        else
                          break;
                    count++;

                    } while (ss);


       } fin.close();


                int y=12;
                bool bin[5]={0};
                int m = 4;
                while (rs1 > 0) {
                    bin[m] = (rs1 % 2);
                      rs1 = rs1 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }
                y=7;
                for(int j=0;j<5;j++)
                    bin[j]=0;
                m = 4;
                while (rs2 > 0) {
                    bin[m] = (rs2 % 2);
                      rs2 = rs2 / 2;
                       m--;
                   }
                for(int i=0;i<5;i++)
                {
                    Mcode[line_no][y]=bin[i];
                    y++;
                }

                bool binImm[12]={0};
                bitset<12> bset2(immediate);

                for(int i=0;i<12;i++)
                   binImm[11-i]=bset2[i];
                /*m = 11;
                while (immediate > 0) {
                    binImm[m] = (immediate % 2);
                      immediate = immediate / 2;
                       m--;
                   }*/

                y=20;
                for(int i=7;i<=11;i++)
                {
                   Mcode[line_no][y]=binImm[i];
                   y++;
                }
                y=0;
                for(int i=0;i<=6;i++)
                {
                  Mcode[line_no][y]=binImm[i];
                   y++;
                }

                //fout<<line_no<<"   ";
               for(int j=0;j<32;j++)
                 {cout<<Mcode[line_no][j]; //fout<<Mcode[line_no][j];
                 }
                 string    hex;
                 hex= bin_2_hex( Mcode[line_no] );
                 fout<<hex;
                 fout<<"\n";
                cout<<endl;

           }

  line_no++;
  jump_line++;

    }






     cout<<line<<endl ;

  code.close();

 return 0;
}

