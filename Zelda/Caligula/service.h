// service.h

#ifndef INCLUDED_SERVICE_H
#define INCLUDED_SERVICE_H

// service-locator instead of having a lot of singletons
// allows global access to the required service (if it gets set ofc)
// Since it is a template it hides class dependencies 

template <typename T>
struct service
{
	static void set(T* p_instance) { m_instance = p_instance; };
	static T* get() { return m_instance; };

private:
	static T* m_instance;
	service() {};
};

template <typename T>
T* service<T>::m_instance = nullptr;

#endif // !INCLUDED_SERVICE_H
