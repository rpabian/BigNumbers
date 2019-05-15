#pragma once
//BigNumberslib.h
#include <vector>
#include <string>


namespace BigNumbers
{
	class BigNumbers
	{
	private:
			std::vector<int> BigNumber;
			const int MAX_SIZE = 1000;
			
			//private functions that work with the operator overloads
			BigNumbers(std::vector<int> numVector);
			std::vector<int> subtractFunc(std::vector<int>& biggerNum, std::vector<int>& smallerNum);
			std::vector<int> addFunc(std::vector<int>& biggerVec, std::vector<int>& smallerVec);
			std::vector<int> multiplyFunc(std::vector<int>& biggerVec, std::vector<int>& smallerVec);
			std::vector<int> divisionFunc(std::vector<int>& dividendVec, std::vector<int>& divisorVec);
			int repeatSubtract(std::vector<int>& remainderVec, std::vector<int>& divisorVec);
			std::vector<int> modulusFunc(std::vector<int>& dividendVec, std::vector<int>& divisorVec);

	public:
			BigNumbers(std::string number);
			std::string bigNumberToString();
			BigNumbers operator=(BigNumbers& secondNum);
			BigNumbers operator+(BigNumbers& secondNum);
			BigNumbers operator-(BigNumbers& secondNum);
			BigNumbers operator*(BigNumbers& secondNum);
			BigNumbers operator/(BigNumbers& secondNum);
			BigNumbers operator%(BigNumbers& secondNum);
		
	};
}