#pragma once
#include "cards.h"
#include "Achievments.h"


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
		PlaySound(TEXT("duolingo-correct.wav"), NULL, SND_FILENAME | SND_ASYNC);
		cards[indexRow1 - 1][indexCol1 - 1] = ' ';
		cards[indexRow2 - 1][indexCol2 - 1] = ' ';
		isMatch = 1;
		result.matches_number_pl++;
	}
	else
	{
		PlaySound(TEXT("windows-xp-critical-error.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
		PlaySound(TEXT("duolingo-correct.wav"), NULL, SND_FILENAME | SND_ASYNC);
		cards[indexRow1 - 1][indexCol1 - 1] = ' ';
		cards[indexRow2 - 1][indexCol2 - 1] = ' ';
		isMatch = 1;
		result.matches_number_pc++;
	}
	else
	{
		PlaySound(TEXT("windows-xp-critical-error.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
	PlaySound(NULL, 0, 0);
}