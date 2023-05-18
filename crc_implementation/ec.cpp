#include <bits/stdc++.h>

using namespace std;

class Data
{
    // consists of only zeros and ones
    string bit;

public:
    // constructors
    Data(const Data &s)
    {
        this->bit = s.bit;
    }
    Data(int length)
    {
        bit = "";
        for (int i = 0; i < length; i++)
        {
            bit += '0';
        }
    }
    Data(string s)
    {
        bit = s;
    }
    bool isZero()
    {
        bool is_zero = true;
        for (char c : bit)
            if (c == '1')
                is_zero = false;
        return is_zero;
    }
    // gets the value of bit string
    string get()
    {
        return bit;
    }
    // sets the bit string to given string
    void set(string b)
    {
        bit = b;
    }
    // left shift
    void ls(int n)
    {
        for (int i = 0; i < n; i++)
        {
            bit += '0';
        }
    }
    // extend string to length k
    void ext(int k)
    {
        int l = bit.length();
        reverse(bit.begin(), bit.end());
        for (int i = 0; i < k - l; i++)
        {
            bit += '0';
        }
        reverse(bit.begin(), bit.end());
    }
    // gets actual data length without leading zeros
    int trl()
    {
        int i = 0;
        while (i < bit.size() && bit[i] != '1')
            i++;
        return bit.size() - i;
    }
    // counts one's present
    int countOnes()
    {
        int cnt = 0;
        for (int i = 0; i < bit.size(); i++)
            if (bit[i] == '1')
                cnt++;
        return cnt;
    }
    // xor - overloading the operator for custom definition
    Data operator^(Data b)
    {
        this->ext(max(b.get().size(), bit.size()));
        b.ext(max(b.get().size(), bit.size()));
        string x = "";
        for (int i = 0; i < bit.length(); i++)
        {
            x += (((bit[i] - '0') ^ (b.get()[i] - '0')) + '0');
        }
        return Data(x);
    }
    // division - overloading the operator for custom definition
    Data operator%(Data b)
    {
        Data temp = Data(b);
        int t = trl();
        t -= b.get().size();
        if (t >= 0)
        {
            temp.ls(t);
            temp = temp ^ (*this);
            return temp % b;
        }
        else
            return (*this);
    }
};

// CRC alogrithm
Data getCRC(Data Mx, Data Gx)
{
    // x^r*M(x)
    Mx.ls(Gx.get().size() - 1);
    // Remainder
    Data Rx = Mx % Gx;
    // T(x)
    return Mx ^ Rx;
}

//checks if the remainder is zero
bool CRCcheck(Data Tx, Data Gx)
{
    Data Rx = Tx % Gx;
    return Rx.isZero();
}

//helper function to create bit errors
Data bitError(Data Tx, int n)
{

    vector<int> ads;

    string s = Tx.get();

    for (int i = 0; i < s.length(); i++)
        ads.push_back(i);

    int randome = n;
    int len = ads.size();

    while (randome--)
    {
        int i = rand() % len;
        s[ads[i]] = (s[ads[i]] - '0') ^ 1 + '0';

        swap(ads[len - 1], ads[i]);
        len--;
    }

    return Data(s);
}

//helper function to create brusty errors
Data brustErrors(Data Tx, int start_index, int len)
{
    string s = Tx.get();

    for (int i = s.size() - start_index; i < (s.size() - start_index) + len; i++)
    {
        if (i >= s.size())
        {
            cout << "Accessing illegal position" << endl;
            exit(1);
        }
        s[i] = (s[i] - '0') ^ 1 + '0';
    }

    Tx.set(s);

    return Tx;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    // parsing the arguments
    if (argc != 3)
    {
        cout << "Wrong number of arguments" << endl;
        return 0;
    }

    Data Gx = Data("100000111");

    vector<Data> transmitted;
    vector<Data> in;

    ifstream input(argv[1]);
    string x;

    while (getline(input, x))
    {
        Data Mx = Data(x);
        in.push_back(Mx);
        transmitted.push_back(getCRC(Mx, Gx));
    }

    ofstream output(argv[2]);

    //getting the crc
    for (int i = 0; i < in.size(); i++)
    {
        output << "Input: " << in[i].get() << endl;
        output << "CRC: " << transmitted[i].get() << endl;
    }

    // introducing bit errors
    for (int i = 0; i < in.size(); i++)
    {
        for (int j = 1; j <= 10; j++)
        {
            output << "Original String: " << in[i].get() << endl;
            output << "Original String with CRC: " << transmitted[i].get() << endl;
            Data Ex = bitError(transmitted[i], 2*j+1);
            output << "Corrupted String: " << Ex.get() << endl;
            output << "Number of Errors Introduced: " << 2*j+1 << endl;
            bool fail = !CRCcheck(Ex, Gx);
            output << "CRC Check: ";
            if(fail) output << "Failed" << endl;
            else output << "Passed" << endl;
        }
    }

    // introducing bursty errors
    for (int i = 0; i < in.size(); i++)
    {
        for (int j = 101; j <= 109; j+=2)
        {
            output << "Original String: " << in[i].get() << endl;
            output << "Original String with CRC: " << transmitted[i].get() << endl;
            Data Ex = brustErrors(transmitted[i], j, 6);
            bool fail = !CRCcheck(Ex, Gx);
            output << "Corrupted String: " << Ex.get() << endl;
            output << "Brusty Error at bit index: " << j << endl;
            output << "Brusty Error of length: " << 6 << endl;
            output << "CRC Check: ";
            if(fail) output << "Failed" << endl;
            else output << "Passed" << endl;
        }
    }

    input.close();
    output.close();

    return 0;
}
