

#include "Filesystem.h"
#include "Logging.h"


#define VECTOR 0

#if VECTOR
#include <vector>
#else
#include <array>
#endif

const int Max = 50000;
const int NumThreads = 50;


void foo(STORAGE::Filesystem *f, int id) {
	for (int i = id; i < Max; i += NumThreads) {

		// Create random file
		std::ostringstream filename;
		filename << "MyFile" << i;
		STORAGE::File file = f->select(filename.str());
		auto writer = f->getWriter(file);
		

		// Create random data
		std::ostringstream data;
		data << "Thread # " << i;

		f->lock(file, STORAGE::WRITE);
		{
			writer.write(data.str().c_str(), data.str().size());
		}
		f->unlock(file, STORAGE::WRITE);

		if (i % 100 == 0) {
			std::cout << "Just finishd " << i << std::endl;
		}
	}
	return;
}

int main() {
	
	static int c;
	using namespace std::literals;
	STORAGE::Filesystem f("test.stash");
#if VECTOR
  std::vector<std::thread> threads;
#else
	std::array<std::thread,NumThreads> threads;
#endif
	
	srand((unsigned int)time(NULL));
	
	for (int i = 0; i < NumThreads; ++i) {
#if VECTOR
		threads.push_back(std::thread(foo, &f, i));
#else
		threads[i] = std::thread(foo, &f, i);
#endif
	}

	for (auto& th : threads) {
		th.join();
	}
#if VECTOR
	threads.clear();
#endif
	
	f.shutdown();
	return 0;
}
