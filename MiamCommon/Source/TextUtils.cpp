/* 
 * This file is part of the MIEM distribution (https://github.com/gwendal-le-vaillant/MIEM).
 * Copyright (c) 2019 Gwendal Le Vaillant.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <regex>

#include <string>
#include <sstream> // stringstream
#include <iomanip> // setprecision

#include "TextUtils.h"
#include "AudioUtils.hpp"

#include "MiamExceptions.h"

#include "XmlUtils.h"

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


int TextUtils::ParseUdpPortNumber(const std::string& udpPort)
{
    int parsedPort = -1;
    try {
        parsedPort = std::stoi(udpPort);
    }
    catch (std::invalid_argument&) {
        throw Miam::ParseException("Invalid argument");
    }
    catch (std::out_of_range&) {
        throw Miam::ParseException("Out of integer type range");
    }
    
    if (0 <= parsedPort && parsedPort <= 65535)
        return parsedPort;
    else
        throw Miam::ParseException("Out of range for an UDP port");
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

std::string TextUtils::GetLimitedDigitsString(double value, int numberOfDigits)
{
    // On récupère d'abord des infos sur ce nombre
    bool isPositive = (value >= 0.0);
    double valueAbs = isPositive ? value : (-value);
    int nbFiguresBeforeComa;
    if (valueAbs < 1.0)
        nbFiguresBeforeComa = 1; // on compte le zéro pour l'affichage
    else
        // the + 0.00001 is here to compensate for log10(10^x) < x
        // due to numeric approximations (for double on macOS, intel proc...)
        nbFiguresBeforeComa = (int)std::ceil(std::log10(valueAbs + 0.00001));
    
    std::stringstream numberStream;
    numberStream << std::fixed << std::setprecision(numberOfDigits-nbFiguresBeforeComa) << value;
    std::string result = numberStream.str();
    if (isPositive)
        result = "+" + result;
    
    return result;
}

std::string TextUtils::GetAmplitude_dB_string_from_Linear(double linearValue, int numberOfDigits)
{
    linearValue = std::abs(linearValue);
    // minus infinity
    if (linearValue < Miam_MinVolume)
        return std::string("-") + String(CharPointer_UTF8 ("\xe2\x88\x9e")).toStdString();
    // or the actual cut dB value
    else
    {
        return GetLimitedDigitsString(AudioUtils<double>::Linear_to_Amplitude_dB(linearValue), numberOfDigits);
    }
}


std::string TextUtils::TryParseAndBoldenIpAddress(juce::TextEditor* textEditor)
{
    std::string ipAddress = textEditor->getText().toStdString();
    if ( XmlUtils::IsIpv4AddressValid(ipAddress) ) // en gras
    {
        textEditor->applyFontToAllText(Font().boldened());
        //ipAddressTextEditor->setColour(TextEditor::ColourIds::textColourId, Colours::palegreen);
        return ipAddress;
    }
    else
    {
        textEditor->applyFontToAllText(Font()); // font par défaut si c'est pas bon
        //ipAddressTextEditor->setColour(TextEditor::ColourIds::textColourId, Colours::white);
        return "";
    }
}
int TextUtils::TryParseAndBoldenUdpPort(juce::TextEditor* textEditor)
{
    bool enteredValueIsCorrect = true;
    int parsedValue = -1;
    try {
        parsedValue = std::stoi(textEditor->getText().toStdString());
    } catch (std::exception) {
        enteredValueIsCorrect = false;
    }
    if (parsedValue <= 0 || 65535 < parsedValue)
        enteredValueIsCorrect = false;
    
    if ( enteredValueIsCorrect ) // en gras
    {
        textEditor->applyFontToAllText(Font().boldened());
        return parsedValue;
    }
    else
    {
        textEditor->applyFontToAllText(Font()); // font par défaut si c'est pas bon
        return -1;
    }
}




juce::String TextUtils::GetCommandKeyDescription(int keyCode)
{
#ifdef JUCE_MAC // knot logo
    return String(CharPointer_UTF8 ("\xe2\x8c\x98")) + GetKeyDescription(keyCode);
#else
	// no space (like VS2017 menus)
    return String("Ctrl+") + GetKeyDescription(keyCode);
#endif
}

juce::String TextUtils::GetCommandShiftKeyDescription(int keyCode)
{
#ifdef JUCE_MAC // knot logo +  shift arrow (shift arrow first in macOS mojave)
    return String(CharPointer_UTF8 ("\xe2\x87\xa7"))
    + String(CharPointer_UTF8 ("\xe2\x8c\x98")) + GetKeyDescription(keyCode);
#else
    return String("Ctrl+") + TRANS("Shift") + "+" + GetKeyDescription(keyCode);
#endif
}

juce::String TextUtils::GetKeyDescription(int keyCode)
{
    // ASCII latin alphabet
    if (('A' <= keyCode) && (keyCode <= 'Z'))
    {
        char charCastedCode = (char)keyCode;
        return String(&charCastedCode, 1); // 1 char only
    }
    // action keys
    else if (keyCode == KeyPress::deleteKey)
        return String("Delete");
#ifdef JUCE_MACOS
	// macOS : simple, light arrows
    else if (keyCode == KeyPress::leftKey)
        return String(CharPointer_UTF8 ("\xe2\x86\x90"));
    else if (keyCode == KeyPress::upKey)
        return String(CharPointer_UTF8 ("\xe2\x86\x91"));
    else if (keyCode == KeyPress::rightKey)
        return String(CharPointer_UTF8 ("\xe2\x86\x92"));
    else if (keyCode == KeyPress::downKey)
        return String(CharPointer_UTF8 ("\xe2\x86\x93"));
#else
	// windows and others : bold arrows (shift always written in letters)
	else if (keyCode == KeyPress::leftKey)
		return String(CharPointer_UTF8("\xe2\xac\x85"));
	else if (keyCode == KeyPress::upKey)
		return String(CharPointer_UTF8("\xe2\xac\x86"));
	else if (keyCode == KeyPress::rightKey)
		return String(CharPointer_UTF8("\xe2\xae\x95"));
	else if (keyCode == KeyPress::downKey)
		return String(CharPointer_UTF8("\xe2\xac\x87"));
#endif
    else
    {
        // Please provide here a description for the key
        assert(false);
        return " ";
    }
}

void TextUtils::AddShortcutToTooltip(SettableTooltipClient& tooltipClient, String shortcutDescription)
{
    auto inputTooltip = tooltipClient.getTooltip();
    String outputTooltip;
    // Empty tooltip : easy, we just replace it by the shortcut description
    if (inputTooltip.isEmpty())
        outputTooltip = shortcutDescription;
    // non-empty.... We have to check for a '.' in the last letters
    else
    {
        // pré-traitement : ajout de parenthèse ou crochets
        shortcutDescription = "  [ " + shortcutDescription + " ]";
        // ajout ensuite. On aura 3 espaces au total
        if (inputTooltip.getLastCharacter() == '.')
            outputTooltip = inputTooltip + " " + shortcutDescription;
        else if (inputTooltip.getLastCharacters(2) == ". ")
            outputTooltip = inputTooltip + shortcutDescription;
        else
            outputTooltip = inputTooltip + ". " + shortcutDescription;
    }
    
    // application at the end
    tooltipClient.setTooltip(outputTooltip);
}


