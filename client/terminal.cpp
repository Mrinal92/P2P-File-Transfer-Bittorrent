#include "lib.h"

int printDownloaded();


int reqAck(int Socket)
{

    char test[6];
  strcpy(test,"test1");


  write(Socket, test,6);

  char data[100];

  read(Socket,data,100);
  cout<<data<<endl;

  return 0;


}

int groups(int Socket,int t)
{
  
  char test[6];
  strcpy(test,"test1");
  int b=t;

  write(Socket, test,6);
  
  int respSize=3*line_size;

  char response[respSize];

  for(int i=0;i<respSize;i++)
  {
    response[i]=0;
  }

  Log("gorup list server respons : " + (string)response);

  vector<string> listofgroups=divideStr((string)response, "##",1);

  int numgroups=listofgroups.size();

  for(size_t j=0;j<numgroups-1;j++)
  cout<<listofgroups[j]<<endl;

  return 0;

}

int requests(int Socket)
{

    Log(" looking for reply ");

     char test[6];
  strcpy(test,"test1");


  write(Socket, test,6);

  int respSize=3*line_size;

  char response[respSize];

   for(int i=0;i<respSize;i++)
  {
    response[i]=0;
  }

  read(Socket,response,respSize);
  
  if((string)response=="**err**")  // change **err** to Error 001
   return -1;
  if((string)response=="**er2**")  // change **err** to Error 002
  return 1;

  
  Log(" list of requests: " + (string)response);

  vector<string> listofrequests = divideStr((string)response, "##",1);
  
  int numlists=listofrequests.size();

  Log(" request list response size: " + to_string(numlists));

  for(size_t j=0;j<numlists-1;j++)
  cout<<listofrequests[j]<<endl;

  return 0;

}

int Files(int Socket)
{
    char test[6];
  strcpy(test,"test1");


  write(Socket, test,6);

  char data[1024];
  bzero(data,1024);
  read(Socket,data,1024);


  vector<string> fileList = divideStr((string)data, "##",1);

  

  for(auto it: fileList)
  {cout<<it;
  cout<<endl;
  }

  return 0;
}


int leftGroup(int Socket)
{
    Log(" looking for reply ");

    char data[100];
    read(Socket,data,100);

    cout<<data<<endl;

    return 0;
}





int HandleCommands(vector<string> terminal, int Socket, int t)
{
  

  char response[10240];

   for(int i=0;i<10240;i++)
  {
    response[i]=0;
  }

 read(Socket, response,10240); 

 cout<<response;

 int x = t;

 Log(" first server response: " + (string)response );

 if((string)response=="Invalid argument count")
 return 0;

 if(terminal[0]=="login")
 {
    if((string)response=="Login Successful")
    {
        AlreadyLogged=true;

        string AddressPeer="";

        AddressPeer= IP_peer + ":" + to_string(portno_peer);
        
        int len = AddressPeer.length();
        write(Socket, &AddressPeer[0],len);
    }
 }

 else if(terminal[0]=="upload_file")
 {
   if((string)response=="Error 102:")
   {
    cout<<"this is not your group ";
    cout<<endl;
    return 0;

   }

   else if((string)response=="Error 103:" )
   {
    cout<<"no such file "<<endl;
    return 0;
   }

   else if((string)response=="Error 101:")
   {
      cout<<"no such group"<<endl;
    return 0;
   }

   return fileUp(terminal, Socket,1);
 }

 else if(terminal[0]=="download_file")
 {
   
   if((string)response == "Error 103:")
   {
    cout<<"no such directory";
    cout<<endl;
    return 0;

   }

   else if((string)response == "Error 101:")
   {
    cout<<"no such group";
    cout<<endl;
    return 0;
   }

   else if((string)response == "Error 102:")
   {
    cout<<"this is not your group ";
    cout<<endl;
    return 0;

   }

   if(AlreadyDown.find(terminal[2])!=AlreadyDown.end())
   {
    cout<<"file has been downloaded already";
    cout<<endl;
    return 0;

   }

   return fileDown(terminal, Socket,1);
 }

 else if(terminal[0]=="logout")

 AlreadyLogged=false;

 else if(terminal[0]=="leave_group" )
leftGroup(Socket);


 else if(terminal[0]=="list_groups" )
return groups(Socket,1);


else if(terminal[0]=="list_requests" )
{
    int d;

 if((d =requests(Socket))< 0)
 {
    cout<<"not admin of current group";
    cout<<endl;
}
else if(d >0)
{
    cout<<"all requests caught up";
    cout<<endl;
 }

    else return 0;
}

else if(terminal[0]=="accept_request" )
reqAck(Socket);



else if(terminal[0]=="stop_share" )
AlreadyUpload[terminal[1]].erase(terminal[2]);

else if(terminal[0]=="show_downloads" )
printDownloaded();

else if(terminal[0]=="list_files" )
Files(Socket);



  return 0;



}


int printDownloaded()
{
    for (const auto& entry : AlreadyDown) 
    cout << "[C] " << entry.second << " " << entry.first << endl;

    return 0;

}