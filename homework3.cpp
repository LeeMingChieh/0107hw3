#include <iostream>
using namespace std;

struct Node {
    int coef; // �Y��
    int exp;  // ����
    Node* link;

    Node(int c = 0, int e = 0) : coef(c), exp(e), link(nullptr) {}
};

class Polynomial {
private:
    Node* first; //����쵲���Ĥ@�Ӹ`�I

    //�R���쵲
    void clear() {
        if (!first) return;
        Node* current = first->link;
        while (current != first) {
            Node* temp = current;
            current = current->link;
            delete temp;
        }
        delete first;
        first = nullptr;
    }

    // �ƻs
    void copy(const Polynomial& other) {
        if (!other.first) {
            first = nullptr;
            return;
        }

        first = new Node();
        Node* current = other.first->link;
        Node* tail = first;
        while (current != other.first) {
            tail->link = new Node(current->coef, current->exp);
            tail = tail->link;
            current = current->link;
        }
        tail->link = first;
    }

public:
    // �غc���
    Polynomial() : first(nullptr) {}

    // �ƻs�غc���
    Polynomial(const Polynomial& other) {
        copy(other);
    }

    //�Ѻc���
    ~Polynomial() {
        clear();
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            clear();
            copy(other);
        }
        return *this;
    }

    // �h�� >>�A�Ω��J
    friend istream& operator>>(istream& is, Polynomial& x) {
        int n;
        is >> n; // ����
        x.clear();
        x.first = new Node();
        Node* tail = x.first;

        for (int i = 0; i < n; ++i) {
            int coef, exp;
            is >> coef >> exp;
            tail->link = new Node(coef, exp);
            tail = tail->link;
        }
        tail->link = x.first;
        return is;
    }

    // �h�� <<�A�Ω��X
    friend ostream& operator<<(ostream& os, const Polynomial& x) {
        if (!x.first) return os;

        Node* current = x.first->link;
        while (current != x.first) {
            os << current->coef << "x^" << current->exp;
            if (current->link != x.first) os << " + ";
            current = current->link;
        }
        return os;
    }

    // �[�k�B��
    Polynomial operator+(const Polynomial& b) const {
        if (!first) return b;  // �p�G��e�h�������šA��^ b
        if (!b.first) return *this;  // �p�G b ���šA��^��e�h����

        Polynomial result;
        Node* aPtr = first->link;
        Node* bPtr = b.first->link;
        Node* tail = result.first = new Node();

        while (aPtr != first || bPtr != b.first) {
            if (aPtr != first && (bPtr == b.first || aPtr->exp > bPtr->exp)) {
                tail->link = new Node(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            }
            else if (bPtr != b.first && (aPtr == first || aPtr->exp < bPtr->exp)) {
                tail->link = new Node(bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            }
            else {
                int sum = aPtr->coef + bPtr->coef;
                if (sum != 0) {
                    tail->link = new Node(sum, aPtr->exp);
                }
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
            tail = tail->link;
        }
        tail->link = result.first;
        return result;
    }

    // ��k�B��
    Polynomial operator-(const Polynomial& b) const {
        if (!first) return b;
        if (!b.first) return *this; 

        Polynomial result;
        Node* aPtr = first->link;
        Node* bPtr = b.first->link;
        Node* tail = result.first = new Node();

        while (aPtr != first || bPtr != b.first) {
            if (aPtr->exp > bPtr->exp) {
                tail->link = new Node(aPtr->coef, aPtr->exp);
                aPtr = aPtr->link;
            }
            else if (aPtr->exp < bPtr->exp) {
                tail->link = new Node(-bPtr->coef, bPtr->exp);
                bPtr = bPtr->link;
            }
            else {
                int diff = aPtr->coef - bPtr->coef;
                if (diff != 0) {
                    tail->link = new Node(diff, aPtr->exp);
                }
                aPtr = aPtr->link;
                bPtr = bPtr->link;
            }
            tail = tail->link;
        }
        tail->link = result.first;
        return result;
    }

    // ���k�B��
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        if (!first) return b;
        if (!b.first) return *this;

        if (!first || !b.first) return result;

        Node* aPtr = first->link;
        while (aPtr != first) {
            Node* bPtr = b.first->link;
            Polynomial temp;
            Node* tail = temp.first = new Node();

            while (bPtr != b.first) {
                tail->link = new Node(aPtr->coef * bPtr->coef, aPtr->exp + bPtr->exp);
                tail = tail->link;
                bPtr = bPtr->link;
            }
            tail->link = temp.first;
            result = result + temp;
            aPtr = aPtr->link;
        }
        return result;
    }

    // �� x �ȭp��h��������
    int Evaluate(int x) const {
        int result = 0;
        if (!first) return result;
        Node* current = first->link;
        while (current != first) {
            result += current->coef * pow(x, current->exp);
            current = current->link;
        }
        return result;
    }
};

int main() {
    Polynomial p1, p2;
    cout << "��J�Ĥ@�Ӧh�����]��J���ơB�C�����Y�ƩM����^�G\n";
    cin >> p1;
    cout << "��J�ĤG�Ӧh�����]��J���ơB�C�����Y�ƩM����^�G\n";
    cin >> p2;

    Polynomial sum = p1 + p2;
    cout << "�[�k���G�G" << sum << endl;

    Polynomial diff = p1 - p2;
    cout << "��k���G�G" << diff << endl;

    Polynomial prod = p1 * p2;
    cout << "���k���G�G" << prod << endl;

    int x;
    cout << "��J x ���ȡG";
    cin >> x;
    cout << "�p�⵲�G(�[�k)�G" << sum.Evaluate(x) << endl;
    cout << "�p�⵲�G(��k)�G" << diff.Evaluate(x) << endl;
    cout << "�p�⵲�G(���k)�G" << prod.Evaluate(x) << endl;

    return 0;
}
