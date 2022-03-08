#ifndef CONNECTIVITY_MATRIX_H_INCLUDED
#define CONNECTIVITY_MATRIX_H_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include "connectivity_matrix.h"


using namespace std;


struct net{
    string netName;          //contains the net name
    string connections = " ";//contains the connection data of the current net
};
// a struct to save each pin information
struct pin{
    string pinN;
    int pinNo;
};
// a struct to save the connectivity Matrix
struct connectiviyMatrix{
    string cellName = "NULL";
    vector <int> connections;
    bool cellFixed = false;
};



int connectiviy_matrix(string pinNetlistFile)
{
	int matrix[100][100] = {0};
    string pinName;
    string prevString = "NULL"; //previous string, used in the large loop
    string currString;          //current string, used in the large loop
    string dummyString;
    string netNo = "N1";        //the number of the net to search for
    int netIndex = 1;           //index of the nets in the struct
    int newLine = 0;            //used to know if a new line started in the input .txt file
    int netFound = 0;           //gives 1 when the net to search for is found.
                                // if a complete loop is done and netFound is still 0, the large loop terminates,
                                // meaning that all nets were found

    // Calling the file
    ifstream myfile;
    myfile.open(pinOriented.txt);

    int n = 0; // estimating the number of nets in the file
    int cells = 0; // number of cells in the file
    while(myfile)//getting the file length
    {
        getline(myfile, dummyString);
        n++;
    }
    cells = n;
    // ROUGH ESTIMATE FOR n (number of nets)
    n = 2*n;
    myfile.close();
    // initializing a struct with an estimated number of nets.
    net netInstance[n];
    pin pinInstance[n];
    int counter=0;
// the large loop
 while(netIndex<=n)
 {
    myfile.open("pinOriented.txt");
    while(myfile)
    {
        myfile >> currString;
        if(currString == prevString) break;
        // getting the pin name
        if(currString[0]=='(')
        {
            currString.erase(remove(currString.begin(), currString.end(), '('), currString.end());
            currString.erase(remove(currString.begin(), currString.end(), ':'), currString.end());
            pinName = currString;
            newLine = 1;
            pinInstance[counter].pinN = pinName;
            pinInstance[counter].pinNo = counter;
            if(counter < n-1)
            {
            	counter++;
			}
        }
        else
        {
            currString.erase(remove(currString.begin(), currString.end(), ')'), currString.end());
            currString.erase(remove(currString.begin(), currString.end(), ','), currString.end());

            if(currString == netNo) //getting the connections data
            {
                netFound = 1;
                if(prevString == "NULL")
                {
                    netInstance[netIndex-1].netName = currString;
                    netInstance[netIndex-1].connections += pinName+" ";
                }
                else
                {
                    netInstance[netIndex-1].netName = currString;
                    netInstance[netIndex-1].connections += pinName+" ";
                }
            }
        }
        if(newLine == 1) //resetting variables, since a new line started
        {
            prevString = "NULL";
            newLine = 0;
        }
        else {prevString = currString;} //updating prevString
    }
    myfile.close();
    if(netFound == 0) break; //ending the large loop condition
    // Updating the net to find
    netIndex++;
    netFound = 0;
    stringstream ss;
    ss << netIndex; //converting the integer to a string
    ss >> dummyString;
    netNo = "N"+dummyString;

 } //the loop terminates when no net is found, which means all nets are found since they are named in increasing order
   // so when the loop terminates, netIndex = nets found + 1, meaning the last net is N(netIndex-1).

 // outputting the file NET-ORIENTED
 ofstream outfile ("netOriented.txt");
    if(outfile.is_open())
    {
        for(int k=0; k<netIndex-1; k++)
        {
            outfile << "(" + netInstance[k].netName + ":" + netInstance[k].connections + ")";
            outfile << "\n";
        }
    }
    outfile.close();

    ////////////////////////////////THE CONNECTIVITY MATRIX CODE///////////////////////////////////
    string dummyString2;
    int stop = 2;   //USED AS THE STOPPING CONDITION FOR THE WHILE LOOP
                    //AND ALSO TO GET TO THE REQUIRED WORD IN ONE LINE OF NET-ORIENTED FILE

ifstream file;

for(int i=0; i<netIndex-1; i++)
{
    stop = 1;

	while(stop != -1)
	{
        stop++;
        dummyString = "NULL";
		file.open("netOriented.txt");
		for(int r=0; r<i; r++)
		{
			getline(file, dummyString2); //GETTING TO THE REQUIRED LINE
		}
		for(int m=0; m<stop; m++)
		{
			file>>dummyString;   // GETTING TO THE REQUIRED WORD
			if(dummyString == ")")
            {
                stop = -1;  //STOPPING CONDITION
                break;
            }
		}


		for(int j =0; j<n; j++) //TWO LOOPS CHEKING PINS TOGETHER
		{
			if(dummyString==pinInstance[j].pinN)
			{
				file>>dummyString;
				if(dummyString == ")")
				{
					break;
				}
				for(int k=0; k<n; k++)
				{
					if(dummyString==pinInstance[k].pinN)
					{
						matrix[j][k]++;
						matrix[k][j]++;
						file>>dummyString;
						k = 0;
						if(dummyString == ")")
						{
							break;
						}
					}
				}
			}

			if(dummyString == ")")
						{
							break;
						}
		}

		file.close();
	}
}
/// new things from here
n = cells;
connectiviyMatrix connectiviyMatrixInstance[n];
/// SAVING THE MATRIX
for(int i=0; i<n-1; i++)
{
    connectiviyMatrixInstance[i].cellName = pinInstance[i].pinN;
}


for(int i=0; i<n-1; i++)
{
	for(int j=0; j<n-1; j++)
	{
	    connectiviyMatrixInstance[i].connections.push_back(matrix[i][j]);
	}
}


///------------------------------------///------------------------------------------------///-----------------------------------
///     INITIAL PARTITIONONG
///------------------------------------///
    int cellsNo = n-1;  // number of cells in the netlist
    int cellsInPartition = cellsNo/2; // number of cells in each partition
    connectiviyMatrix partitionA[cellsInPartition], partitionB[cellsInPartition], dummyStruct;


    for(int i=0;i<cellsInPartition;i++)
    {
        partitionA[i] = connectiviyMatrixInstance[i];
        partitionB[i] = connectiviyMatrixInstance[i+cellsInPartition];
    }


///-------------------------------------///
///     FINDING A BETTER PARTITION
///-------------------------------------///
struct cellGain{
    int gainA = 0;
    int gainB = 0;
    int maxGain = 0;
    int swapA, swapB;
};
int cyclesCounter = 0;
int Gain = 0; // gain of a swap
int gainMax = 0; //contains the maximum gain to execute the a single swap
int maxGainCycle = 0; // counter to stop the loop of gain calculation
int totalGain = 0; // used to calculate the max gain in one cycle
int totalGainMax; // contains the max. gain in the whole iteration according to the swaps executed.
int cumulativeGain = 0; //the cumulative gain, when it is less than zero the the whole iteration stops.
int swapsToExecute = 0; // contains the number of swaps to be executed
int reverseSwap = 0; //cellsInPartition - swapsToExecute, which is the no. of swaps to be reversed.
int aSwap = 0; // will take the value of swapA just to make the code easier to read, since swapA is in a struct
int bSwap = 0; // the same as aSwap
bool firstSwap = true; // true if it is the first swap in the small iteration
cellGain cellGainInstance[cellsInPartition];

while(totalGainMax > 0)
{
    //if(cyclesCounter == 0 ){cumulativeGain = 0;}
    totalGainMax = 0;
    while(maxGainCycle < cellsInPartition)
    {
        ///double for loops to find the gain of each cell
        for(int i=0; i<cellsInPartition; i++)
        {
            cellGainInstance[i].gainA = 0;
            cellGainInstance[i].gainB = 0;
            for(int j=0; j<cellsInPartition; j++)
            {
                if(partitionA[i].cellFixed == false) // calculating gain for partition A cell
                {
                    cellGainInstance[i].gainA+= partitionB[j].connections.at(i) - partitionA[j].connections.at(i);
                }
                if(partitionB[i].cellFixed == false) // calculating gain for partition B cell
                {
                    cellGainInstance[i].gainB+= partitionA[j].connections.at(i+cellsInPartition) - partitionB[j].connections.at(i+cellsInPartition);
                }
            }
        }
        ///double for loops to find the max gain of each swap
        firstSwap = true;
        for(int m=0; m<cellsInPartition; m++)
        {
            if(partitionA[m].cellFixed == true) continue; // jumping to the next loop iteration if the cell is fixed.

            for(int l=0; l<cellsInPartition; l++)
            {
                if(partitionB[l].cellFixed == true) continue; // jumping to the next loop iteration if the cell is fixed.

                Gain = cellGainInstance[m].gainA + cellGainInstance[l].gainB - 2*partitionA[m].connections.at(l+cellsInPartition);

                if(Gain>gainMax || firstSwap==true)
                {
                    gainMax = Gain;
                    cellGainInstance[maxGainCycle].maxGain = Gain;
                    cellGainInstance[maxGainCycle].swapA = m; cellGainInstance[maxGainCycle].swapB = l;
                    aSwap = m; bSwap = l;
                    firstSwap = false;
                }
            }
        }
        ///executing the swaps and updating fixing condition and maxGainCycle
        if(maxGainCycle<cellsInPartition)
        {
            dummyStruct = partitionA[aSwap]; // swapping the two cells
            partitionA[aSwap] = partitionB[bSwap];
            partitionB[bSwap] = dummyStruct;
            for(int k=0; k<cellsInPartition; k++) /// for updating the connectivity matrix after swapping cells
            {
                swap(partitionA[k].connections.at(aSwap),partitionA[k].connections.at(bSwap+cellsInPartition));
                swap(partitionB[k].connections.at(aSwap),partitionB[k].connections.at(bSwap+cellsInPartition));
            }
            //updating fixed cells condition
            partitionA[aSwap].cellFixed = true;
            partitionB[bSwap].cellFixed = true;
            // updating the total gain
            totalGain+= cellGainInstance[maxGainCycle].maxGain;
            // updating the counter
            maxGainCycle++;


        }
    }
    //resetting the counter and releasing the fix on all cells for starting a new iteration
    maxGainCycle = 0;
    for(int y=0; y<cellsInPartition; y++)
    {
        partitionA[y].cellFixed = false;
        partitionB[y].cellFixed = false;
    }
    cyclesCounter++;
    /* here we should decide which swaps (check max gain in each cellGain instance)*/
    for(int i=0; i<cellsInPartition; i++)
    {
        totalGain = 0;
        for(int j=0; j<=i; j++)
        {
            totalGain+= cellGainInstance[j].maxGain;
        }
        if(i==0){totalGainMax = totalGain; swapsToExecute = 1;}
        else
        {
            if(totalGain>totalGainMax){totalGainMax = totalGain; swapsToExecute = i+1;}
        }
    }
    /*  to be executed  (reverse the swaps after the max gain)*/
    reverseSwap = cellsInPartition-swapsToExecute;
    for(int i=0; i<reverseSwap; i++)
    {
        aSwap = cellGainInstance[cellsInPartition-i-1].swapA;
        bSwap = cellGainInstance[cellsInPartition-i-1].swapB;
        dummyStruct = partitionA[aSwap]; // get the swaps from the struct
        partitionA[aSwap] = partitionB[bSwap];
        partitionB[bSwap] = dummyStruct;
        for(int k=0; k<cellsInPartition; k++) /// for updating the connectivity matrix after swapping cells
        {
            swap(partitionA[k].connections.at(aSwap),partitionA[k].connections.at(bSwap+cellsInPartition));
            swap(partitionB[k].connections.at(aSwap),partitionB[k].connections.at(bSwap+cellsInPartition));
        }
    }
    /*  then we calculate the total gain  (update the total gain variable)
    *   it is cumulativeGain
    */
    cumulativeGain+= totalGainMax;

    if(totalGainMax == 0)
    {
        /// DISPLAYING THE EACH PARTITIONING CYCLE
        cout<<"\t\tKL ALGORITHM";
        cout<<endl<<"----------------------------------------------------------------------";
        cout<<endl<<"PARTITIONING AFTER "<<cyclesCounter<<" ITERATIONS,"<<" Cumulative Gain = "<<cumulativeGain<<endl;;
        cout<<"Partition A:"<<endl;
        for(int i=0; i<cellsInPartition; i++)
        {
            cout<<"\t"<<partitionA[i].cellName;
        }

        cout<<endl;
        cout<<"Partition B:"<<endl;
        for(int i=0; i<cellsInPartition; i++)
        {
            cout<<"\t"<<partitionB[i].cellName;
        }
        cout<<endl<<"----------------------------------------------------------------------"<<endl;
    }


}

    return 0;
}

#endif // CONNECTIVITY_MATRIX_H_INCLUDED
