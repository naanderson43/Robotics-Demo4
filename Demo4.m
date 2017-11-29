clear all
clc


bot = serial('COM9', 'BaudRate', 9600, 'Terminator', 'CR');
fopen(bot)
cam = webcam(2);
set(cam, 'Brightness', 64)
preview(cam);

%fprintf(bot, 'UP');
%pause(4);

%fprintf(bot, 'CO');
%pause(2);

%fprintf(bot, 'DN');
%pause(3.5);

%fprintf(bot, 'CO');

pause(5)

TOL1 = 60;
TOL2 = 10;

while 1
    img = snapshot(cam);
    
    rowCent = 0;
    colCent = 0;
    c = 0;
    
    for i = 1 : 480
        for j = 1 : 640
            r = img(i, j, 1);
            g = img(i, j, 2);
            b = img(i, j, 3);
            
            if r > 180
                if g < 128
                    if b < 128
                        rowCent = rowCent + i;
                        colCent = colCent + j;
                        c = c + 1;
                    end
                end
            end
        end
    end
    
    if c < 100
        break
    end
    
    rowCent = rowCent / c;
    colCent = colCent / c;
    
    img = insertMarker(img,[colCent rowCent],'*','color','black','size',10);
    imshow(img)
    hold on
    
    if rowCent < ((480 / 2) - TOL1)
        fprintf(bot, 'UP');
    
    elseif rowCent > ((480 / 2) + TOL1)
        fprintf(bot, 'DN');
        
    else
        fprintf(bot, 'LS');
    end
    
    if colCent < ((640 / 2) - (TOL1 +TOL2))
        fprintf(bot, 'LT');
    
    elseif colCent > ((640 / 2) + (TOL1 + TOL2))
        fprintf(bot, 'RT');
        
    else
        fprintf(bot, 'RS');
    end
end

fprintf(bot, 'CO');
fclose(bot);
clear('bot')