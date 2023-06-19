#include <iostream>
#include <vector>

struct vec2
{
    double x;
    double y;
    
    vec2(double x, double y) : x(x), y(y)
    {
    }
};

using namespace std;

double calculateHexagonArea(const vector<vec2>& hexagonCoordinates)
{
    double area = 0.0;
    
    double height = hexagonCoordinates[2].y - hexagonCoordinates[1].y;
    double width = hexagonCoordinates[3].x - hexagonCoordinates[1].x;
    double rectangleArea = width * height;
    
    double triangleArea1 = abs(
        (
        hexagonCoordinates[0].x * (hexagonCoordinates[1].y - hexagonCoordinates[2].y)
        +
        hexagonCoordinates[1].x * (hexagonCoordinates[2].y - hexagonCoordinates[0].y)
        +
        hexagonCoordinates[2].x * (hexagonCoordinates[0].y - hexagonCoordinates[1].y)
        )
    /
    2.0
    );
    
    double triangleArea2 = abs(
        (
        hexagonCoordinates[3].x * (hexagonCoordinates[4].y - hexagonCoordinates[5].y)
        +
        hexagonCoordinates[4].x * (hexagonCoordinates[5].y - hexagonCoordinates[3].y)
        +
        hexagonCoordinates[5].x * (hexagonCoordinates[3].y - hexagonCoordinates[4].y)
        )
    /
    2.0
    );

    return rectangleArea + triangleArea1 + triangleArea2;
}



int main()
{
    vector<vec2> h1 = { vec2(340, 845), vec2(390, 790), vec2(390, 900), vec2(880, 790), vec2(880, 900), vec2(930, 845) };
    vector<vec2> h2 = { vec2(990, 845), vec2(1040, 790), vec2(1040, 900), vec2(1530, 790), vec2(1530, 900), vec2(1580, 845) };
    vector<vec2> h3 = { vec2(340, 985), vec2(390, 930), vec2(390, 1040), vec2(880, 930), vec2(880, 1040), vec2(930, 985) };
    vector<vec2> h4 = { vec2(990, 985), vec2(1040, 930), vec2(1040, 1040), vec2(1530, 930), vec2(1530, 1040), vec2(1580, 985) };

    cout << calculateHexagonArea(h1) << endl;
    cout << calculateHexagonArea(h2) << endl;
    cout << calculateHexagonArea(h3) << endl;
    cout << calculateHexagonArea(h4) << endl;

    return 0;
}
