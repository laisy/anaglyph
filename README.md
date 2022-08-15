# Anaglyph
Aplicação com Visualização 3D por Anaglifo

 REALIDADE VIRTUAL E AUMENTADA 2021.1
   LAISY CRISTINA
   ANAGLYPH

COMANDOS LINUX

gcc anaglyph.c -o anaglyph -lGL -lglut -lm -lGLU -lOpenGL
ps aux | rg Xorg   
./anaglyph   

OBS: View Gnome sobre Xorg

A estratégia é: 
-gerar duas instancias de visualização do objeto: um na cor vermelha e outro na cor azul.
-Transladar os objetos em sentido opostos a fim de encontrar o valor da paralaxe ideal.
-Rotacionar os objetos em sentido opostos (um horário (angulo positivo) e outro antihorário (angulo negativo)).

