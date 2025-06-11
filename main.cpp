#include<iostream>
#include<vector>
#include<string>

using namespace std;

string convert_number_to_string(int n){
    if(n <= 26){
        string s;
        char c = n - 1 + 'A';
        s.push_back(c);
        return s;
    }

    else if(n <= 26 + 26 * 26){
        string s;
        n -= 26;
        int index = 0;
        int t = 0;
        while(t < n){
            t += 26;
            ++index;
        }
        char c = index - 1 + 'A';
        s.push_back(c);
        t -= 26;
        n -= t;
        c = n - 1 + 'A';
        s.push_back(c);
        return s;
    }

    else{
        n -= 26 + 26 * 26;
        string s;
        int index = 0;
        int t = 0;

        while(t < n){
            t += 26 * 26;
            ++index;
        }

        char c = index - 1 + 'A';
        s.push_back(c);
        n += 26 * 26;
        n -= t;
        t = 0;
        index = 0;

        while(t < n){
            t += 26;
            ++index;
        }

        c = index - 1 + 'A';
        s.push_back(c);
        n += 26;
        n -= t;
        c = n - 1 + 'A';
        s.push_back(c);
        return s;
    }

};

class sheet{
private:

    int m_row, m_col;
    int m_curr_row, m_curr_col;
    vector<vector<int>> matrix;

public:

    sheet(int row, int col): m_row(row), m_col(col){
        matrix.resize(m_row);
        for(int i = 0; i < m_row; i++){
            matrix[i].resize(m_col);
        }
        for(int i = 0; i < m_row; i++){
            for(int j = 0; j < m_col; j++){
                matrix[i][j] = 0;
            }
        }
        m_curr_row = 0;
        m_curr_col = 0;
    }
    void print(){
        cout<<"    ";
        for(int j = m_curr_col; j < min(m_curr_col + 10, m_col); j++){
            cout << convert_number_to_string(j+1) << "   ";
        }
        cout << "\n";
        for(int i = m_curr_row; i < min(m_curr_row + 10, m_row); i++){
            cout << i + 1 << "   ";
            for(int j = m_curr_col; j < min(m_curr_col + 10, m_col); j++){
                cout << matrix[i][j] << "   ";
            }
            cout << "\n";
        }
    }
};

int main(){
    int row, col;
    cin >> row >> col;
    sheet s1(row, col);
    s1.print();
}