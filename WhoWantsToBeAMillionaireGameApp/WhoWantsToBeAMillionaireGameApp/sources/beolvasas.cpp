#include "adatok.h"

int Adatok::length(string input_file)
{
    int n = 0;
    string line;
    ifstream file; 
    file.open(input_file, ifstream::in);

    while(getline(file, line))
    {
        ++n;
    }
    
    file.close();

    return n;
}

Adatok* Adatok::read(string* input_arr, int* length_arr, int length_all, string* category, int catLength)
{
    Adatok* arr = new Adatok[length_all];
    ifstream file;
    string line;
    int idx = 0;
    
    for(int i = 0; i < catLength; ++i)
    {
        file.open("text/" + input_arr[i], ifstream::in);

        for(int j = 0; j < length_arr[i]; ++j)
        {
            getline(file, line);
            arr[idx].question = line.substr(2, line.find('?') - 1);
            int counter = 0;
            size_t v1 = 0;
            size_t v2 = 0;
            size_t v3 = 0;

            for(size_t k = 0; k < line.length(); ++k)
            {
                if(line[k] == ',')
                {
                    switch(counter)
                    {
                        case 0:
                            v1 = k;
                            break;
                        case 1:
                            v2 = k;
                            break;
                        case 2:
                            v3 = k;
                            break;
                    }
                    ++counter;
                    if(counter == 3)
                    {
                        break;
                    }
                }
            }

            arr[idx].a1 = line.substr(line.find('?') + 13, v1 - (line.find('?') + 13));
            arr[idx].a2 = line.substr(v1 + 2, v2 - (v1 + 2));
            arr[idx].a3 = line.substr(v2 + 2, v3 - (v2 + 2));
            arr[idx].a4 = line.substr(v3 + 2, (line.find_last_of(':') - 10) - (v3 + 3));
            arr[idx].solution = line.substr(line.find_last_of(':') + 2, line.length() - (line.find_last_of(':') + 2));
            arr[idx].cat = category[i];
            arr[idx].diff = line[0];
            ++idx;
        }

        file.close();
    }

    return arr;
} 

int Adatok::getfirstoccurrence(string category, Adatok* arr, int length_all)
{
    int tmp = 0;

    for(int i = 0; i < length_all; ++i)
    {
        if(arr[i].cat == category)
        {
            tmp = i;
            break;
        }
    }

    return tmp;
}

string Adatok::answer1(Adatok* arr, int idx)
{
    return arr[idx].a1;
}

string Adatok::answer2(Adatok* arr, int idx)
{
    return arr[idx].a2;
}

string Adatok::answer3(Adatok* arr, int idx)
{
    return arr[idx].a3;
}

string Adatok::answer4(Adatok* arr, int idx)
{
    return arr[idx].a4;
}

string Adatok::questions(Adatok* arr, int idx)
{
    return arr[idx].question;
}

string Adatok::solutions(Adatok* arr, int idx)
{
    return arr[idx].solution;
}

char Adatok::difficulty(Adatok* arr, int idx)
{
    return arr[idx].diff;
}

string Adatok::category(Adatok* arr, int idx)
{
    return arr[idx].cat;
}

int Adatok::Random(int min, int max)
{
    ++max;
    return rand() % (max - min) + min;
}