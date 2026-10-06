# 41443152
# 題目一 阿克曼函數（Ackermann Function)

本專案實作並比較了**遞迴 (Recursive)** 與 **非遞迴 (Non-Recursive)** 兩種版本的阿克曼函數（Ackermann Function），並針對其計算複雜度與執行效能進行深入分析。

---

## 1. 解題說明 

### 1.1 核心想法
阿克曼函數（Ackermann Function）是一個極度快速成長的非原始遞迴函數（Non-primitive recursive function）。其數學定義如下：

$$
A(m, n) = 
\begin{cases} 
n + 1 & \text{if } m = 0 \\ 
A(m - 1, 1) & \text{if } m > 0 \text{ and } n = 0 \\ 
A(m - 1, A(m, n - 1)) & \text{if } m > 0 \text{ and } n > 0 
\end{cases}
$$

1. **遞迴版本 (Recursive)**：
   直接依照數學定義進行函式自我呼叫。由於 $A(m-1, A(m, n-1))$ 包含巢狀遞迴，系統會呼叫堆疊自動記錄每一層計算狀態。
   
2. **非遞迴版本 (Non-Recursive)**：
   利用自訂陣列模擬系統堆疊的行為。因為阿克曼函數在計算過程中，外層需要等內層計算出結果後才能繼續處理，因此我們只需將**外層待執行的 $m$ 值**推入堆疊，並即時更新變數 $n$ 的狀態即可。

### 1.2 步驟範例說明：計算 $A(1, 2)$

* **遞迴展開過程**：
  $$A(1, 2) \rightarrow A(0, A(1, 1)) \rightarrow A(0, A(0, A(1, 0))) \rightarrow A(0, A(0, A(0, 1))) = A(0, A(0, 2)) = A(0, 3) = 4$$

* **非遞迴 Stack 追蹤過程**：
  1. **初始狀態**：Stack = `[1]`, $n = 2$
  2. **Pop 1** ($m=1, n=2>0$)：推入外層 $m-1=0$ 與內層 $m=1$。Stack = `[0, 1]`, $n = 1$
  3. **Pop 1** ($m=1, n=1>0$)：推入外層 $m-1=0$ 與內層 $m=1$。Stack = `[0, 0, 1]`, $n = 0$
  4. **Pop 1** ($m=1, n=0$)：推入 $m-1=0$。Stack = `[0, 0, 0]`, $n = 1$
  5. **Pop 0** ($m=0, n=1$)：n 更新為 $n+1=2$。Stack = `[0, 0]`, $n = 2$
  6. **Pop 0** ($m=0, n=2$)：n 更新為 $n+1=3$。Stack = `[0]`, $n = 3$
  7. **Pop 0** ($m=0, n=3$)：Top 變為 -1 (Stack 已空)，回傳 $n+1 = 4$。

---

## 2. 程式實作

完整 C++ 原始碼如下:

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

## 3. 效能分析 (Analysis) 

### 3.1 時間複雜度 (Time Complexity)
阿克曼函數的時間複雜度取決於函數總呼叫次數，其成長速度隨著 $m$ 的增加呈指數甚至超冪級數增加：

- $m = 0$: $\mathcal{O}(1)$
- $m = 1$: $\mathcal{O}(n)$
- $m = 2$: $\mathcal{O}(n)$
- $m = 3$: $\mathcal{O}(2^n)$
- $m = 4$: $\mathcal{O}(2^{2^{.^{.^{2}} logic}})$ (Tetration，超冪階乘級數)

整體時間複雜度與回傳結果 $A(m, n)$ 同階，屬於 **$\mathcal{O}(A(m, n))$**。

### 3.2 空間複雜度 (Space Complexity)
- **遞迴版本**：
  空間複雜度取決於**系統 Call Stack 的最大深度**。對於 $m \ge 3$，最大堆疊深度趨近於 $\mathcal{O}(A(m, n))$，極易導致系統預設 Stack 容量耗盡。
- **非遞迴版本**：
  空間複雜度取決於自訂陣列的大小 **$\mathcal{O}(S)$**（本程式宣告為固定大小 `100000`）。若計算過程中所需的 Stack 深度超過此限制則會發生溢位。

---

## 4. 測試與驗證 (Testing and Proving) 

為驗證遞迴與非遞迴邏輯的一致性，透過數學推導公式進行數值驗證：

| 測試案例 $(m, n)$ | 理論數學公式結果 | 遞迴版本輸出 | 非遞迴版本輸出 |
| :---: | :---: | :---: | :---: |
| $(0, 5)$ | $n + 1 = 6$ | 6 | 6 | 
| $(1, 3)$ | $n + 2 = 5$ | 5 | 5 |
| $(2, 2)$ | $2n + 3 = 7$ | 7 | 7 |
| $(3, 3)$ | $2^{n+3} - 3 = 29$ | 29 | 29 |
| $(3, 4)$ | $2^{n+3} - 3 = 125$ | 125 | 125 | 

---

## 5. 心得討論 

1. **遞迴 vs. 非遞迴的權衡**：
   - **遞迴**語法簡潔，但在處理深度遞迴時極易觸發作業系統的 Stack Overflow。
   - **非遞迴**利用自訂陣列模擬 Stack，理論上能擺脫系統預設 Stack 容量限制（通常為 1MB~8MB）。

2. **學習結論**：
   阿克曼函數是展示「遞迴演算法空間複雜度爆炸」的經典範例。透過手動改寫為非遞迴 Stack 模擬，能更深入理解編譯器底層在處理 Stack Frame 與函數呼叫時的實際運作機制。

---

# 題目二 冪集 (PowerSet)

本專案實作以 C++ 遞迴演算法生成給定字串（集合）的所有子集合（Power Set），並將其格式化輸出為數學集合表達式。


---

## 1. 解題說明

### 核心想法

對於長度為 $n$ 的字串 $S$（視為包含 $n$ 個相異元素的集合），其冪集（Power Set）共包含 $2^n$ 個子集合。

本實作採用二元決策樹（Binary Decision Tree）的遞迴回溯（Backtracking）想法：

* 從第一個字元開始，針對每個字元 $S[i]$ 都面臨 **選 (Include)** 或 **不選 (Exclude)** 兩種抉擇。

* 透過遞迴深度 $i$ 記錄當前決策位置，並攜帶字串 `curr` 記錄目前已選擇的子集合元素組合。

* **格式控制**：當 `curr` 為空時直接放入字元；若已有元素則在前面補上逗號 `,` 分隔，維持標準集合輸出格式。

### 步驟範例說明：計算 `S = "ab"`

輸入字串 $S = \text{"ab"}$，長度 $N=2$：

```
                  powerset("ab", i=0, curr="")
                         /             \
             不選 'a'   /               \ 選 'a'
                      /                 \
        powerset("ab", 1, "")        powerset("ab", 1, "a")
           /           \                /           \
   不選 'b'/             \選 'b' 不選 'b'/             \選 'b'
         /               \            /               \
   (2, "")            (2, "b")    (2, "a")          (2, "a,b")
   ↓                  ↓           ↓                 ↓
  印出 ()            印出 (b)     印出 (a)          印出 (a,b)

```

**最終輸出結果**：`{ () (b) (a) (a,b) }`

## 2. Algorithm Design & Programming 

完整 C++ 實作原始碼如下：

```
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

## 3. 效能分析 (Analysis)

### 時間複雜度 (Time Complexity)

* **決策樹節點總數**：對於長度為 $n$ 的字串，決策樹的深度為 $n$，總葉子節點數（子集合個數）為 $2^n$。整棵二元樹的總節點數為 $2^0 + 2^1 + \dots + 2^n = 2^{n+1} - 1$。

* **字串複製開銷**：每次遞迴傳遞字串或進行 `curr + "," + S[i]` 字串拼接時，需花費 $\mathcal{O}(n)$ 時間。

* **整體時間複雜度**：$\mathcal{O}(n \cdot 2^n)$。

### 空間複雜度 (Space Complexity)

* **遞迴呼叫堆疊 (Call Stack)**：遞迴最大深度等於字串長度 $n$，空間開銷為 $\mathcal{O}(n)$。

* **字串變數空間**：每一層遞迴建立的 `curr` 與 `next` 長度不超過 $n$。

* **整體空間複雜度**：$\mathcal{O}(n)$（極度節省記憶體）。

## 4. 測試與驗證 (Testing and Proving) 

將程式執行結果與數學理論進行比對測試，驗證邏輯正確性：

| 測試案例 ($S$) | 集合大小 ($n$) | 理論子集合數 ($2^n$) | 程式輸出內容 | 
| ----- | ----- | ----- | ----- |  
| `""` (空字串) | 0 | 1 | `{ () }` |
| `"a"` | 1 | 2 | `{ () (a) }` | 
| `"ab"` | 2 | 4 | `{ () (b) (a) (a,b) }` | 
| `"abc"` | 3 | 8 | `{ () (c) (b) (b,c) (a) (a,c) (a,b) (a,b,c) }` | 


## 5. 心得討論 

1. **遞迴與數學邏輯的結合**：
   這次作業透過寫程式來實現數學上的「冪集（Power Set）」概念，非常直觀地展現了遞迴（Recursion）的強大。對每個元素做「選」與「不選」兩種決策，恰好構成一棵高度為 $n$ 的二元樹，非常優雅地枚舉出所有 $2^n$ 種組合。

2. **C++ 預設參數的便利性**：
   程式中巧用 C++ 的預設參數（Default Arguments `i = 0`, `curr = ""`），讓 `main()` 函式可以直接呼叫 `powerset(S)`，不必在外面多寫一個封裝函式（Wrapper Function），大幅簡化了介面呼叫的複雜度。

3. **效能瓶頸**：
   在測試 $n \ge 15$ 的情況時，發現主要的瓶頸並非演算法本身的計算時間，而是大量的輸出（`cout`）。另外，每次遞迴以傳值（Pass-by-value）方式複製 `string` 也是一筆開銷。
