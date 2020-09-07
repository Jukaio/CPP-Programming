// updatable.h

#ifndef INCLUDED_UPDATABLE_H
#define INCLUDED_UPDATABLE_H

struct updatable
{
	~updatable() = default;

	void set_active(bool p_active);
	bool get_active();
	virtual void update(int p_ms) = 0;

protected:

	virtual void on_enable() = 0;
	virtual void on_disable() = 0;
	updatable(bool p_active);

private:
	bool m_active;
};
#endif // !INCLUDED_UPDATABLE_H



