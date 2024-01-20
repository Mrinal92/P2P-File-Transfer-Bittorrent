#include "lib.h"

using namespace std;

typedef struct PeerInfoFile
{
 
  string nameofFile;
 long long Size;
 string Pserv;


} PeerInfoFile;

typedef struct chunkInfo
{
 
 long long IDchunk;
 string target;
 string Pserv;
 string nameofFile;
 
}chunkInfo;

int partSend(char* fileDir, int IDchunk, int Csocket)
{
  
  ifstream inp(fileDir, ios::in|ios::binary);

 inp.seekg(IDchunk*part_size, inp.beg);

 Log(" sending starting from" + to_string(inp.tellg()));
 char data[part_size]={0};

 int codeReturn;

 codeReturn=0;

 string delivered="";

 inp.read(data, sizeof(data));

 int cnt=inp.gcount();

 if ((codeReturn=send(Csocket, data,cnt,0)) == -1) 
   {
        perror("file sending error");
    exit(1);
 }
    
  Log(" to part"+to_string(inp.tellg()));

  inp.close();
  
  return 0;

}

int partWrite(int Psock, long long IDchunk, char* fileDir, int t)
{
    int s;
    int total;

    char data[part_size];

    string stuff;
    stuff="";

    int c =t;

    while(total<part_size)
    {
      s = read(Psock, data, part_size-1);

        if (s < 1)
        {
            break;
        }

        data[s] = 0;

        fstream output(fileDir,fstream::in | fstream::out | fstream::binary);
        output.seekp(IDchunk*part_size+total,ios::beg);
        output.write(data, s);
        output.close();

        Log(" starting location writing : "+ to_string(IDchunk*part_size + total));
        Log(" ending position writing: " + to_string(IDchunk*part_size + total + s-1));
        Log("\n");

        stuff = stuff + data;
        total = total +s;

        bzero(data, part_size);  
    }

    string parthash;
    parthash="";

    StoreHash(stuff, parthash,1);

    int var=2;

    while(var--)
    {
        parthash.pop_back();
    }

    if(parthash!=PartHashOngoing[IDchunk])
    {
        compromised=true;
    }


    string nameofFile = divideStr(string(fileDir),"/",1).back();
    PartSet(nameofFile, IDchunk,IDchunk,false);
    
    return 0;
}

void PartMeta(PeerInfoFile* inf)
{
 Log(" info of :" + inf->nameofFile+" from" +inf->Pserv);

    vector<string> SPAddr = divideStr((string)inf->Pserv, ":",1);

    string cmd = "get_chunk_vector##" + string(inf->nameofFile);
    string response = peerConnect(&SPAddr[0][0], &SPAddr[1][0],cmd,1);

    for(size_t j=0; j<OngoingChunksDown.size(); j++)
    {
        if(response[j]=='1')
        {
            OngoingChunksDown[j].push_back(string(inf->Pserv));
        }
    }

    delete inf;
}

void PartGet(chunkInfo* WantChunk)
{
     Log("Chunk fetching details :" + WantChunk->nameofFile + " " + 
        WantChunk->Pserv + " " + to_string(WantChunk->IDchunk));

    string NameOffile = WantChunk->nameofFile;
    
    vector<string> Pserv = divideStr(WantChunk->Pserv,":",1);

    long long IDchunk = WantChunk->IDchunk;

    string target = WantChunk->target;

    string cmd = "get_chunk##" + NameOffile + "##" + to_string(IDchunk) + "##" + target;
    
    peerConnect(&Pserv[0][0], &Pserv[1][0], cmd,1);
    
    delete WantChunk;
    return;  
}

void SelectPiece(vector<string> terminal, vector<string> peers)
 {
      // inpt = [command, group id, filename, destination]

    long long size =stoll(peers.back());

    peers.pop_back();
    
    long long parts;
    parts = size/part_size + 1;

    OngoingChunksDown.clear();
    OngoingChunksDown.resize(parts);


    Log(" piece selection algorithm ");
    
    vector<thread> thread1;
    vector<thread> thread2;
    
     for(size_t j=0;j<peers.size();j++)
     {
        PeerInfoFile* inf = new PeerInfoFile();

        inf->Pserv = peers[j];
        inf->Size = parts;
        inf->nameofFile = terminal[2];
        
        thread1.push_back(thread(PartMeta,inf));
    }

    for(auto itr=thread1.begin();itr!=thread1.end();itr++)
    {
        if(itr->joinable())
        itr->join();
    }
    
     Log("defualt values put in file");

    for(size_t j=0;j<OngoingChunksDown.size(); j++)
    {
        if(OngoingChunksDown[j].size() ==0)
        {
            cout <<"not all parts there"<<endl;
            return;
        }
    }
    
     thread1.clear();
    srand((unsigned) time(0));
    long long partsGot;
    partsGot=0;


    string target =terminal[3]+"/" +terminal[2];

    FILE* inp =fopen(&target[0],"r+");

	if(inp!=0)
    {
		cout<<" file exists in the first place"<<endl;

        fclose(inp);
        
        return;
	}

     string ss(size,'\0');

    fstream inp1(&target[0], ios::out | ios::binary );

    inp1.write(ss.c_str(),strlen(ss.c_str()));  
    inp1.close();

    partMeta[terminal[2]].resize(parts,0);
    compromised = false;

    vector<int> temp(parts, 0);

    partMeta[terminal[2]] =temp;
    
    string pathPeer;

     while(partsGot<parts)
     {
        Log(" current segment is: " +to_string(partsGot));
        
        long long piece;

        while(true)
        {
            piece = rand() % parts;
            Log("piece is = " + to_string(piece));

            if(partMeta[terminal[2]][piece]==0)
            break;
        }

        long long havePiecePeer = OngoingChunksDown[piece].size();
        string AnyPeer = OngoingChunksDown[piece][rand()%havePiecePeer];

        chunkInfo* Want = new chunkInfo();
        Want->nameofFile =terminal[2];
        Want->Pserv = AnyPeer;
        Want->IDchunk = piece;
        Want->target = terminal[3]+"/"+terminal[2];

        Log("for this part , a thread is given "+ to_string(Want->IDchunk));
        partMeta[terminal[2]][piece] = 1;

        thread2.push_back(thread(PartGet,Want));
        partsGot++;
        pathPeer = AnyPeer;
    }    
    
    for(auto k=thread2.begin();k!=thread2.end();k++)
    {
        if(k->joinable()) 
        k->join();
    } 

    if(compromised)
    {
        cout <<"corrupted file downloaded. Maybe !"<<endl;
    }
    else
    {
         cout<< "no corruption"<<endl;
    }

    AlreadyDown.insert({terminal[2], terminal[1]});

    vector<string> AddServ = divideStr(pathPeer, ":",1);

    peerConnect(&AddServ[0][0],&AddServ[1][0],"get_file_path##" + terminal[2],1);

    return;
 }




int fileDown(vector<string> terminal, int Socket, int t)
{
    if(terminal.size()!=4)
    return 0;

    int c=t;

     string infoFile = "";
    infoFile = infoFile+terminal[2] + "##";

    infoFile = infoFile+  terminal[3] + "##";
    infoFile = infoFile+terminal[1];
    // fileDetails = [filename, destination, group id]
    
    Log("sending file details for download : " + infoFile);

    int len=strlen(&infoFile[0]);

    if(send(Socket ,&infoFile[0],len ,MSG_NOSIGNAL ) == -1)
    {
        cout<<strerror(errno);
        
        return -1;
    }

    char response[524288] = {0}; 

    read(Socket ,response,524288); 

    if(string(response)=="no such file")
    {
        cout <<response<<endl;
        return 0;
    }
    vector<string> PeersHavingFile = divideStr(response, "##",1);
    
    char test[6];
    strcpy(test,"test1");


    write(Socket, test,6);

    bzero(response,524288);

    read(Socket ,response,524288); 

    vector<string> temp = divideStr(string(response), "##",1);
    PartHashOngoing = temp;

    SelectPiece(terminal,PeersHavingFile);

    return 0;
}

 

int fileUp(vector<string> terminal, int Socket, int t)
{
    if(terminal.size()!= 3)
   { 
    return 0;

    }
  
  int g =t;

    string infoFile = "";
    char* fileDir = &terminal[1][0];

    string filename = divideStr((string)fileDir,"/",1).back();

    if(AlreadyUpload[terminal[2]].find(filename) != AlreadyUpload[terminal[2]].end())
    {
        cout<<"already downloaded file" <<endl;

        if(send(Socket ,"error" ,5 , MSG_NOSIGNAL ) == -1)
        {
            cout<<strerror(errno);
            return -1;
        }
        return 0;
    }

    else
    {
        AlreadyUpload[terminal[2]][filename] =true;

        LocalPath[filename] =string(fileDir);
    }

    string HashPiece = Combined_Hash(fileDir, 1);

    if(HashPiece =="#")
    return 0;

    string filehash = sha256full(fileDir,1);
    string size = to_string(size_of_file(fileDir));

    infoFile += string(fileDir) + "##";
    infoFile += string(IP_peer) + ":" + to_string(portno_peer) + "##";
    infoFile += size + "##";
    infoFile += filehash + "##";
    infoFile += HashPiece;
    
    Log("details for uploading file " + infoFile);
    
    int len=strlen(&infoFile[0]);

    if(send(Socket,&infoFile[0] ,len,MSG_NOSIGNAL) ==-1)
    {
        cout<<strerror(errno);
        return -1;
    }
    
    char response[10240] = {0}; 
    read(Socket,response,10240); 

    cout <<response<<endl;
    Log("reply of server for sending file " + string(response));

    PartSet(filename,0,stoll(size)/part_size + 1, true);

    return 0;
}