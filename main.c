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
void printMap(int map[ROW][COLUMN]){

    for(int aux1 = 0; aux1 < 3; aux1 ++){

        for(int aux2 = 0; aux2 < 3; aux2 ++){

            printf("%d ", map[aux1][aux2]);

        }

        printf("\n");

    }

}

// Função para aleatorizar a lista / decidir a ordem dos itens
gint compareFunction(gconstpointer num1, gconstpointer numb, gpointer data){

    return((g_random_int_range(0, 2) * 2) - 1);

} 

int main(){

    g_random_set_seed((guint32) time(NULL));

    int input1 = 0;

    struct boards objective = {{{0, 1, 2,}, {3, 4, 5}, {6, 7, 8}}, 0};

    printf("Objetivo:\n");
    printMap(objective.map);
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
                    g_list_foreach(possibleNumbers, printList, NULL);
                    printf("\n");

                    aux ++;

                }

                //g_list_free(returnList); bug de liberação de memória 

            }
            
        }else if(input1 == 1){

            chosenOrder = g_list_copy(possibleNumbers);
            chosenOrder = g_list_sort_with_data(chosenOrder, compareFunction, NULL);

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

    printMap(board.map);

    // criar um hashmap ou um array?

  return 0;
  
}