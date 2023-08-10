extern long int* alocaMem();
extern void iniciaAlocador();
extern void liberaMem();
extern void finalizaAlocador();
extern void imprimeMapa();

int main() {
  void *a, *b;
  iniciaAlocador();
  imprimeMapa();

  a=alocaMem(32);
  imprimeMapa();

  b=alocaMem(50);
  imprimeMapa();

  liberaMem(a);
  imprimeMapa();

  a=alocaMem(15);
  imprimeMapa();

  finalizaAlocador();
  imprimeMapa();
}
