#include <stdio.h>
#include <stdbool.h>



void decimalToBinary(int number, char* bits)
{
int index = 0;
int result = number;
while( result != 0)
{
int remainder = result % 2;
if(remainder > 0)
bits[index] = '1';
result = result / 2;
index++;
}
}// End Of The Decimal To Binary



void printBinary(char* bits)
{
for(int i = 31; i >= 0; --i)
{
printf("%c", bits[i]);
}
printf("\n");
}//End Of The 32 Bits Display To Console



void clearBinaryBits(char* bits)
{
decimalToBinary(0, &bits[0]);
}//End Of The Consolse Cleanup



void convertPrintToBinary(int number, char* bits)
{
clearBinaryBits(&bits[0]);
decimalToBinary(number, &bits[0]);
printBinary(&bits[0]);
}//End Of The Binary Array To Printed Into Console



int main (int argc, const char* argv[])
{
int number = 0;
bool ok = false;
char bits[32]=
{
'0', '0', '0', '0', '0', '0', '0', '0',
'0', '0', '0', '0', '0', '0', '0', '0',
'0', '0', '0', '0', '0', '0', '0', '0',
'0', '0', '0', '0', '0', '0', '0', '0'
};//Creation Of The 32 Bit Storage Unit
//------------------------
//Prompt For The Inserted Storge Number
//------------------------
while(!ok)
{
printf("\nDisplay a Number between 0 and 1,000\n");
scanf("%d",&number);
if (number < 0 || number > 1000)
{
printf("The Number You Entered Is Not Between 0 And 1,000\n");
}
else
{
ok = true;
}
}
printf("The Binary Number is: ");
convertPrintToBinary(number, &bits[0]);
printf("The number in The Hexidecimal Is: 0x%08X\n", number);
int result = ((number << 10) & 0xFFFFFC00) | 0x03FF;
printf("The Result In A Decimal Is: %d\n", result);
printf("The Result In The Hexidecimal Is: 0x%08X\n", result);
printf("The Result In Binary Is: ");
convertPrintToBinary(result, &bits[0]);
return 0;
}