#include "file.h"

void CleanBuffer(byte* buffer)
{
	if (buffer == NULL)
	{
		error("Tried to Free Null Pointer.");
		return;
	}

	free(buffer);
}

byte* ReadBinary(char* fileName, uint32_t* pNumberOfBytes)
{
	FILE* pFile = NULL;

	fopen_s(&pFile, fileName, "rb");

	if (pFile == NULL)
	{
		error("Unable to open File.");
		return NULL;
	}

	fseek(pFile, 0, SEEK_END);
	*pNumberOfBytes = ftell(pFile);

	byte* buffer = calloc(*pNumberOfBytes, sizeof(byte));

	if (buffer == NULL)
	{
		error("Unable to allocate memory for file to read.");
		return NULL;
	}

	fseek(pFile, 0, SEEK_SET);

	fread(buffer, sizeof(byte), *pNumberOfBytes, pFile);

	fclose(pFile);

	return buffer;
}