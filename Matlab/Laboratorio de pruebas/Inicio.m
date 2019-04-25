%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Universidad Nacional Autónoma de México
%Instituto de Ciencias Nucleares
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Programa: COLMENA
%Director: Dr. Gustavo Adolfo Medina Tanco
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Simulador de formacion de clusters de elementos dispersos en un área 
%arbitraria (Laboratorio de pruebas)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Desarrollador: Ing. Led Eduardo Ruvalcaba Tamés
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Actualizado 18/03/19 1135 hrs UTC-6
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
clc;
close all;

format long

%Carga de parametros iniciales
[REPETICIONES,ITERACIONES,L,N,A_POTENCIAL,ALFA_POTENCIAL,R,DELTA_AVANCE,DELTA_ROTACION,W,MEDIA_RUIDO,SIGMA_POTENCIAL,SIGMA_ROTACION,R_COLISION,ACTIVAR_GRAFICOS,TAM_IDENTIFICADOR]=CargaParametros();
colores = ['c*';'m*';'y*';'m+';'g*';'b*';'y+';'r*';'c+'];

%Simulacion

robots= Inicializacion(N,L,R,A_POTENCIAL);
clusters = [];
[robots,colision]=RevisarColision(robots,h,l,R_COLISION);
                
    if ~colision
        if ACTIVAR_GRAFICOS
            [x,y]=GeneradorCircunferencia([robots(h).posicionRect.x,robots(h).posicionRect.y],[robots(l).posicionRect.x,robots(l).posicionRect.y]);
            hold on
            plot(x,y,'k');
            drawnow;
        end
    end
robots(1).posicionRect.x = robots(1).posicionRect.x+DELTA_AVANCE * cos(robots(1).thetha);
robots(1).posicionRect.y = robots(1).posicionRect.y+DELTA_AVANCE * sin(robots(1).thetha);

robots(1).posicionRect.x = robots(2).posicionRect.x+DELTA_AVANCE * cos(robots(2).thetha);
robots(1).posicionRect.y = robots(2).posicionRect.y+DELTA_AVANCE * sin(robots(2).thetha);
