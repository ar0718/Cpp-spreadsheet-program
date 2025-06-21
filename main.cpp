#include<bits/stdc++.h>

using namespace std;


int ROW = 999;
int COL = 18278;
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

int convert_string_to_number(string s){

    int size = s.size();
    if(s.size() == 1){
        int t = s[0] - 'A';
        return t;
    }
    else if(s.size() == 2){
        int t = 26;
        int r1 = s[0] - 'A';
        int r2 = s[1] - 'A';
        t += r1 * 26 + r2;
        return t;
    }
    else{
        int t = 26 * 26 + 26;
        int r1 = s[0] - 'A';
        int r2 = s[1] - 'A';
        int r3 = s[2] - 'A';
        t += r1 * 26 * 26;
        t += r2 * 26;
        t += r3;
        return t;
    }
}

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
    void set_pointer(int a, int b){
        m_curr_row += a;
        m_curr_col += b;
        m_curr_row = min(m_row - 1, m_curr_row);
        m_curr_row = max(m_curr_row, 0);
        m_curr_col = min(m_col - 1, m_curr_col);
        m_curr_col = max(m_curr_col, 0);
    }
    void print(){
        cout<<"     ";
        for(int j = m_curr_col; j < min(m_curr_col + 10, m_col); j++){
            cout << convert_number_to_string(j+1) << "    ";
        }
        cout << "\n";
        for(int i = m_curr_row; i < min(m_curr_row + 10, m_row); i++){
            cout << i + 1 << "    ";
            for(int j = m_curr_col; j < min(m_curr_col + 10, m_col); j++){
                cout << matrix[i][j] << "    ";
            }
            cout << "\n";
        }
    }
    void set(int i, int j, int value){
        matrix[i][j] = value;
    }
};

bool is_valid_coloumn(const string& col){
    if(col.length() == 0 || col > convert_number_to_string(COL)) return false;
    for(char c: col){
        if(c < 'A' || c > 'Z') return false;
    }
    return true;
}

bool is_valid_cell(const string& s){
    smatch match;
    if(regex_match(s, match, regex("([A-Z]+)([1-9][0-9]*)"))){
        string col = match[1];
        int row = stoi(match[2]);
        return is_valid_coloumn(col) && row >= 1 && row <= ROW;
    }
    return false;
}

bool is_valid_number(const string& s){
    return regex_match(s, regex("[0-9]+"));
}

bool is_valid_operator_expression(const string& s){
    smatch match;
    if(regex_match(s, match, regex("([A-Z]+[1-9][0-9]*|[0-9]+)([\\+\\-\\*/])([A-Z]+[1-9][0-9]*|[0-9]+)"))){
        string left = match[1], right = match[3];
        return (is_valid_cell(left) || is_valid_number(left))&&(is_valid_cell(right) || is_valid_number(right));
    }
    return false;   
}

bool is_valid_range(const string& s, int check = 0){
    smatch match;
    if(!regex_match(s, match, regex("([A-Z]+[1-9][0-9]*):([A-Z]+[1-9][0-9]*)"))) return false;
    string c1 = match[1], c2 = match[2];
    smatch m1, m2;
    regex rgx("([A-Z]+)([1-9][0-9]*)");
    regex_match(c1, m1, rgx);
    regex_match(c2, m2, rgx);
    string col1 = m1[1], col2 = m2[1];
    int row1 = stoi(m1[2]), row2 = stoi(m2[2]);
    if(!is_valid_coloumn(col1) || !is_valid_coloumn(col2)) return false;
    check = 1;
    if(row1 > ROW || row2 > ROW) return false;
    if(col1 > col2 || row1 > row2) return false;
    check  = 0;
    return true;
}

bool is_valid_function(const string& s ,int& check){
    smatch match;
    if(regex_match(s, match, regex("(MIN|MAX|AVG|SUM|STDEV)\\(([^)]+)\\)"))){
        return is_valid_range(match[2], check);
    }
    else if(regex_match(s, match, regex("SLEEP\\(([^)]+)\\)"))){
        string arg = match[1];
        return is_valid_number(arg) || is_valid_cell(arg);
    }
    return false;
}   

int parse_string(const string& input){
    int check = 0;
    if(input.find(' ') != string::npos) return false;
    int eq = input.find("=");
    if(eq == string::npos) return false;
    string lhs = input.substr(0, eq);
    string rhs = input.substr(eq + 1);

    if(!is_valid_cell(lhs)) return false;

    if(is_valid_number(rhs)) return 1;

    else if(is_valid_cell(rhs)) return 2;

    else if(is_valid_operator_expression(rhs)) return 3;

    else if(is_valid_function(rhs, check)) return 4;

    else if(check == 1) return 5;

    else return 0;
}

int main(){
    int row, col;
    cin >> row >> col;
    ROW = row;
    COL = col;
    while(row < 1 || col < 1 || row > 999 || col > 18278){
        cout << "INVALID number of rows and coloumns\n";
        cout << "Enter again\n";
        cin >> row >> col;
    }
    ROW = row;
    COL = col;
    sheet s1(row, col);
    map<string,set<array<string,2>>> adj, rev_adj;
    map<string,int> value;

    function<void(string&)> update_matrix=[&](string& s){
        string col;
        string row1;
        for(int i = 0; i < s.size(); i++){
            if(s[i] >= 'A' && s[i] <= 'Z'){
                col.push_back(s[i]);
            }
            else{
                row1.push_back(s[i]);
            }
        }
        int row = stoi(row1);
        int col_no = convert_string_to_number(col);
        s1.set(row - 1, col_no, value[s]);
    };

    auto start = chrono::high_resolution_clock::now();
    s1.print();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout  << "["<< fixed << setprecision(1)<< duration.count()<< "] (ok) > ";
    cout.flush();
    start = chrono::high_resolution_clock::now();

    string input;
    cin>>input;

    function<void(array<string, 2>)> update = [&] (array<string, 2> u){
        if(u[1] == "max"){
            int t = -1e9;
            for(auto v : adj[u[0]]){
                t =  max(t, value[v[0]]);
            }
            value[u[0]] = t;
        }
        else if(u[1] == "min"){
            int t = 1e9;
            for(auto v : adj[u[0]]){
                t = min(t, value[v[0]]);
            }
            value[u[0]] = t;
        }
        else if(u[1] == "sum"){
            int t = 0;
            for(auto v :  adj[u[0]]){
                t += value[v[0]];
            }
            value[u[0]] = t;
        }
        else if(u[1] == "stdev"){
            int cnt = 0;
            int sum_of_square = 0;
            int sum = 0;
            for(auto v : adj[u[0]]){
                sum += value[v[0]];
                sum_of_square += value[v[0]] * value[v[0]];
                ++cnt;
            }
            float mean = sum * 1.00 / cnt;
            float dev = 0.00;
            dev = (float)sum_of_square / cnt  - mean * mean;
            dev = sqrt(dev);
            value[u[0]] = dev;
        }
        else if(u[1] == "avg"){
            int cnt = 0;
            int sum = 0;
            for(auto v : adj[u[0]]){
                sum += value[v[0]];
                ++cnt;
            }
            float mean = sum * 1.00 / cnt;
            value[u[0]] =  mean;
        }
        else if(u[1] == "sleep"){
            for(auto v : adj[u[0]]){
                value[u[0]] = value[v[0]];
            }
        }

        // else if(u[1] == "product"){
        //     int t = 1;
        //     for(auto v : adj[u[0]]){
        //         t *= value[v[0]];
        //     }
        //     value[u[0]] = t;
        // }
        // else if(u[1] == "divide"){
        //     array<string, 2> first_element = *(adj[u[0]].begin());
        //     auto it = adj[u[0]].begin(); ++it;
        //     array<string, 2> second_element = (*it);  
        //     value[u[0]] = value[first_element[0]] / value[second_element[0]];      
        // }
        // else if(u[1] == "diff"){
        //     array<string, 2> first_element = *(adj[u[0]].begin());
        //     auto it = adj[u[0]].begin(); ++it;
        //     array<string, 2> second_element = (*it);  
        //     value[u[0]] = value[first_element[0]] - value[second_element[0]];          
        // }
        
        else{
            string s1,s2;
            int value1=0;
            int value2=0;
            if(u[1].find('+') != string::npos){
                s1=u[1].substr(0, u[1].find('+'));
                s2=u[1].substr(u[1].find('+') + 1);
                if(is_valid_cell(s1)) value1 = value[s1];
                else  value1 = stoi(s1);
                if(is_valid_cell(s2)) value2 = value[s2];
                else value2 = stoi(s2);
                value[u[0]] = value1 + value2;
            }
            else if(u[1].find('-') != string::npos){
                s1=u[1].substr(0, u[1].find('-'));
                s2=u[1].substr(u[1].find('-') + 1);
                if(is_valid_cell(s1)) value1 = value[s1];
                else  value1 = stoi(s1);
                if(is_valid_cell(s2)) value2 = value[s2];
                else value2 = stoi(s2);
                value[u[0]] = value1 - value2;
            }
            else if(u[1].find('*') != string::npos){
                s1=u[1].substr(0, u[1].find('*'));
                s2=u[1].substr(u[1].find('*') + 1);
                if(is_valid_cell(s1)) value1 = value[s1];
                else  value1 = stoi(s1);
                if(is_valid_cell(s2)) value2 = value[s2];
                else value2 = stoi(s2);
                value[u[0]] = value1 * value2;
            }
            else{
                s1=u[1].substr(0, u[1].find('/'));
                s2=u[1].substr(u[1].find('/') + 1);
                if(is_valid_cell(s1)) value1 = value[s1];
                else  value1 = stoi(s1);
                if(is_valid_cell(s2)) value2 = value[s2];
                else value2 = stoi(s2);
                value[u[0]] = value1 / value2;
            }
        }
        update_matrix(u[0]);
    };
    function<bool(array<string,2>, map<string,int>&)> find_cycle = [&] (array<string, 2> node, map<string,int>& visited){
        visited[node[0]] = 1;
        for(auto u : adj[node[0]]){
            // cout<<u[0]<<"\n";
            if(visited[u[0]] == 2) continue;
            else if(visited[u[0]]){
                return 1;
            }
            int t = find_cycle(u, visited);
            if(t) return 1;

        }
        visited[node[0]] = 2;
        return 0;
    };
    while(input != "q"){
        duration = end - start;
        string response = "ok";
        if(input == "w"){
            s1.set_pointer(-10, 0);
        }
        else if(input == "d"){
            s1.set_pointer(0, 10);
        }
        else if(input == "a"){
            s1.set_pointer(0, -10);
        }
        else if(input == "s"){
            s1.set_pointer(10, 0);
        }
        else{
            int code = parse_string(input);
            int eq = input.find("=");
            string lhs, rhs;
            if(eq != string::npos){
                lhs = input.substr(0, eq);
                rhs = input.substr(eq + 1);
            }
            if(code == 1){
                for(auto& u:adj[lhs]){
                    rev_adj[u[0]].erase({lhs,u[1]});
                }
                adj[lhs].clear();
                value[lhs] = stoi(rhs);
                update_matrix(lhs);
                map<string, bool> visited;
                vector<array<string,2>> top_sort_order;
                function<void(string)> dfs = [&] (string node){
                    visited[node] = true;
                    for(auto u : rev_adj[node]){
                        if(visited[u[0]]) continue;
                        dfs(u[0]);
                        top_sort_order.push_back(u);
                    }
                };
                dfs(lhs);
                reverse(top_sort_order.begin(), top_sort_order.end());
                for(auto u:top_sort_order){
                    update(u);
                }
            }
            else if(code == 2){
                for(auto& u:adj[lhs]){
                    rev_adj[u[0]].erase({lhs, u[1]});
                }
                adj[lhs].clear();
                adj[lhs].insert({rhs, "sum"});
                rev_adj[rhs].insert({lhs, "sum"});
                map<string, int> visited;
                int check = find_cycle({lhs,"sum"}, visited);
                if(check){
                    response = "circular dependencies";
                    adj[lhs].clear();
                    rev_adj[rhs].erase({lhs, "sum"});
                }
                else{
                    map<string, int> visited;
                    value[lhs] = value[rhs];
                    update_matrix(lhs);
                    vector<array<string,2>> top_sort_order;
                    function<void(string)> dfs = [&] (string node){
                        visited[node] = true;
                        for(auto u : rev_adj[node]){
                            if(visited[u[0]]) continue;
                            dfs(u[0]);
                            top_sort_order.push_back(u);
                        }
                    };
                    dfs(lhs);
                    reverse(top_sort_order.begin(), top_sort_order.end());
                    for(auto u:top_sort_order){
                        update(u);
                    }

                }
            }
            else if(code == 3){
                if(rhs.find('+') != string::npos){
                    int op = rhs.find('+');
                    string rhs1 = rhs.substr(0, op);
                    string rhs2 = rhs.substr(op + 1);
                    if(is_valid_cell(rhs1) && is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        adj[lhs].insert({rhs1, "sum"});
                        adj[lhs].insert({rhs2, "sum"});
                        rev_adj[rhs1].insert({lhs, "sum"});
                        rev_adj[rhs2].insert({lhs, "sum"});
                        map<string, int> visited;
                        int check = find_cycle({lhs, "sum"}, visited);
                        if(check){
                            response = "circular dependencies";
                            adj[lhs].erase({rhs1, "sum"});
                            adj[lhs].erase({rhs2, "sum"});
                            rev_adj[rhs1].erase({lhs, "sum"});
                            rev_adj[rhs2].erase({lhs, "sum"});
                        }
                        else{
                            map<string, int> visited;
                            value[lhs] = value[rhs1] + value[rhs2];
                            update_matrix(lhs);
                            vector<array<string,2>> top_sort_order;
                            function<void(string)> dfs = [&] (string node){
                                visited[node] = true;
                                for(auto u : rev_adj[node]){
                                    if(visited[u[0]]) continue;
                                    dfs(u[0]);
                                    top_sort_order.push_back(u);
                                }
                            };
                            dfs(lhs);
                            reverse(top_sort_order.begin(), top_sort_order.end());
                            for(auto u:top_sort_order){
                                update(u);
                            }
                        }
                    }
                    else if(is_valid_cell(rhs1) || is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        if(is_valid_cell(rhs1)){
                            adj[lhs].insert({rhs1, rhs});
                            rev_adj[rhs1].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs1, rhs});
                                rev_adj[rhs1].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs1] + stoi(rhs2);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                    
                        else{
                            adj[lhs].insert({rhs2, rhs});
                            rev_adj[rhs2].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs2, rhs});
                                rev_adj[rhs2].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs2] + stoi(rhs1);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                    }
                    else{
                        for(auto& u:adj[lhs]){
                            rev_adj[u[0]].erase({lhs,u[1]});
                        }
                        adj[lhs].clear();
                        value[lhs] = stoi(rhs1) + stoi(rhs2);
                        update_matrix(lhs);
                        map<string, bool> visited;
                        vector<array<string,2>> top_sort_order;
                        function<void(string)> dfs = [&] (string node){
                            visited[node] = true;
                            for(auto u : rev_adj[node]){
                                if(visited[u[0]]) continue;
                                dfs(u[0]);
                                top_sort_order.push_back(u);
                            }
                        };
                        dfs(lhs);
                        reverse(top_sort_order.begin(), top_sort_order.end());
                        for(auto u:top_sort_order){
                            update(u);
                        }
                    }
                }
                else if(rhs.find('-') != string::npos){
                    int op = rhs.find('-');
                    string rhs1 = rhs.substr(0, op);
                    string rhs2 = rhs.substr(op + 1);
                    if(is_valid_cell(rhs1) && is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        adj[lhs].insert({rhs1, rhs});
                        adj[lhs].insert({rhs2, rhs});
                        rev_adj[rhs1].insert({lhs, rhs});
                        rev_adj[rhs2].insert({lhs, rhs});
                        map<string, int> visited;
                        int check = find_cycle({lhs, rhs}, visited);
                        if(check){
                            response = "circular dependencies";
                            adj[lhs].erase({rhs1, rhs});
                            adj[lhs].erase({rhs2, rhs});
                            rev_adj[rhs1].erase({lhs, rhs});
                            rev_adj[rhs2].erase({lhs, rhs});
                        }
                        else{
                            map<string, int> visited;
                            value[lhs] = value[rhs1] - value[rhs2];
                            update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                        }
                    }
                    else if(is_valid_cell(rhs1) || is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        if(is_valid_cell(rhs1)){
                            adj[lhs].insert({rhs1, rhs});
                            rev_adj[rhs1].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs1, rhs});
                                rev_adj[rhs1].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs1] - stoi(rhs2);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                        else{
                            adj[lhs].insert({rhs2, rhs});
                            rev_adj[rhs2].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs2, rhs});
                                rev_adj[rhs2].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs2] - stoi(rhs1);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                    }
                    else{
                        for(auto& u:adj[lhs]){
                            rev_adj[u[0]].erase({lhs,u[1]});
                        }
                        adj[lhs].clear();
                        value[lhs] = stoi(rhs1) - stoi(rhs2);
                        update_matrix(lhs);
                        map<string, bool> visited;
                                 vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                    }
                }
                else if(rhs.find('*') != string::npos){
                    int op = rhs.find('*');
                    string rhs1 = rhs.substr(0, op);
                    string rhs2 = rhs.substr(op + 1);
                    if(is_valid_cell(rhs1) && is_valid_cell(rhs2)){

                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        adj[lhs].insert({rhs1, rhs});
                        adj[lhs].insert({rhs2, rhs});
                        rev_adj[rhs1].insert({lhs, rhs});
                        rev_adj[rhs2].insert({lhs, rhs});
                        map<string, int> visited;
                        int check = find_cycle({lhs, rhs}, visited);
                        if(check){
                            response = "circular dependencies";
                            adj[lhs].erase({rhs1, rhs});
                            adj[lhs].erase({rhs2, rhs});
                            rev_adj[rhs1].erase({lhs, rhs});
                            rev_adj[rhs2].erase({lhs, rhs});
                        }
                        else{
                            map<string, int> visited;
                            value[lhs] = value[rhs1] * value[rhs2];
                            update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                        }
                    }
                    else if(is_valid_cell(rhs1) || is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        if(is_valid_cell(rhs1)){
                            adj[lhs].insert({rhs1, rhs});
                            rev_adj[rhs1].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs1, rhs});
                                rev_adj[rhs1].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs1] * stoi(rhs2);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                        else{
                            adj[lhs].insert({rhs2, rhs});
                            rev_adj[rhs2].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs2, rhs});
                                rev_adj[rhs2].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs2] * stoi(rhs1);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                    }
                    else{
                        for(auto& u:adj[lhs]){
                            rev_adj[u[0]].erase({lhs,u[1]});
                        }
                        adj[lhs].clear();
                        value[lhs] = stoi(rhs1) * stoi(rhs2);
                        update_matrix(lhs);
                        map<string, bool> visited;
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                } 
                    }
                }
                else{
                    int op = rhs.find('/');
                    string rhs1 = rhs.substr(0, op);
                    string rhs2 = rhs.substr(op + 1);
                    if(is_valid_cell(rhs1) && is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        adj[lhs].insert({rhs1, rhs});
                        adj[lhs].insert({rhs2, rhs});
                        rev_adj[rhs1].insert({lhs, rhs});
                        rev_adj[rhs2].insert({lhs, rhs});
                        map<string, int> visited;
                        int check = find_cycle({lhs, rhs}, visited);
                        if(check){
                            response = "circular dependencies";
                            adj[lhs].erase({rhs1, rhs});
                            adj[lhs].erase({rhs2, rhs});
                            rev_adj[rhs1].erase({lhs, rhs});
                            rev_adj[rhs2].erase({lhs, rhs});
                        }
                        else{
                            map<string, int> visited;
                            value[lhs] = value[rhs1] / value[rhs2];
                            update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                        }
                    }
                    else if(is_valid_cell(rhs1) || is_valid_cell(rhs2)){
                        for(auto& u : adj[lhs]){
                            rev_adj[u[0]].erase({lhs, u[1]});
                        }
                        adj[lhs].clear();
                        if(is_valid_cell(rhs1)){
                            adj[lhs].insert({rhs1, rhs});
                            rev_adj[rhs1].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs1, rhs});
                                rev_adj[rhs1].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs1] / stoi(rhs2);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                        else{
                            adj[lhs].insert({rhs2, rhs});
                            rev_adj[rhs2].insert({lhs, rhs});
                            map<string, int> visited;
                            int check = find_cycle({lhs, rhs}, visited);
                            if(check){
                                response = "circular dependencies";
                                adj[lhs].erase({rhs2, rhs});
                                rev_adj[rhs2].erase({lhs, rhs});
                            }
                            else{
                                map<string, int> visited;
                                value[lhs] = value[rhs2] / stoi(rhs1);
                                update_matrix(lhs);
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }
                            }
                        }
                    }
                    else{
                        for(auto& u:adj[lhs]){
                            rev_adj[u[0]].erase({lhs,u[1]});
                        }
                        adj[lhs].clear();
                        value[lhs] = stoi(rhs1) / stoi(rhs2);
                        update_matrix(lhs);
                        map<string, bool> visited;
                                vector<array<string,2>> top_sort_order;
                                function<void(string)> dfs = [&] (string node){
                                    visited[node] = true;
                                    for(auto u : rev_adj[node]){
                                        if(visited[u[0]]) continue;
                                        dfs(u[0]);
                                        top_sort_order.push_back(u);
                                    }
                                };
                                dfs(lhs);
                                reverse(top_sort_order.begin(), top_sort_order.end());
                                for(auto u:top_sort_order){
                                    update(u);
                                }  
                    }
                }
            }
            else if(code == 4){
                smatch match;
                if(regex_match(rhs, match, regex("(MIN|MAX|AVG|SUM|STDEV)\\(([^)]+)\\)"))){
                    string func = match[1];
                    string range = match[2];

                    smatch m1, m2;
                    regex rgx("([A-Z]+)([1-9][0-9]*)");
                    string range1 = range.substr(0, range.find(':'));
                    string range2 = range.substr(range.find(':') + 1);
                    regex_match(range1, m1, rgx);
                    regex_match(range2, m2, rgx);

                    string col1 = m1[1], col2 = m2[1];
                    int row1 = stoi(m1[2]), row2 = stoi(m2[2]);

                    for(auto& u: adj[lhs]){
                        rev_adj[u[0]].erase({lhs, u[1]});
                    }
                    adj[lhs].clear();
                    for (char& c : func) c = tolower(c);
                    for(int i = row1; i <= row2; i++){
                        for(int j = convert_string_to_number(col1); j <= convert_string_to_number(col2); j++){
                            string s = convert_number_to_string(j + 1) + to_string(i);
                            adj[lhs].insert({s, func});
                            rev_adj[s].insert({lhs, func});
                        }
                    }

                    map<string, int> visited;
                    int check = find_cycle({lhs, func}, visited);
                    if(check){
                        response = "circular dependencies";
                        for(auto& u: adj[lhs]){
                            rev_adj[u[0]].erase({lhs, func});
                        }
                        adj[lhs].clear();
                    }
                    else{
                        update({lhs, func});
                        map<string, int> visited;
                        vector<array<string,2>> top_sort_order;
                        function<void(string)> dfs = [&] (string node){
                            visited[node] = true;
                            for(auto u : rev_adj[node]){
                                if(visited[u[0]]) continue;
                                dfs(u[0]);
                                top_sort_order.push_back(u);
                            }
                        };
                        dfs(lhs);
                        reverse(top_sort_order.begin(), top_sort_order.end());
                        for(auto u:top_sort_order){
                            update(u);
                        }
                    }
                }
                else if(regex_match(rhs, match, regex("SLEEP\\(([^)]+)\\)"))){
                    string arg = match[1];

                    for(auto& u: adj[lhs]){
                        rev_adj[u[0]].erase({lhs, u[1]});
                    }
                    adj[lhs].clear();

                    if(is_valid_cell(arg)){
                        adj[lhs].insert({arg, "sleep"});
                        rev_adj[arg].insert({lhs, "sleep"});
                        map<string, int> visited;
                        int check = find_cycle({lhs, "sleep"}, visited);
                        if(check){
                            response = "circular dependencies";
                            adj[lhs].erase({arg, "sleep"});
                            rev_adj[arg].erase({lhs, "sleep"});
                        }
                        else{
                            this_thread::sleep_for(chrono::milliseconds(value[arg]));
                            value[lhs] = value[arg];
                            update_matrix(lhs);
                            vector<array<string,2>> top_sort_order;
                            map<string, int> visited;
                            function<void(string)> dfs = [&] (string node){
                                visited[node] = true;
                                for(auto u : rev_adj[node]){
                                    if(visited[u[0]]) continue;
                                    dfs(u[0]);
                                    top_sort_order.push_back(u);
                                }
                            };
                            dfs(lhs);
                            reverse(top_sort_order.begin(), top_sort_order.end());
                            for(auto u:top_sort_order){
                                update(u);
                            }
                        }
                    }
                    else{
                        int sleep_time = stoi(arg);
                        this_thread::sleep_for(chrono::milliseconds(sleep_time));
                        value[lhs] = sleep_time;
                        update_matrix(lhs);
                    }
                }
            }

            else if(code == 5){
                response = "Invalid range";
            }
            else{
                response = "unrecognized cmd";
            }
        }
        s1.print();
        end = chrono::high_resolution_clock::now();
        duration = end - start;
        cout << "["<< fixed << setprecision(1)<< duration.count()<< "] ("<< response<< ") > ";
        cout.flush();
        cin>>input;
        cout.flush();
        start = chrono::high_resolution_clock::now();
    }
}