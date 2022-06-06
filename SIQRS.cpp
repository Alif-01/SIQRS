#include "RunAllTests.h"
#include "Protocol.h"

using namespace std;

int main()
{
	cout << "Hello SIQRS." << endl;
#ifdef DEBUG
	cout << "DEBUG on." << endl;
#endif
    // RunAllTests();
	while(1) run_protocol();

	return 0;
}