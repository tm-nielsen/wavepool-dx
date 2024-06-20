@REM build for web
call em++ -o builds/web/wave_pool.html main.cpp -Os -Wall web_dependencies/lib/libraylib.a -Iweb_dependencies/include -L. -s USE_GLFW=3 -s ASYNCIFY --shell-file web_dependencies/minishell.html --embed-file resources -sALLOW_MEMORY_GROWTH -DPLATFORM_WEB

@REM push web build
cd builds/web
if exist wave_pool.html if exist index.html del index.html
if exist wave_pool.html ren wave_pool.html index.html

call butler push . klungore/wave-pool-dx:web

if exist index.html ren index.html wave_pool.html

@REM build for windows
cd ../..
call g++ -o builds/win64/wave_pool.exe main.cpp -lraylib -lgdi32 -lwinmm -mwindows

@REM push windows build
call butler push --ignore settings.txt builds/win64 klungore/wave-pool-dx:windows