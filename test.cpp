#include <bits/stdc++.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <iostream>

using namespace std;

struct telemitry
{
     string Name;
     bool isFloat;
     int value;
     float fValue;
     long timestamp;
};

enum appState
{
	idle,
	scanning,
	calibrating
};
long timeit();
//string stats[] = { "sta=","upt=","spd=","dir=","faa","raa","m1t","m2t=","m3t=","m4t=","til=","err=" };

int main(){
	
     /* string relay = stats[3] + " status \n";
     telemitry *read = new telemitry();
     string ff = "";
     read->isFloat = false;
     read->value = 255;
     read->Name = "matthew";
     read->timestamp = 150000021;
     char arr[stats[0].length() + 1]; 
     strcpy(arr, stats[0].c_str());
     if(strcmp(arr,"sta=")==0)
     {
          cout << "Success?" << endl;
     } 
     cout << read->Name << relay << "\n" << endl; */
     


     namespace sc = std::chrono;
     auto time = sc::system_clock::now();
     auto since_epoch = time.time_since_epoch();
     auto millis = sc::duration_cast<sc::milliseconds>(since_epoch);
     long now = millis.count();


	return 0;
}

long timeit()
{
    
     return now;
}

void loopforawhile()
{
long ct = 0;
     while(1)
     {
          long val = timeit();
          long diff = val - ct;
          if (diff > 1000000)
          {
               break;
          }
          ct = val;
     }
     cout << ct << endl;
}


class terminalDisplay
{

     public:
     
     string textToDisplay;
     string formatText(string iput);

}

terminalDisplay::formatText(string iput)
{
     
}