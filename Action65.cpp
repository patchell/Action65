// Action65.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Global.h"

int main(int argc, char *argv[])
{
	CParser ActionParser;
	FILE* pIn, *pOut;

    fprintf(stderr, "Action65 Ver 0.01\n");
	printf("Open %s amd %s\n", argv[1], argv[2]);
	fopen_s(&pIn,argv[1],"r");
	fopen_s(&pOut, argv[2], "w");
	ActionParser.Create(pIn, pOut);
	ActionParser.Run();
	fprintf(stderr, "Done\n");
}

