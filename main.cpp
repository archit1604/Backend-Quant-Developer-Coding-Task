#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <iomanip>
#include <ctime>
#include <deque>

using namespace std;

tm parseDate(const string& dateStr) {   //time parsing function
    tm tm = {};
    istringstream ss(dateStr);
    ss >> get_time(&tm, "%d/%m/%Y");
    

    return tm;
}

// Function to compare two dates
bool isDateBefore(const string& date1, const string& date2) {
    // Parse dates into tm structures
    tm tm1 = parseDate(date1);
    tm tm2 = parseDate(date2);

    // Convert tm to time_t for easy comparison
    time_t time1 = mktime(&tm1);
    time_t time2 = mktime(&tm2);

    // Compare dates
    return difftime(time1, time2) <= 0;
}

int readDayData(const string& filename,string givenDate, string frmDate){ //function to find the 30day average from current date
    ifstream ip(filename);
    if(!ip.is_open()) cout<<"Error: File Open"<<endl;
    string date;
    string stock;
    string vol;

    int n=1;
    int curAvg=0;
    bool flag=false;
    while(ip.good() ){
        getline(ip,date,',');   //to get the date from file
        getline(ip,stock,',');  //to get the stock name from file
        getline(ip,vol,'\n');   //to get the volume of purchase
        if(date==givenDate){
            return curAvg;
        }

        if(date==frmDate) flag=true;
        if(flag){
            curAvg=curAvg+(stoi(vol)-curAvg)/n;
            n++;
        }        
    }    
    return curAvg;
}

void timestamp(const string& filename, int dayAvg){ // Function to find the crossover time-stamp
    ifstream ip(filename);
    if(!ip.is_open()) cout<<"Error: File Open"<<endl;
    string date;
    string timeSt;
    string stock;
    string vol;

    deque<int> mdq;     //created a deque for running window of 60 minutes
    int n=0;
    int sum=0;
    bool flag=false;

    while(ip.good() ){
        getline(ip,date,',');
        getline(ip,timeSt,','); //to get the timestamp of the stock
        getline(ip,stock,',');
        getline(ip,vol,'\n');
        
        if(!flag){
            mdq.push_back(stoi(vol));
            sum+=stoi(vol);
            n++;
            if(n==3600) flag=true;
        }
        
        if(flag){
            if(sum>=dayAvg) {
                cout<<timeSt<<'\n';
                return;
            }
            
            int first=mdq.front();
            mdq.pop_front();
            mdq.push_back(stoi(vol));
            sum+=stoi(vol)-first;
        }
    }
    cout<<"NONE"<<endl; // printing NONE in case there is no time stamp when sum does not increase 30days average
    return;
}


int main() {
    // Driver Code
    int dayavg19=readDayData("SampleDayData.csv","19/04/24","02/03/24");
    int dayavg22=readDayData("SampleDayData.csv","22/04/24","04/03/24");
    cout<<"Time Stamp for 19th April is == ";
    timestamp("19thAprilSampleData.csv",dayavg19);
    cout<<"Time Stamp for 22nd April is == ";
    timestamp("22ndAprilSampleData.csv",dayavg22);
    
    return 0;
}
