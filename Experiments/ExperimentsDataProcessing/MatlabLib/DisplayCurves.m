function [ ] = DisplayCurves( globalParams, experiments, expeData, expeId, graphSynthId )
%DISPLAYCURVES Summary of this function goes here
%   Detailed explanation goes here


% Sub-plot en 12 parties
%
%    essai1   essai2     synth a     synth b
%   synth c  etc....
%
% dans l'ordre chrono pour voir l'évolution (si évolution il y a)
figure('Name', strcat('Visualisation of experiment #', num2str(expeId)));

% Tableau des couleurs : chaque paramètre aura sa couleur. source couleurs 2014b http://math.loyola.edu/~loberbro/matlab/html/colorsInMatlab.html
couleurs = [0.0000 0.4470 0.7410 ;
            0.8500 0.3250 0.0980 ;
            0.9290 0.6940 0.1250 ;
            0.4940 0.1840 0.5560 ;
            0.4660 0.6740 0.1880 ;
            0.3010 0.7450 0.9330 ;
            0.6350 0.0780 0.1840 ];


% - - - - - Si graphID <= 0 : on affiche TOUT - - - - -

if (graphSynthId <= 0)
    % Plot des essais pour commencer
    subplot(3,4, 1);
    synthId = 1;
    if experiments(expeId).isSynthRecorded(synthId, 1)
        hold off;
        i2bis = 1;
        for paramId = 1:globalParams.parametersCount
            plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
            hold on;
        end
        gridxy([], [0.0],'Color','k','Linestyle','-.');
        title(strcat('F-', globalParams.synthNames(synthId)));
    else
        title('No data');
    end

    subplot(3,4, 2);
    synthId = 1;
    if experiments(expeId).isSynthRecorded(synthId, 2)
        hold off;
        i2bis = 2;
        for paramId = 1:globalParams.parametersCount
            plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
            hold on;
        end
        gridxy([], [0.0],'Color','k','Linestyle','-.');
        title(strcat('I-', globalParams.synthNames(synthId)));
    else
        title('No data');
    end

    % Ensuite, plot des 10 échantillons testés
    for i=3:12
        subplot(3, 4, i);
        hold off;

        synthId = experiments(expeId).synthIndexesInAppearanceOrder(i-1);
        if (synthId > 0)

            if ( experiments(expeId).isSynthRecorded(synthId, 1) ) 
                i2bis = 1;
                for paramId = 1:globalParams.parametersCount
                    plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                    hold on;
                end
                gridxy([], [0.0],'Color','k','Linestyle','-.');
                title(strcat('F-', globalParams.synthNames(synthId)));
            elseif  (experiments(expeId).isSynthRecorded(synthId, 2) ) 
                i2bis = 2;
                for paramId = 1:globalParams.parametersCount
                    plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                    hold on;
                end
                gridxy([], [0.0],'Color','k','Linestyle','-.');
                title(strcat('I-', globalParams.synthNames(synthId)));
            else
                title('No data');
            end

        else
            title('No data');
        end
    end

    
% - - - - sinon, on n'affiche que le graphe demandé - - - -
else

    synthId = experiments(expeId).synthIndexesInAppearanceOrder(graphSynthId);
    %synthId = graphSynthId;
    if (synthId > 0)
            hold off;

        if ( experiments(expeId).isSynthRecorded(synthId, 1) ) % valable pour tout param.... normalement.
            i2bis = 1;
            % Synth 1 est très spécial... c'est le test, donc il a le [F]
            % et le [I]
            if (synthId == 1)
                subplot(1,2,1);
            end
            
            hold off;
            for paramId = 1:globalParams.parametersCount
                plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                hold on;
            end
            gridxy([], [0.0],'Color','k','Linestyle','-.');
            title(strcat('F-', globalParams.synthNames(synthId)));
        else
            title('No data');
        end
        
        if  (experiments(expeId).isSynthRecorded(synthId, 2)) 
            i2bis = 2;
            
            if (synthId == 1)
                subplot(1,2,2);
            end
            
            for paramId = 1:globalParams.parametersCount
                plot(expeData{expeId, synthId, i2bis, paramId}(:, 1), expeData{expeId, synthId, i2bis, paramId}(:, 2));
                hold on;
            end
            gridxy([], [0.0],'Color','k','Linestyle','-.');
            title(strcat('I-', globalParams.synthNames(synthId)));
        else
            if (synthId == 1)
                title('No data');
            end
        end

    else
        title('No data');
    end
    
end



