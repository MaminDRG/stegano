#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iterator>
#include <cstdlib>
using namespace std;

typedef unsigned char byte;


#pragma pack(push, 1)
typedef struct
{
   byte   rgbBlue;
   byte   rgbGreen;
   byte   rgbRed;
}RGB;



typedef struct
{
    unsigned char   b1,b2;
    unsigned long   bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned long   bfOffBits;

    unsigned int    biSize;
    int             biWidth;
    int             biHeight;
    unsigned short  biPlanes;
    unsigned short  biBitCount;
    unsigned int    biCompression;
    unsigned int    biSizeImage;
    int             biXPelsPerMeter;
    int             biYPelsPerMeter;
    unsigned int    biClrUsed;
    unsigned int    biClrImportant;
}BMPHEADER;
#pragma pack(pop)

bool cipher(string &line, string OptAlf, int key){
	for (int i = 0; i < line.size(); i++){
		if (line[i] == ' ') continue;
		int indAlf = OptAlf.find(line[i]);
		if (indAlf < 0) { // esli simvol ne najden, to vyvoditsya oshibka i vypolnenie funkcii prekrashhaetsya
			cout << " neverno zadany parametry" << endl;
			return false;
		}
		else{
			if (abs(key) > OptAlf.size()) key = key % OptAlf.size();
			int index = (indAlf + key + OptAlf.size()) % OptAlf.size();
			line[i] = OptAlf[index];
		}
	}
	return true;
}

void file(string OptAlf, int key, string source, string dest){
	string line;
	vector<string> ::iterator it;
	vector <string> text;

	// esli dest - ne zadan, to prisvaivaem znachenie source
	bool goodAlf = true; // peremennaya dlya opredeleniya pravilnosti alfavita
	ifstream fin(source.c_str()); // sozdaem fajl dlya chteniya
	if (!fin.is_open())
		cout << source <<" fajl dlya chteniya ne najden" << endl;
	else{
		while (!fin.eof()){
			getline(fin, line); // schityvaem pervuyu stroku fajla
			if (cipher(line, OptAlf, key)) // funkciya dlya shifrovaniya stroki i proverki pravilnosti alfavita
				text.push_back(line); // zanosim zashifrovannuyu stroku v vektor
			else {
				goodAlf = false;
				break;
			}
		}
	}
	fin.close();
	if (goodAlf){
		ofstream fout(dest.c_str()); // sozdaem fajl dlya zapisi
		for (it = text.begin(); it < text.end(); it++) // zanosim vektor v fajl
			fout << *it << endl;
		fout.close();
	}
}

//kopirovanie pamyati
void memcpy(void* dest, void* src, size_t size)
{
    for(int i = 0; i < size; i++)
    {
        *((byte*)dest + i) = *((byte*)src + i);
    }
}






 void pack(string textfilepath, string bmpfilepath){
 	fstream bmpfile;
bmpfile.open(bmpfilepath.c_str(), ios::binary | ios::in);
if (bmpfile.fail())
{
cout << "oshibka bmp1\n";
exit(0);
}
//opredelyaem razmer faila
bmpfile.seekg(0, ios::end);
size_t bmpfileSize = bmpfile.tellg();
bmpfile.seekg(0, ios::beg); //
//zapisyvaem fail v ukazatel` p
byte *pbmpFile = new byte[bmpfileSize];
bmpfile.read((char*)pbmpFile, bmpfileSize);
bmpfile.close();


	fstream textfile;
textfile.open(textfilepath.c_str(), ios::binary | ios::in);
if (textfile.fail())
{
cout << "oshibka text1\n";
exit(0);
}
//opredelyaem razmer faila
textfile.seekg(0, ios::end);
unsigned short textfileSize = textfile.tellg();
textfile.seekg(0, ios::beg); //
//zapisyvaem fail v ukazatel` p
byte *ptextFile = new byte[textfileSize + 2];
textfile.read((char*)ptextFile + 2, textfileSize);
textfile.close();

BMPHEADER *bmpHeader = (BMPHEADER*)pbmpFile;
int rows = bmpHeader->biHeight;
int cols = bmpHeader->biWidth;

//proverka razmera texta
if (textfileSize + 2 >rows*cols) {
	cout << "slishkom bol`shoy text" << endl;
	exit(0);
}

RGB *currentLine = (RGB*)(pbmpFile + bmpHeader->bfOffBits);
//razmer stroki s vyravnivaniem do 4
int LineSize = 3 * cols + (4 - ((3 * cols) % 4)) % 4;

unsigned short* shortsize = &textfileSize;

memcpy(ptextFile, shortsize, 2);

zxcv:

 for (int i=0;i<rows;i++){
	for (int j=0;j<cols;j++){

		if(i * cols + j == textfileSize + 2)
		{
			bmpfile.open(bmpfilepath.c_str(), ios::binary | ios::out | ios::trunc);
			bmpfile.write((char*)pbmpFile,bmpfileSize);
			bmpfile.close();
			return;
		}

		//obnulyaem bity po sheme 2-3-3
		currentLine[j].rgbRed = currentLine[j].rgbRed >> 2;
		currentLine[j].rgbRed = currentLine[j].rgbRed << 2;

		currentLine[j].rgbGreen = currentLine[j].rgbGreen >> 3;
        currentLine[j].rgbGreen = currentLine[j].rgbGreen << 3;

        currentLine[j].rgbBlue = currentLine[j].rgbBlue >> 3;
        currentLine[j].rgbBlue = currentLine[j].rgbBlue << 3;

// zapis` texta v kanaly RGB
currentLine[j].rgbRed |= ptextFile[i*cols+j] & 3;
currentLine[j].rgbGreen |= (ptextFile[i*cols+j] & 28)>>2;
currentLine[j].rgbBlue|= (ptextFile[i*cols+j] & 224)>>5;






}
currentLine=(RGB*)((byte*)currentLine+LineSize);


 }
cout << "rabota okonchena " <<endl;
}








void unpack(string textfilepath, string bmpfilepath){
 	fstream bmpfile;
bmpfile.open(bmpfilepath.c_str(), ios::binary | ios::in);
if (bmpfile.fail())
{
cout << "oshibka bmp1\n";
exit(0);
}
//opredelyaem razmer faila
bmpfile.seekg(0, ios::end);
size_t bmpfileSize = bmpfile.tellg();
bmpfile.seekg(0, ios::beg); //
//zapisyvaem fail v ukazatel` p
byte *pbmpFile = new byte[bmpfileSize];
bmpfile.read((char*)pbmpFile, bmpfileSize);
bmpfile.close();



fstream textfile;
textfile.open(textfilepath.c_str(), ios::binary | ios::trunc | ios::out);
if (textfile.fail())
{
cout << "oshibka text1\n";
exit(0);
}
//privedenie tipov
BMPHEADER *bmpHeader = (BMPHEADER*)pbmpFile;
int rows = bmpHeader->biHeight;
int cols = bmpHeader->biWidth;

RGB *currentLine = (RGB*)(pbmpFile + bmpHeader->bfOffBits);
//razmer stroki s vyravnivaniem do 4
int LineSize = 3 * cols + (4 - ((3 * cols) % 4)) % 4;


unsigned short sizzze = 0;
byte* psizzze = (byte*)&sizzze;

 for (int i=0;i<rows;i++){
	for (int j=0;j<cols;j++){
		if(i * cols + j == 2)
		{
			//prinuditel`no vihodim iz 2-uh ciklov
			goto ahaha;
		}
        //razmer perenosim v file
		psizzze[i*cols+j] |=  (currentLine[j].rgbRed & 3);
        psizzze[i*cols+j] |=  (currentLine[j].rgbGreen & 7)<<2;
        psizzze[i*cols+j] |=  (currentLine[j].rgbBlue & 7)<<5;
     }
     //perehod na sled strochku
currentLine=(RGB*)((byte*)currentLine+LineSize);

}

ahaha:

    currentLine = (RGB*)(pbmpFile + bmpHeader->bfOffBits);

//string tekst;
char *tekst = new char[sizzze + 2];
//tekst.resize(sizzze + 2);
for(int i = 0; i < sizzze + 2 ; i++)
{
    tekst[i] = '\0';
}

 for (int i=0;i<rows;i++){
	for (int j=0;j<cols;j++){
		if(i * cols + j == sizzze + 2)
		{
			bmpfile.open(textfilepath.c_str(), ios::binary | ios::out | ios::trunc);
			//bmpfile.write((tekst.c_str()) + 2,sizzze);
			bmpfile.write(tekst + 2,sizzze);
			bmpfile.close();
			return;
		}
        //zapis` teksta
		tekst[i*cols+j] |=  currentLine[j].rgbRed & 3;
        tekst[i*cols+j] |=  (currentLine[j].rgbGreen & 7)<<2 ;
        tekst[i*cols+j] |=  (currentLine[j].rgbBlue & 7)<<5;
     }
currentLine=(RGB*)((byte*)currentLine+LineSize);

}




}




int main(int argc, char* argv[]){
	string OptType = "encode", textfile , bmpfile , source ;
	string OptAlf = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789";
	int key = 0;
	if (argc<3) {
		cout <<"parametry ne zadany" << endl;
		exit(1);
	}

textfile = argv[1];
OptType  = argv[2];
bmpfile  = argv[3];
key      = atoi(argv[4]);
source   = string(textfile);

if (OptType=="encode") {
	file(OptAlf, key, source, source );
	pack(textfile,  bmpfile);
}
if (OptType=="decode") {
    unpack(textfile,bmpfile);
    key=-key;
	file(OptAlf, key, source , source);

}

return 0;
}
