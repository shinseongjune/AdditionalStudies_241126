#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

using namespace std;

struct TestObject {
    int value;
    TestObject(int v) : value(v) {
        cout << "TestObject 생성됨: " << value << endl;
    }
    ~TestObject() {
        cout << "TestObject 소멸됨: " << value << endl;
    }
};

// ObjectData를 템플릿으로 변경하여 타입 보존
template <typename T>
struct ObjectData {
    unique_ptr<T> ptr;
    int gen;
    vector<void*> subscribers;
};

class Instantiator {
private:
    static unique_ptr<Instantiator> _instance;
    static const int capacity = 8;

    // ObjectData를 다형성 없이 처리
    vector<unique_ptr<ObjectData<TestObject>>> objs;

    Instantiator() = default;
    Instantiator(const Instantiator&) = delete;
    Instantiator& operator=(const Instantiator&) = delete;

public:
    ~Instantiator() {
        cout << "Instantiator 소멸" << endl;
    }

    static Instantiator* GetInstance() {
        if (_instance.get() == nullptr) {
            cout << "Instantiator 인스턴스 초기화" << endl;
            _instance.reset(new Instantiator());
        }
        else {
            cout << "Instantiator 인스턴스 재사용" << endl;
        }
        return _instance.get();
    }

    template <typename T, typename... Args>
    T* Make(Args&&... args) {
        if (objs.size() >= capacity) {
            CollectGarbage();

            if (objs.size() >= capacity) {
                cout << "객체 생성 실패: 용량 초과" << endl;
                return nullptr;
            }
        }

        auto newObject = make_unique<T>(forward<Args>(args)...);

        auto data = make_unique<ObjectData<T>>();
        data->ptr = move(newObject);
        data->gen = 1;

        objs.push_back(move(data));
        return objs.back()->ptr.get();
    }

    void AddReference(void* obj, void* ref) {
        auto it = find_if(objs.begin(), objs.end(),
            [obj](const unique_ptr<ObjectData<TestObject>>& data) {
                return data->ptr.get() == obj;
            });

        if (it != objs.end() && find((*it)->subscribers.begin(), (*it)->subscribers.end(), ref) == (*it)->subscribers.end()) {
            (*it)->subscribers.push_back(ref);
        }
    }

    void RemoveReference(void* obj, void* ref) {
        auto it = find_if(objs.begin(), objs.end(),
            [obj](const unique_ptr<ObjectData<TestObject>>& data) {
                return data->ptr.get() == obj;
            });

        if (it != objs.end()) {
            auto& subs = (*it)->subscribers;
            subs.erase(remove(subs.begin(), subs.end(), ref), subs.end());
        }
    }

    void CollectGarbage() {
        int initialSize = objs.size();

        for (int i = 1; i <= 4; ++i) {
            objs.erase(remove_if(objs.begin(), objs.end(),
                [i](unique_ptr<ObjectData<TestObject>>& objData) {
                    return objData->gen == i && objData->subscribers.empty();
                }),
                objs.end());

            if (objs.size() < capacity) break;
        }

        for (auto& obj : objs) {
            if (obj->gen < 4) obj->gen++;
        }

        cout << "Collected " << (initialSize - objs.size()) << " objects." << endl;
    }

    int GetCapacity() const {
        return capacity;
    }

    int GetSize() const {
        return objs.size();
    }
};

unique_ptr<Instantiator> Instantiator::_instance = nullptr;