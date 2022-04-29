#include <stdio.h>
#include <stdlib.h>
   
int main() {
   /* ponteiro para memória que será alocada */
   int *p;
   int i;
   
   /* alocar 10 elementos inteiros, ou seja, ( sizeof (int) * 10 ) */
   p = (int *) malloc (sizeof (int) * 10);
         
   if (p == NULL) {
      printf ("Erro: Não foi possivel alocar memória\n");
      exit(1);
   }
      
   for(i = 0; i < 10; i++) {
      p[i] = i * 2;
      printf ("%d\n", p[i]);
   }
   
   /* libera a memória alocada por malloc */
   free (p);
   
   return 0;
}

