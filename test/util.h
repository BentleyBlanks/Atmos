#pragma once
#include <Atmos.h>

using namespace std;

void main()
{
    a3Log::print("-------------------------Lower Upper-------------------------\n");
    string lower = "whosyourdaddy", upper = "WHOSYOURDADDY";

    a3Log::print("lower to upper: %s\n", a3ToUpper(lower).c_str());
    a3Log::print("upper to lower: %s\n", a3ToLower(upper).c_str());

    int intValue = 10;
    float floatValue = 12.5f;
    double doubleValue = 13.5;
    vector<int> intList = {10, 20, 30};

    a3Log::print("-------------------------To String-------------------------\n");
    a3Log::print("%d %s\n", intValue, a3ToString(intValue).c_str());
    a3Log::print("%f %s\n", floatValue, a3ToString(floatValue).c_str());
    a3Log::print("%f %s\n", doubleValue, a3ToString(doubleValue).c_str());

    for(auto i : intList)
    {
        a3Log::print("%d ", i);
    }
    a3Log::print("%s\n", a3ToString(intList).c_str());

    a3Log::print("-------------------------String To-------------------------\n");
    string sint = "100", sfloat = "1000.0f", sdouble = "123.0", sbool = "false", schar = "abc";

    a3Log::print("%d\n", a3ToInt(sint));

    a3Log::print("%f\n", a3ToFloat(sfloat));

    a3Log::print("%f\n", a3ToDouble(sdouble));

    a3Log::print("%s\n", a3ToBool(sbool) == false ? "FALSE" : "TRUE");

    a3Log::print("%c\n", a3ToChar(schar));

    a3Log::print("-------------------------To Hex-------------------------\n");

    a3Log::print("%s\n", a3ToHex(intValue).c_str());
    a3Log::print("%s\n", a3ToHex(floatValue).c_str());
    a3Log::print("%s\n", a3ToHex(doubleValue).c_str());

    a3Log::print("%s\n", a3ToHex("1000").c_str());

    a3Log::print("-------------------------Hex To-------------------------\n");
    
    // Converts a hexadecimal representation of an string
    a3Log::print("%s\n", a3HexToString("61626364656667").c_str());

    a3Log::print("%f\n", a3HexToFloat("4060000000000000"));

    a3Log::print("%d\n", a3HexToInt("0xff"));

    a3Log::print("%c\n", a3HexToChar("61"));

    getchar();
}