

gcc -Wall -Werror -o testmacro macro.c usbservice.c logger.c command.c stack.c regset.c -D_DEFAULTMACRO_FRUITBUY10 -DPRINTLOG -DTESTMACRO -I./
echo 'Compile finished.. about to execute...'
sleep 2
./testmacro
rm ./testmacro
