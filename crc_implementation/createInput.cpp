#include <bits/stdc++.h>

using namespace std;

int main()
{

    int bits;
    int number;
    string filename;

    cout << "Enter number of bits: ";
    cin >> bits;

    cout << "Enter the number of bit strings required: ";
    cin >> number;

    cout << "Enter file name: ";
    cin >> filename;

    ofstream input(filename);

    string m_x;

    srand(time(NULL));

    for (int i = 0; i < number; i++)
    {
        m_x = "";
        for (int i = 0; i < bits; i++)
        {
            m_x += ('0' + rand() % 2);
        }
        input << m_x << endl;
    }
}