//------------------------------------------------------------------
// University of Central Florida
// COP3360 - Fall 2016
// Program Author: Kyle Gildea
//------------------------------------------------------------------

#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <stdlib.h>
#include <string.h>

typedef struct CRC
{
    char *mode;
    char *inputStringHex;
    char *inputStringBin;
    char *poly;
    char *pad;
    int padChars;
    char *crcVal;
    char *observedCRC;
}CRC;

void errorPrint(char *error);
CRC *initialSetup(char *modeUsed, char *stringToProcess);
char *hexStrtoBinStr(char *stringToProcess);
char *binStrtoHexStr(char *stringToProcess);
void printHeader (CRC *usedCRC);
void printLegibleBits(char *bitString);
void xor (CRC *given);
void crcCalc(CRC *given);
void crcOutput(CRC *given);
char binToHex(char *binStr);


int main(int argc, char **argv)

{
  CRC *given;
  char *temp = argv[2];
  
  for(int i =0; i < strlen(temp) - 1; i++)
  {
    if(temp[i] < '0' || (temp[i] > '9' && temp[i] < 'A') || temp[i] > 'F')
    {
      errorPrint("Invalid String Entered\n\n");
      return -1;
    }
  }

  if(strcmp(argv[1], "c") == 0 || strcmp(argv[1], "C") == 0)
  {
    given = initialSetup(argv[1], argv[2]);    
    printHeader(given);
  
    given->crcVal = malloc(sizeof(char) * (strlen(given->inputStringBin) + given->padChars + 1));
    strncpy(given->crcVal, given->inputStringBin, strlen(given->inputStringBin));
    strcat(given->crcVal, given->pad);
    crcCalc(given);
    crcOutput(given);
  }
  
  else if((strcmp(argv[1], "v") == 0 || strcmp(argv[1], "V") == 0))
  {
    given = initialSetup(argv[1], argv[2]);
    printHeader(given);
    
    given->crcVal = malloc(sizeof(char) * (strlen(given->inputStringBin) + given->padChars + 1));
    strncpy(given->crcVal, given->inputStringBin, strlen(given->inputStringBin) - given->padChars);
    strcat(given->crcVal, given->pad);
    crcCalc(given);
    crcOutput(given);
  }
  return 0;
}


void crcOutput(CRC *given)
{
  char *first = malloc(5 * sizeof(char));
  char *second = malloc(5 * sizeof(char));
  char *third = malloc(5 * sizeof(char));
  char *hex = malloc(4 * sizeof(char));
  char temp[12];

  
  strncpy(first, (given->crcVal) + (strlen((given->crcVal)) - 12), 4);
  strncpy(second, (given->crcVal) + (strlen((given->crcVal)) - 8), 4);
  strncpy(third, (given->crcVal) + (strlen((given->crcVal)) - 4), 4);
  
  first[5] = '\0';
  second[5] = '\0';
  third[5] = '\0';
  hex[3] = '\0'; 
  
  hex[0] = binToHex(first);
  hex[1] = binToHex(second);
  hex[2] = binToHex(third);
  
  printf("The CRC computed from the input ");
  printLegibleBits(given->inputStringBin);
  printf(" (bin) = %s (hex).\n",  hex);
  
  if(strcmp(given->mode, "v") == 0 || strcmp(given->mode, "V") == 0)
  {
    printf("Did the CRC check pass? (Yes or No): ");
    if(strcmp(given->observedCRC, hex) == 0)
      printf("Yes\n");
    else
      printf("No\n");
  }
}

char binToHex(char *binStr)
{
  int dec = (int)strtol(binStr, NULL, 2);
  
  switch(dec)
    {
      case 0:
        return '0';
        break;
      case 1:
        return '1';
        break;
      case 2:
        return '2';
        break;
      case 3:
        return '3';
        break;
      case 4:
        return '4';
        break;
      case 5: 
        return '5';
        break;
      case 6:
        return '6';
        break;
      case 7:
        return '7';
        break;
      case 8:
        return '8';
        break;
      case 9:
        return '9';
        break;
      case 10:
        return 'A';
        break;
      case 11:
        return 'B';
        break;
      case 12: 
        return 'C';
        break;
      case 13:
        return 'D';
        break;
      case 14:
        return 'E';
        break;
      case 15:
        return 'F';
        break;
      default:
        errorPrint("Invalid binary digit found");   
    };
}

 
void crcCalc(CRC *given)
{
  int i, j, bookmark;
  
  printf("The binary string difference after each XOR step of the CRC calculation:\n");
  printLegibleBits(given->crcVal);
    printf("\n");
  
  for(i = 0; i <= strlen(given->crcVal)  - strlen(given->poly); i++)
  {
    if(given->crcVal[i] == '1')
    {
      bookmark = i;
      for(j = 0; j < strlen(given->poly); j++)
      {
        if(given->crcVal[bookmark] == given->poly[j])
          given->crcVal[bookmark] = '0';
        else
          given->crcVal[bookmark] = '1';
        bookmark++;
        i = 0;
      }
      printLegibleBits(given->crcVal);
      printf("\n");
    }
  }
}

CRC *initialSetup(char *modeUsed, char *stringToProcess)
{
  CRC *given;
  given = malloc(sizeof(CRC));
  given->observedCRC= malloc(sizeof(char) * 4);
  char *p;
  
  
  if(given == NULL)
  {
    errorPrint("CRC Malloc Failed!\n");
  }
  
  else
  {
    given->mode = modeUsed;
    given->inputStringHex = stringToProcess;
    given->inputStringBin = hexStrtoBinStr(stringToProcess);
    given->poly = "1100010110011";
    given->pad = "000000000000";
    given->padChars = 12;
    
    if(strcmp(given->mode, "v") == 0 || strcmp(given->mode, "V") == 0)
    {
      p = &(given->inputStringHex[strlen(given->inputStringHex) - 3]);
      strncpy(given->observedCRC, p, 3);
      given->observedCRC[3] = '\0';
      given->inputStringHex[strlen(given->inputStringHex) - 3] = '\0';
    }
    else
      given->observedCRC = NULL;
  }
  return given;
}

void printHeader (CRC *usedCRC)
{
  int bitsPerChar = 4;
  
  printf("\n\nCIS 3360 CRC Tester by Kyle Gildea\n\n");
  
  if(strcmp(usedCRC->mode, "c") == 0 || strcmp(usedCRC->mode, "C") == 0)
  {
    printf("Mode of operation: calculation\n");
    printf("The input string (hex): %s\n", usedCRC->inputStringHex);
  }
  else if(strcmp(usedCRC->mode, "v") == 0 || strcmp(usedCRC->mode, "V") == 0) 
  {
    printf("Mode of operation: verification\n");
    printf("The input string (hex): %s%s\n", usedCRC->inputStringHex, usedCRC->observedCRC);
  }
    
    printf("The input string (bin): ");
    printLegibleBits(usedCRC->inputStringBin);
    printf("\n\nThe polynomial that was used (binary bit string: ");
    printLegibleBits(usedCRC->poly);
    if(strcmp(usedCRC->mode, "c") == 0 || strcmp(usedCRC->mode, "C") == 0)
      printf("\nNumber of zeroes that will be appended to the binary input: %d\n\n", usedCRC->padChars);
    else if(strcmp(usedCRC->mode, "v") == 0 || strcmp(usedCRC->mode, "V") == 0)
    {
      char *temp = hexStrtoBinStr(usedCRC->observedCRC);
      printf("\nThe CRC observed at the end of the input:");
      printLegibleBits(temp);
      printf(" (bin) = %s\n\n", usedCRC->observedCRC);
    }
}


void printLegibleBits(char *bitString)
{
  int i = 0;
  while(i < strlen(bitString))
  {
    if(i < 4 || (i > 4 && i % 4 != 0))
    
      printf("%c",bitString[i]);
    else if(i % 4 == 0)
    {
      printf("%c%c", ' ', bitString[i]);
    }
    i++;
  }
}

char *hexStrtoBinStr(char *stringToProcess)
{
  int bitsPerChar = 4, i = 0;
  char *binStr, binBuff, *badCharPtr;
  
  binStr = malloc(strlen(stringToProcess) * bitsPerChar * sizeof(char) + 1);
  binStr[0] = '\0';
  
  for(i; i < strlen(stringToProcess); i++)
  {
    switch(stringToProcess[i])
    {
      case '0':
        strcat(binStr, "0000");
        break;
      case '1':
        strcat(binStr, "0001");
        break;
      case '2':
        strcat(binStr, "0010");
        break;
      case '3':
        strcat(binStr, "0011");
        break;
      case '4':
        strcat(binStr, "0100");
        break;
      case '5': 
        strcat(binStr, "0101");
        break;
      case '6':
        strcat(binStr, "0110");
        break;
      case '7':
        strcat(binStr, "0111"); 
        break;
      case '8':
        strcat(binStr, "1000");
        break;
      case '9':
        strcat(binStr, "1001");
        break;
      case 'A':
        strcat(binStr, "1010");
        break;
      case 'B':
        strcat(binStr, "1011");
        break;
      case 'C': 
        strcat(binStr, "1100");
        break;
      case 'D':
        strcat(binStr, "1101");
        break;
      case 'E':
        strcat(binStr, "1110");
        break;
      case 'F':
        strcat(binStr, "1111");
        break;
      default:
        errorPrint("Invalid hexadecimal digit found");     
    };
  }
  return binStr;
}

//Prints error string
void errorPrint(char *error) 
{ 
  fprintf(stderr, "%s", error);
  
}
