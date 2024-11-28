#pragma once
#include <vector>
#include <map>
#include <algorithm>

// 객체 생성기

using namespace std;

struct ObjectData {
	void* ptr;
	int gen;

	vector<void*> subscribers;
};

class Instantiator {
private:
	static Instantiator* _instance;
	static const int capacity = 8;

	vector<ObjectData> objs;

	Instantiator() = default;
	Instantiator(const Instantiator&) = delete;
	Instantiator& operator=(const Instantiator&) = delete;

public:
	~Instantiator() {
		for (auto& objData: objs) {
			if (objData.ptr) {
				delete objData.ptr;
			}
		}
		objs.clear();
	}

	static Instantiator* GetInstance() {
		if (_instance == nullptr) {
			_instance = new Instantiator();
		}

		return _instance;
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

		T* newObject = new T(forward<Args>(args)...);

		ObjectData data = {
			newObject,
			0,
			vector<void*>()
		};

		objs.push_back(data);
		return newObject;
	}

	void AddReference(void* obj, void* ref) {
		auto it = find_if(objs.begin(), objs.end(),
			[obj](const ObjectData& data) {
				return data.ptr == obj;
			});

		if (it != objs.end() && find(it->subscribers.begin(), it->subscribers.end(), ref) == it->subscribers.end()) {
			it->subscribers.push_back(ref);
		}
	}

	void RemoveReference(void* obj, void* ref) {
		auto it = find_if(objs.begin(), objs.end(),
			[obj](const ObjectData& data) {
				return data.ptr == obj;
			});

		if (it != objs.end()) {
			auto& subs = it->subscribers;
			subs.erase(remove(subs.begin(), subs.end(), ref), subs.end());
		}
	}

	void CollectGarbage() {
		int initialSize = objs.size();

		for (int i = 0; i <= 4; ++i) {
			objs.erase(remove_if(objs.begin(), objs.end(),
				[i](ObjectData& objData) {
					if (objData.gen == i && objData.subscribers.empty()) {
						if (objData.ptr) {
							delete objData.ptr;
							objData.ptr = nullptr;
						}
						return true;
					}
					return false;
				}),
				objs.end());
		}

		for (auto& objData : objs) {
			if (objData.gen < 4) {
				objData.gen++;
			}
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

Instantiator* Instantiator::_instance = nullptr;