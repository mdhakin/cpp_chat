#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <signal.h>
#include <mutex>
#define MAX_LEN 200
#define NUM_COLORS 6

using namespace std;

bool exit_flag = false;
thread t_send, t_recv;
int client_socket;
string def_col = "\033[0m";
string colors[] = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};

void catch_ctrl_c(int signal);
string color(int code);
int eraseText(int cnt);
void send_message(int client_socket);
void recv_message(int client_socket);

int main(int argc, char *argv[])
{
	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket: ");
		exit(-1);
	}

	struct sockaddr_in client;
	client.sin_family = AF_INET;
	client.sin_port = htons(atoi(argv[2])); // Port no. of server
	client.sin_addr.s_addr = inet_addr(argv[1]);
	;
	bzero(&client.sin_zero, 0);
	if (argc > 2)
	{

		cout << "IP Address of server " << client.sin_addr.s_addr << endl;
		cout << "Port of the server " << client.sin_port << endl;
	}
	if ((connect(client_socket, (struct sockaddr *)&client, sizeof(struct sockaddr_in))) == -1)
	{
		perror("connect: ");
		exit(-1);
	}
	signal(SIGINT, catch_ctrl_c);
	char name[MAX_LEN] = "GUI";
	send(client_socket, name, sizeof(name), 0);
	cout << "==========Crawler control program. By Matthew Hakin. Sep 2022 =============" << endl;
	cout << colors[NUM_COLORS - 1] << "\n\t*********************************************"
		 << "\n"
		 << def_col;
	cout << colors[NUM_COLORS - 1] << "\n\t********                           **********"
		 << "\n"
		 << def_col;
	cout << colors[NUM_COLORS - 1] << "\n\t*******  MISTRAS CRAWLER INTERFACE  *********"
		 << "\n"
		 << def_col;
	cout << colors[NUM_COLORS - 1] << "\n\t********                            **********"
		 << "\n"
		 << def_col;
	cout << colors[NUM_COLORS - 1] << "\n\t*********************************************"
		 << "\n"
		 << def_col;

	thread t1(send_message, client_socket);
	thread t2(recv_message, client_socket);

	t_send = move(t1);
	t_recv = move(t2);

	if (t_send.joinable())
		t_send.join();
	if (t_recv.joinable())
		t_recv.join();

	return 0;
}

// Handler for "Ctrl + C"
void catch_ctrl_c(int signal)
{
	char str[MAX_LEN] = "#exit";
	send(client_socket, str, sizeof(str), 0);
	exit_flag = true;
	t_send.detach();
	t_recv.detach();
	close(client_socket);
	exit(signal);
}

string color(int code)
{
	return colors[code % NUM_COLORS];
}

// Erase text from terminal
int eraseText(int cnt)
{
	char back_space = 8;
	for (int i = 0; i < cnt; i++)
	{
		cout << back_space;
	}
	return 0;
}

// Send message to everyone
void send_message(int client_socket)
{
	while (1)
	{
		cout << colors[1] << def_col;
		char str[MAX_LEN];
		cin.getline(str, MAX_LEN);
		send(client_socket, str, sizeof(str), 0);
		if (strcmp(str, "#exit") == 0 || strcmp(str, "e") == 0)
		{
			exit_flag = true;
			t_recv.detach();
			close(client_socket);
			return;
		}
		if (strcmp(str, "cmd") == 0)
		{
			cout << "\n\t************  MISTRAS CRAWLER INTERFACE  ****************"
				 << "\n"
				 << endl;
			;
			cout << "*****************  List of commands  **********************" << endl;
			cout << "***********************************************************" << endl;
			cout << "                                                          |" << endl;
			cout << "                                                          |" << endl;
			cout << "  -------------------------------------------------------- " << endl;
			cout << "  ----------- **Telemetry Command Reference** ------------ " << endl;
			cout << "  -------------------------------------------------------- " << endl;
			cout << " Command         |   Description                          |" << endl;
			cout << "___________________________________________________________" << endl;
			cout << " mtr1t           |   Motor 1 Temperature                  |" << endl;
			cout << " mtr2t           |   Motor 2 Temperature                  |" << endl;
			cout << " mtr3t           |   Motor 3 Temperature                  |" << endl;
			cout << " mtr4t           |   Motor 4 Temperature                  |" << endl;
			cout << " spd             |   Craweler current speed in ft/sec     |" << endl;
			cout << " dir             |   Craweler current direction           |" << endl;
			cout << " ang             |   Tilt sensor data                     |" << endl;
			cout << " volt            |   Battery Voltage                      |" << endl;
			cout << " err             |   Report errors                        |" << endl;
			cout << " mtrT            |   Motor Torque                         |" << endl;
			cout << " state           |   crawler state                        |" << endl;
			cout << " clicks          |   total clicks from start              |" << endl;
			cout << " mtrmode         |   motor mode                           |" << endl;
			cout << " uptime          |   time on                              |" << endl;
			cout << " info            |   crawler info                         |" << endl;
			cout << " XXX             |   XXXXXXXXXXXXX                        |" << endl;
			cout << " XXX             |   XXXXXXXXXXXXX                        |" << endl;
			cout << " XXX             |   XXXXXXXXXXXXX                        |" << endl;
			cout << " XXX             |   XXXXXXXXXXXXX                        |" << endl;
			cout << " XXX             |   XXXXXXXXXXXXX                        |" << endl;
			cout << " XXX             |   XXXXXXXXXXXXX                        |" << endl;
			cout << "----------------------------------------------------------|" << endl;
			cout << "----------------------------------------------------------|" << endl;
			cout << "-------------- **Control Command Reference** -------------|" << endl;
			cout << "==========================================================|" << endl;
			cout << " mode [args]     | Puts the crawler into a specific mode  |" << endl;
			cout << " -args: [idle] [scan] [calibration] [transit] [safe stop] |" << endl;
			cout << " -response: success or fail                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << " setspd [args]   | Sets the crawler speed                 |" << endl;
			cout << " -args:  [speed]                                          |" << endl;
			cout << " -response: New spped                                     |" << endl;
			cout << "==========================================================|" << endl;
			cout << " cal [args]      | Puts crawler int calibration mode      |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: Success or Fail                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << " stop [   ]      | Stops the crawler                      |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: Success or Fail                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << " Change direction [] | Alternate crawler direction        |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: new direction                                 |" << endl;
			cout << "==========================================================|" << endl;
			cout << " Auto Enable [T or F] | Enables iRad to control crawler   |" << endl;
			cout << " -args: True or False                                     |" << endl;
			cout << " -response: Enabled or Disabled                           |" << endl;
			cout << "==========================================================|" << endl;
			cout << " xxxx [   ]      | XXXXXXXXXXXXXXXXX                      |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: XXXXXXXXXXXXXXX                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << " xxxx [   ]      | XXXXXXXXXXXXXXXXX                      |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: XXXXXXXXXXXXXXX                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << " xxxx [   ]      | XXXXXXXXXXXXXXXXX                      |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: XXXXXXXXXXXXXXX                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << " xxxx [   ]      | XXXXXXXXXXXXXXXXX                      |" << endl;
			cout << " -args:                                                   |" << endl;
			cout << " -response: XXXXXXXXXXXXXXX                               |" << endl;
			cout << "==========================================================|" << endl;
			cout << "**********************************************************|" << endl;
			cout << "**********************************************************|" << endl;
		}
	}
}

// Receive message
void recv_message(int client_socket)
{
	while (1)
	{
		if (exit_flag)
		{
			return;
		}
		char name[MAX_LEN], str[MAX_LEN];
		int color_code;
		int bytes_received = recv(client_socket, name, sizeof(name), 0);
		if (bytes_received <= 0)
		{
			continue;
		}
		cout << def_col;
		cout << name << endl;
		fflush(stdout);
	}
}