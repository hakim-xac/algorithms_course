#include <iostream>
#include "simplex_table.h"
#include <string>

int main()
{
    setlocale(LC_ALL, "");

    using namespace std::literals;
    KHAS::HeaderVariable hv{ "x1"s, "x2"s, "x3"s, "x4"s, "x5"s, "-"s };
    KHAS::VerticalVariable vv{ "w1"s, "w2"s, "w3"s, "F"s };
    KHAS::ObjectiveFunction of{ {"x1", -5}, {"x2",-1}};

    KHAS::SimplexTable st{ std::move(hv), std::move(vv), std::move(of)
        , {4, 1, -1, 0, 0, 9
        , 3, 2, 0, -1, 0, 13
        , 2, 5, 0, 0, -1, 16
        , -9, -8, 1, 1, 1, -38}};

    st.start();


    return 0;
}