/*
* why are all the throws in functions??? fuck is wrong with you???

* throw calls expand into a big chunk of instructions, by putting them in functions, it saves on code size wrapping them in functions
* this is probably resolved by compiler optimizations, oh well.
*/

#include <stdexcept>


namespace Exceptions {

	using Exception = std::exception;

	using RuntimeError = std::runtime_error;
	using InvalidArgument = std::invalid_argument;
	using RangeError = std::range_error;

	using OverflowError = std::overflow_error;
	using UnderflowError = std::underflow_error;

	using BadAllocation = std::bad_alloc;


	using IOError = std::ios::failure;

	

	class ShaderCompilationError : public RuntimeError {
	public:
		explicit ShaderCompilationError(const std::string& _Message) : RuntimeError(_Message.c_str()) {}
		explicit ShaderCompilationError(const char* _Message) : RuntimeError(_Message) {}
	};

	class ProgramLinkageError : public RuntimeError {
	public:
		explicit ProgramLinkageError(const std::string& _Message) : RuntimeError(_Message.c_str()) {}
		explicit ProgramLinkageError(const char* _Message) : RuntimeError(_Message) {}
	};

	class FramebufferUncompletenessError : public RuntimeError {
	public:
		explicit FramebufferUncompletenessError(const std::string& _Message, GLenum _FramebufferStatus) : RuntimeError(_Message.c_str()) {
			this->FramebufferStatus = _FramebufferStatus;
		}

		explicit FramebufferUncompletenessError(const char* _Message, GLenum _FramebufferStatus) : RuntimeError(_Message) {
			this->FramebufferStatus = _FramebufferStatus;
		}

		mutable GLenum FramebufferStatus;
	};
}

namespace Exceptions {

	template<class ExceptionClass>
	[[noreturn]]
#ifdef DEBUG_BUILD
	inline void ThrowException(const ExceptionClass& Exception)
#else
	void ThrowException(const ExceptionClass& Exception)
#endif
	{
		static_assert(std::is_base_of_v<std::exception, ExceptionClass>, "Attempted to throw a exception that is not derived from class 'std::exception'.");
		throw Exception;
	}

	template<class Exception>
	[[noreturn]]
	inline void ThrowException(const char* Message) {
		ThrowException<Exception>(Exception(Message));
	}

	template<class Exception>
	[[noreturn]]
	inline void ThrowException(const std::string& Message) {
		ThrowException<Exception>(Exception(Message));
	}


	template<class Exception, typename ...Args_T>
	[[noreturn]]
	inline void ThrowExceptionExpression(Args_T... Message) {
		ThrowException<Exception>(std::move(StringHelper::Combine<Args_T...>(Message...)));
	}

	template<class Exception>
	[[noreturn]]
	inline void ThrowException() {
		ThrowException<Exception>(Exception());
	}


	template<typename ...Args_T>
	[[noreturn]]
	inline void ThrowSDLError(Args_T... Message) {
		ThrowExceptionExpression<RuntimeError>(Message..., "\nSDL Error: ", SDL_GetError());
	}
}