#include <header.hpp>
#include <mutex>

int main(int argc, char *argv[]) {
  std::mutex mut;
  Create_HASH::logging();
  unsigned int thread_count;
  if (argc >= 2)
  {
    thread_count = boost::lexical_cast<int unsigned>(argv[1]);
  }
  else thread_count = std::thread::hardware_concurrency();
  std::cout << "Thread count: "<< thread_count << std::endl;

  std::vector<std::thread> threads;
  threads.reserve(thread_count);
  for (unsigned i = 0; i < thread_count; i++) {
    threads.emplace_back(Create_HASH::create_hash, std::ref(mut));
  }
  for (std::thread &thr : threads) {
    thr.join();
  }
  Create_HASH::exit_f();
  return 0;
}