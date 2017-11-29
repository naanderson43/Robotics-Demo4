clear all
clc

% Connect to Arduino
bot = serial('COM9', 'BaudRate', 9600, 'Terminator', 'CR');
fopen(bot)

% Initialize webcam and open preview
cam = webcam(2);
set(cam, 'Brightness', 64)
preview(cam);

% Tolerances for centering the object centroid
% in the camera frame
TOL1 = 60;
TOL2 = 10;

% Main program
% Finds the red centroid in webcam snapshots and 
% sends commands to the arduino to keep the red
% centroid centered in the camera frame
while 1
    % Initialize variables
    rowCent = 0; % Centroid row coordinate
    colCent = 0; % Centroid column coordinate
    c = 0;       % Red pixel count
    
    
    % Take snapshot
    img = snapshot(cam);
    
    % For each pixel in the snapshot:
    for i = 1 : 480
        for j = 1 : 640
        
            % Get the RGB values
            r = img(i, j, 1);
            g = img(i, j, 2);
            b = img(i, j, 3);
            
            % If the pixel is red:
            if r > 180
                if g < 128
                    if b < 128
                        % Sum the row coordinates of the red pixels
                        rowCent = rowCent + i;
                        
                        % Sum the column coordinates of the red pixels
                        colCent = colCent + j;
                        
                        % count the red pixels
                        c = c + 1;
                    end
                end
            end
        end
    end
    
    % End program if there are less than 100 red pixels in the snapshot
    if c < 100
        break
    end
    
    % Average the coordinates of the red pixels to find the centroid
    rowCent = rowCent / c;
    colCent = colCent / c;
    
    % Display the snapshot with the centroid marked
    img = insertMarker(img,[colCent rowCent],'*','color','black','size',10);
    imshow(img)
    hold on
    
    % If the red centoid is above the image center,
    % send the "up" command to the Arduino
    if rowCent < ((480 / 2) - TOL1)
        fprintf(bot, 'UP');
    
    % Else, if the red centoid is below the image center,
    % send the "down" command to the Arduino
    elseif rowCent > ((480 / 2) + TOL1)
        fprintf(bot, 'DN');
    
    % Else, send the "lift stop" command to the Arduino
    else
        fprintf(bot, 'LS');
    end
    
    % If the red centoid is left of the image center,
    % send the "left" command to the Arduino
    if colCent < ((640 / 2) - (TOL1 +TOL2))
        fprintf(bot, 'LT');
    
    % Else, if the red centoid is right of the image center,
    % send the "right" command to the Arduino
    elseif colCent > ((640 / 2) + (TOL1 + TOL2))
        fprintf(bot, 'RT');
    
    % Else, send the "rotate stop" command to the Arduino
    else
        fprintf(bot, 'RS');
    end
end

% When the main program exits:

% Send the "clear outputs" command to the Arduino
fprintf(bot, 'CO');

% Close the connection to the Arduino
fclose(bot);
clear('bot')
