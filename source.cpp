#include <iostream>
#include <vector>

const int ROWS = 9;
const int COLS = 9;
void print_board(int board[ROWS][COLS])
{
	std::cout << "-------------------------" << std::endl;							// heading line 

	for(int board_row = 0; board_row < ROWS; ++board_row)							// for every row
	{
		if( board_row % 3 == 0 && board_row != 0 )									// every third row print dashes 
		{
			std:: cout << "- - - - - - - - - - - -" << std::endl;
		}

		for(int board_col = 0; board_col < COLS; ++board_col)						// for every column
		{
			if(board_col % 3 == 0 && board_col != 0 )								// after every third column print vertical bar
			{
				std::cout << " | ";
			}
		
			std::cout << board[board_row][board_col] << " ";						// show data at board[row][column]

			if(board_col == 8)
			{
				std::cout << "|" ;
			}
		}
		std::cout << std::endl;														// end line for next row
	}
	std:: cout << "- - - - - - - - - - - -" << std::endl;							// print final dashed line


	std::cout << "-------------------------" << std::endl;							// finishing line to match heading line
}

bool canPlace(int arr[ROWS][COLS], int row, int col, int value)
{
	if(arr[row][col] != 0 )													   		// if value is not zero, cannot place
	{
		return false;
	}

	bool status = true;													      		 // status for if we can place that number in that cell
	int gridx = (col / 3) * 3;												  		 // individual grid x value
	int gridy = (row / 3) * 3;												   		// individual grid y value

	for(int i = 0; i < ROWS; ++i)
	{
		if(arr[row][i] == value)											  		// break out of loop on first false
		{
			status = false;
			break;
		}

		if(arr[i][col] == value)
		{
			status = false;
			break;
		}

		if(arr[gridy + i / 3][gridx + i % 3] == value)								 // numbers cannot repeat within each individual grid
		{
			status = false;
			break;
		}
	}

	return status;
}

void nextEmpty(int arr[ROWS][COLS], int row, int col, int &nrow, int &ncol) 		// pass nrow and ncol as references so we can modify original values
{
	int index = ROWS * COLS + 1;													// high index

	for(int i = row * COLS + col + 1; i < ROWS * COLS; ++i)  						// start 1 cell beyond our current cell
	{
		if(arr[i / 9][i % 9] == 0)													// deduce down to individual row and column index
		{
			index = i;																// index = to this i value
			break;
		}
	}

	nrow = index / 9;																// individual row
	ncol = index % 9 ;																// individual column
}	

void Copy(int arr[ROWS][COLS], int arrCpy[ROWS][COLS])
{
	for(int y = 0; y < ROWS; ++y)
	{
		for(int x = 0; x < COLS; ++x)
		{
			arrCpy[y][x] = arr[y][x];												// copy orignal board into arrCpy
		}
	}
}

std::vector<int> find_valid(int arr[ROWS][COLS], int row, int col)					// find valid numbers that we can place
{
	std::vector<int> placeables = {};

	for(int i = 1; i <= ROWS; ++i)													// 9 possible digits we can place
	{
		if(canPlace(arr, row, col, i))												// if this number is placeable, add to vector
		{	
			placeables.push_back(i);
		}
	}

	return placeables;																// return vector of valid placeable numbers
}		

bool solveSudoku(int arr[ROWS][COLS], int row, int col)
{
	if(row > 8)																		// exit condition if we have computed all rows
	{
		return true;
	}

	if(arr[row][col] != 0 )															// if our current cell is not 0
	{
		int nextRow;
		int nextCol;

		nextEmpty(arr, row, col, nextRow, nextCol);									// find next empty
		return solveSudoku(arr, nextRow, nextCol);									// resume solve sudoku from here
	}

	std::vector<int> placeables  = find_valid(arr, row, col);						// get the possible placeable values

	if(placeables.size() == 0)
	{
		return false;																// can't place anything
	}

	bool status = false;															// status for solution

	for(int i = 0; i < placeables.size(); ++i)										// for every item in the placeable items vector
	{
		int n = placeables[i];
		int arrCpy[ROWS][COLS];
		Copy(arr,arrCpy);															// copy array so we can run tests on the copy

		arrCpy[row][col] = n;														// what we want to check

		int nextRow = row;
		int nextCol = col;

		nextEmpty(arrCpy, row, col, nextRow, nextCol);								// find the next empty value

		if(solveSudoku(arrCpy, nextRow, nextCol))									// if the solve is successful
		{
			Copy(arrCpy, arr);														// copy is solved, so copy into original board
			status = true;															// solution status set to true
			break;																	// explore no further
		}	
	}

	return status;																	// return solution status
}

int main()
{
	// example from https://dingo.sbs.arizona.edu/~sandiway/sudoku/examples.html : Easiest, first example
	int easy_board[ROWS][COLS] = {
		{0,0,0,2,6,0,7,0,1},
		{6,8,0,0,7,0,0,9,0},
		{1,9,0,0,0,4,5,0,0},
		{8,2,0,1,0,0,0,4,0},
		{0,0,4,6,0,2,9,0,0},
		{0,5,0,0,0,3,0,2,8},
		{0,0,9,3,0,0,0,7,4},
		{0,4,0,0,5,0,0,3,6},
		{7,0,3,0,1,8,0,0,0}
	};

	std::cout << "Unsolved board: EASY " << std::endl;

	print_board(easy_board);

	if(solveSudoku(easy_board,0,0))
	{
		std::cout << "Solved board: EASY " << std::endl;
		print_board(easy_board);
	}
	else
	{
		std::cout << "board cannot be solved!" << std::endl;
	}



	// example from https://dingo.sbs.arizona.edu/~sandiway/sudoku/examples.html : Intermediate, first example
	int intermediate_board[ROWS][COLS] = {
		{0,2,0,6,0,8,0,0,0},
		{5,8,0,0,0,9,7,0,0},
		{0,0,0,0,4,0,0,0,0},
		{3,7,0,0,0,0,5,0,0},
		{6,0,0,0,0,0,0,0,4},
		{0,0,8,0,0,0,0,1,3},
		{0,0,0,0,2,0,0,0,0},
		{0,0,9,8,0,0,0,3,6},
		{0,0,0,3,0,6,0,9,0},
	};

	std::cout << "Unsolved board: INTERMEDIATE " << std::endl;

	print_board(intermediate_board);

	if(solveSudoku(intermediate_board,0,0))
	{
		std::cout << "Solved board: INTERMEDIATE " << std::endl;
		print_board(intermediate_board);
	}
	else
	{
		std::cout << "board cannot be solved!" << std::endl;
	}


	// example from https://dingo.sbs.arizona.edu/~sandiway/sudoku/examples.html : Difficult, first example
	int difficult_board[ROWS][COLS] = {
		{0,0,0,6,0,0,4,0,0},
		{7,0,0,0,0,3,6,0,0},
		{0,0,0,0,9,1,0,8,0},
		{0,0,0,0,0,0,0,0,0},
		{0,5,0,1,8,0,0,0,3},
		{0,0,0,3,0,6,0,4,5},
		{0,4,0,2,0,0,0,6,0},
		{9,0,3,0,0,0,0,0,0},
		{0,2,0,0,0,0,1,0,0},
	};


	std::cout << "Unsolved board: DIFFICULT " << std::endl;

	print_board(difficult_board);

	if(solveSudoku(difficult_board,0,0))
	{
		std::cout << "Solved board: DIFFICULT " << std::endl;
		print_board(difficult_board);
	}
	else
	{
		std::cout << "board cannot be solved!" << std::endl;
	}

	return 0;
}