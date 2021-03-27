// Copyright 2021 Alexander Guchkov <firer.a45@gmail.com>

#include <header.hpp>

const unsigned size_file = 10 * 1024 * 1024;

auto result = nlohmann::json::object();
auto mass_for_out = nlohmann::json::array();

bool a = true;

Create_HASH::Create_HASH(){}

Create_HASH::~Create_HASH(){}

void Create_HASH::exit_signal_catcher(int sig)
{
  sig += 1;
  a = false;
}

void Create_HASH::create_hash(std::mutex &mtx)
{
  signal(SIGINT, &Create_HASH::exit_signal_catcher);
  std::string for_zeros_ending_check = "0000";
  bool k = true;
  auto start = std::chrono::high_resolution_clock::now();
  auto finish = std::chrono::high_resolution_clock::now();
  while (a)
  {
    std::string input_rand = std::to_string(std::rand());
    std::string hash_key = picosha2::hash256_hex_string(input_rand);
    if (k)
    {
      start = std::chrono::high_resolution_clock::now();
    }
    if (hash_key.substr(hash_key.size() - for_zeros_ending_check.size()) ==
        for_zeros_ending_check)
    {
      finish = std::chrono::high_resolution_clock::now();
      k = true;
      int r_time = std::chrono::duration_cast<std::chrono::microseconds>
          (finish - start).count();
      bool m = true;
      while (m)
      {
        if (mtx.try_lock())
        {
          m = false;
          BOOST_LOG_TRIVIAL(info) << std::endl
                                  << "data: [" << input_rand
                                  << "] hash: [" << hash_key << "]"
                                  << std::endl;
          result["time"] = r_time;
          result["hash"] = hash_key;
          result["data"] = input_rand;
          mass_for_out.push_back(result);
          mtx.unlock();
        }
      }
    } else {
      k = false;
      BOOST_LOG_TRIVIAL(trace) << std ::endl
                               << "data: [" << input_rand
                               << "] hash: [" << hash_key << "]" << std::endl;
    }
  }
}

void Create_HASH::logging()
{
  const std::string format = "[%TimeStamp%]: %Message%";

  auto sink_file = boost::log::add_file_log(
      boost::log::keywords::file_name = "logs/log_info_%N.log",
      boost::log::keywords::rotation_size = size_file,
      boost::log::keywords::format = format);
  sink_file->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::trace);

  auto sink_console = boost::log::add_console_log(
      std::cout,
      boost::log::keywords::format = format);
  sink_console->set_filter(
      boost::log::trivial::severity >= boost::log::trivial::info);

  boost::log::add_common_attributes();

  srand(time(nullptr));
}

void Create_HASH::exit_f()
{
  std::ofstream fout("JSON.json");
  fout << mass_for_out.dump(4);
  fout.close();
}