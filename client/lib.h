
#include<pthread.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include <openssl/sha.h>
#include<fcntl.h>
#include <bits/stdc++.h>


using namespace std;










extern bool AlreadyLogged;



extern vector<vector<string>> OngoingChunksDown;



extern unordered_map<string, string> LocalPath;



extern vector<string> PartHashOngoing;



extern uint16_t portno_peer;
extern uint16_t portno_track1;

extern uint16_t portno_track2;




extern unordered_map <string, string> AlreadyDown;



extern bool compromised;


extern string NameLogfile;
extern string IP_track1;
extern string IP_track2;
extern string IP_peer;
extern string NameSeeder;



extern unordered_map<string, unordered_map<string, bool>> AlreadyUpload; //group -> filename -> bool



extern unordered_map<string, vector<int>> partMeta;




//---------------------------------------------------------------------------------------------------------


int StoreHash (string, string&, int ); 

string sha256full(char*, int );


string Combined_Hash(char*, int);






int deleteLog();



int partWrite(int ,long long,char*, int );



int RequestHandling(int );



string peerConnect(char*, char*,string, int);



void* peerAserver(void*);



vector<string> divideStr(string,string,int);


long long size_of_file(char*);



int TrackCon(int, struct sockaddr_in &,int, int);



int Log(const string &);



int HandleCommands(vector<string>,int,int);



int handleArg(int ,char **);



int partSend(char*,int,int);



vector<string> metaTrack(char*,int);



int printDownloaded();


int PartSet(string, long long, long long, bool);


int groups(int,int);



int requests(int);



int reqAck(int);



int leftGroup(int);



int Files(int);



int partsAlgo(vector<string>, vector<string>);


int fileDown(vector<string>,int,int);



int fileUp(vector<string>,int, int );




//------------------------------------------------------------------------------------------------------------------------------



#define part_size 524288

#define line_size 32768
