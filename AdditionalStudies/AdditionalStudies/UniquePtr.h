#pragma once

template <typename T>
class UniquePtr {
private:
	T* ptr;

public:
	// Constructor
	UniquePtr() noexcept : ptr(nullptr) {}
	explicit UniquePtr(T* p = nullptr) : ptr(p) {}

	// Destructor
	~UniquePtr() { reset(); }

	// Prevent Copy
	UniquePtr(const UniquePtr&) = delete;
	UniquePtr& operator=(const UniquePtr&) = delete;

	// Move Semantics
	UniquePtr(UniquePtr&& other) noexcept : ptr(std::exchange(other.ptr, nullptr)) {}
	
	UniquePtr& operator=(UniquePtr&& other) noexcept {
		if (this != &other) {
			if (ptr != nullptr) delete ptr;
			ptr = std::exchange(other.ptr, nullptr);
		}
		return *this;
	}

	// Access
	T* get() const { return ptr; }
	T* operator->() const { return ptr; }
	T& operator*() const { return *ptr; }

	// Functions
	T* release() {
		T* temp = ptr;
		ptr = nullptr;
		return temp;
	}

	void reset(T* other = nullptr) {
		T* old_ptr = std::exchange(ptr, other);
		if (old_ptr != nullptr) delete old_ptr;
	}

	operator bool() const {
		return ptr != nullptr;
	}
};