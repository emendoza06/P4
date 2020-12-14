all: compfs 

compfs: 
	gcc -Wall tree.c node.c token.c main.c parser.c scanner.c testtree.c staticsemantics.c codeGen.c -o compfs

clean:
	rm compfs  
	rm *.asm
