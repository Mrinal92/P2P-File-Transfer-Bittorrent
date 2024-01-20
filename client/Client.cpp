#include "lib.h"

 string NameLogfile;
 string IP_track1;


 uint16_t portno_track2;
vector<vector<string>> OngoingChunksDown;
 unordered_map<string, string> LocalPath;

vector<string> PartHashOngoing;

 unordered_map <string, string> AlreadyDown;


 bool compromised;

bool AlreadyLogged;

string IP_track2;
 string IP_peer;
string NameSeeder;

 uint16_t portno_peer;
 uint16_t portno_track1;

 int HandleCommands(vector<string>,int,int);
 unordered_map<string, unordered_map<string, bool>> AlreadyUpload; //group -> filename -> bool

 unordered_map<string, vector<int>> partMeta;


int main(int argc,char* argv[])
{
    int numberArg=argc;

    if(numberArg!=3)
    {
        cout<<"correct format is : peer IP address: port  tracker info file "<<endl;
        return -1;
    }
    
    handleArg(numberArg, argv);

    int Socket;

    Socket=0;

    struct sockaddr_in AddrServ;

    pthread_t threadServ;

    Socket=socket(AF_INET, SOCK_STREAM,0);

    if(Socket<0)
    {   
        cout<<endl;
        cout<<" not able to create socket "<<endl;
        return -1;
    }

    Log(" socket for the peer created ");

    if(pthread_create(&threadServ,NULL,peerAserver,NULL)==-1)
    {
      perror("pthread");
      exit(1);
    }

    if(TrackCon(1,AddrServ,Socket,1)<0)
    exit(-1);

    while(1)
    {
        cout<<"->";

        string str;
        string line;

        getline(cin,line);
         
         int len=line.length();

        if(len<1)
        continue;

        stringstream stream(line);

        vector<string> terminal;

        while(stream >>str)
        terminal.push_back(str);

        if(AlreadyLogged==true && terminal[0]== "login")
        {
            cout<<"existing session"<<endl;
            continue;
        }

        if(AlreadyLogged==false && terminal[0] !="create_user" && terminal[0] !="login")
        {
            cout<<"register or login"<<endl;
            continue;
        }
        
        int len1 = strlen(&line[0]);

        if(send(Socket, &line[0],len1,MSG_NOSIGNAL)==-1)
        {
          cout<<strerror(errno);
          return -1;

        }

        Log(" to server send: " +terminal[0]);

        HandleCommands(terminal,Socket,1);
    }

    close(Socket);
 
    return 0;
}