#include "Instantiator.h"


int main() {
    // Instantiator 인스턴스 가져오기
    Instantiator* instantiator = Instantiator::GetInstance();

    // 객체 생성 테스트
    cout << "객체 생성 테스트:" << endl;
    TestObject* obj1 = instantiator->Make<TestObject>(1);
    TestObject* obj2 = instantiator->Make<TestObject>(2);
    TestObject* obj3 = instantiator->Make<TestObject>(3);
    TestObject* obj4 = instantiator->Make<TestObject>(4);

    // 참조 추가 테스트
    cout << "참조 추가 테스트:" << endl;
    instantiator->AddReference(obj1, obj2); // obj2가 obj1을 참조
    instantiator->AddReference(obj3, obj4); // obj4가 obj3을 참조

    // 가비지 컬렉션 전 상태 확인
    cout << "\n가비지 컬렉션 전:" << endl;
    cout << "현재 객체 수: " << instantiator->GetSize() << endl;

    // 참조 제거
    cout << "참조 제거 테스트:" << endl;
    instantiator->RemoveReference(obj1, obj2);
    instantiator->RemoveReference(obj3, obj4);

    // 가비지 컬렉션 실행
    cout << "\n가비지 컬렉션 실행:" << endl;
    instantiator->CollectGarbage();

    // 가비지 컬렉션 후 상태 확인
    cout << "가비지 컬렉션 후:" << endl;
    cout << "현재 객체 수: " << instantiator->GetSize() << endl;

    // 용량 초과 테스트
    cout << "\n용량 초과 테스트:" << endl;
    for (int i = 5; i <= 12; ++i) {
        TestObject* obj = instantiator->Make<TestObject>(i);
        if (obj == nullptr) {
            cout << "객체 생성 실패: " << i << endl;
        }
    }

    // 프로그램 종료 시 소멸자 호출 확인
    cout << "\n프로그램 종료 시 객체 소멸 테스트" << endl;
    return 0;
}
