/*
Copyright (C) <2023> <Hyeonjun Park(202301996, RICE, CNU)>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>
*/

/*
 * @This program employs to merge all source files as one text file.
 * @author Hyeonjun Park(RICE, CNU) <RICE202301996@o.cnu.ac.kr>
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <Windows.h>

// filenaming
void get_filename(char name[15])
{
	char datetime[6] = { '\0' };
	int loa = -1;
	int idx = 0;

	printf("Input the assignment's date(mmdd) (0: datetime now)\n");
	scanf("%s", datetime);

	if (!(strcmp(datetime, "0"))) {
		time_t t = time(NULL);     // get datetime now
		struct tm tm = *localtime(&t);     // divide datetime using struct tm

		sprintf(datetime, "%02d%02d", tm.tm_mon + 1, tm.tm_mday);
	}

	strcpy(name, datetime);

	printf("Lab(0) or Assgn(1)?\n");
	scanf("%d", &loa);
	if (loa == 0) {
		name[4] = 'L';
		name[5] = 'a';
		name[6] = 'b';
	}
	else {
		name[4] = 'A';
		name[5] = 's';
		name[6] = 's';
		name[7] = 'g';
		name[8] = 'n';
	}

	idx = strlen(name);

	name[idx] = '.';
	name[++idx] = 't';
	name[++idx] = 'x';
	name[++idx] = 't';
	name[++idx] = '\0';
}

int compare(const void* a, const void* b) {
	return strcmp(*(const char**)a, *(const char**)b);
}

void merge(char name[15])
{
	struct dirent* de = NULL;  // Pointer for directory entry

	// opendir() returns a pointer of DIR type.
	DIR* dr = opendir(".");

	if (dr == NULL) {
		printf("Could not open current directory\n");
		exit(1);
	}

	FILE* outputFile = fopen(name, "w");
	if (outputFile == NULL) {
		printf("Could not open output file\n");
		closedir(dr);
		exit(1);
	}

	// Step 1: Count the number of .c files and store their names in an array
	char* files[1000];
	int fileCount = 0;

	while ((de = readdir(dr)) != NULL) {
		char* ext = strrchr(de->d_name, '.');
		if (ext != NULL && strcmp(ext, ".c") == 0) {
			files[fileCount] = malloc(strlen(de->d_name) + 1);
			strcpy(files[fileCount], de->d_name);
			fileCount++;
		}
	}

	// Step 2: Sort the array of file names
	qsort(files, fileCount, sizeof(char*), compare);

	// Step 3: Merge the files in the sorted order
	for (int i = 0; i < fileCount; i++) {
		FILE* inputFile = fopen(files[i], "r");
		if (inputFile != NULL) {
			char line[256];
			while (fgets(line, sizeof(line), inputFile) != NULL) {
				fputs(line, outputFile);
			}
			fputs("\n\n", outputFile);  // Add newlines between files
			fclose(inputFile);
		}
		free(files[i]);  // Free the memory allocated for the file name
	}

	printf("Merging complete.\n");
	fclose(outputFile);
	closedir(dr);
	Sleep(3000);
}

int main(void)
{
	char name[15] = { '\0' };

	get_filename(name);
	merge(name);

	return 0;
}
