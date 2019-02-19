function [ ] = DisplayCurves( globalParams, experiments, expeData, expeId )
%DISPLAYCURVES Summary of this function goes here
%   Detailed explanation goes here


% Sub-plot en 12 parties
%
%    essai1   essai2     synth a     synth b
%   synth c  etc....
%
% dans l'ordre chrono pour voir l'?volution (si ?volution il y a)
figure('Name', strcat('Visualisation of experiment #', num2str(expeId)));


% Plot des essais pour commencer
subplot(3,4, 1);
synthId = 1;
if experiments(expeId).isFaderSynthRecorded(synthId)
    hold off;
    if (~isempty(expeData{expeId, synthId, 1, 1})) % valable pour tout param....
        for paramId = 1:globalParams.parametersCount
            plot(expeData{expeId, synthId, 1, paramId}(:, 1), expeData{expeId, synthId, 1, paramId}(:, 2));
            hold on;
        end
        title(strcat('Faders - ', globalParams.synthNames(synthId)));
    else
        title('Unvalid data');
    end
end

subplot(3,4, 2);
synthId = 1;
if experiments(expeId).isInterpSynthRecorded(synthId)
    hold off;
    if (~isempty(expeData{expeId, synthId, 2, 1})) % valable pour tout param.... normalement.
        for paramId = 1:globalParams.parametersCount
            plot(expeData{expeId, synthId, 2, paramId}(:, 1), expeData{expeId, synthId, 2, paramId}(:, 2));
            hold on;
        end
        title(strcat('Interp - ', globalParams.synthNames(synthId)));
    end
end

% Ensuite, plot des 10 ?chantillons testes
for i=3:12
    subplot(3, 4, i);
    hold off;
    
    synthId = experiments(expeId).synthIndexesInAppearanceOrder(i-1);
    if (synthId > 0)

        if ( ~ isempty(expeData{expeId, synthId, 1, paramId}) )
            for paramId = 1:globalParams.parametersCount
                plot(expeData{expeId, synthId, 1, paramId}(:, 1), expeData{expeId, synthId, 1, paramId}(:, 2));
                hold on;
            end
            title('Fader - ');
        elseif  (~ isempty(expeData{expeId, synthId, 2, paramId}) )
            for paramId = 1:globalParams.parametersCount
                plot(expeData{expeId, synthId, 2, paramId}(:, 1), expeData{expeId, synthId, 2, paramId}(:, 2));
                hold on;
            end
            title('Interp - ');
        else
            title('Unvalid data');
        end

    else
        title('Unvalid data');
    end
end

end

