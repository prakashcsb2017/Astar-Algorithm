// A C++ Program to implement A* Search Algorithm 
#include "Astarfunctions.h"
#include<iostream>

using namespace std;
// Driver program to test above function 
int main() 
{ 
 /* Description of the Grid- 
	Since every block has 8 possible neighbor, 
	hence 8 bit is used to resemble whether 
	a block has a reachable neighbor or not . If the bit is
	1 means a block is reachable from that particular
	block else if 0 then it is unreachable.
	-> North-west    1st bit
	-> North		 2nd bit
	-> North-East    3rd bit
	-> East          4th bit
	-> South-East    5th bit
	-> South         6th bit
	-> South-West    7th bit
	-> West          8th bit */

	
	int8_t grid[ROW][COL] = 
	{ 
		{ (int8_t)56, (int8_t)248, (int8_t)248, (int8_t)248, (int8_t)248, (int8_t)240, (int8_t)8  , (int8_t)152, (int8_t)184, (int8_t)224 }, 
		{ (int8_t)62, (int8_t)247, (int8_t)15 , (int8_t)143, (int8_t)159, (int8_t)187, (int8_t)249, (int8_t)192, (int8_t)127, (int8_t)227 }, 
		{ (int8_t)62, (int8_t)235, (int8_t)201, (int8_t)152, (int8_t)160, (int8_t)63 , (int8_t)255, (int8_t)253, (int8_t)238, (int8_t)195 }, 
		{ (int8_t)62, (int8_t)231, (int8_t)56 , (int8_t)192, (int8_t)99 , (int8_t)62 , (int8_t)239, (int8_t)223, (int8_t)167, (int8_t)32  }, 
		{ (int8_t)62, (int8_t)243, (int8_t)14 , (int8_t)141, (int8_t)130, (int8_t)126, (int8_t)227, (int8_t)32 , (int8_t)16 , (int8_t)34  }, 
		{ (int8_t)46, (int8_t)219, (int8_t)249, (int8_t)248, (int8_t)252, (int8_t)238, (int8_t)195, (int8_t)98 , (int8_t)34 , (int8_t)34  }, 
		{ (int8_t)54, (int8_t)12 , (int8_t)159, (int8_t)191, (int8_t)255, (int8_t)231, (int8_t)56 , (int8_t)194, (int8_t)98 , (int8_t)34  }, 
		{ (int8_t)10, (int8_t)137, (int8_t)128, (int8_t)127, (int8_t)255, (int8_t)227, (int8_t)62 , (int8_t)237, (int8_t)194, (int8_t)98  }, 
		{ (int8_t)56, (int8_t)248, (int8_t)204, (int8_t)142, (int8_t)143, (int8_t)131, (int8_t)126, (int8_t)247, (int8_t)120, (int8_t)224 },
        { (int8_t)14, (int8_t)143, (int8_t)137, (int8_t)136, (int8_t)136, (int8_t)140, (int8_t)142, (int8_t)143, (int8_t)143, (int8_t)131 } 
	}; 

    Pair ver[9]={ {0,0} , {3,3} , {6,1} , {1,7} , {4,7} , {9,9} , {0,9} , {9,0} , {3,9} };

    char source ,destn,ans;
    while(true){
        cout<<"Enter Source Node : ";
        cin>>source;
        cout<<endl<<"Enter Destination Node : ";
        cin>>destn;
        // Source is the left-most bottom-most corner 
        unsigned temp1 = source - 'A';
        // Destination is the left-most top-most corner 
        unsigned temp2 = destn - 'A';
        cout<<"Using Eucleadian Distance as Heuristics !!"<<endl;
        aStarSearchEucleadian(grid, ver[temp1], ver[temp2]); 
        cout<<"Using Manhattan Distance as Heuristics !!"<<endl;
        aStarSearchManhattan(grid, ver[temp1], ver[temp2]);
        cout<<endl<<"Do you want to continue ? [y/n] : ";
        cin>>ans;
        cout<<endl;
        if(ans == 'n' || ans == 'N')
            break;
    }

	return(0); 
}
