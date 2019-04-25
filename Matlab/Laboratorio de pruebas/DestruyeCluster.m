function robots = DestruyeCluster(robots,N,robot_actual)
%Destruye un cluster
    
    if RevisaCluster(robots,N,robot_actual) == 1
        robots(robot_actual).clustered = 0;
        robots(robot_actual).vecindad = zeros(N);
        robots(robot_actual).clase = 1;
        robots(robot_actual).tiempo_estabilidad = 20;
        
        for i=1:N
            if robots(robot_actual).vecindad(i) == 1
                robots(i).vecindad(robot_actual) = 0;
                robots(robot_actual).clase = robots(robot_actual).clase - 1;
            end
        end
    end
 
end

