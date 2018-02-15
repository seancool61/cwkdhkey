#include "dhkey.h"
#include <chrono>
#include <thread>

#include "boost/random.hpp"
#include "boost/generator_iterator.hpp"
#include <boost/multiprecision/cpp_int.hpp>

void temp01();
bool getCommandLineParameters(int argc , char **argv, boost::program_options::variables_map &);
bool getParamValues(boost::program_options::variables_map, std::string&, std::string&);
void printParam(boost::program_options::variables_map);

int main(int argc , char **argv)
{
	boost::program_options::variables_map vm;
	std::string base;
	std::string modulus;
	boost::multiprecision::cpp_int A_AlicesPublicKey=0;
	boost::multiprecision::cpp_int B_BobsPublicKey=0;
	boost::multiprecision::cpp_int a_AlicesPrivateKey = 0;
	boost::multiprecision::cpp_int b_BobsPrivateKey = 0;
	
	typedef boost::mt19937 RNGType;
	RNGType rng;
	boost::uniform_int<> one_to_six( 5, 19 );
	boost::variate_generator< RNGType, boost::uniform_int<> >
		dice(rng, one_to_six);
	a_AlicesPrivateKey = dice();
	b_BobsPrivateKey = dice();
	
	std::cout << "dhkey!" << std::endl;
	temp01();
	getCommandLineParameters(argc, argv, vm);
	if (EXIT_FAILURE == getParamValues(vm, base, modulus))  {
		return EXIT_FAILURE;
	}
	printParam(vm);
	
	std::cout << ".....Private keys" << std::endl;
	std::cout << ".......Alice's=" << a_AlicesPrivateKey << std::endl;
	std::cout << ".......Bob's  =" << b_BobsPrivateKey << std::endl;
	
	boost::multiprecision::cpp_int ipow=a_AlicesPrivateKey;
	boost::multiprecision::cpp_int ibase(base.c_str());
	boost::multiprecision::cpp_int imodulus(modulus.c_str());
	std::cout << ".....3 ibase=" << ibase << " ipow=" << ipow << " imodulus=" << imodulus << std::endl;
	//boost::multiprecision::cpp_int iresult = powm(ibase, ipow, imodulus);
	A_AlicesPublicKey = powm(ibase, ipow, imodulus);

	ipow=b_BobsPrivateKey;
	std::cout << ".....4 ibase=" << ibase << " ipow=" << ipow << " imodulus=" << imodulus << std::endl;
	//boost::multiprecision::cpp_int iresult = powm(ibase, ipow, imodulus);
	B_BobsPublicKey = powm(ibase, ipow, imodulus);
	std::cout << ".....Public keys" << std::endl;
	std::cout << ".......Alice's (g^a mod p)=" << A_AlicesPublicKey << std::endl;
	std::cout << ".......Bob's   (g^b mod p)=" << B_BobsPublicKey << std::endl;

	boost::multiprecision::cpp_int A_verification=0;
	boost::multiprecision::cpp_int B_verification=0;
	A_verification = powm(A_AlicesPublicKey, b_BobsPrivateKey, imodulus);
	B_verification = powm(B_BobsPublicKey,   a_AlicesPrivateKey, imodulus);

	std::cout << ".....Verification" << std::endl;
	std::cout << ".......Alice's verification (g^a mod p)^b mod p=" << A_verification << std::endl;
	std::cout << ".......Bob's verification   (g^b mod p)^a mod p=" << B_verification << std::endl;
}
bool getParamValues(boost::program_options::variables_map vm, std::string &base, std::string &modulus)  {
	if (vm.count("base")) {
		base = vm["base"].as< std::vector<std::string> >()[0];
		std::cout << ".....base=" << base << std::endl;
	} else {
		std::cout << ".....base omitted." << std::endl;
		return EXIT_FAILURE;
	}
	if (vm.count("modulus")) {
		modulus = vm["modulus"].as< std::vector<std::string> >()[0];
		std::cout << ".....modulus=" << modulus << std::endl;
	} else {
		std::cout << ".....modulus omitted." << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
bool getCommandLineParameters(int argc , char **argv, boost::program_options::variables_map &vm)  {
	//https://chuckaknight.wordpress.com/2013/03/24/boost-command-line-argument-processing/
	boost::program_options::options_description desc(
		"An example command using Boost command line"
		"arguments.\n\nAllowed arguments");
	desc.add_options()
		("help,h", "Produce this help message.")
		("memory-report,e", "Print a memory usage report to the log at termination.")
		("restart,r","Restart the application.")
		("base,b", boost::program_options::value< std::vector<std::string> >(), "Public (prime) base, known to everyone; e.g. g=5.")
		("modulus,m", boost::program_options::value< std::vector<std::string> >(), "Public (prime) modulus, known to everyone; e.g. p=23.")
		("output-file,o", boost::program_options::value< std::vector<std::string> >(), "Specifies output file.")
		("input-file,i", boost::program_options::value< std::vector<std::string> >(), "Specifies input file.");
		boost::program_options::positional_options_description p;
		p.add("input-file", -1);
		// Parse the command line catching and displaying any 
		// parser errors
	try
	{
		store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		notify(vm);
	} catch (std::exception &e)  {
		std::cout << std::endl << e.what() << std::endl;
		std::cout << desc << std::endl; 
		}
	// Display help text when requested
	if (vm.count("help")) {
		std::cout << "–help specified" << std::endl;
		std::cout << desc << std::endl;
		}
	// Display the state of the arguments supplied
	if (vm.count("memory-report")) {
		std::cout << "-memory-report specified" << std::endl;
		}
	if (vm.count("restart")) {
		std::cout << "-restart specified" << std::endl;
		}
	if (vm.count("template")) {
		std::cout << "-template specified" << std::endl;
	}
	if (vm.count("validate")) {
		std::cout << "-validate specified" << std::endl;
	}
	if (vm.count("base")) {
		std::string base = vm["base"].as< std::vector<std::string> >()[0];
		//std::vector<std::string> base = vm["base"].as< std::vector<std::string> >();
		std::cout << "-base specified with value = "<< base << std::endl;
	}
	if (vm.count("modulus")) {
		std::string modulus = vm["modulus"].as< std::vector<std::string> >()[0];
		//std::vector<std::string> modulus = vm["modulus"].as< std::vector<std::string> >();
		std::cout << "-modulus specified with value = "<< modulus << std::endl;
	}
	if (vm.count("output-file")) {
		std::vector<std::string> outputFilename = vm["output-file"].as< std::vector<std::string> >();
		std::cout << "-output-file specified with value = "<< outputFilename[0] << std::endl;
	}
	if (vm.count("input-file")) {
		std::vector<std::string> inputFilename =
		vm["input-file"].as< std::vector<std::string> >();
		std::cout << "-input-file specified with value = " << inputFilename[0] << std::endl;
	}
	return EXIT_SUCCESS;
}

void printParam(boost::program_options::variables_map vm)  {
	std::cout << ".....parameters:" << std::endl;
	for (const auto& it : vm) {
		std::cout << "......." << it.first.c_str() << " ";
		auto& value = it.second.value();
		if (auto v = boost::any_cast<uint32_t>(&value))
			std::cout << *v;
		else if (auto v = boost::any_cast<std::string>(&value))
			std::cout << *v;
		else
			std::cout << "error:";
		std::cout << std::endl;
	}
}
void temp01()  {
	std::string strPower="8912627233012800753578052027888001981";
	std::string strModulus="0x86f71688cdd2612c117d1f54bdae029";

	boost::multiprecision::cpp_int pow(strPower);
	boost::multiprecision::cpp_int mod(strModulus);
	boost::multiprecision::cpp_int base("12345");

	boost::multiprecision::cpp_int result = powm(base, pow, mod);

	std::cout << "result is: " << result << std::endl;
}