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
%Actualizado 18/03/19 1135 hrs UTC-6
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
clc;
close all;

format long

%Carga de parametros iniciales
[REPETICIONES,ITERACIONES,L,N,A_POTENCIAL,ALFA_POTENCIAL,R,DELTA_AVANCE,DELTA_ROTACION,W,MEDIA_RUIDO,SIGMA_POTENCIAL,SIGMA_ROTACION,R_COLISION,ACTIVAR_GRAFICOS,TAM_IDENTIFICADOR]=CargaParametros();
colores = ['c*';'m*';'y*';'m+';'g*';'b*';'y+';'r*';'c+'];
%Creacion de manejadores de archivos
identificador = strcat(CreaIdentificador(TAM_IDENTIFICADOR),'-',date);
api=CreaArchivo(identificador,1);
his=CreaArchivo(identificador,2);
clu=CreaArchivo(identificador,3);
tie=CreaArchivo(identificador,4);
fprintf(api,'REPETICIONES = %d',REPETICIONES);
fprintf(api,'\n');
fprintf(api,'ITERACIONES = %d',ITERACIONES); 
fprintf(api,'\n');
fprintf(api,'L = %d',L);
fprintf(api,'\n');
fprintf(api,'N = %d',N);
fprintf(api,'\n');
fprintf(api,'A_POTENCIAL = ');
fprintf(api,'%d ',A_POTENCIAL); 
fprintf(api,'\n');
fprintf(api,'ALFA_POTENCIAL = %d',ALFA_POTENCIAL);
fprintf(api,'\n');
fprintf(api,'R = %d',R);
fprintf(api,'\n');
fprintf(api,'DELTA_AVANCE = %d',DELTA_AVANCE); 
fprintf(api,'\n');
fprintf(api,'DELTA_ROTACION = %d',DELTA_ROTACION);
fprintf(api,'\n');
fprintf(api,'W = ');
fprintf(api,'%d ',W);
fprintf(api,'\n');
fprintf(api,'SIGMA_ROTACION = %d',SIGMA_ROTACION);
fprintf(api,'\n');
fprintf(api,'R_COLISION = %d',R_COLISION);
fprintf(api,'\n');
fprintf(api,'ACTIVAR_GRAFICOS = %d',ACTIVAR_GRAFICOS);
fprintf(api,'\n');
fprintf(api,'TAM_IDENTIFICADOR = %d',TAM_IDENTIFICADOR); 

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
                    r = 15; 
                    u = r * cos(robots(i).thetha); 
                    v = r * sin(robots(i).thetha);
                    h = quiver(robots(i).posicionRect.x,robots(i).posicionRect.y,u,v);
                    [x,y]=DIB_Circunferencia([robots(i).posicionRect.x,robots(i).posicionRect.y],R);
                    plot(x,y,'k');
%                     plot(robots(i).posicionRect.x,robots(i).posicionRect.y,colores(i,:));
                    drawnow;
                end
            else
                for i=1:N
                    hold on
                    r = 15; 
                    u = r * cos(robots(i).thetha); 
                    v = r * sin(robots(i).thetha);
                    h = quiver(robots(i).posicionRect.x,robots(i).posicionRect.y,u,v);
                    [x,y]=DIB_Circunferencia([robots(i).posicionRect.x,robots(i).posicionRect.y],R);
                    plot(x,y,'k');
%                     plot(robots(i).posicionRect.x,robots(i).posicionRect.y,'ko');
                    drawnow;
                end
            end
        end
        robots=RotacionAvance(robots,DELTA_ROTACION,N,W,ALFA_POTENCIAL,DELTA_AVANCE,SIGMA_ROTACION,MEDIA_RUIDO,SIGMA_POTENCIAL);
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
fclose(api);
fclose(his);
fclose(clu);
fclose(tie);