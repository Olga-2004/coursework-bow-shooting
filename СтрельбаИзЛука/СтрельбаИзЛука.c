#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define ROW 64
#define COLUMN 72
#define NAME_LEN_DEF 50

int area_code_to_num(char, int); // числовое значение зоны мишени
int col_strel(char dannye_strela[][COLUMN], int index); // возвращает сумму очков заданного участника
int index_uchastnika(char dannye_name[][NAME_LEN_DEF]); // возвращает индекс заданного участника
void output_dannye_uchastnika(int len, char name[], char dannye_strela[]); // выводит строку с данными участника
void output_tabl(char dannye_strela[][COLUMN], char dannye_name[][NAME_LEN_DEF], int max_name_len); // выводит или всю таблицу или при отличной от нуля длине имени часть по спортсмену
int search_winner(char dannye_strela[][COLUMN], int*); // определяет индекс участника, набравшего наибольшее количество очков
int search_set(char dannye_strela[][COLUMN], int, int*); // определяет номер лучшего сета и количество очков для заданного участника
char* zona(int ochki); // возвращает наименование зоны мишени по очкам
char* mishen_vse(char dannye_strela[][COLUMN]); // выводит, в какую зону мишени участники попадали чаще всего
char* mishen_uchastnik(char dannye_strela[][COLUMN], char dannye_name[][NAME_LEN_DEF]); // выводит, в какую зону мишени заданный участник попадал чаще всего
int tabl_from_file(char dannye_strela[][COLUMN], char dannye_name[][NAME_LEN_DEF]); // считывает данные из файла в массивы

void main()
{
	system("chcp 1251");

	int max_name_len = 0, name_len, index = 0, max_ochki = 0, set = 0;
	char dannye_strela[ROW][COLUMN];
	char dannye_name[ROW][NAME_LEN_DEF];
	char zona_str[NAME_LEN_DEF];

	while (1)
	{
		int deystvie, deystvie_dop;
		puts(" ______________________\n|                      |\n|   СТРЕЛЬБА ИЗ ЛУКА   |\n|______________________|");

		printf("\n    -###- Меню -###-\n\n  Выберите действие:\n1 - Выбрать файл\n2 - Отобразить таблицу\n3 - Поиск\n4 - Статистика попадания в мишени\n5 - Выход\n\n   > ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &deystvie);

		if (deystvie == 5) break;

		switch (deystvie)
		{
		case 1:
			if (!tabl_from_file(dannye_strela, dannye_name))
			{
				printf("Не удалось открыть файл!");
				printf("\n\nВведите любой символ, чтобы продолжить\n\n   > ");
				getchar();
				getchar();
				break;
			};
			// Вычисляем максимальную длину имени участника
			// Используется при выводе таблицы
			for (int i = 0; i < ROW; i++)
			{
				name_len = strlen(dannye_name[i]);
				if (name_len > max_name_len) max_name_len = name_len;
			}
			break;
		case 2:
			printf("\n-# Показывает считанные из файла данные #-\n\n1 - Таблица полностью\n2 - Для одного участника\n\n   > ");
			fseek(stdin, 0, SEEK_END);
			scanf("%d", &deystvie_dop);

			switch (deystvie_dop)
			{
			case 1:
				output_tabl(dannye_strela, dannye_name, max_name_len);
				break;
			case 2:
				output_tabl(dannye_strela, dannye_name, 0);
				break;
			default:
				printf("!Ошибка!");
			}
			break;
		case 3:
			printf("\n-# Поиск #-\n\n1 - Лучшего участника\n2 - Лучшего сета у участника\n\n   > ");
			fseek(stdin, 0, SEEK_END);
			scanf("%d", &deystvie_dop);

			switch (deystvie_dop)
			{
			case 1:
				printf("\n");
				printf("-# Выводит имя участника с наибольшим количеством очков #-\n\n   > ");
				index = search_winner(dannye_strela, &max_ochki);
				printf("%s", dannye_name[index]);
				printf("\n");
				printf("С результатом  > %d очков", max_ochki);
				printf("\n\nВведите любой символ, чтобы продолжить\n\n   > ");
				getchar();
				getchar();
				break;
			case 2:
				set = search_set(dannye_strela, index_uchastnika(dannye_name), &max_ochki);
				printf("\nЛучший сет  > %d\nС результатом  > %d очков", set, max_ochki);
				printf("\n\nВведите любой символ, чтобы продолжить\n\n   > ");
				getchar();
				getchar();
				break;
			default:
				printf("!Ошибка!");
			}
			break;
		case 4:
			printf("\n-# Выводит статистику попадания в зоны мишени #-\n\n1 - Для всех участников\n2 - Для одного участника\n\n   > ");
			fseek(stdin, 0, SEEK_END);
			scanf("%d", &deystvie_dop);

			switch (deystvie_dop)
			{
			case 1:
				strcpy(zona_str, mishen_vse(dannye_strela));
				printf("\n -# Наибольшее количество попаданий %s #-", zona_str);
				printf("\n\nВведите любой символ, чтобы продолжить\n\n   > ");
				getchar();
				getchar();
				break;
			case 2:
				strcpy(zona_str, mishen_uchastnik(dannye_strela, dannye_name));
				printf("\n -# Наибольшее количество попаданий %s #-", zona_str);
				printf("\n\nВведите любой символ, чтобы продолжить\n\n   > ");
				getchar();
				getchar();
				break;
			default:
				printf("!Ошибка!");
			}
			break;
		default:
			printf("!Ошибка!");
		}
		system("cls");
	}
}

int area_code_to_num(char area_code, int x_plus)
{
	int rez;

	switch (area_code)
	{
	case 'M':
		rez = 0;
		break;
	case 'X':
		rez = 10 + x_plus;
		break;
	default:
		rez = 106 - area_code + 1;
	}
	return rez;
}

int col_strel(char dannye_strela[][COLUMN], int index)
{
	int sum = 0;
	for (int i = 0; i < COLUMN; i++) {
		char c = dannye_strela[index][i];
		sum += area_code_to_num(c, 0);
	}
	return sum;
}

int index_uchastnika(char dannye_name[][NAME_LEN_DEF])
{
	char name[NAME_LEN_DEF];

	printf("\nВведите имя участника\n\n   > ");
	fseek(stdin, 0, SEEK_END);
	scanf("%[^\n]s", name);

	int index = -1;
	for (int i = 0; i < ROW; i++)
	{
		if (strcmp(name, dannye_name[i]) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}

void output_dannye_uchastnika(int len, char name[], char dannye_strela[])
{
	printf("%-*s", len, name);
	for (int i = 0; i < COLUMN; i++)
	{
		printf("| %c ", dannye_strela[i]);
	}
	printf("|\n");
}

void output_tabl(char dannye_strela[][COLUMN], char dannye_name[][NAME_LEN_DEF], int max_name_len)
{
	char name_tabl[NAME_LEN_DEF] = { "Имя Фамилия" };
	int index, len;

	if (max_name_len == 0)
	{
		index = index_uchastnika(dannye_name);
		len = strlen(dannye_name[index]);
	}
	else len = max_name_len;

	if (len < strlen(name_tabl)) len = strlen(name_tabl);
	printf("\n");
	for (int i = 0; i < len; i++)
	{
		printf(" ");
	}
	printf("|         1 сет         |         2 сет         |         3 сет         |         4 сет         |         5 сет         |         6 сет         |         7 сет         |         8 сет         |         9 сет         |        10 сет         |        11 сет         |        12 сет         |\n");
	printf("%-*s", len, name_tabl);
	printf("| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10| 11| 12| 13| 14| 15| 16| 17| 18| 19| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29| 30| 31| 32| 33| 34| 35| 36| 37| 38| 39| 40| 41| 42| 43| 44| 45| 46| 47| 48| 49| 50| 51| 52| 53| 54| 55| 56| 57| 58| 59| 60| 61| 62| 63| 64| 65| 66| 67| 68| 69| 70| 71| 72|\n");
	for (int i = 0; i < len; i++)
	{
		printf(" ");
	}
	printf("|                       |                       |                       |                       |                       |                       |                       |                       |                       |                       |                       |                       |\n");

	if (max_name_len == 0)
		output_dannye_uchastnika(len, dannye_name[index], dannye_strela[index]);
	else
		for (int i = 0; i < ROW; i++)
			output_dannye_uchastnika(len, dannye_name[i], dannye_strela[i]);

	printf("\n\nВведите любой символ, чтобы продолжить\n\n   > ");
	getchar();
	getchar();
}

int search_winner(char dannye_strela[][COLUMN], int* pmax)
{
	int index = 0;
	*pmax = 0;
	for (int i = 0; i < ROW; i++)
	{
		if (*pmax < col_strel(dannye_strela, i)) {
			*pmax = col_strel(dannye_strela, i);
			index = i;
		}
	}

	return index;
}

int search_set(char dannye_strela[][COLUMN], int index, int* pmax)
{
	int set = -1;
	*pmax = 0;
	char znach;
	for (int i = 0; i < COLUMN / 6; i++)
	{
		int sum = 0;
		for (int j = 0; j < 6; j++)
		{
			znach = dannye_strela[index][j + i * 6];
			sum += area_code_to_num(znach, 0);
			if (*pmax < sum)
			{
				*pmax = sum;
				set = 1 + i;
			}
		}
	}

	return set;
}

char* zona(int ochki)
{
	char rez[NAME_LEN_DEF];

	switch (ochki)
	{
	case 0:
		strcpy(rez, "- промахи");
		break;
	case 1:
		strcpy(rez, "в крайний белый");
		break;
	case 2:
		strcpy(rez, "в центральный белый");
		break;
	case 3:
		strcpy(rez, "в крайний чёрный");
		break;
	case 4:
		strcpy(rez, "в центральный чёрный");
		break;
	case 5:
		strcpy(rez, "в крайний голубой");
		break;
	case 6:
		strcpy(rez, "в центральный голубой");
		break;
	case 7:
		strcpy(rez, "в крайний красный");
		break;
	case 8:
		strcpy(rez, "в центральный красный");
		break;
	case 9:
		strcpy(rez, "в крайний золотой");
		break;
	case 10:
		strcpy(rez, "в центральный золотой");
		break;
	case 11:
		strcpy(rez, "в яблочко");
		break;
	default:
		break;
	}

	return rez;
}

char* mishen_vse(char dannye_strela[][COLUMN])
{
	int kol[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COLUMN; j++)
		{
			kol[area_code_to_num(dannye_strela[i][j], 1)]++;
		}
	}
	int zona_max = 0, ind_zona_max = 0;
	for (int i = 0; i < 12; i++)
	{
		if (kol[i] > zona_max)
		{
			zona_max = kol[i];
			ind_zona_max = i;
		}
	}

	return zona(ind_zona_max);
}

char* mishen_uchastnik(char dannye_strela[][COLUMN], char dannye_name[][NAME_LEN_DEF])
{
	int kol[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	int index;
	index = index_uchastnika(dannye_name);
	for (int j = 0; j < COLUMN; j++)
	{
		kol[area_code_to_num(dannye_strela[index][j], 1)]++;
	}
	int zona_max = 0, ind_zona_max = 0;
	for (int i = 0; i < 12; i++)
	{
		if (kol[i] > zona_max)
		{
			zona_max = kol[i];
			ind_zona_max = i;
		}
	}

	return zona(ind_zona_max);
}

int tabl_from_file(char dannye_strela[][COLUMN], char dannye_name[][NAME_LEN_DEF])
{
	FILE* in;
	char fname[NAME_LEN_DEF], buf[256];

	printf("\n-# Введите имя файла #-\n\n   > ");
	fseek(stdin, 0, SEEK_END);
	scanf("%[^\n]s", fname);

	in = fopen(fname, "rt");
	if (in == NULL)
	{
		printf("!Ошибка!");
		return 0;
	}
	int i = 0;
	while (!feof(in) && i < ROW)
	{
		fgets(buf, 256, in);
		strcpy(dannye_name[i], buf);
		for (int j = 0; j < NAME_LEN_DEF; j++)
		{
			if (dannye_name[i][j] == '\n')
			{
				dannye_name[i][j] = '\0';
				break;
			}
		}
		i++;
	}
	fgets(buf, 256, in);
	i = 0;
	while (!feof(in) && i < ROW)
	{
		fgets(buf, 256, in);
		strncpy(dannye_strela[i], buf, 72);
		i++;
	}
	fclose(in);

	return 1;
}