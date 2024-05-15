#include <iostream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <chrono>
#include <conio.h> // Для использования _kbhit() и _getch()
#include <thread>
#include <fstream>
#include <string>
#include <limits> // Для использования std::numeric_limits

#include <windows.h>
#ifdef max
#undef max
#endif

using namespace std;
using namespace std::chrono;



void getValidatedInput(int& input)
{
	while (true)
	{
		cin >> input;
		if (cin.fail())
		{
			cin.clear(); // Сбросить флаг ошибки
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Игнорировать неправильный ввод
			cout << "Invalid input. Enter again:  ";
		}
		else
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очистить буфер ввода
			break;
		}
	}
}

#pragma region Cards
//выбор к-ства игроков и вывод правил
void ChooseGameMode(int* number_of_players)
{
	cout << "  Choose the number of players(1 or 2): ";
	getValidatedInput(*number_of_players);
	if (*number_of_players == 1) cout << "    Here the rules: " << endl << " This is a simple memory game." << endl << " You need to turn over 2 cards, if they match, you're cool." << endl << " If not, don't despair and keep playing." << endl << " The game is over when there are no more cards on the field." << endl << " Good luck!" << endl;
	else if (*number_of_players == 2) cout << "    Here the rules: " << endl << " This is a simple memory game." << endl << " You will play with the computer. " << endl << " You need to turn over 2 cards, if they match, you're cool and continue to play." << endl << " If not, don't despair, but it's the computer's move." << endl << " The game is over when there are no more cards on the field." << endl << " The one with the most matches wins. " << endl << " Good luck!" << endl;
	else { ChooseGameMode(number_of_players); }
}

//Выбор размера поля - к-ства карточек
void ChooseSizeBoard(int* size)
{
	cout << "Enter size of the board (2, 4, 6, 8): ";
	getValidatedInput(*size);
	if (*size < 2 || *size >8 || *size % 2 != 0)  ChooseSizeBoard(size); 
}
//расспечатать карточки
void ShowCardBoard(char** cards, int size)
{
	cout << "\t   ";
	for (int i = 1; i < size + 1; i++)
	{
		cout << i << " ";
	}
	for (int i = 0; i < size; i++)
	{
		cout << endl << "\t" << i + 1 << "  ";
		for (int j = 0; j < size; j++)
		{
			cout << cards[i][j] << " ";
		}
	}
	cout << endl << endl;
}
//Заполнить карточки одним символом 219
void FillCardBoard(char** cards, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cards[i][j] = (char)219;
		}
	}
}
//Удалить массив карточек
void DeleteBoardArr(char** cards, int size)
{

	for (int i = 0; i < size; i++)
	{
		delete[] cards[i];
	}
	delete[] cards;
}

//заполнить карты символами
void FillCards(char** symbols, int size)
{
	int count = 0;
	int symbol = 224;
	for (int i = 0; i < size / 2; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if ((symbol + count) > 249)
			{
				symbol = 160;
				symbols[i][j] = static_cast<char>(symbol + count);
				symbols[i + (size / 2)][j] = static_cast<char>(symbol + count);
				count++;
			}
			else
			{
				symbols[i][j] = static_cast<char>(symbol + count);
				symbols[i + (size / 2)][j] = static_cast<char>(symbol + count);
				count++;
			}
		}
	}
}
//перемешать символы
void MixSymbols(char** symbols, int size)
{
	mt19937 rng(time(nullptr));
	for (int i = size - 1; i > 0; i--)
	{
		for (int j = size - 1; j > 0; j--)
		{
			uniform_int_distribution<int> dist(0, i);
			int randomRow = dist(rng);
			int randomCol = dist(rng);
			swap(symbols[i][j], symbols[randomRow][randomCol]);
		}
	}
}
//показать символы
void ShowCards(char** symbols, int size)
{
	cout << "\t   ";
	for (int i = 1; i < size + 1; i++)
	{
		cout << i << " ";
	}
	for (int i = 0; i < size; i++)
	{
		cout << endl << "\t" << i + 1 << "  ";
		for (int j = 0; j < size; j++)
		{
			cout << symbols[i][j] << " ";
		}
	}
}
//удалить массив символов
void DeleteSymbolsArr(char** symbols, int size)
{

	for (int i = 0; i < size; i++)
	{
		delete[] symbols[i];
	}
	delete[] symbols;
}
#pragma endregion


#pragma region Achievements
//структура достижений
struct Achievements
{
	int id = 0;
	int game_mode = 0;
	int size_board = 0;
	int min_duration = 0;
	int sec_duration = 0;
	int card_turns_pl = 0;
	int matches_number_pl = 0;
	int card_turns_pc = 0;
	int matches_number_pc = 0;
	bool result = 0;
};

void WriteGameResultToFile(const Achievements& result)
{
	ofstream file("game_results.txt", ios::app); // Открытие файла для записи (добавление в конец файла)
	if (file.is_open()) 
	{
		file << result.id;
		file << " ";
		if (result.game_mode == 1) file <<  " Single-user " ;
		else if (result.game_mode == 2) file <<  " Computer/Player ";
		file << " ";
		file  << result.size_board <<"x"<< result.size_board;
		file << " ";
		file <<  result.min_duration << "." << result.sec_duration;
		file << " ";
		file << (result.result ? " Winner " : " Loser ") << endl;
		file << endl;
		file.close();
	}
	else {
		cout << "Error: Unable to open file for writing." << endl;
	}
}

void ViewGameResultsFromFile() 
{
	cout << " ID \t Game mode \t Board size \t Elapsed time \t    Result" << endl << endl;
	ifstream file("game_results.txt"); // Открытие файла для чтения
	if (file.is_open())
	{
		string id, game_mode, board_size, elapsed_time, result;
		while (file >> id >> game_mode >> board_size >> elapsed_time >> result)
		{
			cout <<" " << id << "    " << game_mode << "\t    " << board_size << "\t\t    " << elapsed_time << "\t    " << result << endl;
		}
		file.close();
	}
	else
	{
		cout << "Error: Unable to open file for reading." << endl;
	}
}


void ClearFileContents(const string& filename) 
{
	ofstream file(filename); // Открытие файла для записи (это удалит его содержимое)
	file.close();
}

//вывод результатов после окончания игры
void PrintResults(Achievements& result)
{
	if (result.game_mode == 2)
	{
		if (result.result == 1) 
		{
			//PlaySound(TEXT("fnaf-kids-cheering.wav"), NULL, SND_SYNC | SND_FILENAME);
			cout << "\tCongratulations!! You won!" << endl << endl;
		}
		else { cout << "\tOh..sorry. You lose)" << endl << endl; }
		cout << "Elapsed time: " << result.min_duration << "." << result.sec_duration << " min" << endl << endl;
		cout << "Number of matches (You): " << result.matches_number_pl << "\t" << "(Computer): " << result.matches_number_pc << endl;
		cout << "Card turns (You): " << result.card_turns_pl << "\t" << "(Computer): " << result.card_turns_pc << endl << endl;
	}
	else if (result.game_mode == 1)
	{
		cout << "\tWow, you won, I was already starting to doubt" << endl << endl;
		cout << "Elapsed time: " << result.min_duration << "." << result.sec_duration << " min" << endl << endl;
		cout << "Number of matches (You): " << result.matches_number_pl << endl;
		cout << "Card turns (You): " << result.card_turns_pl << endl << endl;
	}
}

#pragma endregion

//ожидание нажатия пробела
void WaitForSpacebar()
{
	std::cout <<endl<< "Enter Space to continue... ";
	while (_getch() != ' '); // Ожидаем нажатия клавиши пробела
}

#pragma region Timer
//начало отсчета времени
void StartTimer(time_point<steady_clock>& startTime)
{
	startTime = steady_clock::now();
}

//остановка отсчета времени
double StopTimer(const time_point<steady_clock>& startTime)
{
	auto endTime = steady_clock::now();
	duration<double> elapsedTime = duration_cast<duration<double>>(endTime - startTime);
	return elapsedTime.count();
}

//преобразование секунд в минуты 
void ConvertTime(double totalSeconds, int& minutes, int& seconds)
{
	minutes = static_cast<int>(totalSeconds / 60); // Получаем количество минут
	seconds = static_cast<int>(totalSeconds) % 60; // Получаем количество оставшихся секунд
}
#pragma endregion

//ввод индексов нужной карты игроком
void EnterIndex(char** cards, int size, int& indexRow, int& indexCol)
{
	do
	{
		cout << "Enter row number of card: ";
		getValidatedInput(indexRow);
		cout << "Enter col number of card: ";
		getValidatedInput(indexCol);
	} while (indexRow > size || indexRow<1 || indexCol>size || indexCol < 1 || cards[indexRow - 1][indexCol - 1] != (char)219);
}

//рандомный выбор карты компом
void RandomIndex(char** cards, int size, int& indexRow, int& indexCol)
{
	mt19937 rng(random_device{}());
	uniform_int_distribution<int> dist(1, size);
	do
	{
		indexRow = dist(rng);
		indexCol = dist(rng);
	} while (cards[indexRow - 1][indexCol - 1] != (char)219);
}

//проверка на завершение игры
bool IsEndOfGame(char** cards, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (cards[i][j] != ' ')
				return 0;
		}
	}
	return 1;
}

//ход игрока
void PlayerMove(char** cards, char** symbols, int size, bool& isMatch, Achievements& result)
{
	int indexRow1, indexCol1, indexRow2, indexCol2;

	system("cls");
	ShowCardBoard(cards, size);

	EnterIndex(cards, size, indexRow1, indexCol1);
	result.card_turns_pl++;

	system("cls");
	cards[indexRow1 - 1][indexCol1 - 1] = symbols[indexRow1 - 1][indexCol1 - 1];
	ShowCardBoard(cards, size);
	EnterIndex(cards, size, indexRow2, indexCol2);

	system("cls");
	cards[indexRow2 - 1][indexCol2 - 1] = symbols[indexRow2 - 1][indexCol2 - 1];
	ShowCardBoard(cards, size);

	result.card_turns_pl++;

	if (cards[indexRow1 - 1][indexCol1 - 1] == cards[indexRow2 - 1][indexCol2 - 1])
	{
		cards[indexRow1 - 1][indexCol1 - 1] = ' ';
		cards[indexRow2 - 1][indexCol2 - 1] = ' ';
		isMatch = 1;
		result.matches_number_pl++;
	}
	else
	{
		cards[indexRow1 - 1][indexCol1 - 1] = (char)219;
		cards[indexRow2 - 1][indexCol2 - 1] = (char)219;
		isMatch = 0;
	}

	this_thread::sleep_for(chrono::seconds(2));
	system("cls");
	ShowCardBoard(cards, size);
}

//ход компа
void ComputerMove(char** cards, char** symbols, int size, bool& isMatch, Achievements& result)
{
	int indexRow1, indexCol1, indexRow2, indexCol2;

	system("cls");
	ShowCardBoard(cards, size);
	RandomIndex(cards, size, indexRow1, indexCol1);
	this_thread::sleep_for(chrono::seconds(1));

	result.card_turns_pc++;

	system("cls");
	cards[indexRow1 - 1][indexCol1 - 1] = symbols[indexRow1 - 1][indexCol1 - 1];
	ShowCardBoard(cards, size);
	RandomIndex(cards, size, indexRow2, indexCol2);
	this_thread::sleep_for(chrono::seconds(1));

	system("cls");
	cards[indexRow2 - 1][indexCol2 - 1] = symbols[indexRow2 - 1][indexCol2 - 1];
	ShowCardBoard(cards, size);

	result.card_turns_pc++;

	if (cards[indexRow1 - 1][indexCol1 - 1] == cards[indexRow2 - 1][indexCol2 - 1])
	{
		cards[indexRow1 - 1][indexCol1 - 1] = ' ';
		cards[indexRow2 - 1][indexCol2 - 1] = ' ';
		isMatch = 1;
		result.matches_number_pc++;
	}
	else
	{
		cards[indexRow1 - 1][indexCol1 - 1] = (char)219;
		cards[indexRow2 - 1][indexCol2 - 1] = (char)219;
		isMatch = 0;
	}

	this_thread::sleep_for(chrono::seconds(2));
	system("cls");
	ShowCardBoard(cards, size);
}

//старт игры с одним игроком
void StartOnePlayerGame(char** cards, char** symbols, int size, Achievements& result)
{
	bool isMatch = 0;
	do
	{
		PlayerMove(cards, symbols, size, isMatch, result);
	} while (IsEndOfGame(cards, size) == 0);
}

//старт игры комп/игрок
void StartTwoPlayerGame(char** cards, char** symbols, int size, Achievements& result)
{
	bool isMatch = 0;
	do
	{
		do
		{
			PlayerMove(cards, symbols, size, isMatch, result);
			if (IsEndOfGame(cards, size) == 1) isMatch = 0;
		} while (isMatch);
		if (IsEndOfGame(cards, size) != 1)
		{
			do
			{
				ComputerMove(cards, symbols, size, isMatch, result);
				if (IsEndOfGame(cards, size) == 1) isMatch = 0;
			} while (isMatch);
		}

	} while (IsEndOfGame(cards, size) == 0);

	if (result.matches_number_pl > result.matches_number_pc)result.result = 1;
	else if (result.matches_number_pl < result.matches_number_pc)result.result = 0;
	else { result.result = 0; }
}

//старт игры
void StartGame(int& counter)
{
	Achievements current_game;
	current_game.id = counter;

	int players = 1;
	int* number_of_players = &players;
	ChooseGameMode(number_of_players);

	int size = 2;
	int* size_board = &size;
	ChooseSizeBoard(size_board);

	current_game.size_board = *size_board;

	char** cards = new char* [size];
	for (int i = 0; i < size; i++)
	{
		cards[i] = new char[size];
	}
	FillCardBoard(cards, size);

	char** symbols = new char* [size];
	for (int i = 0; i < size; i++)
	{
		symbols[i] = new char[size];
	}
	FillCards(symbols, size);
	MixSymbols(symbols, size);


	time_point<steady_clock> startTime;
	StartTimer(startTime);

	if (*number_of_players == 1)
	{
		StartOnePlayerGame(cards, symbols, size, current_game);
		current_game.game_mode = 1;
		current_game.result = 1;
	}
	else if (*number_of_players == 2)
	{
		StartTwoPlayerGame(cards, symbols, size, current_game);
		current_game.game_mode = 2;
	}

	double elapsedTime = StopTimer(startTime);
	int minutes, seconds;
	ConvertTime(elapsedTime, minutes, seconds);

	current_game.min_duration = minutes;
	current_game.sec_duration = seconds;

	WaitForSpacebar();
	WriteGameResultToFile(current_game);

	DeleteBoardArr(cards, size);
	DeleteSymbolsArr(symbols, size);
	system("cls");

	

	PrintResults(current_game);
	WaitForSpacebar();
}

//меню
void Menu(int& counter)
{
	cout << "    Welcome to the Matching Game!" << endl << endl;
	cout << "\t1. Start game" << endl << "\t2. View achievements" << endl << "\t3. Exit" << endl << endl;
	int choice;
	do
	{
		cout << "Enter your choice: ";
		getValidatedInput(choice);
	} while (choice < 1 || choice >3);

	system("cls");

	if (choice == 1)
	{
		counter++;
		StartGame( counter);
	}

	else if (choice == 2)
	{
		system("cls");
		ViewGameResultsFromFile();
		WaitForSpacebar();
	}

	else if (choice == 3)
	{
		exit(0);
	}


}

int main()
{
	ClearFileContents("game_results.txt");

	int counter = 0;
	do
	{
		system("cls");
		Menu(counter);
	} while (true);

	
	return 0;
}