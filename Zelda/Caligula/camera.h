// camera-h

#ifndef INCLUDED_CAMERA_H
#define INCLUDED_CAMERA_H

// Only allows one global camera, what can get changed
// a global point in space, which gets used by rendering

struct SDL_Rect;

struct camera
{
	// Singleton Implementation
	camera(const camera&) = delete;
	camera(camera&&) = delete;
	static camera& instance();
	camera& operator=(const camera& rhs) = delete;

	// Methods
	void set_position(int p_x, int p_y);
	SDL_Rect get_position() const;

private:
	// Singleton - This class holds a monopoly over cameras
	camera();				
	
	// There can only be ONE camera

	int m_x = 0;
	int m_y = 0;
};

#endif // !INCLUDED_CAMERA_H



