//Mark Mondt mjm170130

#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

void Display(char*, int, int);

void GetLengths(fstream&, int&, int&);

void Input(fstream&, char*, int);

void Output(fstream&, char*, int, int);

void UpdateBirthDeath(char*, int*, int, int);

void UpdatePos(char*, int*, int);

int main()
{
   int RowLength = 0, TotLength = 0;
   int Gens = 0;

   fstream File;

   File.open("simbac.txt");

   if (File.good())
   {
      File.close();

      GetLengths(File, RowLength, TotLength);

      //create dynamic arrays
      char* Pos = new char [TotLength];
      int* BirthDeath = new int [TotLength];

      Input(File, Pos, TotLength);

      //receive and validate number of generations
      cout << "Simulate How Many Generations? (1-10)" << endl;
      cin >> Gens;

      while (Gens < 1 || Gens > 10)
      {
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(),'\n');

         cout << "Please enter a number between 1 and 10" << endl;
         cin >> Gens;
      }
      cout << endl;

      Display(Pos, RowLength, TotLength);

      //calculate and display changes through each generation
      for (int i = 0; i < Gens; i++)
      {
         UpdateBirthDeath(Pos, BirthDeath, RowLength, TotLength);

         UpdatePos(Pos, BirthDeath, TotLength);

         Display(Pos, RowLength, TotLength);
      }

      Output(File, Pos, RowLength, TotLength);

      //delete dynamic arrays
      delete [] Pos;
      delete [] BirthDeath;

   }
   else
   {
      cout << "There was an error opening the file" << endl;
      File.close();
   }

   return 0;
}



//display current contents of the array
void Display(char* Pos, int Row, int totlength)
{
   //step through each element of Pos and display it
   for (int i = 1; i <= totlength; i++)
   {
      cout << *Pos;

      //output newline at the end of each row
      if (i%Row==0)
         cout << endl;
      Pos++;
   }

   //return pointer to correct position
   Pos -= totlength;

   cout << endl;
}



//open file and calculate row and total length
void GetLengths(fstream& File, int& Row, int& totlength)
{
   char ReadIn;

   //open simbac.txt
   File.open("simbac.txt", ios::in | ios::binary);

   while (!File.eof())
   {
      File.get(ReadIn);

      //record row length after the first newline
      if (Row == 0 && ReadIn == '\n')
         Row = totlength;

      //increment total length if a character other than newline is encountered
      if (ReadIn != '\n' && ReadIn != '\r')
         totlength++;
   }

   //eliminate potential junk character at the end of file
   if (totlength % Row == 1)
       totlength--;

   //update row length in case only one row is present
   if (Row==0)
      Row = totlength;

   //close the file
   File.close();
}



//reads the contents of simbac.txt into the Pos array
void Input(fstream& File, char* Pos,int length)
{
   char ReadIn;
   int counter = 0;

   //open simbac.txt
   File.open("simbac.txt", ios::in | ios::binary);

   while (!File.eof())
   {
      File.get(ReadIn);

      //read in characters that are not newlines
      if (ReadIn != '\r' && ReadIn != '\n' && counter < length)
      {
         *Pos = ReadIn;
         Pos++;
         counter++;
      }

   }

   Pos-=length;

   //close the file
   File.close();
}



//output final Pos elements to output file
void Output(fstream& File, char* Pos, int Row, int totlength)
{
   //open output file
   File.open("simbac.txt", ios::out);

   //step through each element of Pos and display it
   for (int i = 1; i <= totlength; i++)
   {
      File << *Pos;

      //output newline at the end of each row
      if (i%Row==0)
         File << '\n';
      Pos++;
   }

   //close file
   File.close();

   //return pointer to correct position
   Pos -= totlength;
}



//find where births, deaths, or neither occur
void UpdateBirthDeath(char* Pos, int* BirthDeath, int Row, int totlength)
{
   //create pointers to correspond to the adjacent cells
   char* UpLeft = Pos - Row - 1;
   char* Up = Pos - Row;
   char* UpRight = Pos - Row +1;
   char* Left = Pos - 1;
   char* Right = Pos + 1;
   char* DownLeft = Pos + Row - 1;
   char* Down = Pos + Row;
   char* DownRight = Pos + Row + 1;

   //create pointer to refer to the start of the array
   char* Orig = Pos;

   int counter;

   //step through every cell of the array
   for (int i = 1; i <= totlength; i++)
   {
      counter = 0;

      if (Up >= Orig)
         if (*Up == '*')
            counter++;

      if (Down < Orig + totlength)
         if (*Down == '*')
            counter++;

      if ((Left + Row - Orig) / Row == (Pos + Row - Orig) / Row)
         if (*Left == '*')
            counter++;

      if ((Right + Row - Orig) / Row == (Pos + Row - Orig) / Row)
         if (*Right == '*')
            counter++;

      if (UpLeft >= Orig && (UpLeft + Row - Orig) / Row == (Up + Row - Orig) / Row)
         if (*UpLeft == '*')
            counter++;

      if (UpRight >= Orig && (UpRight + Row - Orig) / Row == (Up + Row - Orig) / Row)
         if (*UpRight == '*')
            counter++;

      if (DownLeft < Orig + totlength && (DownLeft + Row - Orig) / Row == (Down + Row - Orig) / Row)
         if (*DownLeft == '*')
            counter++;

      if (DownRight < Orig + totlength && (DownRight + Row - Orig) / Row == (Down + Row - Orig) / Row)
         if (*DownRight == '*')
            counter++;

      //record birth, death, or neutral in that cell
      if (counter > 3)
         *BirthDeath = -1;
      else if (counter < 2)
         *BirthDeath = -1;
      else if (counter == 3)
         *BirthDeath = 1;
      else
         *BirthDeath = 0;

      //increment pointers
      Pos++;
      BirthDeath++;

      UpLeft++;
      Up++;
      UpRight++;
      Left++;
      Right++;
      DownLeft++;
      Down++;
      DownRight++;
   }

   Pos -= totlength;
   BirthDeath -=totlength;
}



//record births and deaths in Pos array
void UpdatePos(char* Pos, int* BirthDeath, int totlength)
{
   for (int i = 1; i <= totlength; i++)
   {
      if (*BirthDeath == 1)
         *Pos = '*';

      if (*BirthDeath == -1)
         *Pos = ' ';

      Pos++;
      BirthDeath++;
   }

   Pos -= totlength;
   BirthDeath -= totlength;
}
