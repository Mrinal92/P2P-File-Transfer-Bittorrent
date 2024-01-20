#include "lib2.h"


int createUser(vector<string> terminal)
{

     if (terminal.size() != 3) 
     {
        cout<<"Invalid argument count"<<endl;
        return -1;  
    }
    
    string Username = terminal[1];

    string Password = terminal[2];

    if(Authen.find(Username) != Authen.end())
    { 
        return -1;
    }

    else
    {
        Authen.insert({Username, Password});

    
    }

    return 0;
}

int validateLogin(vector<string> terminal) {
   
   
    if (terminal.size() != 3) 
     {
        cout<<"Invalid argument count"<<endl;
        return -1;  
    }

    
    string Username = terminal[1];

    string Password = terminal[2];


    auto loginIter = Authen.find(Username);

    if (loginIter ==Authen.end() || loginIter->second != Password) 
    {
        return -1;  // Username/password incorrect
    }

    auto itr = AlreadyLogin.find(Username);

    if (itr == AlreadyLogin.end()) {
        AlreadyLogin[Username] = true;
    } else if (itr->second) {
        return 1;  // User already has an active session
    } else {
        AlreadyLogin[Username] = true;
    }

    return 0;  // Login Successful
}


int fileUp(vector<string> terminal, int Csocket, string UserNameClient) //inpt - upload_file​ <file_path> <group_id​>
{
 
 if(terminal.size()!= 3)
   {
        write(Csocket,"Invalid argument count", 22);
    }
    
     else if(members[terminal[2]].find(UserNameClient) ==members[terminal[2]].end())
    {
        write(Csocket, "Error 102:", 10);
    }


    else if(members.find(terminal[2])==members.end())
    {
        write(Csocket, "Error 101:", 10);
    }
   
    else if(!AlreadyPath(terminal[1],1))
    {
        write(Csocket, "Error 103:", 10);
    }

    else
    {
        char info_file[524288] =  {0};
        
        write(Csocket, "upload in progress...", 12);

        Log("being uploaded");

        if(read(Csocket , info_file, 524288))
        {
            if(string(info_file) == "error") 
            return 0;

            vector<string> detailsFile = divideStr(string(info_file), "##",1);

            //fdet = [filepath, peer address, file size, file hash, piecewise hash] 

            string filename = divideStr(string(detailsFile[0]), "/",1).back();

            string PieceHash = "";

            int len = detailsFile.size();

            for(size_t i=4; i<len; i++)
            {
                PieceHash = PieceHash + detailsFile[i];

                if(i != detailsFile.size()-1) 
                PieceHash = PieceHash +  "##";
            }
            
            hashPiece[filename] = PieceHash;
            
            if(ListSeed[terminal[2]].find(filename) != ListSeed[terminal[2]].end())
            {
                ListSeed[terminal[2]][filename].insert(UserNameClient);
            }

            else
            {
                ListSeed[terminal[2]].insert({filename, {UserNameClient}});
            }

            SizeFile[filename] = detailsFile[2];
            
            write(Csocket, "Uploaded", 8);
        }
    }

    return 0;

}

int fileDown(vector<string> terminal, int Csocket, string UserNameClient)   // inpt - download_file​ <group_id> <file_name> <destination_path>
{
  
   if(members.find(terminal[1]) ==members.end())
   {
        write(Csocket, "Error 101:", 10);
    }

    else if(terminal.size() != 4)
    {
        write(Csocket, "Invalid argument count", 22);
    }
    
      else if(members[terminal[1]].find(UserNameClient) ==members[terminal[1]].end())
    {
        write(Csocket, "Error 102:", 10);
    }
    
      else{
        if(!AlreadyPath(terminal[3],1))
        {
            write(Csocket, "Error 103:", 10);
            return 0;
        }

        char info_file[524288] =  {0};
        // info_file = [filename, destination, group id]
        write(Csocket, "Downloading...", 13);

        if(read(Csocket , info_file, 524288)){
            vector<string> detailsFile = divideStr(string(info_file), "##",1);
            
            string response = "";

         if(ListSeed[terminal[1]].find(detailsFile[0]) != ListSeed[terminal[1]].end())
            {

            for(auto i: ListSeed[terminal[1]][detailsFile[0]])
               
                {
                    if(AlreadyLogin[i])
                 response = response + ntoport[i] + "##";
                    
                }
             response = response + SizeFile[detailsFile[0]];
             Log(" List of seeders "+ response);
            write(Csocket, &response[0], response.length());

            char test[6];
            read(Csocket, test,6);
                
            write(Csocket, &hashPiece[detailsFile[0]][0], hashPiece[detailsFile[0]].length());
            
             ListSeed[terminal[1]][terminal[2]].insert(UserNameClient);
            }

            else
            {
                write(Csocket, "File not found", 14);
            }
        }
        
    }

    return 0;

}

int create_group(vector<string> terminal, int Csocket, string UserNameClient)
 {
    if(terminal.size() != 2)
    {
        write(Csocket,"Invalid argument count", 22);
        return -1;
    }

    

    for(auto i: group)
    {
        if(i == terminal[1]) 
        return -1;
    }

    admins.insert({terminal[1], UserNameClient});
    group.push_back(terminal[1]);
    members[terminal[1]].insert(UserNameClient);
    return 0;
}

int groups(vector<string> inpt, int Csocket)  //inpt - [list_groups];
{
   
    if(inpt.size() != 1)
    {
        write(Csocket, "Invalid argument count",22);
        return 0;
    }

    write(Csocket, "All groups:", 11);

    char test[6];

    read(Csocket, test, 6);

    if(group.size() ==0)
    {
        write(Csocket, "No groups found##",18);
        return 0;
    }

    string response = "";

    for(size_t i=0;i<group.size();i++)
    {
        response = response+ group[i] + "##";
    }
    
    int len = response.length();

    write(Csocket, &response[0], response.length());

    return 0;
}

int GroupJoin(vector<string> terminal, int Csocket, string UserNameClient) //inpt - [join_group gid]
{
    
    if(terminal.size() !=2)
    {
        write(Csocket, "Invalid argument count", 22);
        return 0;
    }

    Log("GroupJoin function ");

    if(admins.find(terminal[1]) == admins.end())
    {
    write(Csocket, "Invalid group ID.", 19);
    }

    else if(members[terminal[1]].find(UserNameClient) ==members[terminal[1]].end())
    {
    ReqPending[terminal[1]].insert(UserNameClient);
    write(Csocket, "Group request sent", 18);
    }

    else
    {
     write(Csocket, "You are already in this group",30);
    }

    return 0;
    
}

int requests(vector<string> terminal, int Csocket, string UserNameClient)// inpt - [list_requests groupid]
{
    
    if(terminal.size() != 2){
        write(Csocket, "Invalid argument count", 22);
        return 0;
    }
    write(Csocket, "Fetching group requests...", 27);

    char test[6];

    read(Csocket, test, 6);

    
    if(admins.find(terminal[1])==admins.end() || admins[terminal[1]] != UserNameClient)
    {
       
        write(Csocket, "**err**", 7);
    }

    else if(ReqPending[terminal[1]].size() == 0)
    {
        write(Csocket, "**er2**", 7);
    }

    else 
    {
        string response = "";

        Log("pending request size: "+ to_string(ReqPending[terminal[1]].size()));

        for (const auto& item : ReqPending[terminal[1]])
         {
        response = response + (string) + "##";
         }


        write(Csocket, &response[0], response.length());

        Log(" response is :" + response);
    }

    return 0;
}

int reqAck(vector<string> terminal, int Csocket, string UserNameClient)  // inpt - [accept_request groupid user_id]
{
   
    if (terminal.size() != 3) {
        write(Csocket, "Invalid argument count", 22);
        return 0;
    }
    write(Csocket, "Accepting request...", 21);

    char test[6];
    read(Csocket, test, 6);


    auto adminIt = admins.find(terminal[1]);

    if (adminIt == admins.end()) 
    {
        Log(" accept_request if");
        write(Csocket, "Invalid group ID.", 19);
    }
     
     else if (adminIt->second == UserNameClient) 
     {
        Log("inside accept_request else if:");
        for (const string& user : ReqPending[terminal[1]]) 
        {
          Log(user);
        }
        ReqPending[terminal[1]].erase(terminal[2]);
        members[terminal[1]].insert(terminal[2]);

        write(Csocket, "Request accepted.", 18);
    }
     else 
    {
        Log("inside accept_request else");
        write(Csocket, "You are not the admin of this group", 35);
    }

    return 0;
}

int leftGroup(vector<string> terminal, int Csocket, string UserNameClient) // inpt - [leave_group groupid]
 {
   
    if (terminal.size() != 2)
     {
        write(Csocket, "Invalid argument count", 22);
        return 0;
    }
    write(Csocket, "Leaving group...", 17);

    auto adminIt = admins.find(terminal[1]);
    auto membersIt = members.find(terminal[1]);

    if (adminIt == admins.end()) 
    {
        write(Csocket, "Invalid group ID.", 19);
    } 
    else if (membersIt != members.end())
    {
        auto member = membersIt->second.find(UserNameClient);
     if (member != membersIt->second.end())
      {
            if (adminIt->second == UserNameClient) 
            {
                write(Csocket, "You are the admin of this group, you can't leave!", 48);
            } 
            else 
            {
                membersIt->second.erase(member);
                write(Csocket, "Group left successfully", 23);
            }
        } 
        else 
        {
            write(Csocket, "You are not in this group", 25);
        }
    }

    return 0;
}

int files(vector<string> inpt, int Csocket) // inpt - list_files​ <group_id>
{
    if (inpt.size() != 2)
     {
        write(Csocket, "Invalid argument count", 22);
        return 0;
    }
    write(Csocket, "Fetching files...", 17);

    char test[5];
    read(Csocket, test, 6);

    Log("test read");

    auto adminIt = admins.find(inpt[1]);
    auto seederListIt = ListSeed.find(inpt[1]);

    if (adminIt == admins.end()) 
    {
        write(Csocket, "Invalid group ID.", 19);
    } 
    else if (seederListIt != ListSeed.end() && !seederListIt->second.empty())
     {
        Log(" list files");

        string response = "";
        
        for (const auto& seeder : seederListIt->second) 
        {
            response += seeder.first + "##";
        }
        int len = response.length();
        response = response.substr(0, len - 2);

        Log("list of files response is : " + response);

        write(Csocket, &response[0], len);
    }
     
     else
     {
        write(Csocket, "No files found.", 15);
    }

    return 0;
}


int noShare(vector<string> terminal, int Csocket, string UserNameClient)    // inpt - stop_share ​<group_id> <file_name>
 {
 
    if (terminal.size() !=3) 
    {
        write(Csocket, "Invalid argument count", 22);
        return 0;
    }
    auto adminIt = admins.find(terminal[1]);
    auto seederListIt = ListSeed.find(terminal[1]);

    if (adminIt == admins.end()) 
    {
        write(Csocket, "Invalid group ID.", 19);
    }
     else if (seederListIt != ListSeed.end() && seederListIt->second.find(terminal[2]) != seederListIt->second.end())
      {
        seederListIt->second[terminal[2]].erase(UserNameClient);

        if (seederListIt->second[terminal[2]].empty())
         {
            seederListIt->second.erase(terminal[2]);
        }
        write(Csocket, "Stopped sharing the file", 25);

    }
     else
      {
        write(Csocket, "File not yet shared in the group", 32);
    }

    return 0;
}

int ConHandle(int Csocket)
{
  string usernameC;
  usernameC = "";
  
  string usergroupC;
  usergroupC="";

  Log(" thread commenced: socket number (client)" + to_string(Csocket));

  while(1)
  {
    char line[2048] = {0}; 

    if(read(Csocket ,line, 2048) <1)
    {
        AlreadyLogin[usernameC] = false;
            close(Csocket);
            break;
        }

     Log("request of client is :" + string(line));   

     string d;
     string input = (string)line;
     stringstream ss(input);
     vector<string> terminal;

    while(ss >>d)
    {
            terminal.push_back(d);
        }

     if (terminal[0] == "create_user") 
     {
        if (terminal.size() != 3) 
        {
        write(Csocket, "Invalid argument count", 22);
        } 
        else 
        {
            int result = createUser(terminal);

            if (result < 0) 
            {
               write(Csocket, "User exists", 11);
            }
            else
            {
                write(Csocket, "Account created", 15);
             }
        }
     }
       


        else if( terminal[0] =="upload_file")
        {
         
             fileUp(terminal, Csocket, usernameC); 
        }
        else if( terminal[0] == "download_file")
        {
            fileDown(terminal, Csocket, usernameC);
        Log("after download");
           
        }
        

      else if(terminal[0] == "login" )
      {
         if (terminal.size() != 3)
          {
       write(Csocket, "Invalid argument count", 22);
        } 
        else 
         {
        int r = validateLogin(terminal);

        if (r < 0) 
        {
            write(Csocket, "Username/password incorrect", 28);
        } 
        else if(r> 0) 
        {
            write(Csocket, "You already have one active session", 35);
        } 
        else 
        {
            write(Csocket, "Login Successful", 16);
            usernameC = terminal[1];
            char data[100];

            read(Csocket, data, 100);
            string peerAddress = string(data);
            ntoport[usernameC] = peerAddress;
        }

        return 0;
}


        }

        else if( terminal[0] == "create_group" )
        {
           if(create_group(terminal, Csocket, usernameC) >-1)
           {
                usergroupC = terminal[1];
                write(Csocket, "Group created", 13);
            }
            else
            {
                write(Csocket, "Group exists", 12);
            }
        }

        else if( terminal[0] ==  "logout" )
        {

           AlreadyLogin[usernameC] = false;

         write(Csocket, "Logout Successful", 17);

          Log("successfully logged out"); 

        }
       
         else if( terminal[0] == "leave_group" )
            leftGroup(terminal, Csocket, usernameC);
        
        else if(terminal[0] == "list_files" )
            files(terminal, Csocket);

         else if(terminal[0] == "list_requests" )
            requests(terminal, Csocket, usernameC);
        
        else if( terminal[0] == "accept_request")
            reqAck(terminal, Csocket, usernameC);
        

    
        else if( terminal[0] == "show_downloads")
            write(Csocket, "Loading...", 10);
        
        else if(terminal[0] == "stop_share" )
            noShare(terminal, Csocket, usernameC);

        else if(terminal[0] == "list_groups" )
            groups(terminal, Csocket);
        
        else if( terminal[0] == "join_group")
            GroupJoin(terminal, Csocket, usernameC);
        
       
        
        else
            write(Csocket, "Invalid command", 16);
        
    }

    Log("threads finshed : socket number (client)" +to_string(Csocket));

    close(Csocket);



  }

