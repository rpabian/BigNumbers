#include "stdafx.h"
#include "BigNumbersLib.h"
#include <string>
#include <iostream>
#include <stdexcept>

namespace BigNumbers
{
		std::vector<int> BigNumber;
		const int MAX_SIZE = 1000;


		//constructor that makes a BigNumber from results of operator overload functions
		BigNumbers::BigNumbers(std::vector<int> numVector)
		{
			if (numVector.size() > MAX_SIZE)
				throw "Overflow";
			BigNumber = numVector;
		}

		//private subtract function for subtract operator overload public func
		std::vector<int> BigNumbers::subtractFunc(std::vector<int>& biggerNum, std::vector<int>& smallerNum)
		{
			//difference vector
			std::vector<int> resultVector(biggerNum.size(), 0);
			std::vector<int>::reverse_iterator diffIter = resultVector.rbegin();

			//iterators that go through both of the vectors that will be subtracted
			std::vector<int>::reverse_iterator biggerIter = biggerNum.rbegin();
			std::vector<int>::reverse_iterator smallerIter = smallerNum.rbegin();

			//subtracts both vectors using smaller vector as bound for the for loop
			for (smallerIter; smallerIter != smallerNum.rend(); smallerIter++, biggerIter++, diffIter++)
			{
				//checks if borrow is necessary for subtraction
				if (*biggerIter < *smallerIter)
				{
					//for loop to check next possible digit to borrow from
					std::vector<int>::reverse_iterator tempIter = biggerIter;
					for (++tempIter; *tempIter == 0; tempIter++)
						*tempIter += 9;
					*tempIter -= 1;
					*biggerIter += 10;
				}

				//subtraction
				int difference = *biggerIter - *smallerIter;

				*diffIter += difference;
			}

			//adds in remainding digits from bigger vector if necessary
			for (biggerIter; biggerIter != biggerNum.rend(); biggerIter++, diffIter++)
			{
				*diffIter += *biggerIter;
			}

			//cleans up leading zeros
			if (*resultVector.begin() == 0)
			{
				//creates two points from which to erase leading zeros
				std::vector<int>::iterator tempEndIter = resultVector.begin();
				std::vector<int>::iterator tempBeginIter = resultVector.begin();

				//goes through all leading zeros, but also makes sure that you dont delete and answer of zero
				while (*tempEndIter == 0 && tempEndIter != resultVector.end() - 1)
					tempEndIter++;

				resultVector.erase(tempBeginIter, tempEndIter);
			}

			return resultVector;
		}

		//private vector addition function that helps with the private multiply function
		std::vector<int> BigNumbers::addFunc(std::vector<int>& biggerVec, std::vector<int>& smallerVec)
		{
			//checks vector size and makes both vectors the same size with leading zeros if necessary
			while (biggerVec.size() > smallerVec.size())
			{
				std::vector<int>::iterator startPoint = smallerVec.begin();
				smallerVec.insert(startPoint, 0);
			}

			//reverse iterators for both vectors
			std::vector<int>::reverse_iterator itOne = biggerVec.rbegin();
			std::vector<int>::reverse_iterator itTwo = smallerVec.rbegin();

			//result vector filled with zeros and iterator
			std::vector<int> resultVector(biggerVec.size(), 0);
			std::vector<int>::reverse_iterator itResult = resultVector.rbegin();

			bool carry = false;
			for (itOne; itOne != biggerVec.rend(); itOne++, itTwo++, itResult++)
			{
				//addition of two vectors
				int sum = *itOne + *itTwo;

				//applies carry if necessary
				if (carry)
					sum++;

				//checks if there is carry for the next digits
				if (sum > 9)
				{
					sum -= 10;
					carry = true;
				}
				else
					carry = false;

				//adds result sum to result vector
				*itResult += sum;
			}

			//applies any carry from the last addition
			if (carry)
				resultVector.insert(resultVector.begin(), 1);

			return resultVector;
		}

		//private multiplication function for operator overload function
		std::vector<int> BigNumbers::multiplyFunc(std::vector<int>& biggerVec, std::vector<int>& smallerVec)
		{
			//vector that holds current and ultimate result of multiplication
			std::vector<int> resultVector(biggerVec.size(), 0);

			//vector reverse iterator for smaller vector
			std::vector<int>::reverse_iterator smallerIter = smallerVec.rbegin();

			//multiplication for loops
			int leadingZeros = 0;
			for (smallerIter; smallerIter != smallerVec.rend(); smallerIter++)
			{
				//temp vector of immediate multiplication and its reverse iterator
				std::vector<int> tempVector(biggerVec.size(), 0);
				std::vector<int>::reverse_iterator tempIter = tempVector.rbegin();

				//carry from single digit multiplication
				int carry = 0;

				std::vector<int>::reverse_iterator biggerIter = biggerVec.rbegin();
				for (biggerIter; biggerIter != biggerVec.rend(); biggerIter++, tempIter++)
				{
					//value of "top" vector stored in temp int variable to avoid changes
					int topNumber = *biggerIter;

					//actual multiplication and application of carry
					int product = *smallerIter * topNumber;
					product += carry;

					//check for carry
					if (product > 9)
					{
						carry = product / 10;
						product = product % 10;
					}
					else
						carry = 0;

					*tempIter += product;
				}

				//apply any leftover carry
				if (carry > 0)
					tempVector.insert(tempVector.begin(), carry);

				//apply leading zeros
				for (int i = 0; i < leadingZeros; i++)
					tempVector.push_back(0);

				//increment leading zeros counter
				leadingZeros++;

				//add this iteration's mulitplication result to total
				resultVector = addFunc(tempVector, resultVector);
			}

			return resultVector;
		}

		//private division function for operator overload function
		std::vector<int> BigNumbers::divisionFunc(std::vector<int>& dividendVec, std::vector<int>& divisorVec)
		{
			//holds current and overall remainder
			std::vector<int> remainderVec;

			//holds quotient value
			std::vector<int> quotientVec;

			//loop through the dividends numbers as we perform long division
			std::vector<int>::iterator dividendIter = dividendVec.begin();

			int quotient = 0;
			for (dividendIter; dividendIter != dividendVec.end(); dividendIter++)
			{
				//adds next dividend digit to remainder
				remainderVec.push_back(*dividendIter);

				//checks if remainder is bigger than divisor

				//if smaller, quotient is zero
				if (remainderVec.size() < divisorVec.size())
					quotient = 0;

				// if same size, checks which value is bigger
				else if (remainderVec.size() == divisorVec.size())
				{
					std::vector<int>::iterator iterOne = remainderVec.begin();
					std::vector<int>::iterator iterTwo = divisorVec.begin();

					bool remainderBigger = true;

					for (iterOne; iterOne != remainderVec.end(); iterOne++, iterTwo++)
					{
						if (*iterOne < *iterTwo)
						{
							remainderBigger = false;
							break;
						}
						else if (*iterOne > *iterTwo)
							break;
						else
							continue;
					}

					//if remainder is smaller return 0, otherwise subtract
					if (remainderBigger)
						quotient = repeatSubtract(remainderVec, divisorVec);
					else
						quotient = 0;
				}

				//if remainder is bigger subtract
				else
					quotient = repeatSubtract(remainderVec, divisorVec);

				//pushes quotient digit to quotient vector
				quotientVec.push_back(quotient);
			}

			//cleans up leading zeros
			if (*quotientVec.begin() == 0)
			{
				//creates two points from which to erase leading zeros
				std::vector<int>::iterator tempEndIter = quotientVec.begin();
				std::vector<int>::iterator tempBeginIter = quotientVec.begin();

				//goes through all zeros, but also makes sure you dont delete an answer of zero
				while (*tempEndIter == 0 && tempEndIter != quotientVec.end() - 1)
					tempEndIter++;

				quotientVec.erase(tempBeginIter, tempEndIter);
			}

			//returns result of integer division
			return quotientVec;
		}

		//private helper function for division and modulus operator overload function (remainderVec's values should change here)
		int BigNumbers::repeatSubtract(std::vector<int>& remainderVec, std::vector<int>& divisorVec)
		{
			//holds quotien variable (amount of subtractions)
			int count = 0;

			//keep subtracting while the remainder is bigger than divisor
			bool remainderBigger = true;
			while (remainderBigger)
			{
				remainderVec = subtractFunc(remainderVec, divisorVec);
				count++;

				//checks if remainder is still bigger than divisor
				if (remainderVec.size() < divisorVec.size())
					remainderBigger = false;

				// if same size, checks which value is bigger
				else if (remainderVec.size() == divisorVec.size())
				{
					std::vector<int>::iterator iterOne = remainderVec.begin();
					std::vector<int>::iterator iterTwo = divisorVec.begin();

					for (iterOne; iterOne != remainderVec.end(); iterOne++, iterTwo++)
					{
						if (*iterOne < *iterTwo)
						{
							remainderBigger = false;
							break;
						}
						else if (*iterOne > *iterTwo)
							break;
						else
							continue;
					}
				}

				//if it is bigger continue the subtract loop
				else
					remainderBigger = true;
			}

			//return quotient digit (number of subtractions)
			return count;
		}

		//private function for the modulus operator overload function
		std::vector<int> BigNumbers::modulusFunc(std::vector<int>& dividendVec, std::vector<int>& divisorVec)
		{
			//holds current and overall remainder
			std::vector<int> remainderVec;

			//holds quotient value (still need this to subtract from remainder)
			std::vector<int> quotientVec;

			//loop through the dividends numbers as we perform long division
			std::vector<int>::iterator dividendIter = dividendVec.begin();

			int quotient = 0;
			for (dividendIter; dividendIter != dividendVec.end(); dividendIter++)
			{
				//adds next dividend digit to remainder
				remainderVec.push_back(*dividendIter);

				//checks if remainder is bigger than divisor
				//if smaller, quotient is zero and remainder doesn't change
				if (remainderVec.size() < divisorVec.size())
					quotient = 0;

				// if same size, checks which value is bigger
				else if (remainderVec.size() == divisorVec.size())
				{
					std::vector<int>::iterator iterOne = remainderVec.begin();
					std::vector<int>::iterator iterTwo = divisorVec.begin();

					bool remainderBigger = true;

					for (iterOne; iterOne != remainderVec.end(); iterOne++, iterTwo++)
					{
						if (*iterOne < *iterTwo)
						{
							remainderBigger = false;
							break;
						}
						else if (*iterOne > *iterTwo)
							break;
						else
							continue;
					}

					//if remainder is smaller return 0, otherwise subtract
					if (remainderBigger)
						quotient = repeatSubtract(remainderVec, divisorVec);
					else
						quotient = 0;
				}

				//if remainder is bigger subtract
				else
					quotient = repeatSubtract(remainderVec, divisorVec);

				//pushes quotient digit to quotient vector
				quotientVec.push_back(quotient);
			}

			//cleans up leading zeros from any subtractions
			if (*remainderVec.begin() == 0)
			{
				//creates two points from which to erase leading zeros
				std::vector<int>::iterator tempEndIter = remainderVec.begin();
				std::vector<int>::iterator tempBeginIter = remainderVec.begin();

				//goes through all zeros, but also makes sure you dont delete an answer of zero
				while (*tempEndIter == 0 && tempEndIter != remainderVec.end() - 1)
					tempEndIter++;

				remainderVec.erase(tempBeginIter, tempEndIter);
			}

			//returns the final value of remainderVec
			return remainderVec;
		}


		//Constructor takes in string and makes it into a vector of single digit ints
		BigNumbers::BigNumbers(std::string number)
		{
			//checks that bigNumber does not exceed 1000 digits
			int numSize = number.length();
			if (numSize > MAX_SIZE)
				throw "Number is too large.";

			//stores characters from strings as ints into the vector
			for (char& c : number)
			{
				int charToInt = c - '0';
				if (charToInt > 9 || charToInt < 0)
					throw "Not a valid number";
				BigNumber.push_back(charToInt);
			}

		}

		std::string BigNumbers::bigNumberToString()
		{
			std::string returnString;

			std::vector<int>::iterator iter;
			for (iter = BigNumber.begin(); iter != BigNumber.end(); iter++)
				returnString = returnString + std::to_string(*iter);

			return returnString;
		}

		//= operator overload
		BigNumbers BigNumbers::operator=(BigNumbers& secondNum)
		{
			BigNumber = secondNum.BigNumber;
			return *this;
		}

		//+ operator overload
		BigNumbers BigNumbers::operator+(BigNumbers& secondNum)
		{
			std::vector<int> resultVector;

			//checks which vector is bigger and calls private add function based on that info
			if (BigNumber.size() > secondNum.BigNumber.size())
				resultVector = addFunc(BigNumber, secondNum.BigNumber);
			else
				resultVector = addFunc(secondNum.BigNumber, BigNumber);

			//return resultant BigNumber
			BigNumbers tempBigNum(resultVector);
			return tempBigNum;
		}


		BigNumbers BigNumbers::operator-(BigNumbers& secondNum)
		{
			std::vector<int> resultVector;

			if (secondNum.BigNumber.size() < BigNumber.size())
				resultVector = subtractFunc(BigNumber, secondNum.BigNumber);

			//checks which vector value is bigger when vector sizes are the same
			else if (BigNumber.size() == secondNum.BigNumber.size())
			{
				std::vector<int>::iterator iterOne = BigNumber.begin();
				std::vector<int>::iterator iterTwo = secondNum.BigNumber.begin();

				bool firstBigger = true;

				for (iterOne; iterOne != BigNumber.end(); iterOne++, iterTwo++)
				{
					if (*iterOne < *iterTwo)
					{
						firstBigger = false;
						break;
					}
					else if (*iterOne > *iterTwo)
						break;
					else
						continue;
				}

				//calls subtract func based on which is vector is bigger
				if (firstBigger)
					resultVector = subtractFunc(BigNumber, secondNum.BigNumber);
				else
					resultVector = subtractFunc(secondNum.BigNumber, BigNumber);
			}

			else
				resultVector = subtractFunc(secondNum.BigNumber, BigNumber);

			BigNumbers tempBigNum(resultVector);
			return tempBigNum;

		}

		BigNumbers BigNumbers::operator*(BigNumbers& secondNum)
		{
			std::vector<int> resultVector;

			//check which vector is bigger, uses private function using correct order
			if (BigNumber.size() >= secondNum.BigNumber.size())
				resultVector = multiplyFunc(BigNumber, secondNum.BigNumber);
			else
				resultVector = multiplyFunc(secondNum.BigNumber, BigNumber);

			BigNumbers tempBigNum(resultVector);
			return tempBigNum;
		}

		BigNumbers BigNumbers::operator/(BigNumbers& secondNum)
		{
			std::vector<int> resultVector;

			//check if dividend is smaller than divisor
			//if smaller return 0 vector
			if (BigNumber.size() < secondNum.BigNumber.size())
				resultVector.push_back(0);

			//if same size check which has greater value
			else if (BigNumber.size() == secondNum.BigNumber.size())
			{
				std::vector<int>::iterator iterOne = BigNumber.begin();
				std::vector<int>::iterator iterTwo = secondNum.BigNumber.begin();

				bool firstBigger = true;

				for (iterOne; iterOne != BigNumber.end(); iterOne++, iterTwo++)
				{
					if (*iterOne < *iterTwo)
					{
						firstBigger = false;
						break;
					}
					else if (*iterOne > *iterTwo)
						break;
					else
						continue;
				}

				//calls division func based on whether dividend vector is bigger in value
				if (firstBigger)
					resultVector = divisionFunc(BigNumber, secondNum.BigNumber);
				else
					resultVector.push_back(0);
			}

			//if bigger call division function
			else
				resultVector = divisionFunc(BigNumber, secondNum.BigNumber);

			BigNumbers resultBigNum(resultVector);
			return resultBigNum;
		}

		BigNumbers BigNumbers::operator%(BigNumbers& secondNum)
		{
			std::vector<int> resultVector;

			//check if dividend is smaller than divisor

			//if smaller return dividend (this) vector
			if (BigNumber.size() < secondNum.BigNumber.size())
				resultVector = BigNumber;

			//if same size check which has greater value
			else if (BigNumber.size() == secondNum.BigNumber.size())
			{
				std::vector<int>::iterator iterOne = BigNumber.begin();
				std::vector<int>::iterator iterTwo = secondNum.BigNumber.begin();

				bool firstBigger = true;

				for (iterOne; iterOne != BigNumber.end(); iterOne++, iterTwo++)
				{
					if (*iterOne < *iterTwo)
					{
						firstBigger = false;
						break;
					}
					else if (*iterOne > *iterTwo)
						break;
					else
						continue;
				}

				//calls modulus func based on whether dividend vector is bigger in value
				if (firstBigger)
					resultVector = modulusFunc(BigNumber, secondNum.BigNumber);
				else
					resultVector = BigNumber;
			}

			//if bigger call modulus function
			else
				resultVector = modulusFunc(BigNumber, secondNum.BigNumber);

			BigNumbers resultBigNum(resultVector);
			return resultBigNum;
		}
}

