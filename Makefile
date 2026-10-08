# Compila ou limpa todos os projetos de uma vez
PROJETOS := calculadora agenda-contatos jogo-da-velha conversor-bases sistema-escolar compactador-rle

.PHONY: all clean $(PROJETOS)

all: $(PROJETOS)

$(PROJETOS):
	$(MAKE) -C $@

clean:
	for p in $(PROJETOS); do $(MAKE) -C $$p clean; done
