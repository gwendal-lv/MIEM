/*
  ==============================================================================

    TextUtils.cpp
    Created: 11 Oct 2017 8:36:32pm
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#include <regex>

#include <sstream> // stringstream
#include <iomanip> // setprecision

#include "TextUtils.h"

#include "MiamExceptions.h"

using namespace Miam;

std::string TextUtils::oscIntInAddressTag = std::string(Miam_OscIntInAddressTag);
std::string TextUtils::oscFloatInAddressTag = std::string(Miam_OscFloatInAddressTag);



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



OSCMessage TextUtils::ParseStringToJuceOscMessage(const std::string& stringToParse)
{
    //std::cout << "nom à parser : '" << stringToParse << "'" <<  std::endl;
    std::string remainingString = stringToParse;
    
    // D'abord on recherche l'adresse OSC ; en tolérant des espaces au début... on les supprime :
    size_t localCursor = 0;
    while (remainingString[localCursor] != '/')
    {
        if( remainingString[localCursor] != ' ' )
            throw ParseException("The string to be parsed needs to start by the OSC address, beginning by a '/'");
        
        localCursor++;
    }
    if (localCursor == remainingString.length()) // Si on est à la fin, erreur...
        throw ParseException("Symbol '/' (beginning of the OSC address) not found.");
    
    remainingString = remainingString.substr(localCursor, remainingString.length() - localCursor);
    localCursor = 0;
    //std::cout << "restant = '" << remainingString << "' ";
    
    // Avec le '/', l'adresse prend jusqu'à l'espace ou tabulation suivant (non-inclus)
    size_t spaceIndex = remainingString.find(' ');
    if (spaceIndex == std::string::npos)
        spaceIndex = remainingString.length();
    size_t tabIndex = remainingString.find('\t');
    if (tabIndex == std::string::npos)
        tabIndex = remainingString.length();
    localCursor = std::min<size_t>(spaceIndex, tabIndex);
    std::string oscAddress = remainingString.substr(0, localCursor);
    //std::cout << "adresse OSC = '" << oscAddress << "' ";
    remainingString = remainingString.substr(localCursor, remainingString.length() - localCursor);
    localCursor = 0;
    //std::cout << "restant = '" << remainingString << "' ";
    
    // On construit le message seulement si l'adresse est valide
    try {
        OSCAddressPattern(oscAddress.c_str()); // pour tester : exception lancée ?
    } catch (OSCFormatError& e) {
        throw ParseException(std::string("Cannot extract a valid OSC address: ") + e.what());
    }
    OSCMessage returnMessage(OSCAddressPattern(oscAddress.c_str())); // duplication du calcul de parse, mais bon...
    
    // Ensuite, on passe dans la boucle de recherche des coeffs,
    // basée sur l'espace sur lequel on se trouve...
    bool lookingForFloatValue = false; // devient vrai si on est tombé sur un type ([int], [float])
    bool lookingForIntValue = false;
    while (localCursor < remainingString.length())
    {
        // Si c'est un espace : on passe à lettre suivante (on coupe à la lettre suivante)
        if ((remainingString[localCursor] == ' ') || (remainingString[localCursor] == '\t'))
        {
            remainingString = remainingString.substr(localCursor+1,remainingString.length()-localCursor-1);
            localCursor = 0;
        }
        
        // Sinon : on rentre dans l'analyse
        else
        {
            //std::cout << "restant = '" << remainingString << "' ";
            
            // on peut chercher une valeur numérique si on l'attendait
            if (lookingForFloatValue || lookingForIntValue)
            {
                // Recherche de la pos du prochain espacement
                spaceIndex = remainingString.find(' ');
                if (spaceIndex == std::string::npos)
                    spaceIndex = remainingString.length();
                tabIndex = remainingString.find('\t');
                if (tabIndex == std::string::npos)
                    tabIndex = remainingString.length();
                localCursor = std::min<size_t>(spaceIndex, tabIndex);
                std::string valueToParse = remainingString.substr(0, localCursor);
                // Parse selon le type souhaité +  ajout du paramètre dans le msg OSC
                if (lookingForIntValue)
                {
                    int parsedValue;
                    try {
                        parsedValue = std::stoi(valueToParse);
                        //std::cout << "parsé à " << parsedValue << " ";
                    } catch (std::exception& e) { // std::invalid_argument ou std::out_of_range
                        throw ParseException(std::string("Cannot parse the value to int ") + e.what());
                    }
                    returnMessage.addInt32(parsedValue);
                    lookingForIntValue = false;
                }
                else if (lookingForFloatValue)
                {
                    float parsedValue;
                    try {
                        parsedValue = std::stof(valueToParse);
                        //std::cout << "parsé à " << parsedValue << " ";
                    } catch (std::exception& e) { // std::invalid_argument ou std::out_of_range
                        throw ParseException(std::string("Cannot parse the value to float ") + e.what());
                    }
                    returnMessage.addFloat32(parsedValue);
                    lookingForFloatValue = false;
                }
                // On supprime tout de suite ce qu'on vient de lire et traiter
                remainingString = remainingString.substr(localCursor, remainingString.length() - localCursor);
                localCursor = 0;
                //std::cout << "restant = '" << remainingString << "' ";
                
            }
            // ou bien on peut chercher ici [int] ou [float]
            else if (remainingString.substr(0,oscIntInAddressTag.length()) == oscIntInAddressTag)
            {
                //std::cout << "tag INT trouvé ";
                lookingForIntValue = true;
                localCursor += oscIntInAddressTag.length();
                remainingString = remainingString.substr(localCursor, remainingString.length()-localCursor);
                localCursor = 0;
            }
            else if (remainingString.substr(0,oscFloatInAddressTag.length()) == oscFloatInAddressTag)
            {
                //std::cout << "tag FLOAT trouvé ";
                lookingForFloatValue = true;
                localCursor += oscFloatInAddressTag.length();
                remainingString = remainingString.substr(localCursor, remainingString.length()-localCursor);
                localCursor = 0;
            }
            // Sinon il y a une erreur, on ne reconnaît pas la suite...
            else
                throw ParseException(std::string("Unexpected symbol: cannot find a ") + Miam_OscIntInAddressTag + " or " + Miam_OscFloatInAddressTag + " at the expected position in string");
        }
    }
    // On peut sortir de la boucle alors qu'un valeur d'argument manque... Pas grave, on oublie...
    
    
    //std::cout << std::endl;
    return returnMessage;
}



