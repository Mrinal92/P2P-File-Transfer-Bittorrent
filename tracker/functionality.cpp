
#include"lib2.h"


int deleteLog()
{
 
 ofstream out;
 out.open(NameLogfile);

 out.clear();

 out.close();

 return 0;

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

vector<string> divideStr(string addr, string delim=":" ,int t=1)
{
 
 vector<string> ans;

 int u=t;

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

bool AlreadyPath(const string &str, int t)
{
  struct stat data;


  return (stat (str.c_str(), &data) ==0);
}


void* validate_inp(void* argument)
{
while(1)
{
    string line;
 getline(cin, line);

     if(line == "quit")
    {
     exit(0);
    }
 }
}

vector<string> metaTrack(char* dir,int t)
{
    fstream trackerMeta;

    trackerMeta.open(dir,ios::in);

    vector<string> ans;

    int a =t;
  
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

int handleArg(int argc,char *argv[], int t)
{
    NameLogfile = "logTracker" + (string)argv[2] + ".txt";

    deleteLog();
     int y=t;
    vector<string> Taddr= metaTrack(argv[1],1);

    if(string(argv[2]) != "1")
    { 

        IP_track2 = Taddr[2];
        portno_track2 = stoi(Taddr[3]);
        Ongoing_TrackIP = IP_track2;
        Ongoing_TrackPort = portno_track2;

    }

    else
    {
         IP_track1 = Taddr[0];
        portno_track1 = stoi(Taddr[1]);
        Ongoing_TrackIP = IP_track1;
        Ongoing_TrackPort = portno_track1;

    }

    Log("addr of tracker 1 " + string(IP_track1)+ ":" +to_string(portno_track1));
    Log("addr of tracker 2 : " + string(IP_track2)+ ":" +to_string(portno_track2));
    Log("name of log file: " + string(NameLogfile));
    Log("\n");

    return 0;

}



