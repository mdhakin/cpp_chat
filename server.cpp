#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#define MAX_LEN 200
#define NUM_COLORS 6
using namespace std;


struct terminal{
	int id;
	string name;
	int socket;
	thread th;
};

vector<terminal> clients;
string def_col="\033[0m";
string colors[]={"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m","\033[36m"};
int seed=0;
mutex cout_mtx,clients_mtx;

string color(int code);
void set_name(int id, char name[]);
void shared_print(string str, bool endLine);
int broadcast_message(string message, int sender_id);
int broadcast_message(int num, int sender_id);
void end_connection(int id);
void handle_client(int client_socket, int id);
int broadcast_message_to_sender(string message, int sender_id);
int broadcast_message_to_sender(int num, int sender_id);
void sendResponse(string message, int sender_id);

int main(int argc, char *argv[]){
	
	if(argc < 3)
	{
		cout << "IP Address or Port was invalid." << endl;
		return 1;
	}

	int server_socket;
	if((server_socket=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in server;
	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[2]));
	server.sin_addr.s_addr= inet_addr (argv[1]);
	bzero(&server.sin_zero,0);

	if((bind(server_socket,(struct sockaddr *)&server,sizeof(struct sockaddr_in)))==-1)
	{
		perror("bind error: ");
		exit(-1);
	}
	if((listen(server_socket,8))==-1)
	{
		perror("listen error: ");
		exit(-1);
	}


	if(argc > 2)
     {
          
          cout << "IP Address of server " << server.sin_addr.s_addr << endl;
          cout << "Port of the server " << server.sin_port << endl;
     }
	struct sockaddr_in client;
	int client_socket;
	unsigned int len=sizeof(sockaddr_in);
	cout << colors[NUM_COLORS-1]<<"\n\t*************************************************"<<"\n"<<def_col;
	cout << colors[NUM_COLORS-1]<<"\n\t********                              ***********"<<"\n"<<def_col;
	cout << colors[NUM_COLORS-1]<<"\n\t*******  MISTRAS NDT CRAWLER INTERFAC  **********"<<"\n"<<def_col;
	cout << colors[NUM_COLORS-1]<<"\n\t********                              ***********"<<"\n"<<def_col;
	cout << colors[NUM_COLORS-1]<<"\n\t*************************************************"<<"\n"<<def_col;

	while(true){
		if((client_socket=accept(server_socket,(struct sockaddr *)&client,&len))==-1)
		{
			perror("accept error: ");
			exit(-1);
		}

		seed++;
		thread t(handle_client,client_socket,seed);
		lock_guard<mutex> guard(clients_mtx);
		clients.push_back({seed, string("Anonymous"),client_socket,(move(t))});
	}

	for(int i=0; i<clients.size(); i++){
		if(clients[i].th.joinable()) clients[i].th.join();
	}

	close(server_socket);
	return 0;
}

string color(int code)
{
	return colors[code%NUM_COLORS];
}

// Set name of client
void set_name(int id, char name[])
{
	for(int i=0; i<clients.size(); i++){
			if(clients[i].id==id) clients[i].name=string(name);		
	}	
}

// For synchronisation of cout statements
void shared_print(string str, bool endLine=true)
{	
	lock_guard<mutex> guard(cout_mtx);
	cout<<str;
	if(endLine) cout<<endl;
}

// Broadcast message to all clients except the sender
int broadcast_message(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++)
	{
		if(clients[i].id!=sender_id) send(clients[i].socket,temp,sizeof(temp),0);
	}
	return 0;		
}

// Broadcast message to the sender
int broadcast_message_to_sender(string message, int sender_id)
{
	char temp[MAX_LEN];
	strcpy(temp,message.c_str());
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id==sender_id) 
		{
			send(clients[i].socket,temp,sizeof(temp),0);
		}
	}	
	return 0;	
}

// Broadcast a number to the sender
int broadcast_message_to_sender(int num, int sender_id)
{
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id==sender_id) 
		{
			send(clients[i].socket,&num,sizeof(num),0);
		}
	}	
	return 0;	
}

// Broadcast a number to all clients except the sender
int broadcast_message(int num, int sender_id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id!=sender_id) send(clients[i].socket,&num,sizeof(num),0);
		
	}	
	return 0;	
}

void end_connection(int id){
	for(int i=0; i<clients.size(); i++){
		if(clients[i].id==id){
			lock_guard<mutex> guard(clients_mtx);
			clients[i].th.detach();
			clients.erase(clients.begin()+i);
			close(clients[i].socket);
			break;
		}
	}				
}

void handle_client(int client_socket, int id){
	char name[MAX_LEN],str[MAX_LEN];
	recv(client_socket,name,sizeof(name),0);
	set_name(id,name);	

	// Display welcome message
	string welcome_message=string(name)+string(" has joined");
	broadcast_message("#NULL",id);	
	broadcast_message(id,id);								
	broadcast_message(welcome_message,id);	
	shared_print(color(id)+welcome_message+def_col);
	
	while(true){
		int bytes_received=recv(client_socket,str,sizeof(str),0);
		
		if(bytes_received<=0)
			return;
		
		if(strcmp(str,"#exit")==0 || strcmp(str,"e")==0){
			// Display leaving message
			string message=string(name)+string(" has left");		
			shared_print(color(id)+message+def_col);
			end_connection(id);							
			return;
		}
		sendResponse((string)str, id);
		shared_print(color(id)+name+" : "+def_col+str);	
	}	
}

void sendResponse(string message, int sender_id)
{
	char temp1[MAX_LEN];
	strcpy(temp1,message.c_str());
	string response = "-No Data-";
	if (strcmp(temp1,"mtr1t")==0)
	{
		response = "140";
	}else if (strcmp(temp1,"mtr2t")==0)
	{
		response = "180";
	}else if (strcmp(temp1,"mtr3t")==0)
	{
		response = "110";
	}else if (strcmp(temp1,"mtr4t")==0)
	{
		response = "280";
	}else if (strcmp(temp1,"spd")==0)
	{
		response = "3 in/sec";
	}else if (strcmp(temp1,"dir")==0)
	{
		response = "fwd";
	}else if (strcmp(temp1,"ang")==0)
	{
		response = "2 degrees";
	}else if (strcmp(temp1,"volt")==0)
	{
		response = "51.56 volt";
	}else if (strcmp(temp1,"fax")==0)
	{
		response = "+3 degrees";
	}else if (strcmp(temp1,"rax")==0)
	{
		response = "-2 degrees";
	}else if (strcmp(temp1,"state")==0)
	{
		response = "scanning";
	}else if (strcmp(temp1,"clicks")==0)
	{
		response = "21523";
	}else if (strcmp(temp1,"mtrmode")==0)
	{
		response = "velocity";
	}else if (strcmp(temp1,"uptime")==0)
	{
		response = "2345 seconds";
	}else if (strcmp(temp1,"cmd")==0)
	{
		response = "\n>>mtr1t \t [motor 1 temp]\n>>mtr2t \t [motor temp]\n>>mtr3t \t [motor 3 temp]\n>>mtr4t \t [motor 4 temp]\n>>spd \t\t [speed]\n>>dir \t\t [direction]\n>>ang \t\t [crawler angle]\n>>volt \t\t [Batter Voltage]\n";
	}

	char temp[MAX_LEN];
	strcpy(temp,response.c_str());
	for(int i=0; i<clients.size(); i++)
	{
		char tName[MAX_LEN];
		strcpy(tName,clients[i].name.c_str());
		broadcast_message_to_sender(string(temp),sender_id);
		shared_print(color(sender_id)+tName+" : "+def_col+temp);	
	}
			
}
