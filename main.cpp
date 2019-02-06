#include <future>
#include <iomanip>
#include <boost/program_options.hpp>
#include "core_native_miner.h"
#include "get_work_provider.h"
#include "get_block_template_provider.h"

using namespace boost::program_options;

template<class MinerT, class ProviderT>
class App : MinerCallback {
  MinerT miner;
  ProviderT provider;
  bool running = false;
  std::vector<std::future<void>> futures;

 public:
  App (std::string host) : provider ("http://" + host) {
    miner.setup (this);
  }

  void update_work_loop () {
    while (running) {
      std::this_thread::sleep_for (std::chrono::seconds (2));
      provider.assign_work (miner);
    }
  }

  void print_status_loop () {
    while (running) {
      std::this_thread::sleep_for (std::chrono::seconds (1));
      std::cout << "Last work update: " << std::put_time (std::localtime (&provider.last_work), "%c") << "   "
                << std::endl;
      std::cout << "Submissions: " << provider.submissions << " (last: "
                << std::put_time (std::localtime (&provider.last_submission), "%c") << ")   " << std::endl;
      std::cout << "Hashrate: " << (miner.current_hashrate () / 1000.0) << " kH/s   " << std::endl;
      //std::cout << "\033[3A";
    }
  }

  void run () {
    running = true;
    provider.assign_work (miner);
    miner.start ();
    futures.push_back (std::async (std::launch::async, &App::update_work_loop, this));
    futures.push_back (std::async (std::launch::async, &App::print_status_loop, this));
  }

  // Callbacks
  void submit_block (Uint8Array block) final {
    provider.submit_block (block);
  }
};

int main (int argc, const char *argv[]) {

  options_description desc{"Options"};
  desc.add_options ()
      ("help,h", "Help screen")
      ("miner", value<std::string> ()->default_value ("core"), "Miner to use")
      ("host", value<std::string> ()->default_value ("127.0.0.1:8648"), "IP address + port of remote host to use")
      ("method", value<std::string> ()->default_value ("work"), "Method to use to get data from the host")
      ("address", value<std::string> (), "Override mining address")
      ("extra-data", value<std::string> (), "Override extra-data");

  variables_map vm;
  store (parse_command_line (argc, argv, desc), vm);

  if (vm.count ("help")) {
    std::cout << desc << std::endl;
    return 0;
  }

  std::string miner = vm["miner"].as<std::string> ();
  std::string host = vm["host"].as<std::string> ();
  std::string method = vm["method"].as<std::string> ();

  if (vm.count ("address")) {
    // TODO
  }

  if (vm.count ("extra-data")) {
    // TODO
  }

  if (miner != "core") {
    std::cout << "Invalid miner: " << miner << std::endl;
    std::cout << "Supported miners: core" << std::endl;
    return 1;
  }

  if (method == "work") {
    App<CoreNativeMiner, GetWorkProvider> app (host);
    app.run ();
    return 0;
  } else if (method == "block-template") {
    App<CoreNativeMiner, GetBlockTemplateProvider> app (host);
    app.run ();
    return 0;
  } else {
    std::cout << "Invalid method: " << method << std::endl;
    std::cout << "Supported methods: work, block-template" << std::endl;
    return 2;
  }
}