function pot=Potencial(x1,x2,A_POTENCIAL,ALFA_POTENCIAL,MEDIA_RUIDO,SIGMA_POTENCIAL)
%Funcion que calcula la funcion de potencial de un vector sobre otro
    
    format long
    P=normrnd(MEDIA_RUIDO,SIGMA_POTENCIAL)
    pot = (A_POTENCIAL/( norm(x1-x2)^ALFA_POTENCIAL) + normrnd(MEDIA_RUIDO,SIGMA_POTENCIAL) );

end