#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <fstream>
#include "ScribeServer.h"

using namespace scribe;

void startScribe(void){

	unsigned long int port=1463;
	std::string config_file="/usr/local/scribe.conf";
    g_handler = boost::shared_ptr<ScribeHandler>(new ScribeHandler(port, config_file));
    g_handler->initialize();
}

int main (int argc, char* argv[])
{
	startScribe();

    // informs test-listener about testresults
    CPPUNIT_NS :: TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS :: TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS :: BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS :: TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS :: TestFactoryRegistry :: getRegistry ().makeTest ());
    testrunner.run (testresult);

    // Log Test Results in a log file
    std::fstream outFile;
    outFile.open("/tmp/results.txt",std::ios::out);
    CPPUNIT_NS :: CompilerOutputter compileroutputter (&collectedresults, outFile);
    compileroutputter.printFailureReport();
    compileroutputter.write ();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful () ? 0 : 1;
}
