function robots = Inicializacion(N,L,R,A_POTENCIAL)
%Carga de los parametros iniciales de entrada
    format long
    
    for i=1:N
        
        robot.thetha = 0 + ((2*pi)-0)*rand(1,1);
        robot.radio = R;
        robot.constante_potencial = A_POTENCIAL(1) + (A_POTENCIAL(2)-A_POTENCIAL(1))*rand(1,1);
        robot.posicionRect.x = 0 + (L-0)*rand(1,1);
        robot.posicionRect.y = 0 + (L-0)*rand(1,1);
        robot.thethaInicial = robot.thetha;
        robot.thethaIncerteza = 0;
        robot.pasos = 0;
        robot.clustered = 0;
        robot.vecindad = zeros(N,1);
        robot.vecindad(i) = 1;
        robot.clase = 1;
        robot.tiempo = 0;
        robot.distancia_recorrida = 0;
        robot.tiempo_estabilidad = 0;
        
        robots(i)=robot;
    end
end

