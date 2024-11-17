#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <windows.h>

#define MAX_X 100
#define N 20

int getmiddleofX(int n);
void CursorMove(int x, int y);
void printtitle();
int menuprocess();
void mainprocess();
void boardwall(int* board);
void spawnsnake(int* board);
void spawnfood(int* board, int* isfood);
void snakemove(int* board, int* dir_info, int* direction, int* score, int* isfood, int* islose);
int findloc(int* board, int n);
void printboard(int* board, int score);
int endprocess();

int main()
{
	while (1)
	{
		system("cls");
		if (menuprocess())
			break;
		mainprocess();
		if (endprocess())
			break;
	}

	return 0;
}

int getmiddleofX(int n)
{
	return (MAX_X - n) / 2;
}
void CursorMove(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	return;
}

void printtitle()
{
	char* str1 = "### Snake game ###";
	CursorMove(getmiddleofX(strlen(str1)), 2);
	printf("%s\n", str1);
	return;
}

int menuprocess()
{
	char input[16] = { 0 };

	printtitle();
	printf("1. Play game\n");
	printf("2. Exit\n");
	while (1)
	{
		printf("\nCommand : ");
		fgets(input, 16, stdin);
		if (atoi(input) == 1)
			return 0;
		else if (atoi(input) == 2)
			return 1;
		else
			printf("Wrong command.");
	}
}
void mainprocess()
{
	int board[N * N] = { 0 }; //0 == empty, 1 == wall, 2 == food, 3 == head
	int dir_info[N * N] = { 4,4,4,4,4,4, };
	int direction = 4; //1 == up 2 == left 3 == down 4 == right
	int score = 0;
	int isfood = 0;
	int islose = 0;

	system("cls");
	printtitle();
	boardwall(board);
	spawnsnake(board);

	while (1)
	{
		if (isfood == 0)
			spawnfood(board, &isfood);
		snakemove(board, dir_info, &direction, &score, &isfood, &islose);
		printboard(board, score);
		if (islose)
			break;
		Sleep(250);
	}
	return;
}
int endprocess()
{
	char input[16] = { 0 };
	while (1)
	{
		printf("\nPlay again? (y/n) : ");
		fgets(input, 16, stdin);
		if (input[0] == 'y')
			return 0;
		else if (input[0] == 'n')
			return 1;
		else
			printf("Wrong command, press again.\n");
	}
}

void boardwall(int* board)
{
	for (int i = 0; i < N; i++)
	{
		*(board + i) = 1;
		*(board + ((N - 1) * N) + i) = 1;
		*(board + (i * N)) = 1;
		*(board + (i * N) + N - 1) = 1;
	}
	return;
}
void spawnsnake(int* board)
{
	*(board + (N * N / 2) + N / 2 - 2) = 6;
	*(board + (N * N / 2) + N / 2 - 1) = 5;
	*(board + (N * N / 2) + N / 2) = 4;
	*(board + (N * N / 2) + N / 2 + 1) = 3;
	return;
}
void spawnfood(int* board, int* isfood)
{
	int n = 0;
	srand(time(NULL));

	while (*(board + n) != 0)
		n = rand() % (N * N);
	*(board + n) = 2;
	*isfood = 1;
	return;
}
void snakemove(int* board, int* dir_info, int* direction, int* score, int* isfood, int* islose)
{
	char commandz = 0;
	int i = 0;
	int isconsume = 0;
	int n = 0;
	//커맨드 입력
	if (_kbhit())
	{
		commandz = _getch();
		switch (commandz)
		{
		case 'w':
			if (*direction != 3)
				*direction = 1;
			break;
		case 'a':
			if (*direction != 4)
				*direction = 2;
			break;
		case 's':
			if (*direction != 1)
				*direction = 3;
			break;
		case 'd':
			if (*direction != 2)
				*direction = 4;
			break;
		default:
			break;
		}
	}
	//이동정보 갱신
	for (int j = *score + 3; j >= 0; j--)
	{
		*(dir_info + j + 1) = *(dir_info + j);
	}
	*(dir_info) = *direction;
	//패배여부, 음식섭취여부
	n = findloc(board, 3);
	switch (*(dir_info))
	{
	case 1:
		if (*(board + n - N) == 1 || *(board + n - N) >= 3)
			*islose = 1;
		if (*(board + n - N) == 2)
			isconsume = 1;
		break;
	case 2:
		if (*(board + n - 1) == 1 || *(board + n - 1) >= 3)
			*islose = 1;
		if (*(board + n - 1) == 2)
			isconsume = 1;
		break;
	case 3:
		if (*(board + n + N) == 1 || *(board + n + N) >= 3)
			*islose = 1;
		if (*(board + n + N) == 2)
			isconsume = 1;
		break;
	case 4:
		if (*(board + n + 1) == 1 || *(board + n + 1) >= 3)
			*islose = 1;
		if (*(board + n + 1) == 2)
			isconsume = 1;
		break;
	default:
		break;
	}
	
	//이동
	for (i = 0; i < *score + 4; i++)
	{
		n = findloc(board, i + 3);
		if (n)
		{
			switch (*(dir_info + i))
			{
			case 1: //up
				*(board + n - N) = *(board + n);
				*(board + n) = 0;
				break;
			case 2: //left
				*(board + n - 1) = *(board + n);
				*(board + n) = 0;
				break;
			case 3: //down
				*(board + n + N) = *(board + n);
				*(board + n) = 0;
				break;
			case 4: //right
				*(board + n + 1) = *(board + n);
				*(board + n) = 0;
				break;
			default:
				break;
			}
		}
	}
	if (isconsume)
	{
		*score = *score + 1;
		*(board + n) = *score + 6;
		*isfood = 0;
	}

	return;
}

int findloc(int* board, int n)
{
	int i = 0;
	for (i; i < N * N; i++)
	{
		if (*(board + i) == n)
			return i;
	}

	return 0;
}

void printboard(int* board, int score)
{
	for (int i = 0; i < N; i++)
	{
		CursorMove(getmiddleofX(N), 6 + i);
		for (int j = 0;j < N; j++)
		{
			switch (*(board + (N * i) + j))
			{
			case 0: // empty
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
				printf("*");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			case 1: //wall
				printf("#");
				break;
			case 2: //food
				printf("@");
				break;
			case 3: //head
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
				printf(" ");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			default: //body
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				printf(" ");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
		}
	}

	printf("\n\nScore : %d", score);
	return;
}

