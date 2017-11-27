


bot = serial('COM9', 'BaudRate', 9600, 'Terminator', 'CR');
fopen(bot)
pause(1);
cam = webcam(2);
preview(cam)

fprintf(bot, 'UP');
pause(4);

fprintf(bot, 'CO');
pause(2);

fprintf(bot, 'DN');
pause(4);

fprintf(bot, 'CO');

fclose(bot);
clear('bot')