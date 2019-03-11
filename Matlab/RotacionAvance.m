function robots = RotacionAvance(robots,DELTA_ROTACION,N,W,ALFA_POTENCIAL,DELTA_AVANCE,SIGMA_ROTACION)
%Funcion que realiza una iteracion del simulador para todos los robots

    format long
    for robot_actual=1:N
        tic
        
        
        thetha = [];
        thethaProm = 0;
        divprom = 0;
        for frecuencia=1:N
            
            thetha(frecuencia) = BarridoFrecuencia(robot_actual,frecuencia,robots,DELTA_ROTACION,ALFA_POTENCIAL,N,SIGMA_ROTACION);
        end
        for valnocero = 1:length(thetha)
            if thetha(valnocero) ~= 0
                divprom = divprom + W(valnocero);
            end
        end
        
        giro= 0;
        incerteza = robots(robot_actual).thetha;
        while giro<=(2*pi)
            incerteza = incerteza + normrnd(DELTA_ROTACION,SIGMA_ROTACION);
            giro = giro + DELTA_ROTACION;
        end
        if incerteza > (2*pi)
            while incerteza > (2*pi)
                incerteza = incerteza-(2*pi);
            end
        end
        if thetha < 0
            while thetha < 0
                thetha=thetha+(2*pi);
            end
        end
        thethaProm=dot(W,thetha)/divprom;
        robots(robot_actual).thetha = incerteza+(thethaProm-robots(robot_actual).thetha);
        estimado = incerteza+(thethaProm-robots(robot_actual).thetha);
        real = thethaProm;
        %robots(robot_actual).thetha = robots(robot_actual).thethaIncerteza+(thethaProm-robots(robot_actual).thethaIncerteza);
        if robots(robot_actual).clustered == 0 
            robots(robot_actual).posicionRect.x = robots(robot_actual).posicionRect.x+DELTA_AVANCE * cos(robots(robot_actual).thetha);
            robots(robot_actual).posicionRect.y = robots(robot_actual).posicionRect.y+DELTA_AVANCE * sin(robots(robot_actual).thetha);
            robots(robot_actual).pasos = robots(robot_actual).pasos+1;
            if robots(robot_actual).tiempo_estabilidad > 0
                robots(robot_actual).tiempo_estabilidad = robots(robot_actual).tiempo_estabilidad - 1;
            end
        end
        robots(robot_actual).tiempo = robots(robot_actual).tiempo + toc;
    end
end

