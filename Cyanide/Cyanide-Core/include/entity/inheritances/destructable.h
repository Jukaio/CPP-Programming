// destructable.h

#ifndef INCLUDED_DESTRUCTABLE_H
#define INCLUDED_DESTRUCTABLE_H

struct destructable
{
	destructable() = default;
	virtual ~destructable() = default;
};

#endif // !INCLUDED_DESTRUCTABLE_H
