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

    a.若指數相同，將係數相加；若結果不為0，則存入結果多項式。

    b.其中一方指數較大，則優先將該項放入結果多項式。

  - 跑完結束，將剩下沒被處理完的項依序補入結果多項式。
  
  3.動態擴充機制：

  - 呼叫newTerm()新增項目時，若ters == capacity，則配置兩倍大的新陣列，將舊資料複製過去後釋放就記憶體，確保不會溢位。
    
## 程式實作
```cpp
#include<iostream>
#include<algorithm>
#include<cmath>

using namespace std;

class Polynomial;
class Term {
	friend Polynomial; // 讓 Polynomial 類別可以直接存取裡面的私有資料
private:
	float coef; // 這項的「係數」（例如 3x^2 裡面的 3）
	int exp;    // 這項的「指數」（例如 3x^2 裡面的 2）
};
class Polynomial {
private:
	Term *termArray; // 動態陣列
	int capacity;    // 陣列目前最大的容量
	int terms;       // 目前多項式實際上有幾項

public:
	Polynomial() :capacity(2), terms(0) {
		termArray = new Term[capacity];
	}
	void newTerm(const float theCoef, const int theExp);
	int getTerms() const { return terms; }         
	float getCoef(int i) const { return termArray[i].coef; } 
	int getExp(int i) const { return termArray[i].exp; }     
	void reset() { terms = 0; }                      
	Polynomial Add(Polynomial poly) {
		Polynomial c; 
		int i = 0, j = 0; 
		while (i < terms && j < poly.terms) {
			if (termArray[i].exp == poly.termArray[j].exp) {
				float sum = termArray[i].coef + poly.termArray[j].coef;
				if (sum != 0) {
					c.newTerm(sum, termArray[i].exp);
				}
				i++;
				j++;
			} 
			else if (termArray[i].exp < poly.termArray[j].exp) {
				c.newTerm(poly.termArray[j].coef, poly.termArray[j].exp);
				j++;
			} 
			else {
				c.newTerm(termArray[i].coef, termArray[i].exp);
				i++;
			}
		}
		while (i < terms) {
			c.newTerm(termArray[i].coef, termArray[i].exp);
			i++;
		}
		while (j < poly.terms) {
			c.newTerm(poly.termArray[j].coef, poly.termArray[j].exp);
			j++;
		}
		return c;
	}
	float Eval(float f) const {
		float ans = 0;
		for (int i = 0; i < terms; i++) {
			ans += termArray[i].coef * pow(f, termArray[i].exp);
		}
		return ans;
	}
	friend ostream& operator<<(ostream& os, const Polynomial& p) {
		if (p.getTerms() == 0) {
			os << "0"; 
			return os;
		}
		for (int i = 0; i < p.getTerms(); i++) {
			float c = p.getCoef(i);
			int e = p.getExp(i);
			if (i > 0 && c > 0) {
				os << " + ";
			} else if (c < 0) {
				os << " - ";
			} else if (i > 0) {
				os << " ";
			}
			float absC = abs(c); 
			if (absC != 1 || e == 0) {
				os << absC; 
			}
			if (e > 0) {
				os << "x";
				if (e > 1) {
					os << "^" << e; 
				}
			}
		}
		return os;
	}
	friend istream& operator>>(istream& is, Polynomial& p) {
		p.reset(); 
		int n;
		cout << "多項式有幾項？: ";
		is >> n;
		cout << "輸入每一項的(係數 指數)：\n";
		for (int i = 0; i < n; i++) {
			float c;
			int e;
			cout << "第 " << i + 1 << " 項: ";
			is >> c >> e;
			p.newTerm(c, e); 
		}
		return is;
	};
};
void Polynomial::newTerm(const float theCoef, const int theExp)
{
	if (theCoef == 0) return; 
	if (terms == capacity)
	{
		capacity *= 2; 
		Term* temp = new Term[capacity];
		copy(termArray, termArray + terms, temp); 
		delete[] termArray; 
		termArray = temp;
	}
	termArray[terms].coef = theCoef;
	termArray[terms++].exp = theExp;
}
int main()
{
	Polynomial p1, p2; 
	cout << "建立A(x)" << endl;
	cin >> p1; 
	cout << "\n建立B(x)" << endl;
	cin >> p2;
	cout << "\n運算結果" << endl;
	cout << "A(x) = " << p1 << endl;
	cout << "B(x) = " << p2 << endl;
	Polynomial pSum = p1.Add(p2);
	cout << "A(x) + B(x) = " << pSum << endl;
	float xVal;
	cout << "\n請輸入x來計算A(x)+B(x)的值: ";
	cin >> xVal;
	cout << "A(" << xVal << ")+B(" << xVal << ") = " << pSum.Eval(xVal) << endl;
	return 0;
}
```
## 效能分析

### 時間複雜度
- Add運算： $O(m+n)$ ，其中m與n分別代表兩個多項式的非零項數量。由於採用雙指標線性掃描合併，只需掃描兩個陣列一次。
- Eval運算： $O(n*log(exp))$ ，其中n為非零項數量，pow計算指數項需花費 $O(\log(exp))$ 的時間。
- newTerm運算：平攤時間複雜度(Amortized Time Complexity)為 $O(\1)$ ，因為使用容量加倍(Capacity Doubling)策略。

### 空間複雜度
- 內部儲存： $O(n)$ ，其中n為多項式的非零項個數。
- Add運算輔助空間： $O(m+n)$ ，用於建立結果多項式c的動態陣列空間。

## 測試與驗證
<img width="359" height="327" alt="螢幕擷取畫面 2026-11-04 124431" src="https://github.com/user-attachments/assets/77bddb3d-e332-4a18-9e48-ba5f312534bd" />


## 申論及開發報告
