#pragma once
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <random>
#include <chrono>
#include <conio.h> // Для использования _kbhit() и _getch()
#include <thread>
#include <limits> // Для использования std::numeric_limits


#ifdef max
#undef max
#endif

using namespace std;
using namespace std::chrono;
#pragma warning(disable : 4996)
#pragma comment(lib, "winmm.lib")


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


//выбор к-ства игроков и вывод правил
void ChooseGameMode(int* number_of_players)
{
	cout << "  Choose the number of players(1 or 2): ";
	getValidatedInput(*number_of_players);
	PlaySound(TEXT("choice.wav"), NULL, SND_FILENAME | SND_ASYNC);
	if (*number_of_players == 1) cout << "    Here the rules: " << endl << " This is a simple memory game." << endl << " You need to turn over 2 cards, if they match, you're cool." << endl << " If not, don't despair and keep playing." << endl << " The game is over when there are no more cards on the field." << endl << " Good luck!" << endl;
	else if (*number_of_players == 2) cout << "    Here the rules: " << endl << " This is a simple memory game." << endl << " You will play with the computer. " << endl << " You need to turn over 2 cards, if they match, you're cool and continue to play." << endl << " If not, don't despair, but it's the computer's move." << endl << " The game is over when there are no more cards on the field." << endl << " The one with the most matches wins. " << endl << " Good luck!" << endl;
	else { ChooseGameMode(number_of_players); }
}

//Выбор размера поля - к-ства карточек
void ChooseSizeBoard(int* size)
{
	cout << "Enter size of the board (2, 4, 6, 8): ";
	getValidatedInput(*size);
	PlaySound(TEXT("choice.wav"), NULL, SND_FILENAME | SND_ASYNC);
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