//STL
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <functional>
#include <cmath>
#include <exception>

//This program initialize n vectors of arbitrary size k (given in parameter)
//and then used order log2(n) steps to compute a vector of size k that contains
//in each of its entry, the sum of all other vectors entries at the same position

//This is not the most clever thing we can do with std::thread, openmp is probably
//more suited to do so, but it is at least a valid example.

//compile using g++ ./test.cpp -pthread -std=c++11 -o ./test

void reduceVector( std::vector<std::vector<int> >& v, int tid, int maxTid )
{
	if( tid + maxTid <  v.size() )
	{
		std::transform( v.at(tid).begin(), v.at(tid).end(), v.at(tid+maxTid).begin(), v.at(tid).begin(), std::plus<int>() );
	}
}

int giveReductionSize( int in )
{
	//If number is a power of two, then returns the number divided by 2
	//Else give the next power of two fthat follows the number divided by two

	if( in > 0 )
	{
		return std::pow(2, std::ceil(std::log2(in/2.0)) );
	}else
	{
		return 0;
	}
}

int main( int argc, char* argv[] )
{
	if( (argc < 2) )
	{
		std::cout << "please provide size of vector" << std::endl;
		return 0;
	}
	//Size parameters
	int sizeVector = 10;
	int nbVector = std::stoi( argv[1] );

	//Threads
	std::vector<std::thread> t;

	//Actual data
	std::vector<std::vector<int> > vVector;
	vVector.resize( nbVector, std::vector<int>( sizeVector, 1 ) );

	int maxTid = giveReductionSize(nbVector);
	std::cout << "maxTid is " << maxTid << std::endl;
	try
	{
		while( maxTid != 0 )
		{
			//Launch a group of threads
			for (int i = 0; i < maxTid; ++i)
			{
				t.emplace_back( reduceVector, std::ref(vVector), i, maxTid );
			}

			//Join the threads with the main thread
			for( auto it = t.begin(); it != t.end(); it++ )
			{
				it->join();
			}

			//Reset all threads
			t.clear();

			//Half the work is remaining
			maxTid /= 2;
		}

		//Check if the sum is correct
		for(int i = 0; i < sizeVector; ++i)
		{
			std::cout << " The sum is " << vVector.at(0).at(i) << " (should be " << nbVector << " )" << std::endl;
		}
	}catch( std::exception& e )
	{
		std::cout << "There was an error :" << e.what() << std::endl;
	}


	return EXIT_SUCCESS;
}
