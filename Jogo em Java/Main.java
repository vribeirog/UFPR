import java.util.Random;
import java.util.*;

// Victor Ribeiro Garcia GRR20203954
// Isadora Botassari de Souza GRR20206872
// Lucas Araujo GRR20206150

public class Main {
    public static void main(String[] args) {

        /* jogadores */
        JogadorNormal p1 = new JogadorNormal();
        JogadorSuporte p2 = new JogadorSuporte();

        /* criação de setores */
        Setor[][] setores = new Setor[5][5];
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                ArrayList<Inimigo> inimigos = new ArrayList<Inimigo>();
                Setor s = new Setor(inimigos);
                setores[i][j] = s;
                setores[i][j].setPosicao(i, j);
                setores[i][j].setTipo(i, j);
                setores[i][j].gerarPortas();
                setores[i][j].gerarInimigos(inimigos, i, j);
            }
        }

        int turnos = 1;

        // Gerando a posição da fonte
        Random aleat = new Random();
        int x = 2;
        int y = 2;
        while (x == 2 && y == 2) {
            x = aleat.nextInt(5);
            y = aleat.nextInt(5);
        }
        Fonte fonte = new Fonte(x, y);

        Scanner sc = new Scanner(System.in);
        Tabuleiro t = new Tabuleiro();
        char str;
        int inputInvalido;

        /* jogo dura até 25 turnos ou até a morte de P1 ou até a fonte ser encontrada */
        while (turnos <= 25 && p1.status == true && fonte.status == true) {

            for (int i = 0; i < 2; i++) {
                // turno jogador 1
                if (p1.status == true) {
                    t.imprimirTabuleiro(p1, p2, fonte, setores);
                    t.imprimirSetor(p1, p2, setores, p1.x, p1.y);

                    System.out.println("turno: " + turnos);
                    t.imprimirMensagens(p1, setores[p1.x][p1.y]);
                    str = sc.next().charAt(0);
                    inputInvalido = t.trataInputJogador(str, p1, p2, setores[p1.x][p1.y]);

                    System.out.print("\033[H\033[2J");
                    System.out.flush();
                    if (inputInvalido == 1)
                        System.out.println(Cores.ANSI_RED + "Input inválido, Jogador 1. Você perdeu sua jogada."
                                + Cores.ANSI_RESET);

                    fonte.fonteEncontrada(p1);
                    if (fonte.status == false)
                        break;
                } else
                    break;

                // turno jogador 2
                if (p2.status == true) {
                    t.imprimirTabuleiro(p1, p2, fonte, setores);
                    t.imprimirSetor(p1, p2, setores, p2.x, p2.y);

                    System.out.println("Turno: " + turnos);
                    t.imprimirMensagens(p2, setores[p2.x][p2.y]);
                    str = sc.next().charAt(0);
                    inputInvalido = t.trataInputJogador(str, p2, p1, setores[p2.x][p2.y]);

                    System.out.print("\033[H\033[2J");
                    System.out.flush();
                    if (inputInvalido == 1) {
                        System.out.println(Cores.ANSI_RED + "Input inválido, Jogador 2. Você perdeu sua jogada."
                                + Cores.ANSI_RESET);
                    }
                    fonte.fonteEncontrada(p2);
                    if (fonte.status == false)
                        break;
                }
            }

            // ataques inimigos
            for (Inimigo enemy1 : setores[p1.x][p1.y].inimigos) {
                enemy1.ataqueInimigo(enemy1, p1);
            }
            for (Inimigo enemy2 : setores[p2.x][p2.y].inimigos) {
                enemy2.ataqueInimigo(enemy2, p2);
            }

            turnos++;
            System.out.print("\033[H\033[2J");
            System.out.flush();
        }

        if (fonte.status == false) {
            System.out.println(Cores.ANSI_GREEN + "-*-*-*-*-*-*-*-*-*-*-*-*-*-" + Cores.ANSI_RESET);
            System.out.println(Cores.ANSI_GREEN + "| A Fonte foi encontrada! |" + Cores.ANSI_RESET);
            System.out.println(Cores.ANSI_GREEN + "-*-*-*-*-*-*-*-*-*-*-*-*-*-" + Cores.ANSI_RESET);
        } else if (turnos > 25) {
            System.out.println(Cores.ANSI_RED + "Derrota! Seus turnos estouraram!" + Cores.ANSI_RESET);
        } else if (p1.status == false)
            System.out.println(Cores.ANSI_RED + "Derrota! Jogador1 morreu!" + Cores.ANSI_RESET);

        sc.close();
    }
}