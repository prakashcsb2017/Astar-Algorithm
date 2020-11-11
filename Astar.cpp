// A C++ Program to implement A* Search Algorithm 
#include<bits/stdc++.h> 
using namespace std; 

#define ROW 10 
#define COL 10 

// Creating a shortcut for int, int pair type 
typedef pair<int, int> Pair; 

// Creating a shortcut for pair<int, pair<int, int>> type 
typedef pair<double, pair<int, int>> pPair; 

// A structure to hold the neccesary parameters 
struct cell 
{ 
	// Row and Column index of its parent 
	// Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
	int parent_i, parent_j; 
	// f = g + h 
	double f, g, h; 
}; 

// A Utility Function to check whether given cell (row, col) 
// is a valid cell or not. 
bool isValid(int row, int col) 
{ 
	// Returns true if row number and column number 
	// is in range 
	return (row >= 0) && (row < ROW) && 
		(col >= 0) && (col < COL); 
} 

// A Utility Function to check whether destination cell has 
// been reached or not 
bool isDestination(int row, int col, Pair dest) 
{ 
	if (row == dest.first && col == dest.second) 
		return (true); 
	else
		return (false); 
} 

// A Utility Function to calculate the 'h' heuristics. 
// Heuristics 1 . Eucleadian Distance
double calculateHValue(int row, int col, Pair dest) 
{ 
	// Return using the distance formula 
	return ((double)sqrt ((row-dest.first)*(row-dest.first) 
						+ (col-dest.second)*(col-dest.second))); 
} 
// Heuristics 2. Manhattan Distance
double calculateHValueManhattan(int row, int col, Pair dest) 
{ 
	// Return using the distance formula 
	return ((double)abs((row-dest.first) + (col-dest.second))); 
} 

// A Utility Function to trace the path from the source 
// to destination 
void tracePath(cell cellDetails[][COL], Pair dest, float cost) 
{ 
	printf ("\nThe Path is "); 
	int row = dest.first; 
	int col = dest.second;
    unsigned int nodes = 0;  

	stack<Pair> Path; 

	while (!(cellDetails[row][col].parent_i == row 
			&& cellDetails[row][col].parent_j == col )) 
	{ 
		Path.push (make_pair (row, col)); 
		int temp_row = cellDetails[row][col].parent_i; 
		int temp_col = cellDetails[row][col].parent_j; 
		row = temp_row; 
		col = temp_col; 
	} 

	Path.push (make_pair (row, col)); 
	while (!Path.empty()) 
	{ 
		pair<int,int> p = Path.top(); 
		Path.pop();
        nodes++; 
		printf("-> (%d,%d) ",p.first,p.second); 
	} 
    cout<<endl<<"Number of nodes visited : "<<nodes<<endl;
    cout<<"Cost : "<<cost<<endl<<endl;
	return; 
} 

// A Function to find the shortest path between 
// a given source cell to a destination cell according 
// to A* Search Algorithm 
void aStarSearchEucleadian(int8_t grid[][COL], Pair src, Pair dest) 
{ 
	// If the source is out of range 
	if (isValid (src.first, src.second) == false) 
	{ 
		printf ("Source is invalid\n"); 
		return; 
	} 

	// If the destination is out of range 
	if (isValid (dest.first, dest.second) == false) 
	{ 
		printf ("Destination is invalid\n"); 
		return; 
	} 

	// Either the source or the destination is blocked 
	/*if (isUnBlocked(grid, src.first, src.second) == false || 
			isUnBlocked(grid, dest.first, dest.second) == false) 
	{ 
		printf ("Source or the destination is blocked\n"); 
		return; 
	}*/ 

	// If the destination cell is the same as source cell 
	if (isDestination(src.first, src.second, dest) == true) 
	{ 
		printf ("We are already at the destination\n"); 
		return; 
	} 

	// Create a closed list and initialise it to false which means 
	// that no cell has been included yet 
	// This closed list is implemented as a boolean 2D array 
	bool closedList[ROW][COL]; 
	memset(closedList, false, sizeof (closedList)); 

	// Declare a 2D array of structure to hold the details 
	//of that cell 
	cell cellDetails[ROW][COL]; 

	int i, j; 

	for (i=0; i<ROW; i++) 
	{ 
		for (j=0; j<COL; j++) 
		{ 
			cellDetails[i][j].f = FLT_MAX; 
			cellDetails[i][j].g = FLT_MAX; 
			cellDetails[i][j].h = FLT_MAX; 
			cellDetails[i][j].parent_i = -1; 
			cellDetails[i][j].parent_j = -1; 
		} 
	} 

	// Initialising the parameters of the starting node 
	i = src.first, j = src.second; 
	cellDetails[i][j].f = 0.0; 
	cellDetails[i][j].g = 0.0; 
	cellDetails[i][j].h = 0.0; 
	cellDetails[i][j].parent_i = i; 
	cellDetails[i][j].parent_j = j; 

	/* 
	Create an open list having information as- 
	<f, <i, j>> 
	where f = g + h, 
	and i, j are the row and column index of that cell 
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
	This open list is implenented as a set of pair of pair.*/
	set<pPair> openList; 

	// Put the starting cell on the open list and set its 
	// 'f' as 0 
	openList.insert(make_pair (0.0, make_pair (i, j))); 

	// We set this boolean value as false as initially 
	// the destination is not reached. 
	bool foundDest = false; 

	while (!openList.empty()) 
	{ 
		pPair p = *openList.begin(); 

		// Remove this vertex from the open list 
		openList.erase(openList.begin()); 

		// Add this vertex to the closed list 
		i = p.second.first; 
		j = p.second.second; 
		closedList[i][j] = true; 
	
	/* 
		Generating all the 8 successor of this cell 

			   N.W N N.E 
			     \ | / 
			      \|/ 
			W----Cell----E 
				  /|\ 
			     / | \ 
			    S.W S S.E 

		Cell-->Popped Cell (i, j) 
		N --> North	 (i-1, j) 
		S --> South	 (i+1, j) 
		E --> East	 (i, j+1) 
		W --> West		 (i, j-1) 
		N.E--> North-East (i-1, j+1) 
		N.W--> North-West (i-1, j-1) 
		S.E--> South-East (i+1, j+1) 
		S.W--> South-West (i+1, j-1)*/

		// To store the 'g', 'h' and 'f' of the 8 successors 
		double gNew, hNew, fNew; 

		//----------- 1st Successor (North) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i-1, j) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i-1, j, dest) == true && (grid[i][j] & 2) == 2) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i-1][j].parent_i = i; 
				cellDetails[i-1][j].parent_j = j; 
                double cost = cellDetails[i][j].f ;
				printf ("The destination cell is found\n"); 
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i-1][j] == false && 
					(grid[i][j] & 2) == 2) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValue (i-1, j, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i-1][j].f == FLT_MAX || 
						cellDetails[i-1][j].f > fNew) 
				{ 
					openList.insert( make_pair(fNew, 
											make_pair(i-1, j))); 

					// Update the details of this cell 
					cellDetails[i-1][j].f = fNew; 
					cellDetails[i-1][j].g = gNew; 
					cellDetails[i-1][j].h = hNew; 
					cellDetails[i-1][j].parent_i = i; 
					cellDetails[i-1][j].parent_j = j; 
				} 
			} 
		} 

		//----------- 2nd Successor (South) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i+1, j) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i+1, j, dest) == true && (grid[i][j] & 32) == 32) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i+1][j].parent_i = i; 
				cellDetails[i+1][j].parent_j = j; 
				printf("The destination cell is found\n");
                double cost = cellDetails[i][j].f ; 
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i+1][j] == false && 
					(grid[i][j] & 32) == 32) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValue(i+1, j, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i+1][j].f == FLT_MAX || 
						cellDetails[i+1][j].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, make_pair (i+1, j))); 
					// Update the details of this cell 
					cellDetails[i+1][j].f = fNew; 
					cellDetails[i+1][j].g = gNew; 
					cellDetails[i+1][j].h = hNew; 
					cellDetails[i+1][j].parent_i = i; 
					cellDetails[i+1][j].parent_j = j; 
				} 
			} 
		} 

		//----------- 3rd Successor (East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid (i, j+1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j+1, dest) == true && (grid[i][j] & 8) == 8) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i][j+1].parent_i = i; 
				cellDetails[i][j+1].parent_j = j; 
				printf("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j+1] == false && 
					(grid[i][j] & 8) == 8) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValue (i, j+1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i][j+1].f == FLT_MAX || 
						cellDetails[i][j+1].f > fNew) 
				{ 
					openList.insert( make_pair(fNew, 
										make_pair (i, j+1))); 

					// Update the details of this cell 
					cellDetails[i][j+1].f = fNew; 
					cellDetails[i][j+1].g = gNew; 
					cellDetails[i][j+1].h = hNew; 
					cellDetails[i][j+1].parent_i = i; 
					cellDetails[i][j+1].parent_j = j; 
				} 
			} 
		} 

		//----------- 4th Successor (West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i, j-1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j-1, dest) == true && (grid[i][j] & 128) == 128) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i][j-1].parent_i = i; 
				cellDetails[i][j-1].parent_j = j; 
				printf("The destination cell is found\n");
                double cost = cellDetails[i][j].f ; 
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j-1] == false && 
					(grid[i][j] & 128) == 128) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValue(i, j-1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i][j-1].f == FLT_MAX || 
						cellDetails[i][j-1].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, 
										make_pair (i, j-1))); 

					// Update the details of this cell 
					cellDetails[i][j-1].f = fNew; 
					cellDetails[i][j-1].g = gNew; 
					cellDetails[i][j-1].h = hNew; 
					cellDetails[i][j-1].parent_i = i; 
					cellDetails[i][j-1].parent_j = j; 
				} 
			} 
		} 

		//----------- 5th Successor (North-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i-1, j+1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i-1, j+1, dest) == true && (grid[i][j] & 4) == 4) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i-1][j+1].parent_i = i; 
				cellDetails[i-1][j+1].parent_j = j; 
				printf ("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i-1][j+1] == false && 
					(grid[i][j] & 4) == 4) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValue(i-1, j+1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i-1][j+1].f == FLT_MAX || 
						cellDetails[i-1][j+1].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, 
									make_pair(i-1, j+1))); 

					// Update the details of this cell 
					cellDetails[i-1][j+1].f = fNew; 
					cellDetails[i-1][j+1].g = gNew; 
					cellDetails[i-1][j+1].h = hNew; 
					cellDetails[i-1][j+1].parent_i = i; 
					cellDetails[i-1][j+1].parent_j = j; 
				} 
			} 
		} 

		//----------- 6th Successor (North-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid (i-1, j-1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination (i-1, j-1, dest) == true && (grid[i][j] & 1) == 1) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i-1][j-1].parent_i = i; 
				cellDetails[i-1][j-1].parent_j = j; 
				printf ("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i-1][j-1] == false && 
					(grid[i][j] & 1) == 1) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValue(i-1, j-1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i-1][j-1].f == FLT_MAX || 
						cellDetails[i-1][j-1].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, make_pair (i-1, j-1))); 
					// Update the details of this cell 
					cellDetails[i-1][j-1].f = fNew; 
					cellDetails[i-1][j-1].g = gNew; 
					cellDetails[i-1][j-1].h = hNew; 
					cellDetails[i-1][j-1].parent_i = i; 
					cellDetails[i-1][j-1].parent_j = j; 
				} 
			} 
		} 

		//----------- 7th Successor (South-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i+1, j+1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i+1, j+1, dest) == true && (grid[i][j] & 16) == 16) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i+1][j+1].parent_i = i; 
				cellDetails[i+1][j+1].parent_j = j; 
				printf ("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i+1][j+1] == false && 
					(grid[i][j] & 16) == 16) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValue(i+1, j+1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i+1][j+1].f == FLT_MAX || 
						cellDetails[i+1][j+1].f > fNew) 
				{ 
					openList.insert(make_pair(fNew, 
										make_pair (i+1, j+1))); 

					// Update the details of this cell 
					cellDetails[i+1][j+1].f = fNew; 
					cellDetails[i+1][j+1].g = gNew; 
					cellDetails[i+1][j+1].h = hNew; 
					cellDetails[i+1][j+1].parent_i = i; 
					cellDetails[i+1][j+1].parent_j = j; 
				} 
			} 
		} 

		//----------- 8th Successor (South-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid (i+1, j-1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i+1, j-1, dest) == true && (grid[i][j] & 64) == 64) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i+1][j-1].parent_i = i; 
				cellDetails[i+1][j-1].parent_j = j; 
				printf("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i+1][j-1] == false && 
					(grid[i][j] & 64) == 64) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValue(i+1, j-1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i+1][j-1].f == FLT_MAX || 
						cellDetails[i+1][j-1].f > fNew) 
				{ 
					openList.insert(make_pair(fNew, 
										make_pair(i+1, j-1))); 

					// Update the details of this cell 
					cellDetails[i+1][j-1].f = fNew; 
					cellDetails[i+1][j-1].g = gNew; 
					cellDetails[i+1][j-1].h = hNew; 
					cellDetails[i+1][j-1].parent_i = i; 
					cellDetails[i+1][j-1].parent_j = j; 
				} 
			} 
		} 
	} 

	// When the destination cell is not found and the open 
	// list is empty, then we conclude that we failed to 
	// reach the destiantion cell. This may happen when the 
	// there is no way to destination cell (due to blockages) 
	if (foundDest == false) 
		printf("Failed to find the Destination Cell\n"); 

	return; 
} 

void aStarSearchManhattan(int8_t grid[][COL], Pair src, Pair dest) 
{ 
	// If the source is out of range 
	if (isValid (src.first, src.second) == false) 
	{ 
		printf ("Source is invalid\n"); 
		return; 
	} 

	// If the destination is out of range 
	if (isValid (dest.first, dest.second) == false) 
	{ 
		printf ("Destination is invalid\n"); 
		return; 
	} 

	// Either the source or the destination is blocked 
	/*if (isUnBlocked(grid, src.first, src.second) == false || 
			isUnBlocked(grid, dest.first, dest.second) == false) 
	{ 
		printf ("Source or the destination is blocked\n"); 
		return; 
	}*/ 

	// If the destination cell is the same as source cell 
	if (isDestination(src.first, src.second, dest) == true) 
	{ 
		printf ("We are already at the destination\n"); 
		return; 
	} 

	// Create a closed list and initialise it to false which means 
	// that no cell has been included yet 
	// This closed list is implemented as a boolean 2D array 
	bool closedList[ROW][COL]; 
	memset(closedList, false, sizeof (closedList)); 

	// Declare a 2D array of structure to hold the details 
	//of that cell 
	cell cellDetails[ROW][COL]; 

	int i, j; 

	for (i=0; i<ROW; i++) 
	{ 
		for (j=0; j<COL; j++) 
		{ 
			cellDetails[i][j].f = FLT_MAX; 
			cellDetails[i][j].g = FLT_MAX; 
			cellDetails[i][j].h = FLT_MAX; 
			cellDetails[i][j].parent_i = -1; 
			cellDetails[i][j].parent_j = -1; 
		} 
	} 

	// Initialising the parameters of the starting node 
	i = src.first, j = src.second; 
	cellDetails[i][j].f = 0.0; 
	cellDetails[i][j].g = 0.0; 
	cellDetails[i][j].h = 0.0; 
	cellDetails[i][j].parent_i = i; 
	cellDetails[i][j].parent_j = j; 

	/* 
	Create an open list having information as- 
	<f, <i, j>> 
	where f = g + h, 
	and i, j are the row and column index of that cell 
	Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1 
	This open list is implenented as a set of pair of pair.*/
	set<pPair> openList; 

	// Put the starting cell on the open list and set its 
	// 'f' as 0 
	openList.insert(make_pair (0.0, make_pair (i, j))); 

	// We set this boolean value as false as initially 
	// the destination is not reached. 
	bool foundDest = false; 

	while (!openList.empty()) 
	{ 
		pPair p = *openList.begin(); 

		// Remove this vertex from the open list 
		openList.erase(openList.begin()); 

		// Add this vertex to the closed list 
		i = p.second.first; 
		j = p.second.second; 
		closedList[i][j] = true; 
	
	/* 
		Generating all the 8 successor of this cell 

			   N.W N N.E 
			     \ | / 
			      \|/ 
			W----Cell----E 
				  /|\ 
			     / | \ 
			    S.W S S.E 

		Cell-->Popped Cell (i, j) 
		N --> North	 (i-1, j) 
		S --> South	 (i+1, j) 
		E --> East	 (i, j+1) 
		W --> West		 (i, j-1) 
		N.E--> North-East (i-1, j+1) 
		N.W--> North-West (i-1, j-1) 
		S.E--> South-East (i+1, j+1) 
		S.W--> South-West (i+1, j-1)*/

		// To store the 'g', 'h' and 'f' of the 8 successors 
		double gNew, hNew, fNew; 

		//----------- 1st Successor (North) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i-1, j) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i-1, j, dest) == true && (grid[i][j] & 2) == 2) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i-1][j].parent_i = i; 
				cellDetails[i-1][j].parent_j = j; 
                double cost = cellDetails[i][j].f ;
				printf ("The destination cell is found\n"); 
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i-1][j] == false && 
					(grid[i][j] & 2) == 2) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValueManhattan(i-1, j, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i-1][j].f == FLT_MAX || 
						cellDetails[i-1][j].f > fNew) 
				{ 
					openList.insert( make_pair(fNew, 
											make_pair(i-1, j))); 

					// Update the details of this cell 
					cellDetails[i-1][j].f = fNew; 
					cellDetails[i-1][j].g = gNew; 
					cellDetails[i-1][j].h = hNew; 
					cellDetails[i-1][j].parent_i = i; 
					cellDetails[i-1][j].parent_j = j; 
				} 
			} 
		} 

		//----------- 2nd Successor (South) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i+1, j) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i+1, j, dest) == true && (grid[i][j] & 32) == 32) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i+1][j].parent_i = i; 
				cellDetails[i+1][j].parent_j = j; 
				printf("The destination cell is found\n");
                double cost = cellDetails[i][j].f ; 
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 
			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i+1][j] == false && 
					(grid[i][j] & 32) == 32) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValueManhattan(i+1, j, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i+1][j].f == FLT_MAX || 
						cellDetails[i+1][j].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, make_pair (i+1, j))); 
					// Update the details of this cell 
					cellDetails[i+1][j].f = fNew; 
					cellDetails[i+1][j].g = gNew; 
					cellDetails[i+1][j].h = hNew; 
					cellDetails[i+1][j].parent_i = i; 
					cellDetails[i+1][j].parent_j = j; 
				} 
			} 
		} 

		//----------- 3rd Successor (East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid (i, j+1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j+1, dest) == true && (grid[i][j] & 8) == 8) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i][j+1].parent_i = i; 
				cellDetails[i][j+1].parent_j = j; 
				printf("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j+1] == false && 
					(grid[i][j] & 8) == 8) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValueManhattan(i, j+1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i][j+1].f == FLT_MAX || 
						cellDetails[i][j+1].f > fNew) 
				{ 
					openList.insert( make_pair(fNew, 
										make_pair (i, j+1))); 

					// Update the details of this cell 
					cellDetails[i][j+1].f = fNew; 
					cellDetails[i][j+1].g = gNew; 
					cellDetails[i][j+1].h = hNew; 
					cellDetails[i][j+1].parent_i = i; 
					cellDetails[i][j+1].parent_j = j; 
				} 
			} 
		} 

		//----------- 4th Successor (West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i, j-1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i, j-1, dest) == true && (grid[i][j] & 128) == 128) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i][j-1].parent_i = i; 
				cellDetails[i][j-1].parent_j = j; 
				printf("The destination cell is found\n");
                double cost = cellDetails[i][j].f ; 
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i][j-1] == false && 
					(grid[i][j] & 128) == 128) 
			{ 
				gNew = cellDetails[i][j].g + 1.0; 
				hNew = calculateHValueManhattan(i, j-1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i][j-1].f == FLT_MAX || 
						cellDetails[i][j-1].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, 
										make_pair (i, j-1))); 

					// Update the details of this cell 
					cellDetails[i][j-1].f = fNew; 
					cellDetails[i][j-1].g = gNew; 
					cellDetails[i][j-1].h = hNew; 
					cellDetails[i][j-1].parent_i = i; 
					cellDetails[i][j-1].parent_j = j; 
				} 
			} 
		} 

		//----------- 5th Successor (North-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i-1, j+1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i-1, j+1, dest) == true && (grid[i][j] & 4) == 4) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i-1][j+1].parent_i = i; 
				cellDetails[i-1][j+1].parent_j = j; 
				printf ("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i-1][j+1] == false && 
					(grid[i][j] & 4) == 4) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValueManhattan(i-1, j+1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i-1][j+1].f == FLT_MAX || 
						cellDetails[i-1][j+1].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, 
									make_pair(i-1, j+1))); 

					// Update the details of this cell 
					cellDetails[i-1][j+1].f = fNew; 
					cellDetails[i-1][j+1].g = gNew; 
					cellDetails[i-1][j+1].h = hNew; 
					cellDetails[i-1][j+1].parent_i = i; 
					cellDetails[i-1][j+1].parent_j = j; 
				} 
			} 
		} 

		//----------- 6th Successor (North-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid (i-1, j-1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination (i-1, j-1, dest) == true && (grid[i][j] & 1) == 1) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i-1][j-1].parent_i = i; 
				cellDetails[i-1][j-1].parent_j = j; 
				printf ("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i-1][j-1] == false && 
					(grid[i][j] & 1) == 1) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValueManhattan(i-1, j-1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i-1][j-1].f == FLT_MAX || 
						cellDetails[i-1][j-1].f > fNew) 
				{ 
					openList.insert( make_pair (fNew, make_pair (i-1, j-1))); 
					// Update the details of this cell 
					cellDetails[i-1][j-1].f = fNew; 
					cellDetails[i-1][j-1].g = gNew; 
					cellDetails[i-1][j-1].h = hNew; 
					cellDetails[i-1][j-1].parent_i = i; 
					cellDetails[i-1][j-1].parent_j = j; 
				} 
			} 
		} 

		//----------- 7th Successor (South-East) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid(i+1, j+1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i+1, j+1, dest) == true && (grid[i][j] & 16) == 16) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i+1][j+1].parent_i = i; 
				cellDetails[i+1][j+1].parent_j = j; 
				printf ("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath (cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i+1][j+1] == false && 
					(grid[i][j] & 16) == 16) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValueManhattan(i+1, j+1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i+1][j+1].f == FLT_MAX || 
						cellDetails[i+1][j+1].f > fNew) 
				{ 
					openList.insert(make_pair(fNew, 
										make_pair (i+1, j+1))); 

					// Update the details of this cell 
					cellDetails[i+1][j+1].f = fNew; 
					cellDetails[i+1][j+1].g = gNew; 
					cellDetails[i+1][j+1].h = hNew; 
					cellDetails[i+1][j+1].parent_i = i; 
					cellDetails[i+1][j+1].parent_j = j; 
				} 
			} 
		} 

		//----------- 8th Successor (South-West) ------------ 

		// Only process this cell if this is a valid one 
		if (isValid (i+1, j-1) == true) 
		{ 
			// If the destination cell is the same as the 
			// current successor 
			if (isDestination(i+1, j-1, dest) == true && (grid[i][j] & 64) == 64) 
			{ 
				// Set the Parent of the destination cell 
				cellDetails[i+1][j-1].parent_i = i; 
				cellDetails[i+1][j-1].parent_j = j; 
				printf("The destination cell is found\n"); 
                double cost = cellDetails[i][j].f ;
				tracePath(cellDetails, dest,cost); 
				foundDest = true; 
				return; 
			} 

			// If the successor is already on the closed 
			// list or if it is blocked, then ignore it. 
			// Else do the following 
			else if (closedList[i+1][j-1] == false && 
					(grid[i][j] & 64) == 64) 
			{ 
				gNew = cellDetails[i][j].g + 1.414; 
				hNew = calculateHValueManhattan(i+1, j-1, dest); 
				fNew = gNew + hNew; 

				// If it isn’t on the open list, add it to 
				// the open list. Make the current square 
				// the parent of this square. Record the 
				// f, g, and h costs of the square cell 
				//			 OR 
				// If it is on the open list already, check 
				// to see if this path to that square is better, 
				// using 'f' cost as the measure. 
				if (cellDetails[i+1][j-1].f == FLT_MAX || 
						cellDetails[i+1][j-1].f > fNew) 
				{ 
					openList.insert(make_pair(fNew, 
										make_pair(i+1, j-1))); 

					// Update the details of this cell 
					cellDetails[i+1][j-1].f = fNew; 
					cellDetails[i+1][j-1].g = gNew; 
					cellDetails[i+1][j-1].h = hNew; 
					cellDetails[i+1][j-1].parent_i = i; 
					cellDetails[i+1][j-1].parent_j = j; 
				} 
			} 
		} 
	} 

	// When the destination cell is not found and the open 
	// list is empty, then we conclude that we failed to 
	// reach the destiantion cell. This may happen when the 
	// there is no way to destination cell (due to blockages) 
	if (foundDest == false) 
		printf("Failed to find the Destination Cell\n"); 

	return; 
}

// Driver program to test above function 
int main() 
{ 
	/* Description of the Grid- 
	1--> The cell is not blocked 
	0--> The cell is blocked */
    
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
