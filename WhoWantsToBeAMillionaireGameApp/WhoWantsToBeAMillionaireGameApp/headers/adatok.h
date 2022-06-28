#ifndef ADATOK_H
#define ADATOK_H
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::cout;
using std::cin;
using std::string;

class Adatok
{
    char diff;
    string cat;
    string question;
    string a1;
    string a2;
    string a3;
    string a4;
    string solution;

    public:

    int length(string input_file);
    
    Adatok* read(string* input_arr, int* length_arr, int length_all, string* category, int catLength);
    
    int getfirstoccurrence(string category, Adatok* arr, int length_all);

    string answer1(Adatok* arr, int idx);

    string answer2(Adatok* arr, int idx);

    string answer3(Adatok* arr, int idx);

    string answer4(Adatok* arr, int idx);

    string questions(Adatok* arr, int idx);

    string solutions(Adatok* arr, int idx);

    char difficulty(Adatok* arr, int idx);

    string category(Adatok* arr, int idx);

    int Random(int min, int max);
};

#endif