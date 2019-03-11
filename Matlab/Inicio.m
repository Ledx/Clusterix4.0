%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Universidad Nacional Autónoma de México
%Instituto de Ciencias Nucleares
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Programa: COLMENA
%Director: Dr. Gustavo Adolfo Medina Tanco
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Simulador de formacion de clusters de elementos dispersos en un área 
%arbitraria
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Desarrollador: Ing. Led Eduardo Ruvalcaba Tamés
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Actualizado 22/08/18 1135 hrs UTC-6
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
clc;
close all;

format long

%Carga de parametros iniciales
[REPETICIONES,ITERACIONES,L,N,A_POTENCIAL,ALFA_POTENCIAL,R,DELTA_AVANCE,DELTA_ROTACION,W,SIGMA_ROTACION,R_COLISION,ACTIVAR_GRAFICOS,TAM_IDENTIFICADOR]=CargaParametros();
colores = ['c*';'m*';'y*';'m+';'g*';'b*';'y+';'r*';'c+'];
%Creacion de manejadores de archivos
identificador = strcat(CreaIdentificador(TAM_IDENTIFICADOR),'-',date);
% api=CreaArchivo(identificador,1);
his=CreaArchivo(identificador,2);
clu=CreaArchivo(identificador,3);
tie=CreaArchivo(identificador,4);
%fprintf(api,'%d \n',[REPETICIONES,ITERACIONES,L,N,A_POTENCIAL,ALFA_POTENCIAL,R,DELTA_AVANCE,DELTA_ROTACION,W,SIGMA_ROTACION,R_COLISION,ACTIVAR_GRAFICOS,TAM_IDENTIFICADOR]); 
%Simulacion
ci=1;
cr=1;
histograma = zeros(N,1);
while cr<REPETICIONES + 1
    robots= Inicializacion(N,L,R,A_POTENCIAL);
    %load('robots','-mat')
    clusters = [];

    while ci<ITERACIONES + 1 && Exito(robots,N)
        %Revision de colisiones entre robots
        for h=1:N
            for l=1:N
                [robots,colision]=RevisarColision(robots,h,l,R_COLISION);
                
                if ~colision
                    if ACTIVAR_GRAFICOS
                        [x,y]=GeneradorCircunferencia([robots(h).posicionRect.x,robots(h).posicionRect.y],[robots(l).posicionRect.x,robots(l).posicionRect.y]);
                        hold on
                        plot(x,y,'k');
                        drawnow;
                    end
                end
            end
        end
        for h=1:N
            if RevisaCluster(robots,N,h) == 1
                robots = DestruyeCluster(robots,N,h);
            end
        end
        robots = AsignadorClase(robots,N);
        if ACTIVAR_GRAFICOS
            if ci > 1
                for i=1:N
                    hold on
                    plot(robots(i).posicionRect.x,robots(i).posicionRect.y,colores(i,:));
                    drawnow;
                end
            else
                for i=1:N
                    hold on
                    plot(robots(i).posicionRect.x,robots(i).posicionRect.y,'ko');
                    drawnow;
                end
            end
        end
        robots=RotacionAvance(robots,DELTA_ROTACION,N,W,ALFA_POTENCIAL,DELTA_AVANCE,SIGMA_ROTACION);
        ci=ci+1;
    end
    if ACTIVAR_GRAFICOS
        for i=1:N
            hold on
            plot(robots(i).posicionRect.x,robots(i).posicionRect.y,'k*');
            drawnow;
        end
        hold off
    end
    clusters=ConteoClusters(robots,N);
    fprintf(clu,'%d ',clusters);
    fprintf(clu,'%d\n',ci);
    fprintf(tie,'%d ',ci);
    histograma = histograma + clusters;
    cr=cr+1;
    ci=1;
    if ACTIVAR_GRAFICOS
        figure(cr)
        axis([0 L 0 L]);
        grid
    end
end

fprintf(his,'%d ',histograma);
fprintf(his,'\n');
% fclose(api);
fclose(his);
fclose(clu);
fclose(tie);