/*
 * NameChecker.h
 *
 *  Created on: 2011-12-9
 *      Author: yq
 */

#ifndef NAMECHECKER_H_
#define NAMECHECKER_H_

#include <string>
class Trie;

class CNameChecker
{
public:
	static bool Init(const char * szFileName);

	static void ReplaceString(std::string &strSource);

	static bool ReplaceStringSingle(std::string &strSource);

	static bool IsDirtyWord(std::string &strSource);

private:

	static Trie m_trie;
};

#endif /* NAMECHECKER_H_ */
