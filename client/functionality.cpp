
#include "lib.h"


int handleArg(int argc, char*argv[])
{
    string metaPeer=argv[1];

    string trackerMetaName=argv[2];

    NameLogfile = metaPeer+"_log.txt";

    deleteLog();

    vector<string> addrPeer=divideStr(metaPeer,":",1);

    IP_peer=addrPeer[0];

    portno_peer=stoi(addrPeer[1]);

    char thisDirec[200];

    getcwd(thisDirec,200);

    string dir= (string)thisDirec;

    dir=dir+"/" +trackerMetaName;

    vector<string> metaTracker = metaTrack(&dir[0],1);

    IP_track1 = metaTracker[0];

    IP_track2 = metaTracker[2];

    portno_track1= stoi(metaTracker[1]);

    portno_track2= stoi(metaTracker[3]);

    
    Log(" Address of peer: " + (string)IP_peer + ":" + to_string(portno_peer));

    Log(" Address of 1st tracker: " + (string)IP_track1+ ":" + to_string(portno_track1));

    Log(" Address of 1st tracker: " + (string)IP_track2+ ":" + to_string(portno_track2));
    
    Log(" name of the log file is: " + NameLogfile + "\n");

    return 0;

}



int deleteLog()
{
 
 ofstream out;
 out.open(NameLogfile);

 out.clear();

 out.close();

return 0;
}


vector<string> divideStr(string addr, string delim=":", int t=1)
{
 
 vector<string> ans;

 size_t idx;
 idx=0;

 while((idx=addr.find(delim))!=string::npos)
 {
    string d=addr.substr(0,idx);
    ans.push_back(d);

    int len=delim.length();
    addr.erase(0, idx+len);

 }
 ans.push_back(addr);

 return ans;
}

int TrackCon(int IdTrack, struct sockaddr_in &AddrServ, int Socket, int t)
{
 
 char* OngoingIPtrack;

 int g=t;

 uint16_t OngoingPortTrack;

 if(IdTrack!=1)
 {
  OngoingIPtrack = &IP_track2[0]; 
 OngoingPortTrack = portno_track2;
  
 }

 else
 {
    OngoingIPtrack = &IP_track1[0]; 
 OngoingPortTrack = portno_track1;
  
 }

bool error;
error = 0;

AddrServ.sin_port=htons(OngoingPortTrack);
AddrServ.sin_family=AF_INET;

if(inet_pton(AF_INET, OngoingIPtrack,&AddrServ.sin_addr)<1)
error=1;

if(connect(Socket, (struct sockaddr *)&AddrServ, sizeof(AddrServ))<0)
{
    error=1;
}

if(error==1)
{
    if(IdTrack!=1)
    return -1;

    else
    return TrackCon(2,AddrServ,Socket,1);
}

Log(" server to which we are connected: " + to_string(OngoingPortTrack));

return 0;



}


int PartSet(string filename, long long left, long long right, bool AlreadyUpload) 
{
    if (AlreadyUpload) 
    {
        vector<int> tmp(right - left + 1, 1);
        partMeta[filename] = tmp;
    }
    else 
    {
        if (partMeta.find(filename) == partMeta.end()) 
        {
            partMeta[filename] =vector<int>(right - left + 1, 0);
        }

        for (long long i = left; i <= right; ++i) 
        {
            partMeta[filename][i] = 1;
        }

        Log("chunk vector updated for " + filename + " from " + to_string(left) + " to " + to_string(right));
    }

    return 0;
}

vector<string> metaTrack(char* dir, int t)
{
    fstream trackerMeta;

    trackerMeta.open(dir,ios::in);

    vector<string> ans;

    int y =t;
  
  if(trackerMeta.is_open())
  {
    string d;

    while(getline(trackerMeta,d))
    {
        ans.push_back(d);

    }
    trackerMeta.close();
  }

  else
  {
   cout<<"no file for tracker information ";
   cout<<endl;
   exit(-1);
  }

  return ans;


}

int Log(const string &text) 
{
 ofstream log_file(NameLogfile,ios::out |ios::app);

    if (log_file.is_open()) 
    {
        log_file << text << endl;

        log_file.close();  // Close the file explicitly when done.
    } 

    else
    {
        // Handle the case where the file couldn't be opened.
        cout << "Error: Unable to open log file" << endl;
    }
    return 0;
}




