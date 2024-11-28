#include <iostream>
#include "Instantiator.h"
using namespace std;

class Test {
public:
    int value;
    Test(int x) : value(x) {
        cout << "Test ����: " << value << endl;
    }
    ~Test() {
        cout << "Test �Ҹ�: " << value << endl;
    }
};

int main() {
    Instantiator* inst = Instantiator::GetInstance();

    // 1. ��ü ����
    cout << "=== ��ü ���� ===" << endl;
    Test* obj1 = inst->Make<Test>(1);
    Test* obj2 = inst->Make<Test>(2);
    Test* obj3 = inst->Make<Test>(3);
    cout << "���� ��ü ��: " << inst->GetSize() << endl;

    // 2. ���� �߰�
    cout << "=== ���� �߰� ===" << endl;
    inst->AddReference(obj1, obj2);
    inst->CollectGarbage();
    cout << "���� ��ü ��: " << inst->GetSize() << endl;

    // 3. ���� ����
    cout << "=== ���� ���� ===" << endl;
    inst->RemoveReference(obj1, obj2);
    inst->CollectGarbage();
    cout << "���� ��ü ��: " << inst->GetSize() << endl;

    // 4. �뷮 �ʰ� �׽�Ʈ
    cout << "=== �뷮 �ʰ� ===" << endl;
    for (int i = 4; i <= 10; ++i) {
        Test* obj = inst->Make<Test>(i);
        if (obj == nullptr) {
            cout << "��ü ���� ����: " << i << endl;
        }
    }
    inst->CollectGarbage();
    cout << "���� ��ü ��: " << inst->GetSize() << endl;

    // 5. ������ ���� ����
    cout << "=== ������ ���� ���� ===" << endl;
    inst->RemoveReference(obj2, obj3);
    inst->CollectGarbage();
    cout << "���� ��ü ��: " << inst->GetSize() << endl;

    return 0;
}
