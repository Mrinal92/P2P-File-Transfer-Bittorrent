#include"lib2.h"


 string NameLogfile;
 string IP_track1;

 unordered_map<string, string> Authen;


 unordered_map<string, unordered_map<string, set<string>>> ListSeed;

 unordered_map<string, string> SizeFile;

 unordered_map<string, string> admins;

 vector<string> group;

 unordered_map<string, set<string>> members;

 string IP_track2;
 string Ongoing_TrackIP;
string NameSeeder;

 uint16_t portno_track1;

 uint16_t portno_track2;


  unordered_map<string, bool> AlreadyLogin;


   unordered_map<string, set<string>> ReqPending;

 unordered_map<string, string> ntoport;

 unordered_map<string, string> hashPiece; 


 uint16_t Ongoing_TrackPort;

 
int main(int  argc, char *argv[])
{    
    int numarg=argc;

    if(numarg!=3)
    {
     cout<<"give arguments in correct format ";
     cout<<endl;
     return -1;
    }
     
    handleArg(numarg, argv,1);

    int Tsocket;
    struct sockaddr_in Addr;

    int flag;
    flag=1;

    int len=sizeof(Addr);

    pthread_t IDthreadExit;

    if ((Tsocket =socket(AF_INET,SOCK_STREAM,0)) ==0)
     { 
        perror("socket unsuccessful"); 
        exit(1); 
    } 

     Log(" socket (tracker) successfully created");
      
     int len1 = sizeof(flag);

    if (setsockopt(Tsocket,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT,&flag,len1))
     { 
       
        perror("setting socket options"); 
        exit(1); 
    } 


    Addr.sin_family= AF_INET; 
    Addr.sin_port =htons(Ongoing_TrackPort); 

    
    if(inet_pton(AF_INET,&Ongoing_TrackIP[0], &Addr.sin_addr)<1)
      { 
        cout<<"no such address"<<endl; 
        return -1; 
    } 
    
     if (bind(Tsocket, (sockaddr *)&Addr,  sizeof(Addr))<0) 
     { 
        perror("unsuccessful binding "); 
        exit(1); 
    } 
    
    Log(" binding successful");

    if (listen(Tsocket,3) <0)
     { 
        perror("listen"); 
        exit(1); 
    } 
    Log(" being listened");

    vector<thread> Vthread;

    if(pthread_create(&IDthreadExit, NULL, validate_inp,NULL) ==-1)
    {
        perror("pthread"); 

        exit(1); 
    }

    while(1)
    {

    int Csocket;

    if((Csocket = accept(Tsocket,(sockaddr*)&Addr,(socklen_t *)&len)) <0)
    {
            perror( "error in accepting");
            Log("Error in accept"); 
        }

     Log("Accepted the Connection ");


     Vthread.push_back(thread(ConHandle, Csocket));
    }

    for(auto itr=Vthread.begin(); itr!=Vthread.end(); itr++)
    {
        if(itr->joinable()) 
        itr->join();
    }

   Log(" now exit");

    return 0;
}




