function [ ] = DisplayCurves( globalParams, experiments, expeData, expeId, graphPresetId )
%DISPLAYCURVES Summary of this function goes here
%   Detailed explanation goes here

limitesOrdonnees = [-1 1];
maxAbscisses = globalParams.maxTime; % si négatif : pas de limite forcée

% Sub-plot en 20 parties
%
% dans l'ordre chrono pour voir l'évolution (si évolution il y a)
if (experiments(expeId).isValid)
    curvesFigure = figure('Name', strcat('Visualisation of experiment #', num2str(expeId)));
else
    figure('Name', strcat('Data not valid - experiment #', num2str(expeId)));
    return;
end

    
% agrandi pour live script
% ---> quand il y a des subplots, ne fonctionne que si on appelle
% le resize avant les subplots !
set(curvesFigure, 'Position', [0 0 1100 700]);


% Tableau des couleurs : chaque paramètre aura sa couleur. source couleurs 2014b http://math.loyola.edu/~loberbro/matlab/html/colorsInMatlab.html
couleurs = [0.0000 0.4470 0.7410 ;
            0.8500 0.3250 0.0980 ;
            0.9290 0.6940 0.1250 ;
            0.4940 0.1840 0.5560 ;
            0.4660 0.6740 0.1880 ;
            0.3010 0.7450 0.9330 ;
            0.6350 0.0780 0.1840 ];


% - - - - - Si graphID <= 0 : on affiche TOUT - - - - -
% (sauf les essais...)

if (graphPresetId <= 0)

    % Ensuite, plot des 20 échantillons testés
    % sur 4 lignes et 5 colonnes
    subplotRowsCount = 4;
    subplotColsCount = 5;
    
    % plot de tous les synthés dans l'ordre
    for i=(globalParams.trialSynthsCount + 1):(globalParams.presetsCount)
        subplot(subplotRowsCount, subplotColsCount, i - globalParams.trialSynthsCount);
        hold off;

        synthId = experiments(expeId).synthIndexesInAppearanceOrder(i);
        % besoin de savoir aussi si c'était un fader ou un interp qui a été
        % enregistré
        synthType = experiments(expeId).synthTypesInAppearanceOrder(i);
        if (synthId > 0)

            if ( experiments(expeId).isSynthRecorded(synthId, 1) ) && (synthType == 1)
                i2bis = 1;
                for paramId = 1:globalParams.parametersCount
                    plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                    hold on;
                end
                gridxy([], [0.0], 'Color','k','Linestyle','-.'); ylim(limitesOrdonnees);
                if (maxAbscisses > 0.0) % limite max en X si demandée
                    xlim( [0.0 ; maxAbscisses] );
                end
                title(strcat('F-', globalParams.synthNames(synthId)));
            elseif  (experiments(expeId).isSynthRecorded(synthId, 2) )  && (synthType == 2)
                i2bis = 2;
                for paramId = 1:globalParams.parametersCount
                    plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                    hold on;
                end
                gridxy([], [0.0], 'Color','k','Linestyle','-.'); ylim(limitesOrdonnees);
                if (maxAbscisses > 0.0) % limite max en X si demandée
                    xlim( [0.0 ; maxAbscisses] );
                end
                title(strcat('I-', globalParams.synthNames(synthId)));
            else
                title('No data');
            end

        else
            title('No data');
        end
    end

    
% - - - - sinon, on n'affiche que le graphe demandé - - - -
% (pas dans l'ordre chronologique, mais dans l'ordre réel
%       pour pouvoir comparer tous les sujets)
else
    % init à une valeur qui n'affiche rien
    synthId = -1;
    % petites valeurs : synthé d'essai
    if (graphPresetId == 1)
        synthId = 1;
        i2bis = 1;
    elseif (graphPresetId == 2)
        synthId = 2;
        i2bis = 2;
    % plus grandes valeurs : synthés réel de l'expérience
    else
        synthId = floor( (graphPresetId - globalParams.trialSynthsCount - 1) / 2 ) + globalParams.trialSynthsCount + 1;
        i2bis = mod( (graphPresetId - globalParams.trialSynthsCount - 1), 2) + 1;
    end
    
    if (synthId > 0)
            hold off;

        if ( experiments(expeId).isSynthRecorded(synthId, 1) && (i2bis == 1) )
            hold off;
            for paramId = 1:globalParams.parametersCount
                plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                hold on;
            end
            gridxy([], [0.0],'Color','k','Linestyle','-.'); ylim(limitesOrdonnees);
            if (maxAbscisses > 0.0) % limite max en X si demandée
                xlim( [0.0 ; maxAbscisses] );
            end
            title(strcat('F', globalParams.synthNames(synthId)));
            
        elseif  (experiments(expeId).isSynthRecorded(synthId, 2) && (i2bis == 2) ) 
            for paramId = 1:globalParams.parametersCount
                plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                hold on;
            end
            gridxy([], [0.0],'Color','k','Linestyle','-.'); ylim(limitesOrdonnees);
            if (maxAbscisses > 0.0) % limite max en X si demandée
                xlim( [0.0 ; maxAbscisses] );
            end
            title(strcat('I', globalParams.synthNames(synthId)));

        else
            title('No data');
        end

    else
        title('No data');
    end
    
   

end



