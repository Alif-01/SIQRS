#include "SIQRS.h"
#include "RunAllTests.h"

using namespace std;

int main()
{
	cout << "Hello SIQRS." << endl;
#ifdef DEBUG
	cout << "DEBUG on." << endl;
#endif
    RunAllTests();
	return 0;
}