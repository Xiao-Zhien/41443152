#include <iostream>
using namespace std;
// 遞迴阿克曼函數
int ackermann(int m, int n) { 
    if (m == 0) return n + 1;
    if (n == 0) return ackermann(m - 1, 1);
    return ackermann(m - 1, ackermann(m, n - 1));
}
// 非遞迴版本的阿克曼函數（使用自訂陣列模擬系統 Stack 呼叫堆疊）
int ackermannnonrecursive(int m, int n) {
    int stack[100000]; // 建立一個陣列作為堆疊，用來儲存待處理的 m 值
    int top = 0;       // 堆疊頂端指標
    
    stack[top] = m;    // 將初始的 m 值推入堆疊

    // 當堆疊還有元素時，繼續模擬遞迴過程
    while (top >= 0) 
    {
        m = stack[top--]; // 彈出堆疊頂端的 m 值

        // 情況 1：當 m == 0 時，依定義 A(0, n) = n + 1
        if (m == 0) 
        {
            // 若堆疊已清空，代表所有遞迴層級皆計算完畢，回傳最終答案
            if (top < 0) 
            {
                return n + 1;
            }
            // 否則，將當前的 n + 1 作為上一層遞迴傳進去的 n
            n = n + 1;
        } 
        // 情況 2：當 n == 0 且 m > 0 時，依定義 A(m, 0) = A(m - 1, 1)
        else if (n == 0) 
        {
            stack[++top] = m - 1; // 將 (m - 1) 壓入堆疊
            n = 1;               // 將 n 設為 1
        } 
        // 情況 3：當 m > 0 且 n > 0 時，依定義 A(m, n) = A(m - 1, A(m, n - 1))
        else 
        {
            stack[++top] = m - 1; // 壓入外層的 (m - 1)，等待內層算完後呼叫
            stack[++top] = m;     // 壓入內層的 m
            n = n - 1;            // 準備先計算內層的 A(m, n - 1)
        }
    }
    return n;
}
int main() 
{
    int m, n;   
    cin >> m >> n;  // 輸入的 m 和 n
    cout << "A(" << m << ", " << n << ")[recursive] = " << ackermann(m, n) << endl; // 輸出阿克曼函數的結果
    cout << "A(" << m << ", " << n << ")[non-recursive] = " << ackermannnonrecursive(m, n) << endl; // 輸出非遞迴版本的阿克曼函數結果
    return 0;
}
