function [] = DisplayAgeAndSex(globalParams, experiments)
%DISPLAYAGEANDSEX Display the biological sex and age of participants
%   2d scatter plot


ageAndSexFigure = figure();
set(ageAndSexFigure, 'Position', [0 0 700 180]);

sameAgeGraphicDiff = 0.15;

% 1 - création des vecteurs de données (age en abscisse)
% pour une bonne représentation... On va décaler les âges qui existaient
% déjà (sinon le scatter plot ne sera pas bien représentatif)
subjectsI = 1; % actual counter
minAge = -1;
maxAge = -1;
for i = 1:globalParams.experimentsCount
    if (experiments(i).isValid == true)
         
        % ajout direct nouvelle valeur
        ages(subjectsI) = experiments(i).age;
        sexes(subjectsI) = experiments(i).sex;
        if (sexes(subjectsI) == 1)
            pointsColors(subjectsI) = 0.0;
        elseif (sexes(subjectsI) == 2)
            pointsColors(subjectsI) = 1.0;
        else
            pointsColors(subjectsI) = 0;
        end
        % test min/max (et initialisation si nécessaire)
        if (ages(subjectsI) < minAge) || (minAge == -1)
            minAge = ages(subjectsI);
        end
        if (ages(subjectsI) > maxAge) || (maxAge == -1)
            maxAge = ages(subjectsI);
        end
        
        
        if (subjectsI >= 2)
            % léger décalage de l'âge si besoin
            % -> on décalage de + 0.05 (+ sameAgeGraphicDiff)
            numberOfAgeDuplicates = 0;
            for checkI=1:(subjectsI-1)
                % si l'âge existait déjà... on augmente de nouveau max trouvé 
                % de 0.05
                % subjectsAge(i) reste ici un nombre entier pour l'instant
                if (ages(checkI) == ages(subjectsI)) && (sexes(checkI) == sexes(subjectsI))
                    numberOfAgeDuplicates = numberOfAgeDuplicates + 1;
                end
            end
            % on décale verticalement (selon le sexe...)
            if numberOfAgeDuplicates > 0
                sexes(subjectsI) = sexes(subjectsI) + numberOfAgeDuplicates * sameAgeGraphicDiff;
            end
        end
        
        % indice suivant
        subjectsI = subjectsI + 1;
    end
end

% 2 - plot avec colormap simple perso
colormap([0.8 0.2 0.3; 0.2 0.7 0.0] );
scatter(ages, sexes, [], pointsColors);
xlim( [(minAge-1) (maxAge+1)] );
ylim( [0.5 3] );
yticks( 0:3 );
yticklabels({' ', 'Male', 'Female', ' '});

title('Age and biological sex of subjects');

grid on;


end

