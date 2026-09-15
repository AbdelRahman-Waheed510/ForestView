#include "pchApp.h"
#include "ForestView.h"

namespace fs = std::filesystem;
using namespace std;

int main()
{
	TexturesManager texture_manager;  

	ForestView f(texture_manager);
	f.run_loop();
	return 0;
}