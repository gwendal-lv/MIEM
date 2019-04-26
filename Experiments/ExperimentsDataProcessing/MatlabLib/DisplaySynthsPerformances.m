function [] = DisplaySynthsPerformances(globalParams, synthResults)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here


% Affichage des résultats sous forme de boîte à moustache ?
% Ce qui semble le plus simple : utiliser le boxplot à 2 args
% avec en premier toutes les données
% et en deuxième, le numéro de synthé auquel appartenait la donnée

% ça évite de gérer les problèmes de synthés vides !


% + rajouter les moyennes ? Uniquement les médianes sur le boxplot....


% 1 - Construction de 2 gros vecteurs colonne de données pour la
% boîte à moustache.
% On exclut les synthés de test (car impossible de comparer
% les 2 méthodes)
synthPerformances = zeros(0);
synthNames = zeros(0);
for j = (1+globalParams.trialSynthsCount) : globalParams.synthsCount
    % ajout d'un séparateur vide
    synthPerformances = [synthPerformances ;
                         -1 ];
    synthNames = [synthNames ;
                  strcat(int2str(j - globalParams.synthIdOffset), '.') ];
              % plot réel
    for jbis = 1:2
        numberToCopy = length(synthResults.all_P{j,jbis});
        % ajouts en colonne
        synthPerformances = [synthPerformances ;
                             (synthResults.all_P{j,jbis})' ];
        % noms traduits directement (en séparant interp/fader)
        if (jbis == 1)
            faderOrInterpChar = 'F';
        else
            faderOrInterpChar = 'I';
        end
        for n = 1:numberToCopy
            synthNames = [synthNames ;
                          strcat(faderOrInterpChar, int2str(j - globalParams.synthIdOffset)) ];
        end
    end
end
% dernier vide (pour laisser une marge)
synthPerformances = [synthPerformances ;
                         -1 ];
synthNames = [synthNames ;
                  strcat('.', '.') ];

% 2 - Plot + Affichage des labels correspondants à cette grosse matrice
% de données
boxPlotFigure = figure;
boxplot(synthPerformances, synthNames);%,  'PlotStyle','compact');
title('Performances for all synth sounds');
ylim([0 1]);

ylabel(['P', 'Performance score P']);
xlabel('Synth sound (F = Fader, I = Interpolation)');

% Change the boxplot color from blue to green
a = get(get(gca,'children'),'children');   % Get the handles of all the objects
t = get(a,'tag');   % List the names of all the objects 
% --> les objets sont dans l'ordre inverse de création (les premiers
% sont les derniers créés à l'écran)

% les tags sont répartis comme suit : (pour 2 * 10 sons de synthé)
% COMPACT
% 1 - ...rien.... (x20) -> en vrai : labels
% 2 - outliers (x20)
% 3 - median inner (x20)
% 4 - median outer (x20)
% 5 - box (x20)
% 6 - whisker (x20) = moustache !
% numberOfElementsOfEachType = (2 + 1) * globalParams.nonTrialSynthsCount + 1;
% boxesOffset = 5 * numberOfElementsOfEachType;
% whiskersOffset = 6 * numberOfElementsOfEachType;
% labelsOffset = 1 * numberOfElementsOfEachType;
% outliersOffset = 2 * numberOfElementsOfEachType;

% NORMAL
% 1 - outliers (x20)
% 2 - median (x20)
% 3 - box (x20)
% 4 - lower adjacent value (x20)
% 5 - upper adjacent value
% 6 - lower whisker
% 7 - upper whisker
numberOfElementsOfEachType = (2 + 1) * globalParams.nonTrialSynthsCount + 1;
boxesOffset = 3 * numberOfElementsOfEachType;
whiskersOffset = 6 * numberOfElementsOfEachType;
labelsOffset = 1 * numberOfElementsOfEachType;
outliersOffset = 1 * numberOfElementsOfEachType;
% pour faire de 4 à 7 dans une boucle
adjacentAndWhiskerOffset = 4 * numberOfElementsOfEachType;

for j=1:globalParams.nonTrialSynthsCount
    % les boxes commencent à partir de là (en décroissant)
    boxF = a( boxesOffset - 3*(j-1) -1 );
    boxI = a( boxesOffset - 3*(j-1) -2 );
    whiskerF = a( whiskersOffset - 3*(j-1) -1 );
    whiskerI = a( whiskersOffset - 3*(j-1) -2 );
    labelF = a( labelsOffset - 3*(j-1) -1 );
    labelI = a( labelsOffset - 3*(j-1) -2 );
    outlierF = a( outliersOffset - 3*(j-1) -1 );
    outlierI = a( outliersOffset - 3*(j-1) -2 );
    % application des couleurs selon Interp ou Fader
    set(boxF, 'Color', globalParams.colors(1,:));
    set(boxI, 'Color', globalParams.colors(2,:));
    set(whiskerF, 'Color', globalParams.colors(1,:));
    set(whiskerI, 'Color', globalParams.colors(2,:));
    set(labelF, 'Color', globalParams.colors(1,:));
    set(labelI, 'Color', globalParams.colors(2,:));
    set(outlierF, 'Color', globalParams.colors(1,:));
    set(outlierI, 'Color', globalParams.colors(2,:));
    for n = 0:3
        objectF = a( (adjacentAndWhiskerOffset + n*numberOfElementsOfEachType) - 3*(j-1) -1 );
        objectI = a( (adjacentAndWhiskerOffset + n*numberOfElementsOfEachType) - 3*(j-1) -2 );
        set(objectF, 'Color', globalParams.colors(1,:));
        set(objectI, 'Color', globalParams.colors(2,:));
    end
end





% ajout de lignes de séparation
hold on;
for j=1:(globalParams.nonTrialSynthsCount-1)
    line([(1+3*j) (1+3*j)], [0 1], 'Color', 'k'); % vecteur des x et des y resp.
end

% pour passer bien dans Live Script
set(boxPlotFigure, 'Position', [0 0 560 250]);

end

