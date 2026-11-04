# 41443152

# 多項式抽象資料型實作(Polynomial ADT implementation)
## 解題說明

### 問題描述
  本提要求使用C++實作一個基於陣列(Array-based)的多項是抽象資料型別(Polynomial ADT)，多項式的數學定義為：
  
  $p(x)=a_{0}x^{e_{0}} + a_{1}x^{e_{1}} + \dots + a_{n}x^{e_{n}}$

  其中 $a_{i}$ 為非零的浮點數係數(float coefficient)， $a_{i}$ 為非負整數指數(non-negative integer exponent)
  
  題目要求：
  
  1.多項式相加(Add)：計算兩個多項式的和。
  
  2.代入求值(Eval)：給定數值x，計算多項式的結果。
  
  3.輸入與輸出運算子多載(>>與<<)：自訂格式進行多項式的輸入與輸出。
  
  4.動態記憶體管理(newTerm)：當陣列容量不足時，自動進行兩倍擴充(Capacity Doubling)。

### 解題策略
  1.內部資料結構設計：
  
  - 使用Term類別儲存單一項目係數(coef)與指數(exp)。
    
  - Polynomial類別內部維護動態陣列Term *termArry，並用terms記錄目前非零項數量、capacity記錄當前陣列大小。
  
  2.多項式相加演算法：

  - 使用雙指標i與j分別跑兩個多項式的termArray。

  - 比較兩者指數：
    - 若指數相同，將係數相加；若結果不為0，則存入結果多項式。
  
  3.輸入與輸出運算子多載(>>與<<)：自訂格式進行多項式的輸入與輸出。
  
  4.動態記憶體管理(newTerm)：當陣列容量不足時，自動進行兩倍擴充(Capacity Doubling)。
## 程式實作

## 效能分析

## 測試與驗證

## 申論及開發報告
