#pragma once
#include "cards.h"


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
	FILE* file = fopen("game_results.txt", "a"); // Открытие файла для записи (добавление в конец файла)
	if (file != nullptr)
	{
		fprintf(file, "%d ", result.id);
		if (result.game_mode == 1) fprintf(file, " Single-user     ");
		else if (result.game_mode == 2) fprintf(file, " Computer/Player ");
		fprintf(file, " %dx%d ", result.size_board, result.size_board);
		fprintf(file, " %d.%d ", result.min_duration, result.sec_duration);
		fprintf(file, result.result ? " Winner " : " Loser ");
		fputc('\n', file);
		fclose(file);
	}
	else {
		cout << "Error: Unable to open file for writing." << endl;
	}
}

void ViewGameResultsFromFile()
{
	cout << " ID \t Game mode \t Board size \t Elapsed time \t    Result" << endl << endl;
	FILE* file = fopen("game_results.txt", "r"); // Открытие файла для чтения
	if (file != nullptr)
	{
		char id[50], game_mode[50], board_size[50], elapsed_time[50], result[50];
		while (fscanf(file, "%49s %49s %49s %49s %49s", id, game_mode, board_size, elapsed_time, result) == 5)
		{
			cout << " " << id << "    " << game_mode << "\t    " << board_size << "\t\t    " << elapsed_time << "\t    " << result << std::endl;
		}
		fclose(file);
	}
	else
	{
		cout << "Error: Unable to open file for reading." << endl;
	}
}


void ClearFileContents(const string& filename)
{
	FILE* file = fopen("game_results.txt", "w"); // Открытие файла для чтения
	if (file != nullptr) // Открытие файла для записи (это удалит его содержимое)
	{
		fclose(file);
	}
	else
	{
		cout << "Error: Unable to open file." << endl;
	}
}

//вывод результатов после окончания игры
void PrintResults(Achievements& result)
{
	if (result.game_mode == 2)
	{
		if (result.result == 1)
		{
			PlaySound(TEXT("fnaf-kids-cheering.wav"), NULL, SND_FILENAME | SND_ASYNC);
			cout << "\tCongratulations!! You won!" << endl << endl;
		}
		else {
			PlaySound(TEXT("congratulations.wav"), NULL, SND_FILENAME | SND_ASYNC); cout << "\tCongratulations!! You are a failure!" << endl << endl; /*Sleep(5000);  PlaySound(TEXT("emotional-damage-meme.wav"), NULL, SND_FILENAME | SND_ASYNC)*/;
		}
		cout << "Elapsed time: " << result.min_duration << "." << result.sec_duration << " min" << endl << endl;
		cout << "Number of matches (You): " << result.matches_number_pl << "\t" << "(Computer): " << result.matches_number_pc << endl;
		cout << "Card turns (You): " << result.card_turns_pl << "\t" << "(Computer): " << result.card_turns_pc << endl << endl;
	}
	else if (result.game_mode == 1)
	{
		PlaySound(TEXT("fnaf-kids-cheering.wav"), NULL, SND_FILENAME | SND_ASYNC);
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
	std::cout << endl << "Enter Space to continue... ";
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
