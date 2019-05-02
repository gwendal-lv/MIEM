/*
  ==============================================================================

    MiamOscSender.hpp
    Created: 14 Jun 2017 8:44:27am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include <algorithm> // std::min ...

#include "JuceHeader.h"

#include "OscDefines.h"
#include "AppPurpose.h"

#include "TextUtils.h"

#include "ControlStateSender.hpp"
#include "MatrixBackupState.hpp"

#include "boost/endian/conversion.hpp"
#include "boost/lexical_cast.hpp"


namespace Miam
{
    
    template<typename T>
    class MiamOscSender : public ControlStateSender<T> {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        int udpPort;
        std::string ipv4;
        OSCSender oscSender;
        /// \brief for experiments (duplicated messages on osc port +10 000).
        /// Not reliable : no errors will be displayed if this port cannot connect or send
        OSCSender oscSender2;
        const int secondSenderUdpPortOffset = 10000;
#if defined(__MIEM_EXPERIMENTS) || defined(__MIEM_FORCE_DOUBLE_OSC_SENDER)
        const bool alsoUseSender2 = true;
#else
        const bool alsoUseSender2 = false;
#endif
        
        bool sendFirstColOnly = false;
        std::vector<OSCMessage> firstColOscMessages;
        
        // 2 = nombre minimum de coefficients de matrices que l'on doit mettre dans un bloc
        // Si on moins d'infos que ça à transmettre : on transmet un coeff seul
        const int minCoeffsInBlob = 2;
        // Nombre max, de sorte d'avoir une trame de 1500octets max au total.
        // (c'est la limite des jumbo frames / trames géantes). On va prendre
        // 1400 octets de données binaires; dans le doute...
        // Chaque coeff fait 3*4 = 12 octets
        const int maxCoeffsInBlob = 116;
        MemoryBlock oscMemoryBlock;
        
        public :
        /// \brief Nombre de coefficients transmis en même temps lors d'un rafraîchisssement forcé d'un bloc
        const size_t BlockRefreshSize = 64;
        private :
        
        int iToRefresh, jToRefresh;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const override
        {return std::string("MiamOsc");}
        virtual std::string GetAddressAsString() const override
        { return ipv4 + ":" + boost::lexical_cast<std::string>(udpPort); }
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        MiamOscSender() :
        udpPort(8001), ipv4("127.0.0.1"),
        iToRefresh(0),
        jToRefresh(-1) // -1 car on va l'incrémenter dès le premier tour
        {
        }
        
        virtual ~MiamOscSender() {}
        
        
        // - - - - - Communication configuration - - - - -
        public :
        /// \brief Disconnects OSC before changing the configuration.
        void SetUdpPort(int _udpPort)
        {
            oscSender.disconnect();
            if (alsoUseSender2)
                oscSender2.disconnect();
            udpPort = _udpPort;
        }
        /// \brief Disconnects OSC before changing the configuration.
        void SetIpv4(std::string _ipv4)
        {
            oscSender.disconnect();
            if (alsoUseSender2)
                oscSender2.disconnect();
            ipv4 = _ipv4;
        }
        bool TryConnect()
        {
            if ( ( !ipv4.empty() ) && ( udpPort > 0 ) )
            {
                if (alsoUseSender2)
                    oscSender2.connect(ipv4, udpPort + secondSenderUdpPortOffset); // no connection verification for 2nd sender
                
                return oscSender.connect(ipv4, udpPort);
            }
            else
                return false;
        }
        /// \brief Choisit si l'on ne va envoyer que les coeffients de la première colonne
        /// (contrôle de paramètres simple, pas contrôle de matrice entière). Si c'est le cas,
        /// on peut préciser l'adresse OSC pour chaque ligne (sinon, adresse de ligne par défaut
        /// avec numéro de ligne)
        void EnableSendFirstColOnly(bool sendFirstColOnly_,
                                    const std::vector<std::string> & oscAddressPatterns)
        {
            EnableSendFirstColOnly(sendFirstColOnly_);
            
            if (sendFirstColOnly)
            {
                // Création du maximum possible de messages
                firstColOscMessages.reserve(Miam_MaxNumInputs);
                for (size_t i=0 ; i<Miam_MaxNumInputs ; i++)
                {
                    bool useGenericAddressPattern = false;
                    if (i < oscAddressPatterns.size())
                    {
                        if (! oscAddressPatterns[i].empty())
                        {
                            try {
                                OSCMessage message = TextUtils::ParseStringToJuceOscMessage(oscAddressPatterns[i]);
                                firstColOscMessages.push_back(message);
                            }
                            catch (ParseException& /*e*/) {
                                useGenericAddressPattern = true;
                            }
                        }
                        else
                            useGenericAddressPattern = true;
                    }
                    else
                        useGenericAddressPattern = true;
                     // si le parse n'a pas fonctionné, ou si n'avait juste pas de string précisée,
                     // alors on met une adresse par défaut.
                     // Attention ! On fait correspondre les adresses aux numéros des lignes...
                     // (à partir de 1, par à partir de zéro)
                    if (useGenericAddressPattern)
                    {
                        OSCAddressPattern addressPattern(Miam_OSC_Generic_Param_Address
                                                         + std::string("/")
                                                         + boost::lexical_cast<std::string>(i+1));
                        firstColOscMessages.push_back(OSCMessage(addressPattern));
                    }
                }
            }
        }
        void EnableSendFirstColOnly(bool sendFirstColOnly_)
        {
            sendFirstColOnly = sendFirstColOnly_;
            firstColOscMessages.clear(); // dans tous les cas...
        }
        
        // - - - - - Communication commands - - - - -
        public :
        
        /// \brief Sends a whole state without any optimisation.
        ///
        /// Only matrix states at the moment...
        virtual void SendState(ControlState<T>& state) override
        {
            // Capable d'envoyer un état matriciel seulement pour l'instant !
            // On travaille avec pointeur... Sinon avec cast de référence : il faudrait vérifier
            // les exceptions "bad_cast"
            //
            /// Aucune optimisation : coefficients sont tous envoyés 1 par 1 dans des paquets OSC séparés...
            if (MatrixState<T>* matrixState = dynamic_cast<MatrixState<T>*>(&state))
            {
                for (size_t i = 0 ; i<matrixState->GetInputsCount() ; i++)
                {
                    for (size_t j = 0 ; j<matrixState->GetOutputsCount() ; j++)
                    {
                        SendMatrixCoeff((int)i, (int)j, (float)(*matrixState)(i,j) );
                    }
                }
            }
            else
                throw std::logic_error("Cannot send a state that is not a Matrix");
        }
        
        virtual void SendStateModifications(ControlState<T>& state) override
        {
            // Capable d'envoyer un état matriciel avec backup seulement pour l'instant
            if (MatrixBackupState<T>* matrixState = dynamic_cast<MatrixBackupState<T>*>(&state))
            {
                // On met à jour les éléments nécessaires seulement
                auto changesIndexes = matrixState->GetSignificantChangesIndexes();
                
                // - - - mode MATRICE COMPLÈTE - - -
                if (!sendFirstColOnly)
                {
                    SendMatrixCoeffChanges(matrixState, changesIndexes);
                }
                // - - - mode PREMIÈRE COLONNE SEULEMENT - - -
                else
                {
                // with interpolation/distorsion curves
                    SendMatrixParamChanges(matrixState, changesIndexes);
                }
                
                // On informe la matrice que tout a bien été envoyé
                // ... même si on n'a envoyé que la première colonne
                matrixState->ApplyAndClearChangesList();
            }
            else
                throw std::logic_error("Cannot send the modifications of a state that is not a BackupMatrix");
        }

        /// \brief Sends the parameters designated by their index within the
        /// given list. Interpolation/distorsion curves can be specified.
        void SendRawMatrixParamChanges(MatrixState<T>* matrixState, std::vector< size_t >& changesIndexes)
        {
            for (size_t i=0 ; i<changesIndexes.size() ; i++)
            {
                if (matrixState->IsIndexWithinActualInputOutputBounds(changesIndexes[i]))
                {
                    Index2d index2d = matrixState->GetMatrix()->GetIndex2dFromIndex(changesIndexes[i]);
                    if (index2d.j == 0)
                    {
                        SendParam(index2d.i,
                                  (float) (*matrixState)(index2d.i,index2d.j)
                                  );
                    }
                }
                // débordement détecté, ne devrait pas arriver...
                else
                {
                    // En fait SI SI SI un débordement peut arriver ! Lorsqu'un coefficient est resté
                    // enregistré alors que l'on a réduit la taille des états. Pas d'assertion donc
                    // (car on autorise les valeurs qui débordent, mais on ne les envoie pas)
                    //assert(false);
                }
            }
        }
        
        /// \brief Sends the parameters designated by their index within the
        /// given list. Interpolation/distorsion curves will be applied.
        void SendMatrixParamChanges(MatrixBackupState<T>* matrixState,
                                    std::vector< size_t >& changesIndexes)
        {
            for (size_t i=0 ; i<changesIndexes.size() ; i++)
            {
                if (matrixState->IsIndexWithinActualInputOutputBounds(changesIndexes[i]))
                {
                    Index2d index2d = matrixState->GetMatrix()->GetIndex2dFromIndex(changesIndexes[i]);
                    if (index2d.j == 0)
                    {
                        SendParam(index2d.i,
                                  (float) matrixState->GetCurveInterpolatedMatrixCoeff(index2d.i,
                                                                                       index2d.j)
                                  );
                    }
                }
                // débordement détecté, ne devrait pas arriver...
                else
                {
                    // En fait SI SI SI un débordement peut arriver ! Lorsqu'un coefficient est resté
                    // enregistré alors que l'on a réduit la taille des états. Pas d'assertion donc
                    // (car on autorise les valeurs qui débordent, mais on ne les envoie pas)
                    //assert(false);
                }
            }
        }
    
        /// \brief Sends the matrix coefficients designated by their index within
        /// the given list. A MIEM OSC bloc might be used if the optimisation is
        /// considered interesting.
        void SendMatrixCoeffChanges(MatrixState<T>* matrixState, std::vector< size_t >& changesIndexes)
        {
            // Transmission de TRèS PEU de coefficients
            if (0 < changesIndexes.size() && changesIndexes.size() < minCoeffsInBlob)
            {
                for (size_t i=0 ; i<changesIndexes.size() ; i++)
                {
                    if (matrixState->IsIndexWithinActualInputOutputBounds(changesIndexes[i]))
                    {
                        Index2d index2d = matrixState->GetMatrix()->GetIndex2dFromIndex(changesIndexes[i]);
                        SendMatrixCoeff((int) index2d.i, (int) index2d.j,
                                        (float) (*matrixState)[changesIndexes[i]]);
                    }
                }
            }
            // Transmission de PLUSIEURS coefficients
            else if (changesIndexes.size() >= minCoeffsInBlob)
            {
                // Première passe : liste des coeffs avec indices valides
                std::vector<size_t> actualChangesIndexes;
                actualChangesIndexes.reserve(changesIndexes.size()); // on aura assez
                for (size_t i=0 ; i<changesIndexes.size() ; i++)
                    if (matrixState->IsIndexWithinActualInputOutputBounds(changesIndexes[i]))
                        actualChangesIndexes.push_back(changesIndexes[i]);
                // 2nde passe : remplissage et envoie effectifs
                // du ou des blocs de mémoire avec encodage big endian pour transmission réseau.
                // on sait qu'on a minimum 1 bloc (sinon c'était coeff seul), pas besoin de tester
                size_t oscBlobsCount = (actualChangesIndexes.size() / maxCoeffsInBlob) + 1;
                for (size_t i = 0 ; i<oscBlobsCount ; i++)
                {
                    // Remplissage à partir d'un sous vecteur
                    if (i == oscBlobsCount-1) // dernier blob : incomplet
                        fillInternalCoeffsMemoryBlock(matrixState,
                                                      actualChangesIndexes,
                                                      i*maxCoeffsInBlob,
                                                      actualChangesIndexes.size()-i*maxCoeffsInBlob);
                    else // blobs complets proches de 1,4ko
                        fillInternalCoeffsMemoryBlock(matrixState,
                                                      actualChangesIndexes,
                                                      i*maxCoeffsInBlob,
                                                      maxCoeffsInBlob);
                    // Transmission OSC (encodage déjà fait, au remplissage)
                    sendCoeffsInMemoryBlock();
                }
            }
        }
        
        /// \brief  Envoi d'un seul coeff
        void SendMatrixCoeff(int i, int j, float value)
        {
            oscSender.send(Miam_OSC_Matrix_Address, i, j, value);
            if (alsoUseSender2)
                oscSender2.send(Miam_OSC_Matrix_Address, i, j, value); // no check either...
        }
        /// \brief Envoi d'un seul coeff, à une adresse de paramètre OSC
        /// qui doit avoir déjà été initialisée/configurée
        void SendParam(size_t paramIndex, float value)
        {
            // On garde les arguments constants pré-configurés
            OSCMessage oscMessage = firstColOscMessages[paramIndex];
            oscMessage.addFloat32(value);
            oscSender.send(oscMessage);
            if (alsoUseSender2)
                oscSender2.send(oscMessage); // no check either...
        }
        /// \brief Converts the value and sends its as a float to the given OSC address
        ///
        /// Might send an exception if address isn't valid of if message couldn't be sent
        ///
        /// \param addressPattern OSC address that can contain pre-defined arguments ([int] 3 or [float] 4.5 for example)
        void SendToAddressAsFloat(const std::string& addressPattern, T argumentValue)
        {
            OSCMessage message = TextUtils::ParseStringToJuceOscMessage(addressPattern);
            message.addFloat32( (float) argumentValue );
            
            if (! oscSender.send(message))
                throw Miam::OscException("OSC Message couldn't be sent. Check OSC settings and addresses.");
            if (alsoUseSender2)
                oscSender2.send(message); // no check
        }
        
        // Envoi d'un blob avec tout un ensemble de coeffs
        protected :
        void sendCoeffsInMemoryBlock()
        {
            // Vérification que n'on ne passe pas en jumbo frame (dans le doute...)
            if (oscMemoryBlock.getSize() > 1400)
                assert(false);
            
            // Envoi si ça va
            OSCMessage blobMessage(Miam_OSC_Matrix_Address);
            blobMessage.addBlob(oscMemoryBlock);
            oscSender.send(blobMessage);
            if (alsoUseSender2)
                oscSender2.send(blobMessage);
        }
        
        public :
        
        /// \brief Commande d'actualisation d'1 coefficient.
        ///
        /// Au prochain appel, c'est la case d'après qui sera transmise. Sens de parcours :
        /// ligne entière par ligne entière
        void ForceSend1MatrixCoeff(ControlState<T>& state, AppPurpose appPurpose)
        {
            // Capable d'envoyer un état matriciel avec backup seulement pour l'instant
            if (MatrixBackupState<T>* matrixState = dynamic_cast<MatrixBackupState<T>*>(&state))
            {
                // Mise à jour du couple (i,j)toRefresh pour commencer
                // (gère le mode 1 seule colonne)
                increment2dRefreshIndex(matrixState);
                // Envoi du coeff concerné, selon que l'on soit en mode 1 seule colonne ou non.
                if (!sendFirstColOnly)
                    SendMatrixCoeff(iToRefresh, jToRefresh,
                                    (float) (*matrixState)(iToRefresh, jToRefresh));
                // Si 1 seule colonne : on utilise juste SendParam(i,value)
                else // avec le bon nom de message OSC
                {
                    SendParam(iToRefresh, (float) matrixState->GetCurveInterpolatedMatrixCoeff(iToRefresh, jToRefresh)); // j vaudra zéro !
                }
                
                // ----------- à faire -------------
                // ----------- à faire -------------
                // ----------- à faire -------------
                // On informe la matrice que tout a bien été envoyé
                // Aucun update des états "backup"... Pour ne pas générer de pb de logique
                // À améliorer : il faudrait dire qu'on vient d'envoyer JUSTE 1 COEFFICIENT (et pas TOUS les changements d'un coup)
                // ----------- à faire -------------
                // ----------- à faire -------------
                // ----------- à faire -------------
            }
            else
                throw std::logic_error("Cannot send 1 coeff of a state that is not a BackupMatrix");
        }
        
        /// \brief Commande d'actualisation d'un paquet de coefficients
        ///
        /// Transmission dans un OSC blob du nombre MiamOscSender::BlockRefreshSize de coefficients.
        /// à chaque appel, on envoie le blocks de coeffs suivants, ligne par ligne, dans les limites de
        /// la matrice transmise à la fonction.
        void ForceCoeffsBlockRefresh(ControlState<T>& spatState)
        {
            // Capable d'envoyer un état matriciel avec backup seulement pour l'instant
            if (MatrixBackupState<T>* matrixState = dynamic_cast<MatrixBackupState<T>*>(&spatState))
            {
                // sauvegarde locale, car on va bcp s'en servir...
                size_t outsCount = matrixState->GetOutputsCount();
                size_t insCount = matrixState->GetInputsCount();
                // On se sert des mêmes (i,j) que pour le refraîchissement d'un seul coeff
                // SAUF s'il y a eu un soucis :
                // on vérifie que la matrice n'a pas changé et que les indexes collent tjs
                if (iToRefresh >= insCount || jToRefresh >= outsCount)
                {
                    // Sinon on réinitialise juste, sauvagement...
                    iToRefresh = 0;
                    jToRefresh = 0;
                }
                
                // - - - D'abord : détermination du bloc à transmettre + mise à jour de i,j directement - - -
                std::vector<size_t> changesIndexes;
                size_t numberOfCoeffsToSend = std::min(insCount*outsCount, BlockRefreshSize);
                changesIndexes.reserve(numberOfCoeffsToSend);
                // on refraîchit juste toute la matrice
                for (size_t k=0 ; k < numberOfCoeffsToSend ; k++)
                {
                    // Ajout d'un coeff
                    Index2d index2d {(size_t)iToRefresh, (size_t)jToRefresh};
                    changesIndexes.push_back(matrixState->GetMatrix()->GetIndexFromIndex2d(index2d));
                    // Actualisation de i,j
                    increment2dRefreshIndex(matrixState);
                }
                
                // - - - Envoi des coeff concernés - - -
                fillInternalCoeffsMemoryBlock(matrixState, changesIndexes, 0, changesIndexes.size());
                sendCoeffsInMemoryBlock();
                //std::cout << "block de refresh envoyé, taille " << oscMemoryBlock.getSize() << " pour " << changesIndexes.size() << "coefficients" << std::endl;
                
                // ----------- à faire -------------
                // ----------- à faire -------------
                // ----------- à faire -------------
                // On informe la matrice que tout a bien été envoyé
                // Aucun update des états "backup"... Pour ne pas générer de pb de logique
                // À améliorer : il faudrait dire qu'on vient d'envoyer JUSTE QQS COEFFICIENT (et pas TOUS les changements d'un coup)
                // ----------- à faire -------------
                // ----------- à faire -------------
                // ----------- à faire -------------
            }
            else
                throw std::logic_error("Cannot send 1 coeff of a state that is not a BackupMatrix");
        }
        
        
        // - - - - - Various auxiliary functions - - - - -
        /// \brief Incrémente le compteur interne 2d de coeffient
        /// (compteur qui sert par exemple à rafraîchir 1 coeff en particulier)
        /// en appliquant les modulos nécessaire pour ligne/colonne.
        void increment2dRefreshIndex(MatrixState<T>* matrixState)
        {
            // limite en j (sur la ligne) : selon qu'on travaille
            // uniquement sur la colonne 1, ou non !
            int refreshColsCount = 0;
            if (sendFirstColOnly)
                refreshColsCount = 1;
            else
                refreshColsCount = (int) matrixState->GetOutputsCount();
            
            // -> ça fait office de vérification qu'on a des coeffs bien dans la matrice
            // fin de colonne : ligne suivante
            jToRefresh++;
            if ( jToRefresh >= refreshColsCount )
            {
                jToRefresh = 0;
                iToRefresh++;
                // fin de toutes les lignes : on revient à zéro
                if ( iToRefresh >= matrixState->GetInputsCount() )
                    iToRefresh = 0;
            }
        }
        
        
        // - - - - - OSC and Memory blocks helpers - - - - -
        private :
        /// \brief The amount of bytes required to store N matrix coefficients
        ///
        ///   1  x int32        (number of coeffs)
        /// + 2N x int32        ( (i,j) matrix position of the coeff)
        /// + N  x float32      (value of the matrix coeff)
        size_t getRequiredBytesCount(size_t coeffsCount)
        {
            return (sizeof(int32_t) * (2*coeffsCount +1)) + (sizeof(float) * coeffsCount);
        }
        /// \brief Fills the internal memory block, from its first byte,
        /// with the concerned coefficients
        ///
        /// \param startPos is an included bound, it is an index of the source
        /// \param coeffsCount is the number of coeffs written
        void fillInternalCoeffsMemoryBlock(MatrixState<T>* matrixState,
                                           std::vector<size_t>& actualChangesIndexes,
                                           size_t startIndexPos,
                                           size_t coeffsCount)
        {
			// pas un test complet, mais donne déjà une idée
			assert(sizeof(float) == 4); // cannot work on non-IEEE-754 architectures...

            int32_t nativeInt, bigEndianInt;
            oscMemoryBlock.setSize(getRequiredBytesCount(coeffsCount));
            // pour 116 coeffs (valeur pleine maxCoeffsInBlock)
            // -> taille 1396 o < 1,4 ko
            
            // Nombre de coeffs d'abord
            nativeInt = (int32_t) coeffsCount;
            bigEndianInt = boost::endian::native_to_big<int32_t>(nativeInt);
            oscMemoryBlock.copyFrom(&bigEndianInt, 0, sizeof(int32_t));
            // Ensuite, tous les triplets int/int/float
            for (size_t i=0 ; i<coeffsCount ; i++)
            {
                if ((startIndexPos + i) >= 116)
                {
                    bool inutile;
                    inutile = true;
                }
                // TOUJOURS DES BUGS ICI
                // TOUJOURS DES BUGS ICI
                // TOUJOURS DES BUGS ICI
                // TOUJOURS DES BUGS ICI
                // repli par rapport à 116...
                auto matrixStateIndex = actualChangesIndexes[startIndexPos + i];
                auto index2d = matrixState->GetMatrix()->GetIndex2dFromIndex(matrixStateIndex);
                
                nativeInt = (int32_t)index2d.i;
                bigEndianInt = boost::endian::native_to_big<int32_t>(nativeInt);
                oscMemoryBlock.copyFrom(&bigEndianInt,
                                        (int)(getRequiredBytesCount(i)), // destination (= block) offet
                                        sizeof(int32_t));
                nativeInt = (int32_t)index2d.j;
                bigEndianInt = boost::endian::native_to_big<int32_t>(nativeInt);
                oscMemoryBlock.copyFrom(&bigEndianInt,
                                        (int)(getRequiredBytesCount(i) + sizeof(int32_t)),
                                        sizeof(int32_t));
                float floatValue = (float) (*matrixState)[matrixStateIndex];
                oscMemoryBlock.copyFrom(&floatValue,
                                        (int)(getRequiredBytesCount(i) + 2*sizeof(int32_t)),
                                        sizeof(float));
            }
        }
        
        
        // - - - - - XML import/export - - - - -
        public :
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree() override
        {
            auto configurationTree = ControlStateSender<T>::GetConfigurationTree();
            configurationTree->put("ip", ipv4);
            configurationTree->put("udp.port", udpPort);
            // port additionnel facultatif (pour double envoi des données)
            if (alsoUseSender2)
                configurationTree->put("udp.additional_port", udpPort + secondSenderUdpPortOffset);
            return configurationTree;
        }
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override
        {
            // Inutile pour l'instant, mais dans le doute...
            ControlStateSender<T>::SetConfigurationFromTree(tree);
            // Chargement des pptés spécifiques au messages miam osc
            udpPort = tree.get<int>("udp.port", udpPort); // no exception
            ipv4 = tree.get<std::string>("ip", ipv4); // no exception
        }
        
    };
}
