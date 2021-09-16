#include <exception>

namespace Utils
{	
	class NotImplementException : public std::exception
	{
	public:
		const char* what() const noexcept override
		{
			return "Not Implement";
		}
	};
}