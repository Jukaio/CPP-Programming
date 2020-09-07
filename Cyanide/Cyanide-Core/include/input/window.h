// window.h

#ifndef INCLUDED_WINDOW_H
#define INCLUDED_WINDOW_H

struct SDL_Window;

struct window_info
{
	void set_window(SDL_Window* p_window);
	bool resized();
	void refresh_window_size();
	void reset();
	void update_resize(bool p_resized);

	bool is_set();

	int window_w();
	int window_h();
private:
	bool m_current_resized; //If input got received
	SDL_Window* m_window = nullptr;
	int m_w, m_h;
};

#endif // !INCLUDED_WINDOW_H
