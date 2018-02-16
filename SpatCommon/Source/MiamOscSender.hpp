/*
  ==============================================================================

    MiamOscSender.hpp
    Created: 14 Jun 2017 8:44:27am
    Author:  Gwendal Le Vaillant

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

#include "OscDefines.h"

#include "SpatSender.hpp"
#include "MatrixBackupState.hpp"

#include "boost/endian/conversion.hpp"


namespace Miam
{
    
    template<typename T>
    class MiamOscSender : public SpatSender<T> {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        int udpPort;
        std::string ipv4;
        OSCSender oscSender;
        
        // 2 = nombre minimum de coefficients de matrices que l'on doit mettre dans un bloc
        // Si on moins d'infos que ça à transmettre : on transmet un coeff seul
        const int minCoeffsInBlob = 2;
        // Nombre max, de sorte d'avoir une trame de 1500octets max au total.
        // (c'est la limite des jumbo frames / trames géantes). On va prendre
        // 1400 octets de données binaires; dans le doute...
        // Chaque coeff fait 3*4 = 12 octets
        const int maxCoeffsInBlob = 116;
        MemoryBlock oscMemoryBlock;
        
        int iToRefresh, jToRefresh;
        
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const override
        {return std::string("MiamOsc");}
        
        
        
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
            udpPort = _udpPort;
        }
        /// \brief Disconnects OSC before changing the configuration.
        void SetIpv4(std::string _ipv4)
        {
            oscSender.disconnect();
            ipv4 = _ipv4;
        }
        bool TryConnect()
        {
            if ( ( !ipv4.empty() ) && ( udpPort > 0 ) )
                return oscSender.connect(ipv4, udpPort);
            else
                return false;
        }
        
        
        // - - - - - Communication commands - - - - -
        public :
        
        /// \brief Sends the whole spat states without any optimisation
        virtual void SendState(SpatState<T>& spatState) override
        {
            // Capable d'envoyer un état matriciel seulement pour l'instant !
            // On travaille avec pointeur... Sinon avec cast de référence : il faudrait vérifier
            // les exceptions "bad_cast"
            //
            /// Aucune optimisation : coefficients sont tous envoyés 1 par 1 dans des paquets OSC séparés...
            if (MatrixState<T>* matrixState = dynamic_cast<MatrixState<T>*>(&spatState))
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
        
        virtual void SendStateModifications(SpatState<T>& spatState) override
        {
            // Capable d'envoyer un état matriciel avec backup seulement pour l'instant
            if (MatrixBackupState<T>* matrixState = dynamic_cast<MatrixBackupState<T>*>(&spatState))
            {
                // On met à jour les éléments nécessaires seulement
                auto changesIndexes = matrixState->GetSignificantChangesIndexes();
                // Transmission de TRèS PEU de coefficients
                if (0 < changesIndexes.size() && changesIndexes.size() < minCoeffsInBlob)
                {
                    for (size_t i=0 ; i<changesIndexes.size() ; i++)
                    {
                        if (matrixState->IsIndexWithinActualInputOutputBounds(changesIndexes[i]))
                        {
                            Index2d index2d = matrixState->GetIndex2dFromIndex(changesIndexes[i]);
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
                        SendCoeffsInMemoryBlock();
                    }
                }
                
                // On informe la matrice que tout a bien été envoyé
                matrixState->ApplyAndClearChangesList();
            }
            else
                throw std::logic_error("Cannot send the modifications of a state that is not a BackupMatrix");
        }
        
        // Envoi d'un seul coeff
        void SendMatrixCoeff(int i, int j, float value)
        {
            oscSender.send(Miam_OSC_Matrix_Address, i, j, value);
        }
        // Envoi d'un blob avec tout un ensemble de coeffs
        void SendCoeffsInMemoryBlock()
        {
            // Vérification que n'on ne passe pas en jumbo frame (dans le doute...)
            if (oscMemoryBlock.getSize() < 1400)
                assert(false);
            
            // Envoi si ça va
            OSCMessage blobMessage(Miam_OSC_Matrix_Address);
            blobMessage.addBlob(oscMemoryBlock);
            oscSender.send(blobMessage);
        }
        
        /// \brief Commande d'actualisation d'1 coefficient.
        ///
        /// Au prochain appel, c'est la case d'après qui sera transmise. Sens de parcours :
        /// ligne entière par ligne entière
        void ForceSend1MatrixCoeff(SpatState<T>& spatState)
        {
            // Capable d'envoyer un état matriciel avec backup seulement pour l'instant
            if (MatrixBackupState<T>* matrixState = dynamic_cast<MatrixBackupState<T>*>(&spatState))
            {
                // Mise à jour du couple (i,j) pour commencer
                // -> ça fait office de vérification qu'on a des coeffs bien dans la matrice
                // fin de colonne : ligne suivante
                if ( (jToRefresh++) >= matrixState->GetOutputsCount() )
                {
                    jToRefresh = 0;
                    iToRefresh++;
                    // fin de toutes les lignes : on revient à zéro
                    if ( iToRefresh >= matrixState->GetInputsCount() )
                        iToRefresh = 0;
                }
                
                // Envoi du coeff concerné
                SendMatrixCoeff(iToRefresh, jToRefresh,
								(float) (*matrixState)(iToRefresh, jToRefresh));
                
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
        
        
        
        // - - - - - OSC and Memory blocks helpers - - - - -
        private :
        /// \brief The amount of bytes required to store N matrix coefficients
        ///
        ///   1  x int32 (number of coeffs)
        /// + 2N x int32
        /// + N  x float32
        size_t getRequiredBytesCount(size_t coeffsCount)
        {
            return (sizeof(int32_t) * (2*coeffsCount +1)) + (sizeof(Float32) * coeffsCount);
        }
        /// \brief Fills the internal memory block with the concerned coefficients
        ///
        /// startPos is an included bound, count is the number of coeffs written
        void fillInternalCoeffsMemoryBlock(MatrixBackupState<T>* matrixState,
                                           std::vector<size_t>& actualChangesIndexes,
                                           size_t startPos, size_t coeffsCount)
        {
            int32_t nativeInt, bigEndianInt;
            oscMemoryBlock.setSize(getRequiredBytesCount(coeffsCount));
            // Nombre de coeffs d'abord
            nativeInt = (int32_t) coeffsCount;
            bigEndianInt = boost::endian::native_to_big<int32_t>(nativeInt);
            oscMemoryBlock.copyFrom(&bigEndianInt, 0, sizeof(int32_t));
            // Ensuite, tous les triplets int/int/float
            for (size_t i=0 ; i<coeffsCount ; i++)
            {
                auto index2d = matrixState->GetIndex2dFromIndex(actualChangesIndexes[i]);
                nativeInt = (int32_t)index2d.i;
                bigEndianInt = boost::endian::native_to_big<int32_t>(nativeInt);
                oscMemoryBlock.copyFrom(&bigEndianInt,
                                        (int) getRequiredBytesCount(i),
                                        sizeof(int32_t));
                nativeInt = (int32_t)index2d.j;
                bigEndianInt = boost::endian::native_to_big<int32_t>(nativeInt);
                oscMemoryBlock.copyFrom(&bigEndianInt,
                                        (int) getRequiredBytesCount(i) + sizeof(int32_t),
                                        sizeof(int32_t));
                Float32 floatValue = (Float32) (*matrixState)[actualChangesIndexes[i]];
                oscMemoryBlock.copyFrom(&floatValue,
                                        (int) getRequiredBytesCount(i) + 2*sizeof(int32_t),
                                        sizeof(Float32));
                
                if (index2d.i == 0 && index2d.j == 0)
                {
                    std::cout << "------- dans blob : [0;0] == " << floatValue << std::endl;
                }
            }
        }
        
        
        // - - - - - XML import/export - - - - -
        public :
        virtual std::shared_ptr<bptree::ptree> GetConfigurationTree() override
        {
            auto configurationTree = SpatSender<T>::GetConfigurationTree();
            configurationTree->put("ip", ipv4);
            configurationTree->put("udp.port", udpPort);
            return configurationTree;
        }
        virtual void SetConfigurationFromTree(bptree::ptree& tree) override
        {
            // Inutile pour l'instant, mais dans le doute...
            SpatSender<T>::SetConfigurationFromTree(tree);
            // Chargement des pptés spécifiques au messages miam osc
            udpPort = tree.get<int>("udp.port", udpPort); // no exception
            ipv4 = tree.get<std::string>("ip", ipv4); // no exception
        }
        
    };
}
