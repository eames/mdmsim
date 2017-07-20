#ifndef RAYIN_HEADER
#define RAYIN_HEADER
#include <string>

class Rayin {
public:
	Rayin(const std::string& filename);
	~Rayin();
private:
	bool isOwner;
};

#endif
