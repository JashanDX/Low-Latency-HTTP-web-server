#include <iostream>
using namespace std;
int main(){
    int uts;
    double bill=0.0, total=0.0;
    cout << "Enter the number of units consumed: ";
    cin >> uts;
    if (uts < 0){
        cout << "Invalid input. Please enter a non-negative number of units.\n";
    } 
    if (uts < 100){
        bill=0;
        total=bill+50;
    }
    else if (uts > 200){
        bill=60*uts;
        total=bill+50;
    }
    else if (uts <300){
        bill=80*uts;
        total=bill+50;
    }
    else{
        bill=90*uts;
        total=bill + (bill*0.15);
    }
    cout << "Your pay\t"<< total <<"paise"<< endl;
    return 0;

}