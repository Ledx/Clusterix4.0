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

robot_lejano=1;
robot_referencia=2;

%Carga de parametros iniciales
[REPETICIONES,ITERACIONES,L,N,A_POTENCIAL,ALFA_POTENCIAL,R,DELTA_AVANCE,DELTA_ROTACION,W,MEDIA_RUIDO,SIGMA_POTENCIAL,SIGMA_ROTACION,R_COLISION,ACTIVAR_GRAFICOS,TAM_IDENTIFICADOR]=CargaParametros();
colores = ['c*';'m*';'y*';'m+';'g*';'b*';'y+';'r*';'c+'];

%Simulacion
%https://stackoverrun.com/es/q/7057547
robots= Inicializacion(N,L,R,A_POTENCIAL);
%alfa = atan2(robots(robot_lejano).posicionRect.y-robots(robot_referencia).posicionRect.y,robots(robot_lejano).posicionRect.x-robots(robot_referencia).posicionRect.x)

% [xf yf]=ds2nfu(x,y); 
% annotation(gcf,'arrow', xf,yf)
% [robots,colision]=RevisarColision(robots,h,l,R_COLISION);
                
% [x,y]=GeneradorCircunferencia([robots(h).posicionRect.x,robots(h).posicionRect.y],[robots(l).posicionRect.x,robots(l).posicionRect.y]);
% hold on
% plot(x,y,'k');
% drawnow;

% robots(1).posicionRect.x = robots(1).posicionRect.x+DELTA_AVANCE * cos(robots(1).thetha);
% robots(1).posicionRect.y = robots(1).posicionRect.y+DELTA_AVANCE * sin(robots(1).thetha);
% 
% robots(1).posicionRect.x = robots(2).posicionRect.x+DELTA_AVANCE * cos(robots(2).thetha);
% robots(1).posicionRect.y = robots(2).posicionRect.y+DELTA_AVANCE * sin(robots(2).thetha);
% 

figure(1)


r = 15; 
u = r * cos(robots(1).thetha); % convert polar (theta,r) to cartesian
v = r * sin(robots(1).thetha);
h = quiver(robots(1).posicionRect.x,robots(1).posicionRect.y,u,v);
[x,y]=DIB_Circunferencia([robots(1).posicionRect.x,robots(1).posicionRect.y],R);
plot(x,y,'k');



grid
axis([0  300    0  300])
hold on


%clf



