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


namespace Miam
{
    
    template<typename T>
    class MiamOscSender : public SpatSender<T> {
        
        
        // = = = = = = = = = = ATTRIBUTES = = = = = = = = = =
        private :
        int udpPort;
        std::string ipv4;
        OSCSender oscSender;
        
        int iToRefresh, jToRefresh;
        
        // = = = = = = = = = = SETTERS and GETTERS = = = = = = = = = =
        public :
        virtual std::string GetTypeAsString() const override
        {return std::string("MiamOsc");}
        
        
        
        // = = = = = = = = = = METHODS = = = = = = = = = =
        public :
        
        
        // - - - - - Construction / destruction - - - - -
        MiamOscSender() :
        udpPort(8001), ipv4("127.0.0.1"), iToRefresh(0),
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
                for (size_t i=0 ; i<changesIndexes.size() ; i++)
                {
                    Index2d index2d = matrixState->GetIndex2dFromIndex(changesIndexes[i]);
                    // et on n'envoie que les E/S nécessaires (sachant que toutes les matrices ici sont
                    // trop grandes, allouées statiquement à la taille max)
                    if (index2d.i < matrixState->GetInputsCount()
                        && index2d.j < matrixState->GetOutputsCount())
                    SendMatrixCoeff((int) index2d.i, (int) index2d.j,
                                    (float) (*matrixState)[changesIndexes[i]]);
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
                SendMatrixCoeff(iToRefresh, jToRefresh, (*matrixState)(iToRefresh, jToRefresh));
                
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
