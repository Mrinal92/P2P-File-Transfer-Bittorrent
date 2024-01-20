
#include "lib.h"

// Handles different requests from peer client

int RequestHandling(int Csocket)
{
    string IDclient = "";

    Log(" socket of the cleint is: " + to_string(Csocket) );
    Log("\n");

    char line[1000];

    for(int i=0;i<1000;i++)
  {
    line[i]=0;
  }
  
  if(read(Csocket,line,1000)<1)
  {
    close(Csocket);
    return 0;
  }

  Log(" request of the client at the server : " + (string)line);

  vector<string> terminal = divideStr((string)line,"##",1);

  Log(terminal[0]);

  if(terminal[0] =="get_chunk")
  {
    //inpt = [get_chunk, filename, to_string(IDchunk), destination]
    Log("\n ");
    Log("\n chunk being sent ..");

     string locFile= LocalPath[terminal[1]];
    long long IDchunk =stoll(terminal[2]);

     Log("path of file: " + locFile);

    Log(" being sent " + to_string(IDchunk) + " from " + string(IP_peer) + ":" + to_string(portno_peer));

  partSend(&locFile[0], IDchunk, Csocket);

  }

  else if(terminal[0]=="get_file_path")
 {
   string locFile=LocalPath[terminal[1]];

    Log("\n ");
   Log( "client peer command is: " + (string)line);
   int len=strlen(locFile.c_str());
   write(Csocket,&locFile[0], len);
 }

  else if(terminal[0]=="get_chunk_vector")
 { 
     Log("\n ");
  Log(" chunk vector being sent...");

  string NameofFile=terminal[1];

  vector<int> partVector = partMeta[NameofFile];

  string temp= "";

  for(int j: partVector)
  temp=temp+to_string(j);
  
  char* response=&temp[0];
  int len= strlen(response);

  write(Csocket, response, len );

  Log(" response sent: " + (string)response);

 }

 close(Csocket);

 return 0;

  
}


string peerConnect(char* Pserv, char* IPportServ, string cmd, int t)
{
    int Psock;

    struct sockaddr_in Pserv_Addr;
    int a=t;
    Log("\n");
    Log(" trying is connecting to peer ");

    if((Psock=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
     cout<<endl;
     cout<<" error creating socket"<<endl;
     return "error";
    }

    Log("Socket successfully created ");

    Pserv_Addr.sin_family=AF_INET;

    uint16_t portP = stoi((string)IPportServ);

    Pserv_Addr.sin_port=htons(portP);
    
    Log("\n");
    Log("wants to connect to " + (string)Pserv + ":" + to_string(portP));


    if(inet_pton(AF_INET, Pserv, &Pserv_Addr.sin_addr)<0)
    perror("Connecting Error(INET) with peer ");

    if(connect(Psock,(struct sockaddr *)&Pserv_Addr, sizeof(Pserv_Addr))<0)
    perror("Connecting Error peer");

    Log("Peer connected " + (string)Pserv + ":" + to_string(portP));

    string ongoingCMD = divideStr(cmd, "##",1).front();
    
    Log(" this command " + ongoingCMD);

    if(ongoingCMD=="get_file_path")
    {    
      int len =strlen(&cmd[0]);
      if(send(Psock , &cmd[0] , len , MSG_NOSIGNAL ) == -1)
        {
         cout<<strerror(errno);
         return "error"; 
        }
    char response[10240] ;

     for(int i=0;i<10240;i++)
     {
       response[i]=0;
      }
  
    if(read(Psock,response,10240) < 0){
        perror("err: ");
        return "error";
        }
      Log("server's response to acquire file's path:" + (string)response);
     LocalPath[divideStr(cmd,"##",1).back()] =(string)response;

    }

    else if(ongoingCMD=="get_chunk")
    {
     //"get_chunk ## filename ## to_string(IDchunk) ## destination
     int len = strlen(&cmd[0]);
     if(send(Psock , &cmd[0] , len,MSG_NOSIGNAL ) ==-1)
     {
      cout<<strerror(errno);
    return "error"; 

      }
     Log("sent command to peer: " + cmd);
     vector<string> token =divideStr(cmd, "##",1);
        
     string target =token[3];
     long long IDchunk =stoll(token[2]);
     
     Log("\n");
     Log(" getting chunk" +to_string(IDchunk) +"from " + string(IPportServ));

    partWrite(Psock, IDchunk,&target[0],1);

     return "ss";

    }

    else if(ongoingCMD=="get_chunk_vector")
    {
     int len=strlen(&cmd[0]);

     if(send(Psock , &cmd[0], len,MSG_NOSIGNAL )==-1)
     {
        cout<<strerror(errno);
        return "error"; 
     }
    Log("sent command to peer: " + cmd);

     char response[10240] ;
     
     for(int i=0;i<10240;i++)
     {
       response[i]=0;
      }

      if(read(Psock, response,10240) < 0)
      {
        perror("err:");
        return "error";
    }
     Log("received response: " + (string)response);

    close(Psock);

     return string(response);
    }

    close(Psock);
    Log("closing the connection with " +(string)Pserv + ":" + to_string(portP));
    
    return "aa";


}


void* peerAserver(void* arg)
{
  int Ssocket;

  struct sockaddr_in addr;

  int len=sizeof(addr);

  int flag=1;
  
  Log("\n");
  Log(""+ to_string(portno_peer)+ "starting as server");

  if((Ssocket=socket(AF_INET,SOCK_STREAM,0))==0)
  {
    perror("failed socket connection ");
    exit(1);
  }

  Log("socket created (server) successfully ");

  if(setsockopt(Ssocket,SOL_SOCKET,SO_REUSEADDR |SO_REUSEPORT, &flag, sizeof(flag)))
   { 
        
        perror("setting socket option");

        exit(1); 
    } 

    addr.sin_family=AF_INET;
    addr.sin_port=htons(portno_peer);

    if(inet_pton (AF_INET,&IP_peer[0],&addr.sin_addr)<1) 
    {  
        cout<<endl;
        cout<<"not a valid address "<<endl; 
        return NULL; 
    } 
       
    if(bind(Ssocket, (struct sockaddr *) &addr, sizeof(addr))<0)
     { 
        perror("unsuccessful binding"); 
        exit(1); 
    } 
     Log(" successful binding");

    if (listen(Ssocket,3) < 0)
     { 
     perror(" listening"); 
        exit(1); 
    } 

    Log("Listening");
    Log("\n");

        


vector<thread> threadVec;

while(1)
{
    int Csocket;

    if((Csocket =accept(Ssocket, (struct sockaddr *)&addr, (socklen_t *)&len ) )<0)
    {
        perror(" error accepting the connection ");
        Log(" accept error ");
    }
    Log(" Accepted the connection ");

    threadVec.push_back(thread(RequestHandling, Csocket));

}

 for(auto j=threadVec.begin(); j!=threadVec.end();j++)
 {
    if(j->joinable())
    j->join();
 }

  close(Ssocket);




}
