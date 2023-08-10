import java.util.*;

/*
 * TIPO NORMAL = 0
 * TIPO PRIVADO = 1
 * TIPO OCULTO = 2
 */

class Setor {

    // atributos
    protected ArrayList<Inimigo> inimigos;
    protected int tipo;
    protected int i;
    protected int j;
    protected boolean visitado;
    protected int portaCima;
    protected int portaBaixo;
    protected int portaEsquerda;
    protected int portaDireita;

    // construtor
    public Setor(ArrayList<Inimigo> inimigos) {
        this.tipo = -1;
        this.i = -1;
        this.j = -1;
        this.visitado = false;
        this.portaBaixo = 0;
        this.portaCima = 0;
        this.portaEsquerda = 0;
        this.portaDireita = 0;
        this.inimigos = inimigos;
    }

    /* métodos get/set */
    public int getI() {
        return this.i;
    }

    public void setI(int i) {
        if(i >= 0 && i < 5)
            this.i = i;
    }

    public int getJ() {
        return this.j;
    }

    public void setJ(int j) {
        if(j >= 0 && j < 5)
            this.j = j;
    }

    public int getPortaCima() {
        return this.portaCima;
    }

    public void setPortaCima(int portaCima) {
        if(portaCima == 0 || portaCima == 1)
            this.portaCima = portaCima;
    }

    public int getPortaBaixo() {
        return this.portaBaixo;
    }

    public void setPortaBaixo(int portaBaixo) {
        if(portaBaixo == 0 || portaBaixo == 1)
            this.portaBaixo = portaBaixo;
    }

    public int getPortaDireita() {
        return this.portaDireita;
    }

    public void setPortaDireita(int portaDireita) {
        if(portaDireita == 0 || portaDireita == 1)
            this.portaDireita = portaDireita;
    }

    public int getPortaEsquerda() {
        return this.portaCima;
    }

    public void setPortaEsquerda(int portaEsquerda) {
        if(portaEsquerda == 0 || portaEsquerda == 1)
            this.portaEsquerda = portaEsquerda;
    }

    public boolean getVisitado() {
        return this.visitado;
    }

    public void setVisitado(boolean visitado) {
            this.visitado = visitado;
    }

    /*
     * TIPO NORMAL = i+j -> número primo
     * TIPO PRIVADO = i+j -> não primo e par
     * TIPO OCULTO = i+j -> não primo e [impar]
     */
    public void setTipo(int i, int j) {
        if ((i + j + 2 == 2) || (i + j + 2 == 3) || (i + j + 2 == 5) || (i + j + 2 == 7) || (i == 2 && j == 2))
            this.tipo = 0;
        else if ((i + j + 2) % 2 == 0)
            this.tipo = 1;
        else if ((i + j + 2) % 2 != 0)
            this.tipo = 2;
    }

    public int getTipo() {
        return this.tipo;
    }

    public void setPosicao(int i, int j) {
        if (i >= 0 && i < 5)
            this.i = i;
        if (j >= 0 && i < 5)
            this.j = j;
    }

    public void gerarInimigos(ArrayList<Inimigo> inimigos, int x, int y) {

        if(x == 2 && y == 2)
            return;

        Random rand = new Random();
        int num = (rand.nextInt(3)) + 1;

        for (int i = 0; i < num; i++) {
            Inimigo ini = new Inimigo(x, y);
            inimigos.add(ini);
        }
        
    }

    /* gera portas de um determinado setor */
    public void gerarPortas() {

        //TIPO NORMAL(0) = maximo disponivel
        if (this.tipo == 0) {
            if (this.i == 0)
                this.portaBaixo = 1;
            else if (this.i == 4)
                this.portaCima = 1;
            else {
                this.portaBaixo = 1;
                this.portaCima = 1;
            }

            if (this.j == 0)
                this.portaDireita = 1;
            else if (this.j == 4)
                this.portaEsquerda = 1;
            else {
                this.portaDireita = 1;
                this.portaEsquerda = 1;
            }
        }

        //TIPO PRIVADO(1) = 1 ou 2 portas
        if (this.tipo == 1) {
            if (this.i == 0)
                this.portaBaixo = 1;
            else if (this.i == 4)
                this.portaCima = 1;
            else if (this.i % 2 == 0)
                this.portaCima = 1;
            else if (this.i % 2 != 0)
                this.portaBaixo = 1;

            if (this.j == 0)
                this.portaDireita = 1;
            else if (this.j == 4)
                this.portaEsquerda = 1;
            else if (this.j % 2 == 0)
                this.portaDireita = 1;
            else if (this.j % 2 != 0)
                this.portaEsquerda = 1;
        }

        //TIPO OCULTO(2) = 1 porta
        if (this.tipo == 2) {
            if (this.i == 0)
                this.portaBaixo = 1;
            else if (this.i == 4)
                this.portaCima = 1;
            else if (this.j == 0)
                this.portaDireita = 1;
            else if (this.j == 4)
                this.portaEsquerda = 1;
            else
                this.portaCima = 1;
        }
    }

}