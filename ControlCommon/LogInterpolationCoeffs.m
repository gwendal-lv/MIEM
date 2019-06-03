function [a, b, c] = LogInterpolationCoeffs(minY, maxY)
%LOGINTERPOLATIONCOEFFS Computes the coeffs for a log-curve interpolation
% function of form : y = a.exp(b.x) + c
% considering that x is a normalized input, and
% for input y values given at points x=0, x=0.5, x=1

% default values for illegal inputs
a = 0;
b = 0;
c = 0;

% Interpolation log : il faut faire un peu d'analyse et de calcul. Résolution qui fonctionne pour une interpolation croissante uniquement
% On recherche a, b et c tels que (y - minY) = a.exp(bx) + c, avec nécessairement b>0, et immédiatement a = -c pour la correspondance en 0
% On va passer d'abord par la résolution d'une équation du second degré en B = exp(b)
%
% REMARQUE : le code généré n'a plus forcément de sens "physique" 
% à l'intérieur, car Matlab effectue des optimisations (variables
% temporaires supprimées, boucles modifiées, etc...)

y1 = 0; 
y2 = maxY - minY;

% ok, calcul cohérent avec l'affichage eq8 dans Live 10
nombreDecades = log10(maxY / minY)
centerYlog = minY * (10 ^(nombreDecades / 2))
ym = centerYlog - minY;

a_eq2 = ym*ym;
b_eq2 = -ym*ym - (ym - y2)*(ym - y2);
c_eq2 = (ym - y2) * (ym - y2);
delta_eq2 = b_eq2*b_eq2 - 4*a_eq2*c_eq2
% warning n'est pas compatible avec Matlab Codegen (mettre assert() dans le
% code C final)
if (delta_eq2 < 0)
    disp('Pas de solution réelle, renvoi de zéros...')
else
    % racines en vecteur colonne
    B_roots = [(- b_eq2 - sqrt(delta_eq2)) / (2*a_eq2) ;
              (- b_eq2 + sqrt(delta_eq2)) / (2*a_eq2) ];

    % Il est probable que les 2 racines soient valables. On doit calculer les
    % a,b,c jusqu'au bout puis vérifier l'application de l'équation avec une
    % marge d'erreur (qui réduit)
    a_test = zeros(2, 1);
    b_test = zeros(2, 1);
    c_test = zeros(2, 1);
    % - - - - - Calcul des coefficients a, b, c - - - - -
    for i=1:2
        b_test(i) = log( B_roots(i) ); 
        a_test(i) = y2 / (B_roots(i) - 1);
        c_test(i) = -a_test(i);
    end
    % - - - - - Test - - - - -
    numberOk = int32(2);
    validCoeffsIndex = int32(-1);
    iterationsCount = int32(0);
    errorMargin = (maxY - minY)/10; % fera 5% d'erreur pour première itération (division par 2 à chaque fois
    % on sortira de la boucle si 0 ou 1 valeur ne sont pas OK...
    while (numberOk > 1) && (iterationsCount < 20)
        numberOk = int32(0);
        errorMargin = errorMargin / 2.0;
        % pour chaque jeu de réponses, on vérifie les 3 points qui ont
        % servi à construire les systèmes d'équation
        for i=1:2
            % always true at the moment...
            errorNear0 = abs(a_test(i) + c_test(i));
            errorNearOneHalf = abs(a_test(i) * exp(b_test(i)*0.5) + c_test(i) - ym);
            errorNear1 = abs(a_test(i) * exp(b_test(i)*1.0) + c_test(i) - y2);
            if (errorNear0 < errorMargin) && (errorNearOneHalf < errorMargin) && (errorNear1 < errorMargin)
                validCoeffsIndex = int32(i);
                numberOk = numberOk + int32(1);
            end
        end
        iterationsCount = iterationsCount + 1;
    end
    % écriture des bons coeffs, si OK
    if numberOk == 1
        a = a_test(validCoeffsIndex);
        b = b_test(validCoeffsIndex);
        c = c_test(validCoeffsIndex) + minY;
    else % sinon on prend les premiers venus...
        a = a_test(1);
        b = b_test(1);
        c = c_test(1) + minY;
    end
end

end

