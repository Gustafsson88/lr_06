// Copyright 2021 Alexander Guchkov <firer.a45@gmail.com>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <iostream>
#include <vector>
#include <csignal>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <picosha2.h>
#include <boost/log/trivial.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/utility/setup.hpp>
#include <nlohmann/json.hpp>

class Create_HASH
{
 public:
  Create_HASH();
  ~Create_HASH();
  static void logging();
  static void create_hash(std::mutex &mtx);
  static void exit_f();
  static void exit_signal_catcher(int sig);
};
#endif // INCLUDE_HEADER_HPP_
