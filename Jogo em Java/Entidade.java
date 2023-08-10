import java.util.Random;
import java.util.*;

/* 
ID FONTE INFECCAO = -1
ID INIMIGO = 0
ID JOGADOR NORMAL = 1
ID JOGADOR SUP = 2 
*/

abstract class Entidade {

    // atributos
    protected int id;
    protected int atk;
    protected int def;
    protected int x;
    protected int y;
    protected boolean status;

    // construtor
    public Entidade() {
    };

    /* métodos get/set */
    public int getId() {
        return this.id;
    }

    public void setId(int id) {
        if(id >= -1 && id <= 2)
            this.id = id;
    }

    public int getAtk() {
        return this.atk;
    }

    public void setAtk(int atk) {
            this.atk = atk;
    }

    public int getDef() {
        return this.def;
    }

    public void setDef(int def) {
        this.def = def;
    }

    public int getX() {
        return this.x;
    }

    public void setX(int x) {
        if(x >= 0 && x < 5)
            this.x = x;
    }

    public int getY() {
        return this.y;
    }

    public void setY(int y) {
        if(y >= 0 && y < 5)
            this.y = y;
    }

    public boolean getStatus() {
        return this.status;
    }

    public void setStatus(boolean status) {
        this.status = status;
    }

    /* verifica se uma entidade está viva; se não está, muda o status */
    public void estaVivo(Entidade e, Setor s) {
        if(e.def <= 0) {
            e.status = false;
            if (e instanceof Inimigo)
                s.inimigos.remove(e);
        }
    }

    /* método de ataque que serve tanto para jogadores quanto para inimigos */
    public void ataque(Entidade atacante, Entidade alvo, Setor s) {
        if(s.tipo == 2){
            Random aleat = new Random();
            int num;
            num = aleat.nextInt(2);
            if(num == 1){
                alvo.def = alvo.def - atacante.atk;
                alvo.estaVivo(alvo, s);
            }
        }else{
            alvo.def = alvo.def - atacante.atk;
            alvo.estaVivo(alvo, s);
        }
    }

}

abstract class Jogador extends Entidade {

    /* construtor */
    public Jogador() {
    };

    /* método pra movimentar o jogador para o setor acima */
    public void movimentaCima(Jogador j, Setor s) {
        if ( ( (j.x-1) >= 0) && s.portaCima == 1 && s.inimigos.isEmpty() == true){
            j.x--;
            s.visitado = true;
        }else
            System.out.println("Movimento inválido!");
    }

    /* método pra movimentar o jogador para o setor abaixo */
    public void movimentaBaixo(Jogador j, Setor s) {
        if ( ( (j.x+1) <= 4) && s.portaBaixo == 1 && s.inimigos.isEmpty() == true){
            j.x++;
            s.visitado = true;
        }else
            System.out.println("Movimento inválido!");
    }

    /* método pra movimentar o jogador para o setor à esquerda */
    public void movimentaEsq(Jogador j, Setor s) {
        if ( ( (j.y-1) >= 0) && s.portaEsquerda == 1 && s.inimigos.isEmpty() == true){
            j.y--;
            s.visitado = true;
        }else
            System.out.println("Movimento inválido!");
    }

    /* método pra movimentar o jogador para o setor à direita */
    public void movimentaDir(Jogador j, Setor s) {
        if ( ( (j.y+1) <= 4) && s.portaDireita == 1 && s.inimigos.isEmpty() == true){
            j.y++;
            s.visitado = true;
        }else
            System.out.println("Movimento inválido!");
    }

    /* método que implementa a mecânica de procurar no jogo */
    public void procurar(Jogador j, Setor s) {
        Random rand = new Random();
        int num;

        num =(rand.nextInt(6)) + 1;

        switch (num) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                j.def = j.def + 1;
                break;
            case 5:
                j.def = j.def + 2;
                break;
            case 6:
                Iterator<Inimigo> i = s.inimigos.iterator();
                while (i.hasNext()) {
                    Inimigo enemy = i.next();

                    enemy.def = enemy.def - 1;
                    enemy.estaVivo(enemy, s);
                }
                break;

        }

    }

}

class JogadorNormal extends Jogador {

    /* get/set herdados de Entidade */

    /* construtor */
    public JogadorNormal() {
        this.id = 1;
        this.atk = 2;
        this.def = 6;
        this.x = 2;
        this.y = 2;
        this.status = true;
    }

}

class JogadorSuporte extends Jogador {

    /* get/set herdados de Entidade */

    /* construtor */
    public JogadorSuporte() {
        this.id = 2;
        this.atk = 1;
        this.def = 7;
        this.x = 2;
        this.y = 2;
        this.status = true;
    }

    /* método para o aumento de DEF de um jogador */
    void curarAliado(Jogador aliado) {
            aliado.def = aliado.def + 2;
    }

}

class Inimigo extends Entidade {

    /* construtor */
    public Inimigo(int x, int y) {
        Random rand = new Random();
        int num;
        num = (rand.nextInt(3) + 1);

        this.id = 0;
        this.atk = num;
        this.def = num;
        this.x = x;
        this.y = y;
        this.status = true;
    }

    /* função de ataque para inimigos */
    public void ataqueInimigo(Inimigo inimigo, Jogador alvo) {
        Random rand = new Random();
        int num;
        num = (rand.nextInt(5) + 1);

        if(num % 2 == 0) {
            alvo.def = alvo.def - inimigo.atk;
            if(alvo.def <= 0)
                alvo.status = false;
        } else
            return;
    }

}

class Fonte extends Entidade{
    
    /* construtor */
    public Fonte(int x, int y) {
        this.id = -1;
        this.atk = 0;
        this.def = 1;
        this.x = x;
        this.y = y;
        this.status = true;
    }

    /* muda o status da fonte caso seja identificada pelos jogadores */
    public void fonteEncontrada(Jogador p) {
        if (this.x == p.x && this.y == p.y)
            this.status = false;
    }
}