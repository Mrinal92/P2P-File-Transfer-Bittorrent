





#include <openssl/sha.h>
#include <sys/types.h> 

#include <unistd.h> 
#include <sys/socket.h> 

#include <bits/stdc++.h>

#include <sys/ioctl.h> 
#include <pthread.h>

#include <errno.h> 
#include <fcntl.h>
#include <signal.h> 

#include <sys/stat.h>


#include <stdarg.h> 


#include <sys/time.h> 
#include <arpa/inet.h> 
#include <string.h> 



#include <netdb.h> 
#include <sys/types.h>


using namespace std; 

#define Tport 5353










int fileDown(vector<string>, int, string);


int fileUp(vector<string>, int, string);



int LoginCheck(vector<string>,int );




int ConHandle(int);



int files(vector<string>, int);

int noShare(vector<string>, int, string);


int GroupJoin(vector<string>, int, string);



int groups(vector<string>, int);



vector<string> divideStr(string, string, int);


int leftGroup(vector<string>, int, string);


int UserCreate(vector<string>,int);




int deleteLog();


int Log(const string &);

bool AlreadyPath(const string &s, int );



int requests(vector<string>, int, string);




void* validate_inp(void*);



vector<string> metaTrack(char*,int);

int handleArg(int, char **,int);


int reqAck(vector<string>, int, string);



int GroupCreate(vector<string>, int, string,int);







//----------------------------------------------------------------------------------------------------------------------------------------------




extern unordered_map<string, string> Authen;





extern unordered_map<string, bool> AlreadyLogin;




extern string NameLogfile;
extern string IP_track1;
extern string IP_track2;
extern string Ongoing_TrackIP;
extern string NameSeeder;



extern unordered_map<string, unordered_map<string, set<string>>> ListSeed; // groupid -> {map of filenames -> peer address}

extern unordered_map<string, string> SizeFile;


extern unordered_map<string, string> admins;



extern uint16_t portno_track1;

extern uint16_t portno_track2;


extern uint16_t Ongoing_TrackPort;



extern unordered_map<string, set<string>> members;



extern unordered_map<string, set<string>> ReqPending;
;

extern unordered_map<string, string> ntoport;



extern vector<string> group;


extern unordered_map<string, string> hashPiece; 

//------------------------------------------------------------------------------------------------------------------------------------------------------------
