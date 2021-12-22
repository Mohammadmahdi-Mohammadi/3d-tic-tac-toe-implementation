#include <conio.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
using namespace std;
int node_number = 0;
int branch = 0;
/*
Alpha–beta pruning is a search algorithm that seeks to decrease the number
of nodes that are evaluated by the minimax algorithm in its search tree. 
It is an adversarial search algorithm used commonly for machine playing of 
two-player games (Tic-tac-toe, Chess, Go, etc.). ... Such moves need not be evaluated further.

Minimax
(sometimes MinMax, MM[1] or saddle point[2]) is a decision rule used in
artificial intelligence, decision theory, game theory, statistics and philosophy 
for minimizing the possible loss for a worst case (maximum loss) scenario. When
dealing with gains, it is referred to as "maximin"—to maximize the minimum gain.
Originally formulated for two-player zero-sum game theory, covering both the cases 
where players take alternate moves and those where they make simultaneous moves,
it has also been extended to more complex games and to general decision-making 
in the presence of uncertainty
*/

struct Move  // this object use for change situation on X,Y,Z that was selective 
{
	int Z, Y, X;
	Move()
	{
		Z = 0;
		Y = 0;
		X = 0;
	}

	Move(const Move& temp) {
		Z = temp.Z;
		Y = temp.Y;
		X = temp.X;
	}
};

struct Gameboard
{
	int*** Data; /* Board numbers are placed in this variable with variables 0, 2, and 2.
					And set to neutral 0 and 1 for first player and 2 for second player*/

	int _Winner; // 0 or 1 or 2
	int _Value;

	int alpha;
	int beta;

	Move Optimize_Move;


	Gameboard()
	{
		int Z, Y, X;
		Data = new int**[3];
		for (Z = 0; Z < 4; Z++)
		{
			Data[Z] = new int*[3];
			for (Y = 0; Y < 4; Y++)
			{
				Data[Z][Y] = new int[3];
			}
		}


		//     Initial registration
		for (Z = 0; Z < 4; Z++)
		{
			for (Y = 0; Y < 4; Y++)
			{
				for (X = 0; X < 4; X++)
				{
					Data[Z][Y][X] = 0;
				}
			}
		}
		_Winner = 0;
		_Value = 0;
		alpha = -INT_MAX;
		beta = INT_MAX;
		Optimize_Move.Z = 0;
		Optimize_Move.Y = 0;
		Optimize_Move.X = 0;

	}

	Gameboard(const Gameboard& temp)
	{
		int Z, Y, X;
		Data = new int**[3];
		for (Z = 0; Z < 4; Z++)
		{
			Data[Z] = new int*[3];
			for (Y = 0; Y < 4; Y++)
				Data[Z][Y] = new int[3];
		}
		//     Initial registration
		for (Z = 0; Z < 4; Z++)
			for (Y = 0; Y < 4; Y++)
				for (X = 0; X < 4; X++)
					Data[Z][Y][X] = temp.Data[Z][Y][X];

		alpha = temp.alpha;
		beta = temp.beta;
		_Winner = temp._Winner;
		_Value = temp._Value;
		Optimize_Move = temp.Optimize_Move;
	}

};

class Game
{
private:
	Gameboard _Boards;
	int _Max;
	int _Min;

public:
	Game();
	~Game() {};

	void Print(Gameboard _boards);
	Move Give_Move();
	bool set(Gameboard &, Move , int , bool );
	bool undo(Gameboard &, Move , int );

	bool are_you_win(Gameboard &);
	bool Check_Diagonal(Gameboard &);
	bool Check_Ending(Gameboard &);
	int Assessment(Gameboard &);

	bool Best_Move(Gameboard, int, Move &);

	int MiniMax_AlphaBeta(Gameboard &_boards, int _depth, int alpha, int beta, bool _maximizingPlayer, int _player_mark);
	int MiniMax(Gameboard &_boards, int _depth,bool _maximizingPlayer, int _player_mark);

	void Random_Player(Gameboard &, int );
	void Playing();

};

Game::Game()
{
	Print(_Boards);
	Playing();
}

int ch_player(int mark)
{
	if (mark == 1)
		return 2;
	else
		return 1;
}
void Game::Print(Gameboard gameboard)
{
	int z, Y, X;

	for (z = 0; z < 4; z++)
	{
		cout << "\n Board Number: " << z;
		cout << "\n ========== \n";
		for (Y = 0; Y < 4; Y++)
		{
			for (X = 0; X < 4; X++)
			{
				cout << " " << gameboard.Data[z][Y][X] << " ";
			}
			cout << endl;
		}
		cout << " ========== \n";
	}
	cout << "\nnode_number: " << node_number<<"\n";
	cout << "\n branch: " << branch << "\n";

}

Move Game::Give_Move()
{
	int z = 0, Y = 0, X = 0;
	Move tmp_Move;
	cout << "\n ============================================== ";
	cout << "\n Which board would you like to make a move on (0-" << 3 << "): ";
	cin >> z;
	while (0 > z || z >= 4)
	{
		cout << " ... !!! WRONG: Out of range !!! ... \n";
		cout << " Which board would you like to make a move on (0-" << 3 << "): ";
		cin >> z;
	}

	cout << " Which row you would like to insert in (0-" << 3 << "): ";
	cin >> Y;
	while (0 > Y || Y >= 4)
	{
		cout << " ... !!! WRONG: Out of range !!! ... \n";
		cout << " Which row you would like to insert in (0-" << 3 << "): ";
		cin >> Y;
	}

	cout << " Which column you would like to insert in (0-" << 3 << "): ";
	cin >> X;
	while (0 > X || X >= 4)
	{
		cout << " ... !!! WRONG: Out of range !!! ... \n";
		cout << " Which column you would like to insert in (0-" << 3 << "): ";
		cin >> X;
	}


	tmp_Move.Z = z;
	tmp_Move.Y = Y;
	tmp_Move.X = X;
	return tmp_Move;
}
/*
This function is used to record on board
*/
bool Game::set(Gameboard &_gameboard, Move _Move, int _player_mark, bool _print)
{
	if (Check_Ending(_gameboard))
		return NULL;
	if (_gameboard.Data[_Move.Z][_Move.Y][_Move.X] == 0)
	{
		_gameboard.Data[_Move.Z][_Move.Y][_Move.X] = _player_mark;
		if (_print == true)
		{
			//system("cls");
			Print(_Boards);
			cout << " Player" << _player_mark << ": Marked the \" board = " << _Move.Z <<
				" , row  = " << _Move.Y << " , column = " << _Move.X << " \" !!! ... \n";
		}

		return true;
	}
	else
	{
		if (_print == true)
		{
			cout << " Player" << _player_mark << " WRONG: the \" board = " << _Move.Z <<
				" , row  = " << _Move.Y << " , column = " << _Move.X << " \"Has marked !!! ... \n";
		}
		return false;
	}
}
/*
This function is used to return
*/
bool Game::undo(Gameboard &_boards, Move _Move, int _player_mark)
{
	if (_boards.Data[_Move.Z][_Move.Y][_Move.X] == _player_mark)
	{
		_boards.Data[_Move.Z][_Move.Y][_Move.X] = 0;
		return true;
	}
	else
	{
		return false;
	}
}
/*
Checking that the player wins the game and,
in the event of a win, scores a score of +1 and -1
*/
bool Game::are_you_win(Gameboard &_boards)
{
	int z = 0, Y = 0, X = 0;
	int temp = 0;
	for (z = 0; z < 4; z++)
	{
		Y = 0, X = 0;


		//Check 
		// A 0 0 0 
		// A 0 0 0
		// A 0 0 0
		// A 0 0 0
	
		for (X = 0; X < 4; X++)
		{
			temp = _boards.Data[z][Y][X];
			if (temp != 0)
			{
				for (int r = 0; r < 4; r++)
				{
					if (_boards.Data[z][r][X] != temp)
					{
						break;
					}
					if (r == 3)
					{
						_boards._Winner = temp;
						return true;
					}
				}
			}
		}
		Y = 0, X = 0;

		//Check 
		// A A A A 
		// 0 0 0 0
		// 0 0 0 0
		// 0 0 0 0

		for (Y = 0; Y < 4; Y++)
		{
			temp = _boards.Data[z][Y][X];
			if (temp != 0)
			{
				for (int c = 0; c < 4; c++)
				{
					if (_boards.Data[z][Y][c] != temp)
					{
						break;
					}
					if (c == 3)
					{
						_boards._Winner = temp;
						return true;
					}
				}
			}
		}
		Y = 0, X = 0;

		//Check Diameter
		// A 0 0 0
		// 0 A 0 0
		// 0 0 A 0
		// 0 0 0 A

		Y = 0, X = 0;
		temp = _boards.Data[z][Y][X];
		if (temp != 0)
		{
			for (int K = 0; K < 4; K++)
			{

				if (_boards.Data[z][K][K] != temp)
				{
					break;
				}
				if (K == 3)
				{
					_boards._Winner = temp;
					return true;
				}
			}
		}


		//Check Diameter

		// 0 0 0 A		
		// 0 0 A 0		
		// 0 A 0 0		
		// A 0 0 0	
		Y = 0, X = 3;
		temp = _boards.Data[z][Y][X];
		if (temp != 0)
		{
			for (int Y = 0; Y < 4; Y++)
			{

				if (_boards.Data[z][Y][X] != temp)
				{
					break;
				}
				if (Y == 3)
				{
					_boards._Winner = temp;
					return true;
				}
				X--;
			}
		}
		Y = 0, X = 0;

		temp = _boards.Data[X][Y][z];
		if (temp != 0)
		{
			for (int K = 0; K < 4; K++)
			{

				if (_boards.Data[K][K][z] != temp)
				{
					break;
				}
				if (K == 3)
				{
					_boards._Winner = temp;
					return true;
				}
			}
		}

		Y = 3, X = 0;
		temp = _boards.Data[X][Y][z];
		if (temp != 0)
		{
			for (int K = 0; K < 4; K++)
			{

				if (_boards.Data[K][Y][z] != temp)
				{
					break;
				}
				if (K == 3)
				{
					_boards._Winner = temp;
					return true;
				}
				Y--;
			}
		}
		Y = 0, X = 0;

	}
	//Check 
	// A 0 0 0
	// 0 0 0 0
	// 0 0 0 0
	// 0 0 0 0

	 z = 0, Y = 0, X = 0;
	 temp = 0;
	for (Y = 0; Y < 4; Y++)
	{
		for (X = 0; X < 4; X++)
		{
			temp = _boards.Data[z][Y][X];
			if (temp != 0)
			{
				for (int b = 0; b < 4; b++)
				{
					if (_boards.Data[b][Y][X] != temp)
					{
						break;
					}
					if (b == 3)
					{
						_boards._Winner = temp;
						return true;
					}
				}
			}

		}
	}

	return false;
}


bool Game::Check_Diagonal(Gameboard &_boards)
{
	int z, X, Y;
	int temp = 0;


	z = 0, X = 0, Y = 0;
	temp = _boards.Data[z][X][Y];
	if (temp != 0)
	{
		for (int b = 0; b < 4; b++)
		{
			if (_boards.Data[b][X][Y] != temp)
			{
				break;
			}
			if (b == 3)
			{
				_boards._Winner = temp;
				return true;
			}
			X++;
			Y++;
		}
	}
	
	z = 0, X = 3, Y = 3;
	temp = _boards.Data[z][X][Y];
	if (temp != 0)
	{
		for (int b = 0; b < 4; b++)
		{
			if (_boards.Data[b][X][Y] != temp)
			{
				break;
			}
			if (b == 3)
			{
				_boards._Winner = temp;
				return true;
			}
			X--;
			Y--;
		}
	}



	z = 0, X = 3, Y = 0;
	temp = _boards.Data[z][X][Y];
	if (temp != 0)
	{
		for (int b = 0; b < 4; b++)
		{
			if (_boards.Data[b][X][Y] != temp)
			{
				break;
			}
			if (b == 3)
			{
				_boards._Winner = temp;
				return true;
			}
			X--;
			Y++;
		}
	}

	z = 0, X = 0, Y = 3;
	temp = _boards.Data[z][X][Y];
	if (temp != 0)
	{
		for (int b = 0; b < 4; b++)
		{
			if (_boards.Data[b][X][Y] != temp)
			{
				break;
			}
			if (b == 3)
			{
				_boards._Winner = temp;
				return true;
			}
			X++;
			Y--;
		}
	}

	//*************************

	z = 0, X = 0, Y = 0;
	for (Y = 0; Y < 4; Y++)
	{
		X = 0;
		temp = _boards.Data[z][X][Y];
		if (temp != 0)
		{
			for (int b = 0; b < 4; b++)
			{
				if (_boards.Data[b][X][Y] != temp)
				{
					break;
				}
				if (b == 3)
				{
					_boards._Winner = temp;
					return true;
				}
				X++;
			}
		}
	}

	z = 0, X = 3, Y = 0;
	for (Y = 0; Y < 3; Y++)
	{
		X = 4 - 1;
		temp = _boards.Data[z][X][Y];
		if (temp != 0)
		{
			for (int b = 0; b < 4; b++)
			{
				if (_boards.Data[b][X][Y] != temp)
				{
					break;
				}
				if (b == 4 - 1)
				{
					_boards._Winner = temp;
					return true;
				}
				X--;
			}
		}
	}


	z = 0, X = 0, Y = 0;
	for (X = 0; X < 4; X++)
	{
		Y = 0;
		temp = _boards.Data[z][X][Y];
		if (temp != 0)
		{
			for (int b = 0; b < 4; b++)
			{
				if (_boards.Data[b][X][Y] != temp)
				{
					break;
				}
				if (b == 4 - 1)
				{
					_boards._Winner = temp;
					return true;
				}
				Y++;
			}
		}
	}

	z = 0, X = 0, Y = 4 - 1;
	for (X = 0; X < 4; X++)
	{
		Y = 4 - 1;
		temp = _boards.Data[z][X][Y];
		if (temp != 0)
		{
			for (int b = 0; b < 4; b++)
			{
				if (_boards.Data[b][X][Y] != temp)
				{
					break;
				}
				if (b == 4 - 1)
				{
					_boards._Winner = temp;
					return true;
				}
				Y--;
			}
		}
	}
	return false;
}

bool Game::Check_Ending(Gameboard &_boards)
{
	int z = 0, Y = 0, X = 0;
	int temp = 0;

	for (z = 0; z < 4; z++)
	{
		for (Y = 0; Y < 4; Y++)
		{
			for (X = 0; X < 4; X++)
			{
				temp = _boards.Data[z][Y][X];	
				if (temp == 0)
				{
					return false;
				}
			}
		}
	}
	_boards._Winner = 0;
	return true;
}

int Game::Assessment(Gameboard &_boards)
{
	if (are_you_win(_boards) == true)
	{
		if (_Max == _boards._Winner)
		{
			return +1;
		}
		else
		{
			return -1;
		}
	}

	else if (Check_Diagonal(_boards) == true)
	{
		if (_Max == _boards._Winner)
		{
			return +1;
		}
		else
		{
			return -1;
		}
	}
	else if (Check_Ending(_boards) == true)
	{
		return 0;
	}
	else
		return 0;

}

/*
Checks whether the person in the current situation can make a move 
that wins or chooses the best move to prevent the other person from winning.
*/
bool Game::Best_Move(Gameboard _boards, int _player_mark, Move &_Move)
{
	Gameboard temp_b = _boards;
	int z = 0, Y = 0, X = 0;
	int temp = 0;

	//Chose best Move to win the game
	for (z = 0; z < 4; z++)
	{
		for (Y = 0; Y < 4; Y++)
		{
			for (X = 0; X < 4; X++)
			{
				temp = temp_b.Data[z][Y][X];
				if (temp == 0)
				{
					temp_b.Data[z][Y][X] = _player_mark;
					if (are_you_win(temp_b) == true ||
						Check_Diagonal(temp_b) == true){
						if (temp_b._Winner == _player_mark)
						{
							_Move.Z = z;
							_Move.Y = Y;
							_Move.X = X;
							return true;
						}
					}
					else
					{
						temp_b.Data[z][Y][X] = 0;
					}

				}
			}
		}
	}


	temp_b = _boards;

	//Chose best Move to avoid wining player 2
	for (z = 0; z < 4; z++)
	{
		for (Y = 0; Y < 4; Y++)
		{
			for (X = 0; X < 4; X++)
			{
				temp = temp_b.Data[z][Y][X];
				if (temp == 0)
				{
					temp_b.Data[z][Y][X] = ch_player(_player_mark);
					if (are_you_win(temp_b) == true ||
						Check_Diagonal(temp_b) == true)
					{
						if (temp_b._Winner == ch_player(_player_mark))
						{
							_Move.Z = z;
							_Move.Y = Y;
							_Move.X = X;
							return true;
						}
					}
					else
					{
						temp_b.Data[z][Y][X] = 0;
					}
				}
			}
		}
	}
	return false;
}

int Game::MiniMax_AlphaBeta(Gameboard &_boards, int _depth, int alpha, int beta, bool _maximizingPlayer, int _player_mark)
{
	node_number++;
	int z = 0, Y = 0, X = 0;
	bool tmp_Result = false;

	if (_depth == 0)
	{
		return Assessment(_boards);
	}
	if (Assessment(_boards) != 0)
	{
		return Assessment(_boards);
	}

	bool _Break = false;
	Move tmp_Move;

	if (_maximizingPlayer == true)
	{
		int best = -INT_MAX;

		if (Best_Move(_boards, _player_mark, tmp_Move) == true)
		{
			set(_boards, tmp_Move, _Max, false);

			_boards.Optimize_Move = tmp_Move;
			int value = Assessment(_boards);
			best = max(best, value);
			alpha = max(alpha, best);
			undo(_boards, tmp_Move, _Max);
		}
		else
		{
			for (z = 0; z < 4 && _Break == false; z++)
			{
				for (Y = 0; Y < 4 && _Break == false; Y++)
				{
					for (X = 0; X < 4 && _Break == false; X++)
					{
						branch++;
						int temp = _boards.Data[z][Y][X];
						if (temp == 0)
						{
							tmp_Move.Z = z;
							tmp_Move.Y = Y;
							tmp_Move.X = X;
							set(_boards, tmp_Move, _Max, false);

							_boards.Optimize_Move = tmp_Move;

							int value = MiniMax_AlphaBeta(_boards, _depth - 1, alpha, beta, false, ch_player(_player_mark));
							best = max(best, value);
							alpha = max(alpha, best);
							undo(_boards, tmp_Move, _Max);
							if (beta <= alpha)
							{
								_Break = true;
								break;
							}

						}
					}
				}
			}

		}

		return best;
	}

	_Break = false;
	if (_maximizingPlayer == false)
	{
		int best = INT_MAX;
		if (Best_Move(_boards, ch_player(_player_mark), tmp_Move) == true)
		{
			set(_boards, tmp_Move, _Min, false);

			int value = Assessment(_boards);
			best = min(best, value);
			beta = min(beta, best);
			undo(_boards, tmp_Move, _Min);
		}
		else
		{
			for (z = 0; z < 4 && _Break == false; z++)
			{
				for (Y = 0; Y < 4 && _Break == false; Y++)
				{
					for (X = 0; X < 4 && _Break == false; X++)
					{

						int temp = _boards.Data[z][Y][X];
						if (temp == 0)
						{
							tmp_Move.Z = z;
							tmp_Move.Y = Y;
							tmp_Move.X = X;
							set(_boards, tmp_Move, _Min, false);

							_boards.Optimize_Move = tmp_Move;

							int value = MiniMax_AlphaBeta(_boards, _depth - 1, alpha, beta, true, _player_mark);
							best = min(best, value);
							beta = min(beta, best);
							undo(_boards, tmp_Move, _Min);
							if (beta <= alpha)
							{
								_Break = true;
								break;
							}

						}
					}
				}
			}
		}

		return best;
	}


}
//Minimax algorithm with limited depth 
int Game::MiniMax(Gameboard &_boards, int _depth, bool _maximizingPlayer, int _player_mark)
{
	node_number++;
	int z = 0, Y = 0, X = 0;
	bool tmp_Result = false;

	if (_depth == 0)
	{
		return Assessment(_boards);
	}
	if (Assessment(_boards) != 0)
	{
		return Assessment(_boards);
	}

	bool _Break = false;
	Move tmp_Move;

	if (_maximizingPlayer == true)
	{
		int best = -INT_MAX;
		if (Check_Ending(_boards)) {
			cout << "Equal match\n";
			return -100;
		}
		if (Best_Move(_boards, _player_mark, tmp_Move) == true)
		{
			if (Check_Ending(_boards)) {
				cout << "Equal match\n";
				return -100;
			}
			set(_boards, tmp_Move, _Max, false);

			_boards.Optimize_Move = tmp_Move;


			int value = Assessment(_boards);
			best = max(best, value);
			undo(_boards, tmp_Move, _Max);
		}
		else
		{
			for (z = 0; z < 4 && _Break == false; z++)
			{
				for (Y = 0; Y < 4 && _Break == false; Y++)
				{
					for (X = 0; X < 4 && _Break == false; X++)
					{
						branch++;

						int temp = _boards.Data[z][Y][X];
						if (temp == 0)
						{
							tmp_Move.Z = z;
							tmp_Move.Y = Y;
							tmp_Move.X = X;
							set(_boards, tmp_Move, _Max, false);

							_boards.Optimize_Move = tmp_Move;
							if (Check_Ending(_boards)) {
								cout << "Equal match\n";
								return -100;
							}
							int value = MiniMax(_boards, _depth - 1, false, ch_player(_player_mark));
							best = max(best, value);
							undo(_boards, tmp_Move, _Max);
						}
					}
				}
			}

		}

		return best;
	}

	_Break = false;
	if (_maximizingPlayer == false)
	{
		int best = INT_MAX;
		if (Check_Ending(_boards)) {
			cout << "Equal match\n";
			return -100;
		}
		if (Best_Move(_boards, ch_player(_player_mark), tmp_Move) == true)
		{
			set(_boards, tmp_Move, _Min, false);
			int value = Assessment(_boards);
			best = min(best, value);
			undo(_boards, tmp_Move, _Min);
		}
		else
		{
			for (z = 0; z < 4 && _Break == false; z++)
			{
				for (Y = 0; Y < 4 && _Break == false; Y++)
				{
					for (X = 0; X < 4 && _Break == false; X++)
					{

						int temp = _boards.Data[z][Y][X];
						if (temp == 0)
						{
							tmp_Move.Z = z;
							tmp_Move.Y = Y;
							tmp_Move.X = X;
							set(_boards, tmp_Move, _Min, false);

							_boards.Optimize_Move = tmp_Move;
							if (Check_Ending(_boards)) {
								cout << "Equal match\n";
								return -100;
							}
							int value = MiniMax(_boards, _depth - 1, true, _player_mark);
							best = min(best, value);
							undo(_boards, tmp_Move, _Min);

						}
					}
				}
			}
		}

		return best;
	}


}

/*
This function gives random numbers for the length
and width and height variables for the board
*/
void Game::Random_Player(Gameboard &_boards, int _player_mark)
{
	Move tmp_Move;
	Gameboard temp_b = _boards;
	bool exit = false;
	do
	{
		srand((unsigned)time(NULL));
		int z = rand() % 4;
		int Y = rand() % 4;
		int X = rand() % 4;

		tmp_Move.Z = z;
		tmp_Move.Y = Y;
		tmp_Move.X = X;

		if (set(temp_b, tmp_Move, _player_mark, false) == true)
		{
			_boards.Optimize_Move = tmp_Move;
			exit = true;
		}
		else
		{
			undo(temp_b, tmp_Move, _player_mark);
		}
	} while (exit != true);
}


void Game::Playing()
{
	int z = 0, Y = 0, X = 0, winner = 0;
	int turn = true; // Player turn
	_Min = 1;
	_Max = 2;
	int n;
	cout << "1. Minimax  2.AlphaBeta\n";
	cin >> n;
	int m;
	cout << "\n1. computer vs computer \n 2.human vs computer\n 3.random vs computer\n";
	cin >> m;
	while (true)
	{
		if (Assessment(_Boards) != 0)
		{
			cout << " ... !!! Player" << _Boards._Winner << " won the game !!! ... \n";
			return;
		}

		if (turn == true)
		{
			if (n == 2) {//         AlphaBeta
				// PC 
				if (m == 1 || m == 3) {
					int value = MiniMax_AlphaBeta(_Boards, 3, _Boards.alpha, _Boards.beta, true, 1);
					set(_Boards, _Boards.Optimize_Move, 1, true);
				}

				// Human 
				if (m == 2) {
					do
					{
						_Boards.Optimize_Move = Give_Move();

					} while (set(_Boards, _Boards.Optimize_Move, 1, true) == false);
				}

				turn = false;
			}
			if (n == 1) { //            MiniMax
				// PC 
				if (m == 1 || m == 3) {
					int value = MiniMax(_Boards, 3, true, 1);
					set(_Boards, _Boards.Optimize_Move, 1, true);
				}

				// Human 
				if (m == 2) {
					do
					{
						_Boards.Optimize_Move = Give_Move();

					} while (set(_Boards, _Boards.Optimize_Move, 1, true) == false);
				}

				turn = false;
			}
		}
		else
		{
			if (n == 2) { //         AlphaBeta
				// PC Player 2
				if (m == 1 || m == 2) {
					int value = MiniMax_AlphaBeta(_Boards, 5, _Boards.alpha, _Boards.beta, true, 2);
					set(_Boards, _Boards.Optimize_Move, 2, true);
				}

				// Random Player2
				if (m == 3) {
					Random_Player(_Boards, 2);
					set(_Boards, _Boards.Optimize_Move, 2, true);
				}


				turn = true;
			}
		}
		if (n == 1) {//          MiniMax
			// PC Player 2
			if (m == 1 || m == 2) {
				int value = MiniMax(_Boards, 3, true, 2);
				set(_Boards, _Boards.Optimize_Move, 2, true);
			}
			// Random Player2
			if (m == 3) {
				Random_Player(_Boards, 2);
				set(_Boards, _Boards.Optimize_Move, 2, true);
			}
			turn = true;
		}
	}
	if (Check_Ending(_Boards) == true)
	{
		cout << " ... !!! No one could win the game !!! ... \n";
		return;
	}

}


int main()
{
	Game MyGame;
	_getch();
	return 0;
}