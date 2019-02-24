function [ outputMatrix ] = ExtendFirstAndLastValues( inputMatrix, lastTime )
%DUPLICATEFIRSTVALUETOTIMEZERO Summary of this function goes here
%   Detailed explanation goes here

% On met la première ligne avec la même deuxieme colonne, 
% mais en première colonne on force le temps zéro (si ça
% n'est pas déjà le cas)
if (inputMatrix(1,1) == 0)
    outputMatrix = inputMatrix;
else
    outputMatrix = [ 0 inputMatrix(1,2) ; inputMatrix];
end

% Last time : à déplacer si besoin
% (on ne sait pas si c'est ce jeu de données qui a le plus long temps
% d'expérience)
if (inputMatrix(end,1) < lastTime)
    outputMatrix = [ outputMatrix ; lastTime outputMatrix(end,2) ];
else
    % rien
end


end

