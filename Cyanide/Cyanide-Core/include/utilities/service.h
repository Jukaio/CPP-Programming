// service

#ifndef INCLUDED_SERVICE_H
#define INCLUDED_SERVICE_H

template <typename T>
struct service
{
	static void set(T* p_instance) { m_instance = p_instance; }
	static T* get() { return m_instance; }

private:
	service() {}
	static T* m_instance;
};

template <typename T>
T* service<T>::m_instance = nullptr;

#endif // !INCLUDED_SERVICE_H
