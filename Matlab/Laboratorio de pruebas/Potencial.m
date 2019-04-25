function pot=Potencial(x1,x2,A_POTENCIAL,thetha,ALFA_POTENCIAL,MEDIA_RUIDO,SIGMA_POTENCIAL)
%Funcion que calcula la funcion de potencial de un vector sobre otro
    
    format long
    %pot = (A_POTENCIAL/( norm(x1-x2)^ALFA_POTENCIAL) + normrnd(MEDIA_RUIDO,SIGMA_POTENCIAL) );
    pot = ( A_POTENCIAL * ( LobuloCos(abs(thetha(1)-thetha(2))) ) /( norm(x1-x2)^ALFA_POTENCIAL ) + normrnd(MEDIA_RUIDO,SIGMA_POTENCIAL) );

end