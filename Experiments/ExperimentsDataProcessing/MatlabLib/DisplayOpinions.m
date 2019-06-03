function [] = DisplayOpinions(globalParams, opinionResults)
%DISPLAYPREFERENCES Display the opinion (preferences) of all participants, 
% in a single plot

fig = figure;

% construction des abscisses et des légendes/étiquettes associées
%xBar = [1:12]';
xCategories = categorical({'Fastest', 'Most precise' ,'Most intuitive' , 'Preferred'});
% construction des valeurs de l'histogramme
% dans l'ordre demandé dans l'expé : fastest, most precise, most intuitive,
% preferred overall
% (4 groupes de 3)
yBar = [opinionResults.fader.fastestTotal opinionResults.none.fastestTotal opinionResults.interpolation.fastestTotal ;
        opinionResults.fader.preciseTotal opinionResults.none.preciseTotal opinionResults.interpolation.preciseTotal ;
        opinionResults.fader.intuitiveTotal opinionResults.none.intuitiveTotal opinionResults.interpolation.intuitiveTotal ;
        opinionResults.fader.preferredTotal opinionResults.none.preferredTotal opinionResults.interpolation.preferredTotal  ];

    % Utiliser FACECOLOR FLAT à combiner avec une colormap qui donne du 
    % bleu pour interp, du vert pour fader et du gris (ou autre... au
    % milieu)
barPlot = bar(xCategories, yBar);

legend('Fader', 'None', 'Interpolation');

% couleurs personnalisées
for i=1:12
    % fader
    if ( mod(i,3) == 1 )
        
    % none
    elseif ( mod(i,3) == 2 )
        
    % interp
    elseif ( mod(i,3) == 0 )
        barPlot.CData(i,:) = [1 0 0]; %globalParams.colors(1, :);
    end
end

end

