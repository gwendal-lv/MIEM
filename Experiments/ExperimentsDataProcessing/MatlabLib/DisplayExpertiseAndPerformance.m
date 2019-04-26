function [] = DisplayExpertiseAndPerformance(globalParams, expertiseLevels, correspondingPerfs)
%DISPLAYEXPERTISEANDPERFORMANCE Summary of this function goes here
%   Detailed explanation goes here

scatterPlotFigure = figure;

% faders
scatter(expertiseLevels, correspondingPerfs(:,1), [], globalParams.colors(1,:), 's');
% interpolation
hold on;
scatter(expertiseLevels, correspondingPerfs(:,2), [], globalParams.colors(2,:), 'd');

xlim( [0.0 6.0] );
xticks( 1:5 );
xlabel('Self-estimated expertise level in sound synthesis and filtering');

ylim( [0 1] );
ylabel('Mean performance $\bar{P(i)}$ (for all sounds of experiment i)', 'Interpreter', 'latex');

legend('Faders', 'Interpolation', 'Location', 'southeast');

title('Mean performance related to expertise level of subjects');

end

