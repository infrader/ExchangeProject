#include"Logger.hpp"

Logger& Logger::GetIntence(){
	static Logger intence;
	return intence;
}
