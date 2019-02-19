function [ outputMatrix ] = DuplicateFirstValueToTimeZero( inputMatrix )
%DUPLICATEFIRSTVALUETOTIMEZERO Summary of this function goes here
%   Detailed explanation goes here

% On met la premiere ligne avec la meme deuxieme colonne, 
% mais en premiere colonne on force le temps zero (si ca
% n'est pas deja le cas)
if (inputMatrix(1,1) == 0)
    outputMatrix = inputMatrix;
else
    outputMatrix = [ 0 inputMatrix(1,2) ; inputMatrix];
end

end

