#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define HeaderSize 62
#define MaxPathLen 200
#define MaxSliceLen 120

typedef struct
{
	uint8_t* header;
	uint8_t* image;
	uint8_t* data;
	int file_size;
	int header_size;
	int width;
	int height;
} Image;


Image LoadBMP(FILE* f)
{
	Image res;
	uint8_t* head = malloc(HeaderSize * sizeof(uint8_t));
	res.header_size = HeaderSize;
	fread(head, 1, HeaderSize, f);

	uint32_t offset = head[10] + (((uint8_t)head[11]) << 8) + (((uint8_t)head[12]) << 16) + (((uint8_t)head[13]) << 24);
	int w = head[18] + (((uint8_t)head[19]) << 8) + (((uint8_t)head[20]) << 16) + (((uint8_t)head[21]) << 24);
	int h = head[22] + (((uint8_t)head[23]) << 8) + (((uint8_t)head[24]) << 16) + (((uint8_t)head[25]) << 24);

	if (offset > HeaderSize)
	{
		free(head);
		head = malloc(offset * sizeof(uint8_t));
		fseek(f, 0, SEEK_SET);
		fread(head, 1, offset, f);
		res.header_size = (int)offset;
	}

	printf("offset = %d\n", offset);

	int lineSize = (w / 32) * 4;
	if (w % 32 != 0)
		lineSize += 4;

	int fileSize = lineSize * h;

	uint8_t* img = malloc(w * h);
	uint8_t* data = malloc(fileSize);

	fseek(f, (long)offset, SEEK_SET);
	fread(data, 1, fileSize, f);


	printf("w = %d; h = %d \n", w, h);
	printf("bytes in  %d; total size: %d \n", lineSize, fileSize);

	int cur_byte;
	for (int i = 0, reversed_i = h - 1; i < h; i++, reversed_i--)
	{
		for (int j = 0; j < w; j++)
		{
			cur_byte = j / 8;
			uint8_t data_byte = data[i * lineSize + cur_byte];
			uint8_t mask = 0x80 >> j % 8; // 2^7
			int pos = reversed_i * w + j;
			img[pos] = (data_byte & mask) ? 1 : 0;
		}
	}

	res.width = w;
	res.height = h;
	res.header = head;
	res.image = img;
	res.data = data;
	res.file_size = fileSize;
	return res;
}

void FreeBMP(Image* image)
{
	free(image->header);
	free(image->image);
	free(image->data);
}

void MatrixToData(int n, int m, uint8_t** matrix, uint8_t* data)
{
	int cur_pos = 0;
	uint8_t cur_byte = 0;

	for (int i = n; i >= 1; i--)
	{
		for (int j = 1; j <= m; j++)
		{
			uint8_t mask = 0x80 >> (j - 1) % 8;
			if (matrix[i][j] == '0')
				cur_byte = cur_byte | mask;
			if ((j % 8) == 0 || j == m)
			{
				data[cur_pos] = cur_byte;
				cur_pos++;
				cur_byte = 0;
				mask = 0x80;
			}
		}
		while ((cur_pos) % 4 != 0)
			cur_pos++;
	}
}

uint8_t** NextGeneration(int n, int m, uint8_t** cur)
{
	uint8_t** target;
	target = (uint8_t**)malloc((n + 2) * sizeof(uint8_t*));
	for (int i = 0; i < n + 2; i++)
		target[i] = (uint8_t*)malloc((m + 2) * sizeof(uint8_t));

	for (int i = 1; i < n + 1; i++)
	{
		for (int j = 1; j < m + 1; j++)
		{
			int cnt = 0;
			for (int k = -1; k <= 1; k++)
				for (int p = -1; p <= 1; p++)
					if (cur[i + k][j + p] == '1')
						cnt++;
			if (cur[i][j] == '1')
				cnt--;
			target[i][j] = ((cur[i][j] == '1' && (cnt >= 2 && cnt <= 3)) || (cur[i][j] == '0' && cnt == 3) ? '1' : '0');
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cur[i][j] = target[i][j];

	for (size_t i = 0; i < n + 2; i++)
		free(target[i]);
	free(target);
	return cur;
}

int main(int argc, char* argv[]) {

	if (argc < 5 || argc > 9 || (argc % 2 == 0))
	{
		printf("Wrong number of parameters: %d\n", argc);
		return 1;
	}

	char* file_name = NULL;
	char* dir_name = NULL;
	uint64_t max_iter = (uint64_t)INFINITY - 1;
	uint32_t frequency = 1;

	for (int i = 1; i < argc; i += 2)
	{
		if (strcmp(argv[i], "--input") == 0)
		{
			file_name = argv[i + 1];
			printf("Init bmp file: %s\n", file_name);
		}
		else if (strcmp(argv[i], "--output") == 0)
		{
			dir_name = argv[i + 1];
			printf("Output directory: %s\n", dir_name);
		}
		else if (strcmp(argv[i], "--max_iter") == 0)
		{
			max_iter = atoi(argv[i + 1]);
			printf("Max iterations: %lld\n", max_iter);
		}
		else if (strcmp(argv[i], "--dump_freq") == 0)
		{
			frequency = atoi(argv[i + 1]);
			printf("Frequency: %u\n", frequency);
		}
		else
		{
			printf("Wrong parameter: %s\n", argv[i]);
			return 1;
		}
	}

	FILE* input_file = NULL;
	if (file_name)
		input_file = fopen(file_name, "rb");
	if (input_file == NULL)
	{
		printf("Can't open file %s\n", file_name);
		return 1;
	}

	Image bmp_struct = LoadBMP(input_file);

	int n = bmp_struct.height;
	int m = bmp_struct.width;
	uint8_t** matrix;

	matrix = (uint8_t**)malloc((n + 2) * sizeof(uint8_t*));
	for (int i = 0; i < n + 2; i++)
		matrix[i] = (uint8_t*)malloc((m + 2) * sizeof(uint8_t));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (bmp_struct.image[i * m + j])
				matrix[i + 1][j + 1] = '0';
			else
				matrix[i + 1][j + 1] = '1';

	uint64_t iter = 0;
	uint32_t freq_counter = 0;
	int file_counter = 0;
	char* generation_name = "generation";
	uint8_t** cur_matrix;
	cur_matrix = matrix;
	while (iter < max_iter)
	{
		cur_matrix = NextGeneration(n, m, cur_matrix);

		freq_counter++;
		if (freq_counter == frequency)
		{
			FILE* output_file;
			freq_counter = 0;
			file_counter++;

			char file_path[MaxPathLen] = "";
			char cur_generation[MaxSliceLen] = "";
			strcpy(cur_generation, generation_name);
			int length = snprintf(NULL, 0, "%d", file_counter);
			char* str = malloc(sizeof(char) * (length + 1));
			snprintf(str, length + 1, "%d", file_counter);
			printf("%s\n", str);
			strcat(cur_generation, str);
			printf("Cur generation = %s\n", cur_generation);
			strcat(file_path, dir_name);
			strcat(file_path, "\\");
			strcat(file_path, cur_generation);
			strcat(file_path, ".bmp\0");
			printf("%s\n", file_path);

			output_file = fopen(file_path, "wb");
			if (output_file == NULL)
			{
				printf("Can't open file %s\n", file_path);
				return 1;
			}
			printf("Fine\n");
			MatrixToData(n, m, cur_matrix, bmp_struct.data);
			fwrite(bmp_struct.header, sizeof(uint8_t), bmp_struct.header_size, output_file);
			fwrite(bmp_struct.data, sizeof(uint8_t), bmp_struct.file_size, output_file);
			printf("Make image slice\n");
			fclose(output_file);
		}
		iter++;
	}

	printf("Completed successfully\n");
	fclose(input_file);
	for (size_t i = 0; i < n + 2; i++)
		free(matrix[i]);
	free(matrix);
	FreeBMP(&bmp_struct);
	return 0;
}