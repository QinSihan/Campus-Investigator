/*main*/
#include<iostream>
#include<fstream>
#include<vector>
#include<climits>
#include<stack>
#include<sstream>

using namespace std;

struct Info{
    string code;
    string name;
    string info;
    Info(string code, string name, string info): code(code), name(name), info(info), next(nullptr){}
    Info* next;
};

void print_place_info(string start, Info* H){
    ofstream outfile("C:/Users/20148/Desktop/Investigator/Tests/result.txt", ios::app);
    // check
    if (!outfile.is_open()) {
        cerr << "Failed to open file \"result.txt\"" << endl;
        return;
    }
    // output Log
    
    outfile << "Log: searching for info of " << start << "..." << endl;
    
    Info* search = H->next;
    bool found_flag = false;

    // search
    while (search!=nullptr) {
        if (start == search->code || start == search->name) {
            // output result
            outfile << search->info << endl;
            found_flag = true;
            break;
        }
        search = search->next;
    }

    // con't find
    if (!found_flag) {
        outfile << "could not find info of " << start << endl;
    }
}

void floydWarshall(vector<vector<int>>& dist, vector<vector<int>>& next, int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (dist[i][j] != INT_MAX) {
                next[i][j] = j;
            }
        }
    }

    for (int k=0; k<n; k++) {
        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

void printpath(string start, string end, vector<vector<int>>& next){
    int start_index = start[0] - 'A';
    int end_index = end[0] - 'A';

    stack<int> path;
    int current = start_index;

    ofstream outfile("C:/Users/20148/Desktop/Investigator/Tests/result.txt", ios::app);
    outfile << "Log: searching for path from " << start << " to " << end << endl;

    // fail
    if (next[start_index][end_index] == -1) {
        if (outfile.is_open()) {
            outfile << "CANNOT find path from " << start << " to " << end << endl;
        }
        else {
            cerr << "Failed to open file \"result.txt\"" << endl;
        }
        return;
    }

    // exist
    while (current != end_index) {
        path.push(current);
        current = next[current][end_index];
    }
    path.push(end_index);

    // 倒置
    stack<int> path_stack;
    while (!path.empty()) {
        path_stack.push(path.top());
        path.pop();
    }

    // print
    if (outfile.is_open()) {
        outfile << "Path from " << start << " to " << end << ": ";
        while (!path_stack.empty()) {
            char enroute = path_stack.top() + 'A';
            outfile << enroute << " ";
            path_stack.pop();
        }
        outfile << endl;
    }
    else {
        cerr << "Failed to open file \"result.txt\"" << endl;
    }

    outfile.close();
}

int main() {
    // 读取邻接矩阵
    ifstream mapFile("C:/Users/20148/Desktop/Investigator/Tests/basic_map.txt");
    if (!mapFile.is_open()) {
        cerr << "Failed to open file \"basic_map.txt\"" << endl;
        return 1;
    }
    int n;
    mapFile >> n; // 节点数
    vector<vector<int>> matrix(n, vector<int>(n, INT_MAX));
    string value;
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            mapFile >> value;
            if (value == "INT_MAX") {
                matrix[i][j] = INT_MAX;
            }
            else 
            {
                matrix[i][j] = stoi(value);
            }
        }
    }
    mapFile.close();

    // 读取地点信息
    ifstream infoFile("C:/Users/20148/Desktop/Investigator/Tests/information.txt");
    if (!infoFile.is_open()) {
        cerr << "Failed to open file \"information.txt\"" << endl;
        return 1;
    }
    string code, name, info;
    Info head("xxxx", "xxxx", "xxxx");
    Info* current = &head;
    while (infoFile >> code >> name >> info) {
        Info* ptr = new Info(code, name, info);
        current->next = ptr;
        current = ptr;
    }
    infoFile.close();

    // floyd算法
    vector<vector<int>> dist = matrix;
    vector<vector<int>> next(n, vector<int>(n, -1));

    floydWarshall(dist, next, n);

    // 读取要求
    ifstream visitorFile("C:/Users/20148/Desktop/Investigator/Tests/visitor.txt");
    if (!visitorFile.is_open()) {
        cerr << "Failed to open file \"visitor.txt\"" << endl;
        return 1;
    }
    string command, start, end;
    while (visitorFile >> command) {
        // info
        if (command == "info") {
            visitorFile >> start;
            // 输出该地点信息
            print_place_info(start, &head);
        }
        // find_path
        else if (command == "find_path") {
            visitorFile >> start >> end;
            // 计算路径并输出
            printpath(start, end, next);
        }
    }
    visitorFile.close();

    // delete
    Info* ptr = head.next;
    while (ptr!=nullptr) {
        Info* temp = ptr;
        ptr = ptr->next;
        delete temp;
    }

    return 0;
}