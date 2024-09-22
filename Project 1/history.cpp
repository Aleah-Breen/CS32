#include <iostream>
#include "History.h"
#include "globals.h"
using namespace std;

History::History(int nRows, int nCols)
 : m_rows(nRows), m_cols(nCols)
{
    for(int i=0;i<MAXROWS;i++){
        for(int j=0;j<MAXCOLS;j++){
            landed[i][j]=-1;
        }
    }
    for(int i=0;i<nRows;i++){
        for(int j=0;j<nCols;j++){
            landed[i][j]=0;
        }
    }
   
}
bool History::record(int r, int c){
    if(r<1 || c<1){
        return false;
    }
    if(r>m_rows || c>m_cols){
        return false;
    }
    landed[r-1][c-1]+=1;
    return true;
}
void History::display() const{
    clearScreen();
    char mappedVals[28] = ".ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i=0; i<m_rows;i++){
        for(int j=0;j<m_cols;j++){
            int numCurrent = landed[i][j];
            if(numCurrent>=26){
                cout << "Z";
            }
            else{
                cout << mappedVals[numCurrent];
            }
        }
        cout << endl;
    }
    cout <<endl;
}
