#include "ForestView.h"
#include "tree.h"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

namespace fs = std::filesystem;
using namespace std;

int main()
{
	#ifdef _WIN32
		_setmode(_fileno(stdout), _O_U16TEXT);
	#endif
	
	TexturesManager texture_manager;

	ForestView f(texture_manager);
	f.run_loop();
	return 0;
}