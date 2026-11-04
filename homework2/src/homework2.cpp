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
