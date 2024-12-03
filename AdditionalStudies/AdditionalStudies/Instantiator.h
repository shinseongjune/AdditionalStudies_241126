#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

using namespace std;

struct TestObject {
    int value;
    TestObject(int v) : value(v) {
        cout << "TestObject ������: " << value << endl;
    }
    ~TestObject() {
        cout << "TestObject �Ҹ��: " << value << endl;
    }
};

// ObjectData�� ���ø����� �����Ͽ� Ÿ�� ����
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

    // ObjectData�� ������ ���� ó��
    vector<unique_ptr<ObjectData<TestObject>>> objs;

    Instantiator() = default;
    Instantiator(const Instantiator&) = delete;
    Instantiator& operator=(const Instantiator&) = delete;

public:
    ~Instantiator() {
        cout << "Instantiator �Ҹ�" << endl;
    }

    static Instantiator* GetInstance() {
        if (_instance.get() == nullptr) {
            cout << "Instantiator �ν��Ͻ� �ʱ�ȭ" << endl;
            _instance.reset(new Instantiator());
        }
        else {
            cout << "Instantiator �ν��Ͻ� ����" << endl;
        }
        return _instance.get();
    }

    template <typename T, typename... Args>
    T* Make(Args&&... args) {
        if (objs.size() >= capacity) {
            CollectGarbage();

            if (objs.size() >= capacity) {
                cout << "��ü ���� ����: �뷮 �ʰ�" << endl;
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