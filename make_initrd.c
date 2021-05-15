#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_MAGIC 0xBF;

struct initrd_header
{
	unsigned char magic;
	char name[64];
	unsigned int offset;
	unsigned int length;
};

/* There is no sub-directory
 * First 4 byte stores number of file
 * Then there are 64 headers having name, offset and size of file
 * Then the actual data follows */
int main(char argc, char **argv)
{
	int nheaders = (argc - 1) / 2;
	struct initrd_header headers[64];

	unsigned int off = sizeof(struct initrd_header) * 64 + sizeof(int);

	for (int i = 0; i < nheaders; i++)
	{
		printf("Writing file %s->%s at 0x%0x\n", argv[i * 2 + 1], argv[i * 2 + 2], off);
		strcpy(headers[i].name, argv[i * 2 + 2]);
		headers[i].offset = off;
		FILE *stream = fopen(argv[i * 2 + 1], "r");
		if (stream == 0)
		{
			printf("Error: file not found: %s\n", argv[i * 2 + 1]);
			return 1;
		}
		fseek(stream, 0, SEEK_END);
		headers[i].length = ftell(stream);
		off += headers[i].length;
		headers[i].magic = HEADER_MAGIC;
		fclose(stream);
	}

	FILE *wstream = fopen("./initrd.img", "w");
	unsigned char *data = (unsigned char *)malloc(off);

	fwrite(&nheaders, sizeof(int), 1, wstream);
	fwrite(headers, sizeof(struct initrd_header), 64, wstream);

	for (int i = 0; i < nheaders; i++)
	{
		FILE *stream = fopen(argv[i * 2 + 1], "r");
		unsigned char *buff = (unsigned char*) malloc(headers[i].length);
		fread(buff, 1, headers[i].length, stream);
		fwrite(buff, 1, headers[i].length, stream);
		fclose(stream);
		free(buff);
	}

	fclose(wstream);
	free(data);

	return 0;
}