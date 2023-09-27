//
// Created by HuiYi on 2023/6/3.
//

#ifndef HW_TOOLS_SINGLETON_H
#define HW_TOOLS_SINGLETON_H

namespace utils{

	template <typename T>
	class Singleton {
	private:
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		static T* instance;

	protected:
		Singleton() {}

	public:
		static T& Instance() {
			if(instance == nullptr) {
				instance = new T();
			}
			return *instance;
		}
	};

	template <typename T>
	T* Singleton<T>::instance = nullptr;

}

#endif //HW_TOOLS_SINGLETON_H
