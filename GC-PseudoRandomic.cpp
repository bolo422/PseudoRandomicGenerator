#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>  
#include <stdlib.h>     
#include <time.h> 
#include <random>

using namespace std;


class PseudoRandomicGenerator
{
public:

	PseudoRandomicGenerator() {}
	~PseudoRandomicGenerator() {}

    //Convert INT to Float, it supports up to 6 digits whitout rounding the result in the cout.
    // This isn't the best way to do it, but after finishing the code I have tried to adapt it...
    //... to divindg by the LCG %(m) or 100000, but the Exponencial and Normal methods don't work properly...
    //... probably I have messed up in the Expo and Normal code, but using the intToFloat method, it works
    float intToFloat(int i);

    // Using ZX81 parameters. I choose those because of the limitation for the intToFloat function
    // since it's M parameter have less than 6 digits.
    int LCG();

    //Float LCG version. Use the same int list, but convert to float before return.
    float floatLCG();

    //Exponencial, I choose the parameters based on the given avarage (7), but that will work
    //for approximate numbers, like on a real scenario where 7 is a interval for a real probleam.
    float* Exponencial(float lambda);


    //Normal
    float* Normal(float mean, float deviation);


private:
    
    static const int LCGArraySize = 1000;
    int LCGRandArray[LCGArraySize] = {}; //psuedo randomic array
    bool LCGDone = false; //control if the array is empty

    static const int exponencialArraySize = 20;
    float exponencialArray[exponencialArraySize] = {};

    static const int normalArraySize = 20;
    float normalArray[normalArraySize] = {};
};

float PseudoRandomicGenerator::intToFloat(int i)
{
	string s = "0." + to_string(i);
	return stof(s);
}

int PseudoRandomicGenerator::LCG()
{
    // if the array is empty, fill it. (optimization)
    if (!LCGDone) {
        for (int i = 0; i < LCGArraySize; i++)
        {
            static int X = 0;
            const int A = 75;
            const int C = 74;
            const int M = 65537;

            X = (A * X + C) % M;
            LCGRandArray[i] = X;

            //Uncomment to visualize the Exponencial array in the prompt
            //cout << X << endl;
        }
        LCGDone = true;
    }

    //chose random element from the LCG array
    srand(time(NULL));
    return LCGRandArray[rand() % LCGArraySize];
}

float PseudoRandomicGenerator::floatLCG()
{
    if (!LCGDone) { LCG(); } // check if LCG already fullfill the list

    //chose random element from the LCG array and convert to float
    srand(time(NULL));
    return intToFloat(LCGRandArray[rand() % LCGArraySize]);

}

float* PseudoRandomicGenerator::Exponencial(float lambda)
{
    const float units = floatLCG() * exponencialArraySize; // units to distribute, I used the LCG pseudo-randomic to randomize
    //cout << "Units: " << units << endl;
    const int nTests = 10000;  //number of tests

    default_random_engine generator;
    exponential_distribution<double> distribution(lambda);

    float p[exponencialArraySize] = {};

    for (int i = 0; i < nTests; ++i)
    {
        double number = distribution(generator);
        if (number < 1.0) ++p[int(exponencialArraySize * number)];
    }

    for (int i = 0; i < exponencialArraySize; ++i)
    {
        exponencialArray[i] = p[i] * units / nTests;

        //Uncomment to visualize the Exponencial array in the prompt
        //cout << i+1 << ": " << exponencialArray[i] << endl;
    }

    return exponencialArray;
}

float* PseudoRandomicGenerator::Normal(float mean, float deviation)
{
    const float units = floatLCG() * normalArraySize; // units to distribute, I used the LCG pseudo-randomic to randomize
    //cout << "Units: " << units << endl;
    const int nTests = 10000;  //number of tests

    default_random_engine gen;
    normal_distribution<double> dis(mean, deviation);

    float p[normalArraySize] = {};

    for (int i = 0; i < nTests; ++i)
    {
        double number = dis(gen);
        ++p[int(number)];
    }

    for (int i = 0; i < normalArraySize; ++i)
    {
        normalArray[i] = p[i] * units / nTests;
        //Uncomment to visualize the Exponencial array in the prompt
        //cout << i+1 << ": " << normalArray[i] << endl;
    }

    return normalArray;
}




class Tests
{
private:
    PseudoRandomicGenerator rand;
public:
    Tests(){}
    ~Tests(){}

    //Call Exponential function to generate 20 numbers and print them.
    void ExponencialTest(float lambda);

    //Call Normal function to generate 20 numbers and print them.
    void NormalTest(float mean, float deviation);

};

void Tests::ExponencialTest(float lambda)
{
    //EXPONENCIAL TEST
    float* array = new float[20];
    array = rand.Exponencial(lambda);

    cout << "\n\nExponencial Array: \n";
    for (int i = 0; i < 20; i++)
    {
        cout << i+1 << ": " << array[i] << endl;
    }
}

void Tests::NormalTest(float mean, float deviation)
{
    //NORMAL TEST

    float* array2 = new float[20];
    array2 = rand.Normal(mean, deviation);

    cout << "\n\nNormal Array: \n";
    for (int i = 0; i < 20; i++)
    {
        cout << i+1 << ": " << array2[i] << endl;
    }
}



int main()
{
    Tests tests;
    tests.ExponencialTest(7.0f);
    tests.NormalTest(10.0f, 2.0f);
}