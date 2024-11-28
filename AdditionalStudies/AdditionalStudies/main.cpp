#include <iostream>
#include "Instantiator.h"
using namespace std;

class Test {
public:
    int value;
    Test(int x) : value(x) {
        cout << "Test 생성: " << value << endl;
    }
    ~Test() {
        cout << "Test 소멸: " << value << endl;
    }
};

int main() {
    Instantiator* inst = Instantiator::GetInstance();

    // 1. 객체 생성
    cout << "=== 객체 생성 ===" << endl;
    Test* obj1 = inst->Make<Test>(1);
    Test* obj2 = inst->Make<Test>(2);
    Test* obj3 = inst->Make<Test>(3);
    cout << "현재 객체 수: " << inst->GetSize() << endl;

    // 2. 참조 추가
    cout << "=== 참조 추가 ===" << endl;
    inst->AddReference(obj1, obj2);
    inst->CollectGarbage();
    cout << "현재 객체 수: " << inst->GetSize() << endl;

    // 3. 참조 제거
    cout << "=== 참조 제거 ===" << endl;
    inst->RemoveReference(obj1, obj2);
    inst->CollectGarbage();
    cout << "현재 객체 수: " << inst->GetSize() << endl;

    // 4. 용량 초과 테스트
    cout << "=== 용량 초과 ===" << endl;
    for (int i = 4; i <= 10; ++i) {
        Test* obj = inst->Make<Test>(i);
        if (obj == nullptr) {
            cout << "객체 생성 실패: " << i << endl;
        }
    }
    inst->CollectGarbage();
    cout << "현재 객체 수: " << inst->GetSize() << endl;

    // 5. 마지막 참조 제거
    cout << "=== 마지막 참조 제거 ===" << endl;
    inst->RemoveReference(obj2, obj3);
    inst->CollectGarbage();
    cout << "현재 객체 수: " << inst->GetSize() << endl;

    return 0;
}
