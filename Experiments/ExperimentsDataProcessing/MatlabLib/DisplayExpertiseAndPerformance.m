function [] = DisplayExpertiseAndPerformance(globalParams, expertiseLevels, correspondingPerfs)
%DISPLAYEXPERTISEANDPERFORMANCE Summary of this function goes here
%   Detailed explanation goes here

scatterPlotFigure = figure;



% - - - calcul des régressions linéaires - - - 
% (les meilleures ont déjà été identifiées via la toolbox "curve fitting")
% gof = Goodness Of Fit
[fitFaders, gofInterp] = fit(expertiseLevels, correspondingPerfs(:,1), 'poly3');
[fitInterp, gofInterp] = fit(expertiseLevels, correspondingPerfs(:,2), 'poly2');




% - - - affichage - - - 

% faders
%scatter(expertiseLevels, correspondingPerfs(:,1), [], globalParams.colors(1,:), 's');
lastPlot = plot( fitFaders, expertiseLevels, correspondingPerfs(:,1), 'predobs');
lastPlot(1).MarkerEdgeColor = globalParams.colors(1,:);
lastPlot(1).MarkerSize = 12; % données points
lastPlot(2).Color = globalParams.colors(3,:); % régression
lastPlot(3).Color = globalParams.colors(1,:); % intervalle
lastPlot(4).Color = globalParams.colors(1,:);

% interpolation
hold on;
%scatter(expertiseLevels, correspondingPerfs(:,2), [], globalParams.colors(2,:), 'd');
lastPlot = plot( fitInterp, expertiseLevels, correspondingPerfs(:,2), 'predobs');
lastPlot(1).MarkerEdgeColor = globalParams.colors(2,:);
lastPlot(1).MarkerSize = 12; % données point
lastPlot(2).Color = globalParams.colors(4,:); % régression
lastPlot(3).Color = globalParams.colors(2,:); % intervalle
lastPlot(4).Color = globalParams.colors(2,:);

xlim( [0.0 6.0] );
xticks( 1:5 );
xlabel('Self-estimated expertise level in sound synthesis and filtering');

ylim( [0 1] );
ylabel('Mean P for all sounds of experiment i');

%legend('Faders', 'Interpolation', 'Location', 'southeast');
legend('Faders perfs', 'Quadratic regression', '95% prediction bounds', '', 'Interp perfs', 'Linear regression',  '95% prediction bounds', '', 'Location', 'southeast');

title('Mean performance related to expertise level of subjects');

end

