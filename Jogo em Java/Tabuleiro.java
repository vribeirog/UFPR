import java.util.*;

class Tabuleiro {

    /* imprime o tabuleiro do jogo */
    public void imprimirTabuleiro(JogadorNormal p1, JogadorSuporte p2, Fonte fonte, Setor[][] setores) {
        String c1, c2, c3, c4, c5, c6;
        System.out.printf("----------------------------\n");
        System.out.printf("|   Antivírus por um dia   |\n");
        System.out.printf("----------------------------\n");
        System.out.printf("      1   2   3   4   5\n");
        for (int i = 0; i < 5; i++) {
            c1 = "-";
            c2 = "-";
            c3 = "-";
            c4 = "-";
            c5 = "-";
            if (i > 0 && ((((p1.x == i && p1.y == 0) || (p2.x == i && p2.y == 0) || (setores[i][0].visitado == true))
                    && setores[i][0].portaCima == 1)
                    || (((p1.x == i - 1 && p1.y == 0) || (p2.x == i - 1 && p2.y == 0)
                            || (setores[i - 1][0].visitado == true))
                            && setores[i - 1][0].portaBaixo == 1)))
                c1 = "*";
            if (i > 0 && ((((p1.x == i && p1.y == 1) || (p2.x == i && p2.y == 1) || (setores[i][1].visitado == true))
                    && setores[i][1].portaCima == 1)
                    || (((p1.x == i - 1 && p1.y == 1) || (p2.x == i - 1 && p2.y == 1)
                            || (setores[i - 1][1].visitado == true))
                            && setores[i - 1][1].portaBaixo == 1)))
                c2 = "*";
            if (i > 0 && ((((p1.x == i && p1.y == 2) || (p2.x == i && p2.y == 2) || (setores[i][2].visitado == true))
                    && setores[i][2].portaCima == 1)
                    || (((p1.x == i - 1 && p1.y == 2) || (p2.x == i - 1 && p2.y == 2)
                            || (setores[i - 1][2].visitado == true))
                            && setores[i - 1][2].portaBaixo == 1)))
                c3 = "*";
            if (i > 0 && ((((p1.x == i && p1.y == 3) || (p2.x == i && p2.y == 3) || (setores[i][3].visitado == true))
                    && setores[i][3].portaCima == 1)
                    || (((p1.x == i - 1 && p1.y == 3) || (p2.x == i - 1 && p2.y == 3)
                            || (setores[i - 1][3].visitado == true))
                            && setores[i - 1][3].portaBaixo == 1)))
                c4 = "*";
            if (i > 0 && ((((p1.x == i && p1.y == 4) || (p2.x == i && p2.y == 4) || (setores[i][4].visitado == true))
                    && setores[i][4].portaCima == 1)
                    || (((p1.x == i - 1 && p1.y == 4) || (p2.x == i - 1 && p2.y == 4)
                            || (setores[i - 1][4].visitado == true))
                            && setores[i - 1][4].portaBaixo == 1)))
                c5 = "*";
            System.out.printf("    |-%s-|-%s-|-%s-|-%s-|-%s-|\n", c1, c2, c3, c4, c5);
            System.out.printf("%d   |", i + 1);
            for (int j = 0; j < 5; j++) {
                c6 = "|";
                if ((j < 5 && ((p1.x == i && p1.y == j) || (p2.x == i && p2.y == j) || (setores[i][j].visitado == true))
                        && setores[i][j].portaDireita == 1)
                        || (j < 4
                                && ((p1.x == i && p1.y == j + 1) || (p2.x == i && p2.y == j + 1)
                                        || (setores[i][j + 1].visitado == true))
                                && setores[i][j + 1].portaEsquerda == 1))
                    c6 = "*";
                if (j == 2 && i == 2)
                    System.out.printf(" C %s", c6);
                else if ((p1.status == true && p1.x == i && p1.y == j) && (p2.status == true && p2.x == i && p2.y == j))
                    System.out.printf(" P %s", c6);
                else if (p1.status == true && p1.x == i && p1.y == j)
                    System.out.printf("P1 %s", c6);
                else if (p2.status == true && p2.x == i && p2.y == j)
                    System.out.printf("P2 %s", c6);
                else if (fonte.status == true && fonte.x == i && fonte.y == j)
                    System.out.printf(" X %s", c6);
                else
                    System.out.printf("   %s", c6);
            }
            System.out.printf("\n");
        }
        System.out.printf("    |---|---|---|---|---|\n");
    }

    /* imprime o setor onde o jogador da vez está */
    public void imprimirSetor(JogadorNormal p1, JogadorSuporte p2, Setor[][] s, int x, int y) {

        System.out.println(" ");
        System.out.println("Setor [" + s[x][y].i + ", " + s[x][y].j + "]");

        /* verifica se há porta superior; se sim, imprime */
        if (s[x][y].portaCima == 1)
            System.out.println("|------*------|");
        else
            System.out.println("|-------------|");

        for (Inimigo enemy : s[x][y].inimigos) {
            System.out.println("|  " + enemy.atk + "/" + enemy.def + "        | ");
        }

        for (int i = 0; i < 5; i++) {
            System.out.print("|");
            for (int j = 0; j < 5; j++)
                ;
            System.out.print("             ");
            System.out.println("|");
        }

        /* imprime os players que estão nesse setor */
        if (p1.x == x && p1.y == y && p1.status == true)
            System.out.printf("| P1:%2d /%2d   |\n", p1.atk, p1.def);
        else if (p1.status == false)
            System.out.println("|  P1: Morto  |");
        if (p2.x == x && p2.y == y && p2.status == true)
            System.out.printf("| P2:%2d /%2d   |\n", p2.atk, p2.def);
        else if (p2.status == false)
            System.out.println("|  P2: Morto  |");

        /* verifica se há porta inferior; se sim, imprime */
        if (s[x][y].portaBaixo == 1)
            System.out.println("|------*------|");
        else
            System.out.println("|-------------|");
    }

    /*
     * trata o input do jogador e realiza diferentes ações de acordo com esse input
     retorna 0 se o input foi válido, retorna 1 se o input foi, de alguma forma, inválido
     */
    public int trataInputJogador(char input, Jogador jogadorTurno, Jogador jogadorCura, Setor s) {

        Scanner sc = new Scanner(System.in);
        char str;
        int i;

        /* opção de restaurar def exclusiva de JogadorSuporte */
        if (input == 'h' && jogadorTurno instanceof JogadorSuporte) {
            JogadorSuporte aux = (JogadorSuporte) jogadorTurno;
            System.out.println("Quem deseja curar?");
            
            if ((jogadorTurno.x == jogadorCura.x) && (jogadorTurno.y == jogadorCura.y))
                System.out.println("1 - Jogador 1 || 2 - Jogador 2");
            else
                System.out.println("2 - Jogador 2");
            str = sc.next().charAt(0);

            if (str != '1' && str != '2')
                return 1;
            else {
                // só pode curar quando o alvo da cura estiver no mesmo setor
                if (str == '1' && (jogadorCura.x == jogadorTurno.x) && (jogadorCura.y == jogadorTurno.y))
                    aux.curarAliado(jogadorCura);
                if (str == '2')
                    aux.curarAliado(jogadorTurno);
                return 0;
            }

        }

        /* realiza ataques os jogadores */
        if (input == 'a') {
            System.out.println("Quem deseja atacar? Digite o numero do inimigo.");
            i = 1;
            for (Inimigo enemy : s.inimigos) {
                System.out.print(i + "- " + enemy.atk + "/" + enemy.def + "  ||  ");
                i++;
            }
            int num = sc.nextInt();
            num = num - 1;
            /* o jogador do turno ataca o inimigo escolhido */
            if (num <= s.inimigos.size()) {
                jogadorTurno.ataque(jogadorTurno, s.inimigos.get(num), s);
                return 0;
            } else
                return 1;
        }

        /* referente a opções de movimento e de procurar no setor */
        switch (input) {
            case 'c':
                jogadorTurno.movimentaCima(jogadorTurno, s);
                return 0;

            case 'b':
                jogadorTurno.movimentaBaixo(jogadorTurno, s);
                return 0;

            case 'e':
                jogadorTurno.movimentaEsq(jogadorTurno, s);
                return 0;

            case 'd':
                jogadorTurno.movimentaDir(jogadorTurno, s);
                return 0;

            case 'p':
                if (s.tipo != 1)
                    jogadorTurno.procurar(jogadorTurno, s);
                return 0;
        }
        return 1;

    }

    /* imprime mensagens que comunicam informações aos jogadores */
    /* verifica diversas situações do jogo para imprimir ou não uma informação */
    public void imprimirMensagens(Jogador j, Setor s) {

        if (j instanceof JogadorNormal)
            System.out.println("Jogador 1, o que deseja fazer?");
        else
            System.out.println("Jogador 2, o que deseja fazer?");

        if (s.inimigos.isEmpty() == true) {
            if (s.portaCima == 1)
                System.out.println("c - Andar para cima");
            if (s.portaBaixo == 1)
                System.out.println("b - Andar para baixo");
            if (s.portaEsquerda == 1)
                System.out.println("e - Andar para a esquerda");
            if (s.portaDireita == 1)
                System.out.println("d - Andar para a direita");
        }
        if (s.inimigos.isEmpty() == false)
            System.out.println("a - Atacar inimigos");
        if (s.tipo != 1)
            System.out.println("p - Procurar");
        if (j instanceof JogadorSuporte)
            System.out.println("h - Curar");
        System.out.print("Opção: ");
    }
}