/*
  ==============================================================================

    TextUtils.cpp
    Created: 11 Oct 2017 8:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <regex>

#include "TextUtils.h"

using namespace Miam;

std::string TextUtils::FindFilenameInCommandLineArguments(std::string commandLineToParse)
{
    
    std::string commandLineFileName;
    
    // Regex qui permet de récupérer le nom de fichier
    std::regex word_regex( "(\"[^\"]+\"|[^\\s\"]+)" );
    auto words_begin =
    std::sregex_iterator(commandLineToParse.begin(), commandLineToParse.end(), word_regex);
    auto words_end = std::sregex_iterator();
    bool nextWordIsSession = false;
    bool sessionPathFound = false;
    for (std::sregex_iterator i = words_begin; i != words_end && !sessionPathFound; ++i)
    {
        std::smatch match = *i;
        std::string match_str = match.str();
        if (match_str == "-session")
        {
            nextWordIsSession = true;
        }
        else if(nextWordIsSession)
        {
            commandLineFileName = match_str;
            sessionPathFound = true;
        }
    }
    // Suppression des guillemets si nécessaire
    auto firstCharIter = commandLineFileName.begin();
	// Attention à ne pas déréférencer le .end() ...
	if (firstCharIter != commandLineFileName.end())
	{
		// Tentative de suppression du premier "
		if (*firstCharIter == '\"')
			commandLineFileName.erase(firstCharIter);
		// tentative de suppression du " de fin
		if (commandLineFileName.size() > 0)
		{
			auto lastActualCharIter = std::prev(commandLineFileName.end());
			if (*lastActualCharIter == '\"')
				commandLineFileName.erase(lastActualCharIter);
		}
	}
    // retour par copie
    return commandLineFileName;
}

void Miam::TextUtils::intToU16string(int I, std::u16string stringU16[])
{
	std::string stringASCII = std::to_string(I);
	for (int i = 0; i < stringASCII.length(); ++i)
		stringU16[0].push_back(char16_t(stringASCII[i]));
	return;
}


