# 41443152

---

# 題目一：阿克曼函數 (Ackermann Function)

## 解題說明

### 問題描述
這題要寫出計算阿克曼函數（Ackermann Function）的程式，並且要同時實作「遞迴」和「非遞迴」兩種版本進行比較。
阿克曼函數的數學定義如下：

$$
A(m, n) = 
\begin{cases} 
n + 1 & \text{if } m = 0 \\ 
A(m - 1, 1) & \text{if } m > 0 \text{ and } n = 0 \\ 
A(m - 1, A(m, n - 1)) & \text{if } m > 0 \text{ and } n > 0 
\end{cases}
$$

### 解題策略
1. **遞迴版本**：
   這個比較簡單，直接照著數學公式寫 if-else 去呼叫自己就可以了。
2. **非遞迴版本**：
   因為阿克曼函數呼叫層數很深，改用非遞迴時需要自己開一個陣列（`int stack[100000]`）來模擬系統的 Stack。
   當遇到 $m > 0$ 且 $n > 0$ 的情況時，把外層待處理的 $m - 1$ 和內層的 $m$ 依序存進 Stack 裡面，然後用迴圈持續處理 Stack 頂端的數值，直到 Stack 變空為止。

---

## 程式實作

```cpp
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
```

---

## 效能分析

### 時間複雜度
阿克曼函數的計算次數增加速度非常快：
- $m = 0$: $O(1)$
- $m = 1$: $O(n)$
- $m = 2$: $O(n)$
- $m = 3$: $O(2^n)$
- $m = 4$: $O(2^{2^{2^n}})$

整體時間複雜度主要取決於計算結果 $A(m, n)$，為 $O(A(m, n))$。

### 空間複雜度
- **遞迴版本**：
  空間複雜度主要看 Call Stack 的深度，為 $O(A(m, n))$。當 $m$ 和 $n$ 稍微大一點點就會記憶體溢位 (Stack Overflow)。
- **非遞迴版本**：
  使用我們宣告的 Stack 陣列空間，複雜度為 $O(S)$，其中 $S$ 是陣列大小 100000。

---

## 測試與驗證

編譯與執行指令如下：

```shell
$ g++ main.cpp -std=c++17 -o main.exe
$ .\main.exe
2 2
A(2, 2)[recursive] = 7
A(2, 2)[non-recursive] = 7
```

另外測試多組數值結果：
- 輸入 `0 5` 輸出 `6`
- 輸入 `1 3` 輸出 `5`
- 輸入 `2 2` 輸出 `7`
- 輸入 `3 3` 輸出 `29`

---

## 申論及開發報告

### 選用 Stack 資料結構的原因
剛開始寫這題時，發現直接寫遞迴雖然程式碼很少，但輸入稍微大一點數字（像是 $m=4$）程式就會直接當掉。
為了改寫成非遞迴，我選用 Stack（堆疊）這個資料結構。因為遞迴在執行時本來就是後進先出（LIFO）的過程，所以用 Stack 最適合拿來模擬遞迴呼叫。把還沒算完的 $m$ 值暫存起來，等內層算完再拿出來繼續算，這樣就不會用到系統預設的 Call Stack，也比較容易理解遞迴底層是怎麼運作的。

---
---

# 題目二：冪集 (PowerSet)

## 解題說明

### 問題描述
這題要求寫一個程式，輸入一個字串（代表一個集合），印出這個集合的所有子集合（Power Set），並且格式要符合規定的集合樣式。

### 解題策略
長度為 $n$ 的字串總共有 $2^n$ 個子集合。
我選擇使用遞迴的方式來解：
1. 從字串的第一個字元開始看，每個字元都有兩種選擇：**要選這個字元** 或 **不選這個字元**。
2. 透過遞迴一直走到字串結尾（$i == n$），這時候把組好的字串印出來。
3. 輸出格式要在元素之間補上逗號 `,`，所以如果是第一個放入的元素就不加逗號，第二個開始才加逗號。

---

## 程式實作

```cpp
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
```

---

## 效能分析

### 時間複雜度
對於長度為 $n$ 的字串，每層都會分成選與不選兩種情況，總共會有 $2^n$ 個葉子節點。每次處理字串串接需要花費 $O(n)$ 的時間。
因此總時間複雜度為 $O(n \cdot 2^n)$。

### 空間複雜度
主要開銷是遞迴呼叫時的 Call Stack 深度，最深只會到字串長度 $n$，再加上每一層宣告的字串變數，空間複雜度為 $O(n)$。

---

## 測試與驗證

編譯與執行指令如下：

```shell
$ g++ powerset.cpp -std=c++17 -o powerset.exe
$ .\powerset.exe
ab
{ () (b) (a) (a,b) }
```

測試其他輸入範例：
- 輸入 `a` 輸出 `{ () (a) }`
- 輸入 `abc` 輸出 `{ () (c) (b) (b,c) (a) (a,c) (a,b) (a,b,c) }`

---

## 申論及開發報告

### 選用遞迴與決策樹演算法的原因
產生 Power Set 的方法有很多種（像是用 Bitmask 或迴圈），但我選擇用遞迴演算法。
因為把每一個字元當成二元決策樹的一個分支（選擇要或不要），非常符合數學上子集合的定義。而且寫成遞迴程式碼很短、邏輯很直覺。雖然字串長度太長時會因為 $2^n$ 的關係跑很久，但針對一般課堂作業輸入的小字串，這個方法的寫法最簡單也最不容易寫錯。
