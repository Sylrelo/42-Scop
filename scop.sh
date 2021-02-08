gcc $(find . -iregex '.*\.c' -printf 'src/%f\n') -Iinclude -D_REENTRANT -lglfw -lm -lbsd -ldl -lpthread -fsanitize=address -O3 && ./a.out res/Logo\(3\).obj
