CC=gcc
CFLAGS1=-Wall -c -g 
CFLAGS2= -lpthread
OBJS=sym_main.o sym_global.o sym_pthread_zigbee_rcv.o   sym_pthread_main.o sym_pthread_camera.o  sym_pthread_refresh.o  sym_pthread_uart_send.o sym_uart_cache.o 

zigbee :$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS2)
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS1) $< -o $@

.PHONY:clean
clean:
	rm *.o zigbee

