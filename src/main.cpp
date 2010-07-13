#include "scribe_server.h"

using namespace apache::thrift::concurrency;

using namespace facebook::fb303;
using namespace facebook;

using namespace scribe::thrift;
using namespace std;

using boost::shared_ptr;

void print_usage(const char* program_name) {
  cout << "Usage: " << program_name << " [-p port] [-c config_file]" << endl;
}

int main(int argc, char **argv) {

  try {
    /* Increase number of fds */
    struct rlimit r_fd = {65535,65535};
    if (-1 == setrlimit(RLIMIT_NOFILE, &r_fd)) {
      LOG_OPER("setrlimit error (setting max fd size)");
    }

    int next_option;
    const char* const short_options = "hp:c:";
    const struct option long_options[] = {
      { "help",   0, NULL, 'h' },
      { "port",   0, NULL, 'p' },
      { "config", 0, NULL, 'c' },
      { NULL,     0, NULL, 'o' },
    };

    unsigned long int port = 0;  // this can also be specified in the conf file, which overrides the command line
    std::string config_file;
    while (0 < (next_option = getopt_long(argc, argv, short_options, long_options, NULL))) {
      switch (next_option) {
      default:
      case 'h':
        print_usage(argv[0]);
        exit(0);
      case 'c':
        config_file = optarg;
        break;
      case 'p':
        port = strtoul(optarg, NULL, 0);
        break;
      }
    }

    // assume a non-option arg is a config file name
    if (optind < argc && config_file.empty()) {
      config_file = argv[optind];
    }

    // seed random number generation with something reasonably unique
    srand(time(NULL) ^ getpid());

    g_Handler = shared_ptr<scribeHandler>(new scribeHandler(port, config_file));
    g_Handler->initialize();

    scribe::startServer(); // never returns

  } catch(const std::exception& e) {
    LOG_OPER("Exception in main: %s", e.what());
  }

  LOG_OPER("scribe server exiting");
  return 0;
}
