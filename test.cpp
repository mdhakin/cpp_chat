#include <bits/stdc++.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


using namespace std;


enum appState
{
	idle,
	scanning,
	calibrating
};

string stats[] = { "sta=","upt=","spd=","dir=","faa","raa","m1t","m2t=","m3t=","m4t=","til=","err=" };

int main(){
	
    char arr[stats[0].length() + 1]; 
    strcpy(arr, stats[0].c_str());
   if(strcmp(arr,"sta=")==0)
   {
        cout << "Success?" << endl;
   } 
cout << stats[1] << "idle" << endl;

	return 0;
}