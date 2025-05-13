#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gtk-4.0/gtk/gtk.h>
#include <glib-2.0/glib.h>

#define ROW 3
#define COLUMN 3 

GList *Visited_Boards = NULL;
GList *Possible_Boards = NULL;
GList *Best_Path = NULL;

// Struct dos tabuleiros e sua respectiva distância de Manhattan
struct boards{

    int map[ROW][COLUMN];
    // h(n)
    int distanceManhattan;
    // g(n)
    int realCost;
    // f(n) = g(n) + h(n)
    int fn;
    struct boards *parent;

};

// Função para imprir o tabuleiro
void PrintMap(int map[ROW][COLUMN]){

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            printf("%d ", map[aux1][aux2]);

        }

        printf("\n");

    }

}

// Função para imprimir a lista
void PrintList(gpointer list, gpointer data){

    printf("%d, ", GPOINTER_TO_INT(list));

}

void PrintPossibleBoards(gpointer possibleBoard, gpointer data){

    struct boards *board = (struct boards *)possibleBoard;

    PrintMap(board->map);
    printf("\n");

}

// Função para aleatorizar a lista / decidir a ordem dos items
gint CompareFunction(gconstpointer num1, gconstpointer numb2, gpointer data){

    return((g_random_int_range(0, 2) * 2) - 1);

} 

// Função para encontrar a posição x y de uma peça no tabuleiro
void ReturnPosition(int part, int board[ROW][COLUMN], int xy[2]){

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            if(board[aux1][aux2] == part){

                xy[0] = aux2;
                xy[1] = aux1;

                return;

            }

        }

    }

}

// Função para setar a distância de Manhattan dos tabuleiros
int CalcDistanceManhattan(int board[ROW][COLUMN], int objective[ROW][COLUMN]){

    // DM = |x1 - x2| + |y1 - y2|
    // x1y1 é do board

    int distanceManhattan = 0;

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            int xy[2] = {-1, -1};

            ReturnPosition(board[aux1][aux2], objective, xy);
        
            // Seria bom fazer alguma verificação de erro aqui, esses + 1 é para excluir as posições com 0 da jogada
            distanceManhattan =  distanceManhattan + (abs(aux2 - xy[0]) + abs(aux1 - xy[1]));

            //printf("DM: %d\n", distanceManhattan);
 
        }

    }

    return distanceManhattan;

}

// Função para calcula o custo real do tabuleiro inicial
int RealCost(struct boards *current, int next[ROW][COLUMN]){

    int realCost = current->realCost + CalcDistanceManhattan(current->map, next);    
    return realCost;

}

// Função para copiar uma matriz
void CopyArray(int original[ROW][COLUMN], int copy[ROW][COLUMN]){

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            copy[aux1][aux2] = original[aux1][aux2];

        }

    }

} 

// Função para trocar as peças de lugar
void PartsSwap(int board[ROW][COLUMN], int xyPart1[2], int xyPart2[2]){

    // Acho que eu vou ter que receber o endereço de board
    int aux = board[xyPart1[1]][xyPart1[0]];

    board[xyPart1[1]][xyPart1[0]] = board[xyPart2[1]][xyPart2[0]];
    board[xyPart2[1]][xyPart2[0]] = aux;

}

// Função para ordenar a Possible_Boards de forma crescente conforme o f(n)
gint ListOrdering(gconstpointer board1, gconstpointer board2){

    const struct boards *b1 = board1;
    const struct boards *b2 = board2;

    return b1->fn - b2->fn;

}

// Função para comparar dua matrizes
bool MatrixComparison(int board1[ROW][COLUMN], int board2[ROW][COLUMN]){

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            if(board1[aux1][aux2] == board2[aux1][aux2]){

                continue;

            }else {

                return false;

            }

        }

    }

    return true;

}

// Função para verificar se aquele tabuleiro já foi visitado
bool ExistsBoard(int board1[ROW][COLUMN], GList *listCompare){

    for(GList *list = listCompare; list != NULL; list = list->next){

        struct boards *board2 = (struct boards *) list->data;

        if(MatrixComparison(board1, board2->map)){

            return true;

        }

    }

    return false;

}

// Função para criar os possíveis tabuleiros a parti de um tabuleiro
void PossibleBoards(struct boards *board, int objective[ROW][COLUMN]){

    // Posição da peça vazia no tabuleiro
    int xyEmptyPart[2];

    ReturnPosition(0, board->map, xyEmptyPart);
    
    if(xyEmptyPart[0] > 0){

        struct boards *possibleBoard = malloc(sizeof(struct boards));
        possibleBoard->parent = board; 
        int xy[2] = {xyEmptyPart[0] - 1, xyEmptyPart[1]};

        CopyArray(board->map, possibleBoard->map);
        PartsSwap(possibleBoard->map, xyEmptyPart, xy);

        if(!ExistsBoard(possibleBoard->map, Visited_Boards)){

            if(!ExistsBoard(possibleBoard->map, Possible_Boards)){

                possibleBoard->distanceManhattan = CalcDistanceManhattan(possibleBoard->map, objective);
                possibleBoard->realCost = board->realCost + 1;
                possibleBoard->fn = possibleBoard->realCost + possibleBoard->distanceManhattan;

                Possible_Boards = g_list_append(Possible_Boards, possibleBoard);

            }

        }

    }

    if(xyEmptyPart[0] < 2){

        struct boards *possibleBoard = malloc(sizeof(struct boards));
        possibleBoard->parent = board; 
        int xy[2] = {xyEmptyPart[0] + 1, xyEmptyPart[1]};

        CopyArray(board->map, possibleBoard->map);
        PartsSwap(possibleBoard->map, xyEmptyPart, xy);

        if(!ExistsBoard(possibleBoard->map, Visited_Boards)){

            if(!ExistsBoard(possibleBoard->map, Possible_Boards)){

                possibleBoard->distanceManhattan = CalcDistanceManhattan(possibleBoard->map, objective);
                possibleBoard->realCost = board->realCost + 1;
                possibleBoard->fn = possibleBoard->realCost + possibleBoard->distanceManhattan;

                Possible_Boards = g_list_append(Possible_Boards, possibleBoard);

            }

        }

    }

    if(xyEmptyPart[1] > 0){

        struct boards *possibleBoard = malloc(sizeof(struct boards));
        possibleBoard->parent = board; 
        int xy[2] = {xyEmptyPart[0], xyEmptyPart[1] - 1};

        CopyArray(board->map, possibleBoard->map);
        PartsSwap(possibleBoard->map, xyEmptyPart, xy);

        if(!ExistsBoard(possibleBoard->map, Visited_Boards)){

            if(!ExistsBoard(possibleBoard->map, Possible_Boards)){

                possibleBoard->distanceManhattan = CalcDistanceManhattan(possibleBoard->map, objective);
                possibleBoard->realCost = board->realCost + 1;
                possibleBoard->fn = possibleBoard->realCost + possibleBoard->distanceManhattan;

                Possible_Boards = g_list_append(Possible_Boards, possibleBoard);

            }

        }

    }

    if(xyEmptyPart[1] < 2){

        struct boards *possibleBoard = malloc(sizeof(struct boards));
        possibleBoard->parent = board; 
        int xy[2] = {xyEmptyPart[0], xyEmptyPart[1] + 1};

        CopyArray(board->map, possibleBoard->map);
        PartsSwap(possibleBoard->map, xyEmptyPart, xy);

        if(!ExistsBoard(possibleBoard->map, Visited_Boards)){

            if(!ExistsBoard(possibleBoard->map, Possible_Boards)){

                possibleBoard->distanceManhattan = CalcDistanceManhattan(possibleBoard->map, objective);
                possibleBoard->realCost = board->realCost + 1;
                possibleBoard->fn = possibleBoard->realCost + possibleBoard->distanceManhattan;

                Possible_Boards = g_list_append(Possible_Boards, possibleBoard);

            }

        }

    }

}

// Função para fazer a busca A*
struct boards *AStar(int objective[ROW][COLUMN], int iterationLimit){

    int aux = 0;

    while(aux < iterationLimit){

        // Ordenando a Possible_Boards de forma crescente
        Possible_Boards = g_list_sort(Possible_Boards, ListOrdering);

        // Olhando o primeiro tabuleiro que está em Possible_Boards, esse sendo o tabuleiro com menor f(n)
        struct boards *board = (struct boards *) Possible_Boards->data;

        if(MatrixComparison(board->map, objective)){

            printf("São iguais, AH É\n");

            return board;

        }

        // Removendo o primeiro tabuleiro que está em Possible_Boards, esse sendo o tabuleiro com menor f(n)
        Possible_Boards = g_list_delete_link(Possible_Boards, Possible_Boards);
        
        // Adcionando o tabuleiro na lista Visited_Boards
        Visited_Boards = g_list_append(Visited_Boards, board);

        PossibleBoards(board, objective);

        aux++;

    }

    return NULL;

}

// Função para fazer o caminho entre o tabuleiro objetivo e o inicial 
void Path(struct boards *finalBoard){

    for(struct boards *board = finalBoard; board != NULL; board = board->parent){

        Best_Path = g_list_append(Best_Path, board);

    }

    Best_Path = g_list_reverse(Best_Path);

}

gboolean UpdateWindow(gpointer data){

    GtkWidget *grid = GTK_WIDGET(data);

    if(Best_Path == NULL){

        return G_SOURCE_REMOVE;

    }

    // Removendo o primeiro item da lista Best_Path
    struct boards *board = (struct boards *) Best_Path->data;
    Best_Path = g_list_delete_link(Best_Path, Best_Path);

    GtkWidget *child = gtk_widget_get_first_child(grid);

    while(child != NULL){

        GtkWidget *next = gtk_widget_get_next_sibling(child);
        gtk_grid_remove(GTK_GRID(grid), child);
        child = next;

    }

    // Carregando as imagens e colocando na grid
    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            char path[50];
            int part = board->map[aux1][aux2];
            snprintf(path, sizeof(path), "src/images/%d.png", part);
            GtkWidget *image = gtk_picture_new_for_filename(path);
            gtk_grid_attach(GTK_GRID(grid), image, aux2, aux1, 1, 1);
            gtk_widget_set_visible(image, TRUE);

        }

    }

    return G_SOURCE_CONTINUE;

}

// Janela da resolução do puzzle
void PuzzleWindow(GtkApplication *app, gpointer data){

    // Criando a janela
    GtkWidget *puzzleWindow = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(puzzleWindow), "8 Puzzle Solver");
    gtk_window_set_default_size(GTK_WINDOW(puzzleWindow), 480, 480);
    gtk_window_set_resizable(GTK_WINDOW(puzzleWindow), FALSE);

    g_signal_connect(puzzleWindow, "destroy", G_CALLBACK(g_application_quit), NULL);

    // Criando o grid onde as imagens vão ficar
    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
    gtk_widget_set_margin_top(grid, 4);
    gtk_widget_set_margin_start(grid, 4);
    gtk_widget_set_margin_end(grid, 4);
    gtk_widget_set_margin_bottom(grid, 4);

    
    gtk_window_set_child(GTK_WINDOW(puzzleWindow), grid);
    gtk_window_present(GTK_WINDOW(puzzleWindow));

    g_timeout_add(1000, UpdateWindow, grid);
    //g_object_unref(app);


}

int main(){

    g_random_set_seed((guint32) time(NULL));

    int input1 = 0;

    struct boards objective = {{{0, 1, 2,}, {3, 4, 5}, {6, 7, 8}}, 0, 0, 0, NULL};
    
    // Tabuleiro inicial
    struct boards *board = malloc(sizeof(struct boards));

    GList *possibleNumbers = NULL;
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(0));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(1));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(2));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(3));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(4));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(5));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(6));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(7));
    possibleNumbers = g_list_append(possibleNumbers, GINT_TO_POINTER(8));

    GList *chosenOrder = NULL;
    
    // Criação do tabuleiro
    while(input1 < 1 || input1 > 2) {

        printf("\n\nEscolha uma opção abaixo\n\n");

        printf("1 -> Aleatório\n2 -> Manual\n");

        scanf("%d", &input1);
      
        while(getchar() != '\n'){};

        // Criação do mapa pelo usuário
        if(input1 == 2){

            for(int aux = 0; aux <= 8; ){

                int input2 = 0;
                int findInList;
                GList *returnList = NULL;

                printf("\nSelecione um número para ficar na posição: %d\n", aux + 1);

                scanf("%d", &input2);
      
                while(getchar() != '\n'){};

                if((returnList = g_list_find(possibleNumbers, GINT_TO_POINTER(input2)))){

                    findInList = GPOINTER_TO_INT(returnList->data);
                    possibleNumbers = g_list_remove(possibleNumbers, GINT_TO_POINTER(input2));
                    chosenOrder = g_list_append(chosenOrder, GINT_TO_POINTER(findInList));

                    printf("Voçê ainda pode escolher os números:\n");
                    g_list_foreach(possibleNumbers, PrintList, NULL);
                    printf("\n");

                    aux ++;

                }

                //g_list_free(returnList); bug de liberação de memória 

            }
            
        }else if(input1 == 1){

            chosenOrder = g_list_copy(possibleNumbers);
            chosenOrder = g_list_sort_with_data(chosenOrder, CompareFunction, NULL);

            g_list_free(possibleNumbers);

        }
    };

    // Convertendo a lista para o tabuleiro
    board->parent = NULL;

    GList *copyChosenOrder = g_list_copy(chosenOrder);

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            board->map[aux1][aux2] = GPOINTER_TO_INT(g_list_first(copyChosenOrder)->data);
            copyChosenOrder = g_list_remove(copyChosenOrder, GINT_TO_POINTER(board->map[aux1][aux2]));

        }

    }

    g_list_free(copyChosenOrder);

    PrintMap(board->map);

    // Calculando a distância Manhattann g(n) e f(n) do tabuleiro criado
    board->realCost = 0;
    board->distanceManhattan = CalcDistanceManhattan(board->map, objective.map);
    board->fn = board->realCost + board->distanceManhattan;

    printf("A distância de manhattan desse tabuleiro é: %d\n", board->distanceManhattan);

    Possible_Boards = g_list_append(Possible_Boards, board);

    struct boards *finalBoard = AStar(objective.map, 5000);

    if(finalBoard != NULL){

        PrintMap(finalBoard->map);

        Path(finalBoard);

        printf("\n Número de passos: %d", g_list_length(Best_Path));

        GtkApplication *app = gtk_application_new("com.puzzle", G_APPLICATION_DEFAULT_FLAGS);
        g_signal_connect(app, "activate", G_CALLBACK(PuzzleWindow), NULL);
        int status = g_application_run(G_APPLICATION(app), 0, NULL);
        g_object_unref(app);


    }else {

        printf("Solução não encontra\n");

    }
    
  return 0;
  
}