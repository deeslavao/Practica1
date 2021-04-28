busqueda:  
	make -j 2 buscador menu
.PHONY: busqueda

buscador:
	gcc buscador.c -o buscador
	./buscador &
.PHONY: buscador

menu:
	gcc menu.c -o menu
	./menu
.PHONY: menu 

