#include <iostream>
#include <string>
using namespace std;
// 遞迴函數（利用預設參數 i = 0 與 curr = ""）
void powerset(const string& S, size_t i = 0, string curr = "") {
    // 遞迴基底：處理完所有字元，印出目前組合
    if (i == S.length()) 
    {
        cout << "(" << curr << ") ";
        return;
    }
    // 選擇 1：不選當前字元 S[i]
    powerset(S, i + 1, curr);
    // 選擇 2：選當前字元 S[i]（若不是第一個元素則自動加逗號）
    string next;
    if (curr.empty()) 
    {
        next = string(1, S[i]); // 第一個元素，前面不加逗號
    } 
    else 
    {
        next = curr + "," + S[i]; // 非第一個元素，中間加逗號分隔
    }
    powerset(S, i + 1, next);
}
int main() {
    string S;
    if (cin >> S) 
    {
        cout << "{ ";
        powerset(S);
        cout << "}\n";
    }
    return 0;
}
