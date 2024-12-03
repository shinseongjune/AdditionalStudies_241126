#include "Instantiator.h"


int main() {
    // Instantiator �ν��Ͻ� ��������
    Instantiator* instantiator = Instantiator::GetInstance();

    // ��ü ���� �׽�Ʈ
    cout << "��ü ���� �׽�Ʈ:" << endl;
    TestObject* obj1 = instantiator->Make<TestObject>(1);
    TestObject* obj2 = instantiator->Make<TestObject>(2);
    TestObject* obj3 = instantiator->Make<TestObject>(3);
    TestObject* obj4 = instantiator->Make<TestObject>(4);

    // ���� �߰� �׽�Ʈ
    cout << "���� �߰� �׽�Ʈ:" << endl;
    instantiator->AddReference(obj1, obj2); // obj2�� obj1�� ����
    instantiator->AddReference(obj3, obj4); // obj4�� obj3�� ����

    // ������ �÷��� �� ���� Ȯ��
    cout << "\n������ �÷��� ��:" << endl;
    cout << "���� ��ü ��: " << instantiator->GetSize() << endl;

    // ���� ����
    cout << "���� ���� �׽�Ʈ:" << endl;
    instantiator->RemoveReference(obj1, obj2);
    instantiator->RemoveReference(obj3, obj4);

    // ������ �÷��� ����
    cout << "\n������ �÷��� ����:" << endl;
    instantiator->CollectGarbage();

    // ������ �÷��� �� ���� Ȯ��
    cout << "������ �÷��� ��:" << endl;
    cout << "���� ��ü ��: " << instantiator->GetSize() << endl;

    // �뷮 �ʰ� �׽�Ʈ
    cout << "\n�뷮 �ʰ� �׽�Ʈ:" << endl;
    for (int i = 5; i <= 12; ++i) {
        TestObject* obj = instantiator->Make<TestObject>(i);
        if (obj == nullptr) {
            cout << "��ü ���� ����: " << i << endl;
        }
    }

    // ���α׷� ���� �� �Ҹ��� ȣ�� Ȯ��
    cout << "\n���α׷� ���� �� ��ü �Ҹ� �׽�Ʈ" << endl;
    return 0;
}
