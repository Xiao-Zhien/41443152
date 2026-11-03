#include<iostream>
#include<algorithm>
#include<cmath>

using namespace std;

class Polynomial;

// 1. Term 類別：代表多項式裡面的「單獨一項」
class Term {
	friend Polynomial; // 讓 Polynomial 類別可以直接存取裡面的私有資料
private:
	float coef; // 這項的「係數」（例如 3x^2 裡面的 3）
	int exp;    // 這項的「指數」（例如 3x^2 裡面的 2）
};

// 2. Polynomial 類別：代表整個多項式
class Polynomial {
private:
	Term *termArray; // 動態陣列：用來把多項式的所有非零項按順序裝起來
	int capacity;    // 陣列目前最大的容量（空間不夠時會自動變大）
	int terms;       // 目前多項式實際上有幾項

public:
	// 【建構子】一開始建立多項式時會自動執行：容量設為 2，項數設為 0，並幫陣列配置記憶體
	Polynomial() :capacity(2), terms(0) {
		termArray = new Term[capacity];
	}

	// 宣告新增一項的函數（等一下在下面寫它的詳細內容）
	void newTerm(const float theCoef, const int theExp);

	// 【小工具函數】因為資料是 private 封閉的，寫這些小工具讓外面可以安全取得資料
	int getTerms() const { return terms; }           // 回傳目前總共有幾項
	float getCoef(int i) const { return termArray[i].coef; } // 取得第 i 項的係數
	int getExp(int i) const { return termArray[i].exp; }     // 取得第 i 項的指數
	void reset() { terms = 0; }                      // 把項數歸零（重新輸入前會用到）

	// 3. 多項式相加函數 (Add)
	Polynomial Add(Polynomial poly) {
		Polynomial c; // 準備一個新的多項式用來放相加後的答案
		int i = 0, j = 0; // i 用來指著自己的多項式，j 用來指著傳進來的 poly
		// 比較兩個多項式的指數大小，指數一樣就把係數加起來
		while (i < terms && j < poly.terms) {
			if (termArray[i].exp == poly.termArray[j].exp) {
				// 指數相同：係數相加
				float sum = termArray[i].coef + poly.termArray[j].coef;
				if (sum != 0) {
					c.newTerm(sum, termArray[i].exp); // 如果相加不為 0，存入答案
				}
				i++;
				j++;
			} 
			else if (termArray[i].exp < poly.termArray[j].exp) {
				// 對方的指數比較大：先把對方的這項放進答案裡
				c.newTerm(poly.termArray[j].coef, poly.termArray[j].exp);
				j++;
			} 
			else {
				// 自己的指數比較大：先把自己的這項放進答案裡
				c.newTerm(termArray[i].coef, termArray[i].exp);
				i++;
			}
		}
		// 如果自己還有剩下的項沒跑完，全部補到答案裡
		while (i < terms) {
			c.newTerm(termArray[i].coef, termArray[i].exp);
			i++;
		}
		// 如果對方還有剩下的項沒跑完，全部補到答案裡
		while (j < poly.terms) {
			c.newTerm(poly.termArray[j].coef, poly.termArray[j].exp);
			j++;
		}
		return c; // 回傳相加好的多項式
	}
	// 4. 代入求值函數 (Eval)
	// 把數字 f 代進多項式裡面算答案（例如 x=2 代入 3x^2 + 1）
	float Eval(float f) const {
		float ans = 0;
		for (int i = 0; i < terms; i++) {
			// 每一項的答案 = 係數 * (f 的指數次方)，然後全部加起來
			ans += termArray[i].coef * pow(f, termArray[i].exp);
		}
		return ans;
	}
	// 5. 輸出運算子多載 (<<)
	// 讓你可以直接用 cout << 把它漂漂亮亮地印在螢幕上
	friend ostream& operator<<(ostream& os, const Polynomial& p) {
		if (p.getTerms() == 0) {
			os << "0"; // 如果沒有項，就印出 0
			return os;
		}
		for (int i = 0; i < p.getTerms(); i++) {
			float c = p.getCoef(i);
			int e = p.getExp(i);
			// 自動判斷並印出項與項之間的加減符號
			if (i > 0 && c > 0) {
				os << " + ";
			} else if (c < 0) {
				os << " - ";
			} else if (i > 0) {
				os << " ";
			}
			float absC = abs(c); // 取係數的絕對值來印
			if (absC != 1 || e == 0) {
				os << absC; // 係數不是 1、或者指數是 0 的時候，要把數字印出來
			}
			// 如果指數大於 0，就要印出英文字母 x 跟次方
			if (e > 0) {
				os << "x";
				if (e > 1) {
					os << "^" << e; // 指數大於 1 才印出像 ^2 的次方
				}
			}
		}
		return os;
	}
	// 6. 輸入運算子多載 (>>)
	// 讓你可以直接用 cin >> 讓使用者從鍵盤輸入多項式
	friend istream& operator>>(istream& is, Polynomial& p) {
		p.reset(); // 輸入前先把舊資料清空
		int n;
		cout << "多項式有幾項？: ";
		is >> n;
		cout << "輸入每一項的(係數 指數)：\n";
		for (int i = 0; i < n; i++) {
			float c;
			int e;
			cout << "第 " << i + 1 << " 項: ";
			is >> c >> e;
			p.newTerm(c, e); // 把輸入的係數和指數加到多項式裡
		}
		return is;
	};
};
// 7. newTerm 函數的詳細內容
// 負責把新的一項加進陣列裡。如果空間滿了，就會自動開兩倍大的新空間！
void Polynomial::newTerm(const float theCoef, const int theExp)
{
	if (theCoef == 0) return; // 如果係數剛好是 0，就不要存進去了
	// 如果目前的項數已經達到最大容量，代表空間不夠了
	if (terms == capacity)
	{
		capacity *= 2; // 把容量變成兩倍
		Term* temp = new Term[capacity]; // 宣告一個更大的新陣列
		copy(termArray, termArray + terms, temp); // 把舊陣列的資料全部複製過去
		delete[] termArray; // 刪掉舊的陣列，避免佔用記憶體
		termArray = temp; // 讓原本的指標指向這個更大的新陣列
	}
	// 把新的一項放進陣列尾巴，然後把 terms 的數量加 1
	termArray[terms].coef = theCoef;
	termArray[terms++].exp = theExp;
}
int main()
{
	Polynomial p1, p2; // 宣告兩個多項式物件 A 和 B
	cout << "建立A(x)" << endl;
	cin >> p1; // 讓使用者輸入第一個多項式 A
	cout << "\n建立B(x)" << endl;
	cin >> p2; // 讓使用者輸入第二個多項式 B
	cout << "\n運算結果" << endl;
	cout << "A(x) = " << p1 << endl;
	cout << "B(x) = " << p2 << endl;
	// 讓 A 和 B 相加，把結果存到 pSum 裡面
	Polynomial pSum = p1.Add(p2);
	cout << "A(x) + B(x) = " << pSum << endl;
	// 測試代入數字求值
	float xVal;
	cout << "\n請輸入x來計算A(x)+B(x)的值: ";
	cin >> xVal;
	// 帶入剛剛相加完的多項式 (pSum) 算答案
	cout << "A(" << xVal << ")+B(" << xVal << ") = " << pSum.Eval(xVal) << endl;
	return 0;
}
