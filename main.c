#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glib-2.0/glib.h>

#define ROW 3
#define COLUMN 3 

// Struct dos tabuleiros e sua respectiva distância de Manhattan
struct boards{

    int map[ROW][COLUMN];
    int distanceManhattan;

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

// Função para aleatorizar a lista / decidir a ordem dos items
gint CompareFunction(gconstpointer num1, gconstpointer numb, gpointer data){

    return((g_random_int_range(0, 2) * 2) - 1);

} 

/*
// Função para encontrar a posição referente a peça que está em x1y1 na matriz objective
void ReturnX2Y2(int part, int objective[ROW][COLUMN], int *xy[2]){

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            if(objective[aux1][aux2] == part){

                *xy[0] = aux2;
                *xy[1] = aux1;

                return;

            }

        }

    }

}
*/

// Função para setar a distância de Manhattan dos tabuleiros
void CalcDistanceManhattan(struct boards *board, int objective[ROW][COLUMN]){

    // DM = |x1 - x2| + |y1 - y2|
    // x1y1 é do board

    int distanceManhattan = 0;

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            int xy[2] = {-1, -1};

            for(int aux3 = 0; aux3 < 3; aux3 ++){

                for(int aux4 = 0; aux4 < 3; aux4 ++){
        
                    if(board->map[aux1][aux2] == objective[aux3][aux4]){
        
                        xy[0] = aux4;
                        xy[1] = aux3;
                        
                        // Esse break só vai parar esse for o outro vai continuar
                        break;
        
                    }
        
                }
        
            }

            // Seria bom fazer alguma verificação de erro aqui, esses + 1 é para excluir as posições com 0 da jogada
            distanceManhattan =  distanceManhattan + (abs(aux2 - xy[0]) + abs(aux1 - xy[1]));

            //printf("DM: %d\n", distanceManhattan);
 
        }

    }

    board->distanceManhattan = distanceManhattan;

}

int main(){

    g_random_set_seed((guint32) time(NULL));

    int input1 = 0;

    struct boards objective = {{{0, 1, 2,}, {3, 4, 5}, {6, 7, 8}}, 0};

    printf("Objetivo:\n");
    PrintMap(objective.map);
    printf("\n");

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
    struct boards board;

    GList *copyChosenOrder = g_list_copy(chosenOrder);

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            board.map[aux1][aux2] = GPOINTER_TO_INT(g_list_first(copyChosenOrder)->data);
            copyChosenOrder = g_list_remove(copyChosenOrder, GINT_TO_POINTER(board.map[aux1][aux2]));

        }

    }

    g_list_free(copyChosenOrder);

    PrintMap(board.map);

    // Calculando a distância Manhattan do tabuleiro criado
    CalcDistanceManhattan(&board, objective.map);

    printf("A distância manhattan desse tabuleiro é: %d\n", board.distanceManhattan);


    // criar um hashmap ou um array?


  return 0;
  
}