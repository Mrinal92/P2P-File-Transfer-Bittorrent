#include "lib.h"

string sha256full(char* dir, int t)
{
    ifstream temp (dir);
    ostringstream chunk;

    chunk<<temp.rdbuf();

    string str1;
    int x=t;

    str1 = chunk.str();

    string sha256code;

    unsigned char ip[32];

    int len =str1.length();


    if(SHA256(reinterpret_cast<const unsigned char *>(&str1[0]),len, ip))
    {
        for(int j=0;j<32;j++)
        {
            char byte[3];
            sprintf(byte,"%02x",ip[j]&0xff);
            sha256code = sha256code + (string)byte;
        }
    }

    else
    cout<<"Error Calculating SHA256 file hash"<<endl;

    return sha256code;

}

int StoreHash (string part, string& hashStore, int t)
{
 
 int len = part.length();
 unsigned char ip[20];
 int x = t;
 
 if(SHA1(reinterpret_cast<const unsigned char *>(&part[0]),len, ip))
    {
        for(int j=0;j<20;j++)
        {
            char byte[3];
            sprintf(byte,"%02x",ip[j]&0xff);
            hashStore = hashStore + (string)byte;
        }
    }

 else
 cout<<"Error Calculating/Storing SHA1 hash"<<endl;

 hashStore = hashStore + "##";
 return 0;

}

long long size_of_file(char *dir)
{
 FILE *ip=fopen(dir,"rb");
  
  long long end=-1;

 if(ip)
 {
 fseek(ip,0,SEEK_END);
 end = ftell(ip)+1;
 fclose(ip);
 
 }

 else
 {
    cout<<"no such file"<<endl;
    return -1;
 }

 return end;

}

string Combined_Hash(char* dir, int t)
{
  
  FILE *ip;

  int j;
  int total;

  long long size= size_of_file(dir);

  if(size==-1)
  return "#";
  
  char part[line_size+1];
  int parts=(size/part_size) + 1;

  string code;

  code="";

  if(ip)
  {
    
    for(j=0;j<parts;j++)
    {   
        string linestring;
        total=0;
        
        int bytes;
        bytes = fread(part,1,min(line_size,part_size-total),ip);

        while(bytes && part_size > total)
        {
           part[bytes]='\0';

           linestring=linestring+part;
           total=total+strlen(part);
           
           memset(part,0,sizeof(part)); 
        }

        StoreHash(linestring, code, 1); 
    }

    fclose(ip);
  }

  else
  cout<<"no such file"<<endl;

  
  int l=2;

  while(t--)
  code.pop_back();

  return code;
  
}