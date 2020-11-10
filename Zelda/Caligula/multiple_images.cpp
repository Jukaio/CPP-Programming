// multiple_images.cpp

#include "multiple_images.h"

multiple_images::multiple_images()
{
}

void multiple_images::add_image(const static_image& p_image)
{
	m_images.push_back(p_image);
}

void multiple_images::on_enable()
{
}

void multiple_images::update(int p_ms)
{
}

void multiple_images::render()
{
	for(auto&& image : m_images)
		image.render();
}

void multiple_images::on_disable()
{
}

void multiple_images::on_collision(entity* p_other)
{
}

void multiple_images::reset()
{
}
