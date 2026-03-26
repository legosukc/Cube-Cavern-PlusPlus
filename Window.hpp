
namespace Game::Classes {
	class Window;
}

class Game::Classes::Window {

	mutable Uint32 LastTick;

public:
	void Create(const char* WindowTitle, int Width, int Height, Uint32 Flags = 0, int X = SDL_WINDOWPOS_CENTERED, int Y = SDL_WINDOWPOS_CENTERED);
	inline Window() = default;

	inline void Destroy();

	SDL_Window* SDLWindow;
	SDL_GLContext GLContext;


	bool PollEvents();

	Math::IVector2 GetPosition() const;
	Math::IVector2 GetSize() const;

	void Present() const;
};


void Game::Classes::Window::Create(const char* WindowTitle, int Width, int Height, Uint32 Flags, int X, int Y) {

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	this->SDLWindow = SDL_CreateWindow(WindowTitle, X, Y, Width, Height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | Flags);

	unlikely_branch
	if (this->SDLWindow == NULL) {
		Exceptions::ThrowSDLError("Failed to create SDL_Window.");
	}


	this->GLContext = SDL_GL_CreateContext(this->SDLWindow);

	unlikely_branch
	if (this->GLContext == NULL) {
		Exceptions::ThrowSDLError("Failed to create SDL_GLContext.");
	}

	unlikely_branch
	if (const int ErrorCode = SDL_GL_MakeCurrent(this->SDLWindow, this->GLContext); ErrorCode != 0) {
		Exceptions::ThrowSDLError("Error while calling SDL_GL_MakeCurrent. ErrorCode: ", ErrorCode);
	}

	gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);

	this->LastTick = SDL_GetTicks();
}

void Game::Classes::Window::Destroy() {

	if (this->GLContext != NULL) {
		SDL_GL_DeleteContext(this->GLContext);
		this->GLContext = NULL;
	}

	if (this->SDLWindow != NULL) {
		SDL_DestroyWindow(this->SDLWindow);
		this->SDLWindow = NULL;
	}
}



bool Game::Classes::Window::PollEvents() {

	static thread_local SDL_Event Event;
	while (SDL_PollEvent(&Event)) {

		switch (Event.type) {
		case SDL_QUIT:
			return true;
		}
	}

	return false;
}

Math::IVector2 Game::Classes::Window::GetPosition() const {

	Math::IVector2 Position;
	SDL_GetWindowPosition(this->SDLWindow, &Position.X, &Position.Y);
	return Position;
}

Math::IVector2 Game::Classes::Window::GetSize() const {

	Math::IVector2 Size;
	SDL_GetWindowSize(this->SDLWindow, &Size.X, &Size.Y);
	return Size;
}


void Game::Classes::Window::Present() const {

	const Uint32 Ticks = SDL_GetTicks();
	if (Ticks - this->LastTick > 0) {
		SDL_Delay(SDL_min(Ticks - this->LastTick, 16));
	}
	this->LastTick = SDL_GetTicks();

	SDL_GL_SwapWindow(this->SDLWindow);
}