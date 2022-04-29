#include <stdio.h>
#include <stdlib.h> /* Para usar calloc() */

int main() {
    int *p; 
    int n = 10; 
    int i; /* Determina o valor de n em algum lugar */
    
    p = calloc(n, sizeof(int));  /* Aloca n números inteiros p pode agora 
                                    ser tratado como um vetor com n posições */
    //p = malloc(n*sizeof(int)); /* Maneira equivalente usando malloc */
    if(!p) {
        printf("** Erro: Memoria Insuficiente **");
        exit(0);
    }

    for(i = 0; i < n; i++) { /* p pode ser tratado como um vetor 
                                com n posições */
        p[i] = i * i;
        printf("%d\n", p[i]);
    }

    return 0;
}

