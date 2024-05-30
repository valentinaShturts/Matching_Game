#include "cards.h"
#include "Achievments.h"
#include "Start_Game.h"

//меню
void Menu(int& counter)
{
	PlaySound(TEXT("hi.wav"), NULL, SND_FILENAME | SND_ASYNC);
	cout << "    Welcome to the Matching Game!" << endl << endl;
	cout << "\t1. Start game" << endl << "\t2. View achievements" << endl << "\t3. Exit" << endl << endl;
	int choice;
	do
	{
		cout << "Enter your choice: ";
		getValidatedInput(choice);
		PlaySound(TEXT("choice.wav"), NULL, SND_FILENAME | SND_ASYNC);
	} while (choice < 1 || choice >3);

	system("cls");

	if (choice == 1)
	{
		counter++;
		StartGame(counter);
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