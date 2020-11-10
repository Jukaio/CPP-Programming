// multiple_images.h

#ifndef INCLUDED_MULTIPLE_IMAGES_H
#define INCLUDED_MULTIPLE_IMAGES_H

#include "entity.h"
#include <vector>
#include "static_image.h"

// A class which contains multiple images

struct multiple_images : entity
{
	multiple_images();

	void add_image(const static_image& p_image);
	virtual void on_enable() override;
	virtual void update(int p_ms) override;
	virtual void render() override;
	virtual void on_disable() override;
	virtual void on_collision(entity* p_other) override;
	virtual void reset() override;
	
private:
	std::vector<static_image> m_images;

};

#endif // !INCLUDED_MULTIPLE_IMAGES_H



